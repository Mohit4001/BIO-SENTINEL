#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define SDA_PIN 8
#define SCL_PIN 9
#define BUZZER_PIN 10
#define LED_LOW 11
#define LED_HIGH 12
#define LED_CRIT 13

const int POT_HR=1, POT_SPO2=2, POT_TEMP=4, POT_ACTIVITY=5, POT_HUM=6, POT_PM=7;
const int BTN_NORMAL=14, BTN_HEAT=15, BTN_POLLUTION=16, BTN_FALL=17, BTN_CRITICAL=18, BTN_OFFLINE=19;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

enum Scenario { NORMAL, HEAT, POLLUTION, FLOOD, CYCLONE, FALL, CRITICAL, OFFLINE };
Scenario scenario = NORMAL;
bool offline=false;
bool sos=false;
bool fallDetected=false;
unsigned long sosStarted=0;

struct SensorData { float hr, spo2, bodyTemp, activity, humidity, co2, pm25, accelMag; };
struct Features { float hrDev, spo2Dev, tempDev, heatStress, cardioStress, envStress, fatigue, activityNorm, baselineDev, fallScore; };
struct RiskResult { float score, prob; const char* level; const char* action; };
SensorData raw, filtered;
Features feat;
RiskResult risk;

float ema(float prev, float x, float a=0.25f){ return prev + a*(x-prev); }
float clamp01(float x){ return fmaxf(0.0f,fminf(1.0f,x)); }

const char* scenarioName(){
  switch(scenario){case NORMAL:return "NORMAL";case HEAT:return "HEAT WAVE";case POLLUTION:return "AIR POLLUTION";case FLOOD:return "FLOOD";case CYCLONE:return "CYCLONE";case FALL:return "FALL";case CRITICAL:return "CRITICAL";case OFFLINE:return "OFFLINE";} return "NORMAL";
}

void setScenario(Scenario s){
  scenario=s; fallDetected=(s==FALL); offline=(s==OFFLINE); if(s==CRITICAL){offline=false;} 
  Serial.printf("\n[SCENARIO] %s | SIMULATION\n", scenarioName());
  if(s==FALL){ sos=false; sosStarted=0; }
}

float mapf(float x,float in1,float in2,float out1,float out2){ return out1+(x-in1)*(out2-out1)/(in2-in1); }

void generateSensors(){
  float n=sin(millis()/900.0f)*0.7f;
  raw.hr=mapf(analogRead(POT_HR),0,4095,55,135)+n;
  raw.spo2=mapf(analogRead(POT_SPO2),0,4095,90,100);
  raw.bodyTemp=mapf(analogRead(POT_TEMP),0,4095,35.8,40.8);
  raw.activity=mapf(analogRead(POT_ACTIVITY),0,4095,0,1);
  raw.humidity=mapf(analogRead(POT_HUM),0,4095,20,95);
  raw.pm25=mapf(analogRead(POT_PM),0,4095,5,220);
  raw.co2=450+raw.humidity*8;
  raw.accelMag=1.0f+raw.activity*1.2f;

  if(scenario==NORMAL){raw.hr=72+n*0.3f;raw.spo2=98;raw.bodyTemp=36.7;raw.activity=0.35;raw.humidity=50;raw.pm25=12;}
  if(scenario==HEAT){raw.hr=104+n;raw.spo2=97;raw.bodyTemp=39.6;raw.activity=0.85;raw.humidity=78;raw.pm25=20;}
  if(scenario==POLLUTION){raw.hr=94+n;raw.spo2=93.5;raw.bodyTemp=37.1;raw.activity=0.55;raw.humidity=65;raw.pm25=175;}
  if(scenario==FLOOD){raw.hr=88+n;raw.spo2=97;raw.bodyTemp=37.2;raw.activity=0.25;raw.humidity=90;raw.pm25=45;}
  if(scenario==CYCLONE){raw.hr=91+n;raw.spo2=97;raw.bodyTemp=37.1;raw.activity=0.2;raw.humidity=92;raw.pm25=55;}
  if(scenario==FALL){raw.hr=76+n;raw.spo2=98;raw.bodyTemp=36.8;raw.activity=0.05;raw.accelMag=4.2;}
  if(scenario==CRITICAL){raw.hr=125+n;raw.spo2=90;raw.bodyTemp=40.2;raw.activity=0.85;raw.humidity=82;raw.pm25=180;}
}

void filterSignals(){
  static bool first=true;
  if(first){filtered=raw;first=false;return;}
  filtered.hr=ema(filtered.hr,raw.hr); filtered.spo2=ema(filtered.spo2,raw.spo2);
  filtered.bodyTemp=ema(filtered.bodyTemp,raw.bodyTemp); filtered.activity=ema(filtered.activity,raw.activity);
  filtered.humidity=ema(filtered.humidity,raw.humidity); filtered.pm25=ema(filtered.pm25,raw.pm25);
  filtered.co2=ema(filtered.co2,raw.co2); filtered.accelMag=ema(filtered.accelMag,raw.accelMag);
}

void extractFeatures(){
  feat.hrDev=clamp01(fabsf(filtered.hr-72)/45.0f);
  feat.spo2Dev=clamp01((98-filtered.spo2)/8.0f);
  feat.tempDev=clamp01(fabsf(filtered.bodyTemp-36.7)/3.5f);
  feat.activityNorm=clamp01(filtered.activity);
  feat.heatStress=clamp01(((filtered.bodyTemp-37.5)/3.0f)*0.55f + ((filtered.humidity-60)/40.0f)*0.25f + feat.activityNorm*0.20f);
  feat.cardioStress=clamp01(feat.hrDev*0.65f+feat.spo2Dev*0.35f);
  feat.envStress=clamp01((filtered.pm25/200.0f)*0.75f + ((filtered.co2-500)/2500.0f)*0.25f);
  feat.fatigue=clamp01(feat.hrDev*0.35f+feat.tempDev*0.25f+feat.activityNorm*0.40f);
  feat.baselineDev=clamp01(feat.hrDev*0.35f+feat.spo2Dev*0.30f+feat.tempDev*0.35f);
  feat.fallScore=(filtered.accelMag>3.0f && filtered.activity<0.25f)?1.0f:0.0f;
}

float tinyMLInference(){
  // Prototype TinyML-style interface: transparent weighted classifier, replaceable by TFLM/Edge Impulse later.
  float context=0;
  if(scenario==HEAT) context=0.20f;
  else if(scenario==POLLUTION) context=0.18f;
  else if(scenario==FLOOD||scenario==CYCLONE) context=0.08f;
  else if(scenario==CRITICAL) context=0.30f;
  float z=-2.4f + 1.65f*feat.cardioStress + 1.25f*feat.heatStress + 1.35f*feat.envStress + 1.1f*feat.baselineDev + 0.9f*feat.fatigue + context;
  if(feat.fallScore>0.5f) z+=1.8f;
  return 1.0f/(1.0f+expf(-z));
}

void decideRisk(){
  risk.prob=tinyMLInference(); risk.score=roundf(risk.prob*100.0f);
  if(feat.fallScore>0.5f){risk.score=fmaxf(risk.score,78);}
  if(risk.score<25){risk.level="LOW";risk.action="CONTINUE MONITORING";}
  else if(risk.score<50){risk.level="MODERATE";risk.action="REST / HYDRATE";}
  else if(risk.score<75){risk.level="HIGH";risk.action="LOCAL ALERT";}
  else {risk.level="CRITICAL";risk.action="SOS / ASSISTANCE";}
}

void alerts(){
  digitalWrite(LED_LOW,risk.score<50); digitalWrite(LED_HIGH,risk.score>=50 && risk.score<75); digitalWrite(LED_CRIT,risk.score>=75);
  if(risk.score>=75){ tone(BUZZER_PIN,1800,140); }
  else if(risk.score>=50){ tone(BUZZER_PIN,1200,80); }
  else noTone(BUZZER_PIN);
}

void oled(){
  display.clearDisplay();display.setTextColor(SSD1306_WHITE);display.setTextSize(1);display.setCursor(0,0);
  display.println("BIO SENTINEL"); display.println("SIMULATION | LOCAL AI");
  display.printf("HR %.0f  SpO2 %.1f%%\n",filtered.hr,filtered.spo2);
  display.printf("TEMP %.1fC ACT %.0f%%\n",filtered.bodyTemp,filtered.activity*100);
  display.printf("RISK %03.0f %s\n",risk.score,risk.level);
  display.println(scenarioName());
  if(offline) display.println("OFFLINE MODE"); else display.println("NETWORK OPTIONAL");
  if(feat.fallScore>0.5f){display.println("FALL DETECTED");}
  if(risk.score>=75){display.println("SOS COUNTDOWN");}
  display.display();
}

void printLog(){
  Serial.printf("RAW hr=%.1f spo2=%.1f temp=%.2f act=%.2f hum=%.1f pm=%.1f\n",raw.hr,raw.spo2,raw.bodyTemp,raw.activity,raw.humidity,raw.pm25);
  Serial.printf("FEATURES cardio=%.2f heat=%.2f env=%.2f baseline=%.2f fatigue=%.2f fall=%.0f\n",feat.cardioStress,feat.heatStress,feat.envStress,feat.baselineDev,feat.fatigue,feat.fallScore);
  Serial.printf("TINYML probability=%.2f | RISK=%d | %s | ACTION=%s | OFFLINE=%s\n",risk.prob,(int)risk.score,risk.level,risk.action,offline?"YES":"NO");
}

void handleButtons(){
  static uint32_t last[6]={0}; int pins[6]={BTN_NORMAL,BTN_HEAT,BTN_POLLUTION,BTN_FALL,BTN_CRITICAL,BTN_OFFLINE};
  Scenario ss[6]={NORMAL,HEAT,POLLUTION,FALL,CRITICAL,OFFLINE};
  for(int i=0;i<6;i++){if(digitalRead(pins[i])==LOW && millis()-last[i]>350){last[i]=millis();setScenario(ss[i]);}}
}

void setup(){
  Serial.begin(115200); pinMode(BUZZER_PIN,OUTPUT); pinMode(LED_LOW,OUTPUT);pinMode(LED_HIGH,OUTPUT);pinMode(LED_CRIT,OUTPUT);
  int btns[]={BTN_NORMAL,BTN_HEAT,BTN_POLLUTION,BTN_FALL,BTN_CRITICAL,BTN_OFFLINE}; for(int p:btns)pinMode(p,INPUT_PULLUP);
  Wire.begin(SDA_PIN,SCL_PIN); display.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR); display.clearDisplay();display.display();
  Serial.println("BIO SENTINEL | SIMULATION | NETWORK LOSS != HEALTH SYSTEM FAILURE");
}

void loop(){
  handleButtons(); generateSensors(); filterSignals(); extractFeatures(); decideRisk(); alerts(); oled(); printLog();
  if(risk.score>=75 && !sos){sos=true;sosStarted=millis();Serial.println("SOS COUNTDOWN STARTED | SIMULATION");}
  delay(1000);
}

# 🛡️ BIO SENTINEL

## AI-Powered Personal Health & Disaster-Resilience Companion

> A secure, AI-powered, privacy-preserving and offline-first wearable system designed to continuously monitor personal health, understand environmental conditions, estimate health risks and provide early warnings before situations become critical.

---

## 🚀 Smart India Hackathon 2026

| Parameter | Details |
|---|---|
| **Problem Statement ID** | 26181 |
| **Problem Statement** | AI-Powered Personal Health & Disaster-Resilience Companion |
| **Theme** | MedTech / BioTech / HealthTech |
| **Category** | Hardware |
| **Team Name** | BIO SENTINEL |

---

## 💡 Problem

During heat waves, floods, pollution events and other disasters, health risks can escalate rapidly while healthcare access and network connectivity may become unreliable.

Most existing wearable systems primarily display health measurements. They may not sufficiently contextualize these measurements using:

- Personal health baseline
- Physical activity
- Environmental conditions
- Disaster context

This creates a gap between **measuring health data** and **understanding health risk**.

---

# 💙 Our Solution — BIO SENTINEL

BIO SENTINEL is an **offline-first wearable health companion** that combines:

❤️ Physiological Data  
🏃 Activity Data  
🌡️ Environmental Data  
🧠 On-Device AI  
👤 Personal Baseline  
🌪️ Disaster Context  

to generate a **Personal Risk Score** and provide appropriate guidance, alerts and emergency assistance.

---

# ✨ Key Features

### ❤️ Continuous Health Monitoring

Monitors important physiological parameters such as:

- Heart Rate
- SpO₂
- ECG
- Body Temperature

### 🧠 On-Device AI / TinyML

AI inference is performed locally on the wearable using the ESP32-S3.

This enables:

- Low latency
- Reduced cloud dependency
- Offline operation
- Better privacy

### 👤 Personal Baseline

Instead of relying only on fixed thresholds, BIO SENTINEL learns the user's normal pattern and detects deviations from that baseline.

### 🔗 Multi-Sensor Fusion

The system combines:

**Physiology + Activity + Environment**

to improve context-aware risk estimation.

### 🌡️ Environmental Awareness

Environmental parameters are considered during risk assessment, including:

- Temperature
- Humidity
- PM2.5
- CO₂
- VOC
- NOx

### 🌪️ Disaster-Aware Monitoring

Dedicated contextual modes for:

- Heat Waves
- Air Pollution
- Floods
- Cyclones / Storms
- Disease Outbreaks

### 📡 Offline-First Operation

Network loss does not mean health-system failure.

Local processing and alerts continue even when connectivity is unavailable.

### 🚨 Emergency Assistance

For critical conditions, the system can initiate a local SOS flow with optional GPS and communication hardware.

### 🔐 Privacy-Preserving Architecture

Health data is intended to be processed locally whenever possible.

- On-device processing
- Encrypted local storage
- User-controlled sharing
- Reduced cloud dependency

---

# 🏗️ System Architecture

```text
             ┌──────────────────────┐
             │   HEALTH SENSORS     │
             │ HR • SpO₂ • ECG •    │
             │ Body Temperature     │
             └──────────┬───────────┘
                        │
             ┌──────────▼───────────┐
             │   ACTIVITY DATA      │
             │ IMU • Fall • Motion  │
             └──────────┬───────────┘
                        │
             ┌──────────▼───────────┐
             │ ENVIRONMENTAL DATA   │
             │ Temp • Humidity •    │
             │ PM2.5 • CO₂ • VOC    │
             └──────────┬───────────┘
                        │
                        ▼
             ┌──────────────────────┐
             │ SIGNAL PROCESSING    │
             │ Filtering • Quality  │
             │ Missing Data         │
             └──────────┬───────────┘
                        │
                        ▼
             ┌──────────────────────┐
             │ FEATURE EXTRACTION   │
             │ Trends • Variability │
             │ Activity • Exposure  │
             └──────────┬───────────┘
                        │
                        ▼
             ┌──────────────────────┐
             │ PERSONAL BASELINE    │
             │ Deviation Detection  │
             └──────────┬───────────┘
                        │
                        ▼
             ┌──────────────────────┐
             │ SENSOR FUSION        │
             │ Multi-modal Context  │
             └──────────┬───────────┘
                        │
                        ▼
             ┌──────────────────────┐
             │ ON-DEVICE TINYML     │
             │ ESP32-S3 Inference   │
             └──────────┬───────────┘
                        │
                        ▼
             ┌──────────────────────┐
             │ PERSONAL RISK SCORE  │
             └──────────┬───────────┘
                        │
          ┌─────────────┼──────────────┐
          ▼             ▼              ▼
        LOW         MODERATE          HIGH
      MONITOR        ADVICE           ALERT
                                       │
                                       ▼
                                   CRITICAL
                                   LOCAL SOS
#🔬 Hardware
Main Controller
ESP32-S3 N8R8
On-device TinyML inference
BLE / Wi-Fi connectivity
#❤️ Health Sensors
Sensor	Purpose
MAX30102	Heart Rate + SpO₂
AD8232	ECG
TMP117	Body Temperature
#🏃 Motion Sensor
Sensor	Purpose
ICN-42670-P IMU	Accelerometer + Gyroscope
IMU	Motion & Fall Detection
#🌡️ Environmental Sensors
Sensor	Parameters
SCD41	CO₂ + Temperature + Humidity
SEN55	PM + VOC + NOx + Temperature + Humidity
📱 Output & Communication
SSD1306 OLED Display
DRV2605L Vibration Driver
Vibration Motor
Buzzer
BLE
Optional GSM / Cellular
Optional GNSS
Local Flash Storage
#🧠 AI / TinyML Pipeline

BIO SENTINEL follows an edge-first AI pipeline:

RAW SENSOR DATA
       ↓
SIGNAL FILTERING
       ↓
FEATURE EXTRACTION
       ↓
PERSONAL BASELINE
       ↓
SENSOR FUSION
       ↓
TINYML INFERENCE
       ↓
PERSONAL RISK SCORE
       ↓
ACTION

The project plans to begin with interpretable baseline models and compare lightweight approaches such as:

Decision Tree
Random Forest
Compact Neural Models

The selected model can then be quantized for edge deployment.

#📊 Personal Risk Score

BIO SENTINEL converts multi-sensor information into four risk levels.

Risk Level	System Response
🟢 LOW	Continue Monitoring
🟡 MODERATE	Advice / Guidance
🟠 HIGH	Local Alert & Warning
🔴 CRITICAL	Local SOS + GPS
🚨 SOS & Emergency Flow
CRITICAL RISK
      ↓
SOS COUNTDOWN
      ↓
CANCEL / CONFIRM
      ↓
 ┌───────────────┐
 │ False Alarm?  │
 └───────┬───────┘
         │
      No Response
         ↓
     SOS + GPS
         ↓
 Guardian / Responder

Location/SOS transmission requires available communication hardware/channel and user permission.

#🌪️ Disaster-Aware Context Engine

BIO SENTINEL adapts risk assessment according to environmental and disaster context.

              DISASTER CONTEXT
                     │
        ┌────────────┼────────────┐
        ▼            ▼            ▼
    HEAT WAVE    POLLUTION      FLOOD
        │            │            │
        ▼            ▼            ▼
   Heat Stress   Respiratory   Emergency
      Risk          Risk         Flow

Additional supported contexts include:

Cyclone / Storm
Disease Outbreak
#📡 Connectivity Architecture
                BIO SENTINEL
                     │
              Local Processing
                     │
          ┌──────────┴──────────┐
          ▼                     ▼
     Local Alerts              BLE
                                │
                                ▼
                           Mobile App
                                │
                         ┌──────┴──────┐
                         ▼             ▼
                       GSM          Internet
                    (Optional)      (Optional)

During connectivity loss, local inference and local alerts continue.

#🔐 Privacy & Security

BIO SENTINEL follows a privacy-first architecture.

Privacy Principles
On-device processing
Encrypted local storage
User-controlled data sharing
Reduced data transmission
Offline-first architecture

The goal is to keep raw health information under the user's control whenever possible.

#🧪 Validation Plan

The system will be validated using controlled experiments and measurable parameters.

Parameter	Validation Method
Signal Quality	Compare clean vs motion-corrupted readings
Risk Classification	Precision / Recall / F1-score
False Alerts	Count incorrect alerts across repeated trials
Inference Latency	Measure sensor-to-decision time
Emergency Latency	Measure event-to-alert time
Offline Operation	Disconnect network and verify local operation
Battery Endurance	Continuous-use / duty-cycle testing
Fall Detection	Controlled simulated fall experiments
🛠️ Development Roadmap
P0 ──► HARDWARE
       │
P1 ──► SIGNAL PROCESSING
       │
P2 ──► PERSONAL BASELINE + AI
       │
P3 ──► DISASTER CONTEXT
       │
P4 ──► MOBILE + SOS
       │
P5 ──► VALIDATION
#📁 Repository Structure
BIO-SENTINEL/
│
├── hardware/
│   └── README.md
│
├── firmware/
│   └── README.md
│
├── ai-model/
│   └── README.md
│
├── mobile-app/
│   └── README.md
│
├── docs/
│   └── README.md
│
├── assets/
│   └── README.md
│
├── tests/
│   └── README.md
│
└── README.md
#🌍 Impact

BIO SENTINEL is designed to provide value to:

#👴 Elderly & People Living Alone
Fall detection
Early warnings
Caregiver alerts
#👷 Outdoor & Frontline Workers
Heat-stress awareness
Rest / hydration guidance
Environmental risk awareness
#❤️ Vulnerable / Chronic-Care Users
Personal baseline
Abnormal-pattern alerts
#🏘️ Rural & Low-Connectivity Areas
Offline monitoring
Local alerts
Reduced connectivity dependency
#🚑 Disaster Response
Faster identification of individuals at risk
Local emergency assistance
Resilient operation during connectivity disruptions
#⭐ Key Differentiators
Traditional Wearables	BIO SENTINEL
Display health numbers	Estimates personal risk
Fixed thresholds	Personal baseline
Mainly health focused	Health + Activity + Environment
Cloud-dependent approaches	Edge-first / Offline-first
Generic alerts	Context-aware alerts
Limited disaster context	Disaster-aware monitoring
Data transmission dependency	Local processing
#📌 Project Status

#🚧 Prototype / Development in Progress

Current development focuses on:

Hardware integration
Sensor acquisition
Signal processing
Personal baseline
TinyML pipeline
Risk scoring
Disaster-aware context
Mobile connectivity
SOS workflow
System validation
#🔮 Future Scope

The project can evolve from a prototype wearable into a scalable ecosystem connecting:

Wearable
    ↓
Mobile Application
    ↓
Guardian / Caregiver
    ↓
Healthcare Ecosystem
    ↓
Disaster Response

The architecture is designed to remain modular and adaptable across different wearable form factors.

#👥 Team
BIO SENTINEL

Smart India Hackathon 2026

Building technology that doesn't just measure health — it helps understand risk and act before it becomes critical.

#📜 Disclaimer

BIO SENTINEL is a prototype research and engineering project.

The system is intended for health monitoring, risk awareness and early warning assistance. It is not intended to replace professional medical diagnosis or emergency medical services.

#⭐ Support the Project

If you find BIO SENTINEL interesting:

⭐ Star the repository
🍴 Fork the project
🐛 Report issues
💡 Suggest improvements
🤝 Contribute to development

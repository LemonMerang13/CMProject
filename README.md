# CMProject
Wireless Vibration Monitoring of Table Fan

Problem Statement:

Rotationary machines are important components in various industrial application and their reliability directly affects the productivity, efficiency, safety and maintenance costs for said application. Condition Monitoring is an effective form of predictive maintenance that allows for operational faults and reliability issues to be detected far earlier before failure occurs. One of the most used and effective techniques is vibration monitoring which monitors the characteristics of the vibration signal. This form of condition monitoring allows for detection of mechanical issues such as imbalance, misalignment, bearing defects, or looseness through changes in vibration profile.

Aim:

Develop a low-cost, wireless sensor system to monitor vibration levels in a table fan and detect early faults such as unbalance or blade misalignment.

Objectives:

1. Develop hardware design for a wireless monitoring system for vibration analysis of a fan
2. Obtain data from vibration signal using the monitoring system
3. Convert Vibration Signal to FFT diagram
4. Identify key alert features for alarm detection
5. Send necessary data to cloud dashboard for alarm detection
6. Design wireless alert system with cloud dashboard
7. Upload code and project on GitHub



Hardware Design 

Components:
- ESP8266 Microcontroller: Processes vibration data and enables Wi-Fi connectivity.
- MPU6050 Accelerometer: Measures 3-axis vibrations from the table fan

Setup:
1. Components are connected as shown in the schematics below.
2. Both components are placed on the top of motor of the fan.
3. The use of mobile hotspot allows the ESP8266 to connect with thingspeak
4. All of the code is compiled and uploaded to the ESP8266


![Image Alt](https://github.com/LemonMerang13/CMProject/blob/c897ff6fcd6be3197ff1dd9acc242b8a4f2280ba/Pictures%20and%20Videos%20for%20CM%20Project/Circuit%20Schematics.jpeg)

Testing parameters:
- Test 1: Obtain values of normal fan operation
- Test 2: Add weight shift centre mass leading to unbalance
- The values of Test 1 are obtained and compared to test 2. 

![Image Alt](https://github.com/LemonMerang13/CMProject/blob/c897ff6fcd6be3197ff1dd9acc242b8a4f2280ba/Pictures%20and%20Videos%20for%20CM%20Project/Fan%20image.png)

FFT Analysis

FFT Analysis:
- Implemented using Arduino FFT library on ESP8266 for edge-based processing.
- Data received is transferred to and plotted on excel
- Detects unbalance at 1x RPM (15Hz) and impulsive vibrations (e.g., bearing wear).

Healthy Fan:
- Low amplitude at 1x RPM, smooth spectrum with minimal harmonics.
- RMS vibration: 0.4.

Faulty Fan (Unbalance):
- Prominent peak at 1x RPM due to unbalanced blades.
- RMS vibration: more than 1.5 of Healthy Fan RMS (0.6) (triggers warning).

Visual:

Healthy:

![Image Alt](https://github.com/LemonMerang13/CMProject/blob/c897ff6fcd6be3197ff1dd9acc242b8a4f2280ba/Pictures%20and%20Videos%20for%20CM%20Project/Healthy%20Reading.jpg)


Faulty:

![Image Alt](https://github.com/LemonMerang13/CMProject/blob/c897ff6fcd6be3197ff1dd9acc242b8a4f2280ba/Pictures%20and%20Videos%20for%20CM%20Project/Faulty%20Reading.jpg)


Cloud Dashboard 
- Platform: ThingSpeak (free IoT platform with prebuilt visualization tools).

Features:
- Real-time display of vibration trends (RMS, peak frequencies).
- Alerts via Telegram Bot (UTMCM_ExcessiveVibrationbot)

Implementation:
- Vibration data streamed from ESP8266 to ThingSpeak via MQTT.
- Charts display RMS levels, FFT peaks, and fault indicators specific to the table fan.

Dashboard Screenshots

![Image Alt](https://github.com/LemonMerang13/CMProject/blob/c897ff6fcd6be3197ff1dd9acc242b8a4f2280ba/Pictures%20and%20Videos%20for%20CM%20Project/ThingSpeak%20Dashboard.jpg)

Alert System
- Telegram Bot (MyLABAlertBot)
- Threshhold: RMS value > 0.6
- Once alarm values are above threshold, arduino send message to Telegram Bot
  
![Image Alt](https://github.com/LemonMerang13/CMProject/blob/649f65e3bd08b0440d8012ff90e8d861505c64e5/Pictures%20and%20Videos%20for%20CM%20Project/Alert%20Notification.jpg)

Project Outcomes 

Achievements:
- Developed a wireless sensor system for real-time vibration monitoring of a table fan.
- Implemented FFT-based fault detection for unbalance and bearing issues.
- Created a cloud dashboard with trends and automated alerts.

Deliverables:
- GitHub Repository: [https://github.com/LemonMerang13/CMProject/tree/main]
- YouTube Video: [Insert Unlisted YouTube Link]
- Dataset: Sample vibration logs uploaded to GitHub.





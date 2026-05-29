Note:   The load is a resistive 300W @ 248V AC input.  Using 3 x CT Clamps over the single phase mains cable.

===========

UART 1 Opened (RS485 Port)

FLEX Lite - Integrated Firmware for DitroniX Power Energy Monitor SDK Boards

Scanning and Configuring Board.. Please Wait..

Hardware Options and Auto Configuration
────────────────────────────────────────
Scanning I2C Bus for Devices for Auto Configuration ...
 I2C Possible Device(s) found at Address:
 * Hex 0x4A (74)         TMP102 Temperature Sensor
 * Hex 0x50 (80)         EEPROM
 * Hex 0x58 (88)         EEPROM
 * Hex 0x68 (104)        DS3231SN Accurate RTC
 I2C Bus Scan Complete

EPEM E32 IoT Power Energy Monitor Controller

EPEM E32 Register Status and Startup Report
ESP32-C6 Serial ID: F0FEFFBA2010
FLEX Lite for EPEM E32 Firmware Version: 260529
Board Location: Test

Wi-Fi Options
──────────────

Software Options
─────────────────
Connecting to the ATM90E32
====================================
ATM90E32 Connected - OK

EPEM E32 Hardware Setup and Power-Up Test - Board Initialized

EPEM E32 IoT Power Energy Monitor Controller

EPEM E32 Register Status and Startup Report
ESP32-C6 Serial ID: F0FEFFBA2010
FLEX Lite for EPEM E32 Firmware Version: 260529
Location: Test

Hardware Configuration for ATM90E32
────────────────────────────────────
AC Voltage Inputs:      Single Input V1 will be used for Current Phase Calculations
Split AC Voltage:       Dual  or Split Voltage Input Disabled
CT Current Clamps:      Configured for 1, 2, 3 Phase

[Status Values]
System Status:          S0:0x37E                S1:0xFFFE
Meter Status:           E0:0x200                E1:0x0

Mains RMS Voltage
──────────────────
- - - - - - - - - - - - - - - - - - - - - - - - - - - 
Voltage 1: 243.01 V (Single Line Voltage Selected)

Mains RMS Current
──────────────────
Current CT1: 1.17 A
Current CT2: 1.17 A
Current CT3: 1.18 A
- - - - - - - - - - - - - - - - - - - - - - - 
Actual Total Current: 3.52 A (CT1~CT2~CT3)

Calculated RMS Power
─────────────────────
Power V1*I1: 284.32 W   0.28 kW
Power V1*I2: 284.32 W   0.28 kW
Power V1*I3: 286.75 W   0.29 kW
- - - - - - - - - - - - - - - - - - - - - - - - - 
Calculated Total Power: 854.95 W (CT1~CT2~CT3)

Active Power (Absorbed or Used by the Load)
────────────────────────────────────────────
Active Power CT1: 284.79 W      (Import)
Active Power CT2: 284.12 W      (Import)
Active Power CT3: 285.62 W      (Import)
- - - - - - - - - - - - - - - - - - - - - 
Total Active Power: 570.40 W    (Import)
- - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Calculated Total Active Power: 854.53 W (CT1~CT2~CT3)

Re-Active Power
────────────────
ReActive Power CT1: 19.04 VAR
ReActive Power CT2: 18.22 VAR
ReActive Power CT3: 16.76 VAR
- - - - - - - - - - - - - - - - - - - - - - - - 
Total ReActive Power: 35.79 VAR (CT1~CT2~CT3)
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Calculated Total Reactive Power: 54.02 W (CT1~CT2~CT3)

Apparent Power (Total Amount of Power Flowing from Source to Load)
───────────────────────────────────────────────────────────────────
Apparent Power CT1: 285.65 VA
Apparent Power CT2: 284.82 VA
Apparent Power CT3: 286.33 VA
- - - - - - - - - - - - - - - - - - - - - - - - 
Total Apparent Power: 572.05 VA (CT1~CT2~CT3)
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Calculated Total Apparent Power: 856.80 VA (CT1~CT2~CT3)


Other Power Information
────────────────────────
Total Fundamental Power: 569.96 pH(t)

Total Harmonic Power: 0.76 
Power Factor
─────────────
Power Factor CT1: 1.00 PF
Power Factor CT2: 1.00 PF
Power Factor CT3: 1.00 PF
- - - - - - - - - - - - - - - 
Total Power Factor: 1.00 PF

Phase Angle
────────────
Phase Angle CT1: 3.80
Phase Angle CT2: 3.70
Phase Angle CT3: 3.30

Other Information
──────────────────
Chip Temperature: 25.00 °C

Mains Frequency: 50.01 Hz

ADC Raw: 49 > ADC Adjusted: 49 > Calculated: 5.12 V DC


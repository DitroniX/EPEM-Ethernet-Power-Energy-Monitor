## EPEM ESP32-C6 ATM90E36 Ethernet Power Energy Monitor SDK

Exciting news for July/August 2025, with the advent of the powerful ESP32-C6, this new compact EPEM board is due for release. Watch this space........

The new **EPEM** SDK board contains all of the original proven [IPEM](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor) functionality, including the ATM90E36, then adds Ethernet and RS-485 interfaces.

[**For Latest Project Updates - Click Here**](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor?tab=readme-ov-file#updates)

[**Sign up to our Newsletter - Click Here**](https://ditronix.net/newsletter)

![EPEM - Ethernet Power Energy Monitor Preview](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/blob/main/Datasheets%20and%20Information/EPEM%20-%20Ethernet%20Power%20Energy%20Monitor%20Preview.png?raw=true)

## Updates

-   25-07-22 - Prototype Beta boards in fabrication and SMT
-   25-07-06 - Information Preview Launched
-   25-07-06 - Beta Board final layout review before fabrication
-   25-06    - Alpha Testing, Schematics and Layout Topology

## Features

The **EPEM** IoT **Ethernet Power Energy Monitor** board main features and Outline Specifications:

**Board**

-  **Compact and Flexible Proven Design**
	- Only 85mm x 55mm
-  **All connections are Low Voltage, for Safe Installation and Operation**
-  **Easy to interface and Program**
- **Powerful Radio or Ethernet Connections to IoT Network**

**Devices and Interfaces**

-  **Espressif ESP32 C6 MINI 1U**
    -   Wi-Fi 6 2.4 GHz 802.11ax | 802.11b/g/n
    -   Bluetooth | LE | 5.3 | Mesh
    -   Thread 1.4 | 802.15.4 | Mesh
    -   Zigbee 3.0 | 802.15.4 | Mesh
-  **TYPE C Interfaces (x2)**
    - ESP32-C6 USB | JTAG | Debugging | 5V Supply
    - UART (esp32-c6-devkitm-1 compatible) | Serial Monitor | 5V Supply
-  **ATM90E36 Energy Monitor**
	- Enhanced Poly-Phase Energy Metering IC
	- High-Performance Wide-Span
	- High Accuracy
		- ±0.1% for Active Energy
		- ±0.2% for Reactive Energy
	- AC 50Hz-60Hz Energy Monitor
    -   Measures Currents (Board Configurable)
		-   Single phases (up to 3)
		-   Two phase
		-   Dual phase
		-   Split phase
		-   Three-phase four-wire (3P4W, Y0)
		-   Three-phase three-wire (3P3W, Y or Δ), systems
    -   Selectable Current Input Connections (3 Phases Line +1 Phase Neutral inputs)
		-   3 x Phases Line CT Clamp Inputs via 3.5mm Jack (Example YHDC SCT013000)*
		-   3 x Phases Line Rogowski Current Transformer Inputs via 2.54 Pin Header
		-   1 x Phase Neutral CT Clamp Input via 3.5mm Jack (Example YHDC SCT013000)*
		-   1 x Phase Neutral Rogowski Current Transformer via Input 2.54 Pin Header 
    -   AC Inputs
		-   3 x Low Voltage Voltage Inputs (12V AC RMS)
		-   AC Low Voltage Input (for Safety)
		-   Power Safely Derived from a SELV / Wall AC/AC Power Supply 12V AC RMS
		-   (Examples Bell Transformer like GreenBrook DAT01A or TLC TC TR7)
    - DMA Mode (Logging via SPI)
    - SPI Mode
    - Split Phase (1+3 Typically USA)
    - Power Modes Settings
    - ATM Status LEDs
		-   CF1 - Active
		-   CF2 - ReActive
		-   CF3 - Fundamental
		-   CF4 - Harmonic
    - Warning/IRQ Monitor
 	- Single Point Calibration per Phase
-  **W5500 Ethernet**
	- WIZnet Hardwired Internet Controller
	- Integrated Full TCP/IP Stack
	- RJ45 with Active and Link LEDs
-  **RS-485 Interface**
    - MODBUS or Other Protocol
    - Selectable 120R Load Resistor
    - Optional Ground for Shielded Cable to Asymmetrical TVS Diode
-  **24C64 EEPROM**
    - Parameter Settings
    - Logging
    - Meter Readings
-  **DS3231 Real Time Clock**
    - Accurate Real Time Clock
    - Battery Back Up Option
-  **TMP102 Temperature**
    - Ambient Temperature Sensor
- **OLED I2C Interface**
    - I2C Interface
    - OLED I2C Connector
    - Configurable Power Pins
-  **WS2812 RGB LED**
    - RGB Status LED
    - Associated LM2775 5V SMPS
-  **AP63203 Onboard SMPS Board Power Supply**
	- 5V From Either Type C USB
	- Power taken from DC Input (4.5V to 20V)
	- Power taken from AC Input 12V
-  **Buttons**
	- Reset Button
	- User Programmable Button / PGM



## **Further Information**

Additional information, and other technical details on this project, maybe found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE, PlatformIO)*
 -  **Datasheets and Information** *(Component Datasheets, Schematics, Board Layouts, Photos, Technical Documentation)*
 - **Certification** *(Related Repository Project or Part, Certification Information)*

**Repository Tabs**

 - **Wiki** *(Related Repository Wiki pages and Technical User Information)*
 - **Discussions** *(Related Repository User Discussion Forum)*
 - **Issues** *(Related Repository Technical Issues and Fixes)*

***

We value our Customers, Users of our designs and STEM Communities, all over the World . Should you have any other questions, or feedback to share to others, please feel free to:

* Visit the related [Project](https://github.com/DitroniX?tab=repositories) *plus the related* **Discussions** and **Wiki** Pages.  See tab in each separate repository.
* **Project Community Information** can be found at https://www.hackster.io/DitroniX
* [DitroniX.net Website - Contact Us](https://ditronix.net/contact/)
* **Twitter**: [https://twitter.com/DitroniX](https://twitter.com/DitroniX)
* [Supporting the STEM Projects - BuyMeACoffee](https://www.buymeacoffee.com/DitroniX)
*  **LinkedIN**: [https://www.linkedin.com/in/g8puo/](https://www.linkedin.com/in/g8puo/)

***Dave Williams, Maidstone, UK.***

Electronics Engineer | Software Developer | R&D Support | RF Engineering | Product Certification and Testing | STEM Ambassador

## STEM

**Supporting [STEM Learning](https://www.stem.org.uk/)**

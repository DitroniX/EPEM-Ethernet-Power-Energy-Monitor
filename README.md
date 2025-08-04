## EPEM ESP32-C6 ATM90E36 Ethernet Power Energy Monitor SDK

**Supporting STEM Electronic Internet of Things & Home Automation Technology for Smart Energy Monitoring**

[![KickStarter](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/blob/main/Datasheets%20and%20Information/Kickstarter%20-%20Now%20Live%20-%20EPEM.png 'KickStarter')](https://www.kickstarter.com/profile/ditronix/created)

The new **EPEM** SDK board contains all of the original proven [IPEM](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor) functionality, including the ATM90E36, then adds Ethernet and RS-485 interfaces.

[**For Latest Project Updates - Click Here**](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor?tab=readme-ov-file#updates)


![EPEM - Ethernet Power Energy Monitor Overview](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/blob/main/Datasheets%20and%20Information/DitroniX.net%20EPEM%20Ethernet%20Power%20Energy%20Monitor%20Board%20SDK%20Overview.png?raw=true)

## Updates

-   25-07-31 - YouTube Video Published [EPEM](https://www.youtube.com/watch?v=buDLhy5ddp8&ab_channel=DitroniX)
-   25-07-31 - Preparing for launch on [Kickstarter](https://www.kickstarter.com/projects/ditronix/epem-ethernet-power-energy-monitor)
-   25-07-31 - Prototype Beta boards arrived and looking great!
-   25-07-22 - Prototype Beta boards in fabrication and SMT
-   25-07-06 - Information Preview Launched
-   25-07-06 - Beta Board final layout review before fabrication
-   25-06    - Alpha Testing, Schematics and Layout Topology


_EPEM Ethernet Power Energy Monitor - ESP32-C6 ATM90E36 Prototype Beta (31st July 2025)_

![EPEM - Ethernet Power Energy Monitor Preview](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/blob/main/Datasheets%20and%20Information/EPEM%20ESP32-C6%20ATM90E36%20Prototype%20Beta.jpg?raw=true)

## Features

### **What is EPEM?**

**EPEM** is an acronym for **Ethernet, Power Energy Monitor**

**EPEM** is a compact and powerful STEM Community board which is designed for IoT monitoring of mains power circuits.

### **What is a Power Energy Monitor?**

A power energy monitor is a device that safely samples the AC voltage and current flow, from one, or more, electrical phases, and from this, derives a range of accurate mains power measurements which can then be used for smart metering, or energy monitoring purposes.

These individual measurements can then be extrapolated into a range of useful real-time data such as total RMS power, active, re-active power, harmonic power, power factor, phase angles, frequency, etc.

The purpose of extracting the power energy data within Solar Inverter installations, as an example, will enable you to much better manage what you do with the available energy, ways of storage, EV Charging, divert under certain conditions, and control flow from, and to, the grid.

All this data is far more extended information than the basic electric meter functionality, allowing for smart IoT systems and greater efficient use of our own energy. It may sound complicated but it does not need to be and all depends on your setup, use-case and what you want to achieve.

### **Where can EPEM be used?**

The **EPEM** board can be used in a number of projects and installations such as:

-   Homes
-   Education
-   Offices
-   Commercial
-   Factories
-   Farms
-   EV Charging
-   Smart Metering
-   Greenhouses
-   Factories
-   Industry
-   Solar Farms
-   Wind Farms
-   Off Grid Systems

### **EPEM History**

The **EPEM** board is a natural design evolution of the original and popular ESP32 based [IPEM](https://github.com/DitroniX/IPEM-IoT-Power-Energy-Monitor) (IoT Power Energy Monitor), and some features from the [IPEM PiHat](https://github.com/DitroniX/IPEM-PiHat-IoT-Power-Energy-Monitor). These boards are in use around the World and continuity in supply was needed.

Due to some part obsolescence on the original IPEM ESP32 board and natural 'upgrade' options such as USB Type C, a redesign was required and thus the **EPEM** concept was placed on the drawing board. With various ingredients in the pantry, this moved around like a small game of snakes and ladders in candle light and a final design came out of the kitchen oven.

#### The key ingredients changes:

-   **Swap ESP32 to ESP32-C6** - Increased speed and superior radio with Zigbee Mesh
-   **Standardize ATM90E36** only - This version provides all of the ATM90E32 features, plus a lot more functionality and is the more popular of the two
-   **Add Ethernet** - Power is naturally available so PoE overhead not considered a requirement
-   **Add RS-485** - For local data gathering and interfacing
-   **Move to USB Type C** (x 2)
-   **Add Rogowski Inputs** - allows for high current monitoring where needed
-   **Standardize on ESP32-C6 U.FL** only - compact and most popular package

### **Challenge**

My design focus was to maintain the original IPEM board size of 85x55mm, maintaining mechanical compatibility, where possible, but also squeeze in:

-   Ethernet Controller with Full TCP/IP Stack
-   RJ45 Connector
-   RS-485 Interface (MODBUS)
-   DS3231DN RTC & Battery Backup Option
-   Remove Solder Jumpers

In addition, although the board is multi-layer, I wanted to try and keep all SMT ideally to one side in order to keep the board mechanical skyline as low and compact as possible, this meant all standard passive components shrinking down to very small size and so including more features for the user.



### **MCU Change**

All original IPEM boards and variants were based on the ESP32-WROOM.

With the advances in Espressif MCU technologies, moving away from the humble ESP32 to the newer ESP32-C6 provides increased speed advantage plus the superior and powerful radio with Zigbee, thread and mesh, so the move to using the ESP32-C6 in **EPEM** was an obvious choice. It also has a much smaller footprint, which is a big bonus.

### **Ethernet**

For a while, users have been asking about possibility of adding Ethernet to the IPEM board, largely due to either the boards being mounted inside cabinets for example, or away from access points, and also for security or low power closed IoT systems.

The ESP32-C6 does not have an internal EMAC with MII/RMII (as with original ESP32), so I have opted to use the SPI interface to the popular WIZnet W5500 Ethernet Interface.

### **Radio and U.FL**

With **EPEM** boards potentially going into metal, or shielded cabinets, or needing external antennas, aside from ethernet, the U.FL connection interest has also peaked.

As previously mentioned, the ESP32-C6 includes a powerful radio module, opening up a range of IoT applications that include:

#### Wi-Fi 6

-   2.4 GHz (2400 ~ 2483.5 MHz)
-   802.11ax (20 MHz bandwidth)
-   802.11b/g/n (20/40 MHz bandwidth)
-   CE Max EIRP 19.81 dBm

#### Bluetooth

-   2.4 GHz (2400 ~ 2483.5 MHz)
-   Bluetooth LE
-   Bluetooth 5.3
-   Bluetooth Mesh
-   CE Max EIRP 18.46 dBm

#### [Thread](https://www.threadgroup.org/) 1.4

-   2.4 GHz (2405 ~ 2480 MHz)
-   802.15.4
-   Thread Mesh
-   CE Max EIRP 10.29 dBm

#### [Zigbee](https://csa-iot.org/) 3.0

-   2.4 GHz (2405 ~ 2480 MHz)
-   802.15.4
-   Zigbee Mesh
-   CE Max EIRP 10.40 dBm

Zigbee and mesh are expanding in an range of home, farming and industrial applications, so will be interested to see how this develops over time.

Using a mix of Ethernet and Zigbee, installations could offer remote mesh systems, with a mix of communications technologies.

#### _**Tip: MHF3 Antenna Connection**_

_The ESP32-C6-MINI-1U uses an_ [_IPEX MHF3_](https://www.i-pex.com/product/mhf-III) _antenna connector (also known as IPEX3), which is much smaller than the standard commonly used IPEX MHF1, for example, on the ESP32-WROOM-32UE._

_NB. This MHF3 connector, together with its very similar but different MHF4 variant, are commonly used on M.2 cards and laptop type Wi-Fi cards. The MHF3 and MHF4 connectors are not compatible as they are very so slightly different diameter and heights (mating depth)._

### **RS-485**

To interfacing to other Smart Meters, Devices and Smart Batteries which have an RS-485 MODBUS interface, the **EPEM** includes a standard industry interface which allows you to collect, or control, other devices on the RS-485 bus.

This RS-485 interface could also be used to control local CCTV on remote installations, or even send data from the **EPEM** board over a single twisted pair, up to around 1,200 meters (4,000 feet), in distance. Ideal for farms and remote installations.

### **Real Time Clock**

The need to maintain a very accurate real time clock when metering, or logging, was requested by some users. Ideally battery backed up.

This feature was included on the [IPEM PiHat](https://github.com/DitroniX/IPEM-PiHat-IoT-Power-Energy-Monitor) and so I have included it on the EPEM too.

Using the same extremely accurate and low power DS3231SN real time clock IC, with integrated temperature compensated crystal oscillator (TCXO), this is easily controlled via I2C and maintained via external optional plug-in CR2302 3V coin battery.

### **Board Configuration**

Earlier IPEM boards used solder jumpers and whilst these worked, I wanted to make it easier to configure the boards.

On the IPEM PiHat's, I used DIP switches which worked well but these take up board space and are expensive. So on EPEM I have opted to use plug-in 2mm jumpers which give the best of both worlds.

### **Outline Features and Specifications**

The **EPEM** IoT **Ethernet Power Energy Monitor** board main features and Outline Specifications:

#### **Board**

-   **Compact and Flexible Proven Design**
    -   Only 85mm x 55mm
    -   Fully Populated and Tested
-   **All connections are Low Voltage, for Safe Installation and Operation**
-   **Easy to interface and Program**
-   **Powerful Radio or Ethernet Connections to IoT Network**
-   **Board Jumpers for Easy 'No Solder' Configuration**

#### **Devices and Interfaces**

-   **Espressif ESP32 C6**
    -   ESP32-C6-MINI-1U-N4 (MHF3 Antenna Connector)
    -   32-bit RISC-V MCU
    -   Single Core MCU up to 160 MHz
    -   Wi-Fi 6 | 2.4 GHz | 802.11ax | 802.11b/g/n
    -   Bluetooth | LE | 5.3 | Mesh
    -   Thread 1.4 | 802.15.4 | Mesh
    -   Zigbee 3.0 | 802.15.4 | Mesh
    -   4 MB Flash
    -   3 UARTS
    -   I2C
    -   SPI (2xCS)
    -   USB (Type C)
    -   ADC
    -   User LED
-   **TYPE C Interfaces (x2)**
    -   ESP32-C6 USB | JTAG | Debugging | 5V Supply
    -   UART (esp32-c6-devkitm-1 compatible) | CH343P | Serial Monitor | 5V Supply
-   **ATM90E36A Energy Monitor**
    -   Enhanced Poly-Phase Energy Metering IC
    -   High-Performance Wide-Span
    -   High Accuracy
        -   ±0.1% for Active Energy
        -   ±0.2% for Reactive Energy
    -   AC 50Hz-60Hz Energy Monitor
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
        -   (Examples Bell Transformer, GreenBrook DAT01A or TLC TC TR7)
    -   DMA Mode (Logging via SPI)
    -   SPI Mode
    -   Split Phase (1+3 Typically USA)
    -   Power Modes Settings
    -   ATM Status, Active, Re-Active, Fundamental, Harmonic, LEDs
    -   Warning/IRQ Monitor
    -   Single Point Calibration per Phase
-   **W5500 Ethernet**
    -   WIZnet Hardwired Internet Controller
    -   Integrated Full TCP/IP Stack
    -   RJ45 with Active and Link LEDs
    -   SPD/DUP LEDs
-   **RS-485 CS48520D Interface**
    -   MODBUS or Other Protocol
    -   Selectable 120R Load Resistor
    -   Up to 20Mbps (limited only by the ESP32-C6)
    -   Driver with Current Limiter and Thermal Shutdown Protection
    -   Open, Short and Idle Bus Failsafe Protection
    -   Optional Ground for Shielded Cable to Asymmetrical TVS Diode
    -   Chipanalog Half-Duplex Transceiver
    -   EN/TXD LEDs
-   **24C64 EEPROM**
    -   Memory  64kbit (8192x8)
    -   Parameter Settings
    -   Logging
    -   Meter Readings
    -   P24C64C
-   **DS3231SN Real Time Clock**
    -   Extremely Accurate Real Time Clock
    -   Battery Back Up Option
    -   INT_RTC Monitor
-   **TMP102 Temperature**
    -   Ambient PCB Temperature Sensor
    
-   **OLED I2C Interface**
    -   I2C Interface
    -   OLED I2C Connector
    -   Configurable Power Pins (Solder Pads)
-   **WS2812B RGB LED**
    -   RGB Status LED
    -   Associated LM2775 5V SMPS
-   **AP63203 Onboard SMPS Board Power Supply**
    -   5V From Either Type C USB
    -   Power taken from DC Input (4.5V to 20V)
    -   Power taken from AC Input (~12V to 18V RMS)
    -   Power LEDs
-   **Tactile Buttons**
    -   Reset Button
    -   User Programmable Button / PGM

### **Calibration**

The ATM90 series of monitor devices are proven for accuracy. The **EPEM** core is the [**ATM90E36A**](https://www.microchip.com/en-us/product/atm90e36a) which is used around the world in power monitoring instruments that also need to measure voltage, current, THD, DFT and mean power.

The **EPEM** board requires little, to no, calibration and is easy to setup. You can even use ATM90E32 code for the standard functionality.

As the **EPEM** board only safely sample the AC via a low voltage transformer, you are able to monitor varying AC (RMS) voltage mains around the World, such as:

-   USA and Canada which typically is 120 V or 230 V @ 60 Hz
    -   With ranges around 114 V to 126 V
-   Europe and UK which typically is 230 V @ 50 Hz
    -   With ranges around 216 V to 253 V
-   Japan which typically is 100 V @ 50 Hz or 60 Hz
    -   Eastern Japan is 50 Hz, and Western Japan is 60 Hz
-   Should you wish to know more, see this [Wiki](https://en.wikipedia.org/wiki/Mains_electricity_by_country)

### **CT Clamps and Rogowski Current Transformer**

**EPEM** was designed primarily for the standard CT clamp such as the YHDC SCT-013 100A-50mA. Inputs for each channel 1, 2, 3, plus N are via standard 3.5mm jack sockets.

You can also use Rogowski Current Transformers on any inputs. Each input is configurable using a jumper.

### **Mains Monitoring**

All my boards are designed to be safe and easy to use - with NO live working, mains electricity parts or dangerous exposed high voltages.

The **EPEM** uses only low voltage 12 V AC such as from a bell transformer and clip on CT Clamps.

#### **Electrical Circuit Types**

The **EPEM** is capable of monitoring a number of variations in energy monitoring systems such as:

-   Accurately monitors _both_ Import and Export
-   Single phase (Home, Office etc.)
-   Multiple Single Phases. (Different circuits or home/office rings, Inverters etc.)
-   Dual Phase (2 x Live -Typically for USA. i.e. 2 x 110V)
-   Three Phase Star (3 x Live and 1 x Neutral)
-   Three Phase Delta (3 x Live)

### **Data Output**

Data provided from the **EPEM** can be extrapolated through your code in any way you wish, in order to easily integrate with your IoT Home Automation system, or Solar Installation monitor.

All software and code is open source, community based and allows you to develop and integrate as you wish.

The main process typically used for publishing data (internally or externally), is via MQTT, or you could use other direct publishing routes such as to Domoticz, Zabbix or Home Assistant.

#### Data from the ATM90E36 includes:

-   Mains RMS Voltage (each phase)
-   Mains Frequency (each phase)
-   Import and Export Values
    -   Mains RMS Current
    -   Calculated RMS Power
    -   Active Power (Absorbed or Used by the Load)
    -   Calculated Total Active Power
    -   Re-Active Power
    -   Calculated Total Reactive Power
    -   Apparent Power (Total Amount of Power Flowing from Source to Load)
    -   Calculated Total Apparent Power
    -   Fundamental Power
    -   Harmonic Power
    -   Power Factor
    -   Phase Angle

### **Programming and Development**

Development is primarily supported through Visual Studio Code (VSCode) and Arduino IDE. This also includes platforms like ESPHome.

A range of libraries are already available which support the devices on the **EPEM** such as the:

-   ESP32-C6
-   M90E36
-   W5500
-   24C64
-   DS3231
-   TMP102
-   WS2812

Code support is available in my existing [GitHub](https://github.com/DitroniX) IPEM and FLiX repositories, with a specific core EPEM [code](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/tree/main/Code) repository being provided as the board evolves, together with a new [FLeX](https://github.com/DitroniX/FLEX-DitroniX-Power-Energy-Monitor-Firmware) test and bring up firmware, which is in development.

This is a worldwide community project and is able to be integrated in so many systems.



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


# FleX - DitroniX Power Energy Monitor Firmware

## FLeX Development Code Variants

The initial release is **FLEX Lite.** and it's purpose is to test the EPEM board main GPIO's and ATM90E36, on reading data and outputting.

All EPEM boards are tested using this firmware, in order to have a baseline test prior to shipping.   

Powering up the board, will see LEDs flashing and the display, if connected, display the test text.

[Example Video](https://www.youtube.com/shorts/nOvNBkNqQBc)

### FleX [ATM90E36]

 - **[EPEM Ethernet Power Energy Monitor](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor)** 

This code remains in Development as a means of Test and Bring-up Code.   This accompanies the various Arduino code examples, which can be found [here](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/tree/main/Code/ArduinoIDE).

## **Test Setup**

The test setup is an EPEM board, either with, or without an OLED display plugged in.

Connect three CT Clamps, Input 1-3.  Placed on a single phase. known resistive load live connection.  The test baseline is using the YHDC SCT013000 100A / 50mA

My test lab setup uses 300W Resistive loads and an DAT01 Mains transformer, using the 12V AC output.

The Single Phase jumpers are inserted, so the 12V AC voltage on input one, is also connected to input 2 and 3.

## **Jumpers**

It is important to ensure the 2mm jumpers are inserted as shown below.

![EPEM E36 Jumpers](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/blob/main/Datasheets%20and%20Information/EPEM%20E36%20Jumpers.jpg)

![EPEM Board Bottom](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor/blob/main/Datasheets%20and%20Information/EPEM%20E36%20Bottom.jpg)


## **The Code**

The code is developed using PlatformIO.  

Launch PlatformIO and open the Flex Lite folder.


## **Options**

In the FLeX Lite Code, you have some initial test options.

**Switches**

 - Hardware.h
	 - **EnableBasicLoop**
	 *- Set to true to display, in Serial Monitor, loop readings and displaying only one per reset cycle. Default false.*
	 - **EnableDisplayBoardConfiguration**
	 *- Set to true to display, in Serial Monitor, board software configuration Information if DisplayFull is true. Default true.*
	 - **EnableOLEDLoop**
	 *- Set to true to enable OLED Display in Loop. Over-ride via I2C Scan. Check OLED Instance below, for OLED Selection.* Default true.

**OLED**
If you connect an OLED Display, the firmware should automatically detect this in the I2C Scan and enable the OLED 'driver'.

**Current Clamp Inputs**
By Default the EPEM is setup for standard YHDC SCT013000 100A / 50mA


## SDK Boards

 - The **[EPEM](https://github.com/DitroniX/EPEM-Ethernet-Power-Energy-Monitor)** Development Code contains a number of features, which may be either used purely for development, a basis for your own firmware, or with features enabled (via software switches), provide a complete firmware solution, publishing to the outside World.

This firmware has evolved over time and although originally was purely for bring-up testing, it has expanded into much more. It has developed way past its original bring-up task and is now quite comprehensive.  It is though, still work in progress and free to be adapted and used.

This test code is OPEN SOURCE and formatted for easier viewing. It may be freely used, or modified as needed. You may use as is, use for bring-up development, simply use the bits you like! final use and for full IoT publishing. It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

## **FLeX Updates**
  - 251022 - Initial 251022 Release for EPEM ATM90E36

**Continual Development**

Due to the nature of this firmware, all variants of FLeX remain under development and is subject to change, improvements and additions.

During any development, the firmware is carefully tested on the range of supported boards. However, should you notice a gremlin, please feel free to let us know!

**Acknowledgements**

Base 90E36 register formulation is based on the original excellent ground work from Tisham Dhar, whatnick \| ATM90E36 Energy Monitor Core. This then being adapted over time where needed.

VSCode / Platform IO base, ATM90E36 Registers/Code Merged, Adapted, Updated, Software Logic/Routines, Bring Up Firmware, Comprehensive Functions and Wi-Fi integration such as OTA, Domoticz Integration etc. by Date Williams.

## **Further Information**

Additional information, and other technical details on this project, maybe found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE and PlatformIO)*
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

Life is one long exciting learning curve, help others by setting the seed to knowledge.

![DitroniX Supporting STEM](https://hackster.imgix.net/uploads/attachments/1606838/stem_ambassador_-_100_volunteer_badge_edxfxlrfbc1_bjdqharfoe1_xbqi2KUcri.png?auto=compress%2Cformat&w=540&fit=max)




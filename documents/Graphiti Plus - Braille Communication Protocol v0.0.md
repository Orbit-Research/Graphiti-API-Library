# GRAPHITI PLUS

# BRAILLE COMMUNICATION PROTOCOL


## Table of Contents

1. [Introduction](#1-introduction)
2. [USB VCP and Bluetooth SPP Protocol](#2-usb-vcp-and-bluetooth-spp-protocol)
   - 2.1 [Data Transfer Direction: PC to BD (OUT reports)](#21-data-transfer-direction-pc-to-bd-out-reports)
   - 2.2 [Data Transfer Direction: BD to PC (IN Reports)](#22-data-transfer-direction-bd-to-pc-in-reports)
3. [USB HID Protocol](#3-usb-hid-protocol)
   - 3.1 [Transfer-direction: PC to BD (OUT reports)](#31-transfer-direction-pc-to-bd-out-reports)
   - 3.2 [Transfer-direction: BD to PC (IN Reports)](#32-transfer-direction-bd-to-pc-in-reports)
4. [Configuration table for Graphiti Plus](#4-configuration-table-for-graphiti-plus)

---

## 1 Introduction

This document describes the BAUM communication protocol used by the Graphiti Plusdevice for Braille display functionality.

Graphiti Plus supports Braille communication over both USB and Bluetooth. Over USB, it supports HID and VCP classes, while over Bluetooth, it supports the SPP profile. The USB VCP and Bluetooth SPP interfaces use the same protocol. The USB HID protocol differs slightly different.

When connected to a PC via USB, Graphiti Plus creates three HID interfaces and one VCP interface. When Braille communication operates over USB HID, it uses HID Interface 0. When Braille communication operates over USB VCP, HID Interface 0 is not used.

Note: HID Interface 1 is used for the Graphics API, and HID Interface 3 is used for graphics touch functionality, which acts as a mouse pointer.

Currently, the Graphiti Plus Braille display operates similarly to the one of our product Orbit Reader 40. Therefore, when the device is discovered via Bluetooth or queried using the Device ID command, it is identified with the name "Orbit Reader 40".

This protocol can be used for writing the data on Braille Display and get the key press data from the Braille device. There is no functionality to readback the Braille display data.

In this document, the term BD refers to the Graphiti Plus device, and PC refers to an external device such as a personal computer or any system capable of communication.

---

## 2 USB VCP and Bluetooth SPP Protocol

The `<ESC>` character (`0x1B`) marks the beginning of an information block. If an `<ESC>` character appears within the data, it is transmitted twice and interpreted as a single `<ESC>` character at the receiver.

The following information types are defined in the protocol:

### 2.1 Data Transfer Direction: PC to BD (OUT reports)

#### 2.1.1 $01 Display-data

This command is used to update the Braille display. It allows the entire Braille display content to be refreshed in a single operation. There is no provision to update an individual cell; the complete display data must be sent each time an update is required.

Here, bytes 0 to 39 represent data in Braille format. The input data must first be converted into Braille format and then transmitted to the Braille display using this command.

```
ESC
$01
Byte 0
Byte 1
Byte 2
…
…
Byte 39
```

Position of the display-elements: 0 39

Bit 0-7 of Byte data displays on Braille cells as per following:

```
0  3
1  4
2  5
6  7
```

#### 2.1.2 $05 Call major firmware version-number

This command is used to receive the major firmware version from the device. To get full version number please check the $85 information type.

```
ESC
$05
```

#### 2.1.3 $08 Repeat all

This command is used to receive all buttons status (button is pressed/not pressed). BD send the Routing keys, Braille keys, Display keys and Joystick keys status on this command.

```
ESC
$08
```

#### 2.1.4 $15 Protocol on/off

If the protocol is turned on, the device enters in communication mode and BD send the device ID, the device serial number and the length of the braille display.

```
ESC
$15
Data 0 = Off / 1 = On
```

#### 2.1.5 $16 Get Communication Channel

This report is used to get the current communication channel (HID or VCP orBluetooth). Data must be $FF to receive the current channel.

```
ESC
$16
Data $FF – query for the current setting
```

#### 2.1.6 $84 Call Device ID

This report calls the device-ID. BD send the device name as "Orbit Reader 40" on this command.

```
ESC
$84
```

#### 2.1.7 $8A Call Serial-Number

This report calls the serial number of the device.

```
ESC
$8A
```

#### 2.1.8 $8C Call Bluetooth Device Name

This report calls the Bluetooth Device Name. BD send the device name as "Orbit Reader 40" on this command.

```
ESC
$8C
```

---

### 2.2 Data Transfer Direction: BD to PC (IN Reports)

#### 2.2.1 $01 Number of cells

BD sends the number of cells in the braille display with this command byte.

```
ESC
$01
Number of cells e.g. 40
```

#### 2.2.2 $15 Protocol on/off

If the protocol is turned on, the device enters in communication mode and sends the device ID($84), the serial number($8A) and the Braille display length ($01).

```
ESC
$15
Data 0 = Off / 1 = On
If Data=1 Sends Device ID, Serial number and display length
```

#### 2.2.3 $16 Communication-Channel USB, HID or BT

This report will be sent if the report parameter received from PC is $FF.

```
ESC
$16
Data $00 – USB VCP
     $01 – Bluetooth
     $03 – HID
```

#### 2.2.4 $22 Cursor Routing Keys

This report is sent when a cursor routing key is pressed on the Braille display.

CR = Cursor routing

```
ESC
$22
Byte 0
Byte 1
Byte 2
Byte 3
Byte 4
```

Each of the 5 bytes represents 8 Cursor routing, giving a total of 40 bits one bit per One Cursor routing. If a bit is set, it means the cursor routing key for that cell was pressed.

| Byte   | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
|--------|-------|-------|-------|-------|-------|-------|-------|-------|
| Byte 0 | CR 8  | CR 7  | CR 6  | CR 5  | CR 4  | CR 3  | CR 2  | CR 1  |
| Byte 1 | CR 16 | CR 15 | CR 14 | CR 13 | CR 12 | CR 11 | CR 10 | CR 9  |
| Byte 2 | CR 24 | CR 23 | CR 22 | CR 21 | CR 20 | CR 19 | CR 18 | CR 17 |
| Byte 3 | CR 32 | CR 31 | CR 30 | CR 29 | CR 28 | CR 27 | CR 26 | CR 25 |
| Byte 4 | CR 40 | CR 39 | CR 38 | CR 37 | CR 36 | CR 35 | CR 34 | CR 33 |

#### 2.2.5 $24 Display Keys / Panning Keys

This report is sent when the panning keys, located on both sides of the Braille display, are pressed.

```
ESC
$24
Byte
```

| Byte   | bit7 | bit6 | bit5 | Bit4 | bit3 | bit2 | bit1 | bit0 |
|--------|------|------|------|------|------|------|------|------|
| Byte 0 | 0    | 0    | PF   | 0    | 0    | PB   | 0    |      |

PB = Left/right Panning Back  PF = Left/right Panning Front

#### 2.2.6 $33 Braille Keys

This report is sent when single or multiple Braille Dot keys or Space key are pressed. Here, B1-B8 is Dot 1-8 keys. B9 is Space key.

```
ESC
$33
Byte1
Byte2
```

| Byte   | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|--------|-------|-------|-------|-------|-------|-------|-------|-------|
| Byte 1 | 0     | 0     | 0     | 0     | 0     | 0     | 0     | B9    |
| Byte 2 | B8    | B7    | B6    | B5    | B4    | B3    | B2    | B1    |

Positions of braille keys on the keypad:

```
B7 – B3 – B2 – B1    B4 – B5 – B6 – B8
                  B9
```

#### 2.2.7 $34 Joystick Keys / Arrow Keys

This report is sent when any arrow key or Select key is pressed.

```
ESC
$34
Byte
```

| Byte   | Bit 7 | Bit 6 | Bit 5  | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|--------|-------|-------|--------|-------|-------|-------|-------|-------|
| Byte 0 | 0     | 0     | Select | Right | Down  | Left  | Up    |       |

Positions of Joystick keys on the keypad:

```
        Up
Left  Select  Right
       Down
```

#### 2.2.8 $84 Device ID

This report is sent once, if the communication protocol is started. It is also sent, as an answer to the report $84 (Call Device-ID). The 16 bytes following the info type represent the Device-ID in ASCII. If the name has less than 16 characters, it will be filled with 0 (ASCII 0 character). For example, Device-ID is: "Orbit Reader 40".

```
ESC
$84  Info type
Byte 1
Byte 2                  Device name as 16 ASCII bytes (padded with 0 at
Byte 3                  the end for the unused characters)
.
.
.
.
Byte 15
Byte 16
```

#### 2.2.9 $8A Serial Number

This report is sent once, if the protocol is started. It is also sent if info type $8A (Call Serial Number) is received. The 8 bytes following the info type are the serial number as 8 ASCII characters.

```
ESC
$8A
Byte1
Byte2
...
Byte7
Byte8
```

#### 2.2.10 $8C Bluetooth Device Name

This report sends the Device Bluetooth Name. The name will be padded with 0 at the end for unused characters. Bluetooth Device Name is: "Orbit Reader 40 xxxx" (xxxx is the last four digits of the unit serial number)

```
ESC
$8C  Info type
Byte 1
Byte 2                  Bluetooth device name as 20 ASCII bytes
Byte 3                  (padded with 0 at the end for unused characters)
.
.
.
Byte 19
Byte 20
```

---

## 3 USB HID Protocol

The communication between the device and PC is made using USB HID reports.

### 3.1 Transfer-direction: PC to BD (OUT reports)

#### 3.1.1 $01 Display-data

This command is used to update the Braille display. It allows the entire Braille display content to be refreshed in a single operation. There is no provision to update an individual cell; the complete display data must be sent each time an update is required.

Here, bytes 0 to 39 represent data in Braille format. The input data must first be converted into Braille format and then transmitted to the Braille display using this command.

```
$01
Byte 0
Byte 1
Byte 2
…
…
Byte 39
```

Position of the display-elements: 0 39

Bit 0-7 of Byte data displays on Braille cells as per following:

```
0  3
1  4
2  5
6  7
```

#### 3.1.2 $05 Call major firmware version-number

This command is used to receive the major firmware version from the device. To get full version number please check the $85 information type.

```
$05
Any value
```

#### 3.1.3 $02 Info request

This report is used to get the device information. Device send the reports $84, $8A and $01 on this command.

```
$02
Value 0
```

#### 3.1.4 $08 Repeat all

This command is used to receive all buttons status (button is pressed/not pressed). BD send the Routing keys, Braille keys, Display keys and Joystick keys status on this command.

```
$08
Any value
```

#### 3.1.5 $15 Protocol on/off

If the protocol is turned on, the device enters in communication mode and BD send the device ID, the device serial number and the length of the braille display.

```
$15
Data 0 = Off / 1 = On
```

#### 3.1.6 $16 Get Communication Channel

This report is used to get the current communication channel (HID or VCP or Bluetooth). Data must be $FF to receive the current channel.

```
$16
Data $FF – query for the current setting
```

#### 3.1.7 $84 Call Device ID

This report calls the device-ID. BD send the device name as "Orbit Reader 40" on this command.

```
$84
Any value
```

#### 3.1.8 $8A Call Serial-Number

This report calls the serial number of the device.

```
$8A
Any value
```

#### 3.1.9 $8C Call Bluetooth Device Name

This report calls the Bluetooth Device Name. BD send the device name as "Orbit Reader 40" on this command.

```
$8C
Any value
```

---

### 3.2 Transfer-direction: BD to PC (IN Reports)

#### 3.2.1 $01 Number of cells

BD sends the number of cells in the braille display with this command byte.

```
$01
Number of cells e.g. 40
```

#### 3.2.2 $05 Major Version number

BD sends the Major version number from the full version on this command.

```
$05
Major Version
```

#### 3.2.3 $15 Protocol on/off

If the protocol is turned on, the device enters in communication mode and sends the device ID ($84), the serial number ($8A) and the Braille display length ($01).

```
$15
Data 0 = Off / 1 = On
If Data=1 Sends Device ID, Serial number and display length
```

#### 3.2.4 $16 Communication-Channel USB, HID or BT

This report will be sent if the report parameter received from PC is $FF.

```
$16
Data $00 – USB VCP
     $01 – Bluetooth
     $03 – HID
```

#### 3.2.5 $22 Cursor routing keys

This report is sent when a cursor routing key is pressed on the Braille display.

CR = Cursor routing

```
$22
Byte 0
Byte 1
Byte 2
Byte 3
Byte 4
```

Each of the 5 bytes represents 8 Cursor routing, giving a total of 40 bits one bit per One Cursor routing. If a bit is set, it means the cursor routing key for that cell was pressed.

| Byte   | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
|--------|-------|-------|-------|-------|-------|-------|-------|-------|
| Byte 0 | CR 8  | CR 7  | CR 6  | CR 5  | CR 4  | CR 3  | CR 2  | CR 1  |
| Byte 1 | CR 16 | CR 15 | CR 14 | CR 13 | CR 12 | CR 11 | CR 10 | CR 9  |
| Byte 2 | CR 24 | CR 23 | CR 22 | CR 21 | CR 20 | CR 19 | CR 18 | CR 17 |
| Byte 3 | CR 32 | CR 31 | CR 30 | CR 29 | CR 28 | CR 27 | CR 26 | CR 25 |
| Byte 4 | CR 40 | CR 39 | CR 38 | CR 37 | CR 36 | CR 35 | CR 34 | CR 33 |

#### 3.2.6 $24 Display Keys / Panning Keys

This report is sent when the panning keys, located on both sides of the Braille display, are pressed.

```
$24
Byte
```

| Byte   | bit7 | bit6 | bit5 | Bit4 | bit3 | bit2 | bit1 | bit0 |
|--------|------|------|------|------|------|------|------|------|
| Byte 0 | 0    | 0    | PF   | 0    | 0    | PB   | 0    |      |

PB = Left/right Panning Back  PF = Left/right Panning Front

#### 3.2.7 $33 Braille Keys

This report is sent when single or multiple Braille Dot keys or Space key are pressed. Here, B1-B8 is Dot 1-8 keys. B9 is Space key.

```
$33
Byte1
Byte2
```

| Byte   | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|--------|-------|-------|-------|-------|-------|-------|-------|-------|
| Byte 1 | 0     | 0     | 0     | 0     | 0     | 0     | 0     | B9    |
| Byte 2 | B8    | B7    | B6    | B5    | B4    | B3    | B2    | B1    |

Positions of braille keys on the keypad:

```
B7 – B3 – B2 – B1    B4 – B5 – B6 – B8
                  B9
```

#### 3.2.8 $34 Joystick Keys / Arrow Keys

This report is sent when any arrow key or Select key is pressed.

```
$34
Byte
```

| Byte   | Bit 7 | Bit 6 | Bit 5  | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|--------|-------|-------|--------|-------|-------|-------|-------|-------|
| Byte 0 | 0     | 0     | Select | Right | Down  | Left  | Up    |       |

Positions of Joystick keys on the keypad:

```
        Up
Left  Select  Right
       Down
```

#### 3.2.9 $84 Device ID

This report is sent once, if the communication protocol is started. It is also sent, as an answer to the report $84 (Call Device-ID). The 16 bytes following the info type represent the Device-ID in ASCII. If the name has less than 16 characters, it will be filled with 0 (ASCII 0 character). For example, Device-ID is: "Orbit Reader 40".

```
$84  Info type
Byte 1
Byte 2                  Device name as 16 ASCII bytes (padded with 0 at
Byte 3                  the end for the unused characters)
.
.
.
.
Byte 15
Byte 16
```

#### 3.2.10 $8A Serial Number

This report is sent once, if the protocol is started. It is also sent if info type $8A (Call Serial Number) is received. The 8 bytes following the info type are the serial number as 8 ASCII characters.

```
$8A
Byte1
Byte2
...
Byte7
Byte8
```

#### 3.2.11 $8C Bluetooth Device Name

This report sends the Device Bluetooth Name. The name will be padded with 0 at the end for unused characters. Bluetooth Device Name is: "Orbit Reader 40 xxxx" (xxxx is the last four digits of the unit serial number)

```
$8C  Info type
Byte 1
Byte 2                  Bluetooth device name as 20 ASCII bytes
Byte 3                  (padded with 0 at the end for unused characters)
.
.
.
Byte 19
Byte 20
```

---

## 4 Configuration table for Graphiti Plus

| #  | Field                                          | Value                                                                                                          |
|----|------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| 1  | Product Name                                   | Graphiti Plus                                                                                                  |
| 2  | USB Vendor ID (VID) (for USB HID and VCP)      | 0x0483                                                                                                         |
| 3  | USB Product ID (PID) (for USB HID and VCP)     | 0xA366                                                                                                         |
| 4  | Interface Number (for USB HID)                 | 0                                                                                                              |
| 5  | Bluetooth Name                                 | Orbit Reader 40 xxxx (xxxx is the last four digits of the unit serial number and so it varies from unit to unit) |
| 6  | Size of the display                            | 40 cells                                                                                                       |
| 7  | Communication modes supported                  | USB HID, USB VCP (Serial), Bluetooth (SPP profile)                                                             |

---

## Revision History
 
| Rev | Date        | Description of Changes | Author |
|-----|-------------|------------------------|--------|
| 0.0 | 01-Apr-2026 | Created                | Vishal |
 
---
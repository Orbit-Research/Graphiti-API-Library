# GRAPHITI PLUS

# API mode APP NOTE

---

## Table of Contents

- [Overview](#overview)
- [1. Start Graphic Display](#1-start-graphic-display)
  - [1.1 Enter API Mode](#11-enter-api-mode)
  - [1.2 Connect via Graphiti API Library](#12-connect-via-graphiti-api-library)
  - [1.3 Example Code (connection_HID.cpp)](#13-example-code-connection_hidcpp)
- [2. Start Braille Display API](#2-start-braille-display-api)
  - [2.1 Select Braille Protocol Mode](#21-select-braille-protocol-mode)
  - [2.2 USB Connection Parameters for Braille Display](#22-usb-connection-parameters-for-braille-display)
  - [2.3 Graphiti Plus Keys enable for API Mode](#23-graphiti-plus-keys-enable-for-api-mode)
- [3. Combination of Graphiti API and Braille Display API](#3-combination-of-graphiti-api-and-braille-display-api)
- [4. Quick Reference - All Key Combinations](#4-quick-reference---all-key-combinations)
- [5. USB Interface Summary](#5-usb-interface-summary)

---

## Overview

This document explains how to communicate with the Graphiti Plus device using its Graphic Display and Braille Display interfaces. It covers HID mode setup, the Graphiti API Library, and combined display operation using key combinations.

The Graphiti Plus supports multiple communication modes:
- Graphic Display via HID mode (API mode)
- Braille Display via Baum Protocol over HID/VCP

---

## 1. Start Graphic Display

### 1.1 Enter API Mode

Before the Graphiti Plus can communicate with the host computer, you must put the device into HID mode (also called API mode).

Key Combination to Enter HID Mode:

`Space + Dot 7` - Activates HID / API mode on the Graphiti Plus device

### 1.2 Connect via Graphiti API Library

Once the device is in HID mode, run the Graphiti API Library to establish communication. The library connects over USB using the following identifiers:

| Parameter        | Value                                               |
|------------------|-----------------------------------------------------|
| Vendor ID (VID)  | 0x0483                                              |
| Product ID (PID) | 0xA366                                              |
| Interface Number | 1 (interface_number == 1 or path contains mi_01)    |

### 1.3 Example Code (connection_HID.cpp)

The following code is from the Graphiti API Library file connection_HID.cpp. It shows how the library opens a connection to the Graphic Display interface:

```cpp
bool GraphitiConnectionHID::open() {
  if (device_) return true;
  // Graphiti Plus connection
  struct hid_device_info* devs_new =
  hid_enumerate(0x0483, 0xA366);
  struct hid_device_info* cur_new = devs_new;
  std::string chosen_path_str_new;

  while (cur_new) {
    if ((cur_new->interface_number == 1) ||
        (cur_new->path &&
        strstr(cur_new->path, "mi_01") != nullptr)) {
      chosen_path_str_new = cur_new->path;
      break;
    }
    cur_new = cur_new->next;
  }
}
```

The code enumerates all HID devices with VID 0x0483 and PID 0xA366, then selects the one on interface 1 (identified by interface_number == 1 or path containing mi_01). This is the Graphic Display interface.

---

## 2. Start Braille Display API

### 2.1 Select Braille Protocol Mode

Choose your preferred Braille protocol:

| Key Combination                  | Action / Description                               |
|----------------------------------|----------------------------------------------------|
| Left Pan Up + Cursor Routing 2   | Baum Protocol with HID mode (USB HID interface)    |
| Left Pan Up + Cursor Routing 4   | Baum Protocol with VCP mode (Virtual COM Port)     |

### 2.2 USB Connection Parameters for Braille Display

The Braille Display uses the same USB device (VID/PID) as the Graphic Display but connects on a different interface:

| Parameter        | Value                                               |
|------------------|-----------------------------------------------------|
| Vendor ID (VID)  | 0x0483                                              |
| Product ID (PID) | 0xA366                                              |
| Interface Number | 0 (interface_number == 0 or path contains mi_00)    |

To use the Braille Display, modify the connection code to look for interface_number == 0 and mi_00 instead of mi_01. This selects the Braille/Baum protocol interface rather than the Graphic Display interface.

The Graphiti Plus supports running both the Graphic Display and Braille Display at the same time.

### 2.3 Graphiti Plus Keys enable for API Mode

Press the following key to enable Graphiti Plus key handling for API control:

`Right Pan Up + Cursor Routing 39` - Enable Graphiti Plus key handling for API mode

---

## 3. Combination of Graphiti API and Braille Display API

Once both displays are configured,
- The Graphic Display will be controlled via the Graphiti API (HID)
- The Braille Display will operate via Braille API / Baum Protocol (HID or VCP)
- Graphiti Plus keys can be used for Graphic API.

---

## 4. Quick Reference - All Key Combinations

| Key Combination                  | Action / Description                                        |
|----------------------------------|-------------------------------------------------------------|
| Space + Dot 7                    | Enter HID / API mode (required first step for all modes)    |
| Left Pan Up + Cursor Routing 2   | Enable Baum Protocol with HID mode (Braille Display)        |
| Left Pan Up + Cursor Routing 4   | Enable Baum Protocol with VCP mode (Braille Display)        |
| Right Pan Up + Cursor Routing 39 | Disable Graphiti Plus keys & switch to API control          |

---

## 5. USB Interface Summary

| Display         | VID    | PID    | Interface | Path Identifier |
|-----------------|--------|--------|-----------|-----------------|
| Graphic Display | 0x0483 | 0xA366 | 1         | mi_01           |
| Braille Display | 0x0483 | 0xA366 | 0         | mi_00           |

---

## Revision History
 
| Rev | Date        | Description of Changes | Author   |
|-----|-------------|------------------------|----------|
| 0.0 | 01-Apr-2026 | Created                | Himanshu |
 
---
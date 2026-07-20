# GRAPHITI PLUS

# API mode APP NOTE

---

## Table of Contents

- [Overview](#overview)
- [1. Start Graphic Display](#1-start-graphic-display)
  - [1.1 Enter API Mode](#11-enter-api-mode)
  - [1.2 Connect via Graphiti API Library](#12-connect-via-graphiti-api-library)
  - [1.3 Example Code (connection_HID.cpp)](#13-example-code-connection_hidcpp)
  - [1.4 Graphiti Plus Keys enable for API Mode](#14-graphiti-plus-keys-enable-for-api-mode)
- [2. Quick Reference - All Key Combinations](#2-quick-reference---all-key-combinations)
- [3. USB Interface Summary](#3-usb-interface-summary)
- [Revision History](#revision-history)
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

### 1.4 Graphiti Plus Keys enable for API Mode

send the API command "Set Key Event" (5.4.1) to enable or disable key events for API mode.
After this, you can get the key events as per the "Key Events for Graphiti Plus" (5.4.3).

---

## 2. Quick Reference - All Key Combinations

| Key Combination                  | Action / Description                                        |
|----------------------------------|-------------------------------------------------------------|
| Space + Dot 7                    | Enter HID / API mode (required first step for all modes)    |

---

## 3. USB Interface Summary

| Display         | VID    | PID    | Interface | Path Identifier |
|-----------------|--------|--------|-----------|-----------------|
| Graphic Display | 0x0483 | 0xA366 | 1         | mi_01           |

---

## Revision History
 
| Rev | Date        | Description of Changes | Author   |
|-----|-------------|------------------------|----------|
| 0.0 | 01-Apr-2026 | Created                | Himanshu |
| 0.1 | 12-May-2026 | Remove Braille API usage (Braille Display API added in Graphic API) | Vishal |
 
---
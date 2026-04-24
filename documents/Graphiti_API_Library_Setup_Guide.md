# Graphiti API Library — Setup Guide

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Purpose and Definition of Work](#2-purpose-and-definition-of-work)
3. [Setting Up the Device](#3-setting-up-the-device)
4. [Prerequisite Software](#4-prerequisite-software)
5. [Setting Up Your Coding Environment](#5-setting-up-your-coding-environment)
6. [Writing a C++ Project](#6-writing-a-c-project)

---

## 1. Introduction

The **Graphiti API Library** is a software development toolkit designed to interact with the **Graphiti** and **Graphiti Plus** devices by Orbit Research. The library provides developers with a comprehensive set of functions to access and control the devices, enabling communication through **Serial**, **HID**, and **Draw** modes.

Supported programming languages:
- C++
- Python
- Java

This document serves as a setup guide and reference for integrating the Graphiti API into development environments — covering prerequisite installation, environment configuration, and application development using the API.

---

## 2. Purpose and Definition of Work

### 2.1 Purpose

The primary purpose is to provide developers with a clear and structured approach to integrating the Graphiti and Graphiti Plus devices into software applications. The Graphiti API library abstracts the complexities of device communication, allowing developers to focus on building functional and interactive applications without worrying about low-level device handling.

### 2.2 Definition of Work

This project covers:

1. Installation of prerequisite software — Git, Python, Java JDK, Visual C++ Build Tools, and CMake.
2. Setting up the development environment with Visual Studio Code, and configuring compiler and generator tools.
3. Using the Graphiti API to communicate with the device, handle events, and perform actions such as drawing, key input, and touch input.
4. Providing wrapper support for C++, Python, and Java, enabling developers to use the API in their preferred programming language.

---

## 3. Setting Up the Device

### 3.1 Setting Up the Graphiti Device

1. Connect your device to the Graphiti using a **USB-A to Micro USB cable** — USB-A end to your PC, Micro USB end to the Graphiti (located on the left side, closest to Dot 3).

2. Power the device on and select an interface mode using the keypad:

   | Key Command     | Mode              |
   |-----------------|-------------------|
   | Space + Dot 8   | Serial (VCP) mode |
   | Space + Dot 7   | HID mode          |
   | Space + Dot 1   | Draw mode (enables Draw API with Draw events) |

3. Open **Device Manager** from the Windows taskbar (search "device manager").

4. If you selected **Serial mode**, identify the COM port under **"Ports (COM & LPT)"** in Device Manager. Note the port number (e.g., `COM4`) for later use.

---

## 4. Prerequisite Software

### 4.1 Install Git

1. Go to [https://git-scm.com/](https://git-scm.com/)
2. Click **"Download for Windows"** — this downloads a `.exe` installer (e.g., `Git-x.y.z-64-bit.exe`).
3. Double-click the downloaded `.exe` to start installation.
4. Keep default settings for most options.
5. Click **Finish** when done.

---

### 4.2 Set Up Python Environment

This chapter specifies steps to install Python application development environment. You can skip this chapter if you will not be using Python language for application development.

1. Go to [https://www.python.org/downloads/windows/](https://www.python.org/downloads/windows/)
2. Download the appropriate installer:
   - **Windows installer (64-bit)** — recommended for most systems
   - **Windows installer (32-bit)** — for 32-bit systems
3. Double-click the downloaded file: `python-x.x.x-XXXX.exe`
4. ☑ **Check the box: "Add Python 3.x to PATH"** — this makes Python accessible from Command Prompt.
5. Choose an installation type:
   - **Install Now** *(recommended)* — installs Python with default settings.
   - **Customize Installation** — choose features/location (advanced users only).
6. Click **"Install Now"**.
7. Wait for installation to complete — you'll see a **"Setup was successful"** message.
8. Click **Close**.

---

### 4.3 Set Up Java Environment

This chapter specifies steps to install Java application development environment. You can skip this chapter if you will not be using Java SDK and language for application development.

#### Step 1: Download Java JDK

- Go to: [https://www.oracle.com/java/technologies/javase-downloads.html](https://www.oracle.com/java/technologies/javase-downloads.html)
- Click the latest JDK download link (e.g., JDK 21 or JDK 17).
- Under Windows, download the **x64 Installer `.exe`** (e.g., `jdk21_windows-x64_bin.exe`).

#### Step 2: Run the Installer

1. Double-click the downloaded `.exe` file.
2. Click **Next** to begin installation.
3. Leave the default path (e.g., `C:\Program Files\Java\jdk21`) or choose a custom one.
4. Click **Next** - wait for installation - click **Close**.

#### Step 3: Set `JAVA_HOME` and Update `PATH` *(Optional but Recommended)*

1. Open **Environment Variables**:
   - Press `Windows + S`, search **"Environment Variables"**
   - Open: **Edit the system environment variables - Environment Variables**

2. **Set `JAVA_HOME`:**
   - Click **New** under *System Variables*
   - Variable name: `JAVA_HOME`
   - Variable value: path to JDK (e.g., `C:\Program Files\Java\jdk-21`)

3. **Update `PATH`:**
   - In *System Variables*, find `Path` - click **Edit**
   - Click **New** and add: `%JAVA_HOME%\bin`
   - Click **OK** to close all dialogs.

---

### 4.4 Set Up C++ Environment

#### 4.4.1 Visual Studio Code

This chapter specifies steps to download and install Visual Studio Code for C++ application development.

1. Go to [https://code.visualstudio.com/](https://code.visualstudio.com/)
2. Click **"Download for Windows"**.
3. Open the installer: `VSCodeUserSetup.exe`
4. Follow the installation wizard:
   - Accept the license agreement.
   - Choose installation location (default recommended).
   - ☑ Select **"Add to PATH"** and **"Create Desktop Icon"**.
5. Click **Install** - click **Finish** to launch VS Code.

---

#### 4.4.2 Microsoft Visual Studio (C++ Build Tools)

This chapter specifies steps to install C++ application development environment with Microsoft Visual Studio. You can skip this chapter if you will not be using C++ language for application development.

**Option A — Existing Visual Studio Installation:**
1. Open **Visual Studio Installer** from the Windows Start menu.
2. Verify the **"Desktop development with C++"** workload is checked.
3. If not installed, check the box and click **Modify**.

**Option B — Build Tools Only (without full IDE):**
1. Go to the Visual Studio Downloads page - scroll to **"Tools for Visual Studio"** under *All Downloads*.
2. Download **Build Tools for Visual Studio 2022**:
   - Direct link: [https://aka.ms/vs/17/release/vs_BuildTools.exe](https://aka.ms/vs/17/release/vs_BuildTools.exe)
3. Run the installer - check **"Desktop development with C++"** workload - click **Install**.

---

#### 4.4.3 MSYS2 (g++ and Ninja — Required for C++23)

This section describes the steps to install the MSYS2-based C++ development environment, including g++ and Ninja. This setup is required to support the build process and ensure compatibility with the project requirements (C++23 or later).

1. Download and install MSYS2 from [https://www.msys2.org](https://www.msys2.org) — run the installer and follow instructions.

2. Open **MSYS2 MSYS** from the Start Menu.

3. Update packages:
   ```bash
   pacman -Syu
   ```
   Close the shell when prompted, then reopen and run:
   ```bash
   pacman -Su
   ```

4. Install g++ and Ninja:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-ninja
   ```

5. Add the following path to your **System or User Environment Variables → PATH**:
   ```
   C:\msys64\mingw64\bin
   ```

---

### 4.5 Install CMake

#### Step 1: Download CMake

- Go to [https://cmake.org/download/](https://cmake.org/download/)
- Download the **Windows installer (`.msi`)** from the "Latest Release" section.

#### Step 2: Install CMake

1. Double-click the downloaded `.msi` file.
2. Follow the installation wizard - click **Next**.
3. ☑ When prompted, select **"Add CMake to the system PATH for all users"** (or for current user).
4. Click **Install** - click **Finish**.

#### Step 3: Verify CMake in PATH

1. Open **Visual Studio Code**.
2. Open the built-in terminal: `View → Terminal` or `` Ctrl + ` ``
3. Run:
   ```bash
   cmake --version
   ```
4. Expected output:
   ```
   cmake version 3.29.2
   ```
   *(version number may vary)*

---

## 5. Setting Up Your Coding Environment

Each of the following steps is crucial when setting up your coding environment and using the library and cannot be skipped. Using C++23 or C++ version 23 is a requirement of this project

1. **Download the Graphiti API library** to any location under your user directory.

2. **Open the library in VS Code:**
   - Go to **File - Open Folder**
   - Select the folder where you installed the library.

3. **Install the C/C++ Extension Pack** by Microsoft from the VS Code Extensions marketplace.

4. **Install the PowerShell extension** by Microsoft from the VS Code Extensions marketplace.

5. **Open a PowerShell terminal in VS Code:**
   - Go to **Terminal - New Terminal**
   - A PowerShell window will open in the bottom panel.

6. **Allow execution of `.ps1` script files:**

   i. Open the **Explorer tab** (`Ctrl + Shift + E`) - navigate to the `scripts` folder.

   ii. Right-click `Set-ExecutionPolicy.exe` - select **"Reveal in File Explorer"**  
       *(or select the file and press `Shift + Alt + R`)*.

   iii. In File Explorer, right-click `Set-ExecutionPolicy.exe` - **"Run as administrator"**.

   iv. When prompted for permission, select **Yes**.

   v. A message box will appear: **"Execution Policy set to RemoteSigned"** — click OK or close.

   > The source of this `.exe` is in `Set-ExecutionPolicy.ps1` in the same folder.

7. **Install the library and its dependencies** using vcpkg. In the PowerShell terminal, run:
   ```powershell
   .\scripts\install_library_vcpkg.ps1
   ```

   > **If you encounter a PowerShell security/execution policy error**, run:
   > ```powershell
   > Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
   > ```
   > If you don't have permissions, open a terminal **as Administrator**, navigate to the library directory, and re-run the command.

8. **(Optional) Install using a specific generator and compiler:**

   Navigate to the `lib` folder:
   ```powershell
   cd lib
   ```

   Run with specific generator and compiler:
   ```powershell
   .\library.ps1 -Generator "Ninja" -Compiler "g++"
   ```

   Or use defaults:
   ```powershell
   .\library.ps1
   ```

9. **Add include paths** to your C/C++ configuration. Open the configuration with `Ctrl + Shift + P` → **"C/C++: Edit Configurations"**.

10. **If using Java**, add these include paths:
    ```
    ${vcpkgRoot}/x64-windows/include
    C:\Program Files\Java\jdk-20\include
    C:\Program Files\Java\jdk-20\include\win32
    ${env:USERPROFILE}\graphiti\include
    ```

11. **Your environment is now set up.** To test the C++ library, run:
    ```powershell
    .\scripts\tools.ps1 standard
    ```

12. **Language wrapper locations and READMEs:**

    | Language | Folder | README |
    |----------|--------|--------|
    | C        | `Graphiti-API-Library/binding/C` | `binding/C/README.md` |
    | Python   | `Graphiti-API-Library/binding/python` | `binding/python/README.md` |
    | Java (JNI) | `Graphiti-API-Library/binding/jgraphiti` | `binding/jgraphiti/README.md` |

    > Detailed run steps are provided in each folder's respective `README.md` file.

---

## 6. Writing a C++ Project

### Step 1: Create `CMakeLists.txt`

Create a `CMakeLists.txt` file in your project root with the following content:

```cmake
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)
add_definitions(-D_WIN32_WINNT=0x0601)

# Set vcpkg toolchain (adjust path as needed)
# Make sure vcpkg has asio
set(CMAKE_TOOLCHAIN_FILE
    "$ENV{USERPROFILE}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Set path for Graphiti library
set(CMAKE_PREFIX_PATH "$ENV{USERPROFILE}/graphiti")

# Your project
project(RunGraphiti)

# Find asio for Graphiti
find_package(asio REQUIRED)

# Find Graphiti
find_package(Graphiti REQUIRED)

# Create Executable
add_executable(main src/main.cpp)

# Link Executable to Graphiti Library
target_link_libraries(main PRIVATE
    Graphiti::Graphiti)
```

---

### Step 2: Include the Graphiti Library Header

```cpp
#include <Graphiti/Extension.hpp>
```

This single include provides access to all Graphiti API functions.

---

### Step 3: Initialize and Use the API

The general workflow for a C++ Graphiti application:

1. **Create** a `GraphitiExtension` object.
2. **Call `startUpVCP`** with your COM port and key/touch event flags.
3. **Interact** with the device using `graphiti->function(...)`.
4. **Call `shutDownVCP`** and delete the object when done.
5. **Use `graphiti->sleep(2)`** between most API calls to allow the device time to respond.

---

### Step 4: Full Example — `src/main.cpp`

```cpp
#include <cstdio>
#include <Graphiti/Extension.hpp>

GraphitiExtension* graphiti = new GraphitiExtension();

void sleepAndOutput() {
    graphiti->sleep(2);
    auto output = graphiti->getNextOutputEvent();

    if (output.has_value()) {
        std::cout << output.value() << std::endl;
    } else {
        std::cout << "No value" << std::endl;
    }
}

int main() {
    bool keyEventsBool   = false;
    bool touchEventsBool = false;

    std::cout << "Starting" << std::endl;

    graphiti = new GraphitiExtension();

    // Start up on COM4 with key and touch events disabled
    if (!graphiti->startUpVCP("COM4", keyEventsBool, touchEventsBool)) {
        return 1;
    }

    // Example API call
    graphiti->getDateAndTime();
    sleepAndOutput();

    // Shut down and clean up
    graphiti->shutDownVCP(keyEventsBool, touchEventsBool);
    delete graphiti;

    std::cout << "Ending" << std::endl;
    return 0;
}
```

> **Note:** Replace `"COM4"` with the COM port identified in [Section 3.1](#31-setting-up-the-graphiti-device). Set `keyEventsBool` and `touchEventsBool` to `true` if you want to receive key/touch events.

---

## Revision History

| Rev | Date        | Description   | Author            |
|-----|-------------|---------------|-------------------|
| 1.0 | 24-Nov-2025 | First Version | Himanshu Makwana  |
| 1.1 | 20-Mar-2026 | Update        | Himanshu Makwana  |

---
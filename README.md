# UDP Test Utility — User Guide

This document contains complete instructions for the `Config.ini` file format, runtime behavior, how to run the utility
> **Scope / Constraint:** This software is built for **Windows x64** and depends on WinSock2 (WS2_32). The executable should be compiled for the Windows x64 target.

---

## Table of contents

- [Overview](#overview)  
- [Example Config.ini](#example-configini)  
- [Parameter reference and rules](#parameter-reference-and-rules)  
- [Payload handling behavior](#payload-handling-behavior)  
- [How to run the program](#how-to-run-the-program)  

---

## Overview

The UDP Test Utility reads a configuration file containing one or more `[Connection]` sections. Each `[Connection]` block describes:

- the payload to send (or a payload length to auto-generate),  
- the destination IP (unicast or multicast),  
- the UDP port
- the sending period in milliseconds.

For each `[Connection]` block the application spawns a separate worker (background thread) that opens a UDP socket and transmits packets periodically according to the configuration.

---

## Example Config.ini

Lines starting with `#` are comments and ignored by the parser.

```ini
[Connection]
#payload_length=100
payload=adsadhiuashdiusahduiashduiashduisah
destination_ip=192.168.1.100
destination_port=5000
#sending_period rate in ms
sending_period=1000
source_port=4000
source_ip=192.168.1.110

[Connection]
payload_length=1024
destination_ip=239.0.0.99
destination_port=5000
sending_period=200

[Connection]
payload_length=652
destination_ip=229.0.0.99
destination_port=615
sending_period=1
```

## Parameter reference and rules
Each [Connection]
block may contain the following keys:

| Key              | Type     | Required | Description                                                                                     |
|------------------|----------|----------|-------------------------------------------------------------------------------------------------|
| payload          | string   | optional | Literal payload to send. If present, `payload_length` must **not** be present.                  |
| payload_length   | integer  | optional | If present (and `payload` is not), the program allocates a buffer of this size automatically.   |
| destination_ip   | string   | required | Destination IPv4 address (unicast or multicast). The parser will warn for invalid addresses.    |
| destination_port | integer  | required | UDP destination port.                                                                           |
| sending_period   | integer  | required | Delay between sends in milliseconds. Small values (0–5 ms) may saturate CPU or network.         |
| source_port   | integer  | optional | Local UDP Port     |
| source_ip   | integer  | Required only when source_port was specified. Otherwise, optional | Local IPv4 address     |


### Important Rules

- `payload` and `payload_length` must **NOT** coexist in the same `[Connection]` block.
- `sending_period` is the delay in milliseconds between sends.
- Very small `sending_period` values (e.g., 0–5 ms) may saturate CPU and network.
- `destination_ip` must be a valid IPv4 address.
- Comments: anything after a `#` on the same line is ignored.


## Payload handling behavior

 - If `payload` is specified:
    - The program sets `payload_length` = `payload.size()` automatically.
    - The literal bytes in the string are used as the packet body.


 - If `payload_length` is specified:
   - The program allocates a buffer of `payload_length` bytes.
   - The buffer is filled automatically. Default behavior in current code: `payload[i] = i & 0xFF` (sequential pattern). `i from 0 to payload_length - 1`
 - If payload_length == 0 or both payload and payload_length are missing → the connection is invalid and will not be started.

 ## How to run the program
 The executable expects a single command-line argument: the absolute path to the INI file.

 Example(Windows Command Line)
 `````
 udptestutility.exe C:\Path\To\Config.ini
 `````

 # UDP Test Utility — Developer Guide

## Table of contents
- [Overview](#overview)  
- [For Tester](#for-tester)  
- [For Developer](#for-developer)

### For Tester
A brief python script was developed to test this application. It has three tests which is described bellow
| Test Description | Acceptance criterea | Test Procedure |
|------------------| --------------------| -------------- |
| Verifies the capability of the software to read a configuration.ini file and send packets with the correct length | The received packet size is equal to payload_length defined in the configuration file | The automated test creates a default configuration.ini file specifying destination IP, port, payload_length and rate. It creates a listener socket using the same configuration, receives the data, and compares the received size with the expected value |
| Verifies the capability of the software to read a configuration.ini file and send the correct raw payload | The received payload matches exactly the payload defined in the configuration file | The automated test creates a default configuration.ini file specifying destination IP, port, payload and rate. It creates a listener socket, receives the data, and compares the received payload with the expected value |
| Verifies the capability of the software to send data to a multicast IP at a specific rate | Data is received from the multicast group at a period within ±10% tolerance of sending_period  | The automated test joins the multicast group, receives consecutive packets, computes the interval between them, and verifies that the timing stays within the acceptable tolerance |

### Requirements to run
 - Python version 3.13 (recommended)
 - udptestutility.exe at relative path of the test.py : ./x64/Debug/udptestutility.exe
    - for example
    ```
    C:.
    │   test.py
    │
    └───x64
        └───Debug
                udptestutility.exe
    ```
 - run the command in a ms windows cmd: `python test.py`

 ## For Developer

### Opening and Building the Project in Visual Studio 2022

Follow the steps below to open, configure, and build the `udptestutility` project using **Microsoft Visual Studio 2022**:

1. **Install Prerequisites**
   - Ensure that **Visual Studio 2022** is installed.
   - During installation, make sure the following workloads/components are selected:
     - **Desktop development with C++**
     - MSVC v143 or later toolchain
     - Windows SDK (10.x or 11.x)
     - C++ CMake tools for Windows (optional)

2. **Locate the Project**
   - Navigate to the directory containing the project solution file (`.sln`).
   - Example structure:
     ```
     C:\
       ProjectFolder\
           udptestutility.sln
           src\
           include\
           x64\
             Debug\
     ```

3. **Open the Solution**
   - Double-click the file:
     ```
     udptestutility.sln
     ```
   - Visual Studio 2022 will automatically load all associated projects.

4. **Select Build Configuration**
   - In the top toolbar, choose:
     - **Configuration:** `Debug`
     - **Platform:** `x64`
   
   > ⚠️ This software is intended to be compiled as **Windows x64**.  
   > Ensure that *Win32* is **not** selected.

5. **Restore Dependencies (if required)**
   - The project uses WinSock2 (Windows native).
   - If Visual Studio reports missing includes like `winsock2.h`, ensure Windows SDK is installed.
   - No external libraries or package managers are required.

6. **Build the Project**
   - From the menu:
     ```
     Build → Build Solution (Ctrl + Shift + B)
     ```
   - The compiled executable will be generated in:
     ```
     /x64/Debug/udptestutility.exe
     ```

7. **Running the Application**
   - The program expects **one command-line argument**: the absolute path to the configuration `.ini` file.
   - Example:
     ```
     udptestutility.exe C:\Config.ini
     ```
   - A project setting set the ```command-line argument``` to variable ```$(SolutionDir)Config.ini```
    When debbuging, the application is going to be started with the Config.ini at project root.

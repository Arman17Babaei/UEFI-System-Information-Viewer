# UEFI System Information Viewer
In this project, we developed a UEFI application designed to present an interactive display of system information. Leveraging UEFI services and protocols, our program retrieves and showcases comprehensive hardware and firmware details, empowering users to seamlessly inspect and interact with key system information.
## Table of contents
* [Introduction](#Introduction)
* [Week-by-Week Milestones](#Week-by-Week-Milestones)
* [Features](#Features)
* [Sample](#Sample)
* [Execution](#Execution)
## Introduction
UEFI stands for "Unified Extensible Firmware Interface." It is a standard interface specification that defines a software interface between the operating system and the system firmware (BIOS or UEFI firmware) during the boot process and the runtime of an operating system. UEFI is designed to replace the traditional BIOS (Basic Input/Output System) and offers several advantages, including support for larger hard drives, faster boot times, and improved security features.

UEFI programming refers to the development of software applications, known as UEFI applications, that run on systems with UEFI firmware. UEFI applications can perform various tasks, such as configuring hardware settings, interacting with the system firmware, and providing pre-boot utilities. UEFI programming often involves using languages such as C or C++ and making use of UEFI services and protocols to interact with the underlying firmware. UEFI programming is essential for creating applications that leverage the capabilities of modern UEFI firmware, providing a more flexible and extensible environment for system initialization and management compared to the traditional BIOS. This is particularly important in the context of modern computing systems, including PCs and servers.

### Important Definitions:
- System Table:
The system table, often referred to as the EFI System Table, is a data structure provided by the UEFI firmware. It contains information about various services and protocols available in the UEFI environment. UEFI applications can access this table to interact with firmware services and obtain critical information about the system.

- SMBIOS (System Management BIOS) Table:
SMBIOS is a standard developed by the Distributed Management Task Force (DMTF) that defines a structure (table) for conveying system information to the operating system. This information includes details about the system's hardware components, such as the processor, memory, BIOS version, and more. UEFI applications can access the SMBIOS table to retrieve and use this hardware information.

- ACPI (Advanced Configuration and Power Interface) Tables:
ACPI is an industry-standard that defines power management and configuration interfaces between the operating system and the hardware. ACPI tables contain information related to system configuration, power management, and other platform-specific details. UEFI applications can use ACPI tables to obtain information about the system's power management capabilities and configurations.

- UEFI Variables:
UEFI variables are a mechanism provided by the UEFI firmware to store and retrieve configuration data. These variables are used by both the firmware and UEFI applications to store persistent information, such as boot configuration settings, system language preferences, and other runtime data. UEFI applications can access these variables to retrieve and modify system-specific settings.

The implementation of this project consists of two main components:
### Display and Navigation Features:
- Implementing methods to display information.
- Implementing navigation, search, and filtering functionalities.
- 
### Getting the System Information:
- Utilizing UEFI functions and protocols to acquire essential system data.
- Retrieving key system information, including the system table, SMBIOS table, ACPI tables, and UEFI variables.

## Week-by-Week Milestones
Week 1: 
- Install and run Edk2
- Build a "Hello World" package.
  
Week 2:
- Familiarization with available hardware and system firmware information
- Understanding information display protocols in UEFI.
  
Week 3:
- Understanding the functions of receiving information and proficiently working with them
- Navigating through the page efficiently
  
Week 4: 
- Presenting specific information
- Transitioning the base between different pages.
  
Week 5 and 6: 
- Verifying the remaining steps for project completion.
- Conducting a more detailed planning and time allocation for each task.
- Understanding functions related to text and display to present information in an organized and aesthetically pleasing manner.
  
## Features
- Navigating between page items vertically and horizontally.
- Implementing search and filter functionality.
- Incorporating word highlighting when searching.
## Sample
## Execution
- Edit `setup_env.sh` to match your system and run `. ./setup_env.sh`.
- Use `make run` to build the package and start qemu. Then enter the following commands to run the program:
```
Shell> FS0:
Shell> MainTable.efi
```
- Press `Ctrl-A X` to terminate.

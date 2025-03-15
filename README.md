# 5DOF-VR-LiveStreaming 🚀

**Renamed from “6ixSense VR”**

This repository contains the code, hardware designs, and documentation for **5DOF-VR-LiveStreaming**, a system that enables near–six degrees of freedom (5 DoF) live video streaming using a mechanically actuated 360° camera. Users wearing a VR headset can move around and look in any direction while experiencing a real-world environment streamed in real time.

---

## Table of Contents
- [Overview](#overview)
- [Motivation](#motivation)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
  - [Mechanical Gantry](#mechanical-gantry)
  - [Motors and Controllers](#motors-and-controllers)
  - [MCU Control Platform](#mcu-control-platform)
  - [VR Headset](#vr-headset)
- [Video Streaming Workflow](#video-streaming-workflow)
- [Project Specifications](#project-specifications)
  - [Functional Specifications](#functional-specifications)
  - [Non-Functional Specifications](#non-functional-specifications)
- [Prototype Data](#prototype-data)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Setup Instructions](#setup-instructions)
- [Safety and Risk Management](#safety-and-risk-management)
- [Acknowledgements](#acknowledgements)

---

## Overview 
**5DOF-VR-LiveStreaming** is a proof-of-concept project designed to let users physically “walk around” within a 360° live stream. A mounted 360° camera moves along an **X** and **Y** rail system, corresponding to a user’s position, while the user wears a VR headset. This approach allows for a nearly complete range of motion, minus one translational axis, achieving five degrees of freedom in real time.

Key subsystems include:
1. **Mechanical Gantry** – A rail-based system that moves the 360° camera in the horizontal plane.  
2. **MCU Control** – Uses microcontrollers to receive (x, y) position data from the VR headset and adjust the camera’s position.  
3. **Video Streaming** – Captures 360° video, encodes it using low-latency protocols, and streams it to the VR headset.  
4. **VR Headset** – Tracks user movement and orientation to determine where the camera should be positioned.

---

## Motivation
Traditional 360° VR video streams only allow *rotational* freedom of movement—users can look around but cannot *walk* around. By adding a mechanical system that physically repositions the camera, **5DOF-VR-LiveStreaming** enhances immersion.  
- **Healthcare**: Potential for live, “walk-around” telepresence in remote consultations.  
- **Education**: Allows students to explore real-world laboratory setups and environments interactively.  
- **Entertainment**: Provides a more engaging VR streaming experience for events and live shows.

---

## Features
- **Real-Time Camera Control**: Stepper motors move the 360° camera to match user coordinates.  
- **Low Latency Streaming**: ~1 second end-to-end delay using RTMP/HLS-based streaming.  
- **Inside-Out Tracking**: Leverages an off-the-shelf VR headset’s built-in sensors to determine user position.  
- **Cost-Effective**: Uses readily available hardware components and open-source software libraries.

---

## System Architecture
Below is a simplified block diagram illustrating how data and signals flow:

1. **VR Headset** obtains user position (x, y).  
2. **MCU** receives these coordinates, calculates motor steps.  
3. **Motors** move the camera along the gantry.  
4. **360° Camera** streams spherical video over RTMP or HLS to a **local server**.  
5. **VR Headset** receives live stream and displays it in real time.

---

## Hardware Components

### Mechanical Gantry
- Built from **aluminum T-slot extrusions** for a stable, rigid frame.  
- Two-axis (X-Y) motion is enabled using **bearing-based sliders**.  
- The camera mount is placed at the intersection, giving it freedom of motion across the plane.

### Motors and Controllers
- **Stepper Motors (NEMA 23)**: Provide precise position control to match the user’s in-VR movement.  
- **Motor Drivers**: Receive direction/step signals from the MCU. Typically require 12V power supply.  
- **Power Supply**: Delivers stable voltage to ensure the motors and controllers operate reliably.

### MCU Control Platform
- Uses an **STM32 NUCLEO-F091RC** board (though other MCUs may be used).  
- Communicates over **UART** to receive position data (x, y) from a computer or direct VR pipeline.  
- Translates coordinates into discrete steps for the stepper motors.

### VR Headset
- **Meta Quest 2** is the primary tested headset.  
- Leverages inside-out tracking to capture user’s translational movement.  
- Receives the live 360° video stream from the local server, rendering it in a 360° player.

---

## Video Streaming Workflow
1. **Capture**: A 360° camera (e.g., Insta360 ONE X2) captures live equirectangular video.  
2. **Encoding**: The camera encodes in H.264 / H.265 and sends via RTMP to a local server (running Docker + NGINX or FFmpeg).  
3. **Packaging**: The server segments the incoming feed into **HLS** (.m3u8) for compatibility.  
4. **Playback**: The VR headset uses its native browser/app to load the stream over Wi-Fi. Latency averages ~1 second.

---

## Project Specifications

### Functional Specifications
1. **Capture 360° Video**: Acquire spherical footage in real time.  
2. **Real-Time Streaming**: Deliver the 360° feed with minimal latency (<1–2s).  
3. **Mechanical 2D Motion**: Precisely reposition the camera in X and Y directions.  
4. **Integration with VR Headset**: Reflect user’s translational movement as camera motion.  
5. **User Position Data**: VR headset must track (x, y) for movement.  

### Non-Functional Specifications
1. **Wireless Freedom**: Headset to camera/MCU communication should be wireless.  
2. **Latency**: Maintain real-time feel, targeting ~1 second or less.  
3. **Stability**: Mechanical system must minimize vibrations.  
4. **Reliability**: Operate for extended periods without failure.  
5. **Cost**: System budget under \$1000.

---

## Prototype Data
- **Motor Timing**: At 2500 µs step delay, the gantry can move ~0.38 m/s.  
- **Streaming Latency**: Measured ~1 second behind a live reference clock.  
- **Mechanical Stability**: T-slot extrusions and bearing-based sliders reduce sway.

---

## Getting Started

### Prerequisites
- **SArduino** (or similar MCU).  
- **Stepper Motors + Drivers** (e.g., NEMA 23 + TB6600 drivers).  
- **360° Camera** (e.g., Insta360 ONE X2).  
- **Meta Quest 2** (recommended VR headset).  
- **Docker** & basic knowledge of RTMP/HLS streaming (optional but recommended).

### Setup Instructions
1. **Assemble Gantry**  
   - Use T-slot extrusions to form a rectangular frame.  
   - Attach sliders, belts, and stepper motors for X-Y motion.  

2. **Wire Electronics**  
   - Connect motor drivers to the motors and to the 12V power supply.  
   - Wire the MCU’s GPIO pins to each driver’s **STEP** and **DIR** inputs.  
   - Confirm correct grounding and power lines.

3. **Flash MCU Firmware**  
   - Use arduino or another toolchain to upload the provided firmware.  
   - Verify correct UART settings (e.g., 115200 or 112500 baud).

4. **Configure Streaming**  
   - Set up a local RTMP server using Docker.  
   - Configure your 360° camera to push RTMP to the server.  
   - The server re-packages to HLS automatically.

5. **VR Headset**  
   - Connect headset to the same local network as the RTMP/HLS server.  
   - Navigate to the server’s URL in the headset browser (e.g., `http://YOUR_SERVER_IP:PORT/stream.m3u8`).
   

## Safety and Risk Management
- **Electrical Safety**: Enclose or insulate exposed wires. Use correct fuses and a reliable 12V power supply.  
- **Mechanical Safety**: Keep clear of moving parts on the gantry. Ensure stable mounting of motors and camera.  
- **VR Comfort**: Excessive latency or mismatches in motion can cause motion sickness. Always test carefully at incremental speeds.

---

## Acknowledgements
- **Project Team**: Original concept by (formerly) *6ixSense VR Team* – [Anunay Kumar, Christopher Santos, Kiyon Farokhi, Mitchell Ostler, Nadeem Said]  
- **Consultant**: Yash Vardhan Pant  
- **Faculty Support**: University of Waterloo, Department of Electrical & Computer Engineering  

Special thanks to ECE faculty members and teaching assistants for guidance on control systems, embedded software, and network protocols.

---

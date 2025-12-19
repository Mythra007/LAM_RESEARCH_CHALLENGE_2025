Lam Research Challenge 2025 – Hardware Hustle

This repository documents the design, development, and integration of a complete robotics system built for the Lam Research Challenge 2025 (Grand Finale). The project demonstrates coordination between manual and autonomous robots, along with a sensor-driven arena implementing fluid delivery, LED activation, and weight validation.

Project Summary

The system consists of three major components:

SARM (Single Arm Robot) – a manually controlled omnidirectional robot used for obstacle removal.

ALFR (Advanced Line Follower Robot) – a fully autonomous robot that navigates a predefined arena.

Smart Arena – an embedded system with gate-based triggers controlling a pump, LEDs, load cell, and LCD.

The complete workflow is designed to execute within a strict 10-minute time limit, as specified by the competition rules.

System Architecture

A mobile application communicates with the SARM using Bluetooth serial communication. The SARM removes obstacles from the arena path. The ALFR autonomously follows the line, detects obstacles, waits until clearance, and triggers arena gates. The arena controller executes pump, LED, and load cell logic based on gate events and displays results on an LCD.

Robots
SARM – Single Arm Robot (Manual)

The Single Arm Robot is built on an omnidirectional base and controlled manually via a mobile application.

Key features:

ESP32-based controller

Omni-wheel platform for full planar mobility

Bluetooth serial communication

3D-printed robotic arm with five servo motors

Independent control of wheels and arm joints

Primary functions:

Navigate freely around the arena

Identify and remove obstacles blocking the ALFR path

Perform lateral, diagonal, rotational, and straight movements

ALFR – Advanced Line Follower Robot (Autonomous)

The Advanced Line Follower Robot is responsible for autonomous navigation and arena interaction.

Key features:

Arduino Nano controller

8-channel IR sensor array for high-precision line tracking

Ultrasonic sensor for obstacle detection

Differential drive using PWM motor control

Primary functions:

Follow the predefined black line path

Stop at obstacles and wait until cleared by SARM

Resume navigation automatically

Trigger arena gates sequentially

Arena and Gate Logic

The arena is constructed according to official specifications and integrates multiple subsystems.

Gate logic:

Gate 1: When ALFR crosses the gate, the peristaltic pump activates and dispenses 125 ml of liquid before stopping.

Gate 2: When ALFR crosses the gate, the LAM LED system is activated.

Gate 3: When ALFR parks, the load cell measures the final weight and displays the result on the LCD.

All gate actions are implemented using non-blocking logic and sensor-based event detection.

Software Overview

Controllers used:

ESP32 for SARM control

Arduino Nano for ALFR navigation

ESP32-based controller for arena logic

Core concepts implemented:

Event-driven state machines

PWM-based motor control

Bluetooth serial communication

Sensor fusion and threshold-based decision logic

Non-blocking timing using millis()

Calibration and validation using load cell feedback

Hardware Stack

Microcontrollers: ESP32, Arduino Nano

Motors: DC geared motors, servo motors

Sensors: IR sensor array, ultrasonic sensor, load cell (HX711)

Actuators: Peristaltic pump, LED array

Display: 1.8-inch TFT LCD

Power management: Buck converter and distributed power rails

Fabrication and Mechanical Design

All mechanical components were designed and fabricated in-house.

Tools and materials:

CAD: Fusion 360, SolidWorks

Slicing: Ultimaker Cura

3D printers: Ender 3, Bambu Labs

Material: PLA, 1.75 mm

Real-world challenges addressed:

Printer tolerance mismatches requiring CAD compensation

Post-processing of prints for smooth motion

Servo current limitations and power distribution issues

PCB failure handled by manual rewiring and debugging

Mechanical redesign to maintain functionality after gripper failure

Repository Structure

SARM

ESP32 firmware

Mobile application

CAD files

ALFR

Arduino firmware

CAD files

Arena

Gate controller firmware

Circuit diagrams

Media

Build photos

Working demo links

Docs

Detailed project report

Competition rulebook

Results

Successful coordination between manual and autonomous robots

Reliable obstacle handling and recovery

Accurate line following and gate triggering

Precise fluid dispensing and weight measurement

Fully integrated system operating within competition constraints

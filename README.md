# Embedded RTOS Simulator — C

[![Build](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/hitesh-bhatnagar/Embedded-RTOS-Simulator-C-Learning-Project)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-C-555.svg)](#)

> **Embedded RTOS Simulator** — small, realistic RTOS kernel written in C for learning and interviews.  
> Implements a preemptive priority scheduler, message queues (strings), semaphores, simulated UART, shell CLI, timer & interrupt simulation, logging and diagnostics — no hardware required.

---

## Table of Contents

- [Why this project](#why-this-project)  
- [Features](#features)  
- [Architecture & design](#architecture--design)  
- [Build & run (quickstart)](#build--run-quickstart)  
- [Shell / CLI commands](#shell--cli-commands)  
- [Example session](#example-session)  
- [Tests and CI](#tests-and-ci)  
- [Project structure](#project-structure)  
- [Limitations & future work](#limitations--future-work)  
- [Contributing](#contributing)  
- [License](#license)  
- [Resume blurb](#resume-blurb)  
- [Contact](#contact)

---


## Why this project

This repository is built to teach the core concepts of operating systems and real-time embedded software through a hands-on, incremental approach:

- Learn how kernels schedule tasks and handle concurrency
- Understand interrupts, ISRs and how they wake tasks
- Practice writing firmware-style C (no OS abstractions)
  
---

## Features

- **Kernel fundamentals**
  - Task Control Block (TCB) style `Task` structure
  - Task states: `READY`, `RUNNING`, `BLOCKED`, `SLEEPING`, `TERMINATED`
  - Preemptive priority-based scheduler with round-robin tie-break
  - Idle task + CPU usage monitoring

- **IPC & sync**
  - Message queue (string messages)
  - Binary semaphore (mutex-like behavior)

- **Peripherals (simulated)**
  - UART simulation (TX/RX circular buffers)
  - Timer simulation with `timer_isr()` callback
  - Interrupt vector table (`registerInterrupt` / `triggerInterrupt`)

- **User Interface**
  - CLI shell running as a task with commands: `help`, `list`, `send`, `pause`, `resume`, `kill`, `log show`, `log clear`, `set threshold`, `status`, `trigger_interrupt`, etc.

- **Diagnostics**
  - Log buffer for sensor data
  - `status` command shows queue usage, tick count, task counts, CPU usage

---

## Architecture

High-level components:

- **main loop**: `tick()` → `runScheduler()` → `sleep/usleep` (simulated)
- **scheduler**: selects highest-priority READY task, runs it (preemptive)
- **tick()**: advances sleep counters, triggers shell wakeups and timer ISR
- **interrupt module**: simple vector table and API to register/trigger ISRs
- **IPC**: circular queue for string messages
- **Shell**: user-facing CLI to control runtime


---

## Build & Run

### Prerequisites

- **Linux / macOS**: `gcc`, `make`
- Windows: MSYS2 with `base-devel`, `gcc`, `make`

### Build

```bash
git clone https://github.com/hitesh-bhatnagar/Embedded-RTOS-Simulator-C-Learning-Project.git
cd Embedded-RTOS-Simulator-C-Learning-Project
make clean
make
./rtos
```

## Shell / Command Line Interface (CLI) Usage:

help                       - Show help
list                       - List all tasks and their state
send <message>             - Send string message to queue
log show                   - Show in-memory log
log clear                  - Clear the log buffer
pause <taskId>             - Pause (BLOCK) a task
resume <taskId>            - Resume (READY) a task
kill <taskId>              - Terminate a task
trigger <taskId>           - Manually set a task READY
trigger_interrupt <id>     - Call registered ISR (simulate button/timer)
status                     - Print detailed system status
clear                      - Clear terminal screen

### Example session

[SHELL] >>> help
[SHELL] >>> send Hello from RTOS
[SHELL] >>> log show
[SHELL] >>> trigger_interrupt 0
[SHELL] >>> status

---

## Limitations & future work

### Limitations

- This is a simulation running on a host OS — not a real microcontroller. Timing and power states are approximated.

- No actual context switching of CPU registers is performed (we call task functions directly).

- No stack/heap per task simulation (can be added later).

### Planned / nice-to-have

- Implement nested interrupts and interrupt priorities

- Task stack usage simulation and simulated per-task memory limits

- Export log to file / CSV

- Add a small unit-test harness for modules

- Port demo tasks to run on a microcontroller (STM32/ESP32) as follow-up project

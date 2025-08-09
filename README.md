

<div align="center">
  
# ⚡ Embedded RTOS Simulator in C  
**A Learning Project for Real-Time OS Concepts, Task Scheduling, and Embedded Systems**  


![Build Status](https://github.com/hitesh-bhatnagar/Embedded-RTOS-Simulator-C-Learning-Project/actions/workflows/ci.yml/badge.svg)
[![GitHub Repo](https://img.shields.io/badge/View%20on-GitHub-blue?style=for-the-badge&logo=github)](https://github.com/hitesh-bhatnagar/Embedded-RTOS-Simulator-C-Learning-Project)
[![Made with C](https://img.shields.io/badge/Made%20with-C-00599C?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](LICENSE)

</div>

---

> **Embedded RTOS Simulator** — small, realistic RTOS kernel written in C for learning and interviews.  
> Implements a preemptive priority scheduler, message queues (strings), semaphores, simulated UART, shell CLI, timer & interrupt simulation, logging and diagnostics — no hardware required.

---

## Table of Contents

- [Why this project](#why-this-project)  
- [Features](#features)  
- [Architecture & design](#architecture--design)  
- [Build & run](#build--run)  
- [Shell / Command Line Interface](#shell--command-line-interface)  
- [Example session](#example-session)  
- [Tests and CI](#tests-and-ci)  
- [Project structure](#project-structure)  
- [Limitations & future work](#limitations--future-work)  
- [License](#license)  


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

## Architecture & design

High-level components:

- **main loop**: `tick()` → `runScheduler()` → `sleep/usleep` (simulated)
- **scheduler**: selects highest-priority READY task, runs it (preemptive)
- **tick()**: advances sleep counters, triggers shell wakeups and timer ISR
- **interrupt module**: simple vector table and API to register/trigger ISRs
- **IPC**: circular queue for string messages
- **Shell**: user-facing CLI to control runtime


```plaintext
+--------------------------------------------------+
|                    Shell Task                   |
| (User Commands: list, send, kill, pause, etc.)   |
+--------------------------------------------------+
               |              |  
               v              v  
+---------------------+   +---------------------+
|   Sensor Reader     |   |    LED Blinker      |
| (Simulated sensors) |   | (Status LED toggle) |
+---------------------+   +---------------------+
               |              ^
               v              |
+---------------------+   +---------------------+
|    Data Logger      |<--| Message Queue       |
| (Logs temperature)  |   | (IPC mechanism)     |
+---------------------+   +---------------------+
               ^
               |
+---------------------+
| Interrupt Handlers  |
| (Timer, Button ISR) |
+---------------------+
```
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

## Shell / Command Line Interface

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
``` bash
[SHELL] >>> help
[SHELL] >>> send Hello from RTOS
[SHELL] >>> log show
[SHELL] >>> trigger_interrupt 0
[SHELL] >>> status
```
### Example output
``` bash
===== RTOS Booting =====
[KERNEL] Task Created: ID=0 | Name=Sensor_Reader | Priority=1
[KERNEL] Task Created: ID=1 | Name=Logger | Priority=1
[KERNEL] Task Created: ID=2 | Name=LED_Blinker | Priority=1
[KERNEL] Task Created: ID=3 | Name=ShellTask | Priority=2

[TICK 0]
[SCHEDULER] Running task: ShellTask (Priority: 2)
[SHELL] >> list
Task[0]: Sensor_Reader | State: READY | Priority: 1
Task[1]: Logger        | State: READY | Priority: 1
Task[2]: LED_Blinker   | State: READY | Priority: 1
Task[3]: ShellTask     | State: RUNNING | Priority: 2

```
---

## Limitations & future work

### Limitations

- This is a simulation running on a host OS — not a real microcontroller. Timing and power states are approximated.

- No actual context switching of CPU registers is performed (we call task functions directly).

- No stack/heap per task simulation (can be added later).

### Planned / nice-to-have

- Implement nested interrupts and interrupt priorities

- Task stack usage simulation and simulated per-task memory limits

## 📜 License
This project is licensed under the MIT License.

- Export log to file / CSV

- Add a small unit-test harness for modules

- Port demo tasks to run on a microcontroller (STM32/ESP32) as follow-up project

# Embedded RTOS Simulator — C

[![Build](https://img.shields.io/github/actions/workflow/status/<GITHUB_USER>/<REPO>/ci.yml?branch=main)](https://github.com/<GITHUB_USER>/<REPO>/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

> **Embedded RTOS Simulator** — small, realistic RTOS kernel written in C for learning and interviews.  
> Implements a preemptive priority scheduler, message queues (strings), semaphores, simulated UART, shell CLI, timer & interrupt simulation, logging and diagnostics — no hardware required.

---

## Table of Contents

- [Demo](#demo)
- [Why this project](#why-this-project)
- [Features](#features)
- [Architecture](#architecture)
- [Build & Run](#build--run)
- [Shell / CLI Usage](#shell--cli-usage)
- [Example Session](#example-session)
- [Testing & CI](#testing--ci)
- [Project Structure](#project-structure)
- [How to record the demo GIF](#how-to-record-the-demo-gif)
- [Contributing](#contributing)
- [License](#license)
- [Resume blurb](#resume-blurb)

---

## Demo

Place a short animated demo GIF here — `docs/demo.gif` (example: shell usage, send, log show, trigger_interrupt).  
Example embed:

![Demo GIF](docs/demo.gif)

(If you don’t have a GIF yet, follow the recording instructions below.)

---

## Why this project

This project is designed to teach you kernel and RTOS concepts by doing — not just reading. It demonstrates the practical mechanics used in real products (FreeRTOS, Zephyr, embedded Linux) while staying small enough to inspect and modify quickly.

Use it to:
- Learn task scheduling, semaphores, queues, ISRs
- Demonstrate embedded skills in interviews
- Prototype algorithms that will later move to MCU code (STM32/ARM, AVR, 8051)

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

See `docs/architecture.png` for a diagram (draw with PlantUML or draw.io).

---

## Build & Run

### Prerequisites

- **Linux / macOS**: `gcc`, `make`, `python` (optional)
- **Windows (MSYS2)**: use `pacman -Syu` then `pacman -S base-devel gcc make`

### Build

```bash
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
set threshold <val>        - Change overheat threshold for monitor
status                     - Print detailed system status
clear                      - Clear terminal screen

### Example session

[SHELL] >>> help
[SHELL] >>> send Hello from RTOS
[SHELL] >>> log show
[SHELL] >>> trigger_interrupt 0
[SHELL] >>> status

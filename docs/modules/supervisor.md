# Supervisor Module

## Overview
The Supervisor module is responsible for overall system health, watchdog management, and failsafe execution.

## Key Features
- Heartbeat monitoring of critical modules.
- Multi-level failsafe management.
- CPU load shedding (throttling non-critical tasks).
- Secure boot sequence enforcement.

## Design
- Based on a Hierarchical State Machine (HSM).
- Implements watchdogs and heartbeat timeouts for all critical tasks.
- Central controller for system-wide safety states (DISARMED, ARMED, FAILSAFE).

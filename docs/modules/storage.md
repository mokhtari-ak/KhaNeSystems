# Storage Module

## Overview
The Storage module provides non-blocking, reliable logging of system events and flight telemetry to SD card media, ensuring data integrity without impacting critical flight control loops.

## Key Features
- Raw sector writing to avoid filesystem overhead.
- Double-buffer ping-pong strategy.
- Checksum verification per block.

## Design
- Uses circular RAM buffers (Ouroboros pattern) to decouple log generation from I/O.
- Raw sector access.
- Non-blocking I/O.

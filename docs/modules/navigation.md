# Navigation Module

## Overview
The Navigation module manages autonomous mission execution and vehicle flight envelope configuration.

## Key Features
- Centralized parameter management with bounds validation.
- Vehicle-specific control logic (Quadcopter/Fixed-Wing support).
- Waypoint-based mission execution.

## Design
- Compile-time selection of flight dynamics logic (static polymorphism).
- Parameter server for safe configuration tuning.
- Waypoint navigation engine with automated safety checks.

# Propulsion Module

## Overview
The Propulsion module handles motor control outputs and safety interlocks.

## Key Features
- Software locking outside of the ARMED state.
- Battery monitoring and voltage sag protection.
- Thrust limit calculation based on battery health.

## Design
- Forced output of 0.0f thrust in non-ARMED states.
- High-priority control loop integration.
- Dynamic thrust slew rate limiting.

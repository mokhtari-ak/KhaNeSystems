# Sensor Fusion Module

## Overview
The Sensor Fusion module provides reliable state estimation (position/attitude) by integrating IMU, GPS, and other sensors.

## Key Features
- EKF (Extended Kalman Filter) implementation using KalmanFilterLibrary.
- Numerical safety validation (NaN/Inf checks).
- Sensor data validation/sanity checks.
- Adaptive EKF order based on CPU load.

## Design
- Encapsulates mathematical complexities.
- Employs Eigen3 for matrix operations.
- Hardened against numerical divergence.

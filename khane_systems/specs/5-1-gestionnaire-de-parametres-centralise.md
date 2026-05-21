# Story 5-1: Gestionnaire de paramètres centralisé (Parameter Server)

Status: ready-for-dev

## Story

As an embedded software engineer,
I want a central parameter server,
so that all system parameters are consistently managed, validated, persisted across reboots, and safely accessible throughout the system.

## Acceptance Criteria

1. Implement a thread-safe `ParameterServer` component capable of storing and retrieving system parameters.
2. The server must support registration of parameters with configurable bounds (min/max).
3. Any attempt to set a parameter value outside of its defined bounds must be rejected with an appropriate error code.
4. Parameters must have defined default values used during system initialization if no valid stored configuration is found.
5. All parameters must be persisted in non-volatile memory (Flash/EEPROM abstraction) and restored automatically upon system reboot.
6. The implementation must include unit tests verifying bounds validation, safe default behavior, and persistence logic.

## Tasks / Subtasks

- [ ] Task 1: Design ParameterServer interface and storage structure (AC: 1, 2)
  - [ ] Implement parameter registration and bounds check mechanisms
- [ ] Task 2: Implement persistent storage abstraction (AC: 5)
  - [ ] Integrate with flash memory driver for read/write
- [ ] Task 3: Implement safety and integrity checks (AC: 3, 4)
  - [ ] Add CRC/Checksum validation for stored parameters
- [ ] Task 4: Unit Testing (AC: 6)
  - [ ] Write mock-based tests for flash storage and bounds validation

## Dev Notes

- Use the RTOS abstraction layer for thread safety (mutexes/semaphores) as defined in `libs/rtos_abstract`.
- Persistent storage should leverage existing flash memory wrappers.
- Ensure the API is C/C++ friendly to integrate with existing modules.

### Project Structure Notes

- New module should be placed in `libs/parameter_server` or similar, depending on existing conventions.
- Follow STM32 HAL-compatible wrappers for storage access.

### References

- [Source: _bmad-output/planning-artifacts/architecture.md]
- [Source: _bmad-output/planning-artifacts/epics.md]

## Dev Agent Record

### Agent Model Used

N/A

### Debug Log References

N/A

### Completion Notes List

N/A

### File List

- `libs/parameter_server/Inc/parameter_server.h`
- `libs/parameter_server/Src/parameter_server.c`
- `tests/unit/test_parameter_server.cpp`

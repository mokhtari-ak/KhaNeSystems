# Story 5.2: Contrôleur de Vol commutable (Quadcopter vs Fixed-Wing)

Status: ready-for-dev

## Story

As a multi-platform developer,
I want to select the vehicle control logic at compile-time,
So that the binary is optimized and specific to the target drone dynamics, and only relevant control policies are included.

## Acceptance Criteria

1. [ ] Create `CMake` option `VEHICLE_TYPE` (values: `QUADCOPTER`, `FIXED_WING`, default: `QUADCOPTER`) for compile-time selection.
2. [ ] Define a `VehicleDynamics` traits class or concept that abstracts control loop behaviors.
3. [ ] Implement static polymorphism in the navigation/control modules to switch between quadcopter and fixed-wing control policies based on `VEHICLE_TYPE`.
4. [ ] Ensure that only the selected vehicle's control code and parameters are linked into the final binary.
5. [ ] Verify that the `FlightController` interface remains agnostic while the underlying implementation is vehicle-specific.

## Tasks / Subtasks

- [ ] Task 1: Update `CMakeLists.txt` with `VEHICLE_TYPE` option.
- [ ] Task 2: Define `VehicleTraits` interface and concrete implementations for `Quadcopter` and `FixedWing`.
- [ ] Task 3: Refactor `FlightController` and dependent modules to use static polymorphism with the `VehicleTraits`.
- [ ] Task 4: Verify compile-time dead-code elimination and binary size optimization.

## Dev Notes

- Use static polymorphism to avoid virtual function overhead and ensure compile-time linking.
- Ensure all flight control logic is encapsulated in vehicle-specific traits.
- The `FlightController` must remain the common interface for the main loop, but instantiate the specific trait-based controller at compile-time.

### Project Structure Notes

- Modify `CMakeLists.txt` in the root and relevant subdirectories in `khane_systems/`.
- Follow the existing C++20 standard constraints (no exceptions, no RTTI).

### References

- [Source: _bmad-output/planning-artifacts/epics.md#Story 5.2: Contrôleur de Vol commutable (Quadcopter vs Fixed-Wing)]
- [Source: khane_systems/FlightController/Inc/FlightController.hpp]
- [Source: CMakeLists.txt]

## Dev Agent Record

### Agent Model Used

Gemini 1.5 Pro

### Completion Notes

- Ultimate context engine analysis completed - comprehensive developer guide created.

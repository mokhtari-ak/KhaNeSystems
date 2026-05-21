# Feature Specification: Hardware Abstraction Boundary

**Feature Branch**: `001-hardware-abstraction-boundary`  
**Created**: 2026-05-07  
**Status**: Draft  
**Input**: User description: "La bibliotheque Wrappers du dossier /Libs/Wrappers contient des classes pour l'abstraction des acces hardware; la bibliotheque HardwareAccessLayer contient les acces hardware propres a STM32."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Use hardware through portable wrappers (Priority: P1)

As an embedded application developer, I need the wrapper library to expose hardware capabilities through platform-neutral classes so application code can depend on stable abstractions instead of microcontroller-specific access details.

**Why this priority**: This is the main value of the boundary: application modules can be written against the abstraction layer and remain insulated from STM32-specific hardware access choices.

**Independent Test**: Can be tested by reviewing a representative hardware usage path and confirming the application-facing class is provided by the wrapper library while the STM32-specific access remains outside the wrapper abstraction surface.

**Acceptance Scenarios**:

1. **Given** an application module needs a supported hardware peripheral, **When** it selects the corresponding wrapper class, **Then** it can express the requested behavior without referencing STM32-specific hardware access classes directly.
2. **Given** a wrapper class represents a hardware capability, **When** its public contract is reviewed, **Then** the contract describes portable behavior and configuration concepts rather than STM32-only access mechanics.

---

### User Story 2 - Keep STM32 access in the hardware access layer (Priority: P2)

As a platform maintainer, I need STM32-specific hardware access to be isolated in the HardwareAccessLayer library so platform changes can be made without changing the application-facing abstraction contracts.

**Why this priority**: The platform-specific implementation boundary must be clear before additional peripherals or boards are added.

**Independent Test**: Can be tested by reviewing the STM32-specific access classes and confirming they are grouped in the HardwareAccessLayer library and consumed through the abstraction boundary.

**Acceptance Scenarios**:

1. **Given** a supported peripheral requires STM32-specific behavior, **When** that behavior is located, **Then** it belongs to the HardwareAccessLayer library rather than the application-facing wrapper contract.
2. **Given** STM32-specific access details change, **When** the change preserves the wrapper contract, **Then** application modules using the wrappers do not need to change.

---

### User Story 3 - Add or review peripherals consistently (Priority: P3)

As a contributor, I need clear ownership rules for new hardware-facing classes so future peripherals follow the same separation between abstraction and STM32 access.

**Why this priority**: Consistency prevents architectural drift as the project grows.

**Independent Test**: Can be tested by proposing a new peripheral capability and verifying that portable behavior, configuration concepts, and STM32-specific access each have an unambiguous destination.

**Acceptance Scenarios**:

1. **Given** a new hardware capability is added, **When** its classes are reviewed, **Then** portable abstraction belongs to Wrappers and STM32-specific access belongs to HardwareAccessLayer.
2. **Given** a code review covers hardware access changes, **When** dependencies are inspected, **Then** reviewers can identify whether a class crosses the architectural boundary.

### Edge Cases

- When a hardware capability is supported by wrappers but not yet by STM32 access, the wrapper contract must make the unsupported state explicit rather than silently depending on incomplete platform behavior.
- When STM32 access exposes capabilities not yet represented by wrappers, those capabilities must remain unavailable to application modules until an abstraction contract is defined.
- When a configuration concept exists only on STM32, the abstraction must either express it as an optional platform capability or keep it inside HardwareAccessLayer.
- When test or diagnostic code needs direct STM32 access, it must be identified as platform-specific and not treated as the normal application-facing usage path.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The Wrappers library MUST define the application-facing abstraction classes for supported hardware capabilities.
- **FR-002**: Wrapper abstraction classes MUST expose portable behavior and configuration concepts suitable for application modules.
- **FR-003**: The HardwareAccessLayer library MUST contain the STM32-specific hardware access classes for supported hardware capabilities.
- **FR-004**: Application modules MUST be able to use supported hardware capabilities through Wrappers without directly depending on STM32-specific access classes.
- **FR-005**: STM32-specific access details MUST remain replaceable without requiring changes to wrapper contracts when the observable hardware behavior is unchanged.
- **FR-006**: New hardware capabilities MUST declare which parts belong to the portable abstraction and which parts belong to STM32-specific access.
- **FR-007**: The project documentation or review criteria MUST describe the boundary between Wrappers and HardwareAccessLayer clearly enough for contributors to classify new classes.
- **FR-008**: The build relationship MUST preserve the intended dependency direction: wrappers may depend on the hardware access layer to fulfill behavior, while application-facing code should depend on wrappers for normal hardware usage.

### Key Entities

- **Wrapper Abstraction Class**: Application-facing class that represents a hardware capability through portable behavior, state, and configuration concepts.
- **Hardware Access Class**: STM32-specific class that performs platform hardware access for a supported capability.
- **Hardware Capability**: A functional peripheral or access category such as digital input/output, communication, conversion, timing, or signal generation.
- **Platform-Specific Detail**: Any behavior, option, dependency, or constraint that is meaningful because the target platform is STM32.
- **Application Module**: Product or domain code that consumes hardware functionality through the abstraction boundary.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 100% of supported hardware capabilities have a clearly identifiable wrapper abstraction and, when STM32 support exists, a corresponding hardware access responsibility.
- **SC-002**: A reviewer can classify a new hardware-facing class as wrapper abstraction or STM32 access within 5 minutes using the documented boundary.
- **SC-003**: At least 90% of normal application hardware usage paths depend on wrapper abstractions rather than direct STM32 access classes.
- **SC-004**: A platform-specific access change that preserves observable behavior can be reviewed without requiring application module changes in at least 95% of cases.

## Assumptions

- The project intentionally separates portable hardware abstraction from STM32-specific hardware access.
- Wrappers are the normal entry point for application modules that need hardware capabilities.
- HardwareAccessLayer is currently STM32-focused and may later serve as the replaceable platform-specific side of the abstraction.
- Direct STM32 access can exist for platform bring-up, diagnostics, or tests, but it is not the default application-facing path.

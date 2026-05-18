# KhaNeSystems

Firmware déterministe et modulaire pour drones multi-plateformes (STM32F407).

## Structure du Projet
- **Core/** : BSP et HAL générés par STM32CubeMX.
- **Drivers/** : Pilotes bas niveau.
- **Libs/** :
    - `common/` : Utilitaires partagés.
    - `rtos_abstract/` : Abstraction RTOS (FreeRTOS/ThreadX).
    - `internal_communication/` : Bus d'événements Mediator statique.
- **KhaneSystems/** : Logique métier (EKF, Flight Control, Navigation).
- **Tests/** : Tests unitaires sur hôte.

## Build
Le projet utilise CMake avec des presets.

### Prérequis
- Arm GNU Toolchain (arm-none-eabi-gcc)
- CMake 3.20+
- Ninja ou Make

### Compilation (STM32)
```bash
cmake --preset stm32-debug
cmake --build --preset stm32-debug
```

### Tests (Host)
```bash
cmake --preset host-test
cmake --build --preset host-test
ctest --preset host-test
```

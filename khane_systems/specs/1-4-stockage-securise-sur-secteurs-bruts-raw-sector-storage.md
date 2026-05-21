# Story: 1-4-stockage-securise-sur-secteurs-bruts-raw-sector-storage

## Context
As a black-box system, I want to write logs directly to SD card raw sectors, so that data integrity is maximized and filesystem overhead/corruption is eliminated.

## Epic
Epic 1: Socle de Communication & Diagnostic (Event Bus & Logging)

## Acceptance Criteria

1. **Given** a block of logs ready for archival
   **When** the Storage module writes to the SD card
   **Then** data is written sequentially to raw sectors without a filesystem layer

2. **Given** a sequence of sector writes
   **When** the log data is read back
   **Then** data integrity is verified using a checksum per block

3. **Given** a system reboot or power loss
   **When** reading the raw sectors
   **Then** the integrity of the last written block is checkable and partial writes are handled safely

## Technical Notes

- Target: STM32F407VG.
- Storage: SD Card (SPI/SDIO).
- Avoid filesystem (FATFS) to prevent corruption during unexpected power loss.
- Must ensure alignment with physical sector size (typically 512 bytes).
- Implement a simple header/footer for each block to store sequence numbers and checksums.
- This module must operate non-blockingly, leveraging the double-buffer/ping-pong strategy mentioned in architecture requirements.

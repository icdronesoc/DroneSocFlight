# DroneSocFlight

## GOALS:
* Simplicity and ease of hacking
* Support existing flight controller hardware **and our own**
* Support different vehicle types

## NON-GOALS:
* Be the best FC firmware out there

# Requirements
* Python
* PlatformIO (which will download the correct toolchains and libraries)
* Git

## Optional
* If you edit the protobuf definitions, you need `protoc` and `nanopb_generator.py` on your PATH for the generated code to be updated.

# Building
To make all targets: `pio run`.

Firmware file will be generated at `.pio\build\{TARGET}\firmware.bin`

TODO generate binary as hex, not bin

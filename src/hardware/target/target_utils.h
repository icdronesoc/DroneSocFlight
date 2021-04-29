#pragma once

#define CALCULATE_PIN_COUNT() constexpr size_t pinCount = sizeof(pins) / sizeof(IO::Pin)

#pragma once

#include <Arduino.h>
#include <etl/string.h>

namespace Scheduler {
    constexpr size_t NameMaxSize = 16;
    using Name = etl::string<NameMaxSize>;
}

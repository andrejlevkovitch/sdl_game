// time.hpp

#pragma once

#include <cstdint>

namespace levi {
/**\return time in milliseconds from start*/
uint32_t get_time();
void delay(uint32_t time);
}; // namespace levi

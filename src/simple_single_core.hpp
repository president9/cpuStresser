#pragma once
#include <cstddef>

void pin_thread_to_core(size_t core_id);
void simple_stress_int(size_t core_id);
void simple_stress_int_single_core();



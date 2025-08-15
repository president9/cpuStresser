// helpers.hpp
#pragma once
#include <pthread.h>
#include <mach/thread_policy.h>
#include <mach/mach.h>

inline void pin_thread_to_core(size_t core_id) {
    thread_affinity_policy_data_t policy = { static_cast<integer_t>(core_id) };
    thread_port_t mach_thread = pthread_mach_thread_np(pthread_self());
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&policy,
                      THREAD_AFFINITY_POLICY_COUNT);
}

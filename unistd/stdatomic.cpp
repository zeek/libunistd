// atomic/stdatomic.cpp
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License Open Source MIT

#include <atomic>
#include "stdatomic.h"

struct atomic_int_impl {
    std::atomic<int> v;
};

atomic_int_t* atomic_int_create(int value) {
    return new atomic_int_impl{ std::atomic<int>(value) };
}

void atomic_int_destroy(atomic_int_t* a) {
    delete a;
}

int atomic_int_load(const atomic_int_t* a) {
    return a->v.load(std::memory_order_seq_cst);
}

void atomic_int_store(atomic_int_t* a, int value) {
    a->v.store(value, std::memory_order_seq_cst);
}

int atomic_int_exchange(atomic_int_t* a, int value) {
    return a->v.exchange(value, std::memory_order_seq_cst);
}

int atomic_int_fetch_add(atomic_int_t* a, int value) {
    return a->v.fetch_add(value, std::memory_order_seq_cst);
}

int atomic_int_fetch_sub(atomic_int_t* a, int value) {
    return a->v.fetch_sub(value, std::memory_order_seq_cst);
}

int atomic_int_compare_exchange(atomic_int_t* a, int expected, int desired) {
    a->v.compare_exchange_strong(expected, desired, std::memory_order_seq_cst);
    return expected; // return old value
}


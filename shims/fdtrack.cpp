/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dlfcn.h>
#include <stdlib.h>

struct android_fdtrack_event;
using android_fdtrack_hook_t = void (*)(android_fdtrack_event*);
using compare_exchange_hook_t = bool (*)(android_fdtrack_hook_t*, android_fdtrack_hook_t);

extern "C" bool android_fdtrack_compare_exchange_hook(android_fdtrack_hook_t* expected,
                                                     android_fdtrack_hook_t value) {
    static void* libc = dlopen("libc.so", RTLD_NOW | RTLD_LOCAL);
    static auto compare_exchange_hook = libc ? reinterpret_cast<compare_exchange_hook_t>(
                                                      dlsym(libc, "android_fdtrack_compare_exchange_hook"))
                                            : nullptr;
    if (!compare_exchange_hook) {
        abort();
    }
    return compare_exchange_hook(expected, value);
}

//
// Created by konstantin on 10.06.24.
//

#pragma once

#if __has_feature(address_sanitizer)

#include <context/sanitizer/address/sanitizer_context.h>

#elif __has_feature(thread_sanitizer)

#include <context/sanitizer/thread/sanitizer_context.h>

#else

#include <context/sanitizer/empty/sanitizer_context.h>

#endif


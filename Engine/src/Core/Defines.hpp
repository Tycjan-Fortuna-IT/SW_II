/**
 * @file Defines.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-02
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

// Unsigned int types.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed int types.
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating point types
typedef float f32;
typedef double f64;

typedef char char8_t; // Filthy disgusting way to disable char8_t from used libraries since we use /Zc:char8_t- flag!

// Ensure all types are of the correct size.
static_assert(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

static_assert(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
static_assert(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
static_assert(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
static_assert(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

#ifdef EXPORT
    // Exports
    #ifdef _MSC_VER
        #define API __declspec(dllexport)
    #else
        #define API __attribute__((visibility("default")))
    #endif
#else
    // Imports
    #ifdef _MSC_VER
        /** @brief Import/export qualifier */
        #define API __declspec(dllimport)
    #else
        /** @brief Import/export qualifier */
        #define API
    #endif
#endif

#ifdef SW_WINDOWS
    #define DEBUG_BREAK() __debugbreak()
#elif defined(SW_LINUX)
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #error "Platform not supported!"
#endif

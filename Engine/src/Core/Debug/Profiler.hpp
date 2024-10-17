/**
 * @file Profiler.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-03
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#ifdef ENABLE_PROFILING

#include <tracy/Tracy.hpp>

#define HZ_PROFILE_MARK_FRAME			FrameMark;
#define PROFILE_FUNCTION(...)			ZoneScoped##__VA_OPT__(N(__VA_ARGS__))
#define PROFILE_SCOPE(...)				PROFILE_FUNCTION(__VA_ARGS__)
#define PROFILE_SCOPE_DYNAMIC(NAME)		ZoneScoped; ZoneName(NAME, strlen(NAME))
#define PROFILE_THREAD(...)				tracy::SetThreadName(__VA_ARGS__)

#else

#define PROFILE_MARK_FRAME
#define PROFILE_FUNCTION(...)
#define PROFILE_SCOPE(...)
#define PROFILE_SCOPE_DYNAMIC(NAME)
#define PROFILE_THREAD(...)

#endif

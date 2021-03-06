#pragma once

#define FASTCALL

#if defined CDECL
#define CALLING_CONV __cdecl
#elif defined CLRCALL
#define CALLING_CONV __clrcall
#elif defined STDCALL
#define CALLING_CONV __stdcall
#elif defined FASTCALL
#define CALLING_CONV __fastcall
#elif defined THISCALL
#define CALLING_CONV __thiscall
#elif defined VECTORCALL
#define CALLING_CONV __vectorcall
#else defined
#define CALLING_CONV
#endif

namespace config
{
#ifdef _DEBUG
    constexpr bool debug = true;
#else
    constexpr bool debug = false;
#endif
}
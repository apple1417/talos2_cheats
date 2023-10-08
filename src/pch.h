#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_NO_STATUS
#define NOMINMAX
#include <windows.h>

#ifdef __cplusplus

#include <cstdint>

using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

#endif

#endif /* PCH_H */

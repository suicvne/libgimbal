#ifndef GIMBAL_ATOMICS_H
#define GIMBAL_ATOMICS_H

#include "gimbal_compiler.h"


#ifdef _WIN32
#   include <winnt.h>
#   define GBL_ATOMIC_UINT16                    volatile uint16_t
#   define GBL_ATOMIC_UINT16_INIT(a, v)         do { a = v; } while(0)
#   define GBL_ATOMIC_UINT16_LOAD(a)            a
#   define GBL_ATOMIC_UINT16_INC(a)             InterlockedIncrement16(&a)
#   define GBL_ATOMIC_UINT16_DEC(a)             InterlockedDecrement16(&a)
#else
#   ifdef GBL_CPP_11
#       include <atomic>
#       define GBL_ATOMIC_UINT16                std::atomic<uint16_t>
#       define GBL_ATOMIC_UINT16_INIT(a,v)      a.store(v)
#       define GBL_ATOMIC_UINT16_LOAD(a)        a.load()
#       define GBL_ATOMIC_UINT16_INC(a)         a.fetch_add(1)
#       define GBL_ATOMIC_UINT16_DEC(a)         a.fetch_sub(1)
#   elif defined(GBL_C_11)
#       include <stdatomic.h>
#       define GBL_ATOMIC_UINT16               atomic_uint_fast16_t
#       define GBL_ATOMIC_UINT16_INIT(a, v)    atomic_init(&a, v)
#       define GBL_ATOMIC_UINT16_LOAD(a)       atomic_load(&a)
#       define GBL_ATOMIC_UINT16_INC(a)        atomic_fetch_add(&a, 1)
#       define GBL_ATOMIC_UINT16_DEC(a)        atomic_fetch_sub(&a, 1)
#   endif
#endif

#endif // GIMBAL_ATOMICS_H
/*! \file
 *  \brief GblArrayList structure and related functions
 *  \ingroup containers
 */

#ifndef GIMBAL_ARRAY_LIST_H
#define GIMBAL_ARRAY_LIST_H

#include "../core/gimbal_api_frame.h"
#include "../algorithms/gimbal_numeric.h"

#define SELF    GblArrayList* pSelf
#define CSELF   const SELF

GBL_DECLS_BEGIN

/*! \brief Contiguous array based abstract list container with C++-style STL API
 *  \ingroup containers
 */
typedef struct GblArrayList {           // Size (32-bit / 64-bit)
    GblContext*     pCtx;               // 4/8      bytes
    uint8_t*        pData;              // 4/8      bytes
    GblSize         size;               // 4/8      bytes
    GblSize         capacity;           // 4/8      bytes
    uint16_t        elementSize;        // 2        bytes
    uint16_t        zeroTerminated: 1;
    uint16_t        stackCapacity:  15; // 2        bytes
} GblArrayList;                         // 20/36    bytes

GBL_EXPORT GBL_RESULT   GblArrayList_construct_6   (SELF,
                                                    uint16_t     elementSize,
                                                    GblSize      elementCount,
                                                    const void*  pInitialData,
                                                    GblSize      structSize,
                                                    GblContext*  pCtx)                                           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT   GblArrayList_construct_5    (SELF,
                                                    uint16_t     elementSize,
                                                    GblSize      elementCount,
                                                    const void*  pInitialData,
                                                    GblSize      structSize)                                     GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT   GblArrayList_construct_4   (SELF,
                                                    uint16_t     elementSize,
                                                    GblSize      elementCount,
                                                    const void*  pInitialData)                                   GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT   GblArrayList_construct_3   (SELF,
                                                    uint16_t     elementSize,
                                                    GblSize      elementCount)                                   GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT   GblArrayList_construct_2   (SELF,
                                                    uint16_t     elementSize)                                    GBL_NOEXCEPT;
#define                 GblArrayList_construct(...) \
                            GBL_VA_OVERLOAD_SELECT(GblArrayList_construct, GBL_VA_OVERLOAD_SUFFIXER_ARGC, __VA_ARGS__)(__VA_ARGS__)
#define                 GblArrayList_alloca(...) \
                            GBL_VA_OVERLOAD_SELECT(GBL_ARRAY_LIST_ALLOCA, GBL_VA_OVERLOAD_SUFFIXER_ARGC, __VA_ARGS__)(__VA_ARGS__)

GBL_EXPORT GBL_RESULT   GblArrayList_destruct      (SELF)                                                        GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_assign        (SELF, const void* pData, GblSize elementCount)               GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_release       (SELF, void** ppVecPtr, GblSize* pSize, GblSize* pCapacity)   GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_acquire       (SELF, void* pData, GblSize size, GblSize capacity)           GBL_NOEXCEPT;

GBL_INLINE GblContext*  GblArrayList_context       (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE uint16_t     GblArrayList_stackBytes    (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_stackBuffer   (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE GblSize      GblArrayList_size          (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE GblSize      GblArrayList_capacity      (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE uint16_t     GblArrayList_elementSize   (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_data          (CSELF)                                                       GBL_NOEXCEPT;

GBL_INLINE GblBool      GblArrayList_empty         (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE GblBool      GblArrayList_stack         (CSELF)                                                       GBL_NOEXCEPT;

GBL_INLINE void*        GblArrayList_at            (CSELF, GblSize index)                                        GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_front         (CSELF)                                                       GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_back          (CSELF)                                                       GBL_NOEXCEPT;

GBL_EXPORT void*        GblArrayList_insert        (SELF, GblSize index, GblSize count, const void* pData)       GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_emplace       (SELF, GblSize index)                                         GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT   GblArrayList_pushFront     (SELF, const void* pData)                                     GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_emplaceFront  (SELF)                                                        GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT   GblArrayList_pushBack      (SELF, const void* pData)                                     GBL_NOEXCEPT;
GBL_INLINE void*        GblArrayList_emplaceBack   (SELF)                                                        GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT   GblArrayList_append        (SELF, const void* pData, GblSize elementCount)               GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_prepend       (SELF, const void* pData, GblSize elementCount)               GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT   GblArrayList_popFront      (SELF, void* pOut)                                            GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_popBack       (SELF, void* pOut)                                            GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT   GblArrayList_erase         (SELF, GblSize begin, GblSize count)                          GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_clear         (SELF)                                                        GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT   GblArrayList_reserve       (SELF, GblSize capacity)                                      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_resize        (SELF, GblSize size)                                          GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT   GblArrayList_shrinkToFit   (SELF)                                                        GBL_NOEXCEPT;

// ========== IMPL ==========

/// \cond
#define GBL_ARRAY_LIST_ALLOCA_5(elemSize, elemCount, pData, size, pCtx) \
    GblArrayList_createInPlace_(GBL_ALLOCA(size), elemSize, elemCount, pData, size, pCtx)

#define GBL_ARRAY_LIST_ALLOCA_4(elemSize, elemCount, pData, size) \
    GBL_ARRAY_LIST_ALLOCA_5(elemSize, elemCount, pData, size, NULL)

#define GBL_ARRAY_LIST_ALLOCA_3(elemSize, elemCount, pData) \
    GBL_ARRAY_LIST_ALLOCA_4(elemSize, elemCount, pData, 0)

#define GBL_ARRAY_LIST_ALLOCA_2(elemSize, elemCount) \
    GBL_ARRAY_LIST_ALLOCA_3(elemSize, elemCount, NULL)

#define GBL_ARRAY_LIST_ALLOCA_1(elemSize) \
    GBL_ARRAY_LIST_ALLOCA_2(elemSize, 0)

/// \endcond

GBL_INLINE void* GblArrayList_stackBuffer(CSELF) GBL_NOEXCEPT {
    return (uint8_t*)((pSelf && pSelf->stackCapacity)? (pSelf + 1) : NULL);
}

GBL_INLINE GblBool GblArrayList_empty(CSELF) GBL_NOEXCEPT  {
    return (pSelf && pSelf->size)? GBL_FALSE : GBL_TRUE;
}

GBL_INLINE GblBool GblArrayList_stack(CSELF) GBL_NOEXCEPT {
    return (pSelf && (pSelf->pData == GblArrayList_stackBuffer(pSelf)))? GBL_TRUE : GBL_FALSE;
}

GBL_INLINE GBL_RESULT GblArrayList_construct_5(SELF, uint16_t elementSize, GblSize elementCount, const void* pInitialData, GblSize structSize) GBL_NOEXCEPT {
    return GblArrayList_construct_6(pSelf, elementSize, elementCount, pInitialData, structSize, NULL);
}

GBL_INLINE GBL_RESULT GblArrayList_construct_4(SELF, uint16_t elementSize, GblSize elementCount, const void* pInitialData) GBL_NOEXCEPT {
    return GblArrayList_construct_5(pSelf, elementSize, elementCount, pInitialData, sizeof(GblArrayList));
}

GBL_INLINE GBL_RESULT GblArrayList_construct_3(SELF, uint16_t elementSize, GblSize elementCount) GBL_NOEXCEPT {
    return GblArrayList_construct_4(pSelf, elementSize, elementCount, NULL);
}

GBL_INLINE GBL_RESULT GblArrayList_construct_2(SELF, uint16_t elementSize) GBL_NOEXCEPT {
    return GblArrayList_construct_3(pSelf, elementSize, 0);
}

GBL_INLINE GblContext* GblArrayList_context(CSELF) GBL_NOEXCEPT {
    return (pSelf)? pSelf->pCtx : NULL;
}

GBL_INLINE uint16_t GblArrayList_stackBytes(CSELF) GBL_NOEXCEPT {
    return (pSelf)? pSelf->stackCapacity * pSelf->elementSize : 0;
}

GBL_INLINE GblSize GblArrayList_size(CSELF) GBL_NOEXCEPT {
    return (pSelf)? pSelf->size : 0;
}

GBL_INLINE GblSize GblArrayList_capacity(CSELF) GBL_NOEXCEPT {
    return (pSelf)? pSelf->capacity : 0;
}

GBL_INLINE uint16_t GblArrayList_elementSize(CSELF) GBL_NOEXCEPT {
    return (pSelf)? pSelf->elementSize : 0;
}

GBL_INLINE void* GblArrayList_data(CSELF) GBL_NOEXCEPT {
    return (pSelf)? pSelf->pData : NULL;
}

GBL_INLINE void* GblArrayList_at(CSELF, GblSize index) GBL_NOEXCEPT {
    void* pData = NULL;
    GBL_API_BEGIN(pSelf->pCtx);
    GBL_API_VERIFY(index < pSelf->size,
                   GBL_RESULT_ERROR_OUT_OF_RANGE);
    pData = &pSelf->pData[index * pSelf->elementSize];
    GBL_API_END_BLOCK();
    return pData;
}

GBL_INLINE void* GblArrayList_front(CSELF) GBL_NOEXCEPT {
    return GblArrayList_at(pSelf, 0);
}

GBL_INLINE void* GblArrayList_back(CSELF) GBL_NOEXCEPT {
    return GblArrayList_at(pSelf, pSelf->size-1);
}

GBL_INLINE GBL_RESULT GblArrayList_pushBack(SELF, const void* pData) GBL_NOEXCEPT {
    GBL_API_BEGIN(pSelf->pCtx);
    GblArrayList_insert(pSelf, GblArrayList_size(pSelf), 1, pData);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_API_END();
}

GBL_INLINE void* GblArrayList_emplaceBack(SELF) GBL_NOEXCEPT {
    return GblArrayList_insert(pSelf, GblArrayList_size(pSelf), 1, NULL);
}

GBL_INLINE GBL_RESULT GblArrayList_pushFront(SELF, const void* pData) GBL_NOEXCEPT {
    GBL_API_BEGIN(pSelf->pCtx);
    GblArrayList_insert(pSelf, 0, 1, pData);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_API_END();
}

GBL_INLINE void* GblArrayList_emplaceFront(SELF) GBL_NOEXCEPT {
    return GblArrayList_insert(pSelf, 0, 1, NULL);
}

GBL_INLINE GblArrayList* GblArrayList_createInPlace (SELF,
                                               uint16_t    elementSize,
                                               GblSize     elementCount,
                                               const void* pInitialData,
                                               GblSize     structSize,
                                               GblContext* pCtx) GBL_NOEXCEPT
{
    GblArrayList* pOut = NULL;
    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblArrayList_construct_6(pSelf, elementSize, elementCount, pInitialData, structSize, pCtx));
    pOut = pSelf;
    GBL_API_END_BLOCK();
    return pOut;
}

GBL_INLINE void* GblArrayList_emplace(SELF, GblSize index) GBL_NOEXCEPT {
    return GblArrayList_insert(pSelf, index, 1, NULL);
}

#undef CSELF
#undef SELF

GBL_DECLS_END

#endif // GIMBAL_ARRAY_LIST_H
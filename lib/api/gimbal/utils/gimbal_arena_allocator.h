/*! \file
 *  \brief GblArenaAllocator zone/region/area-based allocator + API
 *  \ingroup dataTypes
 *  \copydoc GblArenaAllocator
 */

#ifndef GIMBAL_ARENA_ALLOCATOR_H
#define GIMBAL_ARENA_ALLOCATOR_H

#include "../core/gimbal_typedefs.h"
#include "../containers/gimbal_linked_list.h"

#define GBL_SELF_TYPE GblArenaAllocator

GBL_DECLS_BEGIN

/*! \brief Represents a single arena allocation page
 *
 *  GblArenaAllocatorPage is the meta data header and
 *  memory payload buffer for a preset number of bytes
 *  which can be allocated from a GblArenaAllocator
 *  \ingroup dataTypes
 *  \sa GblArenaAllocator
 */
typedef struct GblArenaAllocatorPage {
    union {
        struct GblArenaAllocatorPage* pNext;    ///< Next (used) allocator page
        GblLinkedListNode             listNode; ///< Linked list node base
    };
    GblSize             capacity;               ///< Page capacity
    GblSize             used;                   ///< # of bytes filled on page
    GblBool             staticAlloc;            ///< Whether this page is static or heap allocated
    unsigned char       bytes[1];               ///< first byte of data segment
} GblArenaAllocatorPage;

/*! \brief Arena/zone/region/area-based paged allocator
 *
 *  GblArenaAllocator is a custom allocator which is useful
 *  for when you are allocating a bunch of data dynamically
 *  which all has the same shared lifetime. The allocator is
 *  created once, then as allocations are requested, it
 *  only allocates new memory in per-page incremements rather
 *  than for individual allocations. Finally, when you're
 *  done, rather than calling delete, you simply destruct
 *  the allocator, which frees its pages all at once. This
 *  can be SUBSTANTIALLY faster than calling malloc() or
 *  new for backing data structures with such allocation
 *  patterns.
 *  \note
 *  It's often useful to create the first allocation page
 *  as a static buffer somewhere in a source file, so that
 *  heap memory isn't even used until it's necessary. Be sure
 *  to set the GblArenaAllocatorPage::staticAlloc flag to
 *  ensure the allocator does not attempt to free it!
 *  \ingroup dataTypes
 *  \sa GblArenaAllocatorPage
 */
typedef struct GblArenaAllocator {
    union {
        GblArenaAllocatorPage* pActivePage; ///< Active (unfilled) page at list head
        GblLinkedListNode      listNode;    ///< Linked list node base
    };
    GblContext* pCtx;                       ///< Custom context associated with allocator
    GblSize     pageSize;                   ///< Default page size for all new pages
    GblSize     allocCount;                 ///< Total # of allocations across all pages
} GblArenaAllocator;

GBL_EXPORT GBL_RESULT GblArenaAllocator_construct       (GBL_SELF,
                                                         GblSize                pageSize,
                                                         GblArenaAllocatorPage* pInitialPage,
                                                         GblContext*            pCtx)               GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT GblArenaAllocator_destruct        (GBL_SELF)                                  GBL_NOEXCEPT;

GBL_EXPORT GblSize    GblArenaAllocator_pageCount       (GBL_CSELF)                                 GBL_NOEXCEPT;
GBL_EXPORT GblSize    GblArenaAllocator_bytesUsed       (GBL_CSELF)                                 GBL_NOEXCEPT;
GBL_EXPORT GblSize    GblArenaAllocator_bytesAvailable  (GBL_CSELF)                                 GBL_NOEXCEPT;

GBL_EXPORT GblSize    GblArenaAllocator_totalCapacity   (GBL_CSELF)                                 GBL_NOEXCEPT;
GBL_EXPORT GblSize    GblArenaAllocator_fragmentedBytes (GBL_CSELF)                                 GBL_NOEXCEPT;
GBL_EXPORT float      GblArenaAllocator_utilization     (GBL_CSELF)                                 GBL_NOEXCEPT;

GBL_EXPORT void*      GblArenaAllocator_alloc           (GBL_SELF, GblSize)                         GBL_NOEXCEPT;
GBL_EXPORT void*      GblArenaAllocator_allocAligned    (GBL_SELF, GblSize size, GblSize alignment) GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT GblArenaAllocator_freeAll         (GBL_SELF)                                  GBL_NOEXCEPT;

GBL_DECLS_END

#undef GBL_SELF_TYPE

#endif // GIMBAL_ARENA_ALLOCATOR_H

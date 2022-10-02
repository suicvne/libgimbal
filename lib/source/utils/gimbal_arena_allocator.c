#include <gimbal/utils/gimbal_arena_allocator.h>
#include <gimbal/core/gimbal_api_frame.h>

#define GBL_ARENA_PAGE_(node)  GBL_LINKED_LIST_ENTRY(node, GblArenaAllocatorPage, listNode)

GBL_EXPORT GBL_RESULT (GblArenaAllocator_construct)(GblArenaAllocator*     pSelf,
                                                    GblSize                pageSize,
                                                    GblSize                pageAlign,
                                                    GblArenaAllocatorPage* pInitialPage,
                                                    GblContext*            pCtx)
{
    GBL_API_BEGIN(pCtx);
    memset(pSelf, 0, sizeof(GblArenaAllocator));
    pSelf->pCtx = pCtx;
    GblLinkedList_init(&pSelf->listNode);
    if(pInitialPage)
        GblLinkedList_pushFront(&pSelf->listNode, &pInitialPage->listNode);
    pSelf->pageSize = pageSize;
    pSelf->pageAlign = pageAlign < GBL_ALIGNOF(GBL_MAX_ALIGN_T)? GBL_ALIGNOF(GBL_MAX_ALIGN_T) : pageAlign;
    GBL_API_END();
}

GBL_EXPORT GBL_RESULT GblArenaAllocator_destruct(GblArenaAllocator* pSelf) {
    return GblArenaAllocator_freeAll(pSelf);
}

static GblArenaAllocatorPage* GblArenaAllocator_allocPage_(GblContext* pCtx, GblSize size, GblSize align) {
    GblArenaAllocatorPage* pPage = NULL;
    GBL_API_BEGIN(pCtx);
    const GblSize actualSize = gblAlignedAllocSize(sizeof(GblArenaAllocatorPage) + size-1, align);
    pPage = GBL_API_MALLOC(actualSize, align);
    GblLinkedList_init(&pPage->listNode);
    pPage->capacity = size;
    pPage->used = 0;
    pPage->staticAlloc = GBL_FALSE;
    GBL_API_END_BLOCK();
    return pPage;
}

GBL_EXPORT void* GblArenaAllocator_alloc(GblArenaAllocator* pSelf, GblSize size) {
    return GblArenaAllocator_allocAligned(pSelf, size, 0);
}

GBL_EXPORT void* GblArenaAllocator_allocAligned(GblArenaAllocator* pSelf, GblSize size, GblSize alignment) {
    void* pData = NULL;
    if(size) {

        GblArenaAllocatorPage* pPage = GBL_ARENA_PAGE_(GblLinkedList_front(&pSelf->listNode));

        if(alignment == 0) alignment = GBL_ALIGNOF(GBL_MAX_ALIGN_T);
        const GblSize actualSize = gblAlignedAllocSize(size, alignment);

        if(!pPage || (pPage->capacity - pPage->used < actualSize)) {
            if(actualSize <= pSelf->pageSize) {
                GblArenaAllocatorPage* pNewPage = GblArenaAllocator_allocPage_(pSelf->pCtx,
                                                                               pSelf->pageSize,
                                                                               pSelf->pageAlign);
                GblLinkedList_pushFront(&pSelf->listNode, &pNewPage->listNode);
            } else {
                GBL_API_BEGIN(pSelf->pCtx);
                GBL_API_VERIFY(actualSize <= pSelf->pageSize,
                               GBL_RESULT_ERROR_OVERFLOW,
                               "Cannot allocate chunk of size %u from pages of size %u",
                               actualSize, pSelf->pageSize);
                GBL_API_END_BLOCK();
                goto end;
            }
        }

        pData = &pSelf->pActivePage->bytes[pSelf->pActivePage->used];
        pSelf->pActivePage->used += actualSize;
        ++pSelf->allocCount;

        uintptr_t advancePointer = (alignment - ((uintptr_t)pData % alignment));
        pData = (void*)(((uintptr_t)pData) + advancePointer);

    }
end:
    return pData;
}

GBL_EXPORT GBL_RESULT GblArenaAllocator_freeAll(GblArenaAllocator* pSelf) {
    GBL_API_BEGIN(pSelf->pCtx);

    GblLinkedListNode staticList;
    GblLinkedList_init(&staticList);

    GblLinkedListNode tempNode;
    // iterate over all pages, freeing the heap ones, accumulating + clearing the static ones
    for(GblLinkedListNode* pNode = pSelf->listNode.pNext;
        pNode != &pSelf->listNode;
        pNode = pNode->pNext)
    {
        GblArenaAllocatorPage* pPage = GBL_ARENA_PAGE_(pNode);
        if(!pPage->staticAlloc) {
            tempNode = *pNode;
            GBL_API_FREE(pPage);
            pNode = &tempNode;
        } else {
            pPage->used = 0;
            tempNode = *pNode;
            GblLinkedList_pushBack(&staticList, pNode);
            pNode = &tempNode;
        }
    }

    GblLinkedList_init(&pSelf->listNode);
    GblLinkedList_joinFront(&pSelf->listNode, &staticList);
    pSelf->allocCount   = 0;

    GBL_API_END();
}

GBL_EXPORT GblSize GblArenaAllocator_pageCount(const GblArenaAllocator* pSelf) {
    return GblLinkedList_count(&pSelf->listNode);
}

GBL_EXPORT GblSize GblArenaAllocator_bytesUsed(const GblArenaAllocator* pSelf) {
    GblSize bytes = 0;
    for(GblLinkedListNode* pIt = pSelf->listNode.pNext;
        pIt != &pSelf->listNode;
        pIt = pIt->pNext)
    {
        bytes += GBL_ARENA_PAGE_(pIt)->used;
    }
    return bytes;
}

GBL_EXPORT GblSize GblArenaAllocator_totalCapacity(const GblArenaAllocator* pSelf) {
    GblSize bytes = 0;
    for(GblLinkedListNode* pIt = pSelf->listNode.pNext;
        pIt != &pSelf->listNode;
        pIt = pIt->pNext)
    {
        bytes += GBL_ARENA_PAGE_(pIt)->capacity;
    }
    return bytes;
}

GBL_EXPORT float GblArenaAllocator_utilization(const GblArenaAllocator* pSelf) {
    GblSize capacity = 0;
    GblSize used = 0;
    for(GblLinkedListNode* pIt = pSelf->listNode.pNext;
        pIt != &pSelf->listNode;
        pIt = pIt->pNext)
    {
        capacity += GBL_ARENA_PAGE_(pIt)->capacity;
        used += GBL_ARENA_PAGE_(pIt)->used;
    }
    return (capacity != 0)? (float)used/(float)capacity : 0.0f;
}

GBL_EXPORT GblSize GblArenaAllocator_fragmentedBytes(const GblArenaAllocator* pSelf) {
    GblSize bytes = 0;
    for(GblLinkedListNode* pIt = pSelf->listNode.pNext;
        pIt != &pSelf->listNode;
        pIt = pIt->pNext)
    {
        if(GBL_ARENA_PAGE_(pIt) == pSelf->pActivePage) continue;
        bytes += GBL_ARENA_PAGE_(pIt)->capacity - GBL_ARENA_PAGE_(pIt)->used;
    }
    return bytes;
}

GBL_EXPORT GblSize GblArenaAllocator_bytesAvailable(const GblArenaAllocator* pSelf) {
    GblSize bytes = 0;
    if(pSelf->pActivePage) {
        bytes = pSelf->pActivePage->capacity - pSelf->pActivePage->used;
    }
    return bytes;
}

GBL_EXPORT void GblArenaAllocator_saveState(const GblArenaAllocator* pSelf, GblArenaAllocatorState* pState) {
    pState->pActivePage = pSelf->pActivePage;
    pState->bytesUsed   = pSelf->pActivePage->used;
}

GBL_EXPORT GBL_RESULT GblArenaAllocator_loadState(GblArenaAllocator* pSelf, const GblArenaAllocatorState* pState) {
    GBL_API_BEGIN(pSelf->pCtx);
    while(pSelf->pActivePage != pState->pActivePage) {
        GblArenaAllocatorPage* pPage = GBL_ARENA_PAGE_(GblLinkedList_popFront(&pSelf->listNode));
        GBL_API_FREE(pPage);
    }
    pSelf->pActivePage->used = pState->bytesUsed;
    GBL_API_END();
}


#include "gimbal_type_.h"
#include <gimbal/meta/gimbal_class.h>
#include <gimbal/meta/gimbal_instance.h>
#include <gimbal/meta/gimbal_interface.h>
#include <gimbal/algorithms/gimbal_hash.h>
#include <gimbal/algorithms/gimbal_sort.h>
#include <gimbal/ifaces/gimbal_ivariant.h>

// === STATIC DATA ====

GblContext*              pCtx_           = NULL;
pthread_once_t           initOnce_       = PTHREAD_ONCE_INIT;
GblBool                  initialized_    = GBL_FALSE;
GBL_THREAD_LOCAL GblBool initializing_   = GBL_FALSE;
pthread_mutex_t          typeRegMtx_;
GblHashSet               typeRegistry_;
struct TypeBuiltins_     typeBuiltins_;

GBL_MAYBE_UNUSED static int metaClassIFaceEntryComparator_(const void* pA, const void* pB) {
    const GblTypeInterfaceMapEntry* pIFaceA = pA;
    const GblTypeInterfaceMapEntry* pIfaceB = pB;
    return pIFaceA->interfaceType - pIfaceB->interfaceType;
}

static GblHash metaClassHasher_(const GblHashSet* pMap, const void* pItem) {
    GBL_UNUSED(pMap);
    const GblMetaClass** pMeta = (const GblMetaClass**)pItem;
    return gblHashMurmur((*pMeta)->pName, strnlen((*pMeta)->pName, GBL_META_TYPE_NAME_SIZE_MAX));
}

static GblBool metaClassComparator_(const GblHashSet* pMap, const void* pA, const void* pB) {
    GBL_UNUSED(pMap);
    const GblMetaClass** ppRhs = (const GblMetaClass**)pA, **ppLhs = (const GblMetaClass**)pB;
    return strncmp((*ppRhs)->pName, (*ppLhs)->pName, GBL_META_TYPE_NAME_SIZE_MAX) == 0;
}

static void metaClassElementFree_(const GblHashSet* pMap, void* item) {
    GBL_UNUSED(pMap);
    GBL_API_BEGIN(pCtx_);
    GblMetaClass** ppMetaClass = (GblMetaClass**)item;
    GBL_API_FREE(*ppMetaClass);
    GBL_API_END_BLOCK();
}

static GblType typeRegister_(GblType parent,
                            const char* pName,
                            const GblTypeInfo* pInfo,
                            GblFlags flags)
{
    GblBool hasMutex = GBL_FALSE;
    GblType newType = GBL_INVALID_TYPE;
    GblMetaClass* pParent = (GblMetaClass*)parent;
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_POINTER(pName);
    GBL_API_VERIFY_POINTER(pInfo);

    GBL_API_PUSH_VERBOSE("[GblType] Register Type: %s", pName);

    GBL_API_PUSH_VERBOSE("Type Info");
    GBL_API_VERBOSE("%-20s: %-100.100s", "Parent Type", GblType_name(parent));

    GBL_API_PUSH_VERBOSE("flags");
    GBL_API_VERBOSE("%-20s: %-100u", "CLASSED",         (flags & GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED)? 1 : 0);
    GBL_API_VERBOSE("%-20s: %-100u", "INSTANTIABLE",    (flags & GBL_TYPE_FUNDAMENTAL_FLAG_INSTANTIABLE)? 1 : 0);
    GBL_API_VERBOSE("%-20s: %-100u", "DERIVABLE",       (flags & GBL_TYPE_FUNDAMENTAL_FLAG_DERIVABLE)? 1 : 0);
    GBL_API_VERBOSE("%-20s: %-100u", "DEEP_DERIVABLE",  (flags & GBL_TYPE_FUNDAMENTAL_FLAG_DEEP_DERIVABLE)? 1 : 0);
    GBL_API_VERBOSE("%-20s: %-100u", "BUILTIN",         (flags & GBL_TYPE_FLAG_BUILTIN)? 1 : 0);
    GBL_API_VERBOSE("%-20s: %-100u", "ABSTRACT",        (flags & GBL_TYPE_FLAG_ABSTRACT)? 1 : 0);
    GBL_API_VERBOSE("%-20s: %-100u", "FINAL",           (flags & GBL_TYPE_FLAG_FINAL)? 1 : 0);
    GBL_API_POP(1);

    GBL_API_PUSH_VERBOSE("Class Info");
    GBL_API_VERBOSE("%-20s: %-100u", "size",            pInfo->classSize);
    GBL_API_VERBOSE("%-20s: %-100p", "data",            pInfo->pClassData);
    GBL_API_VERBOSE("%-20s: %-100p", "initializer",     pInfo->pFnClassInit);
    GBL_API_VERBOSE("%-20s: %-100p", "finalizer",       pInfo->pFnClassFinal);
    GBL_API_POP(1);

    if(pInfo->interfaceCount) {
        GBL_API_PUSH_VERBOSE("Interface Map");
        for(GblSize i = 0; i < pInfo->interfaceCount; ++i) {
            GBL_API_VERBOSE("%u: [Type: %s, Class Offset: %u]",
                            i,
                            GblType_name(pInfo->pInterfaceMap[i].interfaceType),
                            pInfo->pInterfaceMap[i].classOffset);
        }
        GBL_API_POP(1);
    }

    GBL_API_PUSH_VERBOSE("Instance Info");
    GBL_API_VERBOSE("%-20s: %-100u", "size",            pInfo->instanceSize);
    GBL_API_VERBOSE("%-20s: %-100p", "initializer",     pInfo->pFnInstanceInit);

    GBL_API_POP(2);

    GblFlags fundamentalFlags = 0;
    if(pParent && !(flags & GBL_TYPE_FUNDAMENTAL_FLAGS_MASK)) { // not a fundamental type
        GblMetaClass* pFundamental = (GblMetaClass*)GblType_fundamental((GblType)pParent);
        fundamentalFlags = pFundamental? pFundamental->flags : 0;

        GBL_API_VERIFY_ARG(pInfo->classSize >= pParent->info.classSize,
                            "Class size [%u] must be >= to parent's class size [%u]!",
                            pInfo->classSize, pParent->info.classSize);

        GBL_API_VERIFY_ARG(pInfo->instanceSize >= pParent->info.instanceSize,
                              "Instance size [%u]must be >= to parent's instance size [%u]!",
                              pInfo->instanceSize, pParent->info.instanceSize);

        GBL_API_VERIFY_ARG(!(flags & GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED),
                           "Invalid use of CLASSED type flag on non-fundamental type!");

        GBL_API_VERIFY_ARG(!(flags & GBL_TYPE_FUNDAMENTAL_FLAG_INSTANTIABLE),
                           "Invalid use of INSTANTIABLE type flag on non-fundamental type!");

        GBL_API_VERIFY_ARG(!(flags & GBL_TYPE_FUNDAMENTAL_FLAG_DERIVABLE),
                           "Invalid use of DERIVABLE type flag on non-fundamental type!");

        GBL_API_VERIFY_ARG(!(flags & GBL_TYPE_FUNDAMENTAL_FLAG_DEEP_DERIVABLE),
                           "Invalid use of DEEP_DERIVABLE type flag on non-fundamental type!");

        if(!(fundamentalFlags & GBL_TYPE_FUNDAMENTAL_FLAG_DEEP_DERIVABLE)) {
            GBL_API_VERIFY(fundamentalFlags & GBL_TYPE_FUNDAMENTAL_FLAG_DERIVABLE, GBL_RESULT_ERROR_INVALID_TYPE,
                           "Cannot derive from NON DERIVABLE type!");

            GBL_API_VERIFY(GblType_fundamental((GblType)pParent), GBL_RESULT_ERROR_INVALID_TYPE,
                           "Cannot derive from DERIVABLE type more than once!");
        }


    } else {    //assumed to be a fundamental type
        GBL_API_VERBOSE("Fundamental Type!");

        if(flags & GBL_TYPE_FUNDAMENTAL_FLAG_DEEP_DERIVABLE) flags |= GBL_TYPE_FUNDAMENTAL_FLAG_DERIVABLE;
        if(flags & GBL_TYPE_FUNDAMENTAL_FLAG_INSTANTIABLE)   flags |= GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED;

        fundamentalFlags = flags;
    }

    if(fundamentalFlags & GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED) {
        GBL_API_VERIFY(pInfo->classSize > 0, GBL_RESULT_ERROR_INVALID_TYPE,
                       "Cannot register a CLASSED type with a class size of 0!");
    } else {
        GBL_API_VERIFY(pInfo->classSize == 0, GBL_RESULT_ERROR_INVALID_TYPE,
                       "Cannot register a NON CLASSED type with a class size > 0!");
    }

    if(fundamentalFlags & GBL_TYPE_FUNDAMENTAL_FLAG_INSTANTIABLE) {
        GBL_API_VERIFY(pInfo->instanceSize > 0, GBL_RESULT_ERROR_INVALID_TYPE,
                       "Cannot register an INSTANTIABLE type with an instance size of 0!");
        GBL_API_VERIFY(fundamentalFlags & GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED, GBL_RESULT_ERROR_INVALID_TYPE,
                       "Cannot register an INSTANTIABLE type without it being CLASSED!");
    } else {
        GBL_API_VERIFY(pInfo->instanceSize == 0, GBL_RESULT_ERROR_INVALID_TYPE,
                       "Cannot register a NON INSTANTIABLE type with an instance size > 0!");
    }

    GBL_API_VERIFY(!(pParent && (pParent->flags & GBL_TYPE_FLAG_FINAL)), GBL_RESULT_ERROR_INVALID_TYPE,
                   "Cannot derive from a FINAL type!");


    {
        uint8_t baseCount = 0;
        GblMetaClass* pParentIt = pParent;
        while(pParentIt) {
            ++baseCount;
            pParentIt = pParentIt->pParent;
        }

        GblSize metaSize = sizeof(GblMetaClass);
        const GblSize ifaceOffset = metaSize;
        const GblSize nameOffset = metaSize += sizeof(GblTypeInterfaceMapEntry) * pInfo->interfaceCount;
        const GblSize classOffset = metaSize += strlen(pName)+1;
        const GblSize basesOffset = metaSize += pInfo->classSize;
        metaSize += baseCount * sizeof(GblMetaClass*);

        const GblSize alignRem = (metaSize % GBL_META_CLASS_ALIGNMENT_);
        if(alignRem)
            metaSize += (GBL_META_CLASS_ALIGNMENT_ - (alignRem));

        GblMetaClass* pMeta = GBL_API_MALLOC(metaSize,
                                             GBL_META_CLASS_ALIGNMENT_,
                                             pName);

        GBL_API_VERIFY_EXPRESSION(!((uint64_t)pMeta & GBL_CLASS_FLAGS_BIT_MASK_),
                                  "malloc returned a misaligned pointer!");

        memset(pMeta, 0, metaSize);
        memcpy(&pMeta->info, pInfo, sizeof(GblTypeInfo));
        atomic_init(&pMeta->refCount, 0);
#ifdef GBL_TYPE_DEBUG
        atomic_init(&pMeta->instanceRefCount, 0);
#endif
        strcpy(((char*)pMeta + nameOffset), pName);
        pMeta->flags                = flags;
        pMeta->depth                = baseCount;
        pMeta->pParent              = (struct GblMetaClass*)parent;
        pMeta->pName                = ((const char*)pMeta + nameOffset);
        pMeta->pClass               = pInfo->classSize?
                                            (GblClass*)((char*)pMeta + classOffset) : NULL;
        pMeta->info.pInterfaceMap   = pInfo->interfaceCount?
                                            (GblTypeInterfaceMapEntry*)((char*)pMeta + ifaceOffset) : NULL;
        pMeta->pBases               = baseCount?
                                            (GblMetaClass**)((char*)pMeta + basesOffset) : NULL;

        memcpy((GblTypeInterfaceMapEntry*)pMeta->info.pInterfaceMap, pInfo->pInterfaceMap, sizeof(GblTypeInterfaceMapEntry)*pInfo->interfaceCount);
        //gblSortQuick((GblTypeInterfaceMapEntry*)pMeta->info.pInterfaceMap, pMeta->info.interfaceCount, sizeof(GblTypeInterfaceMapEntry), metaClassIFaceEntryComparator_);

        pParentIt = pParent;
        baseCount = 0;
        while(pParentIt) {
            pMeta->pBases[pMeta->depth-(++baseCount)] = pParentIt;
            pParentIt = pParentIt->pParent;
        }
        GBL_API_VERIFY_EXPRESSION(baseCount == pMeta->depth);

        pthread_mutex_lock(&typeRegMtx_);
        hasMutex = GBL_TRUE;

        const GblMetaClass* pOldData = GblHashSet_set(&typeRegistry_,
                                                      &pMeta);

        GBL_API_VERIFY(!pOldData, GBL_RESULT_ERROR_INVALID_ARG,
                       "A previous metatype entry named %s existed already!", pName);

        newType = (GblType)pMeta;
        if(flags & GBL_TYPE_FLAG_BUILTIN) {
             GBL_API_CALL(gblVectorPushBack(&typeBuiltins_.vector, &newType));
        }

    }
    GBL_API_END_BLOCK();
    if(hasMutex) pthread_mutex_unlock(&typeRegMtx_);
    return newType;
}

static GBL_RESULT GblType_registerBuiltins_(void) {
    GBL_API_BEGIN(pCtx_);
    GblType_registerBuiltin(GBL_TYPE_BUILTIN_INDEX_INTERFACE,
              GBL_INVALID_TYPE,
              "Interface",
              &((const GblTypeInfo) {
                  .classSize    = sizeof(GblInterface)
              }),
              GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED         |
              GBL_TYPE_FUNDAMENTAL_FLAG_DEEP_DERIVABLE  |
              GBL_TYPE_FLAG_ABSTRACT);
    GBL_API_CALL(GblIVariant_typeRegister_(pCtx_));
    GBL_API_CALL(gblValueTypesRegister_(pCtx_));
    GBL_API_CALL(GblITable_typeRegister_(pCtx_));
    GBL_API_CALL(GblIEventHandler_typeRegister_(pCtx_));
    GBL_API_CALL(GblIEventFilter_typeRegister_(pCtx_));
    GBL_API_CALL(GblObject_typeRegister_(pCtx_));
    GBL_API_CALL(GblIAllocator_typeRegister_(pCtx_));
    GBL_API_CALL(GblILogger_typeRegister_(pCtx_));
    GBL_API_CALL(GblContext_typeRegister_(pCtx_));
    GBL_API_CALL(GblEvent_typeRegister_(pCtx_));
    GBL_API_END();
}

void GblType_init_(void) {
    initializing_ = GBL_TRUE;
    GBL_API_BEGIN(pCtx_);
    GBL_API_PUSH_VERBOSE("[GblType]: Initializing.");
    GBL_API_VERIFY(!initialized_, GBL_RESULT_ERROR_INVALID_OPERATION, "Already initialized!");
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&typeRegMtx_, &mutexAttr);
    pthread_mutex_lock(&typeRegMtx_);
    GBL_API_CALL(GblHashSet_construct(&typeRegistry_,
                                      sizeof(GblMetaClass*),
                                      metaClassHasher_,
                                      metaClassComparator_,
                                      metaClassElementFree_,
                                      GBL_TYPE_REGISTRY_HASH_MAP_CAPACITY_DEFAULT,
                                      pCtx_));
    GBL_API_CALL(gblVectorConstruct(&typeBuiltins_.vector,
                                    pCtx_,
                                    sizeof(GblMetaClass*),
                                    sizeof(typeBuiltins_)));
    GBL_API_CALL(GblType_registerBuiltins_());
    GBL_API_POP(1);
    initialized_    = GBL_TRUE;
    initializing_   = GBL_FALSE;
    GBL_API_END_BLOCK();
    pthread_mutex_unlock(&typeRegMtx_);
}

// === PUBLIC API ====

GblType GblType_registerBuiltin(GblSize            expectedIndex,
                               GblType            parentType,
                               const char*        pName,
                               const GblTypeInfo* pTypeInfo,
                               GblTypeFlags       flags)
{
    GblType type = GBL_INVALID_TYPE;
    GblSize size = 0;
    GBL_API_BEGIN(pCtx_);
    GBL_TYPE_ENSURE_INITIALIZED_();
    GBL_API_PUSH_VERBOSE("[GblType] Registering Builtin Type: %s", pName);
    flags |= GBL_TYPE_FLAG_BUILTIN;
    type = typeRegister_(parentType, pName, pTypeInfo, flags);
    GBL_API_CALL(gblVectorSize(&typeBuiltins_.vector, &size));
    GBL_API_VERIFY_EXPRESSION(size == expectedIndex+1,
                              "Failed to obtain expected index! "
                              "[expected: %u, actual: %u]", expectedIndex, size-1);
    // create initial/static instance of class if builtin (so that class isn't dynamically created/destroyed)
    {
        const GblTypeInfo* pInfo = GblType_info(type);
        GBL_API_VERIFY_POINTER(pInfo);
        if(pInfo->classSize) {
            //GblClass_refFromType(type);
        }
    }
    GBL_API_POP(1);
    GBL_API_END_BLOCK();
    return type;
}

GBL_EXPORT GBL_RESULT GblType_final(void) {
    GblBool hasMutex = GBL_FALSE;
    GBL_API_BEGIN(pCtx_);
    GBL_API_PUSH_VERBOSE("[GblType]: Finalizing");
    pthread_mutex_lock(&typeRegMtx_);
    hasMutex = GBL_TRUE;

    GBL_API_CALL(gblVectorClear(&typeBuiltins_.vector));
    GBL_API_CALL(gblVectorDestruct(&typeBuiltins_.vector));
    GblHashSet_destruct(&typeRegistry_);

    GBL_API_POP(1);
    initialized_ = GBL_FALSE;
    GBL_API_END_BLOCK();
    if(hasMutex) {
        pthread_mutex_unlock(&typeRegMtx_);
        if(!initialized_) pthread_mutex_destroy(&typeRegMtx_);
    }
    return GBL_API_RESULT();
}

GBL_API GblType_init(GblContext* pCtx) {
    GBL_API_BEGIN(NULL);
    if(!initialized_) {
       // GBL_API_PUSH_VERBOSE("[GblType]: Inititalizing.");
        pCtx_ = pCtx;
        GBL_TYPE_ENSURE_INITIALIZED_();
    } else {
        GBL_API_PUSH_VERBOSE("[GblType]: ReInititalizing.");
        GBL_API_CALL(GblType_final());
        pCtx_ = pCtx;
        GblType_init_();
    }
    GBL_API_END();
}

GBL_EXPORT GblType GblType_fromBuiltinIndex(GblUint index) {
    GblType type = GBL_INVALID_TYPE;
    GBL_TYPE_ENSURE_INITIALIZED_();
    //GBL_API_BEGIN(pCtx_);
    {
        GblType* pType = NULL;
        GblSize registeredCount = 0;
        if(GBL_RESULT_SUCCESS(gblVectorSize(&typeBuiltins_.vector, &registeredCount))) {
            if(index < registeredCount) {
                if(GBL_RESULT_SUCCESS(gblVectorAt(&typeBuiltins_.vector, index, (void**)&pType)))
                    type = *pType;
            }
        }
    }
    //GBL_API_END_BLOCK();
    return type;
}

GBL_EXPORT GblType GblType_fromName(const char* pTypeName) {
    GblType foundType = GBL_INVALID_TYPE;
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_POINTER(pTypeName);
    GBL_TYPE_ENSURE_INITIALIZED_();

    GblMetaClass* pTempClass = GBL_ALLOCA(sizeof(GblMetaClass));
    pTempClass->pName = pTypeName;

    if(strnlen(pTypeName, GBL_META_TYPE_NAME_SIZE_MAX+1) > GBL_META_TYPE_NAME_SIZE_MAX)
        GBL_API_WARN("Typename is too large and will be truncated: %s", pTypeName);

    pthread_mutex_lock(&typeRegMtx_);
    GblMetaClass** ppReturnValue = (GblMetaClass**)GblHashSet_get(&typeRegistry_, &pTempClass);
    if(ppReturnValue) {
        foundType = (GblType)*ppReturnValue;
    }
    pthread_mutex_unlock(&typeRegMtx_);

    GBL_API_END_BLOCK();
    return foundType;
}

GBL_EXPORT GblSize GblType_registeredCount(void) {
    GblSize count = 0;
    GBL_API_BEGIN(pCtx_);
    GBL_TYPE_ENSURE_INITIALIZED_();
    pthread_mutex_lock(&typeRegMtx_);
    count = GblHashSet_size(&typeRegistry_);
    pthread_mutex_unlock(&typeRegMtx_);
    GBL_API_END_BLOCK();
    return count;
}


GBL_EXPORT const char* GblType_name(GblType type) {
    const char*     pName   = "Invalid";
    GblMetaClass*   pMeta   = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    if(pMeta) {
        pName = pMeta->pName;
    }
    GBL_API_END_BLOCK();
    return pName;
}

GBL_EXPORT GblType GblType_parent(GblType type) {
    GblType parent      = GBL_INVALID_TYPE;
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    if(pMeta) {
        parent = (GblType)pMeta->pParent;
    }
    GBL_API_END_BLOCK();
    return parent;
}

GBL_EXPORT GblType GblType_fundamental(GblType type) {
    GblType base      = GBL_INVALID_TYPE;
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    while(pMeta) {
        if(!pMeta->pParent || pMeta->flags & GBL_TYPE_FUNDAMENTAL_FLAGS_MASK) {
            base = (GblType)pMeta;
            break;
        }
        pMeta = pMeta->pParent;
    }
    GBL_API_END_BLOCK();
    return base;
}

GBL_EXPORT GblType GblType_base(GblType type, GblUint depth) {
    GblType base = GBL_INVALID_TYPE;
    GblMetaClass* pMeta =  GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    if(pMeta) {
        if(depth < pMeta->depth) {
            base = (GblType)pMeta->pBases[depth];
        } else if(depth == pMeta->depth) {
            base = (GblType)pMeta;
        }
    }
    GBL_API_END_BLOCK();
    return base;
}

GBL_EXPORT GblType GblType_ancestor(GblType type, GblUint level) {
    GblType ancestor = GBL_INVALID_TYPE;
    GblMetaClass* pMeta =  GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    if(pMeta) {
        if(level == 0) {
            ancestor = (GblType)pMeta;
        } else if(level <= pMeta->depth) {
            ancestor = (GblType)pMeta->pBases[pMeta->depth-level];
        }
    }
    GBL_API_END_BLOCK();
    return ancestor;
}

GBL_EXPORT GblUint GblType_depth(GblType type) {
    GblUint depth = 0;
    GBL_API_BEGIN(pCtx_);
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    if(pMeta) {
        depth = pMeta->depth;
    }
    GBL_API_END_BLOCK();
    return depth;
}

GBL_EXPORT GblBool GblType_flagsTest(GblType type, GblFlags mask) {
    GblBool result = GBL_FALSE;
    GBL_API_BEGIN(pCtx_); {
        GblMetaClass* pMeta = GBL_META_CLASS_(type);
        if(pMeta) {
            GblFlags typeMask = (mask & GBL_TYPE_FLAGS_MASK & pMeta->flags);
            GblFlags fundamentalMask = (mask & GBL_TYPE_FUNDAMENTAL_FLAGS_MASK);
            if(fundamentalMask) {
                GblMetaClass* pFundamental = (GblMetaClass*)GblType_fundamental(type);
                if(pFundamental) {
                    fundamentalMask &= pFundamental->flags;
                } else {
                    fundamentalMask = 0; // FLAGS FOR INVALID TYPE
                }
            }
            result = ((typeMask | fundamentalMask) != 0);
        }
    } GBL_API_END_BLOCK();
    return result;
}

GBL_EXPORT const GblTypeInfo* GblType_info(GblType type) {
    const GblTypeInfo* pInfo      = NULL;
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    if(pMeta) pInfo = &pMeta->info;
    GBL_API_END_BLOCK();
    return pInfo;
}

GBL_EXPORT GblIPlugin* GblType_plugin(GblType type) {
    return type == GBL_INVALID_TYPE? NULL : GBL_META_CLASS_(type)->pPlugin;
}


GBL_EXPORT GblContext* GblType_contextDefault(void) {
    return pCtx_;
}

GBL_EXPORT GblContext* GblType_context(GblType type) {
    GBL_UNUSED(type);
    return GblType_contextDefault();
}

static GblBool typeIsA_(GblType derived, GblType base, GblBool classChecks, GblBool ifaceChecks) {
    GblBool         result   = 0;
    GblMetaClass*   pDerived = GBL_META_CLASS_(derived);
    GblMetaClass*   pBase    = GBL_META_CLASS_(base);
    GblMetaClass*   pIter    = pDerived;

    GBL_API_BEGIN(pCtx_);
    if(derived == GBL_INVALID_TYPE && base == GBL_INVALID_TYPE) {
        result = 1;
    } else {
        // Are we even checking for an interface type!?
        if(!GBL_TYPE_IS_INTERFACE(base)) ifaceChecks = GBL_FALSE;

        while(pIter) {
            // check if current class level is base
            if(pIter == pBase) {
                result = classChecks? GBL_TRUE : GBL_FALSE;
                break;
            } else if(ifaceChecks) {
                // recurse over interfaces checking
                for(GblSize i = 0; i < pIter->info.interfaceCount; ++i) {
                    if(typeIsA_(pIter->info.pInterfaceMap[i].interfaceType, base, classChecks, ifaceChecks)) {
                        result = GBL_TRUE;
                        GBL_API_DONE();
                    }
                }
            }
            pIter = pIter->pParent;
        }
    }
    GBL_API_END_BLOCK();
    return result;
}

GBL_EXPORT GblBool GblType_check(GblType type, GblType other) {
    return typeIsA_(type, other, GBL_TRUE, GBL_TRUE);
}

// subtyping is inclusive
GBL_EXPORT GblBool GblType_subsumes(GblType derived, GblType base) {
    return typeIsA_(derived, base, GBL_TRUE, GBL_FALSE);
}

GBL_EXPORT GblBool GblType_implements(GblType concrete, GblType iface) {
    return typeIsA_(concrete, iface, GBL_FALSE, GBL_TRUE);
}


// ==== MAKE SURE TO ITERATE OVER PARENTS AND CHECK THAT CLASS/INSTANCE SIZE IS VALID =====
GBL_EXPORT GblType GblType_registerStatic(GblType                    parent,
                                        const char*                  pName,
                                        const GblTypeInfo*           pInfo,
                                        GblFlags                     flags)
{
    GblType newType = GBL_INVALID_TYPE;
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_POINTER(pName);
    GBL_API_VERIFY_POINTER(pInfo);

    GBL_API_PUSH_VERBOSE("[GblType] Register Static Type: %s", pName);

    GBL_API_VERIFY_ARG(GblType_fromName(pName) == GBL_INVALID_TYPE,
                        "Existing entry for the given type name was found!");

    GBL_API_VERIFY_ARG(!(flags & GBL_TYPE_FLAG_BUILTIN),
                       "Cannot register a builtin type as a static type!");

    GBL_TYPE_ENSURE_INITIALIZED_();

    newType = typeRegister_(parent, pName, pInfo, flags);

    GBL_API_END_BLOCK();
    return newType;
}

GBL_EXPORT GBL_RESULT GblType_unregister(GblType type) {
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_ARG(type != GBL_INVALID_TYPE);
    GBL_API_PUSH_VERBOSE("[GblType] Unregister: %s", pMeta->pName);
    GBL_TYPE_ENSURE_INITIALIZED_();
    {
        GblRefCount refCount = GblClass_refCountFromType(type);
        if(refCount) {
            GBL_API_WARN("Attempting to unregister type with active class references: %u", refCount);
        }

#ifdef GBL_TYPE_DEBUG
        refCount = GblInstance_refCountFromType(type);
        if(refCount) {
            GBL_API_WARN("Attempting to unregister type with active class references: %u", refCount);
        }
#endif
        pthread_mutex_lock(&typeRegMtx_);
        const GblBool success = GblHashSet_erase(&typeRegistry_, &pMeta);
        pthread_mutex_unlock(&typeRegMtx_);
        GBL_API_VERIFY(success, GBL_RESULT_ERROR_INVALID_TYPE, "Failed to remove the type from the registry HashSet!");
    }
    GBL_API_POP(1);
    GBL_API_END();
}

GBL_EXPORT GblType GblType_fromClass(const GblClass* pClass) {
    GblType type = GBL_INVALID_TYPE;
    GBL_API_BEGIN(pCtx_);
    if(pClass) {
        type = GBL_CLASS_TYPE_(pClass);
    }
    GBL_API_END_BLOCK();
    return type;
}

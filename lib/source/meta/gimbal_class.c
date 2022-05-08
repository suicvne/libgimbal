#include <gimbal/meta/gimbal_class.h>
#include <gimbal/meta/gimbal_instance.h>
#include <gimbal/meta/gimbal_interface.h>
#include "gimbal/core/gimbal_api_frame.h"
#include "gimbal_type_.h"

GBL_EXPORT GblClass* GblClass_fromInstance(const GblInstance* pInstance) {
    GblClass* pClass = NULL;
    GBL_API_BEGIN(GblType_contextDefault());
    if(pInstance) {
        pClass = pInstance->pClass;
    }
    GBL_API_END_BLOCK();
    return pClass;
}

GBL_EXPORT GblBool GblClass_isFloating(const GblClass* pSelf) {
    GblBool floating = GBL_FALSE;
    if(pSelf) {
       GblMetaClass* pMeta = GBL_CLASS_META_CLASS_(pSelf);
       if(!pMeta->pClass || pMeta->pClass != pSelf)
           floating = GBL_TRUE;
    }
    return floating;
}

GBL_EXPORT GblBool GblClass_isInterfaceImpl(const GblClass* pSelf) {
    return pSelf? (GblBool)(GBL_CLASS_FLAGS_(pSelf) & GBL_CLASS_FLAG_IFACE_IMPL_) : GBL_FALSE;
}

GBL_EXPORT GblBool GblClass_isOwned(const GblClass* pSelf) {
    return pSelf? (GblBool)(GBL_CLASS_FLAGS_(pSelf) & GBL_CLASS_FLAG_OWNED_) : GBL_FALSE;
}

GBL_EXPORT GblBool GblClass_isInPlace(const GblClass* pSelf) {
    return pSelf? (GblBool)(GBL_CLASS_FLAGS_(pSelf) & GBL_CLASS_FLAG_IN_PLACE_) : GBL_FALSE;
}

static GBL_RESULT gblTypeClassConstruct_(GblClass* pClass, GblMetaClass* pMeta, GblFlags classFlags);

static GBL_RESULT gblTypeInterfaceClassConstruct_(GblInterface* pClass, GblMetaClass* pMeta, int16_t offset) {
    GBL_API_BEGIN(pCtx_);
    GBL_API_PUSH_VERBOSE("InterfaceClass::construct(%s)", GblType_name(GBL_TYPE_(pMeta)));

    GBL_API_VERBOSE("offset: %d", offset);
    pClass->outerClassOffset = offset;
    GBL_API_PUSH_VERBOSE("Default Class Ref");
    void* pInterfaceClass = GblClass_refFromType((GblType)pMeta);
    GBL_API_POP(1);
    GBL_API_VERIFY_EXPRESSION(pInterfaceClass);
    GBL_API_CALL(gblTypeClassConstruct_(&pClass->base, pMeta, GBL_CLASS_FLAG_IFACE_IMPL_));
    GBL_API_POP(1);
    GBL_API_END();
}

/*
 * Constructor initializes immediate data (typeId) then itereates over all bases,
 * initializing them from top to bottom. Each base has its interfaces initialized
 * before the rest of its class.
 */
static GBL_RESULT gblTypeClassConstruct_(GblClass* pClass, GblMetaClass* pMeta, GblFlags classFlags) {
    GBL_API_BEGIN(pCtx_);
    GBL_API_PUSH_VERBOSE("Class::construct(%s)", GblType_name(GBL_TYPE_(pMeta)));
    GBL_API_VERBOSE("Type: %p", pMeta);

    // Zero initiailze class - NOPE OVERWRITING IFACE IF WE DO THAT!
    //memset(pClass, 0, pMeta->info.classSize);

    //IMMEDIATELY initialize its type!!!
    /*
    GBL_API_VERIFY_EXPRESSION(pClass == pMeta->pClass || (classFlags & GBL_CLASS_FLAG_FLOATING_),
                              "Floating class detected but not flagged!");
    */
    GBL_CLASS_PRIVATE_SET_(pClass, pMeta, classFlags);

    if(pMeta->pParent) {
        //GBL_API_PUSH_VERBOSE("Adding reference to parent class: ", pMeta->pParent->pName);
        //ensure construction of parent class (this will recurse)
        GblClass_refFromType((GblType)pMeta->pParent);
        //GBL_API_POP(1);
    } else {
        GBL_API_VERBOSE("Parent Class Ref: None");
    }

    for(uint8_t idx = 0; idx <= pMeta->depth; ++idx) {
        GblMetaClass* pIter = GBL_META_CLASS_(GblType_base((GblType)pMeta, idx));
        GBL_API_PUSH_VERBOSE("Class[%u]: %s", idx, GblType_name(GBL_TYPE_(pIter)));

        //GBL_API_PUSH_VERBOSE("Constructing Interfaces");
        if(!pIter->info.interfaceCount) {
            //GBL_API_VERBOSE("Interfaces: None");
        } else {
            for(GblSize i = 0; i < pIter->info.interfaceCount; ++i) {
                const GblTypeInterfaceMapEntry* pIEntry = &pIter->info.pInterfaceMap[i];
                GblMetaClass* pIMeta = (GblMetaClass*)pIEntry->interfaceType;
                GBL_API_VERIFY_EXPRESSION(pIMeta);
                GBL_API_PUSH_VERBOSE("Interface[%u]: %s", i, GblType_name(GBL_TYPE_(pIMeta)));
                GblInterface* pIClass = (GblInterface*)((char*)pClass + pIEntry->classOffset);
                GBL_API_CALL(gblTypeInterfaceClassConstruct_(pIClass, pIMeta, -pIEntry->classOffset));
                GBL_API_POP(1);
            }
        }
        //GBL_API_POP(1);

        if(pIter->info.pFnClassInit) {
            //GBL_API_VERBOSE("ClassInit(%s)", GblType_name(GBL_TYPE_(pIter)));
            GBL_API_CALL(pIter->info.pFnClassInit(pClass, pMeta->info.pClassData, pCtx_));
        } else {
            //GBL_API_VERBOSE("Ctor: NULL", GblType_name(GBL_TYPE_(pIter)));
        }

        GBL_API_POP(1);
    }

   // GBL_API_POP(1);
    GBL_API_POP(1);
    GBL_API_END();
}


static GblClass* gblTypeClassCreate_(GblMetaClass* pMeta, GblBool floating) {
    GblClass* pFloatingClass = NULL;
    GblClass** ppClass       = &pFloatingClass;
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_ARG(pMeta);
    //GBL_API_PUSH_VERBOSE("Class::create(%s)", GblType_name(GBL_TYPE_(pMeta)));

    if(!floating) {
        GBL_API_VERIFY_EXPRESSION(!atomic_load(&pMeta->refCount),
                                  "Already have a reference to an invalid class object!");

        ppClass = &pMeta->pClass;
    }

    //Allocate a new class structure if one isn't already available

    if(!*ppClass) {
        GBL_API_VERBOSE("Allocating separate class structure!");
        *ppClass = GBL_API_MALLOC(gblAlignedAllocSize(pMeta->info.classSize),
                                  GBL_ALIGNOF(max_align_t),
                                  GblType_name(GBL_TYPE_(pMeta)));
    //Class must've been allocated with Meta class. Use existing data.
    } else {
       //GBL_API_VERBOSE("Using existing inline class allocation.");
    }

    // Call constructor
    GBL_API_CALL(gblTypeClassConstruct_(*ppClass, pMeta, 0));

    //GBL_API_POP(1);
    GBL_API_END_BLOCK();
    return *ppClass;
}

GBL_EXPORT GblClass* GblClass_refFromType(GblType type) GBL_NOEXCEPT {
    GblClass* pClass    = NULL;
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    GBL_API_BEGIN(GblType_contextDefault());
    GBL_API_VERIFY_ARG(type != GBL_INVALID_TYPE);

    GBL_API_VERIFY(pMeta->info.classSize != 0,
                   GBL_RESULT_UNIMPLEMENTED,
                   "[GblType] Attempt to reference a class of size 0!");

    //GBL_API_PUSH_VERBOSE("Class::reference(%s)", GblType_name(GBL_TYPE_(pMeta)));
    GBL_TYPE_ENSURE_INITIALIZED_();

    // Return existing reference to class data
    if(pMeta->pClass && GBL_CLASS_TYPE(pMeta->pClass) != GBL_INVALID_TYPE) {
        GBL_API_VERIFY_EXPRESSION(atomic_load(&pMeta->refCount),
                                  "No references to an initialized class!?");
        //GBL_API_VERBOSE("Using existing class data");
        pClass = pMeta->pClass;

    // Create a new class structure
    } else {
        pClass = gblTypeClassCreate_(pMeta, GBL_FALSE);
        GBL_API_VERIFY_EXPRESSION(pClass && pClass == pMeta->pClass, "Failed to create class!");
    }

    // Either way, we're returning a new reference, add refcount
    GblRefCount oldCount = atomic_fetch_add(&pMeta->refCount, 1);
    GBL_API_VERBOSE("++[%s].refCount: %"GBL_SIZE_FMT, GblType_name(GBL_TYPE_(pMeta)), oldCount+1);

    //GBL_API_POP(1);
    GBL_API_END_BLOCK();
    return pClass;
}

GBL_EXPORT GblClass* GblClass_peekFromType(GblType type) GBL_NOEXCEPT {
    GblClass*       pClass  = NULL;
    GblMetaClass*   pMeta   = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_ARG(type != GBL_INVALID_TYPE);
    GBL_API_VERIFY(pMeta->pClass, GBL_RESULT_ERROR_INVALID_OPERATION,
                  "[GblType] gblClassPeek(%s): no class!", GblType_name(type));
    GBL_API_VERIFY(pMeta->refCount, GBL_RESULT_ERROR_INVALID_OPERATION,
                   "Cannot peek into uninitialized class: %s", GblType_name(type));
    pClass = pMeta->pClass;

    GBL_API_END_BLOCK();
    return pClass;
}

GBL_EXPORT GBL_RESULT GblClass_constructFloating(GblClass* pSelf, GblType type) GBL_NOEXCEPT {
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_ARG(type != GBL_INVALID_TYPE);
    GBL_API_VERIFY_POINTER(pSelf);
    GblClass_refFromType(type); // have to reference actual type
    GBL_API_CALL(gblTypeClassConstruct_(pSelf,
                                        GBL_META_CLASS_(type),
                                        GBL_CLASS_FLAG_IN_PLACE_));
    GBL_API_END();
}

GBL_EXPORT GblClass* GblClass_createFloating(GblType type) GBL_NOEXCEPT {
    GblClass* pClass = NULL;
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_ARG(type != GBL_INVALID_TYPE);
    GblClass_refFromType(type);
    pClass = gblTypeClassCreate_(GBL_META_CLASS_(type), GBL_TRUE);
    GBL_API_END_BLOCK();
    return pClass;
}


static GBL_EXPORT GBL_RESULT gblTypeClassDestruct_(GblClass* pClass) {
    GblMetaClass* pMeta = GBL_META_CLASS_(GBL_CLASS_TYPE(pClass));
    GBL_API_BEGIN(pCtx_);
    GBL_API_DEBUG("Destroying %s class!", GblType_name(GBL_TYPE_(pMeta)));
    GBL_API_PUSH();
    GblMetaClass* pIter = pMeta;
    // walk up the destructor chain
    GBL_API_PUSH_VERBOSE("Walking class destructors.");
    do {
        if(pIter->info.pFnClassFinal) {
            GBL_API_DEBUG("Calling class dtor: [%s]", GblType_name(GBL_TYPE_(pIter)));
            GBL_API_CALL(pIter->info.pFnClassFinal(pMeta->pClass, pMeta->info.pClassData, pCtx_));
        } else {
            GBL_API_DEBUG("No class dtor: [%s]", GblType_name(GBL_TYPE_(pMeta)));
        }

        for(GblSize i = 0; i < pIter->info.interfaceCount; ++i) {
            GblInterface* pInterface = (GblInterface*)((const char*)pClass + pIter->info.pInterfaceMap[i].classOffset);
            GblClass* pDefaultIFaceClass = GblClass_peekFromType(GBL_CLASS_TYPE(pInterface));
            GblClass_unref(pDefaultIFaceClass);

            gblTypeClassDestruct_(GBL_CLASS(pInterface));
            //GblClass_unref(GBL_CLASS(pInterface));
            // unreference default interface class too!
            //GblClass_unref(GblClass_peekFromType(GBL_CLASS_TYPE(pInterface)));
        }
        pIter = pIter->pParent;
    } while(pIter);
    GBL_API_POP(1);

    // Store parent pointer then deallocate
    GblClass* pParentClass = pMeta->pParent? pMeta->pParent->pClass : NULL;
   // GBL_API_FREE(pMeta->pClass);

    //Release reference to parent class;
    if(pParentClass) GblClass_unref(pParentClass);

    // clear the type so it's not looking initialized
    GBL_CLASS_PRIVATE_SET_(pClass, GBL_INVALID_TYPE, 0);

    GBL_API_POP(1);
    GBL_API_END();
}

GBL_EXPORT GBL_RESULT GblClass_destructFloating(GblClass* pSelf) {
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_POINTER(pSelf);
    const GblType type = GBL_CLASS_TYPE(pSelf);
    GBL_API_CALL(gblTypeClassDestruct_(pSelf));
    GblClass_unref(GblClass_peekFromType(type));
    GBL_API_END();
}

GBL_EXPORT GBL_RESULT GblClass_destroyFloating(GblClass* pSelf) {
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_CALL(GblClass_destructFloating(pSelf));
    GBL_API_FREE(pSelf);
    GBL_API_END();
}

GBL_EXPORT GblRefCount GblClass_unref(GblClass* pSelf) GBL_NOEXCEPT {
    GblRefCount refCount    = 0;
    GblMetaClass* pMeta     = NULL;
    GBL_API_BEGIN(pCtx_);
    if(!pSelf) GBL_API_DONE(); //valid to Unref NULL pointer

    GBL_API_VERIFY(GBL_CLASS_TYPE(pSelf) != GBL_INVALID_TYPE,
                   GBL_RESULT_ERROR_INTERNAL,
                   "Class::unreference(): The specified class has an invalid ID!");

    pMeta = GBL_META_CLASS_(GBL_CLASS_TYPE(pSelf));

    GBL_API_PUSH_VERBOSE("Class::unreference(%s): %u",
                         GblClass_typeName(pSelf),
                         pMeta->refCount);

    GBL_API_VERIFY(atomic_load(&pMeta->refCount) != 0,
                   GBL_RESULT_ERROR_INTERNAL,
                   "The refcount for the given class was already at 0!");

    refCount = atomic_fetch_sub(&pMeta->refCount, 1);

    if(refCount-1) {
        GBL_API_VERBOSE("--[%s].refCount: %u", GblType_name(GBL_TYPE_(pMeta)), refCount-1);
    } else {
#ifdef GBL_TYPE_DEBUG
        {
            GblRefCount instanceRefCount = 0;
            instanceRefCount = atomic_load(&pMeta->instanceRefCount);
        GBL_API_VERIFY_EXPRESSION(instanceRefCount > 0,
                                  "Cannot destroy class when there are still %u instances using it!", instanceRefCount)
        }
#endif

        GBL_API_CALL(gblTypeClassDestruct_(pSelf));
    }

    GBL_API_POP(1);
    GBL_API_END_BLOCK();
    return refCount;
}


static GblInterface* typeClassInterfacePeek_(GblClass* pClass, GblType ifaceType) {
    GblInterface* pIClass       = NULL;
    GblMetaClass* pIFaceMeta    = GBL_META_CLASS_(ifaceType);
    GBL_API_BEGIN(pCtx_);
    GBL_API_VERIFY_POINTER(pClass);
    GBL_API_VERIFY_EXPRESSION(GBL_CLASS_TYPE(pClass) != GBL_INVALID_TYPE);
    GBL_API_VERIFY_ARG(ifaceType != GBL_INVALID_TYPE);
    GBL_API_VERIFY_ARG(GBL_TYPE_IS_INTERFACE(ifaceType));
    {
        GblMetaClass* pMeta = GBL_META_CLASS_(GBL_CLASS_TYPE(pClass));
        while(pMeta) {
            for(unsigned i = 0; i < pMeta->info.interfaceCount; ++i) {
                GblInterface* pCurIClass = (GblInterface*)((uintptr_t)pClass + pMeta->info.pInterfaceMap[i].classOffset);
                GBL_API_VERIFY_EXPRESSION(GBL_META_CLASS_(pMeta->info.pInterfaceMap[i].interfaceType) == GBL_META_CLASS_(GBL_CLASS_TYPE(pCurIClass)));
                GBL_API_VERIFY_EXPRESSION(GBL_CLASS_TYPE(pCurIClass) != GBL_INVALID_TYPE);
                if(GBL_META_CLASS_(GBL_CLASS_TYPE(pCurIClass)) == pIFaceMeta) {
                    pIClass = pCurIClass;
                    break;
                } else {
                    pCurIClass = typeClassInterfacePeek_(GBL_CLASS(pCurIClass), ifaceType);
                    if(pCurIClass) {
                        GBL_API_VERIFY_EXPRESSION(GBL_META_CLASS_(GBL_CLASS_TYPE(pCurIClass)) == pIFaceMeta);
                        pIClass = pCurIClass;
                        break;
                    }
                }
            }
            pMeta = pMeta->pParent;
        }
    }
    GBL_API_END_BLOCK();
    return pIClass;
}


static GblClass* typeClassCast_(GblClass* pClass, GblType toType, GblBool check) {
    GblClass* pClassStart = pClass;
    GblClass* pToClass = NULL;
    GBL_API_BEGIN(pCtx_);

    // Casting a NULL pointer or to an invalid type returns NULL
    if(pClass && toType != GBL_INVALID_TYPE) GBL_LIKELY {
        //early exit when class is already of desired type
        if(GBL_CLASS_META_CLASS_(pClass) == GBL_META_CLASS_(toType)) GBL_LIKELY {
            pToClass = pClass;
        } else {
            GblBool toInterface = GBL_FALSE;
            if(GBL_TYPE_IS_INTERFACE(toType)) toInterface = GBL_TRUE;

            // If current node is an interface class, attempt to "back out" to root class node
            while(GBL_TYPE_IS_INTERFACE(GBL_CLASS_TYPE(pClass))) {
                // Move class pointer to interface's concrete class
                pClass = GblInterface_outerClass((GblInterface*)pClass);
                GBL_API_VERIFY_EXPRESSION(pClass);
            }

            GblMetaClass* pMeta = GBL_CLASS_META_CLASS_(pClass);
            GBL_API_VERIFY_EXPRESSION(pMeta);

            // iterate from derived to base class, breadth-first searching
            do {
                //check current class
                if(pMeta == GBL_META_CLASS_(toType)) {
                    pToClass = pClass;
                    break;
                }
                pMeta = pMeta->pParent;
            } while(pMeta);

            if(!pToClass && toInterface) {
                GblInterface* pInterface = typeClassInterfacePeek_(pClass, toType);
                if(pInterface) {
                    pToClass = GBL_CLASS(pInterface);
                }
            }
        }

    }
    GBL_API_END_BLOCK();
    if(check && !pToClass) {
        if(toType == GBL_INVALID_TYPE) {
            GBL_API_WARN("Attempted to cast from type %s to GBL_INVALID_TYPE!",
                         GblClass_typeName(pClassStart));
        } else {
            GBL_API_WARN("Failed to cast from type %s to %s!",
                         GblClass_typeName(pClassStart), GblType_name(toType));
        }
    }
    return pToClass;
}

GBL_EXPORT GblBool GblClass_check(const GblClass* pSelf, GblType toType) {
    GblBool result = GBL_FALSE;
    GBL_API_BEGIN(pCtx_);
    if(!(!pSelf && toType == GBL_INVALID_TYPE)) {
        GBL_API_VERIFY_POINTER(pSelf);
        result = GblType_check(GBL_CLASS_TYPE(pSelf), toType);
    }
    GBL_API_END_BLOCK();
    return result;
}

GBL_EXPORT GblClass* GblClass_cast(GblClass* pSelf, GblType toType) GBL_NOEXCEPT {
    return typeClassCast_(pSelf, toType, GBL_TRUE);
}
GBL_EXPORT GblClass* GblClass_try(GblClass* pSelf, GblType toType) GBL_NOEXCEPT {
    return typeClassCast_(pSelf, toType, GBL_FALSE);
}

GBL_EXPORT GblRefCount GblClass_refCountFromType(GblType type) GBL_NOEXCEPT {
    GblRefCount refCount      = 0;
    GblMetaClass* pMeta = GBL_META_CLASS_(type);
    GBL_API_BEGIN(pCtx_);
    if(pMeta) refCount = atomic_load(&pMeta->refCount);
    GBL_API_END_BLOCK();
    return refCount;
}



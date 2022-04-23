#include <gimbal/ifaces/gimbal_ivariant.h>
#include <gimbal/types/gimbal_variant.h>
#include <gimbal/meta/gimbal_variant.h>

extern GBL_RESULT GblIVariant_typeRegister_(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    gblTypeRegisterBuiltin(1,
    GBL_TYPE_INTERFACE,
    "IVariant",
    &((const GblTypeInfo) {
        .classSize    = sizeof(GblIVariantIFace),
        .classAlign   = GBL_ALIGNOF(GblIVariantIFace),
    }),
    GBL_TYPE_FLAG_ABSTRACT);
    GBL_API_END();
}

GBL_API GblIVariantIFace_validate(const GblIVariantIFace* pSelf) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);

    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_MASK)
    {
        GBL_API_VERIFY_POINTER(pSelf->pFnConstruct,
                               "Pure virtual constructor with constructor ops is not allowed!");
    }
    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_MASK)
    {
        GBL_API_VERIFY_POINTER(pSelf->pFnSet,
                               "Pure virtual set function with set ops is not allowed!");
    }
    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_GET_MASK)
    {
        GBL_API_VERIFY_POINTER(pSelf->pFnGet,
                               "Pure virtual get function with get ops is not allowed!");
    }

    if(!(pSelf->supportedOps & (GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                                GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY)))
    {
        GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_TYPE,
                           "Type has no way of having its value set!");
    }

    if(!(pSelf->supportedOps & (GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY |
                                GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK)))
    {
        GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_TYPE,
                           "Type has no way of having its value retrieved!");
    }

    if(!(pSelf->supportedOps & (GBL_IVARIANT_OP_FLAG_SET_COPY |
                                GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY |
                                GBL_IVARIANT_OP_FLAG_RELOCATABLE)))
    {
        GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_TYPE,
                           "Type has no way of being copied!");
    }
    GBL_API_END();
}


GBL_API GblIVariantIFace_constructDefault(const GblIVariantIFace* pSelf, GblVariant* pVariant) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pVariant);
    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
        GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
        GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
    }
    GBL_API_END();
}


/* Order of operations
 * 1) Try copy constructor
 * 2) Try default constructor + copy assignment
 * 3) Shit pants
 */
GBL_API GblIVariantIFace_constructCopy(const GblIVariantIFace* pSelf, GblVariant* pVariant, const GblVariant* pOther) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pVariant);
    GBL_API_VERIFY_POINTER(pOther);
    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY) {
        GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
        GBL_API_CALL(pSelf->pFnConstruct(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY));
    } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_COPY) {
        GBL_API_VERIFY_POINTER(pSelf->pFnSet);
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
            GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
            GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
        }
        GBL_API_CALL(pSelf->pFnSet(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_SET_COPY));
    } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_RELOCATABLE) {
        memcpy(pVariant, pOther, sizeof(GblVariant));
    } else {
        GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                           "[GblIVariant] Cannot copy construct type: %s",
                           gblTypeName(pVariant->type));
    }
    GBL_API_END();
}

/* Attempt to move construct a variant from another
 *  Order of operations
 * 1) Try move constructor
 * 2) Try default constructor + move assignment
 * 3) Try copy constructor
 * 4) Try default constructor + copy assignment
 * 5) Try memcpy
 * 6) Shit pants
 */
GBL_API GblIVariantIFace_constructMove(const GblIVariantIFace* pSelf, GblVariant* pVariant, GblVariant* pOther) GBL_NOEXCEPT {
   GBL_API_BEGIN(NULL);
   GBL_API_VERIFY_POINTER(pSelf);
   GBL_API_VERIFY_POINTER(pVariant);
   GBL_API_VERIFY_POINTER(pOther);
   if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_MOVE) {
       GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
       GBL_API_CALL(pSelf->pFnConstruct(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_CONSTRUCT_MOVE));
   } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_MOVE) {
       GBL_API_VERIFY_POINTER(pSelf->pFnSet);
       if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
            GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
            GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
       }
       GBL_API_CALL(pSelf->pFnSet(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_SET_MOVE));
   } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY) {
        GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
        GBL_API_CALL(pSelf->pFnConstruct(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY));
   } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_COPY) {
       GBL_API_VERIFY_EXPRESSION(pSelf->pFnSet);
       if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
            GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
            GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
       }
       GBL_API_CALL(pSelf->pFnSet(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_SET_COPY));
   } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_RELOCATABLE) {
       memcpy(pVariant, pOther, sizeof(GblVariant));
   } else {
       GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                          "[GblIVariant] Cannot move construct type: %s",
                          gblTypeName(pVariant->type));
   }
   GBL_API_END();
}

GBL_API GblIVariantIFace_setCopy(const GblIVariantIFace* pSelf, GblVariant* pVariant, const GblVariant* pOther) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pVariant);
    GBL_API_VERIFY_POINTER(pOther);
    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_COPY) {
        GBL_API_VERIFY_POINTER(pSelf->pFnSet);
        GBL_API_CALL(pSelf->pFnSet(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_SET_COPY));
    } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_RELOCATABLE) {
        memcpy(pVariant, pOther, sizeof(GblVariant));
    } else {

        GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                           "[GblIVariant] Cannot copy assign type: %s",
                           gblTypeName(GblVariant_type(pVariant)));

    }
    GBL_API_END();
}

/* Order of operations
 * 1) Try move assignment
 * 2) Try copy assignment
 * 3) Try memcpy
 * 4) Shit pants
 */
GBL_API GblIVariantIFace_setMove(const GblIVariantIFace* pSelf, GblVariant* pVariant, GblVariant* pOther) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pVariant);
    GBL_API_VERIFY_POINTER(pOther);
    if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_MOVE) {
        GBL_API_VERIFY_POINTER(pSelf->pFnSet);
        GBL_API_CALL(pSelf->pFnSet(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_SET_MOVE));
    } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_COPY) {
        GBL_API_VERIFY_POINTER(pSelf->pFnSet);
        GBL_API_CALL(pSelf->pFnSet(pVariant, 1, pOther, GBL_IVARIANT_OP_FLAG_SET_COPY));
    } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_RELOCATABLE) {
        memcpy(pVariant, pOther, sizeof(GblVariant));
    } else {
        GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                           "[GblIVariant] Cannot move assign type: %s",
                           gblTypeName(GblVariant_type(pVariant)));
    }
    GBL_API_END();
}


GBL_API GblIVariantIFace_destruct(const GblIVariantIFace* pSelf, GblVariant* pVariant) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pVariant);
    if(pSelf->pFnDestruct) {
        GBL_API_CALL(pSelf->pFnDestruct(pVariant));
    }
    GBL_API_END();
}

GBL_API GblIVariantIFace_compare(const GblIVariantIFace* pSelf, const GblVariant* pVariant, const GblVariant* pOther, GblInt* pCmpResult) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pVariant);
    GBL_API_VERIFY_POINTER(pOther);
    GBL_API_VERIFY_POINTER(pCmpResult);
    if(pVariant->type != pOther->type) {
        *pCmpResult = INT_MAX;
    } else if(pSelf->pFnCompare) {
        GBL_API_CALL(pSelf->pFnCompare(pVariant, pOther, pCmpResult));
    } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_VALUELESS_TYPE) {
        *pCmpResult = 0;
    }
    GBL_API_END();
}

static GBL_RESULT GblIVariantIFace_valuesFromVa_(const GblIVariantIFace* pSelf, const char* pFmt, va_list* pArgList, GblUint* pCount, GblVariant pVariant[]) {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf);
    GBL_API_VERIFY_POINTER(pCount);
    GBL_API_VERIFY_POINTER(pVariant);
    *pCount = 0;
    char curChar;
    while((curChar = pFmt[*pCount]) != '\0') {
        GblVariant* pCurVariant = &pVariant[*pCount];
        switch(curChar) {
        case 'q': pCurVariant->type = GBL_TYPE_INT64;   pCurVariant->i64        = va_arg(*pArgList, int64_t);  break;
        case 'l': pCurVariant->type = GBL_TYPE_UINT32;  pCurVariant->u32        = va_arg(*pArgList, uint32_t); break;
        case 'i': pCurVariant->type = GBL_TYPE_INT32;   pCurVariant->i32        = va_arg(*pArgList, int32_t);  break;
        case 'd': pCurVariant->type = GBL_TYPE_DOUBLE;  pCurVariant->f64        = va_arg(*pArgList, double);   break;
        case 'p': pCurVariant->type = GBL_TYPE_POINTER; pCurVariant->pVoid      = va_arg(*pArgList, void*);    break;
        default: GBL_API_ERROR("[GblIVariant] Unknown va value type: %c", curChar); break;
        }

        if(*pCount + 1 >= GBL_IVARIANT_VALUE_VAR_ARG_MAX) break;
        ++(*pCount);
    }
    GBL_API_END();
}
/* 1) Try construct value copy
 * 2) Try set value copy (and maybe constructor)
 * 3) shit pants
 */
GBL_API GblIVariantIFace_constructValueCopy(const GblIVariantIFace* pSelf, GblVariant* pVariant, va_list* pArgs) {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pSetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
            GBL_API_CALL(pSelf->pFnConstruct(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY));
        } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnSet);
            if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
                GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
                GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
            }
            GBL_API_CALL(pSelf->pFnSet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot copy construct values for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}
/* 1) Try construct value move
 * 2) Try default constructor + set value move
 * 3) Try construct value copy
 * 4) Try default constructor + construct value copy
 * 5) Shit pants
 */
GBL_API GblIVariantIFace_constructValueMove(const GblIVariantIFace* pSelf, GblVariant* pVariant, va_list* pArgs) {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pSetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_MOVE) {
            GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
            GBL_API_CALL(pSelf->pFnConstruct(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_MOVE));
        } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_VALUE_MOVE) {
            GBL_API_VERIFY_POINTER(pSelf->pFnSet);
            if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
                GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
                GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
            }
            GBL_API_CALL(pSelf->pFnSet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_SET_VALUE_MOVE));
        } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
            GBL_API_CALL(pSelf->pFnConstruct(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY));
        } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnSet);
            if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
                GBL_API_VERIFY_POINTER(pSelf->pFnConstruct);
                GBL_API_CALL(pSelf->pFnConstruct(pVariant, 0, NULL, GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT));
            }
            GBL_API_CALL(pSelf->pFnSet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot move construct values for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}

GBL_API GblIVariantIFace_setValueCopy(const GblIVariantIFace* pSelf, GblVariant* pVariant, va_list* pArgs) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pSetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnSet);
            GBL_API_CALL(pSelf->pFnSet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot copy set values for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}
GBL_API GblIVariantIFace_setValueMove(const GblIVariantIFace* pSelf, GblVariant* pVariant, va_list* pArgs) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pSetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_VALUE_MOVE) {
            GBL_API_VERIFY_POINTER(pSelf->pFnSet);
            GBL_API_CALL(pSelf->pFnSet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_SET_VALUE_MOVE));
        } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnSet);
            GBL_API_CALL(pSelf->pFnSet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot move set values for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}


GBL_API GblIVariantIFace_getValueCopy(const GblIVariantIFace* pSelf, const GblVariant* pVariant, va_list* pArgs) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pGetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnGet);
            GBL_API_CALL(pSelf->pFnGet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot get values by copy for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}

GBL_API GblIVariantIFace_getValuePeek(const GblIVariantIFace* pSelf, const GblVariant* pVariant, va_list* pArgs) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pGetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK) {
            GBL_API_VERIFY_POINTER(pSelf->pFnGet);
            GBL_API_CALL(pSelf->pFnGet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot get values by peeking for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}

GBL_API GblIVariantIFace_getValueTake(const GblIVariantIFace* pSelf, const GblVariant* pVariant, va_list* pArgs) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL); {
        GBL_API_VERIFY_POINTER(pSelf);
        GblUint count = 0;
        GblVariant* pVariants = GBL_ALLOCA(sizeof(GblVariant)*GBL_IVARIANT_VALUE_VAR_ARG_MAX);
        GBL_API_CALL(GblIVariantIFace_valuesFromVa_(pSelf, pSelf->pGetValueFmt, pArgs, &count, pVariants));
        if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_GET_VALUE_TAKE) {
            GBL_API_VERIFY_POINTER(pSelf->pFnGet);
            GBL_API_CALL(pSelf->pFnGet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_GET_VALUE_TAKE));
        } else if(pSelf->supportedOps & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY) {
            GBL_API_VERIFY_POINTER(pSelf->pFnGet);
            GBL_API_CALL(pSelf->pFnGet(pVariant, count, pVariants, GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY));
        } else {
            GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblIVariant] Cannot get values by peeking for type: %s",
                               gblTypeName(GblVariant_type(pVariant)));
        }
    } GBL_API_END();
}

GBL_API GblIVariantIFace_save(const GblIVariantIFace* pSelf, const GblVariant* pVariant, GblString* pString) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf->pFnSave);
    GBL_API_CALL(pSelf->pFnSave(pVariant, pString));
    GBL_API_END();
}
GBL_API GblIVariantIFace_load(const GblIVariantIFace* pSelf, GblVariant* pVariant, const GblString* pString) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pSelf->pFnLoad);
    GBL_API_CALL(pSelf->pFnLoad(pVariant, pString));
    GBL_API_END();

}



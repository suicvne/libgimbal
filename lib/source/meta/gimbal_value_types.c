#include <gimbal/meta/gimbal_type.h>
#include <gimbal/core/gimbal_api_frame.h>
#include <gimbal/ifaces/gimbal_ivariant.h>
#include <gimbal/meta/gimbal_variant.h>
#include <gimbal/types/gimbal_string.h>
#include <gimbal/types/gimbal_variant.h>

typedef struct GblPrimitiveClass {
    GblClass            base;
    GblIVariantIFace    iVariantIFace;
} GblPrimitiveClass;

static GBL_RESULT GblPrimitiveClass_init(GblPrimitiveClass* pClass, GblIVariantIFace* pIFace, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    memcpy(&pClass->iVariantIFace.supportedOps,
           &pIFace->supportedOps,
           sizeof(GblIVariantIFace)-offsetof(GblIVariantIFace, supportedOps));
#if 0
    memcpy(&pClass->iVariantIFace.supportedOps,
           &pIFace->supportedOps,
           sizeof(GblIVariantIFace) - offsetof(GblIVariantIFace, supportedOps));
#endif
    GBL_API_END();
}

static GBL_RESULT nilSave_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_EXPRESSION(GblVariant_type(pVariant) == GBL_TYPE_NIL);
    GBL_API_CALL(gblStringFromNil(pString));
    GBL_API_END();
}

static GBL_RESULT nilLoad_(GblVariant* pVariant, const GblString* pString) {
    GblBool isNil = GBL_FALSE;
    GBL_API_BEGIN(pString->data.pCtx);
    GBL_API_CALL(gblStringToNil(pString, &isNil));
    GBL_API_VERIFY_EXPRESSION(isNil);
    GBL_API_VERIFY_TYPE(pVariant->type, GBL_TYPE_NIL);
    GBL_API_END();
}

static GBL_RESULT boolSave_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_EXPRESSION(GblVariant_type(pVariant) == GBL_TYPE_NIL);
    GBL_API_CALL(gblStringFromb(pString, pVariant->boolean));
    GBL_API_END();
}

static GBL_RESULT boolLoad_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(pString->data.pCtx);
    GBL_API_CALL(gblStringTob(pString, &pVariant->boolean));
    GBL_API_END();
}

static GBL_RESULT boolSet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    pVariant->boolean = pArgs->i32? GBL_TRUE : GBL_FALSE;
    GBL_API_END();
}

static GBL_RESULT boolGet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((GblBool*)pArgs->pVoidPtr) = pVariant->boolean;
    GBL_API_END();
}

static GBL_RESULT charSave_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "'%c'", pVariant->character);
    GBL_API_END();
}

static GBL_RESULT charLoad_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    const char* pStr = NULL;
    GblSize length = 0;
    GBL_API_CALL(gblStringLength(pString, &length));
    if(length == 3) {
        GBL_API_CALL(gblStringCStr(pString, &pStr));
        GBL_API_VERIFY_EXPRESSION(pStr[0] == '\'');
        GBL_API_VERIFY_EXPRESSION(pStr[2] == '\'');
        pVariant->character = pStr[1];
    }
    GBL_API_END();
}

static GBL_RESULT charSet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT32);
    pVariant->character = pArgs->i32;
    GBL_API_END();
}

static GBL_RESULT charGet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((char*)pArgs->pVoidPtr) = pVariant->character;
    GBL_API_END();
}


static GBL_RESULT u8Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%u", pVariant->u8);
    GBL_API_END();
}

static GBL_RESULT u8Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->u8 = (uint8_t)integer;
    GBL_API_END();
}

static GBL_RESULT u8Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT32);
    pVariant->u8 = (uint8_t)pArgs->i32;
    GBL_API_END();
}

static GBL_RESULT u8Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((uint8_t*)pArgs->pVoidPtr) = pVariant->u8;
    GBL_API_END();
}


static GBL_RESULT i16Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%d", pVariant->i16);
    GBL_API_END();
}

static GBL_RESULT i16Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->i16 = (int16_t)integer;
    GBL_API_END();
}

static GBL_RESULT i16Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT32);
    pVariant->i16 = (int16_t)pArgs->i32;
    GBL_API_END();
}

static GBL_RESULT i16Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((int16_t*)pArgs->pVoidPtr) = pVariant->i16;
    GBL_API_END();
}

static GBL_RESULT u16Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%u", pVariant->u16);
    GBL_API_END();
}

static GBL_RESULT u16Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->u16 = (uint16_t)integer;
    GBL_API_END();
}

static GBL_RESULT u16Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT32);
    pVariant->u16 = (uint16_t)pArgs->i32;
    GBL_API_END();
}

static GBL_RESULT u16Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((uint16_t*)pArgs->pVoidPtr) = pVariant->u16;
    GBL_API_END();
}

static GBL_RESULT i32Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%d", pVariant->i32);
    GBL_API_END();
}

static GBL_RESULT i32Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->i32 = (int32_t)integer;
    GBL_API_END();
}

static GBL_RESULT i32Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT32);
    pVariant->i32 = (int32_t)pArgs->i32;
    GBL_API_END();
}

static GBL_RESULT i32Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((int32_t*)pArgs->pVoidPtr) = pVariant->i32;
    GBL_API_END();
}

static GBL_RESULT u32Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%u", pVariant->u32);
    GBL_API_END();
}

static GBL_RESULT u32Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->u32 = (uint32_t)integer;
    GBL_API_END();
}

static GBL_RESULT u32Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_UINT32);
    pVariant->u32 = (uint32_t)pArgs->u32;
    GBL_API_END();
}

static GBL_RESULT u32Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((uint32_t*)pArgs->pVoidPtr) = pVariant->u32;
    GBL_API_END();
}


static GBL_RESULT i64Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%d", pVariant->i64);
    GBL_API_END();
}

static GBL_RESULT i64Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->i64 = (int64_t)integer;
    GBL_API_END();
}

static GBL_RESULT i64Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT64);
    pVariant->i64 = (int64_t)pArgs->i64;
    GBL_API_END();
}

static GBL_RESULT i64Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((int64_t*)pArgs->pVoidPtr) = pVariant->i64;
    GBL_API_END();
}

static GBL_RESULT u64Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    gblStringSprintf(pString, "%u", pVariant->u64);
    GBL_API_END();
}

static GBL_RESULT u64Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblInt integer = 0;
    GBL_API_CALL(gblStringToi(pString, &integer));
    pVariant->u64 = (uint64_t)integer;
    GBL_API_END();
}

static GBL_RESULT u64Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_INT64);
    pVariant->u64 = (uint64_t)pArgs->i64;
    GBL_API_END();
}

static GBL_RESULT u64Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((uint64_t*)pArgs->pVoidPtr) = pVariant->u64;
    GBL_API_END();
}

static GBL_RESULT f32Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_CALL(gblStringFromf(pString, pVariant->f32));
    GBL_API_END();
}

static GBL_RESULT f32Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_CALL(gblStringTof(pString, &pVariant->f32));
    GBL_API_END();
}

static GBL_RESULT f32Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_DOUBLE);
    pVariant->f32 = (float)pArgs->f64;
    GBL_API_END();
}

static GBL_RESULT f32Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((float*)pArgs->pVoidPtr) = pVariant->f32;
    GBL_API_END();
}

static GBL_RESULT f64Save_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_CALL(gblStringFromd(pString, pVariant->f64));
    GBL_API_END();
}

static GBL_RESULT f64Load_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_CALL(gblStringTod(pString, &pVariant->f64));
    GBL_API_END();
}

static GBL_RESULT f64Set_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_DOUBLE);
    pVariant->f64 = pArgs->f64;
    GBL_API_END();
}

static GBL_RESULT f64Get_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((double*)pArgs->pVoidPtr) = pVariant->f64;
    GBL_API_END();
}

static GBL_RESULT pSave_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_CALL(gblStringFromp(pString, pVariant->pVoid));
    GBL_API_END();
}

static GBL_RESULT pLoad_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GBL_API_CALL(gblStringToi(pString, (GblInt*)&pVariant->pVoid));
    GBL_API_END();
}

static GBL_RESULT pSet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_POINTER);
    pVariant->pVoid = pArgs->pVoid;
    GBL_API_END();
}

static GBL_RESULT pGet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    *((void**)pArgs->pVoidPtr) = pVariant->pVoid;
    GBL_API_END();
}

static GBL_RESULT stringConstruct_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pVariant);

    // Default constructor
    if(op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT) {
        GBL_API_CALL(gblStringConstruct(&pVariant->string, sizeof(GblString), NULL, NULL));

    // Copy constructor
    } else if(op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY) {
        GBL_API_VERIFY_ARG(argc == 1);
        GBL_API_VERIFY_TYPE(GblVariant_type(&pArgs[0]), GBL_TYPE_STRING);
        GBL_API_CALL(gblVectorConstruct_6(&pVariant->string.data,
                                          pArgs[0].string.data.pCtx,
                                          pArgs[0].string.data.elementSize,
                                          sizeof(GblString),
                                          pArgs[0].string.data.pBuffer,
                                          pArgs[0].string.data.size));
    // Move constructor
    } else if(op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_MOVE) {
        GBL_API_VERIFY_ARG(argc == 1);
        GBL_API_VERIFY_TYPE(GblVariant_type(&pArgs[0]), GBL_TYPE_STRING);
        GblSize outSize   = 0;
        char*   pOutPtr   = NULL;
        GBL_API_CALL(gblStringTake(&pArgs[0].string, &pOutPtr, &outSize));
        GBL_API_CALL(gblStringConstruct(&pVariant->string,
                                        sizeof(GblString),
                                        pArgs[0].string.data.pCtx,
                                        NULL));
        GBL_API_CALL(gblStringGive(&pVariant->string, pOutPtr, outSize));

    // Value copying constructor
    } else if(op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY) {
        GBL_API_VERIFY_ARG(argc == 1);
        GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
        //GBL_API_VERIFY_TYPE(pArgs[1].type, GBL_TYPE_INT32);
        //GBL_API_VERIFY_TYPE(pArgs[2].type, GBL_TYPE_POINTER);
        GblStringView view = {
            .pBuffer    = pArgs[0].pVoid,
            .size       = pArgs[0].pVoid? strlen(pArgs[0].pVoid) : 0
        };
        GBL_API_CALL(gblStringConstruct(&pVariant->string, sizeof(GblString), NULL, &view));

    // Value moving constructor
    } else if(op & GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_MOVE) {
        GBL_API_VERIFY_ARG(argc == 3);
        GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
        GBL_API_VERIFY_TYPE(pArgs[1].type, GBL_TYPE_INT32);
        GBL_API_VERIFY_TYPE(pArgs[2].type, GBL_TYPE_POINTER);
        GBL_API_CALL(gblStringConstruct(&pVariant->string, sizeof(GblString), pArgs[2].pVoid, NULL));
        GBL_API_CALL(gblStringGive(&pVariant->string, pArgs[0].pVoid, pArgs[1].i32));
    }
    GBL_API_END();
}

static GBL_RESULT stringDestruct_(GblVariant* pVariant) {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_TYPE(pVariant->type, GBL_TYPE_STRING);
    GBL_API_CALL(gblStringDestruct(&pVariant->string));
    GBL_API_END();
}

static GBL_RESULT stringSave_(const GblVariant* pVariant, GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblStringView view = { 0 };
    GBL_API_CALL(gblStringCStr(&pVariant->string, &view.pBuffer));
    GBL_API_CALL(gblStringAssign(pString, &view));
    GBL_API_END();
}

static GBL_RESULT stringLoad_(GblVariant* pVariant, const GblString* pString) {
    GBL_API_BEGIN(NULL);
    GblStringView view = { 0 };
    GBL_API_CALL(gblStringCStr(pString, &view.pBuffer));
    GBL_API_CALL(gblStringAssign(&pVariant->string, &view));
    GBL_API_END();
}

static GBL_RESULT stringSet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_TYPE(pArgs->type, GBL_TYPE_POINTER);
    if(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY) {
        GBL_API_VERIFY_ARG(argc == 1);
        GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
        //GBL_API_VERIFY_TYPE(pArgs[1].type, GBL_TYPE_INT32);
        //GBL_API_VERIFY_TYPE(pArgs[2].type, GBL_TYPE_POINTER);
        const GblStringView view = {
            .pBuffer = pArgs[0].pVoid,
            .size    = pArgs[0].pVoid ? strlen(pArgs[0].pVoid) : 0
        };
        GBL_API_CALL(gblStringAssign(&pVariant->string, &view));
    } else if(op & GBL_IVARIANT_OP_FLAG_SET_VALUE_MOVE) {
        GBL_API_VERIFY_ARG(argc == 3);
        GBL_API_CALL(gblStringGive(&pVariant->string, pArgs[0].pVoid, pArgs[0].i32));

    } else if(op & GBL_IVARIANT_OP_FLAG_SET_COPY) {
        GblStringView view;
        GBL_API_VERIFY_ARG(argc == 1);
        GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_STRING);
        GBL_API_CALL(gblStringCStr(&pArgs[0].string, &view.pBuffer));
        GBL_API_CALL(gblStringLength(&pArgs[0].string, &view.size));
        GBL_API_CALL(gblStringAssign(&pVariant->string, &view));

    } else if(op & GBL_IVARIANT_OP_FLAG_SET_MOVE) {
        GBL_API_VERIFY_ARG(argc == 1);
        GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_STRING);
        char* pBuffer = NULL;
        GblSize capacity = 0;
        GBL_API_CALL(gblStringTake(&pArgs[0].string, &pBuffer, &capacity));
        GBL_API_CALL(gblStringGive(&pVariant->string, pBuffer, capacity));
    }
    GBL_API_END();
}

static GBL_RESULT stringGet_(GblVariant* pVariant, GblUint argc, GblVariant* pArgs, GBL_IVARIANT_OP_FLAGS op) {
    GBL_API_BEGIN(NULL);
    GBL_UNUSED(argc);
    GBL_API_VERIFY_EXPRESSION(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY);
    GBL_API_VERIFY_TYPE(pArgs[0].type, GBL_TYPE_POINTER);
    // these two probably shouldn't be unified?
    if(op & (GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY | GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK))
    {
        GBL_API_CALL(gblStringCStr(&pVariant->string, pArgs->pVoidPtr));
    } else if(op & GBL_IVARIANT_OP_FLAG_GET_VALUE_TAKE) {
        GblSize capacity = 0;
        GBL_API_CALL(gblStringTake(&pVariant->string, pArgs->pVoidPtr, &capacity));
    }
    GBL_API_END();
}

static GBL_RESULT stringCompare_(const GblVariant* pVariant, const GblVariant* pOther, GblInt* pResult) {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_POINTER(pVariant);
    GBL_API_VERIFY_POINTER(pOther);
    GBL_API_VERIFY_POINTER(pResult);
    GBL_API_VERIFY_TYPE(pVariant->type, GBL_TYPE_STRING);
    GBL_API_VERIFY_TYPE(pOther->type, GBL_TYPE_STRING);
    GBL_API_CALL(gblStringCompare(&pVariant->string, &pOther->string, pResult));
    GBL_API_END();
}


extern GBL_RESULT gblValueTypesRegister_(GblContext* pCtx) {

    GBL_API_BEGIN(pCtx);
    GBL_API_PUSH_VERBOSE("[GblType] Registering Builtin Types");

    const GblType iVariantType = GBL_TYPE_IVARIANT;

    // =============== NIL ===============
    static GblIVariantIFace nilIVariantIFace = {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE |
                        GBL_IVARIANT_OP_FLAG_VALUELESS_TYPE,
        .pFnDestruct = NULL,
        .pFnSave = nilSave_,
        .pFnLoad = nilLoad_
    };
    gblTypeRegisterBuiltin(2,
      GBL_TYPE_INVALID,
      "nil",
      &((const GblTypeInfo) {
          .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
          .classSize    = sizeof(GblPrimitiveClass),
          .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
          .pClassData   = &nilIVariantIFace,
          .interfaceCount = 1,
          .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
               .interfaceType  = iVariantType,
               .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
          })
      }),
      GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== BOOL ===============
    static GblIVariantIFace boolIVariantIFace = {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "i"},
            .pGetValueFmt = { "p" },
            .pFnSet  = boolSet_,
            .pFnGet  = boolGet_,
            .pFnSave = boolSave_,
            .pFnLoad = boolLoad_
      };
    gblTypeRegisterBuiltin(3,
      GBL_TYPE_INVALID,
      "bool",
      &((const GblTypeInfo) {
          .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
          .classSize    = sizeof(GblPrimitiveClass),
          .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
          .pClassData   = &boolIVariantIFace,
          .interfaceCount = 1,
          .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
             .interfaceType   = iVariantType,
             .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
        })
      }),
      GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== CHAR ===============
    static const GblIVariantIFace charIVariantIFace = {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "i"},
            .pGetValueFmt = { "p" },
            .pFnSet  = charSet_,
            .pFnGet  = charGet_,
            .pFnSave = charSave_,
            .pFnLoad = charLoad_
      };
    gblTypeRegisterBuiltin(4,
          GBL_TYPE_INVALID,
          "char",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &charIVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType   = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

     // =============== UINT8 ===============
    static const GblIVariantIFace uint8IVariantIFace = {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "i"},
            .pGetValueFmt = { "p" },
            .pFnSet  = u8Set_,
            .pFnGet  = u8Get_,
            .pFnSave = u8Save_,
            .pFnLoad = u8Load_
    };
    gblTypeRegisterBuiltin(5,
      GBL_TYPE_INVALID,
      "uint8",
      &((const GblTypeInfo) {
          .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
          .classSize    = sizeof(GblPrimitiveClass),
          .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
          .pClassData   = &uint8IVariantIFace,
          .interfaceCount = 1,
          .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
               .interfaceType   = iVariantType,
               .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
          })
      }),
      GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== INT16 ===============
    static const GblIVariantIFace int16IVariantIFace = {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "i"},
        .pGetValueFmt = { "p" },
        .pFnSet  = i16Set_,
        .pFnGet  = i16Get_,
        .pFnSave = i16Save_,
        .pFnLoad = i16Load_
    };
    gblTypeRegisterBuiltin(6,
          GBL_TYPE_INVALID,
          "int16",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &int16IVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType   = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== UNT16 ===============
    static const GblIVariantIFace uint16IVariantIFace = {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "i"},
        .pGetValueFmt = { "p" },
        .pFnSet  = u16Set_,
        .pFnGet  = u16Get_,
        .pFnSave = u16Save_,
        .pFnLoad = u16Load_
    };
    gblTypeRegisterBuiltin(7,
          GBL_TYPE_INVALID,
          "uint16",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &uint16IVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType  = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== INT32 ===============
    const static GblIVariantIFace int32IVariantIFace = {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "i"},
            .pGetValueFmt = { "p" },
            .pFnSet  = i32Set_,
            .pFnGet  = i32Get_,
            .pFnSave = i32Save_,
            .pFnLoad = i32Load_
      };
    gblTypeRegisterBuiltin(8,
          GBL_TYPE_INVALID,
          "int32",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &int32IVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType  = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== UINT32 ===============
    const static GblIVariantIFace uint32IVariantIFace = {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "l"},
        .pGetValueFmt = { "p" },
        .pFnSet  = u32Set_,
        .pFnGet  = u32Get_,
        .pFnSave = u32Save_,
        .pFnLoad = u32Load_
    };
    gblTypeRegisterBuiltin(9,
          GBL_TYPE_INVALID,
          "uint32",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &uint32IVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType  = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== INT64 ===============
    const static GblIVariantIFace int64IVariantIFace =  {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "q"},
        .pGetValueFmt = { "p" },
        .pFnSet  = i64Set_,
        .pFnGet  = i64Get_,
        .pFnSave = i64Save_,
        .pFnLoad = i64Load_
    };
    gblTypeRegisterBuiltin(10,
          GBL_TYPE_INVALID,
          "int64",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &int64IVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType  = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== UINT64 ===============
    const static GblIVariantIFace uint64IVariantIFace =  {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "q"},
            .pGetValueFmt = { "p" },
            .pFnSet  = u64Set_,
            .pFnGet  = u64Get_,
            .pFnSave = u64Save_,
            .pFnLoad = u64Load_
      };
    gblTypeRegisterBuiltin(11,
          GBL_TYPE_INVALID,
          "uint64",
          &((const GblTypeInfo) {
              .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
              .classSize    = sizeof(GblPrimitiveClass),
              .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
              .pClassData   = &uint64IVariantIFace,
              .interfaceCount = 1,
              .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                   .interfaceType  = iVariantType,
                   .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
              })
          }),
          GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== ENUM  ===============
    const static GblIVariantIFace enumIVariantIFace =  {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "q"},
            .pGetValueFmt = { "p" },
            .pFnSet  = u64Set_,
            .pFnGet  = u64Get_,
            .pFnSave = u64Save_,
            .pFnLoad = u64Load_
      };
    gblTypeRegisterBuiltin(12,
                  GBL_TYPE_INVALID,
                  "enum",
                  &((const GblTypeInfo) {
                      .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
                      .classSize    = sizeof(GblPrimitiveClass),
                      .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
                      .pClassData   = &enumIVariantIFace,
                      .interfaceCount = 1,
                      .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                           .interfaceType  = iVariantType,
                           .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
                      })
                  }),
                  GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== FLAGS  ===============
    static const GblIVariantIFace flagsIVariantIFace =  {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "q"},
        .pGetValueFmt = { "p" },
        .pFnSet  = u64Set_,
        .pFnGet  = u64Get_,
        .pFnSave = u64Save_,
        .pFnLoad = u64Load_
    };
    gblTypeRegisterBuiltin(13,
                  GBL_TYPE_INVALID,
                  "flags",
                  &((const GblTypeInfo) {
                      .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
                      .classSize    = sizeof(GblPrimitiveClass),
                      .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
                      .pClassData   = &flagsIVariantIFace,
                      .interfaceCount = 1,
                      .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                           .interfaceType  = iVariantType,
                           .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
                      })
                  }),
                  GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== FLOAT ===============
    static const GblIVariantIFace floatIVariantIFace = {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "d"},
        .pGetValueFmt = { "p" },
        .pFnSet  = f32Set_,
        .pFnGet  = f32Get_,
        .pFnSave = f32Save_,
        .pFnLoad = f32Load_
    };
    gblTypeRegisterBuiltin(14,
                  GBL_TYPE_INVALID,
                  "float",
                  &((const GblTypeInfo) {
                      .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
                      .classSize    = sizeof(GblPrimitiveClass),
                      .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
                      .pClassData   = &floatIVariantIFace,
                      .interfaceCount = 1,
                      .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                           .interfaceType   = iVariantType,
                           .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
                      })
                   }),
                  GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== DOUBLE ===============
    static const GblIVariantIFace doubleIVariantIFace =  {
            .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                            GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                            GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
            .pSetValueFmt = { "d"},
            .pGetValueFmt = { "p" },
            .pFnSet  = f64Set_,
            .pFnGet  = f64Get_,
            .pFnSave = f64Save_,
            .pFnLoad = f64Load_
      };
    gblTypeRegisterBuiltin(15,
                  GBL_TYPE_INVALID,
                  "double",
                  &((const GblTypeInfo) {
                      .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
                      .classSize    = sizeof(GblPrimitiveClass),
                      .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
                      .pClassData   = &doubleIVariantIFace,
                      .interfaceCount = 1,
                      .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                           .interfaceType  = iVariantType,
                           .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
                      })
                  }),
                  GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== POINTER ===============
    static const GblIVariantIFace pointerIVariantIFace =  {
        .supportedOps = GBL_IVARIANT_OP_FLAG_RELOCATABLE    |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY,
        .pSetValueFmt = { "p"},
        .pGetValueFmt = { "p" },
        .pFnSet  = pSet_,
        .pFnGet  = pGet_,
        .pFnSave = pSave_,
        .pFnLoad = pLoad_
    };

    gblTypeRegisterBuiltin(16,
                  GBL_TYPE_INVALID,
                  "pointer",
                  &((const GblTypeInfo) {
                      .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
                      .classSize    = sizeof(GblPrimitiveClass),
                      .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
                      .pClassData   = &pointerIVariantIFace,
                      .interfaceCount = 1,
                      .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                           .interfaceType  = iVariantType,
                           .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
                      })
                  }),
                  GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    // =============== STRING ===============
    static const GblIVariantIFace stringIface = {
        .supportedOps = GBL_IVARIANT_OP_FLAG_CONSTRUCT_DEFAULT      |
                        GBL_IVARIANT_OP_FLAG_CONSTRUCT_COPY         |
                        GBL_IVARIANT_OP_FLAG_CONSTRUCT_MOVE         |
                        GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_COPY   |
                        GBL_IVARIANT_OP_FLAG_CONSTRUCT_VALUE_MOVE   |
                        GBL_IVARIANT_OP_FLAG_SET_COPY               |
                        GBL_IVARIANT_OP_FLAG_SET_MOVE               |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_COPY         |
                        GBL_IVARIANT_OP_FLAG_SET_VALUE_MOVE         |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_COPY         |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_PEEK         |
                        GBL_IVARIANT_OP_FLAG_GET_VALUE_TAKE,
        .pSetValueFmt   = { "p"},
        .pGetValueFmt   = { "p" },
        .pFnConstruct   = stringConstruct_,
        .pFnDestruct    = stringDestruct_,
        .pFnSet         = stringSet_,
        .pFnGet         = stringGet_,
        .pFnCompare     = stringCompare_,
        .pFnSave        = stringSave_,
        .pFnLoad        = stringLoad_
    };
    gblTypeRegisterBuiltin(17,
                  GBL_TYPE_INVALID,
                  "string",
                  &((const GblTypeInfo) {
                      .pFnClassInit = (GblTypeClassInitFn)GblPrimitiveClass_init,
                      .classSize    = sizeof(GblPrimitiveClass),
                      .classAlign   = GBL_ALIGNOF(GblPrimitiveClass),
                      .pClassData   = &stringIface,
                      .interfaceCount = 1,
                      .pInterfaceMap = &((const GblTypeInterfaceMapEntry) {
                             .interfaceType   = iVariantType,
                             .classOffset    = offsetof(GblPrimitiveClass, iVariantIFace)
                      })
                  }),
                  GBL_TYPE_FUNDAMENTAL_FLAG_CLASSED);

    gblTypeRegisterBuiltin(18,
                               GBL_TYPE_INVALID,
                               "type",
                               &((const GblTypeInfo) {}),
                               GBL_TYPE_FLAG_ABSTRACT);

    gblTypeRegisterBuiltin(19,
                               GBL_TYPE_INVALID,
                               "boxed",
                               &((const GblTypeInfo) {}),
                               GBL_TYPE_FLAG_ABSTRACT);

    GBL_API_POP(1);
    GBL_API_END();
}
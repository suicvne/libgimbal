#ifndef GIMBAL_VARIANT_H
#define GIMBAL_VARIANT_H

#include "../types/gimbal_typedefs.h"
#include "../core/gimbal_api_frame.h"
#include "../strings/gimbal_string_ref.h"
#include "../meta/gimbal_primitives.h"
#include "../meta/gimbal_enum.h"
#include "../meta/gimbal_flags.h"
#include "../meta/gimbal_pointer.h"
#include "../meta/gimbal_boxed.h"

#define GBL_VARIANT_INIT    { .type = GBL_INVALID_TYPE }
#define GBL_VARIANT(name)   GblVariant name = GBL_VARIANT_INIT

#define SELF    GblVariant* pSelf
#define CSELF   const SELF

GBL_DECLS_BEGIN

typedef GBL_RESULT (*GblVariantConverterFn)(CSELF, GblVariant* pOther);

typedef struct GblVariant {
    GblType             type;
    union {
        char            character;
        GblBool         boolean;
        uint8_t         u8;
        uint16_t        u16;
        int16_t         i16;
        uint32_t        u32;
        int32_t         i32;
        uint64_t        u64;
        int64_t         i64;
        GblEnum         enumeration;
        GblFlags        flags;
        float           f32;
        double          f64;
        void*           pVoid;
        GblStringRef*   pString;
        GblType         typeValue;
        GblInstance*    pInstance;
        GblObject*      pObject;
    };
} GblVariant;


GBL_INLINE GblType          GblVariant_type                     (CSELF)                         GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_constructDefault         (SELF,  GblType type)           GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_constructCopy            (SELF,
                                                                 const GblVariant* pOther)      GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_constructMove            (SELF,
                                                                 GblVariant* pOther)            GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_constructValueCopy       (SELF,  GblType type, ...)      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_constructValueCopyVaList (SELF,
                                                                 GblType type,
                                                                 va_list* pList)                GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_constructValueMove       (SELF,  GblType type, ...)      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_constructValueMoveVaList (SELF,
                                                                 GblType type,
                                                                 va_list* pList)                GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructNil             (SELF)                          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructBool            (SELF, GblBool value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructChar            (SELF, char value)              GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructUint8           (SELF, uint8_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructUint16          (SELF, uint16_t value)          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructInt16           (SELF, int16_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructUint32          (SELF, uint32_t value)          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructInt32           (SELF, int32_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructUint64          (SELF, uint64_t value)          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructInt64           (SELF, int64_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructFloat           (SELF, float value)             GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructDouble          (SELF, double value)            GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructString          (SELF, const char* pValue)      GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructStringView      (SELF, GblStringView value)     GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructTypeValue       (SELF, GblType type)            GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructEnum            (SELF,
                                                                 GblType type,
                                                                 GblEnum value)                 GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructFlags           (SELF,
                                                                 GblType type,
                                                                 GblFlags value)                GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructPointer         (SELF, void* pValue)            GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructOpaque          (SELF,
                                                                 GblType ptrType,
                                                                 void* pValue)                  GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructBoxedCopy       (SELF,
                                                                 GblType boxedType,
                                                                 void*  pValue)                 GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructBoxedMove       (SELF,
                                                                 GblType boxedType,
                                                                 void*  pValue)                 GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_constructInstance        (SELF, GblInstance* pValue)     GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructObjectCopy      (SELF, GblObject* pValue)       GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_constructObjectMove      (SELF, GblObject* pValue)       GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_destruct                 (SELF)                          GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_setCopy                  (SELF,
                                                                 const GblVariant* pOther)      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_setMove                  (SELF,  GblVariant* pOther)     GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_setValueCopy             (SELF,  GblType type, ...)      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_setValueCopyVaList       (SELF,
                                                                 GblType  type,
                                                                 va_list* pVarArgs)             GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_setValueMove             (SELF,  GblType type, ...)      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_setValueMoveVaList       (SELF,
                                                                 GblType  type,
                                                                 va_list* pVarArgs)             GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_invalidate               (SELF)                          GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setNil                   (SELF)                          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setBool                  (SELF, GblBool value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setChar                  (SELF, char value)              GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setUint8                 (SELF, uint8_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setUint16                (SELF, uint16_t value)          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setInt16                 (SELF, int16_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setUint32                (SELF, uint32_t value)          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setInt32                 (SELF, int32_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setUint64                (SELF, uint64_t value)          GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setInt64                 (SELF, int64_t value)           GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setFloat                 (SELF, float value)             GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setDouble                (SELF, double value)            GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setString                (SELF, const char* pValue)      GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setStringView            (SELF, GblStringView value)     GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setTypeValue             (SELF, GblType value)           GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setEnum                  (SELF,
                                                                 GblType enumType,
                                                                 GblEnum value)                 GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setFlags                 (SELF,
                                                                 GblType flagsType,
                                                                 GblFlags value)                GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setPointer               (SELF, void* pValue)            GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setOpaque                (SELF,
                                                                 GblType ptrType,
                                                                 void* pValue)                  GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setBoxedCopy             (SELF,
                                                                 GblType boxedType,
                                                                 void*  pValue)                 GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setBoxedMove             (SELF,
                                                                 GblType boxedType,
                                                                 void*  pValue)                 GBL_NOEXCEPT;

GBL_INLINE GBL_RESULT       GblVariant_setInstance              (SELF, GblInstance* pValue)     GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setObjectCopy            (SELF, GblObject* pValue)       GBL_NOEXCEPT;
GBL_INLINE GBL_RESULT       GblVariant_setObjectMove            (SELF, GblObject* pValue)       GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_getValueCopy             (CSELF,  ...)                   GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_getValueCopyVaList       (CSELF, va_list* pVarArgs)      GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_getValuePeek             (CSELF,  ...)                   GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_getValuePeekVaList       (SELF, va_list* pVarArgs)       GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_getValueMove             (SELF,  ...)                    GBL_NOEXCEPT;
GBL_EXPORT GBL_RESULT       GblVariant_getValueMoveVaList       (SELF, va_list* pVarArgs)       GBL_NOEXCEPT;

GBL_INLINE GblBool          GblVariant_isValid                  (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE GblBool          GblVariant_isNil                    (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE GblBool          GblVariant_getBool                  (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE char             GblVariant_getChar                  (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE uint8_t          GblVariant_getUint8                 (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE uint16_t         GblVariant_getUint16                (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE int16_t          GblVariant_getInt16                 (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE uint32_t         GblVariant_getUint32                (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE int32_t          GblVariant_getInt32                 (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE uint64_t         GblVariant_getUint64                (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE int64_t          GblVariant_getInt64                 (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE GblEnum          GblVariant_getEnum                  (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE GblFlags         GblVariant_getFlags                 (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE float            GblVariant_getFloat                 (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE double           GblVariant_getDouble                (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE const char*      GblVariant_getString                (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE GblStringView    GblVariant_getStringView            (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE GblType          GblVariant_getTypeValue             (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE void*            GblVariant_getPointer               (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE void*            GblVariant_getBoxedCopy             (CSELF)                         GBL_NOEXCEPT;
GBL_INLINE void*            GblVariant_getBoxedMove             (SELF)                          GBL_NOEXCEPT;
GBL_INLINE void*            GblVariant_getBoxedPeek             (CSELF)                         GBL_NOEXCEPT;

GBL_INLINE GblInstance*     GblVariant_getInstance              (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT GblObject*       GblVariant_getObjectCopy            (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT GblObject*       GblVariant_getObjectMove            (SELF)                          GBL_NOEXCEPT;
GBL_EXPORT GblObject*       GblVariant_getObjectPeek            (CSELF)                         GBL_NOEXCEPT;

GBL_EXPORT GblBool          GblVariant_toBool                   (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT char             GblVariant_toChar                   (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT uint8_t          GblVariant_toUint8                  (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT uint16_t         GblVariant_toUint16                 (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT int16_t          GblVariant_toInt16                  (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT uint32_t         GblVariant_toUint32                 (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT int32_t          GblVariant_toInt32                  (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT uint64_t         GblVariant_toUint64                 (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT int64_t          GblVariant_toInt64                  (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT GblEnum          GblVariant_toEnum                   (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT GblFlags         GblVariant_toFlags                  (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT float            GblVariant_toFloat                  (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT double           GblVariant_toDouble                 (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT void*            GblVariant_toPointer                (CSELF)                         GBL_NOEXCEPT;
GBL_EXPORT const char*      GblVariant_toStringAppend           (CSELF, GblStringBuffer* pBuff) GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_registerConverter        (GblType fromType,
                                                                 GblType toType,
                                                                 GblVariantConverterFn pFnConv) GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_unregisterConverter      (GblType fromType,
                                                                 GblType toType)                GBL_NOEXCEPT;

GBL_EXPORT GblSize          GblVariant_converterCount           (void)                          GBL_NOEXCEPT;

GBL_EXPORT GblBool          GblVariant_canConvert               (GblType fromType,
                                                                 GblType toType)                GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_convert                  (CSELF,
                                                                 GblVariant* pToVariant)        GBL_NOEXCEPT;

GBL_EXPORT GblInt           GblVariant_compare                  (CSELF,
                                                                 const GblVariant* pOther)      GBL_NOEXCEPT;

GBL_INLINE GblBool          GblVariant_equals                   (CSELF,
                                                                 const GblVariant* pOther)      GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_save                     (CSELF,
                                                                 GblStringBuffer* pString)      GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT       GblVariant_load                     (SELF,
                                                                 const GblStringBuffer* pStr)   GBL_NOEXCEPT;

//========== MACRO OVERLOADS ==========

#define GBL_VARIANT_CONSTRUCT_TABLE  (                                  \
        GBL_META_GENERIC_MACRO_NO_DEFAULT,                              \
        (                                                               \
            (GblBool,                GblVariant_constructBool),         \
            (char,                   GblVariant_constructChar),         \
            (uint8_t,                GblVariant_constructUint8),        \
            (uint16_t,               GblVariant_constructUint16),       \
            (int16_t,                GblVariant_constructInt16),        \
            (uint32_t,               GblVariant_constructUint32),       \
            (int32_t,                GblVariant_constructInt32),        \
            (uint64_t,               GblVariant_constructUint64),       \
            (int64_t,                GblVariant_constructInt64),        \
            (float,                  GblVariant_constructFloat),        \
            (double,                 GblVariant_constructDouble),       \
            (const char*             GblVariant_constructString),       \
            (GblStringView           GblVariant_constructStringView),   \
            (void*                   GblVariant_constructPointer),      \
            (const GblVariant*       GblVariant_constructCopy)          \
                                                                        \
        )                                                               \
    )
#define GblVariant_construct_N(pVariant, ...)   GBL_META_GENERIC_MACRO_GENERATE(GBL_VARIANT_CONSTRUCT_TABLE, GBL_ARG_1(__VA_ARGS__))(pVariant, __VA_ARGS__)
#define GblVariant_construct_1(pVariant)        GblVariant_constructNil(pVariant)
#define GblVariant_construct(...)               GBL_VA_OVERLOAD_CALL(GblVariant_construct, GBL_VA_OVERLOAD_SUFFIXER_1_N, __VA_ARGS__)

#define GBL_VARIANT_SET_TABLE  (                                    \
        GBL_META_GENERIC_MACRO_NO_DEFAULT,                          \
        (                                                           \
            (GblBool,                GblVariant_setBool),           \
            (char,                   GblVariant_setChar),           \
            (uint8_t,                GblVariant_setUint8),          \
            (uint16_t,               GblVariant_setUint16),         \
            (int16_t,                GblVariant_setInt16),          \
            (uint32_t,               GblVariant_setUint32),         \
            (int32_t,                GblVariant_settInt32),         \
            (uint64_t,               GblVariant_settUint64),        \
            (int64_t,                GblVariant_setInt64),          \
            (float,                  GblVariant_setFloat),          \
            (double,                 GblVariant_setDouble),         \
            (const char*             GblVariant_setString),         \
            (GblStringView           GblVariant_settStringView),    \
            (void*                   GblVariant_setPointer),        \
            (const GblVariant*       GblVariant_settCopy)           \
                                                                    \
        )                                                           \
    )
#define GblVariant_set(pVariant, ...) GBL_META_GENERIC_MACRO_GENERATE(GBL_VARIANT_SET_TABLE, GBL_ARG_1(__VA_ARGS__))(pVariant, __VA_ARGS__)



// ========== IMPL ==========
GBL_INLINE GblType GblVariant_type(const GblVariant* pSelf) GBL_NOEXCEPT {
    return pSelf->type;
}

GBL_INLINE GBL_RESULT GblVariant_constructNil(SELF) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructDefault(pSelf, GBL_NIL_TYPE));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructBool(SELF, GblBool value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_BOOL_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructChar(SELF, char value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_CHAR_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructUint8(SELF, uint8_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_UINT8_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructUint16(SELF, uint16_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_UINT16_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructInt16(SELF, int16_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_INT16_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructUint32(SELF, uint32_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_UINT32_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructInt32(SELF, int32_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_INT32_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructUint64(SELF, uint64_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_UINT64_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructInt64(SELF, int64_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_INT64_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructEnum(SELF, GblType type, GblEnum value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY(GBL_ENUM_TYPE_CHECK(type),
                   GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, type, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructFlags(SELF, GblType type, GblFlags value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY(GBL_FLAGS_TYPE_CHECK(type),
                   GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, type, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructFloat(SELF, float value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_FLOAT_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructDouble(SELF, double value) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_DOUBLE_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructPointer(SELF, void* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_POINTER_TYPE, pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructOpaque(SELF, GblType ptrType, void* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY(GBL_POINTER_TYPE_CHECK(ptrType),
                   GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, ptrType, pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructString(SELF, const char* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_STRING_TYPE, pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructBoxedCopy(SELF, GblType boxedType, void*  pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(boxedType, GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, boxedType, pValue));
    GBL_API_END();
}

GBL_INLINE GBL_RESULT GblVariant_constructBoxedMove(SELF, GblType boxedType, void* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(boxedType, GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_constructValueMove(pSelf, boxedType, pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructStringView(SELF, GblStringView view) GBL_NOEXCEPT {
    return GblVariant_constructString(pSelf, GBL_STRING_VIEW_CSTR(view));
}
GBL_INLINE GBL_RESULT GblVariant_constructTypeValue(SELF, GblType type) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_TYPE_TYPE, type));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructInstance(SELF, GblInstance* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_INSTANCE_TYPE(pValue), pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructObjectCopy(SELF, GblObject* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(pSelf, GBL_INSTANCE_TYPE(pValue), pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_constructObjectMove(SELF, GblObject* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_CALL(GblVariant_constructValueMove(pSelf, GBL_INSTANCE_TYPE(pValue), pValue));
    GBL_API_END();
}
GBL_INLINE GblBool GblVariant_isValid(CSELF) GBL_NOEXCEPT {
    return pSelf && GblVariant_type(pSelf) != GBL_INVALID_TYPE;
}
GBL_INLINE GblBool GblVariant_isNil(CSELF) GBL_NOEXCEPT {
    return pSelf && GblVariant_type(pSelf) == GBL_NIL_TYPE;
}
GBL_INLINE GblBool GblVariant_getBool(CSELF) GBL_NOEXCEPT {
    GblBool value = GBL_FALSE;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_BOOL_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE char GblVariant_getChar(CSELF) GBL_NOEXCEPT {
    char value = '\0';;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_CHAR_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE uint8_t GblVariant_getUint8(CSELF) GBL_NOEXCEPT {
    uint8_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_UINT8_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE uint16_t GblVariant_getUint16(CSELF) GBL_NOEXCEPT {
    uint16_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_UINT16_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE int16_t GblVariant_getInt16(CSELF) GBL_NOEXCEPT {
    int16_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_INT16_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE uint32_t GblVariant_getUint32(CSELF) GBL_NOEXCEPT {
    uint32_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_UINT32_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE int32_t GblVariant_getInt32(CSELF) GBL_NOEXCEPT {
    int32_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_INT32_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE uint64_t GblVariant_getUint64(CSELF) GBL_NOEXCEPT {
    uint64_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_UINT64_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE int64_t GblVariant_getInt64(CSELF) GBL_NOEXCEPT {
    int64_t value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_INT64_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE GblEnum GblVariant_getEnum(CSELF) GBL_NOEXCEPT {
    GblEnum value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_ENUM_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE GblEnum GblVariant_getFlags(CSELF) GBL_NOEXCEPT {
    GblFlags value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_FLAGS_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE float GblVariant_getFloat(CSELF) GBL_NOEXCEPT {
    float value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_FLOAT_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE double GblVariant_getDouble(CSELF) GBL_NOEXCEPT {
    double value = 0;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_DOUBLE_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE void* GblVariant_getPointer(CSELF) GBL_NOEXCEPT {
    void* pValue = GBL_NULL;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_POINTER_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValuePeek(pSelf, &pValue));
    GBL_API_END_BLOCK();
    return pValue;
}
GBL_INLINE void* GblVariant_getBoxedCopy(CSELF) GBL_NOEXCEPT {
    void* pValue = GBL_NULL;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueCopy(pSelf, &pValue));
    GBL_API_END_BLOCK();
    return pValue;
}
GBL_INLINE void* GblVariant_getBoxedMove(SELF) GBL_NOEXCEPT {
    void* pValue = GBL_NULL;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValueMove(pSelf, &pValue));
    GBL_API_END_BLOCK();
    return pValue;
}
GBL_INLINE void* GblVariant_getBoxedPeek(CSELF) GBL_NOEXCEPT {
    void* pValue = GBL_NULL;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValuePeek(pSelf, &pValue));
    GBL_API_END_BLOCK();
    return pValue;
}
GBL_INLINE const char* GblVariant_getString(CSELF) GBL_NOEXCEPT {
    const char* pValue = GBL_NULL;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_STRING_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValuePeek(pSelf, &pValue));
    GBL_API_END_BLOCK();
    return pValue;
}
GBL_INLINE GblStringView GblVariant_getStringView(CSELF) GBL_NOEXCEPT {
    return GblStringRef_view(GblVariant_getString(pSelf));
}
GBL_INLINE GblType GblVariant_getTypeValue(CSELF) GBL_NOEXCEPT {
    GblType value = GBL_INVALID_TYPE;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY_TYPE(GblVariant_type(pSelf), GBL_TYPE_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_getValuePeek(pSelf, &value));
    GBL_API_END_BLOCK();
    return value;
}
GBL_INLINE GblInstance* GblVariant_getInstance(CSELF) GBL_NOEXCEPT {
    GblInstance* pValue = GBL_NULL;
    GBL_API_BEGIN(GBL_NULL);
    GBL_API_VERIFY(GBL_TYPE_IS_INSTANTIABLE(GblVariant_type(pSelf)),
                   GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_API_VERIFY_CALL(GblVariant_getValuePeek(pSelf, &pValue));
    GBL_API_END_BLOCK();
    return pValue;
}
GBL_INLINE GBL_RESULT GblVariant_invalidate(SELF) GBL_NOEXCEPT {
    if(GblVariant_type(pSelf) != GBL_INVALID_TYPE)
        GblClass_unref(GblClass_peek(GblVariant_type(pSelf)));
    memset(pSelf, 0, sizeof(GblVariant));
    pSelf->type = GBL_INVALID_TYPE;
    return GBL_RESULT_SUCCESS;
}

GBL_INLINE GBL_RESULT GblVariant_setNil(SELF) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_destruct(pSelf));
    GBL_API_VERIFY_CALL(GblVariant_constructDefault(pSelf, GBL_NIL_TYPE));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setBool(SELF, GblBool value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_BOOL_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setChar(SELF, char value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_CHAR_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setUint8(SELF, uint8_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_UINT8_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setUint16(SELF, uint16_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_UINT16_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setInt16(SELF, int16_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_INT16_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setUint32(SELF, uint32_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_UINT32_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setInt32(SELF, int32_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_INT32_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setUint64(SELF, uint64_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_UINT64_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setInt64(SELF, int64_t value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_INT64_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setEnum(SELF,
                                         GblType enumType,
                                         GblEnum value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_TYPE(enumType, GBL_ENUM_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, enumType, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setFlags(SELF,
                                          GblType flagsType,
                                          GblFlags value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_TYPE(flagsType, GBL_FLAGS_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, flagsType, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setFloat(SELF, float value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_FLOAT_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setDouble(SELF, double value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_DOUBLE_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setPointer(SELF, void* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_POINTER_TYPE, pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setOpaque(SELF,
                                           GblType ptrType,
                                           void* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_TYPE(ptrType, GBL_POINTER_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, ptrType, pValue));
    GBL_API_END();
}

GBL_INLINE GBL_RESULT GblVariant_setBoxedCopy(SELF, GblType boxedType, void*  pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_TYPE(boxedType, GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, boxedType, pValue));
    GBL_API_END();
}

GBL_INLINE GBL_RESULT GblVariant_setBoxedMove(SELF, GblType boxedType, void*  pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_TYPE(boxedType, GBL_BOXED_TYPE);
    GBL_API_VERIFY_CALL(GblVariant_setValueMove(pSelf, boxedType, pValue));
    GBL_API_END();
}

GBL_INLINE GBL_RESULT GblVariant_setString(SELF, const char* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_STRING_TYPE, pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setStringView(SELF, GblStringView value) GBL_NOEXCEPT {
    return GblVariant_setString(pSelf, GBL_STRING_VIEW_CSTR(value));
}
GBL_INLINE GBL_RESULT GblVariant_setTypeValue(SELF, GblType value) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_TYPE_TYPE, value));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setInstance(SELF, GblInstance* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_INSTANCE_TYPE(pValue), pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setObjectCopy(SELF, GblObject* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueCopy(pSelf, GBL_INSTANCE_TYPE(pValue), pValue));
    GBL_API_END();
}
GBL_INLINE GBL_RESULT GblVariant_setObjectMove(SELF, GblObject* pValue) GBL_NOEXCEPT {
    GBL_API_BEGIN(NULL);
    GBL_API_VERIFY_CALL(GblVariant_setValueMove(pSelf, GBL_INSTANCE_TYPE(pValue), pValue));
    GBL_API_END();
}
GBL_INLINE GblBool GblVariant_equals(CSELF, const GblVariant* pOther) GBL_NOEXCEPT {
    return GblVariant_compare(pSelf, pOther) == 0;
}

GBL_DECLS_END

#undef CSELF
#undef SELF

#endif // GIMBAL_VARIANT_H

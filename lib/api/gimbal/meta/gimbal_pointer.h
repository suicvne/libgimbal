#ifndef GIMBAL_POINTER_H
#define GIMBAL_POINTER_H

#include "gimbal_primitives.h"

#define GBL_POINTER_TYPE                (GBL_BUILTIN_TYPE(POINTER))
#define GBL_POINTER_TYPE_CHECK(type)    (GblType_check(type, GBL_POINTER_TYPE))
#define GBL_OPAQUE_TYPE_CHECK(type)     (GblType_derives(type, GBL_POINTER_TYPE))

GBL_DECLS_BEGIN

GblType GblPointer_registerOpaque(const char* pName) GBL_NOEXCEPT;

GBL_DECLS_END


#endif // GIMBAL_POINTER_H
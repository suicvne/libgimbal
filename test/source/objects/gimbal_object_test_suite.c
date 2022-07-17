#include "objects/gimbal_object_test_suite.h"
#include <gimbal/objects/gimbal_object.h>
#include <gimbal/objects/gimbal_event.h>
#include <gimbal/types/gimbal_variant.h>
#include <gimbal/test/gimbal_test.h>
#include <math.h>

GBL_FORWARD_DECLARE_STRUCT(TestObject);

#define GBL_OBJECT_TEST_SUITE_(inst)        ((GblObjectTestSuite_*)GBL_INSTANCE_PRIVATE(inst, GBL_OBJECT_TEST_SUITE_TYPE))

typedef struct GblObjectTestSuite_ {
    TestObject*     pTestObj;
} GblObjectTestSuite_;

#define TEST_OBJECT_TYPE                    (TestObject_type())
#define TEST_OBJECT_STRUCT                  TestObject
#define TEST_OBJECT_CLASS_STRUCT            TestObjectClass
#define TEST_OBJECT(instance)               GBL_INSTANCE_CAST_PREFIX(instance, TEST_OBJECT)
#define TEST_OBJECT_CHECK(instance)         GBL_INSTANCE_CHECK_PREFIX(instance, TEST_OBJECT)
#define TEST_OBJECT_CLASS(klass)            GBL_CLASS_CAST_PREFIX(klass, TEST_OBJECT)
#define TEST_OBJECT_CLASS_CHECK(klass)      GBL_CLASS_CHECK_PREFIX(klass, TEST_OBJECT)
#define TEST_OBJECT_GET_CLASS(instance)     GBL_INSTANCE_GET_CLASS_PREFIX(instance, TEST_OBJECT)

static GblType TestObject_type(void);

typedef struct TestObjectClass {
    GblObjectClass  base;
    int32_t staticInt32;
    char string[256];
} TestObjectClass;

typedef struct TestObject {
    GblObject           base;
    float               floater;
    char                stringer[256];
    int                 eventHandlerCount;
    GblType             eventHandlerLastType;
    GblBool             eventHandlerAccept;
    int                 eventFilterCount;
    GblType             eventFilterLastType;
    GblBool             eventFilterAccept;
    GblIEventHandler*   eventFilterLastTarget;
} TestObject;


static GBL_RESULT TestObject_IEventHandler_handleEvent(GblIEventHandler* pHandler, GblEvent* pEvent) {
    GBL_API_BEGIN(pHandler);
    TestObject* pTest = TEST_OBJECT(pHandler);
    pTest->eventHandlerCount++;
    pTest->eventHandlerLastType = GblEvent_typeOf(pEvent);
    if(pTest->eventHandlerAccept) GblEvent_accept(pEvent);
    GBL_API_END();
}

static GBL_RESULT TestObject_IEventFilter_filterEvent(GblIEventFilter* pFilter, GblIEventHandler* pTarget, GblEvent* pEvent) {
    GBL_API_BEGIN(pFilter);
    TestObject* pTest = TEST_OBJECT(pFilter);
    pTest->eventFilterCount++;
    pTest->eventFilterLastType = GblEvent_typeOf(pEvent);
    pTest->eventFilterLastTarget = pTarget;
    if(pTest->eventFilterAccept) GblEvent_accept(pEvent);
    GBL_API_END();
}

static GBL_RESULT TestObject_constructor(GblObject* pSelf) {
    GBL_API_BEGIN(pSelf);
    GblObjectClass* pParentClass = GBL_OBJECT_CLASS(GBL_CLASS_SUPER(GBL_INSTANCE_CLASS(pSelf)));
    pParentClass->pFnConstructor(pSelf);
    TestObject* pTest = TEST_OBJECT(pSelf);
    pTest->floater = -NAN;
    strcpy(pTest->stringer, "INVALID");
    GBL_API_END();
}

static GBL_RESULT TestObject_constructed(GblObject* pSelf) {
    GBL_API_BEGIN(pSelf);
    GblObjectClass* pParentClass = GBL_OBJECT_CLASS(GBL_CLASS_SUPER(GBL_INSTANCE_CLASS(pSelf)));
    if(pParentClass->pFnConstructed) pParentClass->pFnConstructed(pSelf);
    GBL_API_END();
}

static GBL_RESULT TestObject_destructor(GblObject* pSelf) {
    GBL_API_BEGIN(pSelf);
    GblObjectClass* pParentClass = GBL_OBJECT_CLASS(GBL_CLASS_SUPER(GBL_INSTANCE_CLASS(pSelf)));
    pParentClass->pFnDestructor(pSelf);
    GBL_API_END();
}

enum TEST_OBJECT_PROPERTIES {
    TEST_OBJECT_PROPERTY_FLOATER,
    TEST_OBJECT_PROPERTY_STRINGER,
    TEST_OBJECT_PROPERTY_STATICINT32,
    TEST_OBJECT_PROPERTY_USERDATA,
    TEST_OBJECT_PROPERTY_COUNT
};

static GBL_RESULT TestObject_propertyGet(const GblObject* pSelf, GblSize slot, GblVariant* pValue, const GblProperty* pProp) {
    GBL_API_BEGIN(pSelf);
    switch(slot) {
    case TEST_OBJECT_PROPERTY_FLOATER:
        GblVariant_setValueCopy(pValue, GblProperty_valueType(pProp), TEST_OBJECT(pSelf)->floater);
        break;
    case TEST_OBJECT_PROPERTY_STRINGER:
        GblVariant_setValueCopy(pValue, GblProperty_valueType(pProp), TEST_OBJECT(pSelf)->stringer);
        break;
    case TEST_OBJECT_PROPERTY_STATICINT32:
        GblVariant_setValueCopy(pValue, GblProperty_valueType(pProp), TEST_OBJECT_GET_CLASS(pSelf)->staticInt32);
        break;
    case TEST_OBJECT_PROPERTY_USERDATA:
        GblVariant_setValueCopy(pValue, GblProperty_valueType(pProp), GblObject_userdata(GBL_OBJECT(pSelf)));
        break;
    default: GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_PROPERTY,
                                "Attempt to get unregistered property: %s",
                                GblProperty_nameString(pProp));
    }
    GBL_API_END();
}

static GBL_RESULT TestObject_propertySet(GblObject* pSelf, GblSize slot, const GblVariant* pValue, const GblProperty* pProp) {
    GBL_API_BEGIN(pSelf);
    switch(slot) {
    case TEST_OBJECT_PROPERTY_FLOATER: {
        float value = NAN;
        GBL_API_CALL(GblVariant_getValueCopy(pValue, &value));
        TEST_OBJECT(pSelf)->floater = value;
        break;
    }
    case TEST_OBJECT_PROPERTY_STRINGER: {
        const char* pStr = NULL;
        GBL_API_CALL(GblVariant_getValuePeek(pValue, &pStr));
        if(pStr) strcpy(TEST_OBJECT(pSelf)->stringer, pStr);
        break;
    }
    case TEST_OBJECT_PROPERTY_USERDATA: {
        void* pUserdata = NULL;
        GBL_API_CALL(GblVariant_getValueCopy(pValue, &pUserdata));
        GblObject_userdataSet(pSelf, pUserdata);
        break;
    }
    default: GBL_API_RECORD_SET(GBL_RESULT_ERROR_INVALID_PROPERTY,
                                "Attempt to set unregistered property: %s",
                                GblProperty_nameString(pProp));
    }
    GBL_API_END();
}

static GBL_RESULT TestObjectClass_init_(GblClass* pClass, const void* pUd, GblContext* pCtx) {
    TestObjectClass* pTestClass = TEST_OBJECT_CLASS(pClass);
    GBL_API_BEGIN(pCtx);
    if(GBL_CLASS_TYPEOF(pTestClass) == TEST_OBJECT_TYPE && !GblType_classRefCount(TEST_OBJECT_TYPE)) {
        gblPropertyTableInsert(GBL_CLASS_TYPEOF(pTestClass),
                               GblQuark_fromStringStatic("floater"),
                               TEST_OBJECT_PROPERTY_FLOATER,
                               GBL_FLOAT_TYPE,
                               GBL_PROPERTY_FLAG_READ | GBL_PROPERTY_FLAG_WRITE | GBL_PROPERTY_FLAG_CONSTRUCT);
        gblPropertyTableInsert(GBL_CLASS_TYPEOF(pTestClass),
                               GblQuark_fromStringStatic("stringer"),
                               TEST_OBJECT_PROPERTY_STRINGER,
                               GBL_STRING_TYPE,
                               GBL_PROPERTY_FLAG_READ | GBL_PROPERTY_FLAG_WRITE | GBL_PROPERTY_FLAG_CONSTRUCT);
        gblPropertyTableInsert(GBL_CLASS_TYPEOF(pTestClass),
                               GblQuark_fromStringStatic("staticInt32"),
                               TEST_OBJECT_PROPERTY_STATICINT32,
                               GBL_INT32_TYPE,
                               GBL_PROPERTY_FLAG_READ);
        gblPropertyTableInsert(GBL_CLASS_TYPEOF(pTestClass),
                               GblQuark_fromStringStatic("userdata"),
                               TEST_OBJECT_PROPERTY_USERDATA,
                               GBL_POINTER_TYPE,
                               GBL_PROPERTY_FLAG_CONSTRUCT | GBL_PROPERTY_FLAG_READ | GBL_PROPERTY_FLAG_WRITE | GBL_PROPERTY_FLAG_OVERRIDE);
    }
    pTestClass->staticInt32 = 77;
    strcpy(pTestClass->string, (const char*)pUd);
    pTestClass->base.iEventHandlerIFace.pFnEvent = TestObject_IEventHandler_handleEvent;
    pTestClass->base.iEventFilterIFace.pFnEventFilter = TestObject_IEventFilter_filterEvent;
    pTestClass->base.pFnConstructor = TestObject_constructor;
    pTestClass->base.pFnDestructor  = TestObject_destructor;
    pTestClass->base.pFnConstructed = TestObject_constructed;
    pTestClass->base.pFnPropertyGet = TestObject_propertyGet;
    pTestClass->base.pFnPropertySet = TestObject_propertySet;
    GBL_API_END();
}

static GblType TestObject_type(void) {
    static GblType type = GBL_INVALID_TYPE;
    if(type == GBL_INVALID_TYPE) {
        const GblTypeInfo info = {
            .pFnClassInit   = TestObjectClass_init_,
            .classSize      = sizeof(TestObjectClass),
            .pClassData     = (void*)"Davey Havoc",
            .instanceSize   = sizeof(TestObject)
        };

        type = GblType_registerStatic("TestObject",
                                      GBL_OBJECT_TYPE,
                                      &info,
                                      GBL_TYPE_FLAGS_NONE);
    }
    return type;
}

static GBL_RESULT GblObjectTestSuite_init_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GblObjectTestSuite_* pSelf_ = GBL_OBJECT_TEST_SUITE_(pSelf);
    memset(pSelf_, 0, sizeof(GblObjectTestSuite_));
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_final_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblType_classRefCount(TEST_OBJECT_TYPE), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newDefault_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GblObjectTestSuite_* pSelf_ = GBL_OBJECT_TEST_SUITE_(pSelf);

    TestObject* pObj = TEST_OBJECT(GblObject_new(TEST_OBJECT_TYPE, NULL));
    pSelf_->pTestObj = pObj;

    GBL_TEST_COMPARE(GblType_classRefCount(TEST_OBJECT_TYPE), 1);

    // validate class
    TestObjectClass* pClass = TEST_OBJECT_GET_CLASS(pObj);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pClass), TEST_OBJECT_TYPE);
    GBL_TEST_COMPARE(&pClass->base, GBL_OBJECT_CLASS(pClass));
    GBL_TEST_COMPARE(pClass->string, "Davey Havoc");
    GBL_TEST_COMPARE(pClass->staticInt32, 77);

    // validate interfaces
    GblIVariantIFace* pIVariantIFace = GBL_IVARIANT_GET_IFACE(pObj);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pIVariantIFace), GBL_IVARIANT_TYPE);
    GBL_TEST_COMPARE(pClass, TEST_OBJECT_CLASS(pIVariantIFace));
    GBL_TEST_COMPARE(pIVariantIFace->pVTable->pGetValueFmt, "p");

    GblITableIFace* pITableIFace = GBL_ITABLE_GET_IFACE(pObj);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pITableIFace), GBL_ITABLE_TYPE);
    GBL_TEST_COMPARE(pClass, TEST_OBJECT_CLASS(pITableIFace));

    GblIEventHandlerIFace* pIEventHandlerIFace = GBL_IEVENT_HANDLER_TRY_IFACE(pObj);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pIEventHandlerIFace), GBL_IEVENT_HANDLER_TYPE);
    GBL_TEST_COMPARE(pClass, TEST_OBJECT_CLASS(pIEventHandlerIFace));

    GblIEventFilterIFace* pIEventFilterIFace = GBL_IEVENT_FILTER_GET_IFACE(pObj);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pIEventFilterIFace), GBL_IEVENT_FILTER_TYPE);
    GBL_TEST_COMPARE(pClass, TEST_OBJECT_CLASS(pIEventFilterIFace));

    // validate sanity of random casts
    GBL_TEST_COMPARE(pITableIFace, GBL_ITABLE_IFACE(pIEventHandlerIFace));
    GBL_TEST_COMPARE(pIVariantIFace, GBL_IVARIANT_IFACE(pIEventFilterIFace));

    //validate some of the compatible checks
    GBL_TEST_VERIFY(GBL_IVARIANT_IFACE_CHECK(pClass));
    GBL_TEST_VERIFY(!TEST_OBJECT_CLASS_CHECK(pIVariantIFace));
    GBL_TEST_VERIFY(GBL_OBJECT_CLASS_CHECK(pClass));
    GBL_TEST_VERIFY(GBL_ITABLE_IFACE_CHECK(GBL_OBJECT_CLASS(pITableIFace)));

    //validate insanity
    GBL_TEST_COMPARE(pClass, TEST_OBJECT_CLASS(GBL_ITABLE_IFACE(GBL_OBJECT_CLASS(GBL_OBJECT_CLASS_TRY(GBL_IEVENT_FILTER_IFACE(TEST_OBJECT_GET_CLASS(pObj)))))));

    // validate instance checks and casts
    GBL_TEST_VERIFY(GBL_IVARIANT_CHECK(pObj));
    GBL_TEST_VERIFY(GBL_IEVENT_FILTER_CHECK(GBL_OBJECT_TRY(GBL_IEVENT_FILTER(pObj))));
    GBL_TEST_COMPARE(pObj, TEST_OBJECT(GBL_IVARIANT(GBL_OBJECT(GBL_IEVENT_FILTER(TEST_OBJECT(pObj))))));

    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_name_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GblObjectTestSuite_* pSelf_ = GBL_OBJECT_TEST_SUITE_(pSelf);

    GblObject_nameSet(GBL_OBJECT(pSelf_->pTestObj), "TestName");
    GBL_TEST_COMPARE(GblObject_name(GBL_OBJECT(pSelf_->pTestObj)), "TestName");
    GblObject_nameSet(GBL_OBJECT(pSelf_->pTestObj), "TestName2");
    GBL_TEST_COMPARE(GblObject_name(GBL_OBJECT(pSelf_->pTestObj)), "TestName2");

    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_ref_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GblObjectTestSuite_* pSelf_ = GBL_OBJECT_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblObject_refCount(GBL_OBJECT(pSelf_->pTestObj)), 1);
    GblObject* pObj = GBL_OBJECT(GblObject_ref(GBL_OBJECT(pSelf_->pTestObj)));
    GBL_TEST_COMPARE(GBL_OBJECT(pSelf_->pTestObj), pObj);
    GBL_TEST_COMPARE(GblObject_refCount(pObj), 2);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_unref_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GblObjectTestSuite_* pSelf_ = GBL_OBJECT_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pSelf_->pTestObj)), 1);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pSelf_->pTestObj)), 0);
    GBL_TEST_COMPARE(GblType_classRefCount(TEST_OBJECT_TYPE), 0);

    // K now replace what we deleted... lulz.
    pSelf_->pTestObj = NULL;
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_extendedData_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE, NULL);

    GBL_TEST_COMPARE(GblObject_name(pObj),           NULL);
    GBL_TEST_COMPARE(GblObject_userdata(pObj),       NULL);
    GBL_TEST_COMPARE(GblObject_parent(pObj),         NULL);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GblObject_nameSet(pObj, "Sing the Sorrow");
    GBL_TEST_COMPARE(GblObject_name(pObj), "Sing the Sorrow");

    GblObject_userdataSet(pObj, (void*)0xbadbeef);
    GBL_TEST_COMPARE(GblObject_userdata(pObj), (void*)0xbadbeef);

    // make sure we didn't screw up other extended data
    GBL_TEST_COMPARE(GblObject_parent(pObj),            NULL);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newVaArgs_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Fuckwad",
                                    "userdata", (void*)0xdeadbeef,
                                    NULL);

    GBL_TEST_COMPARE(GblObject_name(pObj),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newInPlaceVaArgs_(GblTestSuite* pSelf, GblContext* pCtx) {
    TestObject obj;
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblObject_newInPlace((GblObject*)&obj, TEST_OBJECT_TYPE,
                                            "name",     "Fuckwad",
                                            "userdata", (void*)0xdeadbeef,
                                            NULL));

    GBL_TEST_COMPARE(GblObject_name(GBL_OBJECT(&obj)),        "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(GBL_OBJECT(&obj)),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(GBL_OBJECT(&obj)),  0);

    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(&obj)), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newVaArgsWithClass_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    GblObject* pObj = GblObject_newWithClass(pClass,
                                    "name",     "Fuckwad",
                                    "userdata", (void*)0xdeadbeef,
                                    NULL);

    GBL_TEST_COMPARE(GblObject_name(pObj),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_VERIFY_CALL(GblClass_destroyFloating(GBL_CLASS(pClass)));
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newInPlaceVaArgsWithClass_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));
    TestObject obj;
    GBL_API_VERIFY_CALL(GblObject_newInPlaceWithClass((GblObject*)&obj, pClass,
                                                        "name",     "Fuckwad",
                                                        "userdata", (void*)0xdeadbeef,
                                                        NULL));

    GBL_TEST_COMPARE(GblObject_name(GBL_OBJECT(&obj)),        "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(GBL_OBJECT(&obj)),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(GBL_OBJECT(&obj)),  0);

    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(&obj)), 0);
    GBL_API_VERIFY_CALL(GblClass_destroyFloating(GBL_CLASS(pClass)));
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newInPlaceVaArgsWithClassInPlace_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    TestObjectClass klass;
    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblClass_constructFloating(GBL_CLASS(&klass), TEST_OBJECT_TYPE));

    TestObject obj;
    GBL_API_VERIFY_CALL(GblObject_newInPlaceWithClass((GblObject*)&obj, GBL_OBJECT_CLASS(&klass),
                                                        "name",     "Fuckwad",
                                                        "userdata", (void*)0xdeadbeef,
                                                        NULL));

    GBL_API_VERIFY_CALL(GblInstance_sinkClass(GBL_INSTANCE(&obj)));

    GBL_TEST_COMPARE(GblObject_name(GBL_OBJECT(&obj)),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(GBL_OBJECT(&obj)),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(GBL_OBJECT(&obj)),  0);

    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(&obj)), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newVariants_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[0], GBL_STRING_TYPE, "Fuckwad"));
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[1], GBL_POINTER_TYPE, (void*)0xdeadbeef));

    GblObject* pObj = GblObject_newVariants(TEST_OBJECT_TYPE, 2, keys, variants);

    GBL_TEST_COMPARE(GblObject_name(pObj),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newVariantsWithClass_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[0], GBL_STRING_TYPE, "Fuckwad"));
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[1], GBL_POINTER_TYPE, (void*)0xdeadbeef));

    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    GblObject* pObj = GblObject_newVariantsWithClass(pClass, 2, keys, variants);

    GBL_TEST_COMPARE(GblObject_name(pObj),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_VERIFY_CALL(GblClass_destroyFloating(GBL_CLASS(pClass)));
    GBL_API_END();
}


static GBL_RESULT GblObjectTestSuite_newInPlaceVariants_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[0], GBL_STRING_TYPE, "Fuckwad"));
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[1], GBL_POINTER_TYPE, (void*)0xdeadbeef));

    TestObject obj;
    GblObject* pObj = &obj.base;

    GBL_API_VERIFY_CALL((GblObject_newInPlaceVariants(pObj, TEST_OBJECT_TYPE, 2, keys, variants)));

    GBL_TEST_COMPARE(GblObject_name(pObj),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_newInPlaceVariantsWithClass_(GblTestSuite* pSelf, GblContext* pCtx){
    GBL_UNUSED(pSelf);
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GBL_API_BEGIN(pCtx);
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[0], GBL_STRING_TYPE, "Fuckwad"));
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variants[1], GBL_POINTER_TYPE, (void*)0xdeadbeef));

    TestObject obj;
    GblObject* pObj = &obj.base;

    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    GBL_API_VERIFY_CALL(GblObject_newInPlaceVariantsWithClass(pObj, pClass, 2, keys, variants));

    GBL_TEST_COMPARE(GblObject_name(pObj),       "Fuckwad");
    GBL_TEST_COMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    GBL_TEST_COMPARE(GblObject_eventFilterCount(pObj),  0);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_VERIFY_CALL(GblClass_destroyFloating(GBL_CLASS(pClass)));
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_propertyGet_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObject* pObj0 = GblObject_new(GBL_OBJECT_TYPE, NULL);
    GBL_TEST_VERIFY(pObj0);

    GblObject* pObj1 = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Bulbasaur",
                                    "userdata", (void*)0xdeadbeef,
                                    "parent",   pObj0,
                                    "floater",  -77.7,
                                    "stringer", "Fuckin Inheritance!",
                                    NULL);
    GBL_TEST_VERIFY(pObj1);

    uint32_t    refCount    = 0;
    void*       pUserdata   = NULL;
    const char* pName       = NULL;
    GblObject*  pParent     = NULL;
    float       floater     = 0.0f;
    const char* pStringer   = NULL;

    GBL_RESULT result = GblObject_get(pObj1, "userdata", &pUserdata,
                                             "refCount", &refCount,
                                             "name",     &pName,
                                             "parent",   &pParent,
                                             "floater",  &floater,
                                             "stringer", &pStringer,
                                             NULL);

    GBL_TEST_COMPARE(result,         GBL_RESULT_SUCCESS);
    GBL_TEST_COMPARE(pUserdata,      (void*)0xdeadbeef);
    GBL_TEST_COMPARE(refCount,       1);
    GBL_TEST_COMPARE(pName,          "Bulbasaur");
    GBL_TEST_COMPARE(pParent,        pObj0);
    GBL_TEST_COMPARE(floater,        -77.7f);
    //GBL_TEST_COMPARE(pStringer, "Fuckin Inheritance!");

    GBL_TEST_COMPARE(GblObject_unref(pObj1), 0);
    GBL_TEST_COMPARE(GblObject_unref(pObj0), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_propertySet_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Bulbasaur",
                                    "userdata", (void*)0xdeadbeef,
                                    "parent",   NULL,
                                    "floater",  -77.7,
                                    "stringer", "Fuckin Inheritance!",
                                    NULL);
    GblVariant variant;
    void* pUd;
    GBL_API_VERIFY_CALL(GblVariant_constructValueCopy(&variant, GBL_POINTER_TYPE, (void*)0xcafebabe));
    GBL_API_VERIFY_CALL(GblObject_propertySetString(pObj, "userdata",  &variant));

    GBL_API_VERIFY_CALL(GblObject_getValue(pObj, "userdata", &pUd));
    GBL_TEST_COMPARE(pUd, (void*)0xcafebabe);

    GBL_API_VERIFY_CALL(GblObject_setValue(pObj, "userdata", (void*)0xbadf00d));
    GBL_API_VERIFY_CALL(GblObject_getValue(pObj, "userdata", &pUd));
    GBL_TEST_COMPARE(pUd, (void*)0xbadf00d);

    GBL_API_VERIFY_CALL(GblObject_set(pObj, "userdata", (void*)0x012345,
                                            "floater", 33.33f,
                                            NULL));
    float floater = 0.0f;
    GBL_API_VERIFY_CALL(GblObject_get(pObj, "userdata", &pUd,
                                            "floater", &floater,
                                            NULL));

    GBL_TEST_COMPARE(pUd, (void*)0x12345);
    GBL_TEST_COMPARE(floater, 33.33f);

    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_parenting_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    GblObject* pChild1 = GblObject_new(GBL_OBJECT_TYPE,
                                    "name",     "Child1",
                                    NULL);
    GblObject* pChild2 = GblObject_new(GBL_OBJECT_TYPE,
                                    "name",     "Child2",
                                    NULL);
    GblObject* pChild3 = GblObject_new(GBL_OBJECT_TYPE,
                                    "name",     "Child3",
                                    "parent",   pChild2,
                                    NULL);
    GblObject* pParent = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Parent",
                                    NULL);

    GblObject_parentSet(pChild1, pParent);
    GBL_TEST_COMPARE(GblObject_childFindByIndex(pParent, 0), pChild1);
    GBL_TEST_COMPARE(GblObject_parent(pChild1), pParent);

    GblObject_childAdd(pParent, pChild2);
    GBL_TEST_COMPARE(GblObject_parent(pChild2), pParent);
    GBL_TEST_COMPARE(GblObject_siblingNext(pChild1), pChild2);

    GBL_TEST_COMPARE(GblObject_parent(pChild3), pChild2);

    GBL_TEST_COMPARE(GblObject_ancestorFindByType(pChild3, TEST_OBJECT_TYPE), pParent);
    GBL_TEST_COMPARE(GblObject_ancestorFindByName(pChild2, "Parent"), pParent);
    GBL_TEST_COMPARE(GblObject_childFindByType(pChild2, GBL_OBJECT_TYPE), pChild3);
    GBL_TEST_COMPARE(GblObject_siblingFindByName(pChild1, "Child2"), pChild2);
    GBL_TEST_COMPARE(GblObject_childFindByName(pParent, "Child1"), pChild1);

    GblObject_childRemove(pParent, pChild1);
    GBL_TEST_COMPARE(GblObject_childFindByIndex(pParent, 0), pChild2);

    GblObject_childRemove(pChild2, pChild3);
    GBL_TEST_COMPARE(GblObject_childFindByIndex(pChild2, 0), NULL);

    GBL_TEST_COMPARE(GblObject_unref(pChild3), 0);
    GBL_TEST_COMPARE(GblObject_unref(pChild1), 0);
    GBL_TEST_COMPARE(GblObject_unref(pParent), 0);
    GBL_TEST_COMPARE(GblObject_unref(pChild2), 0);

    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_classSwizzle_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    // Create new object with name
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "DickWhisp",
                                    NULL);

    // "staticInt32" is a class-level property which is initialized to 77, fetch + verify
    int32_t value;
    GBL_TEST_VERIFY(GBL_RESULT_SUCCESS(GblObject_getValue(pObj, "staticInt32", &value)));
    GBL_TEST_COMPARE(value, 77);

    // pull a new, floating TestObjectClass out of our ass, preinitialized, ready-to-go
    TestObjectClass* pClass = TEST_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    // overwrite the default value
    pClass->staticInt32 = -666;

    // overwrite the existing class for the given object
    GBL_API_VERIFY_CALL(GblInstance_swizzleClass(GBL_INSTANCE(pObj), GBL_CLASS(pClass)));

    // sink swizzled class for the instance to claim ownership
    GBL_API_VERIFY_CALL(GblInstance_sinkClass(GBL_INSTANCE(pObj)));

    // now check the value of the static class property has changed accordingly
    GBL_API_VERIFY_CALL(GblObject_getValue(pObj, "staticInt32", &value));
    GBL_TEST_COMPARE(value, -666);

    // clean up our shit
    GBL_TEST_COMPARE(GblObject_unref(pObj), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_eventNotify_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    TestObject* pObj = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "DickWhisp",
                                    NULL);
    GblEvent event;
    GBL_API_VERIFY_CALL(GblEvent_construct(&event, GBL_EVENT_TYPE));

    GBL_TEST_COMPARE(pObj->eventHandlerCount, 0);
    GBL_TEST_COMPARE(pObj->eventHandlerLastType, GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GblEvent_state(&event), GBL_EVENT_STATE_PENDING);

    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pObj), &event));

    GBL_TEST_COMPARE(pObj->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pObj->eventHandlerLastType, GBL_EVENT_TYPE);
    GBL_TEST_COMPARE(GblEvent_state(&event), GBL_EVENT_STATE_PENDING);

    pObj->eventHandlerAccept = GBL_TRUE;

    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pObj), &event));

    GBL_TEST_COMPARE(pObj->eventHandlerCount, 2);
    GBL_TEST_COMPARE(pObj->eventHandlerLastType, GBL_EVENT_TYPE);
    GBL_TEST_COMPARE(GblEvent_state(&event), GBL_EVENT_STATE_ACCEPTED);

    GblEvent_destruct(&event);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pObj)), 0);
    GBL_API_END();
}


static GBL_RESULT GblObjectTestSuite_eventNotifyAncestors_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    TestObject* pGrand = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Grandparent",
                                    NULL);
    TestObject* pParent = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Parent",
                                    "parent", pGrand,
                                    NULL);
    TestObject* pChild = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Child",
                                    "parent", pParent,
                                    NULL);

    GblEvent event;
    GBL_API_VERIFY_CALL(GblEvent_construct(&event, GBL_EVENT_TYPE));

    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pChild), &event));

    GBL_TEST_COMPARE(pChild->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pParent->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pGrand->eventHandlerCount, 1);

    pParent->eventHandlerAccept = GBL_TRUE;

    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pChild), &event));

    GBL_TEST_COMPARE(pChild->eventHandlerCount, 2);
    GBL_TEST_COMPARE(pParent->eventHandlerCount, 2);
    GBL_TEST_COMPARE(pGrand->eventHandlerCount, 1);

    GblEvent_destruct(&event);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pChild)), 0);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pParent)), 0);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pGrand)), 0);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_eventSendAncestorsEmit(TestObject* pObj, GblEvent* pEvent) {
    GBL_API_BEGIN(pObj);
    GBL_API_EVENT(pEvent);
    GBL_API_END();
}

static GBL_RESULT GblObjectTestSuite_eventSendAncestors_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    TestObject* pGrand = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Grandparent",
                                    NULL);
    TestObject* pParent = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Parent",
                                    "parent", pGrand,
                                    NULL);
    TestObject* pChild = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Child",
                                    "parent", pParent,
                                    NULL);

    GblEvent event;
    GBL_API_VERIFY_CALL(GblEvent_construct(&event, GBL_EVENT_TYPE));
    pGrand->eventHandlerAccept = GBL_TRUE;

    GBL_API_VERIFY_CALL(GblObjectTestSuite_eventSendAncestorsEmit(pChild, &event));

    GBL_TEST_COMPARE(pChild->eventHandlerCount, 0);
    GBL_TEST_COMPARE(pParent->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pGrand->eventHandlerCount, 1);
    GBL_TEST_COMPARE(GblEvent_state(&event), GBL_EVENT_STATE_ACCEPTED);

    GblEvent_destruct(&event);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pChild)), 0);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pParent)), 0);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pGrand)), 0);
    GBL_API_END();
}


static GBL_RESULT GblObjectTestSuite_eventFilters_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_API_BEGIN(pCtx);
    TestObject* pGrand = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Grandparent",
                                    NULL);
    TestObject* pParent = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Parent",
                                    "parent", pGrand,
                                    NULL);
    TestObject* pChild = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "Child",
                                    "parent", pParent,
                                    NULL);

    GBL_API_VERIFY_CALL(GblObject_eventFilterInstall(GBL_OBJECT(pParent), GBL_IEVENT_FILTER(pChild)));

    GblEvent event;
    GBL_API_VERIFY_CALL(GblEvent_construct(&event, GBL_EVENT_TYPE));

    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pChild), &event));

    GBL_TEST_COMPARE(pChild->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pParent->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pGrand->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pChild->eventFilterCount, 1);
    GBL_TEST_COMPARE(pChild->eventFilterLastType, GBL_EVENT_TYPE);
    GBL_TEST_COMPARE(pChild->eventFilterLastTarget, GBL_IEVENT_HANDLER(pParent));

    pChild->eventFilterAccept = GBL_TRUE;

    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pChild), &event));
    GBL_TEST_COMPARE(pChild->eventHandlerCount, 2);
    GBL_TEST_COMPARE(pParent->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pGrand->eventHandlerCount, 1);
    GBL_TEST_COMPARE(pChild->eventFilterCount, 2);

    GBL_API_VERIFY_CALL(GblObject_eventFilterUninstall(GBL_OBJECT(pParent), GBL_IEVENT_FILTER(pChild)));

    event.state = GBL_EVENT_STATE_PENDING;
    GBL_API_VERIFY_CALL(GblObject_eventNotify(GBL_OBJECT(pChild), &event));
    GBL_TEST_COMPARE(pChild->eventHandlerCount, 3);
    GBL_TEST_COMPARE(pParent->eventHandlerCount, 2);
    GBL_TEST_COMPARE(pGrand->eventHandlerCount, 2);
    GBL_TEST_COMPARE(pChild->eventFilterCount, 2);

    GblEvent_destruct(&event);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pChild)), 0);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pParent)), 0);
    GBL_TEST_COMPARE(GblObject_unref(GBL_OBJECT(pGrand)), 0);
    GBL_API_END();
}

GBL_EXPORT GblType GblObjectTestSuite_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    const static GblTestCase cases[] = {
        { "newDefault",                         GblObjectTestSuite_newDefault_                          },
        { "name",                               GblObjectTestSuite_name_                                },
        { "ref",                                GblObjectTestSuite_ref_                                 },
        { "unref",                              GblObjectTestSuite_unref_                               },
        { "extendedData",                       GblObjectTestSuite_extendedData_                        },
        { "newVaArgs",                          GblObjectTestSuite_newVaArgs_                           },
        { "newInPlaceVaArgs",                   GblObjectTestSuite_newInPlaceVaArgs_                    },
        { "newVaArgsWithClass",                 GblObjectTestSuite_newVaArgsWithClass_                  },
        { "newInPlaceVaArgsWithClass",          GblObjectTestSuite_newInPlaceVaArgsWithClass_           },
        { "newInPlaceVaArgsWithClassInPlace",   GblObjectTestSuite_newInPlaceVaArgsWithClassInPlace_    },
        { "newVariants",                        GblObjectTestSuite_newVariants_                         },
        { "newVariantsWithClass",               GblObjectTestSuite_newVariantsWithClass_                },
        { "newInPlaceVariants",                 GblObjectTestSuite_newInPlaceVariants_                  },
        { "newInPlaceVariantsWithClass",        GblObjectTestSuite_newInPlaceVariantsWithClass_         },
        { "propertyGet",                        GblObjectTestSuite_propertyGet_                         },
        { "propertySet",                        GblObjectTestSuite_propertySet_                         },
        { "parenting",                          GblObjectTestSuite_parenting_                           },
        { "classSwizzle",                       GblObjectTestSuite_classSwizzle_                        },
        { "eventNotify",                        GblObjectTestSuite_eventNotify_                         },
        { "eventNotifyAncestors",               GblObjectTestSuite_eventNotifyAncestors_                },
        { "eventSendAncestors",                 GblObjectTestSuite_eventSendAncestors_                  },
        { "eventFilter",                        GblObjectTestSuite_eventFilters_                        },
        { NULL,                                 NULL                                                    }
    };

    const static GblTestSuiteClassVTable vTable = {
        .pFnSuiteInit   = GblObjectTestSuite_init_,
        .pFnSuiteFinal  = GblObjectTestSuite_final_,
        .pCases         = cases
    };

    if(type == GBL_INVALID_TYPE) {
        GBL_API_BEGIN(NULL);
        type = GblTestSuite_register(GblQuark_internStringStatic("ObjectTestSuite"),
                                     &vTable,
                                     sizeof(GblObjectTestSuite),
                                     sizeof(GblObjectTestSuite_),
                                     GBL_TYPE_FLAGS_NONE);
        GBL_API_VERIFY_LAST_RECORD();
        GBL_API_END_BLOCK();
    }
    return type;
}

#include <test_gimbal_object.hpp>
#include <gimbal/objects/gimbal_object.hpp>
#include <gimbal/objects/gimbal_event.h>
#include <gimbal/types/gimbal_variant.h>

#define TEST_OBJECT_TYPE                    (TestObject_type())
#define TEST_OBJECT_STRUCT                  TestObject
#define TEST_OBJECT_CLASS_STRUCT            TestObjectClass
#define TEST_OBJECT(instance)               GBL_TYPE_CAST_INSTANCE_PREFIX(instance, TEST_OBJECT)
#define TEST_OBJECT_CHECK(instance)         GBL_TYPE_CHECK_INSTANCE_PREFIX(instance, TEST_OBJECT)
#define TEST_OBJECT_CLASS(klass)            GBL_TYPE_CAST_CLASS_PREFIX(klass, TEST_OBJECT)
#define TEST_OBJECT_CLASS_CHECK(klass)      GBL_TYPE_CHECK_CLASS_PREFIX(klass, TEST_OBJECT)
#define TEST_OBJECT_GET_CLASS(instance)     GBL_TYPE_CAST_GET_CLASS_PREFIX(instance, TEST_OBJECT)

namespace gimbal::test {

static GblType TestObject_type(void);

struct TestObjectClass {
    GblObjectClass  base;
    int32_t staticInt32;
    char string[256];
};

struct TestObject {
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
};

static GBL_RESULT TestObject_IEventHandler_handleEvent(GblIEventHandler* pHandler, GblEvent* pEvent) {
    GBL_API_BEGIN(pHandler);
    TestObject* pTest = TEST_OBJECT(pHandler);
    pTest->eventHandlerCount++;
    pTest->eventHandlerLastType = GblEvent_type(pEvent);
    if(pTest->eventHandlerAccept) GblEvent_accept(pEvent);
    GBL_API_END();
}

static GBL_RESULT TestObject_IEventFilter_filterEvent(GblIEventFilter* pFilter, GblIEventHandler* pTarget, GblEvent* pEvent) {
    GBL_API_BEGIN(pFilter);
    TestObject* pTest = TEST_OBJECT(pFilter);
    pTest->eventFilterCount++;
    pTest->eventFilterLastType = GblEvent_type(pEvent);
    pTest->eventFilterLastTarget = pTarget;
    if(pTest->eventFilterAccept) GblEvent_accept(pEvent);
    GBL_API_END();
}

static GBL_RESULT TestObject_constructor(GblObject* pSelf) {
    GBL_API_BEGIN(pSelf);
    GblObjectClass* pParentClass = GBL_OBJECT_CLASS(GblInstance_superOf(GBL_INSTANCE(pSelf)));
    pParentClass->pFnConstructor(pSelf);
    TestObject* pTest = TEST_OBJECT(pSelf);
    pTest->floater = -NAN;
    strcpy(pTest->stringer, "INVALID");
    GBL_API_END();
}

static GBL_RESULT TestObject_constructed(GblObject* pSelf) {
    GBL_API_BEGIN(pSelf);
    GblObjectClass* pParentClass = GBL_OBJECT_CLASS(GblInstance_superOf(GBL_INSTANCE(pSelf)));
    if(pParentClass->pFnConstructed) pParentClass->pFnConstructed(pSelf);
    GBL_API_END();
}

static GBL_RESULT TestObject_destructor(GblObject* pSelf) {
    GBL_API_BEGIN(pSelf);
    GblObjectClass* pParentClass = GBL_OBJECT_CLASS(GblInstance_superOf(GBL_INSTANCE(pSelf)));
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

static GBL_RESULT TestObject_propertyGet(const GblObject* pSelf, GblUint slot, GblVariant* pValue, const GblProperty* pProp) {
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

static GBL_RESULT TestObject_propertySet(GblObject* pSelf, GblUint slot, const GblVariant* pValue, const GblProperty* pProp) {
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
        GBL_API_CALL(GblVariant_getValueCopy(pValue, &pStr));
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


static GblType TestObject_type(void) {
    static GblType type = GBL_TYPE_INVALID;
    if(type == GBL_TYPE_INVALID) {
        const GblTypeInfo info = {
            .pFnClassInit = [](GblClass* pClass,
                               const void* pData,
                               GblContext* pCtx) {
                auto pTestClass = TEST_OBJECT_CLASS(pClass);
                GBL_API_BEGIN(pCtx);
                if(GBL_CLASS_TYPE(pTestClass) == TEST_OBJECT_TYPE) {
                    gblPropertyTableInsert(GBL_CLASS_TYPE(pTestClass),
                                           gblQuarkFromStringStatic("floater"),
                                           TEST_OBJECT_PROPERTY_FLOATER,
                                           GBL_TYPE_FLOAT,
                                           GBL_PROPERTY_FLAG_READ | GBL_PROPERTY_FLAG_WRITE | GBL_PROPERTY_FLAG_CONSTRUCT);
                    gblPropertyTableInsert(GBL_CLASS_TYPE(pTestClass),
                                           gblQuarkFromStringStatic("stringer"),
                                           TEST_OBJECT_PROPERTY_STRINGER,
                                           GBL_TYPE_STRING,
                                           GBL_PROPERTY_FLAG_READ | GBL_PROPERTY_FLAG_WRITE | GBL_PROPERTY_FLAG_CONSTRUCT);
                    gblPropertyTableInsert(GBL_CLASS_TYPE(pTestClass),
                                           gblQuarkFromStringStatic("staticInt32"),
                                           TEST_OBJECT_PROPERTY_STATICINT32,
                                           GBL_TYPE_INT32,
                                           GBL_PROPERTY_FLAG_READ);
                    gblPropertyTableInsert(GBL_CLASS_TYPE(pTestClass),
                                           gblQuarkFromStringStatic("userdata"),
                                           TEST_OBJECT_PROPERTY_USERDATA,
                                           GBL_TYPE_POINTER,
                                           GBL_PROPERTY_FLAG_CONSTRUCT | GBL_PROPERTY_FLAG_READ | GBL_PROPERTY_FLAG_WRITE | GBL_PROPERTY_FLAG_OVERRIDE);
                }
                pTestClass->staticInt32 = 77;
                strcpy(pTestClass->string, (const char*)pData);
                pTestClass->base.iEventHandlerIFace.pFnEvent = TestObject_IEventHandler_handleEvent;
                pTestClass->base.iEventFilterIFace.pFnEventFilter = TestObject_IEventFilter_filterEvent;
                pTestClass->base.pFnConstructor = TestObject_constructor;
                pTestClass->base.pFnDestructor  = TestObject_destructor;
                pTestClass->base.pFnConstructed = TestObject_constructed;
                pTestClass->base.pFnPropertyGet = TestObject_propertyGet;
                pTestClass->base.pFnPropertySet = TestObject_propertySet;
                GBL_API_END();
            },
            .classSize      = sizeof(TestObjectClass),
            .classAlign     = alignof(TestObjectClass),
            .pClassData     = (void*)"Davey Havoc",
            .instanceSize   = sizeof(TestObject),
            .instanceAlign  = alignof(TestObject)
        };

        type = gblTypeRegisterStatic(GBL_TYPE_OBJECT,
                                     "TestObject",
                                    &info,
                                    GBL_TYPE_FLAGS_NONE);
    }
    return type;
}

void CObject::newDefault(void) {
    TestObject* pObj = TEST_OBJECT(GblObject_new(TEST_OBJECT_TYPE, NULL));
    pTestObj_ = pObj;

    QCOMPARE(gblTypeClassRefCount(TEST_OBJECT_TYPE), 1);

    // validate class
    TestObjectClass* pClass = TEST_OBJECT_GET_CLASS(pObj);
    QCOMPARE(gblTypeFromClass(GBL_CLASS(pClass)), TEST_OBJECT_TYPE);
    QCOMPARE(&pClass->base, GBL_OBJECT_CLASS(pClass));
    QCOMPARE(QString(pClass->string), "Davey Havoc");
    QCOMPARE(pClass->staticInt32, 77);

    // validate interfaces
    GblIVariantIFace* pIVariantIFace = GBL_IVARIANT_GET_IFACE(pObj);
    QCOMPARE(gblTypeFromClass(GBL_CLASS(pIVariantIFace)), GBL_TYPE_IVARIANT);
    QCOMPARE(pClass, TEST_OBJECT_CLASS(pIVariantIFace));
    QCOMPARE(QString(pIVariantIFace->pGetValueFmt), QString("p"));

    GblITableIFace* pITableIFace = GBL_ITABLE_GET_IFACE(pObj);
    QCOMPARE(gblTypeFromClass(GBL_CLASS(pITableIFace)), GBL_TYPE_ITABLE);
    QCOMPARE(pClass, TEST_OBJECT_CLASS(pITableIFace));

    GblIEventHandlerIFace* pIEventHandlerIFace = GBL_IEVENT_HANDLER_GET_IFACE(pObj);
    QCOMPARE(gblTypeFromClass(GBL_CLASS(pIEventHandlerIFace)), GBL_TYPE_IEVENT_HANDLER);
    QCOMPARE(pClass, TEST_OBJECT_CLASS(pIEventHandlerIFace));

    GblIEventFilterIFace* pIEventFilterIFace = GBL_IEVENT_FILTER_GET_IFACE(pObj);
    QCOMPARE(gblTypeFromClass(GBL_CLASS(pIEventFilterIFace)), GBL_TYPE_IEVENT_FILTER);
    QCOMPARE(pClass, TEST_OBJECT_CLASS(pIEventFilterIFace));

    // validate sanity of random casts
    QCOMPARE(pITableIFace, GBL_ITABLE_IFACE(pIEventHandlerIFace));
    QCOMPARE(pIVariantIFace, GBL_IVARIANT_IFACE(pIEventFilterIFace));

    //validate some of the compatible checks
    QVERIFY(GBL_IVARIANT_IFACE_CHECK(pClass));
    QVERIFY(!TEST_OBJECT_CLASS_CHECK(pIVariantIFace));
    QVERIFY(GBL_OBJECT_CLASS_CHECK(pClass));
    QVERIFY(GBL_ITABLE_IFACE_CHECK(GBL_OBJECT_CLASS(pITableIFace)));

    //validate insanity
    QCOMPARE(pClass, TEST_OBJECT_CLASS(GBL_ITABLE_IFACE(GBL_OBJECT_CLASS(GBL_OBJECT_CLASS(GBL_IEVENT_FILTER_IFACE(TEST_OBJECT_GET_CLASS(pObj)))))));

    // validate instance checks and casts
    QVERIFY(GBL_IVARIANT_CHECK(pObj));
    QVERIFY(GBL_IEVENT_FILTER_CHECK(GBL_OBJECT(GBL_IEVENT_FILTER(pObj))));
    QCOMPARE(pObj, TEST_OBJECT(GBL_IVARIANT(GBL_OBJECT(GBL_IEVENT_FILTER(TEST_OBJECT(pObj))))));
}

void CObject::ref(void) {
    QCOMPARE(GblObject_refCount(GBL_OBJECT(pTestObj_)), 1);
    GblObject* pObj = GBL_OBJECT(GblObject_ref(GBL_OBJECT(pTestObj_)));
    QCOMPARE(GBL_OBJECT(pTestObj_), pObj);
    QCOMPARE(GblObject_refCount(pObj), 2);
}

void CObject::unref(void) {
    QCOMPARE(GblObject_unref(GBL_OBJECT(pTestObj_)), 1);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pTestObj_)), 0);
    QCOMPARE(gblTypeClassRefCount(TEST_OBJECT_TYPE), 0);

    // K now replace what we deleted... lulz.
    pTestObj_ = nullptr;
}

void CObject::extendedData(void) {
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE, NULL);

    QCOMPARE(GblObject_name(pObj),           nullptr);
    QCOMPARE(GblObject_userdata(pObj),       nullptr);
    QCOMPARE(GblObject_parent(pObj),         nullptr);
    QCOMPARE(GblObject_childFirst(pObj),        nullptr);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    GblObject_nameSet(pObj, "Sing the Sorrow");
    QCOMPARE(GblObject_name(pObj), "Sing the Sorrow");

    GblObject_userdataSet(pObj, (void*)0xbadbeef);
    QCOMPARE(GblObject_userdata(pObj), (void*)0xbadbeef);

    // make sure we didn't screw up other extended data
    QCOMPARE(GblObject_parent(pObj),            nullptr);
    QCOMPARE(GblObject_childFirst(pObj),        nullptr);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);

}

void CObject::newVaArgs(void) {
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Fuckwad",
                                    "userdata", (void*)0xdeadbeef,
                                    NULL);

    QCOMPARE(GblObject_name(pObj),              "Fuckwad");
    QCOMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);
}

void CObject::newInPlaceVaArgs(void) {
    TestObject obj;
    verifyResult(GblObject_newInPlace((GblObject*)&obj, TEST_OBJECT_TYPE,
                                        "name",     "Fuckwad",
                                        "userdata", (void*)0xdeadbeef,
                                        NULL));

    QCOMPARE(GblObject_name(GBL_OBJECT(&obj)),              "Fuckwad");
    QCOMPARE(GblObject_userdata(GBL_OBJECT(&obj)),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(GBL_OBJECT(&obj)),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(GBL_OBJECT(&obj)),  0);

    QCOMPARE(GblObject_unref(GBL_OBJECT(&obj)), 0);
}

void CObject::newVaArgsWithClass(void) {
    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    GblObject* pObj = GblObject_newWithClass(pClass,
                                    "name",     "Fuckwad",
                                    "userdata", (void*)0xdeadbeef,
                                    NULL);

    QCOMPARE(GblObject_name(pObj),              "Fuckwad");
    QCOMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);
    verifyResult(gblTypeClassDestroyFloating(GBL_CLASS(pClass)));
}

void CObject::newInPlaceVaArgsWithClass(void) {
    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));
    TestObject obj;
    verifyResult(GblObject_newInPlaceWithClass((GblObject*)&obj, pClass,
                                                "name",     "Fuckwad",
                                                "userdata", (void*)0xdeadbeef,
                                                NULL));

    QCOMPARE(GblObject_name(GBL_OBJECT(&obj)),              "Fuckwad");
    QCOMPARE(GblObject_userdata(GBL_OBJECT(&obj)),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(GBL_OBJECT(&obj)),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(GBL_OBJECT(&obj)),  0);

    QCOMPARE(GblObject_unref(GBL_OBJECT(&obj)), 0);
    verifyResult(gblTypeClassDestroyFloating(GBL_CLASS(pClass)));
}

void CObject::newInPlaceVaArgsWithClassInPlace(void) {
    TestObjectClass klass;
    verifyResult(GblClass_constructFloating(GBL_CLASS(&klass), TEST_OBJECT_TYPE));

    TestObject obj;
    verifyResult(GblObject_newInPlaceWithClass((GblObject*)&obj, GBL_OBJECT_CLASS(&klass),
                                                "name",     "Fuckwad",
                                                "userdata", (void*)0xdeadbeef,
                                                NULL));

    verifyResult(GblObject_attributeSet(GBL_OBJECT(&obj), GBL_OBJECT_ATTRIBUTE_CLASS_OWNED, GBL_TRUE));
    verifyResult(GblObject_attributeSet(GBL_OBJECT(&obj), GBL_OBJECT_ATTRIBUTE_CLASS_CONSTRUCTED_IN_PLACE, GBL_TRUE));

    QCOMPARE(GblObject_name(GBL_OBJECT(&obj)),              "Fuckwad");
    QCOMPARE(GblObject_userdata(GBL_OBJECT(&obj)),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(GBL_OBJECT(&obj)),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(GBL_OBJECT(&obj)),  0);

    QCOMPARE(GblObject_unref(GBL_OBJECT(&obj)), 0);
}

void CObject::newVariants(void) {
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GblVariant_constructValueCopy(&variants[0], GBL_TYPE_STRING, "Fuckwad");
    GblVariant_constructValueCopy(&variants[1], GBL_TYPE_POINTER, (void*)0xdeadbeef);

    GblObject* pObj = GblObject_newVariants(TEST_OBJECT_TYPE, 2, keys, variants);

    QCOMPARE(GblObject_name(pObj),              "Fuckwad");
    QCOMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);
}

void CObject::newVariantsWithClass(void) {
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GblVariant_constructValueCopy(&variants[0], GBL_TYPE_STRING, "Fuckwad");
    GblVariant_constructValueCopy(&variants[1], GBL_TYPE_POINTER, (void*)0xdeadbeef);

    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    GblObject* pObj = GblObject_newVariantsWithClass(pClass, 2, keys, variants);

    QCOMPARE(GblObject_name(pObj),              "Fuckwad");
    QCOMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);
    verifyResult(gblTypeClassDestroyFloating(GBL_CLASS(pClass)));
}


void CObject::newInPlaceVariants(void) {
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GblVariant_constructValueCopy(&variants[0], GBL_TYPE_STRING, "Fuckwad");
    GblVariant_constructValueCopy(&variants[1], GBL_TYPE_POINTER, (void*)0xdeadbeef);

    TestObject obj;
    GblObject* pObj = &obj.base;

    verifyResult(GblObject_newInPlaceVariants(pObj, TEST_OBJECT_TYPE, 2, keys, variants));

    QCOMPARE(GblObject_name(pObj),              "Fuckwad");
    QCOMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);
}

void CObject::newInPlaceVariantsWithClass(void) {
    GblVariant variants[2] = {
        GBL_VARIANT_INIT,
        GBL_VARIANT_INIT
    };
    const char* keys[2] = {
        "name",
        "userdata"
    };

    GblVariant_constructValueCopy(&variants[0], GBL_TYPE_STRING, "Fuckwad");
    GblVariant_constructValueCopy(&variants[1], GBL_TYPE_POINTER, (void*)0xdeadbeef);

    TestObject obj;
    GblObject* pObj = &obj.base;

    GblObjectClass* pClass = GBL_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    verifyResult(GblObject_newInPlaceVariantsWithClass(pObj, pClass, 2, keys, variants));

    QCOMPARE(GblObject_name(pObj),              "Fuckwad");
    QCOMPARE(GblObject_userdata(pObj),          (void*)0xdeadbeef);
    QCOMPARE(GblObject_siblingNext(pObj),       nullptr);
    QCOMPARE(GblObject_eventFilterCount(pObj),  0);

    QCOMPARE(GblObject_unref(pObj), 0);
    verifyResult(gblTypeClassDestroyFloating(GBL_CLASS(pClass)));
}

void CObject::propertyGet(void) {
    GblObject* pObj0 = GblObject_new(GBL_TYPE_OBJECT, NULL);
    QVERIFY(pObj0);

    GblObject* pObj1 = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Bulbasaur",
                                    "userdata", (void*)0xdeadbeef,
                                    "parent",   pObj0,
                                    "floater",  -77.7,
                                    "stringer", "Fuckin Inheritance!",
                                    nullptr);
    QVERIFY(pObj1);

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
                                             nullptr);

    QCOMPARE(result,    GBL_RESULT_SUCCESS);
    QCOMPARE(pUserdata, (void*)0xdeadbeef);
    QCOMPARE(refCount,  1);
    QCOMPARE(pName,     "Bulbasaur");
    QCOMPARE(pParent,   pObj0);
    QCOMPARE(floater,   -77.7f);
    //QCOMPARE(pStringer, "Fuckin Inheritance!");

    QCOMPARE(GblObject_unref(pObj1), 0);
    QCOMPARE(GblObject_unref(pObj0), 0);
}

void CObject::propertySet(void) {
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Bulbasaur",
                                    "userdata", (void*)0xdeadbeef,
                                    "parent",   NULL,
                                    "floater",  -77.7,
                                    "stringer", "Fuckin Inheritance!",
                                    nullptr);
    GblVariant variant;
    void* pUd;
    GblVariant_constructValueCopy(&variant, GBL_TYPE_POINTER, (void*)0xcafebabe);
    GblObject_propertySetString(pObj, "userdata",  &variant);

    GblObject_getValue(pObj, "userdata", &pUd);
    QCOMPARE(pUd, (void*)0xcafebabe);

    GblObject_setValue(pObj, "userdata", (void*)0xbadf00d);
    GblObject_getValue(pObj, "userdata", &pUd);
    QCOMPARE(pUd, (void*)0xbadf00d);

    GblObject_set(pObj, "userdata", (void*)0x012345,
                        "floater", 33.33f,
                        NULL);
    float floater = 0.0f;
    GblObject_get(pObj, "userdata", &pUd,
                        "floater", &floater,
                        NULL);

    QCOMPARE(pUd, (void*)0x12345);
    QCOMPARE(floater, 33.33f);

    QCOMPARE(GblObject_unref(pObj), 0);
}

void CObject::parenting(void) {
    GblObject* pChild1 = GblObject_new(GBL_TYPE_OBJECT,
                                    "name",     "Child1",
                                    NULL);
    GblObject* pChild2 = GblObject_new(GBL_TYPE_OBJECT,
                                    "name",     "Child2",
                                    NULL);
    GblObject* pChild3 = GblObject_new(GBL_TYPE_OBJECT,
                                    "name",     "Child3",
                                    "parent",   pChild2,
                                    NULL);
    GblObject* pParent = GblObject_new(TEST_OBJECT_TYPE,
                                    "name",     "Parent",
                                    NULL);

    GblObject_parentSet(pChild1, pParent);
    QCOMPARE(GblObject_childFirst(pParent), pChild1);
    QCOMPARE(GblObject_parent(pChild1), pParent);

    GblObject_childAdd(pParent, pChild2);
    QCOMPARE(GblObject_parent(pChild2), pParent);
    QCOMPARE(GblObject_siblingNext(pChild1), pChild2);

    QCOMPARE(GblObject_parent(pChild3), pChild2);

    QCOMPARE(GblObject_ancestorFindByType(pChild3, TEST_OBJECT_TYPE), pParent);
    QCOMPARE(GblObject_ancestorFindByName(pChild2, "Parent"), pParent);
    QCOMPARE(GblObject_childFindByType(pChild2, GBL_TYPE_OBJECT), pChild3);
    QCOMPARE(GblObject_siblingFindByName(pChild1, "Child2"), pChild2);
    QCOMPARE(GblObject_childFindByName(pParent, "Child1"), pChild1);

    GblObject_childRemove(pParent, pChild1);
    QCOMPARE(GblObject_childFirst(pParent), pChild2);

    GblObject_childRemove(pChild2, pChild3);
    QCOMPARE(GblObject_childFirst(pChild2), nullptr);

    QCOMPARE(GblObject_unref(pChild3), 0);
    QCOMPARE(GblObject_unref(pChild1), 0);
    QCOMPARE(GblObject_unref(pParent), 0);
    QCOMPARE(GblObject_unref(pChild2), 0);
}

void CObject::classSwizzle(void) {
    // Create new object with name
    GblObject* pObj = GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "DickWhisp",
                                    NULL);

    // "staticInt32" is a class-level property which is initialized to 77, fetch + verify
    int32_t value;
    QVERIFY(GBL_RESULT_SUCCESS(GblObject_getValue(pObj, "staticInt32", &value)));
    QCOMPARE(value, 77);

    // pull a new, floating TestObjectClass out of our ass, preinitialized, ready-to-go
    TestObjectClass* pClass = TEST_OBJECT_CLASS(GblClass_createFloating(TEST_OBJECT_TYPE));

    // overwrite the default value
    pClass->staticInt32 = -666;

    // overwrite the existing class for the given object
    QVERIFY(GBL_RESULT_SUCCESS(gblTypeInstanceClassSwizzle(GBL_INSTANCE(pObj), GBL_CLASS(pClass))));

    // now check the value of the static class property has changed accordingly
    QVERIFY(GBL_RESULT_SUCCESS(GblObject_getValue(pObj, "staticInt32", &value)));
    QCOMPARE(value, -666);

    // clean up our shit
    QCOMPARE(GblObject_unref(pObj), 0);
}

void CObject::eventNotify(void) {
    TestObject* pObj = (TestObject*)GblObject_new(TEST_OBJECT_TYPE,
                                    "name", "DickWhisp",
                                    NULL);
    GblEvent event;
    verifyResult(GblEvent_construct(&event, GBL_TYPE_EVENT));

    QCOMPARE(pObj->eventHandlerCount, 0);
    QCOMPARE(pObj->eventHandlerLastType, GBL_TYPE_INVALID);
    QCOMPARE(GblEvent_state(&event), GBL_EVENT_STATE_PENDING);

    verifyResult(GblObject_eventNotify(GBL_OBJECT(pObj), &event));

    QCOMPARE(pObj->eventHandlerCount, 1);
    QCOMPARE(pObj->eventHandlerLastType, GBL_TYPE_EVENT);
    QCOMPARE(GblEvent_state(&event), GBL_EVENT_STATE_PENDING);

    pObj->eventHandlerAccept = GBL_TRUE;

    verifyResult(GblObject_eventNotify(GBL_OBJECT(pObj), &event));

    QCOMPARE(pObj->eventHandlerCount, 2);
    QCOMPARE(pObj->eventHandlerLastType, GBL_TYPE_EVENT);
    QCOMPARE(GblEvent_state(&event), GBL_EVENT_STATE_ACCEPTED);


    GblEvent_destruct(&event);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pObj)), 0);
}


void CObject::eventNotifyAncestors(void) {
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
    verifyResult(GblEvent_construct(&event, GBL_TYPE_EVENT));

    verifyResult(GblObject_eventNotify(GBL_OBJECT(pChild), &event));

    QCOMPARE(pChild->eventHandlerCount, 1);
    QCOMPARE(pParent->eventHandlerCount, 1);
    QCOMPARE(pGrand->eventHandlerCount, 1);

    pParent->eventHandlerAccept = GBL_TRUE;

    verifyResult(GblObject_eventNotify(GBL_OBJECT(pChild), &event));

    QCOMPARE(pChild->eventHandlerCount, 2);
    QCOMPARE(pParent->eventHandlerCount, 2);
    QCOMPARE(pGrand->eventHandlerCount, 1);

    GblEvent_destruct(&event);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pChild)), 0);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pParent)), 0);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pGrand)), 0);
}

void CObject::eventSendAncestors(void) {
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
    verifyResult(GblEvent_construct(&event, GBL_TYPE_EVENT));
    pGrand->eventHandlerAccept = GBL_TRUE;

    {
        GBL_API_BEGIN(pChild);
        GBL_API_EVENT(&event);
        GBL_API_END_BLOCK();
    }

    QCOMPARE(pChild->eventHandlerCount, 0);
    QCOMPARE(pParent->eventHandlerCount, 1);
    QCOMPARE(pGrand->eventHandlerCount, 1);
    QCOMPARE(GblEvent_state(&event), GBL_EVENT_STATE_ACCEPTED);

    GblEvent_destruct(&event);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pChild)), 0);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pParent)), 0);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pGrand)), 0);
}


void CObject::eventFilters(void) {
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

    verifyResult(GblObject_eventFilterInstall(GBL_OBJECT(pParent), GBL_IEVENT_FILTER(pChild)));

    GblEvent event;
    verifyResult(GblEvent_construct(&event, GBL_TYPE_EVENT));

    verifyResult(GblObject_eventNotify(GBL_OBJECT(pChild), &event));

    QCOMPARE(pChild->eventHandlerCount, 1);
    QCOMPARE(pParent->eventHandlerCount, 1);
    QCOMPARE(pGrand->eventHandlerCount, 1);
    QCOMPARE(pChild->eventFilterCount, 1);
    QCOMPARE(pChild->eventFilterLastType, GBL_TYPE_EVENT);
    QCOMPARE(pChild->eventFilterLastTarget, GBL_IEVENT_HANDLER(pParent));

    pChild->eventFilterAccept = GBL_TRUE;

    verifyResult(GblObject_eventNotify(GBL_OBJECT(pChild), &event));
    QCOMPARE(pChild->eventHandlerCount, 2);
    QCOMPARE(pParent->eventHandlerCount, 1);
    QCOMPARE(pGrand->eventHandlerCount, 1);
    QCOMPARE(pChild->eventFilterCount, 2);

    verifyResult(GblObject_eventFilterUninstall(GBL_OBJECT(pParent), GBL_IEVENT_FILTER(pChild)));

    event.state = GBL_EVENT_STATE_PENDING;
    verifyResult(GblObject_eventNotify(GBL_OBJECT(pChild), &event));
    QCOMPARE(pChild->eventHandlerCount, 3);
    QCOMPARE(pParent->eventHandlerCount, 2);
    QCOMPARE(pGrand->eventHandlerCount, 2);
    QCOMPARE(pChild->eventFilterCount, 2);

    GblEvent_destruct(&event);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pChild)), 0);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pParent)), 0);
    QCOMPARE(GblObject_unref(GBL_OBJECT(pGrand)), 0);
}


}
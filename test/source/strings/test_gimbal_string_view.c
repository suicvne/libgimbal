#include <gimbal/core/gimbal_api_frame.h>
#include <gimbal/strings/gimbal_string_view.h>
#include <gimbal/utils/gimbal_test.h>
#include "strings/test_gimbal_string_view.h"

GBL_API GblStringView_test_testCaseInit(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_API_END();
}

GBL_API GblStringView_test_testCaseFinal(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_API_END();
}

GBL_API GblStringView_test_fromEmpty(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_fromEmpty();
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);

    view = GBL_STRING_VIEW(NULL);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);

    view = GBL_STRV(NULL);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);

    GBL_API_END();
}
GBL_API GblStringView_test_fromString(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_fromString("lulz");
    GBL_COMPARE(view.length, 4);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);

    view = GBL_STRING_VIEW("lulz");
    GBL_COMPARE(view.length, 4);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);

    view = GBL_STRV("lulz");
    GBL_COMPARE(view.length, 4);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);

    view = GBL_STRV("");
    GBL_COMPARE(view.length, 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(strcmp(view.pData, ""), 0);

    GBL_API_END();
}

GBL_API GblStringView_test_fromStringSized(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_fromStringSized("lulz", 4);
    GBL_COMPARE(view.length, 4);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);

    view = GblStringView_fromStringSized("lulz", 0);
    GBL_COMPARE(view.length, 4);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);

    view = GBL_STRING_VIEW("lulz", 2);
    GBL_COMPARE(view.length, 2);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);

    GBL_API_END();
}

GBL_API GblStringView_test_fromQuark(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_fromQuark(GBL_QUARK_INVALID);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.length, 0);
    GBL_COMPARE(view.nullTerminated, 0);

    view = GblStringView_fromQuark(GblQuark_fromString("lulz"));
    GBL_COMPARE(strcmp(view.pData, "lulz"), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 4);

    GBL_API_END();
}

GBL_API GblStringView_test_compare(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_COMPARE(GblStringView_compare(GblStringView_fromEmpty(),
                                      GBL_STRV(NULL)), 0);

    GBL_COMPARE(GblStringView_compare(GBL_STRV(NULL),
                                      GBL_STRV("")), 0);

    GBL_COMPARE(GblStringView_compare(GBL_STRV("Lulz"),
                                      GBL_STRING_VIEW("Lulzy", 4)), 0);

    GBL_VERIFY(GblStringView_compare(GBL_STRV("lulz"),
                                     GBL_STRV(NULL)) > 0);

    GBL_VERIFY(GblStringView_compare(GBL_STRV(NULL),
                                     GBL_STRV("Lulz")) < 0);

    GBL_API_END();
}

GBL_API GblStringView_test_compareIgnoreCase(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_COMPARE(GblStringView_compareIgnoreCase(GblStringView_fromEmpty(),
                                                GBL_STRV(NULL)), 0);

    GBL_COMPARE(GblStringView_compareIgnoreCase(GBL_STRV(NULL),
                                                GBL_STRV("")), 0);

    GBL_COMPARE(GblStringView_compareIgnoreCase(GBL_STRV("Lulz"),
                                                GBL_STRING_VIEW("Lulzy", 4)), 0);

    GBL_VERIFY(GblStringView_compareIgnoreCase(GBL_STRV("lulz"),
                                                GBL_STRV(NULL)) > 0);

    GBL_VERIFY(GblStringView_compareIgnoreCase(GBL_STRV(NULL),
                                               GBL_STRV("Lulz")) < 0);

    GBL_COMPARE(GblStringView_compareIgnoreCase(GBL_STRV("LULZ"),
                                                GBL_STRING_VIEW("lUlZ", 4)), 0);
    GBL_API_END();
}

GBL_API GblStringView_test_equals(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_COMPARE(GblStringView_equals(GblStringView_fromEmpty(),
                                     GBL_STRV(NULL)), 1);

    GBL_COMPARE(GblStringView_equals(GBL_STRV(NULL),
                                     GBL_STRV("")), 1);

    GBL_COMPARE(GblStringView_equals(GBL_STRV("Lulz"),
                                     GBL_STRING_VIEW("Lulzy", 4)), 1);

    GBL_COMPARE(GblStringView_equals(GBL_STRV("Lulz"),
                                     GBL_STRING_VIEW("nope")), 0);

    GBL_COMPARE(GblStringView_equals(GBL_STRV("lulz"),
                                     GBL_STRV(NULL)), 0);

    GBL_COMPARE(GblStringView_equals(GBL_STRV(NULL),
                                     GBL_STRV("Lulz")), 0);

    GBL_API_END();
}


GBL_API GblStringView_test_equalsIgnoreCase(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_COMPARE(GblStringView_equalsIgnoreCase(GblStringView_fromEmpty(),
                                               GBL_STRV(NULL)), 1);

    GBL_COMPARE(GblStringView_equalsIgnoreCase(GBL_STRV(NULL),
                                               GBL_STRV("")), 1);

    GBL_COMPARE(GblStringView_equalsIgnoreCase(GBL_STRV("Lulz"),
                                               GBL_STRING_VIEW("Lulzy", 4)), 1);

    GBL_COMPARE(GblStringView_equalsIgnoreCase(GBL_STRV("Lulz"),
                                               GBL_STRING_VIEW("LULz", 4)), 1);

    GBL_COMPARE(GblStringView_equalsIgnoreCase(GBL_STRV("lulz"),
                                               GBL_STRV(NULL)), 0);

    GBL_COMPARE(GblStringView_equalsIgnoreCase(GBL_STRV(NULL),
                                               GBL_STRV("Lulz")), 0);

    GBL_API_END();
}

GBL_API GblStringView_test_copyInvalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_TEST_EXPECT_ERROR();

    char buffer[6] = { '\0' };

    GBL_COMPARE(GblStringView_copy(GBL_STRV("Trolo"), buffer, 2, 3),
                GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_copy(GBL_STRV(NULL), buffer, 0, 1),
                GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_API_END();
}


GBL_API GblStringView_test_copy(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    char buffer[6] = { '\0' };

    GBL_API_VERIFY_CALL(GblStringView_copy(GBL_STRV("Trolololo"), buffer, 3, 3));

    GBL_COMPARE(strcmp(buffer, "lol"), 0);

    GBL_API_END();
}

GBL_API GblStringView_test_empty(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(GblStringView_empty(GBL_STRV(NULL)));
    GBL_VERIFY(GblStringView_empty(GBL_STRV("")));
    GBL_VERIFY(!GblStringView_empty(GBL_STRV("x")));
    GBL_API_END();
}

GBL_API GblStringView_test_blank(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(GblStringView_blank(GBL_STRV(NULL)));
    GBL_VERIFY(GblStringView_blank(GBL_STRV("")));
    GBL_VERIFY(!GblStringView_blank(GBL_STRV("x")));
    GBL_VERIFY(GblStringView_blank(GBL_STRV("\n\r\t ")));
    GBL_VERIFY(!GblStringView_blank(GBL_STRV("   \t\r\nx")));
    GBL_API_END();
}

GBL_API GblStringView_test_at_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_TEST_EXPECT_ERROR();

    GBL_COMPARE(GblStringView_at(GBL_STRV(NULL), 0), '\0');
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_at(GBL_STRV(""), 0), '\0');
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_at(GBL_STRV("lulz"), strlen("lulz")), '\0');
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_API_END();
}

GBL_API GblStringView_test_at(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_at(GBL_STRV("lulz"), 0), 'l');
    GBL_COMPARE(GblStringView_at(GBL_STRV("lulz"), 3), 'z');
    GBL_API_END();
}


GBL_API GblStringView_test_first_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GBL_COMPARE(GblStringView_first(GBL_STRV(NULL)), '\0');
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_first(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_first(GBL_STRV("lulz")), 'l');
    GBL_API_END();
}

GBL_API GblStringView_test_last_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GBL_COMPARE(GblStringView_last(GBL_STRV(NULL)), '\0');
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_last(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_last(GBL_STRV("lulz")), 'z');
    GBL_API_END();
}

GBL_API GblStringView_test_removePrefix_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();

    GblStringView view = GblStringView_removePrefix(GBL_STRV(NULL), 1);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    view = GblStringView_removePrefix(GBL_STRV("lulz"), 5);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_API_END();
}
GBL_API GblStringView_test_removePrefix(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_removePrefix(GBL_STRV(""), 0);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, ""), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 0);

    view = GblStringView_removePrefix(GBL_STRV("lolz"), 0);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, "lolz"), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 4);

    view = GblStringView_removePrefix(GBL_STRV("lolz"), 2);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, "lz"), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 2);

    view = GblStringView_removePrefix(GBL_STRV("lolz"), 4);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, ""), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 0);

    GBL_API_END();
}
GBL_API GblStringView_test_removeSuffix_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_TEST_EXPECT_ERROR();

    GblStringView view = GblStringView_removeSuffix(GBL_STRV(NULL), 1);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    view = GblStringView_removeSuffix(GBL_STRV("lulz"), 5);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_API_END();
}
GBL_API GblStringView_test_removeSuffix(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_removeSuffix(GBL_STRV(""), 0);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, ""), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 0);

    view = GblStringView_removeSuffix(GBL_STRV("lolz"), 1);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_VERIFY(GblStringView_equals(view, GBL_STRV("lol")));
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 3);

    view = GblStringView_removeSuffix(GBL_STRV("lolz"), 4);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_VERIFY(GblStringView_equals(view, GBL_STRV("")));
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);

    GBL_API_END();
}

GBL_API GblStringView_test_chomp(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(GblStringView_equals(GblStringView_chomp(GBL_STRV(NULL)),
                                    GBL_STRV("")));
    GBL_VERIFY(GblStringView_equals(GblStringView_chomp(GBL_STRV("lolz")),
                                    GBL_STRV("lolz")));
    GBL_VERIFY(GblStringView_equals(GblStringView_chomp(GBL_STRV("lolz\n\n")),
                                    GBL_STRV("lolz\n")));
    GBL_VERIFY(GblStringView_equals(GblStringView_chomp(GBL_STRV("lolz\r\n\r\n")),
                                    GBL_STRV("lolz\r\n")));
    GBL_API_END();
}

GBL_API GblStringView_test_substr_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GBL_TEST_EXPECT_ERROR();

    GblStringView view = GblStringView_substr(GBL_STRV(NULL), 0, 1);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    GblStringView_substr(GBL_STRV(NULL), 1, 0);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    GblStringView_substr(GBL_STRV("lulz"), 0, 5);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    GblStringView_substr(GBL_STRV("lulz"), 1, 4);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_API_END();
}

GBL_API GblStringView_test_substr(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);

    GblStringView view = GblStringView_substr(GBL_STRV(NULL), 0, 0);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(view.pData, NULL);
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 0);

    view = GblStringView_substr(GBL_STRV(""), 0, 0);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, ""), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 0);

    view = GblStringView_substr(GBL_STRV("lolz"), 0, 4);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_COMPARE(strcmp(view.pData, "lolz"), 0);
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 4);

    view = GblStringView_substr(GBL_STRV("lolz"), 0, 3);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_VERIFY(GblStringView_equals(view, GBL_STRV("lol")));
    GBL_COMPARE(view.nullTerminated, 0);
    GBL_COMPARE(view.length, 3);

    view = GblStringView_substr(GBL_STRV("lolz"), 4, 0);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_VERIFY(GblStringView_equals(view, GBL_STRV("")));
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 0);


    view = GblStringView_substr(GBL_STRV("lolz"), 3, 1);
    GBL_API_VERIFY_LAST_RECORD();
    GBL_VERIFY(GblStringView_equals(view, GBL_STRV("z")));
    GBL_COMPARE(view.nullTerminated, 1);
    GBL_COMPARE(view.length, 1);

    GBL_API_END();
}

GBL_API GblStringView_test_startsWith(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(GblStringView_startsWith(GBL_STRV("lolz"), GBL_STRV("lo")));
    GBL_VERIFY(GblStringView_startsWith(GBL_STRV("lolz"), GBL_STRV("lolz")));
    GBL_VERIFY(!GblStringView_startsWith(GBL_STRV("lolz"), GBL_STRV("z")));
    GBL_VERIFY(GblStringView_startsWith(GBL_STRV(NULL), GBL_STRV(NULL)));
    GBL_VERIFY(GblStringView_startsWith(GBL_STRV(""), GBL_STRV(NULL)));
    GBL_API_END();
}

GBL_API GblStringView_test_endsWith(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(GblStringView_endsWith(GBL_STRV("lolz"), GBL_STRV("lz")));
    GBL_VERIFY(GblStringView_endsWith(GBL_STRV("lolz"), GBL_STRV("lolz")));
    GBL_VERIFY(!GblStringView_endsWith(GBL_STRV("lolz"), GBL_STRV(" lolz")));
    GBL_VERIFY(GblStringView_endsWith(GBL_STRV(NULL), GBL_STRV(NULL)));
    GBL_VERIFY(GblStringView_endsWith(GBL_STRV(""), GBL_STRV(NULL)));
    GBL_API_END();
}

GBL_API GblStringView_test_contains(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(GblStringView_contains(GBL_STRV(NULL), GBL_STRV("")));
    GBL_VERIFY(!GblStringView_contains(GBL_STRV("lol"), GBL_STRV("")));
    GBL_VERIFY(!GblStringView_contains(GBL_STRV(""), GBL_STRV("lol")));
    GBL_VERIFY(GblStringView_contains(GBL_STRV("lolzer"), GBL_STRV("lze")));
    GBL_VERIFY(GblStringView_contains(GBL_STRV("lolzer"), GBL_STRV("lolzer")));
    GBL_API_END();
}

GBL_API GblStringView_test_count(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_count(GBL_STRV(NULL), GBL_STRV("")), 0);
    GBL_COMPARE(GblStringView_count(GBL_STRV("lol"), GBL_STRV(NULL)), 0);
    GBL_COMPARE(GblStringView_count(GBL_STRV(""), GBL_STRV("lol")), 0);
    GBL_COMPARE(GblStringView_count(GBL_STRV("lol"), GBL_STRV("lol")), 1);
    GBL_COMPARE(GblStringView_count(GBL_STRV("lolol"), GBL_STRV("lol")), 1);
    GBL_COMPARE(GblStringView_count(GBL_STRV("lollolz"), GBL_STRV("lol")), 2);
    GBL_COMPARE(GblStringView_count(GBL_STRV("lollolz"), GBL_STRV("zero")), 0);
    GBL_API_END();
}

GBL_API GblStringView_test_find_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();

    GBL_COMPARE(GblStringView_find(GBL_STRV(NULL), GBL_STRV("lol"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV("lol"), 4), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_find(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_find(GBL_STRV(NULL), GBL_STRV("lol"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV("l"), 0), 0);
    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV("lol"), 0), 0);
    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV("ol"), 0), 1);
    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV("l"), 1), 2);
    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV("lz"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_find(GBL_STRV("lol"), GBL_STRV(NULL), 0), GBL_STRING_VIEW_NPOS);
    GBL_API_END();
}

GBL_API GblStringView_test_rfind_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();

    GBL_COMPARE(GblStringView_rfind(GBL_STRV(NULL), GBL_STRV("lol"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lol"), GBL_STRV("lol"), 4), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_rfind(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV(NULL), GBL_STRV("lol"), GBL_STRING_VIEW_NPOS), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lol"), GBL_STRV(""), GBL_STRING_VIEW_NPOS), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lolz"), GBL_STRV("lol"), GBL_STRING_VIEW_NPOS), 0);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lol"), GBL_STRV("lol"), 2), 0);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("zlol lol"), GBL_STRV("lol"), 4), 1);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lol"), GBL_STRV("lo"), 1), 0);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lol"), GBL_STRV("zl"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_rfind(GBL_STRV("lolol"), GBL_STRV("lol"), GBL_STRING_VIEW_NPOS), 2);
    GBL_API_END();
}

GBL_API GblStringView_test_findFirstOf_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV("lolz"), GBL_STRV(NULL), 5), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV(""), GBL_STRV("lolz"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_findFirstOf(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV("lolz"), GBL_STRV("o"), 0), 1);
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV("lolz"), GBL_STRV("abcl"), 1), 2);
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV("lolz"), GBL_STRV("zlo"), 0), 0);
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV("lolz"), GBL_STRV("a"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV("lolz"), GBL_STRV(NULL), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findFirstOf(GBL_STRV(""), GBL_STRV(NULL), 0), GBL_STRING_VIEW_NPOS);
    GBL_TEST_END();
}

GBL_API GblStringView_test_findFirstNotOf_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV("lolz"), GBL_STRV(NULL), 5), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV(""), GBL_STRV("lolz"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_findFirstNotOf(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV("lolz"), GBL_STRV("lz"), 0), 1);
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV("lolz"), GBL_STRV("abcl"), 1), 1);
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV("lolz"), GBL_STRV("alo"), 0), 3);
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV("lolz"), GBL_STRV("zol"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV("lolz"), GBL_STRV(NULL), 0), 0);
    GBL_COMPARE(GblStringView_findFirstNotOf(GBL_STRV(""), GBL_STRV(NULL), 0), 0);
    GBL_API_END();
}

GBL_API GblStringView_test_findLastOf_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV(NULL), 5), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV(""), GBL_STRV("lolz"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_findLastOf(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV("o"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV("o"), GBL_STRING_VIEW_NPOS), 1);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV("abcl"), 3), 2);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV("zlo"), 1), 1);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV("a"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV("lolz"), GBL_STRV(NULL), 3), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findLastOf(GBL_STRV(""), GBL_STRV(NULL), 0), GBL_STRING_VIEW_NPOS);
    GBL_API_END();
}


GBL_API GblStringView_test_findLastNotOf_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV(NULL), 5), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV(""), GBL_STRV("lolz"), 1), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_API_CLEAR_LAST_RECORD();
    GBL_API_END();
}

GBL_API GblStringView_test_findLastNotOf(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV("ozl"), 0), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV("oz"), 0), 0);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV("abcl"), 2), 1);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV("alz"), GBL_STRING_VIEW_NPOS), 1);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV("zol"), 3), GBL_STRING_VIEW_NPOS);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV("lolz"), GBL_STRV(NULL), 3), 3);
    GBL_COMPARE(GblStringView_findLastNotOf(GBL_STRV(""), GBL_STRV(NULL), 0), GBL_STRING_VIEW_NPOS);
    GBL_API_END();
}

GBL_API GblStringView_test_quark(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_quark(GBL_STRV(NULL)),
                GblQuark_fromString(""));

    GBL_COMPARE(GblStringView_quark(GBL_STRV("StringViewTestQuark")),
                GblQuark_fromString("StringViewTestQuark"));

    GBL_API_END();
}

GBL_API GblStringView_test_quarkTry(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_quarkTry(GBL_STRV(NULL)),
                GblQuark_tryString(""));

    GBL_COMPARE(GblStringView_quarkTry(GBL_STRV("Not Interned")),
                GblQuark_tryString("Not Interned"));

    GBL_COMPARE(GblStringView_quarkTry(GBL_STRV("StringViewTestQuark")),
                GblQuark_tryString("StringViewTestQuark"));
    GBL_API_END();
}

GBL_API GblStringView_test_intern(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_intern(GBL_STRV(NULL)),
                GblQuark_internString(""));

    GBL_COMPARE(GblStringView_intern(GBL_STRV("StringViewTestIntern")),
                GblQuark_internString("StringViewTestIntern"));

    GBL_API_END();
}

GBL_API GblStringView_test_hash(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_hash(GBL_STRV("trolo")),
                gblHashMurmur("trolo", 5));
    GBL_COMPARE(GblStringView_hash(GBL_STRV(NULL)), 0);

    GBL_API_END();
}

GBL_API GblStringView_test_toCString(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    char buffer[100];
    GBL_COMPARE(strcmp(GblStringView_toCString(GBL_STRV("lol"), buffer, sizeof(buffer)), "lol"), 0);
    GBL_COMPARE(strcmp(GblStringView_toCString(GBL_STRV("lol"), buffer, 3), "lo"), 0);
    GBL_API_END();
}

GBL_API GblStringView_test_toNil(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(!GblStringView_toNil(GBL_STRV("lol")));
    GBL_VERIFY(GblStringView_toNil(GBL_STRV(NULL)));
    GBL_VERIFY(GblStringView_toNil(GBL_STRV("")));
    GBL_VERIFY(GblStringView_toNil(GBL_STRV("nil")));
    GBL_API_END();
}

GBL_API GblStringView_test_toBool(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_VERIFY(!GblStringView_toBool(GBL_STRV("lol")));
    GBL_VERIFY(!GblStringView_toBool(GBL_STRV(NULL)));
    GBL_VERIFY(!GblStringView_toBool(GBL_STRV("")));
    GBL_VERIFY(!GblStringView_toBool(GBL_STRV("false")));
    GBL_VERIFY(GblStringView_toBool(GBL_STRV("true")));
    GBL_VERIFY(!GblStringView_toBool(GBL_STRV("0")));
    GBL_VERIFY(GblStringView_toBool(GBL_STRV("1")));
    GBL_VERIFY(!GblStringView_toBool(GBL_STRV("nil")));
    GBL_API_END();
}

GBL_API GblStringView_test_toInt(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_toInt(GBL_STRV(NULL)), 0);
    GBL_COMPARE(GblStringView_toInt(GBL_STRV("")), 0);
    GBL_COMPARE(GblStringView_toInt(GBL_STRV("0")), 0);
    GBL_COMPARE(GblStringView_toInt(GBL_STRV("-22")), -22);
    GBL_COMPARE(GblStringView_toInt(GBL_STRV("333")), 333);
    GBL_API_END();
}

GBL_API GblStringView_test_toUint(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_toUint(GBL_STRV(NULL)), 0);
    GBL_COMPARE(GblStringView_toUint(GBL_STRV("")), 0);
    GBL_COMPARE(GblStringView_toUint(GBL_STRV("0")), 0);
    GBL_COMPARE(GblStringView_toUint(GBL_STRV("-22")), (unsigned)-22);
    GBL_COMPARE(GblStringView_toUint(GBL_STRV("333")), 333);
    GBL_API_END();
}

GBL_API GblStringView_test_toFloat(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV(NULL)), 0.0f);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV("")), 0.0f);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV("0.0f")), 0.0f);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV("-22")), -22.0f);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV("333")), 333.0f);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV("3.33")), 3.33f);
    GBL_COMPARE(GblStringView_toFloat(GBL_STRV("-0.005")), -0.005f);
    GBL_API_END();
}

GBL_API GblStringView_test_toDouble(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV(NULL)), 0.0);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV("")), 0.0);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV("0.0")), 0.0);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV("-22")), -22.0);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV("333")), 333.0);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV("3.423")), 3.423);
    GBL_COMPARE(GblStringView_toDouble(GBL_STRV("-0.005")), -0.005);
    GBL_API_END();
}

GBL_API GblStringView_test_toPointer_invalid(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();

    GblStringView_toPointer(GBL_STRV(NULL));
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_API_CLEAR_LAST_RECORD();

    GblStringView_toPointer(GBL_STRV("lolz"));
    GBL_COMPARE(GBL_API_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_API_CLEAR_LAST_RECORD();

    GBL_API_END();
}

GBL_API GblStringView_test_toPointer(GblContext* pCtx) {
    GBL_API_BEGIN(pCtx);
    GBL_COMPARE(GblStringView_toPointer(GBL_STRV("0xdeadbeef")), (void*)0xdeadbeef);
    GBL_API_END();
}



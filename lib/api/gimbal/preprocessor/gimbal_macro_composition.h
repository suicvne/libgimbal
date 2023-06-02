/*! \file
 *  \brief Macro primitives for variadic function overloads and resolution
 *  \ingroup preprocessor
 *  \sa gimbal_sequences.h
 *
 *  \author Falco Girgis
 */

#ifndef GIMBAL_MACRO_COMPOSITION_H
#define GIMBAL_MACRO_COMPOSITION_H

#include "gimbal_macro_sequences.h"

// Generic meta macro primitives and constructs
#define GBL_IDENTITY(A)                 A
#define GBL_COMMA                       ,

#define GBL_VA_ARGS(...)                ,##__VA_ARGS__
#define GBL_VA_REQUIRE(...)             ...

#define GBL_STRINGIFY(a)                GBL_STRINGIFY_(a)
#define GBL_STRINGIFY_(a)               #a
#define GBL_STRINGIFY_MULTI(...)        GBL_STRINGIFY(__VA_ARGS__)

#define GBL_CAT(A, B)                   A##B
#define GBL_GLUE(A, B)                  GBL_CAT(A, B)

#define GBL_APPEND_SUFFIX(NAME, NUM)    GBL_CAT(NAME##_, NUM)
#define GBL_COMPOSE(NAME, ARGS)         NAME ARGS

#define GBL_MACRO_CALL(macro, ...)      macro(__VA_ARGS__)
#define GBL_MACRO_CALL_NULL(...)

#define GBL_MACRO_CONDITIONAL_CALL_0(macro, ...) \
    GBL_MACRO_CALL_NULL()
#define GBL_MACRO_CONDITIONAL_CALL_1(macro, ...) \
    GBL_MACRO_CALL(macro, __VA_ARGS__)

#define GBL_MACRO_CONDITIONAL_CALL(enabled, ...) \
    GBL_CAT(GBL_MACRO_CONDITIONAL_CALL_,enabled)(__VA_ARGS__)

#define GBL_NARG(...) \
         GBL_NARG_(__VA_ARGS__, GBL_RSEQ_N())
#define GBL_NARG_(...) \
         GBL_ARG_N(__VA_ARGS__)
#define GBL_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
         _71,_72,_73,_74,_75,_76,_77,_78,_79,_80, N,...) N
#define GBL_RSEQ_N() \
         80,\
         79,78,77,76,75,74,73,72,71,70, \
         69,68,67,66,65,64,63,62,61,60, \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define GBL_ARG_1(_1, ...) _1
#define GBL_ARG_2(_1, _2, ...) _2
#define GBL_ARG_3(_1, _2, _3, ...) _3
#define GBL_ARG_4(_1, _2, _3, _4, ...) _4
#define GBL_ARG_5(_1, _2, _3, _4, _5, ...) _5
#define GBL_ARG_6(_1, _2, _3, _4, _5, _6, ...) _6
#define GBL_ARG_7(_1, _2, _3, _4, _5, _6, _7, ...) _7
#define GBL_ARG_8(_1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define GBL_ARG_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9

// Variadic function overloading
#define GBL_VA_SLIDING_ARG(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, COUNT, ...) COUNT

#define GBL_VA_OVERLOAD_SUFFIXER(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) \
    GBL_COMPOSE(GBL_VA_SLIDING_ARG, (GBL_EVAL __VA_ARGS__ (), _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) )

#define GBL_VA_OVERLOAD_SUFFIXER_ARGC(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_1_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, N, N, N, N, N, N, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_0_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(N, N, N, N, N, N, N, N, N, N, 0, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_2_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, N, N, N, N, N, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_3_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, N, N, N, N, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_4_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, N, N, N, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_5_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, N, N, 5, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_6_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, N, 6, 5, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_7_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, N, 7, 6, 5, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_8_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, N, 8, 7, 6, 5, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SUFFIXER_9_N(...) \
    GBL_VA_OVERLOAD_SUFFIXER(0, N, 9, 8, 7, 6, 5, 4, 3, 2, 1, ##__VA_ARGS__)

#define GBL_VA_OVERLOAD_SELECT(BASE, SUFFIXER, ...) \
    GBL_APPEND_SUFFIX(BASE, SUFFIXER(__VA_ARGS__))

#define GBL_VA_OVERLOAD_CALL(BASE, SUFFIXER, ...) \
    GBL_VA_OVERLOAD_SELECT(BASE, SUFFIXER, ## __VA_ARGS__)(__VA_ARGS__)

#define GBL_VA_OVERLOAD_CALL_ARGC(BASE, ...) \
GBL_VA_OVERLOAD_CALL(BASE, GBL_VA_OVERLOAD_SUFFIXER_ARGC, ## __VA_ARGS__)

// Tuple processing
#define GBL_TUPLE_FIRST(...) GBL_TUPLE_FIRST__(__VA_ARGS__,)
#define GBL_TUPLE_FIRST__(X,...) X

#define GBL_TUPLE_REST(X,...) __VA_ARGS__

#define GBL_TUPLE_FOREACH(MACRO_,DATA_,TUPLE_) GBL_GLUE(GBL_TUPLE_FOREACH__,GBL_NARG TUPLE_)(MACRO_,DATA_,TUPLE_)

#define GBL_TUPLE_FOREACH__1(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_)
#define GBL_TUPLE_FOREACH__2(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__1(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__3(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__2(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__4(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__3(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__5(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__4(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__6(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__5(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__7(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__6(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__8(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__7(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__9(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__8(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__10(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__9(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__11(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__10(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__12(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__11(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__13(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__12(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__14(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__13(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__15(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__14(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__16(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__15(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__17(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__16(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__18(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__17(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__19(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__18(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__20(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__19(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__21(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__20(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__22(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__21(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__23(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__22(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__24(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__23(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__25(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__24(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__26(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__25(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__27(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__26(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__28(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__27(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__29(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__28(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__30(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__29(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__31(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__30(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__32(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__31(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__33(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__32(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__34(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__33(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__35(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__34(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__36(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__35(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__37(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__36(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__38(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__37(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__39(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__38(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__40(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__39(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__41(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__40(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__42(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__41(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__43(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__42(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__44(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__43(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__45(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__44(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__46(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__45(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__47(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__46(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__48(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__47(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__49(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__48(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__50(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__49(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__51(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__50(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__52(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__51(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__53(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__52(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__54(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__53(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__55(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__54(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__56(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__55(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__57(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__56(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__58(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__57(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__59(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__58(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__60(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__59(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__61(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__60(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__62(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__61(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__63(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__62(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__64(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__63(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__65(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__64(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__66(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__65(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__67(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__66(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__68(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__67(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__69(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__68(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__70(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__69(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__71(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__70(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__72(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__71(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__73(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__72(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__74(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__73(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__75(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__74(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__76(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__75(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__77(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__76(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__78(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__77(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__79(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__78(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))
#define GBL_TUPLE_FOREACH__80(MACRO_,DATA_,TUPLE_) MACRO_(DATA_,GBL_TUPLE_FIRST TUPLE_) GBL_TUPLE_FOREACH__79(MACRO_,DATA_,(GBL_TUPLE_REST TUPLE_))

#endif // GIMBAL_MACRO_COMPOSITION_H

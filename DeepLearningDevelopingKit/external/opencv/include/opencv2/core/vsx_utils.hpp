/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCV_HAL_VSX_UTILS_HPP
#define OPENCV_HAL_VSX_UTILS_HPP

#include "opencv2/core/cvdef.h"

#ifndef SKIP_INCLUDES
#   include <assert.h>
#endif

//! @addtogroup core_utils_vsx
//! @{
#if CV_VSX

#define __VSX_S16__(c, v) (c){v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}
#define __VSX_S8__(c, v)  (c){v, v, v, v, v, v, v, v}
#define __VSX_S4__(c, v)  (c){v, v, v, v}
#define __VSX_S2__(c, v)  (c){v, v}

typedef __vector unsigned char vec_uchar16;
#define vec_uchar16_set(...) (vec_uchar16){__VA_ARGS__}
#define vec_uchar16_sp(c)    (__VSX_S16__(vec_uchar16, c))
#define vec_uchar16_c(v)     ((vec_uchar16)(v))
#define vec_uchar16_mx       vec_uchar16_sp(0xFF)
#define vec_uchar16_mn       vec_uchar16_sp(0)
#define vec_uchar16_z        vec_uchar16_mn

typedef __vector signed char vec_char16;
#define vec_char16_set(...) (vec_char16){__VA_ARGS__}
#define vec_char16_sp(c)    (__VSX_S16__(vec_char16, c))
#define vec_char16_c(v)     ((vec_char16)(v))
#define vec_char16_mx       vec_char16_sp(0x7F)
#define vec_char16_mn       vec_char16_sp(-0x7F-1)
#define vec_char16_z        vec_char16_sp(0)

typedef __vector unsigned short vec_ushort8;
#define vec_ushort8_set(...) (vec_ushort8){__VA_ARGS__}
#define vec_ushort8_sp(c)    (__VSX_S8__(vec_ushort8, c))
#define vec_ushort8_c(v)     ((vec_ushort8)(v))
#define vec_ushort8_mx       vec_ushort8_sp(0xFFFF)
#define vec_ushort8_mn       vec_ushort8_sp(0)
#define vec_ushort8_z        vec_ushort8_mn

typedef __vector signed short vec_short8;
#define vec_short8_set(...) (vec_short8){__VA_ARGS__}
#define vec_short8_sp(c)    (__VSX_S8__(vec_short8, c))
#define vec_short8_c(v)     ((vec_short8)(v))
#define vec_short8_mx       vec_short8_sp(0x7FFF)
#define vec_short8_mn       vec_short8_sp(-0x7FFF-1)
#define vec_short8_z        vec_short8_sp(0)

typedef __vector unsigned int vec_uint4;
#define vec_uint4_set(...) (vec_uint4){__VA_ARGS__}
#define vec_uint4_sp(c)    (__VSX_S4__(vec_uint4, c))
#define vec_uint4_c(v)     ((vec_uint4)(v))
#define vec_uint4_mx       vec_uint4_sp(0xFFFFFFFFU)
#define vec_uint4_mn       vec_uint4_sp(0)
#define vec_uint4_z        vec_uint4_mn

typedef __vector signed int vec_int4;
#define vec_int4_set(...)  (vec_int4){__VA_ARGS__}
#define vec_int4_sp(c)     (__VSX_S4__(vec_int4, c))
#define vec_int4_c(v)      ((vec_int4)(v))
#define vec_int4_mx        vec_int4_sp(0x7FFFFFFF)
#define vec_int4_mn        vec_int4_sp(-0x7FFFFFFF-1)
#define vec_int4_z         vec_int4_sp(0)

typedef __vector float vec_float4;
#define vec_float4_set(...)  (vec_float4){__VA_ARGS__}
#define vec_float4_sp(c)     (__VSX_S4__(vec_float4, c))
#define vec_float4_c(v)      ((vec_float4)(v))
#define vec_float4_mx        vec_float4_sp(3.40282347E+38F)
#define vec_float4_mn        vec_float4_sp(1.17549435E-38F)
#define vec_float4_z         vec_float4_sp(0)

typedef __vector unsigned long long vec_udword2;
#define vec_udword2_set(...) (vec_udword2){__VA_ARGS__}
#define vec_udword2_sp(c)    (__VSX_S2__(vec_udword2, c))
#define vec_udword2_c(v)     ((vec_udword2)(v))
#define vec_udword2_mx       vec_udword2_sp(18446744073709551615ULL)
#define vec_udword2_mn       vec_udword2_sp(0)
#define vec_udword2_z        vec_udword2_mn

typedef __vector signed long long vec_dword2;
#define vec_dword2_set(...) (vec_dword2){__VA_ARGS__}
#define vec_dword2_sp(c)    (__VSX_S2__(vec_dword2, c))
#define vec_dword2_c(v)     ((vec_dword2)(v))
#define vec_dword2_mx       vec_dword2_sp(9223372036854775807LL)
#define vec_dword2_mn       vec_dword2_sp(-9223372036854775807LL-1)
#define vec_dword2_z        vec_dword2_sp(0)

typedef  __vector double vec_double2;
#define vec_double2_set(...) (vec_double2){__VA_ARGS__}
#define vec_double2_c(v)     ((vec_double2)(v))
#define vec_double2_sp(c)    (__VSX_S2__(vec_double2, c))
#define vec_double2_mx       vec_double2_sp(1.7976931348623157E+308)
#define vec_double2_mn       vec_double2_sp(2.2250738585072014E-308)
#define vec_double2_z        vec_double2_sp(0)

#define vec_bchar16           __vector __bool char
#define vec_bchar16_set(...) (vec_bchar16){__VA_ARGS__}
#define vec_bchar16_c(v)     ((vec_bchar16)(v))
#define vec_bchar16_f        (__VSX_S16__(vec_bchar16, 0))
#define vec_bchar16_t        (__VSX_S16__(vec_bchar16, 1))

#define vec_bshort8           __vector __bool short
#define vec_bshort8_set(...) (vec_bshort8){__VA_ARGS__}
#define vec_bshort8_c(v)     ((vec_bshort8)(v))
#define vec_bshort8_f        (__VSX_S8__(vec_bshort8, 0))
#define vec_bshort8_t        (__VSX_S8__(vec_bshort8, 1))

#define vec_bint4             __vector __bool int
#define vec_bint4_set(...)   (vec_bint4){__VA_ARGS__}
#define vec_bint4_c(v)       ((vec_bint4)(v))
#define vec_bint4_f          (__VSX_S4__(vec_bint4, 0))
#define vec_bint4_t          (__VSX_S4__(vec_bint4, 1))

#define vec_bdword2            __vector __bool long long
#define vec_bdword2_set(...)  (vec_bdword2){__VA_ARGS__}
#define vec_bdword2_c(v)      ((vec_bdword2)(v))
#define vec_bdword2_f         (__VSX_S2__(vec_bdword2, 0))
#define vec_bdword2_t         (__VSX_S2__(vec_bdword2, 1))


#define VSX_FINLINE(tp) extern inline tp __attribute__((always_inline))

#define VSX_REDIRECT_1RG(rt, rg, fnm, fn2)   \
VSX_FINLINE(rt) fnm(const rg& a) { return fn2(a); }

#define VSX_REDIRECT_2RG(rt, rg, fnm, fn2)   \
VSX_FINLINE(rt) fnm(const rg& a, const rg& b) { return fn2(a, b); }

/*
 * GCC VSX compatibility
**/
#if defined(__GNUG__) && !defined(__clang__)

// inline asm helper
#define VSX_IMPL_1RG(rt, rto, rg, rgo, opc, fnm) \
VSX_FINLINE(rt) fnm(const rg& a)                 \
{ rt rs; __asm__ __volatile__(#opc" %x0,%x1" : "="#rto (rs) : #rgo (a)); return rs; }

#define VSX_IMPL_1VRG(rt, rg, opc, fnm) \
VSX_FINLINE(rt) fnm(const rg& a)        \
{ rt rs; __asm__ __volatile__(#opc" %0,%1" : "=v" (rs) : "v" (a)); return rs; }

#define VSX_IMPL_2VRG_F(rt, rg, fopc, fnm)     \
VSX_FINLINE(rt) fnm(const rg& a, const rg& b)  \
{ rt rs; __asm__ __volatile__(fopc : "=v" (rs) : "v" (a), "v" (b)); return rs; }

#define VSX_IMPL_2VRG(rt, rg, opc, fnm) VSX_IMPL_2VRG_F(rt, rg, #opc" %0,%1,%2", fnm)

#if __GNUG__ < 7
// up to GCC 6 vec_mul only supports precisions and llong
#   ifdef vec_mul
#       undef vec_mul
#   endif
/*
 * there's no a direct instruction for supporting 16-bit multiplication in ISA 2.07,
 * XLC Implement it by using instruction "multiply even", "multiply odd" and "permute"
 * todo: Do I need to support 8-bit ?
**/
#   define VSX_IMPL_MULH(Tvec, Tcast)                                               \
    VSX_FINLINE(Tvec) vec_mul(const Tvec& a, const Tvec& b)                         \
    {                                                                               \
        static const vec_uchar16 even_perm = {0, 1, 16, 17, 4, 5, 20, 21,           \
                                              8, 9, 24, 25, 12, 13, 28, 29};        \
        return vec_perm(Tcast(vec_mule(a, b)), Tcast(vec_mulo(a, b)), even_perm);   \
    }
    VSX_IMPL_MULH(vec_short8,  vec_short8_c)
    VSX_IMPL_MULH(vec_ushort8, vec_ushort8_c)
    // vmuluwm can be used for unsigned or signed integers, that's what they said
    VSX_IMPL_2VRG(vec_int4,  vec_int4,  vmuluwm, vec_mul)
    VSX_IMPL_2VRG(vec_uint4, vec_uint4, vmuluwm, vec_mul)
    // redirect to GCC builtin vec_mul, since it already supports precisions and llong
    VSX_REDIRECT_2RG(vec_float4,  vec_float4,  vec_mul, __builtin_vec_mul)
    VSX_REDIRECT_2RG(vec_double2, vec_double2, vec_mul, __builtin_vec_mul)
    VSX_REDIRECT_2RG(vec_dword2,  vec_dword2,  vec_mul, __builtin_vec_mul)
    VSX_REDIRECT_2RG(vec_udword2, vec_udword2, vec_mul, __builtin_vec_mul)
#endif // __GNUG__ < 7

#if __GNUG__ < 6
/*
 * Instruction "compare greater than or equal" in ISA 2.07 only supports single
 * and double precision.
 * In XLC and new versions of GCC implement integers by using instruction "greater than" and NOR.
**/
#   ifdef vec_cmpge
#       undef vec_cmpge
#   endif
#   ifdef vec_cmple
#       undef vec_cmple
#   endif
#   define vec_cmple(a, b) vec_cmpge(b, a)
#   define VSX_IMPL_CMPGE(rt, rg, opc, fnm) \
    VSX_IMPL_2VRG_F(rt, rg, #opc" %0,%2,%1\n\t xxlnor %x0,%x0,%x0", fnm)

    VSX_IMPL_CMPGE(vec_bchar16, vec_char16,  vcmpgtsb, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bchar16, vec_uchar16, vcmpgtub, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bshort8, vec_short8,  vcmpgtsh, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bshort8, vec_ushort8, vcmpgtuh, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bint4,   vec_int4,    vcmpgtsw, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bint4,   vec_uint4,   vcmpgtuw, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bdword2, vec_dword2,  vcmpgtsd, vec_cmpge)
    VSX_IMPL_CMPGE(vec_bdword2, vec_udword2, vcmpgtud, vec_cmpge)

// redirect to GCC builtin cmpge, since it already supports precisions
    VSX_REDIRECT_2RG(vec_bint4,   vec_float4,  vec_cmpge, __builtin_vec_cmpge)
    VSX_REDIRECT_2RG(vec_bdword2, vec_double2, vec_cmpge, __builtin_vec_cmpge)

// up to gcc5 vec_nor doesn't support bool long long
#   undef vec_nor
    template<typename T>
    VSX_REDIRECT_2RG(T, T, vec_nor, __builtin_vec_nor)

    VSX_FINLINE(vec_bdword2) vec_nor(const vec_bdword2& a, const vec_bdword2& b)
    { return vec_bdword2_c(__builtin_vec_nor(vec_dword2_c(a), vec_dword2_c(b))); }

// vec_packs doesn't support double words in gcc4 and old versions of gcc5
#   undef vec_packs
    VSX_REDIRECT_2RG(vec_char16,  vec_short8,  vec_packs, __builtin_vec_packs)
    VSX_REDIRECT_2RG(vec_uchar16, vec_ushort8, vec_packs, __builtin_vec_packs)
    VSX_REDIRECT_2RG(vec_short8,  vec_int4,    vec_packs, __builtin_vec_packs)
    VSX_REDIRECT_2RG(vec_ushort8, vec_uint4,   vec_packs, __builtin_vec_packs)

    VSX_IMPL_2VRG_F(vec_int4,  vec_dword2,  "vpksdss %0,%2,%1", vec_packs)
    VSX_IMPL_2VRG_F(vec_uint4, vec_udword2, "vpkudus %0,%2,%1", vec_packs)
#endif // __GNUG__ < 6

#if __GNUG__ < 5
// vec_xxpermdi in gcc4 missing little-endian supports just like clang
#   define vec_permi(a, b, c) vec_xxpermdi(b, a, (3 ^ ((c & 1) << 1 | c >> 1)))
#else
#   define vec_permi vec_xxpermdi
#endif // __GNUG__ < 5

// shift left double by word immediate
#ifndef vec_sldw
#   define vec_sldw __builtin_vsx_xxsldwi
#endif

// vector population count
VSX_IMPL_1VRG(vec_uchar16, vec_uchar16, vpopcntb, vec_popcntu)
VSX_IMPL_1VRG(vec_uchar16, vec_char16,  vpopcntb, vec_popcntu)
VSX_IMPL_1VRG(vec_ushort8, vec_ushort8, vpopcnth, vec_popcntu)
VSX_IMPL_1VRG(vec_ushort8, vec_short8,  vpopcnth, vec_popcntu)
VSX_IMPL_1VRG(vec_uint4,   vec_uint4,   vpopcntw, vec_popcntu)
VSX_IMPL_1VRG(vec_uint4,   vec_int4,    vpopcntw, vec_popcntu)
VSX_IMPL_1VRG(vec_udword2, vec_udword2, vpopcntd, vec_popcntu)
VSX_IMPL_1VRG(vec_udword2, vec_dword2,  vpopcntd, vec_popcntu)

// converts between single and double-precision
VSX_REDIRECT_1RG(vec_float4,  vec_double2, vec_cvfo, __builtin_vsx_xvcvdpsp)
VSX_REDIRECT_1RG(vec_double2, vec_float4,  vec_cvfo, __builtin_vsx_xvcvspdp)

// converts word and doubleword to double-precision
#ifdef vec_ctd
#   undef vec_ctd
#endif
VSX_IMPL_1RG(vec_double2, wd, vec_int4,    wa, xvcvsxwdp, vec_ctdo)
VSX_IMPL_1RG(vec_double2, wd, vec_uint4,   wa, xvcvuxwdp, vec_ctdo)
VSX_IMPL_1RG(vec_double2, wd, vec_dword2,  wi, xvcvsxddp, vec_ctd)
VSX_IMPL_1RG(vec_double2, wd, vec_udword2, wi, xvcvuxddp, vec_ctd)

// converts word and doubleword to single-precision
#undef vec_ctf
VSX_IMPL_1RG(vec_float4, wf, vec_int4,    wa, xvcvsxwsp, vec_ctf)
VSX_IMPL_1RG(vec_float4, wf, vec_uint4,   wa, xvcvuxwsp, vec_ctf)
VSX_IMPL_1RG(vec_float4, wf, vec_dword2,  wi, xvcvsxdsp, vec_ctfo)
VSX_IMPL_1RG(vec_float4, wf, vec_udword2, wi, xvcvuxdsp, vec_ctfo)

// converts single and double precision to signed word
#undef vec_cts
VSX_IMPL_1RG(vec_int4,  wa, vec_double2, wd, xvcvdpsxws, vec_ctso)
VSX_IMPL_1RG(vec_int4,  wa, vec_float4,  wf, xvcvspsxws, vec_cts)

// converts single and double precision to unsigned word
#undef vec_ctu
VSX_IMPL_1RG(vec_uint4, wa, vec_double2, wd, xvcvdpuxws, vec_ctuo)
VSX_IMPL_1RG(vec_uint4, wa, vec_float4,  wf, xvcvspuxws, vec_ctu)

// converts single and double precision to signed doubleword
#ifdef vec_ctsl
#   undef vec_ctsl
#endif
VSX_IMPL_1RG(vec_dword2, wi, vec_double2, wd, xvcvdpsxds, vec_ctsl)
VSX_IMPL_1RG(vec_dword2, wi, vec_float4,  wf, xvcvspsxds, vec_ctslo)

// converts single and double precision to unsigned doubleword
#ifdef vec_ctul
#   undef vec_ctul
#endif
VSX_IMPL_1RG(vec_udword2, wi, vec_double2, wd, xvcvdpuxds, vec_ctul)
VSX_IMPL_1RG(vec_udword2, wi, vec_float4,  wf, xvcvspuxds, vec_ctulo)

// just in case if GCC doesn't define it
#ifndef vec_xl
#   define vec_xl vec_vsx_ld
#   define vec_xst vec_vsx_st
#endif

#endif // GCC VSX compatibility

/*
 * CLANG VSX compatibility
**/
#if defined(__clang__) && !defined(__IBMCPP__)

/*
 * CLANG doesn't support %x<n> in the inline asm template which fixes register number
 * when using any of the register constraints wa, wd, wf
 *
 * For more explanation checkout PowerPC and IBM RS6000 in https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
 * Also there's already an open bug https://bugs.llvm.org/show_bug.cgi?id=31837
 *
 * So we're not able to use inline asm and only use built-in functions that CLANG supports
 * and use __builtin_convertvector if clang missng any of vector conversions built-in functions
*/

// convert vector helper
#define VSX_IMPL_CONVERT(rt, rg, fnm) \
VSX_FINLINE(rt) fnm(const rg& a) { return __builtin_convertvector(a, rt); }

#if __clang_major__ < 5
// implement vec_permi in a dirty way
#   define VSX_IMPL_CLANG_4_PERMI(Tvec)                                                 \
    VSX_FINLINE(Tvec) vec_permi(const Tvec& a, const Tvec& b, unsigned const char c)    \
    {                                                                                   \
        switch (c)                                                                      \
        {                                                                               \
        case 0:                                                                         \
            return vec_mergeh(a, b);                                                    \
        case 1:                                                                         \
            return vec_mergel(vec_mergeh(a, a), b);                                     \
        case 2:                                                                         \
            return vec_mergeh(vec_mergel(a, a), b);                                     \
        default:                                                                        \
            return vec_mergel(a, b);                                                    \
        }                                                                               \
    }
    VSX_IMPL_CLANG_4_PERMI(vec_udword2)
    VSX_IMPL_CLANG_4_PERMI(vec_dword2)
    VSX_IMPL_CLANG_4_PERMI(vec_double2)

// vec_xxsldwi is missing in clang 4
#   define vec_xxsldwi(a, b, c) vec_sld(a, b, (c) * 4)
#else
// vec_xxpermdi is missing little-endian supports in clang 4 just like gcc4
#   define vec_permi(a, b, c) vec_xxpermdi(b, a, (3 ^ ((c & 1) << 1 | c >> 1)))
#endif // __clang_major__ < 5

// shift left double by word immediate
#ifndef vec_sldw
#   define vec_sldw vec_xxsldwi
#endif

// Implement vec_rsqrt since clang only supports vec_rsqrte
#ifndef vec_rsqrt
    VSX_FINLINE(vec_float4) vec_rsqrt(const vec_float4& a)
    { return vec_div(vec_float4_sp(1), vec_sqrt(a)); }

    VSX_FINLINE(vec_double2) vec_rsqrt(const vec_double2& a)
    { return vec_div(vec_double2_sp(1), vec_sqrt(a)); }
#endif

// vec_promote missing support for doubleword
VSX_FINLINE(vec_dword2) vec_promote(long long a, int b)
{
    vec_dword2 ret = vec_dword2_z;
    ret[b & 1] = a;
    return ret;
}

VSX_FINLINE(vec_udword2) vec_promote(unsigned long long a, int b)
{
    vec_udword2 ret = vec_udword2_z;
    ret[b & 1] = a;
    return ret;
}

// vec_popcnt should return unsigned but clang has different thought just like gcc in vec_vpopcnt
#define VSX_IMPL_POPCNTU(Tvec, Tvec2, ucast)   \
VSX_FINLINE(Tvec) vec_popcntu(const Tvec2& a)  \
{ return ucast(vec_popcnt(a)); }
VSX_IMPL_POPCNTU(vec_uchar16, vec_char16, vec_uchar16_c);
VSX_IMPL_POPCNTU(vec_ushort8, vec_short8, vec_ushort8_c);
VSX_IMPL_POPCNTU(vec_uint4,   vec_int4,   vec_uint4_c);
// redirect unsigned types
VSX_REDIRECT_1RG(vec_uchar16, vec_uchar16, vec_popcntu, vec_popcnt)
VSX_REDIRECT_1RG(vec_ushort8, vec_ushort8, vec_popcntu, vec_popcnt)
VSX_REDIRECT_1RG(vec_uint4,   vec_uint4,   vec_popcntu, vec_popcnt)

// converts between single and double precision
VSX_REDIRECT_1RG(vec_float4,  vec_double2, vec_cvfo, __builtin_vsx_xvcvdpsp)
VSX_REDIRECT_1RG(vec_double2, vec_float4,  vec_cvfo, __builtin_vsx_xvcvspdp)

// converts word and doubleword to double-precision
#ifdef vec_ctd
#   undef vec_ctd
#endif
VSX_REDIRECT_1RG(vec_double2, vec_int4,  vec_ctdo, __builtin_vsx_xvcvsxwdp)
VSX_REDIRECT_1RG(vec_double2, vec_uint4, vec_ctdo, __builtin_vsx_xvcvuxwdp)

VSX_IMPL_CONVERT(vec_double2, vec_dword2,  vec_ctd)
VSX_IMPL_CONVERT(vec_double2, vec_udword2, vec_ctd)

// converts word and doubleword to single-precision
#if __clang_major__ > 4
#   undef vec_ctf
#endif
VSX_IMPL_CONVERT(vec_float4, vec_int4,    vec_ctf)
VSX_IMPL_CONVERT(vec_float4, vec_uint4,   vec_ctf)
VSX_REDIRECT_1RG(vec_float4, vec_dword2,  vec_ctfo, __builtin_vsx_xvcvsxdsp)
VSX_REDIRECT_1RG(vec_float4, vec_udword2, vec_ctfo, __builtin_vsx_xvcvuxdsp)

// converts single and double precision to signed word
#if __clang_major__ > 4
#   undef vec_cts
#endif
VSX_REDIRECT_1RG(vec_int4,  vec_double2, vec_ctso, __builtin_vsx_xvcvdpsxws)
VSX_IMPL_CONVERT(vec_int4,  vec_float4,  vec_cts)

// converts single and double precision to unsigned word
#if __clang_major__ > 4
#   undef vec_ctu
#endif
VSX_REDIRECT_1RG(vec_uint4, vec_double2, vec_ctuo, __builtin_vsx_xvcvdpuxws)
VSX_IMPL_CONVERT(vec_uint4, vec_float4,  vec_ctu)

// converts single and double precision to signed doubleword
#ifdef vec_ctsl
#   undef vec_ctsl
#endif
VSX_IMPL_CONVERT(vec_dword2, vec_double2, vec_ctsl)
// __builtin_convertvector unable to convert, xvcvspsxds is missing on it
VSX_FINLINE(vec_dword2) vec_ctslo(const vec_float4& a)
{ return vec_ctsl(vec_cvfo(a)); }

// converts single and double precision to unsigned doubleword
#ifdef vec_ctul
#   undef vec_ctul
#endif
VSX_IMPL_CONVERT(vec_udword2, vec_double2, vec_ctul)
// __builtin_convertvector unable to convert, xvcvspuxds is missing on it
VSX_FINLINE(vec_udword2) vec_ctulo(const vec_float4& a)
{ return vec_ctul(vec_cvfo(a)); }

#endif // CLANG VSX compatibility

/*
 * Common GCC, CLANG compatibility
**/
#if defined(__GNUG__) && !defined(__IBMCPP__)

#ifdef vec_cvf
#   undef vec_cvf
#endif

#define VSX_IMPL_CONV_EVEN_4_2(rt, rg, fnm, fn2) \
VSX_FINLINE(rt) fnm(const rg& a)                 \
{ return fn2(vec_sldw(a, a, 1)); }

VSX_IMPL_CONV_EVEN_4_2(vec_double2, vec_float4, vec_cvf,  vec_cvfo)
VSX_IMPL_CONV_EVEN_4_2(vec_double2, vec_int4,   vec_ctd,  vec_ctdo)
VSX_IMPL_CONV_EVEN_4_2(vec_double2, vec_uint4,  vec_ctd,  vec_ctdo)

VSX_IMPL_CONV_EVEN_4_2(vec_dword2,  vec_float4, vec_ctsl, vec_ctslo)
VSX_IMPL_CONV_EVEN_4_2(vec_udword2, vec_float4, vec_ctul, vec_ctulo)

#define VSX_IMPL_CONV_EVEN_2_4(rt, rg, fnm, fn2) \
VSX_FINLINE(rt) fnm(const rg& a)                 \
{                                                \
    rt v4 = fn2(a);                              \
    return vec_sldw(v4, v4, 3);                  \
}

VSX_IMPL_CONV_EVEN_2_4(vec_float4, vec_double2, vec_cvf, vec_cvfo)
VSX_IMPL_CONV_EVEN_2_4(vec_float4, vec_dword2,  vec_ctf, vec_ctfo)
VSX_IMPL_CONV_EVEN_2_4(vec_float4, vec_udword2, vec_ctf, vec_ctfo)

VSX_IMPL_CONV_EVEN_2_4(vec_int4,   vec_double2, vec_cts, vec_ctso)
VSX_IMPL_CONV_EVEN_2_4(vec_uint4,  vec_double2, vec_ctu, vec_ctuo)

// Only for Eigen!
/*
 * changing behavior of conversion intrinsics for gcc has effect on Eigen
 * so we redfine old behavior again only on gcc, clang
*/
#if !defined(__clang__) || __clang_major__ > 4
    // ignoring second arg since Eigen only truncates toward zero
#   define VSX_IMPL_CONV_2VARIANT(rt, rg, fnm, fn2)     \
    VSX_FINLINE(rt) fnm(const rg& a, int only_truncate) \
    {                                                   \
        assert(only_truncate == 0);                     \
        (void)only_truncate;                            \
        return fn2(a);                                  \
    }
    VSX_IMPL_CONV_2VARIANT(vec_int4,   vec_float4,  vec_cts, vec_cts)
    VSX_IMPL_CONV_2VARIANT(vec_float4, vec_int4,    vec_ctf, vec_ctf)
    // define vec_cts for converting double precision to signed doubleword
    // which isn't combitable with xlc but its okay since Eigen only use it for gcc
    VSX_IMPL_CONV_2VARIANT(vec_dword2, vec_double2, vec_cts, vec_ctsl)
#endif // Eigen

#endif // Common GCC, CLANG compatibility

/*
 * XLC VSX compatibility
**/
#if defined(__IBMCPP__)

// vector population count
#define vec_popcntu vec_popcnt

// overload and redirect with setting second arg to zero
// since we only support conversions without the second arg
#define VSX_IMPL_OVER
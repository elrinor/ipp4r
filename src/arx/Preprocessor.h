#ifndef __ARX_PREPROCESSOR_H__
#define __ARX_PREPROCESSOR_H__

#include "config.h"

#ifdef ARX_USE_BOOST
#  include <boost/config.hpp>
#  define ARX_JOIN BOOST_JOIN
#  define ARX_STRINGIZE BOOST_STRINGIZE

#  include <boost/preprocessor/logical/bool.hpp>
#  define ARX_BOOL BOOST_PP_BOOL

#  include <boost/preprocessor/if.hpp>
#  define ARX_IF BOOST_PP_IF

#  include <boost/preprocessor/array.hpp>
#  define ARX_ARRAY_SIZE BOOST_PP_ARRAY_SIZE
#  define ARX_ARRAY_DATA BOOST_PP_ARRAY_DATA
#  define ARX_ARRAY_ELEM BOOST_PP_ARRAY_ELEM
#  define ARX_ARRAY_REVERSE BOOST_PP_ARRAY_REVERSE

#  include <boost/preprocessor/tuple.hpp>
#  define ARX_TUPLE_ELEM BOOST_PP_TUPLE_ELEM
#  define ARX_TUPLE_REVERSE BOOST_PP_TUPLE_REVERSE
#else // ARX_USE_BOOST


/**
 * Joins the two arguments together, even when one of the arguments is itself a macro
 */
#  define ARX_JOIN(X, Y) ARX_JOIN_I(X, Y)
#  define ARX_JOIN_I(X, Y) ARX_JOIN_II(X, Y)
#  define ARX_JOIN_II(X, Y) X ## Y


/**
 * Converts the parameter X to a string after macro replacement on X has been performed.
 */
#  define ARX_STRINGIZE(X) ARX_STRINGIZE_I(X)
#  define ARX_STRINGIZE_I(X) #X


/**
 * Converts the given INT (i.e. 0 to 256) to BOOL (i.e 0 or 1)
 * 
 * May have problems with mwerks compiler.
 */
#  define ARX_BOOL(x) ARX_BOOL_I(x)
#  define ARX_BOOL_I(x) ARX_BOOL_ ## x
#  define ARX_BOOL_0 0
#  define ARX_BOOL_1 1
#  define ARX_BOOL_2 1
#  define ARX_BOOL_3 1
#  define ARX_BOOL_4 1
#  define ARX_BOOL_5 1
#  define ARX_BOOL_6 1
#  define ARX_BOOL_7 1
#  define ARX_BOOL_8 1
#  define ARX_BOOL_9 1
#  define ARX_BOOL_10 1
#  define ARX_BOOL_11 1
#  define ARX_BOOL_12 1
#  define ARX_BOOL_13 1
#  define ARX_BOOL_14 1
#  define ARX_BOOL_15 1
#  define ARX_BOOL_16 1
#  define ARX_BOOL_17 1
#  define ARX_BOOL_18 1
#  define ARX_BOOL_19 1
#  define ARX_BOOL_20 1
#  define ARX_BOOL_21 1
#  define ARX_BOOL_22 1
#  define ARX_BOOL_23 1
#  define ARX_BOOL_24 1
#  define ARX_BOOL_25 1
#  define ARX_BOOL_26 1
#  define ARX_BOOL_27 1
#  define ARX_BOOL_28 1
#  define ARX_BOOL_29 1
#  define ARX_BOOL_30 1
#  define ARX_BOOL_31 1
#  define ARX_BOOL_32 1
#  define ARX_BOOL_33 1
#  define ARX_BOOL_34 1
#  define ARX_BOOL_35 1
#  define ARX_BOOL_36 1
#  define ARX_BOOL_37 1
#  define ARX_BOOL_38 1
#  define ARX_BOOL_39 1
#  define ARX_BOOL_40 1
#  define ARX_BOOL_41 1
#  define ARX_BOOL_42 1
#  define ARX_BOOL_43 1
#  define ARX_BOOL_44 1
#  define ARX_BOOL_45 1
#  define ARX_BOOL_46 1
#  define ARX_BOOL_47 1
#  define ARX_BOOL_48 1
#  define ARX_BOOL_49 1
#  define ARX_BOOL_50 1
#  define ARX_BOOL_51 1
#  define ARX_BOOL_52 1
#  define ARX_BOOL_53 1
#  define ARX_BOOL_54 1
#  define ARX_BOOL_55 1
#  define ARX_BOOL_56 1
#  define ARX_BOOL_57 1
#  define ARX_BOOL_58 1
#  define ARX_BOOL_59 1
#  define ARX_BOOL_60 1
#  define ARX_BOOL_61 1
#  define ARX_BOOL_62 1
#  define ARX_BOOL_63 1
#  define ARX_BOOL_64 1
#  define ARX_BOOL_65 1
#  define ARX_BOOL_66 1
#  define ARX_BOOL_67 1
#  define ARX_BOOL_68 1
#  define ARX_BOOL_69 1
#  define ARX_BOOL_70 1
#  define ARX_BOOL_71 1
#  define ARX_BOOL_72 1
#  define ARX_BOOL_73 1
#  define ARX_BOOL_74 1
#  define ARX_BOOL_75 1
#  define ARX_BOOL_76 1
#  define ARX_BOOL_77 1
#  define ARX_BOOL_78 1
#  define ARX_BOOL_79 1
#  define ARX_BOOL_80 1
#  define ARX_BOOL_81 1
#  define ARX_BOOL_82 1
#  define ARX_BOOL_83 1
#  define ARX_BOOL_84 1
#  define ARX_BOOL_85 1
#  define ARX_BOOL_86 1
#  define ARX_BOOL_87 1
#  define ARX_BOOL_88 1
#  define ARX_BOOL_89 1
#  define ARX_BOOL_90 1
#  define ARX_BOOL_91 1
#  define ARX_BOOL_92 1
#  define ARX_BOOL_93 1
#  define ARX_BOOL_94 1
#  define ARX_BOOL_95 1
#  define ARX_BOOL_96 1
#  define ARX_BOOL_97 1
#  define ARX_BOOL_98 1
#  define ARX_BOOL_99 1
#  define ARX_BOOL_100 1
#  define ARX_BOOL_101 1
#  define ARX_BOOL_102 1
#  define ARX_BOOL_103 1
#  define ARX_BOOL_104 1
#  define ARX_BOOL_105 1
#  define ARX_BOOL_106 1
#  define ARX_BOOL_107 1
#  define ARX_BOOL_108 1
#  define ARX_BOOL_109 1
#  define ARX_BOOL_110 1
#  define ARX_BOOL_111 1
#  define ARX_BOOL_112 1
#  define ARX_BOOL_113 1
#  define ARX_BOOL_114 1
#  define ARX_BOOL_115 1
#  define ARX_BOOL_116 1
#  define ARX_BOOL_117 1
#  define ARX_BOOL_118 1
#  define ARX_BOOL_119 1
#  define ARX_BOOL_120 1
#  define ARX_BOOL_121 1
#  define ARX_BOOL_122 1
#  define ARX_BOOL_123 1
#  define ARX_BOOL_124 1
#  define ARX_BOOL_125 1
#  define ARX_BOOL_126 1
#  define ARX_BOOL_127 1
#  define ARX_BOOL_128 1
#  define ARX_BOOL_129 1
#  define ARX_BOOL_130 1
#  define ARX_BOOL_131 1
#  define ARX_BOOL_132 1
#  define ARX_BOOL_133 1
#  define ARX_BOOL_134 1
#  define ARX_BOOL_135 1
#  define ARX_BOOL_136 1
#  define ARX_BOOL_137 1
#  define ARX_BOOL_138 1
#  define ARX_BOOL_139 1
#  define ARX_BOOL_140 1
#  define ARX_BOOL_141 1
#  define ARX_BOOL_142 1
#  define ARX_BOOL_143 1
#  define ARX_BOOL_144 1
#  define ARX_BOOL_145 1
#  define ARX_BOOL_146 1
#  define ARX_BOOL_147 1
#  define ARX_BOOL_148 1
#  define ARX_BOOL_149 1
#  define ARX_BOOL_150 1
#  define ARX_BOOL_151 1
#  define ARX_BOOL_152 1
#  define ARX_BOOL_153 1
#  define ARX_BOOL_154 1
#  define ARX_BOOL_155 1
#  define ARX_BOOL_156 1
#  define ARX_BOOL_157 1
#  define ARX_BOOL_158 1
#  define ARX_BOOL_159 1
#  define ARX_BOOL_160 1
#  define ARX_BOOL_161 1
#  define ARX_BOOL_162 1
#  define ARX_BOOL_163 1
#  define ARX_BOOL_164 1
#  define ARX_BOOL_165 1
#  define ARX_BOOL_166 1
#  define ARX_BOOL_167 1
#  define ARX_BOOL_168 1
#  define ARX_BOOL_169 1
#  define ARX_BOOL_170 1
#  define ARX_BOOL_171 1
#  define ARX_BOOL_172 1
#  define ARX_BOOL_173 1
#  define ARX_BOOL_174 1
#  define ARX_BOOL_175 1
#  define ARX_BOOL_176 1
#  define ARX_BOOL_177 1
#  define ARX_BOOL_178 1
#  define ARX_BOOL_179 1
#  define ARX_BOOL_180 1
#  define ARX_BOOL_181 1
#  define ARX_BOOL_182 1
#  define ARX_BOOL_183 1
#  define ARX_BOOL_184 1
#  define ARX_BOOL_185 1
#  define ARX_BOOL_186 1
#  define ARX_BOOL_187 1
#  define ARX_BOOL_188 1
#  define ARX_BOOL_189 1
#  define ARX_BOOL_190 1
#  define ARX_BOOL_191 1
#  define ARX_BOOL_192 1
#  define ARX_BOOL_193 1
#  define ARX_BOOL_194 1
#  define ARX_BOOL_195 1
#  define ARX_BOOL_196 1
#  define ARX_BOOL_197 1
#  define ARX_BOOL_198 1
#  define ARX_BOOL_199 1
#  define ARX_BOOL_200 1
#  define ARX_BOOL_201 1
#  define ARX_BOOL_202 1
#  define ARX_BOOL_203 1
#  define ARX_BOOL_204 1
#  define ARX_BOOL_205 1
#  define ARX_BOOL_206 1
#  define ARX_BOOL_207 1
#  define ARX_BOOL_208 1
#  define ARX_BOOL_209 1
#  define ARX_BOOL_210 1
#  define ARX_BOOL_211 1
#  define ARX_BOOL_212 1
#  define ARX_BOOL_213 1
#  define ARX_BOOL_214 1
#  define ARX_BOOL_215 1
#  define ARX_BOOL_216 1
#  define ARX_BOOL_217 1
#  define ARX_BOOL_218 1
#  define ARX_BOOL_219 1
#  define ARX_BOOL_220 1
#  define ARX_BOOL_221 1
#  define ARX_BOOL_222 1
#  define ARX_BOOL_223 1
#  define ARX_BOOL_224 1
#  define ARX_BOOL_225 1
#  define ARX_BOOL_226 1
#  define ARX_BOOL_227 1
#  define ARX_BOOL_228 1
#  define ARX_BOOL_229 1
#  define ARX_BOOL_230 1
#  define ARX_BOOL_231 1
#  define ARX_BOOL_232 1
#  define ARX_BOOL_233 1
#  define ARX_BOOL_234 1
#  define ARX_BOOL_235 1
#  define ARX_BOOL_236 1
#  define ARX_BOOL_237 1
#  define ARX_BOOL_238 1
#  define ARX_BOOL_239 1
#  define ARX_BOOL_240 1
#  define ARX_BOOL_241 1
#  define ARX_BOOL_242 1
#  define ARX_BOOL_243 1
#  define ARX_BOOL_244 1
#  define ARX_BOOL_245 1
#  define ARX_BOOL_246 1
#  define ARX_BOOL_247 1
#  define ARX_BOOL_248 1
#  define ARX_BOOL_249 1
#  define ARX_BOOL_250 1
#  define ARX_BOOL_251 1
#  define ARX_BOOL_252 1
#  define ARX_BOOL_253 1
#  define ARX_BOOL_254 1
#  define ARX_BOOL_255 1
#  define ARX_BOOL_256 1


/**
 * If cond is true (i.e. != 0) then evaluates to t, else to f.
 *
 * May have problems with edge compiler.
 */
#  define ARX_IF(cond, t, f) ARX_IF_I(ARX_BOOL(cond), t, f)
#  define ARX_IF_I(bit, t, f) ARX_IF_II(bit, t, f)
#  define ARX_IF_II(bit, t, f) ARX_IF_I_ ## bit(t, f)
#  define ARX_IF_I_0(t, f) f
#  define ARX_IF_I_1(t, f) t


/**
 * Returns the size of a given array.
 * 
 * Array must be in the form <tt>(SIZE, (ELEMENTS))</tt>
 * 
 * Relies on ARX_TUPLE_ELEM, may have problems with edge compiler.
 */
#  define ARX_ARRAY_SIZE(array) ARX_TUPLE_ELEM(2, 0, array)


/**
 * Returns the data of a given array as a tuple.
 *
 * Array must be in the form <tt>(SIZE, (ELEMENTS))</tt>
 *
 * May have problems with edge compiler.
 */
#  define ARX_ARRAY_DATA(array) ARX_TUPLE_ELEM(2, 1, array)


/**
 * Returns an element of the given array.
 *
 * @param i index of an element needed
 * @param array array to extract element from
 * 
 * May have problems with edge compiler.
 */
#  define ARX_ARRAY_ELEM(i, array) ARX_TUPLE_ELEM(ARX_ARRAY_SIZE(array), i, ARX_ARRAY_DATA(array))


/**
 * Reverses the given array.
 *
 * May have problems with edge compiler.
 */
#  define ARX_ARRAY_REVERSE(array) (ARX_ARRAY_SIZE(array), ARX_TUPLE_REVERSE(ARX_ARRAY_SIZE(array), ARX_ARRAY_DATA(array)))


/**
 * Reverses the given tuple.
 */
#  define ARX_TUPLE_REVERSE(size, tuple) ARX_TUPLE_REVERSE_I(size, tuple)
#  define ARX_TUPLE_REVERSE_I(s, t) ARX_TUPLE_REVERSE_II(ARX_TUPLE_REVERSE_ ## s t)
#  define ARX_TUPLE_REVERSE_II(res) res


#  define ARX_TUPLE_REVERSE_0() ()
#  define ARX_TUPLE_REVERSE_1(a) (a)
#  define ARX_TUPLE_REVERSE_2(a, b) (b, a)
#  define ARX_TUPLE_REVERSE_3(a, b, c) (c, b, a)
#  define ARX_TUPLE_REVERSE_4(a, b, c, d) (d, c, b, a)
#  define ARX_TUPLE_REVERSE_5(a, b, c, d, e) (e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_6(a, b, c, d, e, f) (f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_7(a, b, c, d, e, f, g) (g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_8(a, b, c, d, e, f, g, h) (h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_9(a, b, c, d, e, f, g, h, i) (i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_10(a, b, c, d, e, f, g, h, i, j) (j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_11(a, b, c, d, e, f, g, h, i, j, k) (k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_12(a, b, c, d, e, f, g, h, i, j, k, l) (l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_13(a, b, c, d, e, f, g, h, i, j, k, l, m) (m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) (n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) (o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) (p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) (q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) (r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) (s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) (t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) (u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) (v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) (w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) (x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)
#  define ARX_TUPLE_REVERSE_25(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) (y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a)


/**
 * Returns an element of the given tuple.
 *
 * @param size size of a given tuple
 * @param index index of an element needed
 * @param tuple tuple to extract element from
 * 
 * May have problems with edge, mwerks, and older versions of msvc compilers.
 */
#  define ARX_TUPLE_ELEM(size, index, tuple) ARX_TUPLE_ELEM_I(size, index, tuple)
#  define ARX_TUPLE_ELEM_I(s, i, t) ARX_TUPLE_ELEM_II(ARX_TUPLE_ELEM_ ## s ## _ ## i t)
#  define ARX_TUPLE_ELEM_II(res) res

#  define ARX_TUPLE_ELEM_1_0(a) a
#
#  define ARX_TUPLE_ELEM_2_0(a, b) a
#  define ARX_TUPLE_ELEM_2_1(a, b) b
#
#  define ARX_TUPLE_ELEM_3_0(a, b, c) a
#  define ARX_TUPLE_ELEM_3_1(a, b, c) b
#  define ARX_TUPLE_ELEM_3_2(a, b, c) c
#
#  define ARX_TUPLE_ELEM_4_0(a, b, c, d) a
#  define ARX_TUPLE_ELEM_4_1(a, b, c, d) b
#  define ARX_TUPLE_ELEM_4_2(a, b, c, d) c
#  define ARX_TUPLE_ELEM_4_3(a, b, c, d) d
#
#  define ARX_TUPLE_ELEM_5_0(a, b, c, d, e) a
#  define ARX_TUPLE_ELEM_5_1(a, b, c, d, e) b
#  define ARX_TUPLE_ELEM_5_2(a, b, c, d, e) c
#  define ARX_TUPLE_ELEM_5_3(a, b, c, d, e) d
#  define ARX_TUPLE_ELEM_5_4(a, b, c, d, e) e
#
#  define ARX_TUPLE_ELEM_6_0(a, b, c, d, e, f) a
#  define ARX_TUPLE_ELEM_6_1(a, b, c, d, e, f) b
#  define ARX_TUPLE_ELEM_6_2(a, b, c, d, e, f) c
#  define ARX_TUPLE_ELEM_6_3(a, b, c, d, e, f) d
#  define ARX_TUPLE_ELEM_6_4(a, b, c, d, e, f) e
#  define ARX_TUPLE_ELEM_6_5(a, b, c, d, e, f) f
#
#  define ARX_TUPLE_ELEM_7_0(a, b, c, d, e, f, g) a
#  define ARX_TUPLE_ELEM_7_1(a, b, c, d, e, f, g) b
#  define ARX_TUPLE_ELEM_7_2(a, b, c, d, e, f, g) c
#  define ARX_TUPLE_ELEM_7_3(a, b, c, d, e, f, g) d
#  define ARX_TUPLE_ELEM_7_4(a, b, c, d, e, f, g) e
#  define ARX_TUPLE_ELEM_7_5(a, b, c, d, e, f, g) f
#  define ARX_TUPLE_ELEM_7_6(a, b, c, d, e, f, g) g
#
#  define ARX_TUPLE_ELEM_8_0(a, b, c, d, e, f, g, h) a
#  define ARX_TUPLE_ELEM_8_1(a, b, c, d, e, f, g, h) b
#  define ARX_TUPLE_ELEM_8_2(a, b, c, d, e, f, g, h) c
#  define ARX_TUPLE_ELEM_8_3(a, b, c, d, e, f, g, h) d
#  define ARX_TUPLE_ELEM_8_4(a, b, c, d, e, f, g, h) e
#  define ARX_TUPLE_ELEM_8_5(a, b, c, d, e, f, g, h) f
#  define ARX_TUPLE_ELEM_8_6(a, b, c, d, e, f, g, h) g
#  define ARX_TUPLE_ELEM_8_7(a, b, c, d, e, f, g, h) h
#
#  define ARX_TUPLE_ELEM_9_0(a, b, c, d, e, f, g, h, i) a
#  define ARX_TUPLE_ELEM_9_1(a, b, c, d, e, f, g, h, i) b
#  define ARX_TUPLE_ELEM_9_2(a, b, c, d, e, f, g, h, i) c
#  define ARX_TUPLE_ELEM_9_3(a, b, c, d, e, f, g, h, i) d
#  define ARX_TUPLE_ELEM_9_4(a, b, c, d, e, f, g, h, i) e
#  define ARX_TUPLE_ELEM_9_5(a, b, c, d, e, f, g, h, i) f
#  define ARX_TUPLE_ELEM_9_6(a, b, c, d, e, f, g, h, i) g
#  define ARX_TUPLE_ELEM_9_7(a, b, c, d, e, f, g, h, i) h
#  define ARX_TUPLE_ELEM_9_8(a, b, c, d, e, f, g, h, i) i
#
#  define ARX_TUPLE_ELEM_10_0(a, b, c, d, e, f, g, h, i, j) a
#  define ARX_TUPLE_ELEM_10_1(a, b, c, d, e, f, g, h, i, j) b
#  define ARX_TUPLE_ELEM_10_2(a, b, c, d, e, f, g, h, i, j) c
#  define ARX_TUPLE_ELEM_10_3(a, b, c, d, e, f, g, h, i, j) d
#  define ARX_TUPLE_ELEM_10_4(a, b, c, d, e, f, g, h, i, j) e
#  define ARX_TUPLE_ELEM_10_5(a, b, c, d, e, f, g, h, i, j) f
#  define ARX_TUPLE_ELEM_10_6(a, b, c, d, e, f, g, h, i, j) g
#  define ARX_TUPLE_ELEM_10_7(a, b, c, d, e, f, g, h, i, j) h
#  define ARX_TUPLE_ELEM_10_8(a, b, c, d, e, f, g, h, i, j) i
#  define ARX_TUPLE_ELEM_10_9(a, b, c, d, e, f, g, h, i, j) j
#
#  define ARX_TUPLE_ELEM_11_0(a, b, c, d, e, f, g, h, i, j, k) a
#  define ARX_TUPLE_ELEM_11_1(a, b, c, d, e, f, g, h, i, j, k) b
#  define ARX_TUPLE_ELEM_11_2(a, b, c, d, e, f, g, h, i, j, k) c
#  define ARX_TUPLE_ELEM_11_3(a, b, c, d, e, f, g, h, i, j, k) d
#  define ARX_TUPLE_ELEM_11_4(a, b, c, d, e, f, g, h, i, j, k) e
#  define ARX_TUPLE_ELEM_11_5(a, b, c, d, e, f, g, h, i, j, k) f
#  define ARX_TUPLE_ELEM_11_6(a, b, c, d, e, f, g, h, i, j, k) g
#  define ARX_TUPLE_ELEM_11_7(a, b, c, d, e, f, g, h, i, j, k) h
#  define ARX_TUPLE_ELEM_11_8(a, b, c, d, e, f, g, h, i, j, k) i
#  define ARX_TUPLE_ELEM_11_9(a, b, c, d, e, f, g, h, i, j, k) j
#  define ARX_TUPLE_ELEM_11_10(a, b, c, d, e, f, g, h, i, j, k) k
#
#  define ARX_TUPLE_ELEM_12_0(a, b, c, d, e, f, g, h, i, j, k, l) a
#  define ARX_TUPLE_ELEM_12_1(a, b, c, d, e, f, g, h, i, j, k, l) b
#  define ARX_TUPLE_ELEM_12_2(a, b, c, d, e, f, g, h, i, j, k, l) c
#  define ARX_TUPLE_ELEM_12_3(a, b, c, d, e, f, g, h, i, j, k, l) d
#  define ARX_TUPLE_ELEM_12_4(a, b, c, d, e, f, g, h, i, j, k, l) e
#  define ARX_TUPLE_ELEM_12_5(a, b, c, d, e, f, g, h, i, j, k, l) f
#  define ARX_TUPLE_ELEM_12_6(a, b, c, d, e, f, g, h, i, j, k, l) g
#  define ARX_TUPLE_ELEM_12_7(a, b, c, d, e, f, g, h, i, j, k, l) h
#  define ARX_TUPLE_ELEM_12_8(a, b, c, d, e, f, g, h, i, j, k, l) i
#  define ARX_TUPLE_ELEM_12_9(a, b, c, d, e, f, g, h, i, j, k, l) j
#  define ARX_TUPLE_ELEM_12_10(a, b, c, d, e, f, g, h, i, j, k, l) k
#  define ARX_TUPLE_ELEM_12_11(a, b, c, d, e, f, g, h, i, j, k, l) l
#
#  define ARX_TUPLE_ELEM_13_0(a, b, c, d, e, f, g, h, i, j, k, l, m) a
#  define ARX_TUPLE_ELEM_13_1(a, b, c, d, e, f, g, h, i, j, k, l, m) b
#  define ARX_TUPLE_ELEM_13_2(a, b, c, d, e, f, g, h, i, j, k, l, m) c
#  define ARX_TUPLE_ELEM_13_3(a, b, c, d, e, f, g, h, i, j, k, l, m) d
#  define ARX_TUPLE_ELEM_13_4(a, b, c, d, e, f, g, h, i, j, k, l, m) e
#  define ARX_TUPLE_ELEM_13_5(a, b, c, d, e, f, g, h, i, j, k, l, m) f
#  define ARX_TUPLE_ELEM_13_6(a, b, c, d, e, f, g, h, i, j, k, l, m) g
#  define ARX_TUPLE_ELEM_13_7(a, b, c, d, e, f, g, h, i, j, k, l, m) h
#  define ARX_TUPLE_ELEM_13_8(a, b, c, d, e, f, g, h, i, j, k, l, m) i
#  define ARX_TUPLE_ELEM_13_9(a, b, c, d, e, f, g, h, i, j, k, l, m) j
#  define ARX_TUPLE_ELEM_13_10(a, b, c, d, e, f, g, h, i, j, k, l, m) k
#  define ARX_TUPLE_ELEM_13_11(a, b, c, d, e, f, g, h, i, j, k, l, m) l
#  define ARX_TUPLE_ELEM_13_12(a, b, c, d, e, f, g, h, i, j, k, l, m) m
#
#  define ARX_TUPLE_ELEM_14_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n) a
#  define ARX_TUPLE_ELEM_14_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n) b
#  define ARX_TUPLE_ELEM_14_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n) c
#  define ARX_TUPLE_ELEM_14_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n) d
#  define ARX_TUPLE_ELEM_14_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n) e
#  define ARX_TUPLE_ELEM_14_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n) f
#  define ARX_TUPLE_ELEM_14_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n) g
#  define ARX_TUPLE_ELEM_14_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n) h
#  define ARX_TUPLE_ELEM_14_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n) i
#  define ARX_TUPLE_ELEM_14_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n) j
#  define ARX_TUPLE_ELEM_14_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n) k
#  define ARX_TUPLE_ELEM_14_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n) l
#  define ARX_TUPLE_ELEM_14_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n) m
#  define ARX_TUPLE_ELEM_14_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n) n
#
#  define ARX_TUPLE_ELEM_15_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) a
#  define ARX_TUPLE_ELEM_15_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) b
#  define ARX_TUPLE_ELEM_15_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) c
#  define ARX_TUPLE_ELEM_15_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) d
#  define ARX_TUPLE_ELEM_15_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) e
#  define ARX_TUPLE_ELEM_15_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) f
#  define ARX_TUPLE_ELEM_15_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) g
#  define ARX_TUPLE_ELEM_15_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) h
#  define ARX_TUPLE_ELEM_15_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) i
#  define ARX_TUPLE_ELEM_15_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) j
#  define ARX_TUPLE_ELEM_15_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) k
#  define ARX_TUPLE_ELEM_15_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) l
#  define ARX_TUPLE_ELEM_15_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) m
#  define ARX_TUPLE_ELEM_15_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) n
#  define ARX_TUPLE_ELEM_15_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) o
#
#  define ARX_TUPLE_ELEM_16_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) a
#  define ARX_TUPLE_ELEM_16_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) b
#  define ARX_TUPLE_ELEM_16_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) c
#  define ARX_TUPLE_ELEM_16_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) d
#  define ARX_TUPLE_ELEM_16_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) e
#  define ARX_TUPLE_ELEM_16_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) f
#  define ARX_TUPLE_ELEM_16_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) g
#  define ARX_TUPLE_ELEM_16_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) h
#  define ARX_TUPLE_ELEM_16_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) i
#  define ARX_TUPLE_ELEM_16_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) j
#  define ARX_TUPLE_ELEM_16_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) k
#  define ARX_TUPLE_ELEM_16_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) l
#  define ARX_TUPLE_ELEM_16_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) m
#  define ARX_TUPLE_ELEM_16_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) n
#  define ARX_TUPLE_ELEM_16_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) o
#  define ARX_TUPLE_ELEM_16_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) p
#
#  define ARX_TUPLE_ELEM_17_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) a
#  define ARX_TUPLE_ELEM_17_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) b
#  define ARX_TUPLE_ELEM_17_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) c
#  define ARX_TUPLE_ELEM_17_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) d
#  define ARX_TUPLE_ELEM_17_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) e
#  define ARX_TUPLE_ELEM_17_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) f
#  define ARX_TUPLE_ELEM_17_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) g
#  define ARX_TUPLE_ELEM_17_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) h
#  define ARX_TUPLE_ELEM_17_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) i
#  define ARX_TUPLE_ELEM_17_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) j
#  define ARX_TUPLE_ELEM_17_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) k
#  define ARX_TUPLE_ELEM_17_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) l
#  define ARX_TUPLE_ELEM_17_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) m
#  define ARX_TUPLE_ELEM_17_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) n
#  define ARX_TUPLE_ELEM_17_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) o
#  define ARX_TUPLE_ELEM_17_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) p
#  define ARX_TUPLE_ELEM_17_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) q
#
#  define ARX_TUPLE_ELEM_18_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) a
#  define ARX_TUPLE_ELEM_18_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) b
#  define ARX_TUPLE_ELEM_18_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) c
#  define ARX_TUPLE_ELEM_18_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) d
#  define ARX_TUPLE_ELEM_18_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) e
#  define ARX_TUPLE_ELEM_18_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) f
#  define ARX_TUPLE_ELEM_18_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) g
#  define ARX_TUPLE_ELEM_18_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) h
#  define ARX_TUPLE_ELEM_18_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) i
#  define ARX_TUPLE_ELEM_18_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) j
#  define ARX_TUPLE_ELEM_18_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) k
#  define ARX_TUPLE_ELEM_18_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) l
#  define ARX_TUPLE_ELEM_18_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) m
#  define ARX_TUPLE_ELEM_18_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) n
#  define ARX_TUPLE_ELEM_18_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) o
#  define ARX_TUPLE_ELEM_18_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) p
#  define ARX_TUPLE_ELEM_18_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) q
#  define ARX_TUPLE_ELEM_18_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) r
#
#  define ARX_TUPLE_ELEM_19_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) a
#  define ARX_TUPLE_ELEM_19_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) b
#  define ARX_TUPLE_ELEM_19_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) c
#  define ARX_TUPLE_ELEM_19_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) d
#  define ARX_TUPLE_ELEM_19_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) e
#  define ARX_TUPLE_ELEM_19_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) f
#  define ARX_TUPLE_ELEM_19_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) g
#  define ARX_TUPLE_ELEM_19_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) h
#  define ARX_TUPLE_ELEM_19_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) i
#  define ARX_TUPLE_ELEM_19_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) j
#  define ARX_TUPLE_ELEM_19_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) k
#  define ARX_TUPLE_ELEM_19_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) l
#  define ARX_TUPLE_ELEM_19_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) m
#  define ARX_TUPLE_ELEM_19_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) n
#  define ARX_TUPLE_ELEM_19_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) o
#  define ARX_TUPLE_ELEM_19_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) p
#  define ARX_TUPLE_ELEM_19_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) q
#  define ARX_TUPLE_ELEM_19_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) r
#  define ARX_TUPLE_ELEM_19_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) s
#
#  define ARX_TUPLE_ELEM_20_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) a
#  define ARX_TUPLE_ELEM_20_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) b
#  define ARX_TUPLE_ELEM_20_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) c
#  define ARX_TUPLE_ELEM_20_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) d
#  define ARX_TUPLE_ELEM_20_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) e
#  define ARX_TUPLE_ELEM_20_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) f
#  define ARX_TUPLE_ELEM_20_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) g
#  define ARX_TUPLE_ELEM_20_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) h
#  define ARX_TUPLE_ELEM_20_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) i
#  define ARX_TUPLE_ELEM_20_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) j
#  define ARX_TUPLE_ELEM_20_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) k
#  define ARX_TUPLE_ELEM_20_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) l
#  define ARX_TUPLE_ELEM_20_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) m
#  define ARX_TUPLE_ELEM_20_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) n
#  define ARX_TUPLE_ELEM_20_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) o
#  define ARX_TUPLE_ELEM_20_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) p
#  define ARX_TUPLE_ELEM_20_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) q
#  define ARX_TUPLE_ELEM_20_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) r
#  define ARX_TUPLE_ELEM_20_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) s
#  define ARX_TUPLE_ELEM_20_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) t
#
#  define ARX_TUPLE_ELEM_21_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) a
#  define ARX_TUPLE_ELEM_21_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) b
#  define ARX_TUPLE_ELEM_21_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) c
#  define ARX_TUPLE_ELEM_21_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) d
#  define ARX_TUPLE_ELEM_21_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) e
#  define ARX_TUPLE_ELEM_21_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) f
#  define ARX_TUPLE_ELEM_21_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) g
#  define ARX_TUPLE_ELEM_21_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) h
#  define ARX_TUPLE_ELEM_21_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) i
#  define ARX_TUPLE_ELEM_21_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) j
#  define ARX_TUPLE_ELEM_21_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) k
#  define ARX_TUPLE_ELEM_21_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) l
#  define ARX_TUPLE_ELEM_21_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) m
#  define ARX_TUPLE_ELEM_21_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) n
#  define ARX_TUPLE_ELEM_21_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) o
#  define ARX_TUPLE_ELEM_21_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) p
#  define ARX_TUPLE_ELEM_21_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) q
#  define ARX_TUPLE_ELEM_21_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) r
#  define ARX_TUPLE_ELEM_21_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) s
#  define ARX_TUPLE_ELEM_21_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) t
#  define ARX_TUPLE_ELEM_21_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) u
#
#  define ARX_TUPLE_ELEM_22_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) a
#  define ARX_TUPLE_ELEM_22_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) b
#  define ARX_TUPLE_ELEM_22_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) c
#  define ARX_TUPLE_ELEM_22_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) d
#  define ARX_TUPLE_ELEM_22_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) e
#  define ARX_TUPLE_ELEM_22_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) f
#  define ARX_TUPLE_ELEM_22_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) g
#  define ARX_TUPLE_ELEM_22_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) h
#  define ARX_TUPLE_ELEM_22_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) i
#  define ARX_TUPLE_ELEM_22_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) j
#  define ARX_TUPLE_ELEM_22_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) k
#  define ARX_TUPLE_ELEM_22_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) l
#  define ARX_TUPLE_ELEM_22_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) m
#  define ARX_TUPLE_ELEM_22_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) n
#  define ARX_TUPLE_ELEM_22_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) o
#  define ARX_TUPLE_ELEM_22_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) p
#  define ARX_TUPLE_ELEM_22_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) q
#  define ARX_TUPLE_ELEM_22_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) r
#  define ARX_TUPLE_ELEM_22_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) s
#  define ARX_TUPLE_ELEM_22_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) t
#  define ARX_TUPLE_ELEM_22_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) u
#  define ARX_TUPLE_ELEM_22_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) v
#
#  define ARX_TUPLE_ELEM_23_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) a
#  define ARX_TUPLE_ELEM_23_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) b
#  define ARX_TUPLE_ELEM_23_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) c
#  define ARX_TUPLE_ELEM_23_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) d
#  define ARX_TUPLE_ELEM_23_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) e
#  define ARX_TUPLE_ELEM_23_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) f
#  define ARX_TUPLE_ELEM_23_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) g
#  define ARX_TUPLE_ELEM_23_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) h
#  define ARX_TUPLE_ELEM_23_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) i
#  define ARX_TUPLE_ELEM_23_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) j
#  define ARX_TUPLE_ELEM_23_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) k
#  define ARX_TUPLE_ELEM_23_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) l
#  define ARX_TUPLE_ELEM_23_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) m
#  define ARX_TUPLE_ELEM_23_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) n
#  define ARX_TUPLE_ELEM_23_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) o
#  define ARX_TUPLE_ELEM_23_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) p
#  define ARX_TUPLE_ELEM_23_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) q
#  define ARX_TUPLE_ELEM_23_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) r
#  define ARX_TUPLE_ELEM_23_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) s
#  define ARX_TUPLE_ELEM_23_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) t
#  define ARX_TUPLE_ELEM_23_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) u
#  define ARX_TUPLE_ELEM_23_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) v
#  define ARX_TUPLE_ELEM_23_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) w
#
#  define ARX_TUPLE_ELEM_24_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) a
#  define ARX_TUPLE_ELEM_24_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) b
#  define ARX_TUPLE_ELEM_24_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) c
#  define ARX_TUPLE_ELEM_24_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) d
#  define ARX_TUPLE_ELEM_24_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) e
#  define ARX_TUPLE_ELEM_24_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) f
#  define ARX_TUPLE_ELEM_24_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) g
#  define ARX_TUPLE_ELEM_24_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) h
#  define ARX_TUPLE_ELEM_24_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) i
#  define ARX_TUPLE_ELEM_24_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) j
#  define ARX_TUPLE_ELEM_24_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) k
#  define ARX_TUPLE_ELEM_24_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) l
#  define ARX_TUPLE_ELEM_24_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) m
#  define ARX_TUPLE_ELEM_24_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) n
#  define ARX_TUPLE_ELEM_24_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) o
#  define ARX_TUPLE_ELEM_24_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) p
#  define ARX_TUPLE_ELEM_24_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) q
#  define ARX_TUPLE_ELEM_24_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) r
#  define ARX_TUPLE_ELEM_24_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) s
#  define ARX_TUPLE_ELEM_24_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) t
#  define ARX_TUPLE_ELEM_24_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) u
#  define ARX_TUPLE_ELEM_24_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) v
#  define ARX_TUPLE_ELEM_24_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) w
#  define ARX_TUPLE_ELEM_24_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) x
#
#  define ARX_TUPLE_ELEM_25_0(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) a
#  define ARX_TUPLE_ELEM_25_1(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) b
#  define ARX_TUPLE_ELEM_25_2(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) c
#  define ARX_TUPLE_ELEM_25_3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) d
#  define ARX_TUPLE_ELEM_25_4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) e
#  define ARX_TUPLE_ELEM_25_5(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) f
#  define ARX_TUPLE_ELEM_25_6(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) g
#  define ARX_TUPLE_ELEM_25_7(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) h
#  define ARX_TUPLE_ELEM_25_8(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) i
#  define ARX_TUPLE_ELEM_25_9(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) j
#  define ARX_TUPLE_ELEM_25_10(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) k
#  define ARX_TUPLE_ELEM_25_11(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) l
#  define ARX_TUPLE_ELEM_25_12(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) m
#  define ARX_TUPLE_ELEM_25_13(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) n
#  define ARX_TUPLE_ELEM_25_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) o
#  define ARX_TUPLE_ELEM_25_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) p
#  define ARX_TUPLE_ELEM_25_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) q
#  define ARX_TUPLE_ELEM_25_17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) r
#  define ARX_TUPLE_ELEM_25_18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) s
#  define ARX_TUPLE_ELEM_25_19(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) t
#  define ARX_TUPLE_ELEM_25_20(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) u
#  define ARX_TUPLE_ELEM_25_21(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) v
#  define ARX_TUPLE_ELEM_25_22(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) w
#  define ARX_TUPLE_ELEM_25_23(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) x
#  define ARX_TUPLE_ELEM_25_24(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y) y

#endif // ARX_USE_BOOST


#define ARX_COMMA ,
#define ARX_LPAREN (
#define ARX_RPAREN )

#define ARX_EMPTY

#define ARX_EMPTY_IT(ARG) ARX_EMPTY
#define ARX_IDENTITY(ARG) ARG
#define ARX_PAREN_IT(ARG) (ARG)
#define ARX_BRACK_IT(ARG) [ARG]
#define ARX_ABRACK_IT(ARG) <ARG>


/**
 * Array foreach macro
 */
#define ARX_ARRAY_FOREACH(ARRAY, M, ARG) ARX_ARRAY_FOREACH_OO(ARX_ARRAY_SIZE(ARRAY), ARX_ARRAY_REVERSE(ARRAY), M, ARG)
#define ARX_ARRAY_FOREACH_OO(SIZE, ARRAY, M, ARG) ARX_ARRAY_FOREACH_I(SIZE, ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_I(SIZE, ARRAY, M, ARG) ARX_ARRAY_FOREACH_II(ARX_ARRAY_FOREACH_ ## SIZE (ARRAY, M, ARG))
#define ARX_ARRAY_FOREACH_II(res) res

#define ARX_ARRAY_FOREACH_1(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(0, ARRAY), ARG)
#define ARX_ARRAY_FOREACH_2(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(1, ARRAY), ARG) ARX_ARRAY_FOREACH_1(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_3(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(2, ARRAY), ARG) ARX_ARRAY_FOREACH_2(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_4(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(3, ARRAY), ARG) ARX_ARRAY_FOREACH_3(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_5(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(4, ARRAY), ARG) ARX_ARRAY_FOREACH_4(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_6(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(5, ARRAY), ARG) ARX_ARRAY_FOREACH_5(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_7(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(6, ARRAY), ARG) ARX_ARRAY_FOREACH_6(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_8(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(7, ARRAY), ARG) ARX_ARRAY_FOREACH_7(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_9(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(8, ARRAY), ARG) ARX_ARRAY_FOREACH_8(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_10(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(9, ARRAY), ARG) ARX_ARRAY_FOREACH_9(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_11(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(10, ARRAY), ARG) ARX_ARRAY_FOREACH_10(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_12(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(11, ARRAY), ARG) ARX_ARRAY_FOREACH_11(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_13(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(12, ARRAY), ARG) ARX_ARRAY_FOREACH_12(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_14(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(13, ARRAY), ARG) ARX_ARRAY_FOREACH_13(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_15(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(14, ARRAY), ARG) ARX_ARRAY_FOREACH_14(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_16(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(15, ARRAY), ARG) ARX_ARRAY_FOREACH_15(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_17(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(16, ARRAY), ARG) ARX_ARRAY_FOREACH_16(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_18(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(17, ARRAY), ARG) ARX_ARRAY_FOREACH_17(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_19(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(18, ARRAY), ARG) ARX_ARRAY_FOREACH_18(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_20(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(19, ARRAY), ARG) ARX_ARRAY_FOREACH_19(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_21(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(20, ARRAY), ARG) ARX_ARRAY_FOREACH_20(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_22(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(21, ARRAY), ARG) ARX_ARRAY_FOREACH_21(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_23(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(22, ARRAY), ARG) ARX_ARRAY_FOREACH_22(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_24(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(23, ARRAY), ARG) ARX_ARRAY_FOREACH_23(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH_25(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(24, ARRAY), ARG) ARX_ARRAY_FOREACH_24(ARRAY, M, ARG)


/**
 * Array foreach macro, for nested cycles
 */
#define ARX_ARRAY_FOREACH2(ARRAY, M, ARG) ARX_ARRAY_FOREACH2_OO(ARX_ARRAY_SIZE(ARRAY), ARX_ARRAY_REVERSE(ARRAY), M, ARG)
#define ARX_ARRAY_FOREACH2_OO(SIZE, ARRAY, M, ARG) ARX_ARRAY_FOREACH2_I(SIZE, ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_I(SIZE, ARRAY, M, ARG) ARX_ARRAY_FOREACH2_II(ARX_ARRAY_FOREACH2_ ## SIZE (ARRAY, M, ARG))
#define ARX_ARRAY_FOREACH2_II(res) res

#define ARX_ARRAY_FOREACH2_1(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(0, ARRAY), ARG)
#define ARX_ARRAY_FOREACH2_2(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(1, ARRAY), ARG) ARX_ARRAY_FOREACH2_1(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_3(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(2, ARRAY), ARG) ARX_ARRAY_FOREACH2_2(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_4(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(3, ARRAY), ARG) ARX_ARRAY_FOREACH2_3(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_5(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(4, ARRAY), ARG) ARX_ARRAY_FOREACH2_4(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_6(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(5, ARRAY), ARG) ARX_ARRAY_FOREACH2_5(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_7(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(6, ARRAY), ARG) ARX_ARRAY_FOREACH2_6(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_8(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(7, ARRAY), ARG) ARX_ARRAY_FOREACH2_7(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_9(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(8, ARRAY), ARG) ARX_ARRAY_FOREACH2_8(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_10(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(9, ARRAY), ARG) ARX_ARRAY_FOREACH2_9(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_11(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(10, ARRAY), ARG) ARX_ARRAY_FOREACH2_10(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_12(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(11, ARRAY), ARG) ARX_ARRAY_FOREACH2_11(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_13(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(12, ARRAY), ARG) ARX_ARRAY_FOREACH2_12(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_14(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(13, ARRAY), ARG) ARX_ARRAY_FOREACH2_13(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_15(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(14, ARRAY), ARG) ARX_ARRAY_FOREACH2_14(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_16(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(15, ARRAY), ARG) ARX_ARRAY_FOREACH2_15(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_17(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(16, ARRAY), ARG) ARX_ARRAY_FOREACH2_16(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_18(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(17, ARRAY), ARG) ARX_ARRAY_FOREACH2_17(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_19(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(18, ARRAY), ARG) ARX_ARRAY_FOREACH2_18(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_20(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(19, ARRAY), ARG) ARX_ARRAY_FOREACH2_19(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_21(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(20, ARRAY), ARG) ARX_ARRAY_FOREACH2_20(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_22(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(21, ARRAY), ARG) ARX_ARRAY_FOREACH2_21(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_23(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(22, ARRAY), ARG) ARX_ARRAY_FOREACH2_22(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_24(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(23, ARRAY), ARG) ARX_ARRAY_FOREACH2_23(ARRAY, M, ARG)
#define ARX_ARRAY_FOREACH2_25(ARRAY, M, ARG) M(ARX_ARRAY_ELEM(24, ARRAY), ARG) ARX_ARRAY_FOREACH2_24(ARRAY, M, ARG)


#endif
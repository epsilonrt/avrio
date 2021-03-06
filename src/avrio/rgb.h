/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file rgb.h
 * @brief RGB
 *
 *
 * Revision History ------------------------------------------------------------
 *    20130129 - Initial version by epsilonRT
 */
#ifndef _AVRIO_RGB_H_
#define _AVRIO_RGB_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
#if ! defined(__DOXYGEN__)

/* constants ================================================================ */
/**
 * Couleurs standard
 */
#define RGB_ALICEBLUE 0xF0F8FF /* 240,248,255 */
#define RGB_ANTIQUEWHITE 0xFAEBD7 /* 250,235,215 */
#define RGB_AQUA 0x00FFFF /* 0,255,255 */
#define RGB_AQUAMARINE 0x7FFFD4 /* 127,255,212 */
#define RGB_AZURE 0xF0FFFF /* 240,255,255 */
#define RGB_BEIGE 0xF5F5DC /* 245,245,220 */
#define RGB_BISQUE 0xFFE4C4 /* 255,228,196 */
#define RGB_BLACK 0x000000 /* 0,0,0 */
#define RGB_BLANCHEDALMOND 0xFFEBCD /* 255,235,205 */
#define RGB_BLUE 0x0000FF /* 0,0,255 */
#define RGB_BLUEVIOLET 0x8A2BE2 /* 138,43,226 */
#define RGB_BROWN 0xA52A2A /* 165,42,42 */
#define RGB_BURLYWOOD 0xDEB887 /* 222,184,135 */
#define RGB_CADETBLUE 0x5F9EA0 /* 95,158,160 */
#define RGB_CHARTREUSE 0x7FFF00 /* 127,255,0 */
#define RGB_CHOCOLATE 0xD2691E /* 210,105,30 */
#define RGB_CORAL 0xFF7F50 /* 255,127,80 */
#define RGB_CORNFLOWERBLUE 0x6495ED /* 100,149,237 */
#define RGB_CORNSILK 0xFFF8DC /* 255,248,220 */
#define RGB_CRIMSON 0xDC143C /* 220,20,60 */
#define RGB_CYAN 0x00FFFF /* 0,255,255 */
#define RGB_DARKBLUE 0x00008B /* 0,0,139 */
#define RGB_DARKCYAN 0x008B8B /* 0,139,139 */
#define RGB_DARKGOLDENROD 0xB8860B /* 184,134,11 */
#define RGB_DARKGRAY 0xA9A9A9 /* 169,169,169 */
#define RGB_DARKGREEN 0x006400 /* 0,100,0 */
#define RGB_DARKGREY 0xA9A9A9 /* 169,169,169 */
#define RGB_DARKKHAKI 0xBDB76B /* 189,183,107 */
#define RGB_DARKMAGENTA 0x8B008B /* 139,0,139 */
#define RGB_DARKOLIVEGREEN 0x556B2F /* 85,107,47 */
#define RGB_DARKORANGE 0xFF8C00 /* 255,140,0 */
#define RGB_DARKORCHID 0x9932CC /* 153,50,204 */
#define RGB_DARKRED 0x8B0000 /* 139,0,0 */
#define RGB_DARKSALMON 0xE9967A /* 233,150,122 */
#define RGB_DARKSEAGREEN 0x8FBC8F /* 143,188,143 */
#define RGB_DARKSLATEBLUE 0x483D8B /* 72,61,139 */
#define RGB_DARKSLATEGRAY 0x2F4F4F /* 47,79,79 */
#define RGB_DARKSLATEGREY 0x2F4F4F /* 47,79,79 */
#define RGB_DARKTURQUOISE 0x00CED1 /* 0,206,209 */
#define RGB_DARKVIOLET 0x9400D3 /* 148,0,211 */
#define RGB_DEEPPINK 0xFF1493 /* 255,20,147 */
#define RGB_DEEPSKYBLUE 0x00BFFF /* 0,191,255 */
#define RGB_DIMGRAY 0x696969 /* 105,105,105 */
#define RGB_DIMGREY 0x696969 /* 105,105,105 */
#define RGB_DODGERBLUE 0x1E90FF /* 30,144,255 */
#define RGB_FIREBRICK 0xB22222 /* 178,34,34 */
#define RGB_FLORALWHITE 0xFFFAF0 /* 255,250,240 */
#define RGB_FORESTGREEN 0x228B22 /* 34,139,34 */
#define RGB_FUCHSIA 0xFF00FF /* 255,0,255 */
#define RGB_GAINSBORO 0xDCDCDC /* 220,220,220 */
#define RGB_GHOSTWHITE 0xF8F8FF /* 248,248,255 */
#define RGB_GOLD 0xFFD700 /* 255,215,0 */
#define RGB_GOLDENROD 0xDAA520 /* 218,165,32 */
#define RGB_GRAY 0x808080 /* 128,128,128 */
#define RGB_GREEN 0x008000 /* 0,128,0 */
#define RGB_GREENYELLOW 0xADFF2F /* 173,255,47 */
#define RGB_GREY 0x808080 /* 128,128,128 */
#define RGB_HONEYDEW 0xF0FFF0 /* 240,255,240 */
#define RGB_HOTPINK 0xFF69B4 /* 255,105,180 */
#define RGB_INDIANRED 0xCD5C5C /* 205,92,92 */
#define RGB_INDIGO 0x4B0082 /* 75,0,130 */
#define RGB_IVORY 0xFFFFF0 /* 255,255,240 */
#define RGB_KHAKI 0xF0E68C /* 240,230,140 */
#define RGB_LAVENDER 0xE6E6FA /* 230,230,250 */
#define RGB_LAVENDERBLUSH 0xFFF0F5 /* 255,240,245 */
#define RGB_LAWNGREEN 0x7CFC00 /* 124,252,0 */
#define RGB_LEMONCHIFFON 0xFFFACD /* 255,250,205 */
#define RGB_LIGHTBLUE 0xADD8E6 /* 173,216,230 */
#define RGB_LIGHTCORAL 0xF08080 /* 240,128,128 */
#define RGB_LIGHTCYAN 0xE0FFFF /* 224,255,255 */
#define RGB_LIGHTGOLDENRODYELLOW 0xFAFAD2 /* 250,250,210 */
#define RGB_LIGHTGRAY 0xD3D3D3 /* 211,211,211 */
#define RGB_LIGHTGREEN 0x90EE90 /* 144,238,144 */
#define RGB_LIGHTGREY 0xD3D3D3 /* 211,211,211 */
#define RGB_LIGHTPINK 0xFFB6C1 /* 255,182,193 */
#define RGB_LIGHTSALMON 0xFFA07A /* 255,160,122 */
#define RGB_LIGHTSEAGREEN 0x20B2AA /* 32,178,170 */
#define RGB_LIGHTSKYBLUE 0x87CEFA /* 135,206,250 */
#define RGB_LIGHTSLATEGRAY 0x778899 /* 119,136,153 */
#define RGB_LIGHTSLATEGREY 0x778899 /* 119,136,153 */
#define RGB_LIGHTSTEELBLUE 0xB0C4DE /* 176,196,222 */
#define RGB_LIGHTYELLOW 0xFFFFE0 /* 255,255,224 */
#define RGB_LIME 0x00FF00 /* 0,255,0 */
#define RGB_LIMEGREEN 0x32CD32 /* 50,205,50 */
#define RGB_LINEN 0xFAF0E6 /* 250,240,230 */
#define RGB_MAGENTA 0xFF00FF /* 255,0,255 */
#define RGB_MAROON 0x800000 /* 128,0,0 */
#define RGB_MEDIUMAQUAMARINE 0x66CDAA /* 102,205,170 */
#define RGB_MEDIUMBLUE 0x0000CD /* 0,0,205 */
#define RGB_MEDIUMORCHID 0xBA55D3 /* 186,85,211 */
#define RGB_MEDIUMPURPLE 0x9370DB /* 147,112,219 */
#define RGB_MEDIUMSEAGREEN 0x3CB371 /* 60,179,113 */
#define RGB_MEDIUMSLATEBLUE 0x7B68EE /* 123,104,238 */
#define RGB_MEDIUMSPRINGGREEN 0x00FA9A /* 0,250,154 */
#define RGB_MEDIUMTURQUOISE 0x48D1CC /* 72,209,204 */
#define RGB_MEDIUMVIOLETRED 0xC71585 /* 199,21,133 */
#define RGB_MIDNIGHTBLUE 0x191970 /* 25,25,112 */
#define RGB_MINTCREAM 0xF5FFFA /* 245,255,250 */
#define RGB_MISTYROSE 0xFFE4E1 /* 255,228,225 */
#define RGB_MOCCASIN 0xFFE4B5 /* 255,228,181 */
#define RGB_NAVAJOWHITE 0xFFDEAD /* 255,222,173 */
#define RGB_NAVY 0x000080 /* 0,0,128 */
#define RGB_OLDLACE 0xFDF5E6 /* 253,245,230 */
#define RGB_OLIVE 0x808000 /* 128,128,0 */
#define RGB_OLIVEDRAB 0x6B8E23 /* 107,142,35 */
#define RGB_ORANGE 0xFFA500 /* 255,165,0 */
#define RGB_ORANGERED 0xFF4500 /* 255,69,0 */
#define RGB_ORCHID 0xDA70D6 /* 218,112,214 */
#define RGB_PALEGOLDENROD 0xEEE8AA /* 238,232,170 */
#define RGB_PALEGREEN 0x98FB98 /* 152,251,152 */
#define RGB_PALETURQUOISE 0xAFEEEE /* 175,238,238 */
#define RGB_PALEVIOLETRED 0xDB7093 /* 219,112,147 */
#define RGB_PAPAYAWHIP 0xFFEFD5 /* 255,239,213 */
#define RGB_PEACHPUFF 0xFFDAB9 /* 255,218,185 */
#define RGB_PERU 0xCD853F /* 205,133,63 */
#define RGB_PINK 0xFFC0CB /* 255,192,203 */
#define RGB_PLUM 0xDDA0DD /* 221,160,221 */
#define RGB_POWDERBLUE 0xB0E0E6 /* 176,224,230 */
#define RGB_PURPLE 0x800080 /* 128,0,128 */
#define RGB_RED 0xFF0000 /* 255,0,0 */
#define RGB_ROSYBROWN 0xBC8F8F /* 188,143,143 */
#define RGB_ROYALBLUE 0x4169E1 /* 65,105,225 */
#define RGB_SADDLEBROWN 0x8B4513 /* 139,69,19 */
#define RGB_SALMON 0xFA8072 /* 250,128,114 */
#define RGB_SANDYBROWN 0xF4A460 /* 244,164,96 */
#define RGB_SEAGREEN 0x2E8B57 /* 46,139,87 */
#define RGB_SEASHELL 0xFFF5EE /* 255,245,238 */
#define RGB_SIENNA 0xA0522D /* 160,82,45 */
#define RGB_SILVER 0xC0C0C0 /* 192,192,192 */
#define RGB_SKYBLUE 0x87CEEB /* 135,206,235 */
#define RGB_SLATEBLUE 0x6A5ACD /* 106,90,205 */
#define RGB_SLATEGRAY 0x708090 /* 112,128,144 */
#define RGB_SLATEGREY 0x708090 /* 112,128,144 */
#define RGB_SNOW 0xFFFAFA /* 255,250,250 */
#define RGB_SPRINGGREEN 0x00FF7F /* 0,255,127 */
#define RGB_STEELBLUE 0x4682B4 /* 70,130,180 */
#define RGB_TAN 0xD2B48C /* 210,180,140 */
#define RGB_TEAL 0x008080 /* 0,128,128 */
#define RGB_THISTLE 0xD8BFD8 /* 216,191,216 */
#define RGB_TOMATO 0xFF6347 /* 255,99,71 */
#define RGB_TURQUOISE 0x40E0D0 /* 64,224,208 */
#define RGB_VIOLET 0xEE82EE /* 238,130,238 */
#define RGB_WHEAT 0xF5DEB3 /* 245,222,179 */
#define RGB_WHITE 0xFFFFFF /* 255,255,255 */
#define RGB_WHITESMOKE 0xF5F5F5 /* 245,245,245 */
#define RGB_YELLOW 0xFFFF00 /* 255,255,0 */
#define RGB_YELLOWGREEN 0x9ACD32 /* 154,205,50  */

/* structures =============================================================== */
typedef union xRgbColor {
  uint32_t ulRgb;
  struct {
    uint8_t ucBlue;
    uint8_t ucGreen;
    uint8_t ucRed;
    uint8_t ucHue;
  };
} xRgbColor;

#endif /* __DOXYGEN__ not defined */
__END_C_DECLS
/* ========================================================================== */
#endif  /* _AVRIO_RGB_H_ not defined */

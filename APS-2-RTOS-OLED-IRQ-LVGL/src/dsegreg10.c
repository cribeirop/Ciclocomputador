/*******************************************************************************
 * Size: 10 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/
#define LV_LVGL_H_INCLUDE_SIMPLE

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef DSEGREG10
#define DSEGREG10 1
#endif

#if DSEGREG10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xc0,

    /* U+0030 "0" */
    0x7a, 0x18, 0x61, 0x86, 0x18, 0x61, 0x85, 0xe0,

    /* U+0031 "1" */
    0xff, 0x80,

    /* U+0032 "2" */
    0x78, 0x10, 0x41, 0xfa, 0x8, 0x20, 0x81, 0xe0,

    /* U+0033 "3" */
    0xf8, 0x10, 0x41, 0xf8, 0x10, 0x41, 0x7, 0xe0,

    /* U+0034 "4" */
    0x86, 0x18, 0x61, 0xf8, 0x10, 0x41, 0x4,

    /* U+0035 "5" */
    0x7a, 0x8, 0x20, 0xf8, 0x10, 0x41, 0x5, 0xe0,

    /* U+0036 "6" */
    0x7a, 0x8, 0x20, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0037 "7" */
    0x7a, 0x18, 0x61, 0x84, 0x0, 0x41, 0x4, 0x10,

    /* U+0038 "8" */
    0x7a, 0x18, 0x61, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0039 "9" */
    0x7a, 0x18, 0x61, 0xf8, 0x10, 0x41, 0x5, 0xe0,

    /* U+0041 "A" */
    0x7a, 0x18, 0x61, 0x87, 0xe8, 0x61, 0x86, 0x10,

    /* U+0042 "B" */
    0x82, 0x8, 0x20, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0043 "C" */
    0xfe, 0x8, 0x20, 0x81, 0xf0,

    /* U+0044 "D" */
    0x4, 0x10, 0x41, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0045 "E" */
    0x7e, 0x8, 0x20, 0xfe, 0x8, 0x20, 0x81, 0xf0,

    /* U+0046 "F" */
    0x7e, 0x8, 0x20, 0x83, 0xf8, 0x20, 0x82, 0x0,

    /* U+0047 "G" */
    0x7a, 0x8, 0x20, 0x82, 0x18, 0x61, 0x85, 0xe0,

    /* U+0048 "H" */
    0x82, 0x8, 0x20, 0xfa, 0x18, 0x61, 0x84,

    /* U+0049 "I" */
    0xf0,

    /* U+004A "J" */
    0x4, 0x10, 0x41, 0x2, 0x18, 0x61, 0x85, 0xe0,

    /* U+004B "K" */
    0x7a, 0x8, 0x20, 0x83, 0xe8, 0x61, 0x86, 0x10,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x2, 0x8, 0x20, 0x81, 0xf0,

    /* U+004D "M" */
    0x7a, 0x18, 0x61, 0x84, 0x8, 0x61, 0x86, 0x10,

    /* U+004E "N" */
    0xfa, 0x18, 0x61, 0x84,

    /* U+004F "O" */
    0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0050 "P" */
    0x7a, 0x18, 0x61, 0x87, 0xe8, 0x20, 0x82, 0x0,

    /* U+0051 "Q" */
    0x7a, 0x18, 0x61, 0x87, 0xe0, 0x41, 0x4, 0x10,

    /* U+0052 "R" */
    0xfe, 0x8, 0x20, 0x80,

    /* U+0053 "S" */
    0x82, 0x8, 0x20, 0xf8, 0x10, 0x41, 0x5, 0xe0,

    /* U+0054 "T" */
    0x82, 0x8, 0x20, 0xfe, 0x8, 0x20, 0x81, 0xf0,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x78,

    /* U+0056 "V" */
    0x86, 0x18, 0x61, 0x2, 0x18, 0x61, 0x85, 0xe0,

    /* U+0057 "W" */
    0x86, 0x18, 0x61, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0058 "X" */
    0x86, 0x18, 0x61, 0xfa, 0x18, 0x61, 0x84,

    /* U+0059 "Y" */
    0x86, 0x18, 0x61, 0xf8, 0x10, 0x41, 0x5, 0xe0,

    /* U+005A "Z" */
    0x78, 0x10, 0x41, 0x6, 0x8, 0x20, 0x81, 0xe0,

    /* U+0061 "a" */
    0x7a, 0x18, 0x61, 0x87, 0xe8, 0x61, 0x86, 0x10,

    /* U+0062 "b" */
    0x82, 0x8, 0x20, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0063 "c" */
    0xfe, 0x8, 0x20, 0x81, 0xf0,

    /* U+0064 "d" */
    0x4, 0x10, 0x41, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0065 "e" */
    0x7e, 0x8, 0x20, 0xfe, 0x8, 0x20, 0x81, 0xf0,

    /* U+0066 "f" */
    0x7e, 0x8, 0x20, 0x83, 0xf8, 0x20, 0x82, 0x0,

    /* U+0067 "g" */
    0x7a, 0x8, 0x20, 0x82, 0x18, 0x61, 0x85, 0xe0,

    /* U+0068 "h" */
    0x82, 0x8, 0x20, 0xfa, 0x18, 0x61, 0x84,

    /* U+0069 "i" */
    0xf0,

    /* U+006A "j" */
    0x4, 0x10, 0x41, 0x2, 0x18, 0x61, 0x85, 0xe0,

    /* U+006B "k" */
    0x7a, 0x8, 0x20, 0x83, 0xe8, 0x61, 0x86, 0x10,

    /* U+006C "l" */
    0x82, 0x8, 0x20, 0x2, 0x8, 0x20, 0x81, 0xf0,

    /* U+006D "m" */
    0x7a, 0x18, 0x61, 0x84, 0x8, 0x61, 0x86, 0x10,

    /* U+006E "n" */
    0xfa, 0x18, 0x61, 0x84,

    /* U+006F "o" */
    0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0070 "p" */
    0x7a, 0x18, 0x61, 0x87, 0xe8, 0x20, 0x82, 0x0,

    /* U+0071 "q" */
    0x7a, 0x18, 0x61, 0x87, 0xe0, 0x41, 0x4, 0x10,

    /* U+0072 "r" */
    0xfe, 0x8, 0x20, 0x80,

    /* U+0073 "s" */
    0x82, 0x8, 0x20, 0xf8, 0x10, 0x41, 0x5, 0xe0,

    /* U+0074 "t" */
    0x82, 0x8, 0x20, 0xfe, 0x8, 0x20, 0x81, 0xf0,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x78,

    /* U+0076 "v" */
    0x86, 0x18, 0x61, 0x2, 0x18, 0x61, 0x85, 0xe0,

    /* U+0077 "w" */
    0x86, 0x18, 0x61, 0xfa, 0x18, 0x61, 0x85, 0xe0,

    /* U+0078 "x" */
    0x86, 0x18, 0x61, 0xfa, 0x18, 0x61, 0x84,

    /* U+0079 "y" */
    0x86, 0x18, 0x61, 0xf8, 0x10, 0x41, 0x5, 0xe0,

    /* U+007A "z" */
    0x78, 0x10, 0x41, 0x6, 0x8, 0x20, 0x81, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 32, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 131, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 2, .adv_w = 0, .box_w = 2, .box_h = 1, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 131, .box_w = 1, .box_h = 10, .ofs_x = 6, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 21, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 29, .adv_w = 131, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 36, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 52, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 84, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 131, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 121, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 131, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 136, .adv_w = 131, .box_w = 1, .box_h = 5, .ofs_x = 6, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 145, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 153, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 173, .adv_w = 131, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 186, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 194, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 198, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 131, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 241, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 265, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 131, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 294, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 302, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 131, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 317, .adv_w = 131, .box_w = 1, .box_h = 5, .ofs_x = 6, .ofs_y = 0},
    {.bitmap_index = 318, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 334, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 350, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 354, .adv_w = 131, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 359, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 367, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 375, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 379, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 131, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 422, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 131, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0xd, 0xe
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 15, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 48, .range_length = 10, .glyph_id_start = 4,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 14,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 40,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 4,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t dsegreg10 = {
#else
lv_font_t dsegreg10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if DSEGREG10*/


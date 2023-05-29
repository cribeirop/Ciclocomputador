/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/
#define LV_LVGL_H_INCLUDE_SIMPLE

/*******************************************************************************
 * Size: 30 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef BOT
#define BOT 1
#endif

#if BOT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F544 "?" */
    0x0, 0x0, 0x30, 0x0, 0x0, 0x0, 0x1, 0xe0,
    0x0, 0x0, 0x0, 0x7, 0x80, 0x0, 0x0, 0x0,
    0x1e, 0x0, 0x0, 0x0, 0x0, 0x78, 0x0, 0x0,
    0x0, 0x1, 0xe0, 0x0, 0x0, 0xf, 0xff, 0xff,
    0xc0, 0x0, 0x7f, 0xff, 0xff, 0x80, 0x3, 0xff,
    0xff, 0xfe, 0x0, 0xf, 0xff, 0xff, 0xfc, 0x0,
    0x3f, 0xff, 0xff, 0xf0, 0x0, 0xff, 0xff, 0xff,
    0xc0, 0x3, 0xff, 0xff, 0xff, 0x0, 0xcf, 0xc7,
    0xf8, 0xfc, 0xcf, 0x3e, 0x1f, 0xe1, 0xf3, 0xfc,
    0xf8, 0x7f, 0x87, 0xcf, 0xf3, 0xe1, 0xfe, 0x1f,
    0x3f, 0xcf, 0xc7, 0xf8, 0xfc, 0xff, 0x3f, 0xff,
    0xff, 0xf3, 0xfc, 0xff, 0xff, 0xff, 0xcf, 0xf3,
    0xff, 0xff, 0xff, 0x3f, 0xcf, 0xff, 0xff, 0xfc,
    0xff, 0x3f, 0xff, 0xff, 0xf3, 0xfc, 0xf8, 0x61,
    0x87, 0xcf, 0x73, 0xe1, 0x86, 0x1f, 0x38, 0xf,
    0xff, 0xff, 0xfc, 0x0, 0x3f, 0xff, 0xff, 0xf0,
    0x0, 0xff, 0xff, 0xff, 0xc0, 0x1, 0xff, 0xff,
    0xfe, 0x0, 0x3, 0xff, 0xff, 0xf0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 600, .box_w = 38, .box_h = 30, .ofs_x = 0, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 97, .range_length = 1, .glyph_id_start = 1,
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
    .cmap_num = 1,
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
const lv_font_t bot = {
#else
lv_font_t bot = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 30,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -11,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if BOT*/


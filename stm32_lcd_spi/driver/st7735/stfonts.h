#ifndef __ST_FONTS_H__
#define __ST_FONTS_H__


#include <stdint.h>


typedef struct
{
    const uint16_t width;
    const uint16_t height;
    const uint8_t *data;
    const uint32_t count;
} st_fonts_t;


extern st_fonts_t font_ascii_8x16;
extern st_fonts_t font_ni_hao_shi_jie_16x16;
extern st_fonts_t font_hello_world_8x16;


#endif /* __ST_FONTS_H__ */

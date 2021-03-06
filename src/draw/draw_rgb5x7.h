#ifndef DRAW_16SEG_H
#define DRAW_16SEG_H
#include<stdint.h>
#include"draw_rgbLED.h"
inline int cairo_code_draw_rgb5x7_get_width()
{
    return cairo_code_draw_rgbLED_get_width()*5*1.3;
}
inline int cairo_code_draw_rgb5x7_get_height()
{
    return cairo_code_draw_rgbLED_get_height()*7*1.3;
}

//just subtract 32 from the char and you get the index here
//each byte is a column and msb to lsb is bottom to top row, msb not used
static const uint8_t Font5x7[][5]  =
{
    {0x00, 0x00, 0x00, 0x00, 0x00},// (space)
    {0x00, 0x00, 0x5F, 0x00, 0x00},// !
    {0x00, 0x07, 0x00, 0x07, 0x00},// "
    {0x14, 0x7F, 0x14, 0x7F, 0x14},// #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},// $
    {0x23, 0x13, 0x08, 0x64, 0x62},// %
    {0x36, 0x49, 0x55, 0x22, 0x50},// &
    {0x00, 0x05, 0x03, 0x00, 0x00},// '
    {0x00, 0x1C, 0x22, 0x41, 0x00},// (
    {0x00, 0x41, 0x22, 0x1C, 0x00},// )
    {0x08, 0x2A, 0x1C, 0x2A, 0x08},// *
    {0x08, 0x08, 0x3E, 0x08, 0x08},// +
    {0x00, 0x50, 0x30, 0x00, 0x00},// ,
    {0x08, 0x08, 0x08, 0x08, 0x08},// -
    {0x00, 0x60, 0x60, 0x00, 0x00},// .
    {0x20, 0x10, 0x08, 0x04, 0x02},// /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},// 1
    {0x42, 0x61, 0x51, 0x49, 0x46},// 2
    {0x21, 0x41, 0x45, 0x4B, 0x31},// 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},// 4
    {0x27, 0x45, 0x45, 0x45, 0x39},// 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
    {0x01, 0x71, 0x09, 0x05, 0x03},// 7
    {0x36, 0x49, 0x49, 0x49, 0x36},// 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},// 9
    {0x00, 0x36, 0x36, 0x00, 0x00},// :
    {0x00, 0x56, 0x36, 0x00, 0x00},// ;
    {0x00, 0x08, 0x14, 0x22, 0x41},// <
    {0x14, 0x14, 0x14, 0x14, 0x14},// =
    {0x41, 0x22, 0x14, 0x08, 0x00},// >
    {0x02, 0x01, 0x51, 0x09, 0x06},// ?
    {0x32, 0x49, 0x79, 0x41, 0x3E},// @
    {0x7E, 0x11, 0x11, 0x11, 0x7E},// A
    {0x7F, 0x49, 0x49, 0x49, 0x36},// B
    {0x3E, 0x41, 0x41, 0x41, 0x22},// C
    {0x7F, 0x41, 0x41, 0x22, 0x1C},// D
    {0x7F, 0x49, 0x49, 0x49, 0x41},// E
    {0x7F, 0x09, 0x09, 0x01, 0x01},// F
    {0x3E, 0x41, 0x41, 0x51, 0x32},// G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},// H
    {0x00, 0x41, 0x7F, 0x41, 0x00},// I
    {0x20, 0x40, 0x41, 0x3F, 0x01},// J
    {0x7F, 0x08, 0x14, 0x22, 0x41},// K
    {0x7F, 0x40, 0x40, 0x40, 0x40},// L
    {0x7F, 0x02, 0x04, 0x02, 0x7F},// M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},// N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},// O
    {0x7F, 0x09, 0x09, 0x09, 0x06},// P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},// Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},// R
    {0x46, 0x49, 0x49, 0x49, 0x31},// S
    {0x01, 0x01, 0x7F, 0x01, 0x01},// T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},// U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},// V
    {0x7F, 0x20, 0x18, 0x20, 0x7F},// W
    {0x63, 0x14, 0x08, 0x14, 0x63},// X
    {0x03, 0x04, 0x78, 0x04, 0x03},// Y
    {0x61, 0x51, 0x49, 0x45, 0x43},// Z
    {0x00, 0x00, 0x7F, 0x41, 0x41},// [
    {0x02, 0x04, 0x08, 0x10, 0x20},// "\"
    {0x41, 0x41, 0x7F, 0x00, 0x00},// ]
    {0x04, 0x02, 0x01, 0x02, 0x04},// ^
    {0x40, 0x40, 0x40, 0x40, 0x40},// _
    {0x00, 0x01, 0x02, 0x04, 0x00},// `
    {0x20, 0x54, 0x54, 0x54, 0x78},// a
    {0x7F, 0x48, 0x44, 0x44, 0x38},// b
    {0x38, 0x44, 0x44, 0x44, 0x20},// c
    {0x38, 0x44, 0x44, 0x48, 0x7F},// d
    {0x38, 0x54, 0x54, 0x54, 0x18},// e
    {0x08, 0x7E, 0x09, 0x01, 0x02},// f
    {0x08, 0x14, 0x54, 0x54, 0x3C},// g
    {0x7F, 0x08, 0x04, 0x04, 0x78},// h
    {0x00, 0x44, 0x7D, 0x40, 0x00},// i
    {0x20, 0x40, 0x44, 0x3D, 0x00},// j
    {0x00, 0x7F, 0x10, 0x28, 0x44},// k
    {0x00, 0x41, 0x7F, 0x40, 0x00},// l
    {0x7C, 0x04, 0x18, 0x04, 0x78},// m
    {0x7C, 0x08, 0x04, 0x04, 0x78},// n
    {0x38, 0x44, 0x44, 0x44, 0x38},// o
    {0x7C, 0x14, 0x14, 0x14, 0x08},// p
    {0x08, 0x14, 0x14, 0x18, 0x7C},// q
    {0x7C, 0x08, 0x04, 0x04, 0x08},// r
    {0x48, 0x54, 0x54, 0x54, 0x20},// s
    {0x04, 0x3F, 0x44, 0x40, 0x20},// t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},// u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},// v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},// w
    {0x44, 0x28, 0x10, 0x28, 0x44},// x
    {0x0C, 0x50, 0x50, 0x50, 0x3C},// y
    {0x44, 0x64, 0x54, 0x4C, 0x44},// z
    {0x00, 0x08, 0x36, 0x41, 0x00},// {
    {0x00, 0x00, 0x7F, 0x00, 0x00},// |
    {0x00, 0x41, 0x36, 0x08, 0x00},// }
    {0x08, 0x08, 0x2A, 0x1C, 0x08},// ->//should be tilde
    {0x08, 0x1C, 0x2A, 0x08, 0x08} // <-
};

inline void cairo_code_draw_rgb5x7_render(cairo_t *cr, char num, float r, float g, float b)
{

    float offset = 1.3*cairo_code_draw_rgbLED_get_width();//1.3 so there is a space between
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<5; j++)
        {
            cairo_save( cr );
            //move
            cairo_translate(cr,j*offset,i*offset);//origin is left upper corner
            //draw led
            cairo_code_draw_rgbLED_render(cr, Font5x7[num-32][j]&(0x01<<i), r,g,b);
            cairo_restore(cr);
        }
    }
    //todo bounding box?

}

inline void cairo_code_draw_red5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, 1, 0, 0);
}
inline void cairo_code_draw_blue5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, .1, .2, 1);
}
inline void cairo_code_draw_green5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, 1, 0.88235, 0);
}
inline void cairo_code_draw_orange5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, 1, .33333, 0);
}
inline void cairo_code_draw_yellow5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, 0.92157, 0.92157, 0);
}
inline void cairo_code_draw_purple5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, 0.78431, 0, 1);
}
inline void cairo_code_draw_white5x7_render(cairo_t *cr, char num)
{
    cairo_code_draw_rgb5x7_render(cr, num, 1, 1, 1);
}
#endif

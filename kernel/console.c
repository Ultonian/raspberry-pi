/*
 * Copyright (c) 2014 Marco Maccaferri and Others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "console.h"

#define FONT                    vgafont8
#define BIT_SHIFT               (7 - s_bit_no)

#define CHAR_W                  8
#define CHAR_H                  8

static unsigned char vgafont8[128 * 8]= {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7e, 0x81, 0xa5, 0x81, 0xbd, 0x99, 0x81, 0x7e,
    0x7e, 0xff, 0xdb, 0xff, 0xc3, 0xe7, 0xff, 0x7e,
    0x6c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38, 0x10, 0x00,
    0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00,
    0x38, 0x7c, 0x38, 0xfe, 0xfe, 0x7c, 0x38, 0x7c,
    0x10, 0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x7c,
    0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00,
    0xff, 0xff, 0xe7, 0xc3, 0xc3, 0xe7, 0xff, 0xff,
    0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00,
    0xff, 0xc3, 0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff,
    0x0f, 0x07, 0x0f, 0x7d, 0xcc, 0xcc, 0xcc, 0x78,
    0x3c, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x7e, 0x18,
    0x3f, 0x33, 0x3f, 0x30, 0x30, 0x70, 0xf0, 0xe0,
    0x7f, 0x63, 0x7f, 0x63, 0x63, 0x67, 0xe6, 0xc0,
    0x99, 0x5a, 0x3c, 0xe7, 0xe7, 0x3c, 0x5a, 0x99,
    0x80, 0xe0, 0xf8, 0xfe, 0xf8, 0xe0, 0x80, 0x00,
    0x02, 0x0e, 0x3e, 0xfe, 0x3e, 0x0e, 0x02, 0x00,
    0x18, 0x3c, 0x7e, 0x18, 0x18, 0x7e, 0x3c, 0x18,
    0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,
    0x7f, 0xdb, 0xdb, 0x7b, 0x1b, 0x1b, 0x1b, 0x00,
    0x3e, 0x63, 0x38, 0x6c, 0x6c, 0x38, 0xcc, 0x78,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x7e, 0x00,
    0x18, 0x3c, 0x7e, 0x18, 0x7e, 0x3c, 0x18, 0xff,
    0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x00,
    0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00,
    0x00, 0x18, 0x0c, 0xfe, 0x0c, 0x18, 0x00, 0x00,
    0x00, 0x30, 0x60, 0xfe, 0x60, 0x30, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfe, 0x00, 0x00,
    0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00, 0x00,
    0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00,
    0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00,
    0x30, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x30, 0x00,
    0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,
    0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00,
    0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,
    0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
    0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00,
    0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60,
    0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00,
    0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00,
    0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0x7c, 0x00,
    0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00,
    0x78, 0xcc, 0x0c, 0x38, 0x60, 0xcc, 0xfc, 0x00,
    0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00,
    0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00,
    0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00,
    0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00,
    0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00,
    0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00,
    0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00,
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00,
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,
    0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00,
    0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0x00,
    0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00,
    0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00,
    0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00,
    0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00,
    0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00,
    0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00,
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00,
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,
    0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3e, 0x00,
    0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00,
    0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00,
    0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00,
    0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00,
    0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00,
    0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00,
    0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00,
    0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x1c, 0x00,
    0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00,
    0x78, 0xcc, 0xe0, 0x70, 0x1c, 0xcc, 0x78, 0x00,
    0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00,
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,
    0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00,
    0xc6, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00,
    0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00,
    0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00,
    0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
    0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00,
    0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,
    0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,
    0xe0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78, 0x00,
    0x1c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,
    0x38, 0x6c, 0x60, 0xf0, 0x60, 0x60, 0xf0, 0x00,
    0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00,
    0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78,
    0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00,
    0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
    0x00, 0x00, 0xcc, 0xfe, 0xfe, 0xd6, 0xc6, 0x00,
    0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0x00,
    0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78, 0x00,
    0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0,
    0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e,
    0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0xf0, 0x00,
    0x00, 0x00, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x00,
    0x10, 0x30, 0x7c, 0x30, 0x30, 0x34, 0x18, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,
    0x00, 0x00, 0xc6, 0xd6, 0xfe, 0xfe, 0x6c, 0x00,
    0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00,
    0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,
    0x00, 0x00, 0xfc, 0x98, 0x30, 0x64, 0xfc, 0x00,
    0x1c, 0x30, 0x30, 0xe0, 0x30, 0x30, 0x1c, 0x00,
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,
    0xe0, 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00,
    0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00,
};

int con_width;
int con_height;
int txt_width;
int txt_height;
int cur_x;
int cur_y;
pixel_t cur_fore;
pixel_t cur_back;

static int offs_x;
static int offs_y;

static pixel_t cursor_fore;
static int     cursor_visible;
static pixel_t cursor_buffer[CHAR_H * CHAR_W];

void initscr(int width, int height) {
    if (width == 0 || height == 0) {
        width = fb_width / CHAR_W;
        height = fb_height / CHAR_H;
    }

    con_width = width * CHAR_W;
    con_height = height * CHAR_H;

    offs_x = (fb_width - con_width) / 2;
    offs_y = (fb_height - con_height) / 2;

    txt_width = width;
    txt_height = height;
    cur_x = cur_y = 0;

    cur_fore = cursor_fore = WHITE;
    cur_back = BLACK;
}

static void console_newline() {
    cur_y++;

    // Scroll up if necessary
    if (cur_y == txt_height) {
        int line_offset = offs_y * fb_pitch + offs_x * BYTES_PER_PIXEL;
        int line_byte_width = txt_width * CHAR_W * BYTES_PER_PIXEL;
        int height = txt_height * CHAR_H - CHAR_H;

        for (int line = 0; line < height; line++) {
            memcpy((void *) (fb_addr + line_offset), (void *) (fb_addr + line_offset + CHAR_H * fb_pitch), line_byte_width);
            line_offset += fb_pitch;
        }

        for (int line = 0; line < CHAR_H; line++) {
            volatile pixel_t *fb = (pixel_t *) (fb_addr + line_offset);
            for (int c_x = 0; c_x < txt_width * CHAR_W; c_x++) {
                fb[c_x] = cur_back;
            }
            line_offset += fb_pitch;
        }

        cur_y--;
    }
}

int addch(int c) {
    hide_cursor();

    if (c == '\n')
        console_newline();
    else if (c == '\r') {
        cur_x = 0;
    }
    else if (c == '\t') {
        cur_x = ((cur_x / 8) + 1) * 8;
        if (cur_x >= txt_width) {
            console_newline();
            cur_x -= txt_width;
        }
    }
    else if (c == 0x07) {
        // BELL, ignore
    }
    else {
        if (cur_x >= txt_width) {
            console_newline();
            cur_x = 0;
        }

        int line_d_offset = (offs_y + cur_y * CHAR_H) * fb_pitch + (offs_x + cur_x * CHAR_W) * BYTES_PER_PIXEL;
        int s_offset = (int) c * CHAR_W * CHAR_H;

        for (int c_y = 0; c_y < CHAR_H; c_y++) {
            volatile pixel_t *fb = (pixel_t *) (fb_addr + line_d_offset);

            for (int c_x = 0; c_x < CHAR_W; c_x++) {
                int s_byte_no = s_offset / 8;
                int s_bit_no = s_offset % 8;

                unsigned char s_byte = FONT[s_byte_no];
                if ((s_byte >> BIT_SHIFT) & 0x1)
                    *fb++ = cur_fore;
                else
                    *fb++ = cur_back;
                s_offset++;
            }

            line_d_offset += fb_pitch;
        }

        cur_x++;
    }

    return c;
}

void move(int y, int x) {
    hide_cursor();

    while(y < 0) {
        y += txt_height;
    }
    while(x < 0) {
        x += txt_width;
    }

    cur_y = y % txt_height;
    cur_x = x % txt_width;
}

int addstr(const char *s) {
    int r = 0;

    while(*s) {
        addch(*s++);
        r++;
    }

    return r;
}

void clear() {
    int line_offset = offs_y * fb_pitch + offs_x * BYTES_PER_PIXEL;

    hide_cursor();

    for (int c_y = 0; c_y < txt_height * CHAR_H; c_y++) {
        volatile pixel_t *fb = (pixel_t *) (fb_addr + line_offset);
        for (int c_x = 0; c_x < txt_width * CHAR_W; c_x++) {
            *fb++ = cur_back;
        }
        line_offset += fb_pitch;
    }

    cur_x = cur_y = 0;
}

int mvaddstr(int y, int x, const char *s) {
    int r = 0;

    hide_cursor();
    cur_y = y % txt_width;
    cur_x = x % txt_height;

    while(*s) {
        addch(*s++);
        r++;
    }

    return r;
}

int mvaddch(int y, int x, int c) {
    hide_cursor();

    while(y < 0) {
        y += txt_height;
    }
    while(x < 0) {
        x += txt_width;
    }

    cur_y = y % txt_height;
    cur_x = x % txt_width;

    return addch(c);
}

void refresh() {
    // Do nothing, updates are immediate
    fb_flip();
}

void toggle_cursor() {
    int line_d_offset = (offs_y + cur_y * CHAR_H) * fb_pitch + (offs_x + cur_x * CHAR_W) * BYTES_PER_PIXEL;
    int s_offset = 0;

    if (!cursor_visible) {
        for (int c_y = 0; c_y < CHAR_H; c_y++) {
            volatile pixel_t *fb = (pixel_t *) (fb_addr + line_d_offset);
            for (int c_x = 0; c_x < CHAR_W; c_x++) {
                cursor_buffer[s_offset++] = *fb;
                *fb++ = cursor_fore;
            }
            line_d_offset += fb_pitch;
        }
        cursor_visible = 1;
    }
    else {
        for (int c_y = 0; c_y < CHAR_H; c_y++) {
            volatile pixel_t *fb = (pixel_t *) (fb_addr + line_d_offset);
            for (int c_x = 0; c_x < CHAR_W; c_x++) {
                *fb++ = cursor_buffer[s_offset++];
            }
            line_d_offset += fb_pitch;
        }
        cursor_visible = 0;
    }
}

void hide_cursor() {
    int line_d_offset = (offs_y + cur_y * CHAR_H) * fb_pitch + (offs_x + cur_x * CHAR_W) * BYTES_PER_PIXEL;
    int s_offset = 0;

    if (cursor_visible) {
        for (int c_y = 0; c_y < CHAR_H; c_y++) {
            volatile pixel_t *fb = (pixel_t *) (fb_addr + line_d_offset);
            for (int c_x = 0; c_x < CHAR_W; c_x++) {
                *fb++ = cursor_buffer[s_offset++];
            }
            line_d_offset += fb_pitch;
        }
        cursor_visible = 0;
    }
}

int addstrf(const char *ptr, ...) {
    va_list var;
    char buffer[256];

    va_start (var, ptr);
    vsnprintf (buffer, sizeof(buffer) - 1, ptr, var);
    va_end (var);

    return addstr(buffer);
}

int mvaddstrf(int y, int x, const char *ptr, ...) {
    va_list var;
    char buffer[256];

    va_start (var, ptr);
    vsnprintf (buffer, sizeof(buffer) - 1, ptr, var);
    va_end (var);

    return mvaddstr(y, x, buffer);
}

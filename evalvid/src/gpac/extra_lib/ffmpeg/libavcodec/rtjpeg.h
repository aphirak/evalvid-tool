/*
 * RTJpeg decoding functions
 * copyright (c) 2006 Reimar Doeffinger
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef RTJPEG_H
#define RTJPEG_H

typedef struct {
    int w, h;
    DSPContext *dsp;
    uint8_t scan[64];
    uint32_t lquant[64];
    uint32_t cquant[64];
} RTJpegContext;

void rtjpeg_decode_init(RTJpegContext *c, DSPContext *dsp,
                        int width, int height,
                        uint32_t *lquant, uint32_t *cquant);

int rtjpeg_decode_frame_yuv420(RTJpegContext *c, AVFrame *f,
                               uint8_t *buf, int buf_size);
#endif

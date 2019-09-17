/***************************************************************************
*
* Copyright (c) 2019 Baidu.com, Inc. All Rights Reserved
*
**************************************************************************/

/*
 * zhangwenming02
 * 20190715
 * */
#include "arm_neon.h"
#include <iostream>
#include "yuv2bgr_with_convert.h"

G_NAMESPACE1_BEGIN(g_sdk_namespace)

const unsigned int g_block_length = 16;
void rotate_bgr_block(unsigned char* src, int src_row_step, unsigned char* dst, int dst_col_step, int dst_row_step) {
    unsigned char* src_tmp;
    unsigned char* dst_tmp;
    for (int r=0; r < g_block_length; ++r) {
        src_tmp = src;
        dst_tmp = dst;
        for (int c = 0; c < g_block_length; ++c) {
            *dst_tmp = *src_tmp++;
            *(dst_tmp+1) = *src_tmp++;
            *(dst_tmp+2) = *src_tmp++;
            dst_tmp += dst_col_step;
        }
        src += src_row_step;
        dst += dst_row_step;
    }
}

template<typename ratate_mirror>
bool rotate_bgr(unsigned char* src, unsigned char* dst, int h, int w) {
    if (!src || !dst || h % g_block_length || w % g_block_length) {
        return false;
    }

    ratate_mirror r_m(h, w, 3);
    dst = dst + r_m.start_shift;
    unsigned char* src_temp;
    unsigned char* dst_temp;

    int blocks_row  = h / g_block_length;
    int blocks_col = w / g_block_length;
    int src_row_step = w*3;
    int src_row_block_step = w*3*g_block_length;
    int dst_row_block_step = r_m.row_step * g_block_length;
    int dst_col_block_step = r_m.col_step * g_block_length;

    for (int i = 0; i < blocks_row; ++i) {
        src_temp = src;
        dst_temp = dst;
        for (int j = 0; j < blocks_col; ++j) {
            rotate_bgr_block(src_temp, src_row_step, dst_temp, r_m.col_step, r_m.row_step);
            src_temp += g_block_length * 3;
            dst_temp += dst_col_block_step;
        }
        src += src_row_block_step;
        dst += dst_row_block_step;
    }

    return true;
}

int32x4_t int32x4_0 = vdupq_n_s32(0);
int32x4_t int32x4_100 = vdupq_n_s32(100);
int32x4_t int32x4_208 = vdupq_n_s32(208);
int32x4_t int32x4_409 = vdupq_n_s32(409);
int32x4_t int32x4_516 = vdupq_n_s32(516);
int32x4_t int32x4_298 = vdupq_n_s32(298);
int32x4_t int32x4_65535 = vdupq_n_s32(65535);

int16x8_t int16x8_128 = vdupq_n_s16(128);
int16x8_t int16x8_16 = vdupq_n_s16(16);
int16x8_t int16x8_0 = vdupq_n_s16(0);

uint8x8_t get_channel_8(int32x4_t& int32x4_Y0, int32x4_t& int32x4_Y1, int32x4x2_t& int32x4x2_T){
    int32x4_t int32x4_YTR = vaddq_s32(int32x4_Y0, int32x4x2_T.val[0]);
    int32x4_YTR = vmaxq_s32(int32x4_YTR, int32x4_0);
    int32x4_YTR = vminq_s32(int32x4_YTR, int32x4_65535);
    int16x4_t int16x4_YTR0 = vshrn_n_s32(int32x4_YTR, 8);

    int32x4_YTR = vaddq_s32(int32x4_Y1, int32x4x2_T.val[1]);
    int32x4_YTR = vmaxq_s32(int32x4_YTR, int32x4_0);
    int32x4_YTR = vminq_s32(int32x4_YTR, int32x4_65535);
    int16x4_t int16x4_YTR1 = vshrn_n_s32(int32x4_YTR, 8);
    return vqmovun_s16(vcombine_s16(int16x4_YTR0, int16x4_YTR1));
}

bool decode_yuv_to_bgr(unsigned char* yuv, unsigned char* bgr, int h, int w) {
    if (0!=(w&3) || w<8 || 0!=(h&1) || h<2 || !yuv || !bgr) {
        return false;
    }

    int const h_step = h>>1;
    int const w_step = w>>3;
    int dst_row_step = w * 3;

    unsigned char const* y0 = yuv;
    unsigned char const* y1;
    unsigned char const* uv = yuv + (w*h);

    unsigned char* dst_this_0 = bgr;
    unsigned char* dst_this_1;

    for (int i=0; i< h_step; ++i) {
        y1 = y0+w;
        dst_this_1 = dst_this_0 + dst_row_step;
        for (int j=0; j< w_step; ++j) {
            uint8x8_t uint8x8_uv = vld1_u8(uv);
            int16x8_t int16x8_uv = vreinterpretq_s16_u16(vmovl_u8(uint8x8_uv));
            int16x8_uv = vsubq_s16(int16x8_uv, int16x8_128);
            int16x4_t int16x4_uv0 = vget_low_s16(int16x8_uv);
            int16x4_t int16x4_uv1 = vget_high_s16(int16x8_uv);
            int16x4x2_t int16x4x2_uv = vuzp_s16(int16x4_uv0, int16x4_uv1);
            int32x4_t int32x4_V = vmovl_s16(int16x4x2_uv.val[0]);
            int32x4_t int32x4_U = vmovl_s16(int16x4x2_uv.val[1]);

            int32x4_t int32x4_TR = vmulq_s32(int32x4_V, int32x4_409);
            int32x4_t int32x4_TG = vmlsq_s32(int32x4_0, int32x4_U, int32x4_100);
            int32x4_TG = vmlsq_s32(int32x4_TG, int32x4_V, int32x4_208);
            int32x4_t int32x4_TB = vmulq_s32(int32x4_U, int32x4_516);

            int32x4x2_t int32x4x2_TB = vzipq_s32(int32x4_TB, int32x4_TB);
            int32x4x2_t int32x4x2_TG = vzipq_s32(int32x4_TG, int32x4_TG);
            int32x4x2_t int32x4x2_TR = vzipq_s32(int32x4_TR, int32x4_TR);

            uint8x8_t uint8x8_y0 = vld1_u8(y0);
            int16x8_t int16x8_y0 = vreinterpretq_s16_u16(vmovl_u8(uint8x8_y0));
            int16x8_y0 = vsubq_s16(int16x8_y0, int16x8_16);
            int16x8_y0 = vmaxq_s16(int16x8_y0, int16x8_0);

            int32x4_t int32x4_Y00 = vmovl_s16(vget_low_s16(int16x8_y0));
            int32x4_Y00 = vmulq_s32(int32x4_Y00, int32x4_298);
            int32x4_t int32x4_Y01 = vmovl_s16(vget_high_s16(int16x8_y0));
            int32x4_Y01 = vmulq_s32(int32x4_Y01, int32x4_298);

            uint8x8x3_t uint8x8x3_rgb;
            uint8x8x3_rgb.val[0] = get_channel_8(int32x4_Y00, int32x4_Y01, int32x4x2_TB);
            uint8x8x3_rgb.val[1] = get_channel_8(int32x4_Y00, int32x4_Y01, int32x4x2_TG);
            uint8x8x3_rgb.val[2] = get_channel_8(int32x4_Y00, int32x4_Y01, int32x4x2_TR);
            vst3_u8(dst_this_0 ,uint8x8x3_rgb);
            dst_this_0 += 24;

            uint8x8_t uint8x8_y1 = vld1_u8(y1);
            int16x8_t int16x8_y1 = vreinterpretq_s16_u16(vmovl_u8(uint8x8_y1));
            int16x8_y1 = vsubq_s16(int16x8_y1, int16x8_16);
            int16x8_y1 = vmaxq_s16(int16x8_y1, int16x8_0);

            int32x4_t int32x4_Y10 = vmovl_s16(vget_low_s16(int16x8_y1));
            int32x4_Y10 = vmulq_s32(int32x4_Y10, int32x4_298);
            int32x4_t int32x4_Y11 = vmovl_s16(vget_high_s16(int16x8_y1));
            int32x4_Y11 = vmulq_s32(int32x4_Y11, int32x4_298);

            uint8x8x3_rgb.val[0] = get_channel_8(int32x4_Y10, int32x4_Y11, int32x4x2_TB);
            uint8x8x3_rgb.val[1] = get_channel_8(int32x4_Y10, int32x4_Y11, int32x4x2_TG);
            uint8x8x3_rgb.val[2] = get_channel_8(int32x4_Y10, int32x4_Y11, int32x4x2_TR);
            vst3_u8(dst_this_1 ,uint8x8x3_rgb);

            uv += 8;
            y0+=8;
            y1+=8;
            dst_this_1 += 24;

        }
        y0 = y1;
        dst_this_0 = dst_this_1;
    }

    return true;
}

class Rotate0 {
public:
    Rotate0(int rows, int cols, int channels) :
            start_shift(0),
            row_step(cols * channels),
            col_step(channels){ }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate0Mirror {
public:
    Rotate0Mirror(int rows, int cols, int channels) :
            start_shift((cols - 1) * channels),
            row_step(cols * channels),
            col_step(-channels){ }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate90 {
public:
    Rotate90(int rows, int cols, int channels) :
            start_shift((rows - 1) * channels),
            row_step(-channels),
            col_step(rows * channels) { }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate90Mirror {
public:
    Rotate90Mirror(int rows, int cols, int channels) :
            start_shift(0),
            row_step(channels),
            col_step(rows * channels) { }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate180 {
public:
    Rotate180(int rows, int cols, int channels) :
            start_shift((rows * cols - 1) * channels),
            row_step(-cols * channels),
            col_step(-channels) { }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate180Mirror {
public:
    Rotate180Mirror(int rows, int cols, int channels) :
            start_shift(((rows - 1) * cols) * channels),
            row_step(-cols * channels),
            col_step(channels) { }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate270 {
public:
    Rotate270(int rows, int cols, int channels) :
            start_shift(rows * (cols - 1) * channels),
            row_step(channels),
            col_step(-rows * channels) { }

    const int start_shift;
    const int row_step;
    const int col_step;
};

class Rotate270Mirror {
public:
    Rotate270Mirror(int rows, int cols, int channels) :
            start_shift((rows * cols - 1) * channels),
            row_step(-channels),
            col_step(-rows * channels) { }

    const int start_shift;
    const int row_step;
    const int col_step;
};

bool yuv_to_bgr(ConvertParameter& cp){
    if (!cp.yuv || ! cp.bgr || (cp.h % g_block_length) || (cp.w % g_block_length) || cp.w<8 || cp.h< 8) {
        return false;
    }

    unsigned char* buffer = new (std::nothrow) unsigned char[cp.h * cp.w * 3];
    bool ret = decode_yuv_to_bgr(cp.yuv, buffer, cp.h, cp.w);
    if (!ret) {
        delete[] buffer;
        return ret;
    }

    switch (cp.rotate) {
        case 0:
            if (cp.is_mirror) {
                ret = rotate_bgr<Rotate0Mirror>(buffer, cp.bgr, cp.h, cp.w);
            }
            else {
                ret = rotate_bgr<Rotate0>(buffer, cp.bgr, cp.h, cp.w);
            }
            break;
        case 90:
            if (cp.is_mirror) {
                ret = rotate_bgr<Rotate90Mirror>(buffer, cp.bgr, cp.h, cp.w);
            }
            else {
                ret = rotate_bgr<Rotate90>(buffer, cp.bgr, cp.h, cp.w);
            }
            break;
        case 180:
            if (cp.is_mirror) {
                ret = rotate_bgr<Rotate180Mirror>(buffer, cp.bgr, cp.h, cp.w);
            }
            else {
                ret = rotate_bgr<Rotate180>(buffer, cp.bgr, cp.h, cp.w);
            }
            break;
        case 270:
            if (cp.is_mirror) {
                ret = rotate_bgr<Rotate270Mirror>(buffer, cp.bgr, cp.h, cp.w);
            }
            else {
                ret = rotate_bgr<Rotate270>(buffer, cp.bgr, cp.h, cp.w);
            }
            break;
        default:
            ret = false;
            break;
    }

    delete[] buffer;
    return ret;
}

G_NAMESPACE1_END()



const unsigned int g_block_length = 16;
const unsigned int g_block_half_length = g_block_length / 2;

void decode_yuv_8_block(unsigned char* y, unsigned char* uv, unsigned char* bgr,
                        int w, int row_step, int col_step) {
    unsigned char* y0 = y;
    unsigned char* y1 = y + w;
    unsigned char* uv0 = uv;
    int double_w = w<<1;
    int double_row_step = row_step<<1;

    unsigned char* bgr_0 = bgr;
    unsigned char* bgr_1 = bgr_0 + row_step;

    int Y00, Y01, Y10, Y11;
    int V, U;
    int tR, tG, tB;

    for (int r=0; r < g_block_half_length; ++r) {
        for (int c=0; c < g_block_half_length; ++c) {
            V = *uv0++;
            U = *uv0++;
            Y00 = Y_TABLE[*y0++];
            Y01 = Y_TABLE[*y0++];
            Y10 = Y_TABLE[*y1++];
            Y11 = Y_TABLE[*y1++];
            tB = BU_TABLE[U];
            tG = GV_TABLE[V] + GU_TABLE[U];
            tR = RV_TABLE[V];

            store_pixel(bgr_0, Y00+tR, Y00+tG, Y00+tB);
            bgr_0 += col_step;
            store_pixel(bgr_0, Y01+tR, Y01+tG, Y01+tB);
            bgr_0 += col_step;
            store_pixel(bgr_1, Y10+tR, Y10+tG, Y10+tB);
            bgr_1 += col_step;
            store_pixel(bgr_1, Y11+tR, Y11+tG, Y11+tB);
            bgr_1 += col_step;
        }

        y += double_w;
        y0 = y;
        y1 = y + w;

        uv += w;
        uv0 = uv;

        bgr += double_row_step;
        bgr_0 = bgr;
        bgr_1 = bgr + row_step;
    }
}


    template<typename ratate_mirror>
    bool decode_yuv(ConvertParameter* con_param, unsigned char alpha=0xff)
    {
        int height = con_param->img_in.rows;
        int width = con_param->img_in.cols;
        unsigned char* y = con_param->img_in.data;
        unsigned char* uv = y + (width*height);
        unsigned char* bgr = con_param->img_out.data;

        if (0 != (width & 7) || width == 0 ||
            0 != (height & 7) || height == 0 ||
            !y || !bgr) {
            return false;
        }

        ratate_mirror r_m(height, width, con_param->img_out.channels);
        bgr = bgr + r_m.start_shift;

        unsigned char* y_temp;
        unsigned char* uv_temp;
        unsigned char* bgr_temp;

        int blocks_r_n  = height / g_block_length;
        int blocks_c_n = width / g_block_length;
        for (int r=0; r<blocks_r_n; ++r) {
            y_temp = y;
            uv_temp = uv;
            bgr_temp = bgr;

            for (int c=0; c<blocks_c_n; ++c) {
                decode_yuv_8_block(y_temp, uv_temp, bgr_temp, width, r_m.row_step, r_m.col_step);
                y_temp += g_block_length;
                uv_temp += g_block_length;
                bgr_temp += g_block_length * r_m.col_step;
            }
            y += g_block_length * width;
            uv += g_block_half_length * width;
            bgr += g_block_length * r_m.row_step;
        }
        return true;
    }

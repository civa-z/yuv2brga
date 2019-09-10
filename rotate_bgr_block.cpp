


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
void rotate_bgr(unsigned char* src, unsigned char* dst, int h, int w) {
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
}

template void rotate_bgr<Rotate90Mirror> (unsigned char* src, unsigned char* dst, int h, int w);


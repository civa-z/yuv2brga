


template<typename ratate_mirror>
void rotate_bgr_base(unsigned char* src, unsigned char* dst, int h, int w) {
    ratate_mirror r_m(h, w, 3);
    dst = dst + r_m.start_shift;
    unsigned char* dst_temp;
    for (int i = 0; i < h; ++i) {
        dst_temp = dst;
        for (int j = 0; j < w; ++j) {
            *dst_temp = *src++;
            *(dst_temp+1) = *src++;
            *(dst_temp+2) = *src++;
            dst_temp+=r_m.col_step;
        }
        dst += r_m.row_step;
    }
}

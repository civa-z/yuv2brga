
#ifndef __YUV2BGRA_WITH_CONVERT_HPP__
#define __YUV2BGRA_WITH_CONVERT_HPP__

typedef struct {
    unsigned int   type;
    unsigned int   rows;
    unsigned int   cols;
    unsigned int   channels;
    unsigned char* data;
} ImageInfo;

typedef struct {
    ImageInfo img_in;
    ImageInfo img_out;
    int       rotate;
    int       is_mirror;
} ConvertParameter;

bool nv21_to_bgr(ConvertParameter* con_param);

#endif
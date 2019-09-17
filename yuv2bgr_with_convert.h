/***************************************************************************
*
* Copyright (c) 2019 Baidu.com, Inc. All Rights Reserved
*
**************************************************************************/

/*
 * zhangwenming02
 * 20190715
 *
 * */

#pragma once

#include "utils/src/nsab.h"

G_NAMESPACE1_BEGIN(g_sdk_namespace)

typedef struct {
    unsigned char* yuv;
    unsigned char* bgr;
    int h;
    int w;
    int rotate;
    bool is_mirror;
} ConvertParameter;

bool yuv_to_bgr(ConvertParameter& con_param);

G_NAMESPACE1_END()


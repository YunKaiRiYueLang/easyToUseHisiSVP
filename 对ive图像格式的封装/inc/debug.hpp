/**
 * @file debug.hpp 
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-15
 * 
 * @copyright Copyright (c) 2021
 * 
 * delete error(msg)
 */

#pragma once

// #include"sample_assist.h"

// #define CV_IMWRITE
#define DEBUG

#ifdef DEBUG
// #define error(msg) printf("\033[31m %s \n\033[0m", msg)
#define errorCode(msg, index) printf("\033[31m function:%s line:%d %s code:%x \n\033[0m", __FUNCTION__, __LINE__, msg, index)

#ifdef CV_IMWRITE
#define cvwrite(path, img)      \
    {                           \
        cv::imwrite(path, img); \
    }
#endif

#else
// #define error(msg)
#define errorCode(msg, index)

#ifndef CV_IMWRITE
#define cvwrite(path, img)
#endif

#endif

#define GET_TIME(time, code)                         \
    {                                                \
        (time) = HI_GetTickFrequency();              \
        code(time) = HI_GetTickFrequency() - (time); \
    }

#define CHECK_MAT(img, msg, ret) \
    if (img.data == NULL)        \
    {                            \
        printf("%s\n", msg);     \
        return ret;              \
    }

#define CHECK_HISIIMAGE(src, w, h)                           \
    if (src.iveImg.u32Height > h || src.iveImg.u32Width > w) \
    {                                                        \
        errorCode("image size is inappropriate", 0);         \
    }
    
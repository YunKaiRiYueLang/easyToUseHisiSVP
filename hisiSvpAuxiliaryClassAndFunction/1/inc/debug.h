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
// #define message(msg) printf("%s\n", msg);
// #define message(msg)

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

#define CHECK_HISI_FUNCTION(msg, ret) \
    if (0 != ret)                     \
    {                                 \
        errorCode(msg, ret);          \
        return ret;                   \
    }

#define BLOCK_IVE_FUNCTION(condition, handle)         \
    if (condition)                                    \
    {                                                 \
        HI_BOOL finish = (HI_BOOL)0;                  \
        HI_BOOL block = (HI_BOOL)1;                   \
        do                                            \
        {                                             \
            HI_MPI_IVE_Query(handle, &finish, block); \
        } while (!finish);                            \
    }

#define BLOCK_IVE_FUNCTION_RET(condition, handle, ret)                      \
    if (condition)                                                          \
    {                                                                       \
        HI_BOOL finish = (HI_BOOL)0;                                        \
        HI_BOOL block = (HI_BOOL)1;                                         \
        do                                                                  \
        {                                                                   \
            int ive_query_ret = HI_MPI_IVE_Query(handle, &finish, block);   \
            CHECK_IVE_FUNCTION_RET("HI_MPI_IVE_Query", ive_query_ret, ret); \
        } while (!finish);                                                  \
    }

#define CHECK_IVE_FUNCTION(name, code) \
    if (0 != code)                     \
    {                                  \
        errorCode(name, code);         \
        return;                        \
    }

#define CHECK_IVE_FUNCTION_RET(name, code, ret) \
    if (0 != code)                              \
    {                                           \
        errorCode(name, code);                  \
        return ret;                             \
    }

#define CHECK0_RET(str, code) \
    if (0 != code)            \
    {                         \
        errorCode(str, code); \
        return code;          \
    }

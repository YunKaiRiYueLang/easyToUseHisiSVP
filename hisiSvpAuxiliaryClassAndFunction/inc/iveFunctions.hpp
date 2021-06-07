/**
 * @file iveFunctions.hpp
 * @author wangbianjiang (1510627880@qq.com)
 * @brief 
 * @version 0.1
 * @date 2021-2-23
 * 
 * @copyright Copyright (c) 2021
 *  
 * do not malloc memory
 */

#pragma once
#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_ive.h"

#include "hisiImage.h"
#include "debug.h"

bool iveDMA2(IVE_DATA_S &src, IVE_DATA_S &dst, int mode, int needBlock);
void theChosenGauss(hisiImage &src, hisiImage &dst);
void iveDMA(hisiImage &src1, hisiImage &dst, int mode, int needBlock);
bool iveAnd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, int needBlock);
bool IVEAnd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, HI_BOOL needblock);
void iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const int bInstant);
bool iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const HI_BOOL bInstant);
void iveSub(const hisiImage &hisrc1, const hisiImage &hisrc2, hisiImage &hidst, int mode, int needBlock);
void iveSobel(const hisiImage &src, hisiImage &dstH, hisiImage &dstV, signed char mask[25], int mode, int needblock);
void iveSobel(const hisiImage &src, hisiImage &dstH, hisiImage &dstV, IVE_SOBEL_CTRL_S &ctrl, HI_BOOL needblock);
void iveThresh(const hisiImage &src, hisiImage &dst, unsigned char thrV, unsigned char minV, unsigned char maxV, int mode, int needBlock);
void iveThreshS16(const hisiImage &src, hisiImage &dst, IVE_THRESH_S16_CTRL_S &ctrl, HI_BOOL needblock);
void iveFilter(const hisiImage &hisrc, hisiImage &hidst, signed char mask[25], unsigned char norm, int needBlock);
void iveOrdStatFilter(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock);
bool iveCSC(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock);
bool iveInteg(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock);
bool ive16BitTo8Bit(const hisiImage &hisisrc, hisiImage &hisidst, int mode, int needblock, unsigned short u16Denominator = 1, unsigned char u8Numerator = 1, signed char s8bias = 0);
#define CHECK_IVE_IMAGE_SIZE_RET(w, h, minw, minh, maxw, maxh, false) \
    if (w < minw || w > maxw || h < minh || h > maxh)                 \
    {                                                                 \
        errorCode("image size transcend", 0);                         \
        return false;                                                 \
    }

#define CHECK_STRIDE(stride, align, false)                   \
    if (stride % align != 0)                                 \
    {                                                        \
        errorCode("csc image stride must align with 16", 0); \
    }

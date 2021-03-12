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
#include "debug.hpp"
#include "hisiImage.h"

#define CHECK_IVE_FUNCTION(name, code) \
    if (0 != code)                     \
    {                                  \
        errorCode(name, code);         \
        return;                        \
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

namespace eive
{
    void IVEAnd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, HI_BOOL needblock)
    {

        IVE_HANDLE handle;
        IVE_IMAGE_S ivesrc1 = src1.getIVEImage();
        IVE_IMAGE_S ivesrc2 = src2.getIVEImage();
        IVE_IMAGE_S ivesrc3 = dst.getIVEImage();
        int s32ret = HI_MPI_IVE_And(&handle, &ivesrc1, &ivesrc2, &ivesrc3, needblock);
        if (0 != s32ret)
        {
            printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
            return;
        }
        if (needblock)
        {
            HI_BOOL finish;
            HI_BOOL block = HI_TRUE;
            do
            {
                s32ret = HI_MPI_IVE_Query(handle, &finish, block);
                if (0 != s32ret)
                {
                    printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
                    return;
                }
            } while (!finish);
        }
    }
    void iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const HI_BOOL bInstant)
    {
        IVE_ADD_CTRL_S pstAddCtrl;
        pstAddCtrl.u0q16X = x;
        pstAddCtrl.u0q16Y = y;
        IVE_HANDLE handle;
        IVE_IMAGE_S img1 = src1.getIVEImage();
        IVE_IMAGE_S img2 = src2.getIVEImage();
        IVE_IMAGE_S img3 = dst.getIVEImage();
        int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, bInstant);
        if (0 != s32ret)
        {
            printf("HI_MPI_IVE_Add error occured,code:%x\n", s32ret);
            return;
        }
        if (bInstant)
        {
            HI_BOOL finish;
            HI_BOOL block = HI_TRUE;
            do
            {
                s32ret = HI_MPI_IVE_Query(handle, &finish, block);
                if (0 != s32ret)
                {
                    printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
                    return;
                }
            } while (!finish);
        }
    }
    void iveSub(const hisiImage &hisrc1, const hisiImage &hisrc2, hisiImage &hidst, int mode, int needBlock)
    {
        IVE_IMAGE_S src1 = hisrc1.getIVEImage();
        IVE_IMAGE_S src2 = hisrc2.getIVEImage();
        IVE_IMAGE_S dst = hidst.getIVEImage();
        ctrl.enMode = (IVE_SUB_MODE_E)mode;
        IVE_HANDLE subHandle;
        int s32Ret = HI_MPI_IVE_Sub(&subHandle, &src1, &src2, &dst, &ctrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_Sub", s32Ret);
        BLOCK_IVE_FUNCTION(needBlock, subHandle);
    }
    // 不需要的输出可以是空图像
    // format,src: u8c1;dstH,dstV:s16c1
    //
    void iveSobel(const hisiImage &src, hisiImage &dstH, hisiImage &dstV, IVE_SOBEL_CTRL_S &ctrl, HI_BOOL needblock)
    {
        IVE_SRC_IMAGE_S stSrc = src.getIVEImage();
        IVE_DST_IMAGE_S stDstH = dstH.getIVEImage();
        IVE_DST_IMAGE_S stDstV = dstV.getIVEImage();
        IVE_HANDLE handle;
        int s32Result = HI_MPI_IVE_Sobel(&handle, &stSrc, IVE_SOBEL_OUT_CTRL_VER != ctrl.enOutCtrl ? &stDstH : HI_NULL, IVE_SOBEL_OUT_CTRL_HOR != ctrl.enOutCtrl ? &stDstV : HI_NULL, &ctrl, needblock);
        if (0 != s32Result)
        {
            printf("line%d HI_MPI_IVE_Sobel:%x\n", __LINE__, s32Result);
            return;
        }
        if (needblock)
        {
            HI_BOOL finish = (HI_BOOL)0;
            HI_BOOL block = (HI_BOOL)1;
            do
            {
                HI_MPI_IVE_Query(handle, &finish, block);
                // printf("延时\n");
                // usleep(5);
            } while (!finish);
        }
    }
    void iveThreshS16(const hisiImage &src, hisiImage &dst, IVE_THRESH_S16_CTRL_S &ctrl, HI_BOOL needblock)
    {
        IVE_IMAGE_S stSrc = src.getIVEImage();
        IVE_IMAGE_S stDst = dst.getIVEImage();
        IVE_HANDLE handle;
        int s32Result = HI_MPI_IVE_Thresh_S16(&handle, &stSrc, &stDst, &ctrl, needblock);
        if (0 != s32Result)
        {
            printf("\033[31m line %d HI_MPI_IVE_Sobel %x \n\033[0m", __LINE__, s32Result);
            return;
        }
        if (needblock)
        {
            HI_BOOL finish = (HI_BOOL)0;
            HI_BOOL block = (HI_BOOL)1;
            do
            {
                HI_MPI_IVE_Query(handle, &finish, block);
                // printf("延时\n");
                // usleep(5);
            } while (!finish);
        }
    }

    /**
     * @brief 根据不同的模板系数，实现不同的滤波
     * 
     */
    void iveFilter(const hisiImage &hisrc, hisiImage &hidst, signed char mask[25], unsigned char norm, int needBlock)
    {
        IVE_IMAGE_S src = hisrc.getIVEImage();
        IVE_IMAGE_S dst = hidst.getIVEImage();
        IVE_HANDLE filterHandle;
        IVE_FILTER_CTRL_S ctrl;
        memcpy(ctrl.as8Mask, mask, 25);
        ctrl.u8Norm = norm;
        int s32Result = HI_MPI_IVE_Filter(&filterHandle, &src, &dst, &ctrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_Filter", s32Result);
        BLOCK_IVE_FUNCTION(needBlock, filterHandle);
    }

    /**
     * @brief 
     * 
     * @param hisrc 
     * @param hidst 
     * @param mode 
     * @param needBlock 
     */
    void iveOrdStatFilter(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock)
    {
        IVE_IMAGE_S src = hisrc.getIVEImage();
        IVE_IMAGE_S dst = hidst.getIVEImage();
        IVE_ORD_STAT_FILTER_CTRL_S ctrl;
        ctrl.enMode = (IVE_ORD_STAT_FILTER_MODE_E)mode;
        IVE_HANDLE osfHandle;
        int s32Ret = HI_MPI_IVE_OrdStatFilter(&osfHandle, &src, &dst, &ctrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_OrdStatFilter", s32Ret);
        BLOCK_IVE_FUNCTION(needBlock, osfHandle);
    }
}

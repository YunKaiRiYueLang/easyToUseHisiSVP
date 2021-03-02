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
}

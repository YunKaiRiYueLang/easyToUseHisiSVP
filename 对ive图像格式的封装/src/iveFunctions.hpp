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
    // IVE_DMA_MODE_DIRECT_COPY.并且只把一个图像从一处拷贝到另一处
    void iveDMA(hisiImage &src1, hisiImage &dst, int mode, int needBlock)
    {
        IVE_HANDLE dmaHandle;
        IVE_DATA_S ivesrc1 = src1.getIVEData();
        IVE_DATA_S ivedst = dst.getIVEData();
        IVE_DMA_CTRL_S ctrl;
        memset(&ctrl, 0, sizeof(IVE_DMA_CTRL_S));
        ctrl.enMode = (IVE_DMA_MODE_E)mode;
        int s32ret = HI_MPI_IVE_DMA(&dmaHandle, &ivesrc1, &ivedst, &ctrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_DMA", s32ret);
        BLOCK_IVE_FUNCTION(needBlock, dmaHandle);
    }

    void iveAnd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, int needBlock)
    {

        IVE_HANDLE handle;
        IVE_IMAGE_S ivesrc1 = src1.getIVEImage();
        IVE_IMAGE_S ivesrc2 = src2.getIVEImage();
        IVE_IMAGE_S ivesrc3 = dst.getIVEImage();
        int s32ret = HI_MPI_IVE_And(&handle, &ivesrc1, &ivesrc2, &ivesrc3, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_And", s32ret);
        BLOCK_IVE_FUNCTION(needBlock, handle);
    }

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
    void iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const int bInstant)
    {
        if (src1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || src2.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || dst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
        {
            errorCode("iveAdd input image format error", 0);
            return;
        }
        IVE_ADD_CTRL_S pstAddCtrl;
        pstAddCtrl.u0q16X = x;
        pstAddCtrl.u0q16Y = y;
        IVE_HANDLE handle;
        IVE_IMAGE_S img1 = src1.getIVEImage();
        IVE_IMAGE_S img2 = src2.getIVEImage();
        IVE_IMAGE_S img3 = dst.getIVEImage();
        int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, (HI_BOOL)bInstant);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_Add", s32ret);
        BLOCK_IVE_FUNCTION(bInstant, handle);
    }
    void iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const HI_BOOL bInstant)
    {
        if (src1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || src2.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || dst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
        {
            errorCode("iveAdd input image format error", 0);
            return;
        }
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
        if (hisrc1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || hisrc2.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
        {
            errorCode("iveSub input image format error", 0);
            return;
        }
        if (mode == 0 && hidst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
        {
            errorCode("iveSub dst image format error", 0);
            return;
        }
        if (mode == 1 && hidst.iveImg.enType != IVE_IMAGE_TYPE_S8C1)
        {
            errorCode("iveSub dst image format error", 0);
            return;
        }

        IVE_IMAGE_S src1 = hisrc1.getIVEImage();
        IVE_IMAGE_S src2 = hisrc2.getIVEImage();
        IVE_IMAGE_S dst = hidst.getIVEImage();
        IVE_SUB_CTRL_S ctrl;
        ctrl.enMode = (IVE_SUB_MODE_E)mode;
        IVE_HANDLE subHandle;
        int s32Ret = HI_MPI_IVE_Sub(&subHandle, &src1, &src2, &dst, &ctrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_Sub", s32Ret);
        BLOCK_IVE_FUNCTION(needBlock, subHandle);
    }
    // 不需要的输出可以是空图像
    // format,src: u8c1;dstH,dstV:s16c1
    //
    void iveSobel(const hisiImage &src, hisiImage &dstH, hisiImage &dstV, signed char mask[25], int mode, int needblock)
    {
        CHECK_HISIIMAGE(src, 1920, 1024);
        if (src.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || dstH.iveImg.enType != IVE_IMAGE_TYPE_S16C1 || dstV.iveImg.enType != IVE_IMAGE_TYPE_S16C1)
        {
            errorCode("iveSobel input image format error", 0);
            return;
        }
        IVE_IMAGE_S stSrc = src.getIVEImage();
        IVE_IMAGE_S stDstH = dstH.getIVEImage();
        IVE_IMAGE_S stDstV = dstV.getIVEImage();
        IVE_HANDLE iveHandle;
        IVE_SOBEL_CTRL_S ctrl;
        memcpy(ctrl.as8Mask, mask, sizeof(HI_S8) * 25);
        ctrl.enOutCtrl = (IVE_SOBEL_OUT_CTRL_E)mode;
        int s32Result = HI_MPI_IVE_Sobel(&iveHandle, &stSrc, IVE_SOBEL_OUT_CTRL_VER != ctrl.enOutCtrl ? &stDstH : HI_NULL, IVE_SOBEL_OUT_CTRL_HOR != ctrl.enOutCtrl ? &stDstV : HI_NULL, &ctrl, (HI_BOOL)needblock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_Sobel", s32Result);
        BLOCK_IVE_FUNCTION(needblock, iveHandle);
    }
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

    /**
     * @brief 
     * 
     * @param src 
     * @param dst   
     * @param thrV 
     * @param minV 
     * @param maxV 
     * @param mode 
     * @param needBlock 
     */
    void iveThresh(const hisiImage &src, hisiImage &dst, unsigned char thrV, unsigned char minV, unsigned char maxV, int mode, int needBlock)
    {
        IVE_THRESH_CTRL_S threshCtrl;
        threshCtrl.enMode = (IVE_THRESH_MODE_E)mode;
        threshCtrl.u8LowThr = thrV;
        threshCtrl.u8MinVal = minV;
        threshCtrl.u8MaxVal = maxV;
        IVE_HANDLE threshHandle;
        IVE_IMAGE_S iveSrc = src.getIVEImage();
        IVE_IMAGE_S iveDst = dst.getIVEImage();
        int s32ret = HI_MPI_IVE_Thresh(&threshHandle, &iveSrc, &iveDst, &threshCtrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION("HI_MPI_IVE_Thresh", s32ret);
        BLOCK_IVE_FUNCTION(needBlock, threshHandle);
    }

    /*控制参数太多，先保留ive类型的控制参数*/
    void iveThreshS16(const hisiImage &src, hisiImage &dst, IVE_THRESH_S16_CTRL_S &ctrl, HI_BOOL needblock)
    {
        if (src.iveImg.enType != IVE_IMAGE_TYPE_S16C1)
        {
            errorCode("iveThreshS16 input image format error\n input src must be IVE_IMAGE_TYPE_S16C1", 0);
            return;
        }
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

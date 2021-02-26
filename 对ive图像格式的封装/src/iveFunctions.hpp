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
}

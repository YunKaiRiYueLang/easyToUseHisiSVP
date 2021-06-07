#ifndef __IVE_ADD_H__
#define __IVE_ADD_H__
#include "hisiImage.h"
#include <algorithm>
#include <cstring>
using namespace std;

/**
 * 
 * 
*/
#define DEBUG
void iveAdd(const hisiImage &src1, const hisiImage &src2, const unsigned short x, const unsigned short y, const HI_BOOL wait, hisiImage &dst)
{
    IVE_ADD_CTRL_S pstAddCtrl;
    pstAddCtrl.u0q16X = x;
    pstAddCtrl.u0q16Y = y;
    IVE_HANDLE handle;
#ifdef DEBUG
    double dTime = (HI_DOUBLE)HI_GetTickCount();
#endif
    IVE_IMAGE_S img1 = src1.getIVEImage();
    IVE_IMAGE_S img2 = src2.getIVEImage();
    IVE_IMAGE_S img3 = dst.getIVEImage();

    int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, wait);
    if (0 != s32ret)
    {
        printf("HI_MPI_IVE_Add error occured,code:%x\n", s32ret);
        return;
    }
    if (wait)
    {
        HI_BOOL finish;
        HI_BOOL block = HI_TRUE;
        do
        {
            HI_MPI_IVE_Query(handle, &finish, block);
            // printf("延时\n");
            // usleep(5);
        } while (!finish);
    }
#ifdef DEBUG
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) / 1000000000;
    printf("HI_MPI_IVE_Add 1920*1024 time:%f\n", dTime);
    writeGrayBmpImage("/mnt/bjwang/iveAdd.bmp", dst);
#endif
};

void iveAdd1(const hisiImage &src1, const hisiImage &src2, const unsigned short x, const unsigned short y, const HI_BOOL wait, hisiImage &dst)
{
    IVE_ADD_CTRL_S pstAddCtrl;
    pstAddCtrl.u0q16X = x;
    pstAddCtrl.u0q16Y = y;
    IVE_HANDLE handle;
#ifdef DEBUG
    double dTime = (HI_DOUBLE)HI_GetTickCount();
#endif
    IVE_IMAGE_S img1 = src1.getIVEImage();
    IVE_IMAGE_S img2 = src2.getIVEImage();
    IVE_IMAGE_S img3 = dst.getIVEImage();

    int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, wait);
    if (0 != s32ret)
    {
        printf("HI_MPI_IVE_Add error occured,code:%x\n", s32ret);
        return;
    }
    if (wait)
    {
        HI_BOOL finish;
        HI_BOOL block = HI_TRUE;
        do
        {
            HI_MPI_IVE_Query(handle, &finish, block);
            // printf("延时\n");
            // usleep(5);
        } while (!finish);
    }
#ifdef DEBUG
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) / 1000000000;
    printf("HI_MPI_IVE_Add 1920*1024 time:%f\n", dTime);
    writeGrayBmpImage("/mnt/bjwang/iveAddtest.bmp", dst);
#endif
}


void iveAdd2(const hisiImage &src1, const hisiImage &src2, const unsigned short x, const unsigned short y, const HI_BOOL wait, hisiImage &dst)
{
    IVE_ADD_CTRL_S pstAddCtrl;
    pstAddCtrl.u0q16X = x;
    pstAddCtrl.u0q16Y = y;
    IVE_HANDLE handle;
#ifdef DEBUG
    double dTime = (HI_DOUBLE)HI_GetTickCount();
#endif
    IVE_IMAGE_S img1 = src1.getIVEImage();
    IVE_IMAGE_S img2 = src2.getIVEImage();
    IVE_IMAGE_S img3 = dst.getIVEImage();

    int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, wait);
    if (0 != s32ret)
    {
        printf("HI_MPI_IVE_Add error occured,code:%x\n", s32ret);
        return;
    }
    if (wait)
    {
        HI_BOOL finish;
        HI_BOOL block = HI_TRUE;
        do
        {
            HI_MPI_IVE_Query(handle, &finish, block);
            // printf("延时\n");
            // usleep(5);
        } while (!finish);
    }
#ifdef DEBUG
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) / 1000000000;
    printf("HI_MPI_IVE_Add 1920*1024 time:%f\n", dTime);
    writeGrayBmpImage("/mnt/bjwang/iveAddtest2.bmp", dst);
#endif
}

void iveAddSample()
{
    hisiImage src1;
    hisiImage src2;
    src1.imread("./test.bmp");
    src2.imread("./test.bmp");
    hisiImage dst;
    dst.create(src1);
    printf("1920*1024 两个图像相加");
    iveAdd(src1, src2, 30000, 65535 - 10000, HI_TRUE, dst); // 相加溢出65535 相当于有一个亮度增益
    printf("分辨率宽*高 不符合要求，但是跨度*高符合要求\n");
    hisiImage wh40_128stride128;
    hisiImage wh40_128stride128_2;
    wh40_128stride128.create(40, 128, 128);
    wh40_128stride128_2.create(40, 128, 128);
    fill((unsigned char *)wh40_128stride128.iveImg.au64VirAddr[0], (unsigned char *)wh40_128stride128.iveImg.au64VirAddr[0] + 128 * 128, 128);
    hisiImage wh40_128stride128_d;
    wh40_128stride128_d.create(40, 128, 128);
    iveAdd1(wh40_128stride128, wh40_128stride128_2, 30000, 65535 - 10000, HI_TRUE, wh40_128stride128_d);
    //实验证明，会报错，参数（宽度）不符合要求。
   
    hisiImage wh64_128stride128;
    hisiImage wh64_128stride128_2;
    wh64_128stride128.create(64, 128, 128);
    wh64_128stride128_2.create(64, 128, 128);
    fill((unsigned char *)wh64_128stride128.iveImg.au64VirAddr[0], (unsigned char *)wh64_128stride128.iveImg.au64VirAddr[0] + 128 * 128, 128);
    hisiImage wh64_128stride128_d;
    wh64_128stride128_d.create(64, 128, 128);
    iveAdd1(wh64_128stride128, wh64_128stride128_2, 30000, 65535 - 10000, HI_TRUE, wh64_128stride128_d);
    //改为64*128后正常运行，图像变小后，运行时间也会缩短。

    hisiImage wh103_205stride128;
    hisiImage wh103_205stride128_2;
    wh103_205stride128.create(114, 72, 128);
    wh103_205stride128_2.create(114, 72, 128);
    fill((unsigned char *)wh103_205stride128.iveImg.au64VirAddr[0], (unsigned char *)wh103_205stride128.iveImg.au64VirAddr[0] + 128 * 128, 128);
    hisiImage wh103_205stride128_d;
    wh103_205stride128_d.create(114, 72, 128);
    iveAdd2(wh103_205stride128, wh103_205stride128_2, 30000, 65535 - 10000, HI_TRUE, wh103_205stride128_d);
    //宽高需要为偶数 。奇数不行。

}

#endif
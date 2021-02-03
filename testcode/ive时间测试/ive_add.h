#ifndef __IVE_ADD_H__
#define __IVE_ADD_H__
#include"hisiImage.h"


/**
 * 
 * 
*/
#define DEBUG
void iveAdd(const hisiImage &src1,const hisiImage&src2,const unsigned short x,const unsigned short y,const HI_BOOL wait ,hisiImage &dst){
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

    int s32ret= HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, wait);
    if(0!=s32ret){
        printf("HI_MPI_IVE_Add error occured,code:%x\n", s32ret);
        return;
    }
    if(wait){
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
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) /1000000000;
    printf("HI_MPI_IVE_Add 1920*1024 time:%f\n", dTime);
    writeGrayBmpImage("/mnt/bjwang/iveAdd.bmp", dst);
    #endif
    
}

void iveAddSample(){
    hisiImage src1;
    hisiImage src2;
    src1.imread("./test.bmp");
    src2.imread("./test.bmp");
    hisiImage dst;
    dst.create(src1);
     
    iveAdd(src1, src2, 30000, 65535 -10000, HI_TRUE,dst); // 相加溢出65535 相当于有一个亮度增益

}

#endif
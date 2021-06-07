#pragma once
#include"solohisiImage.h"

#include"hi_common.h"
#include"unistd.h"

void doIVEAnd(const hisiImage& src1,const hisiImage&src2,hisiImage&src3){

    IVE_HANDLE handle;
    IVE_IMAGE_S ivesrc1 = src1.getIVEImage();
    IVE_IMAGE_S ivesrc2 = src2.getIVEImage();
    IVE_IMAGE_S ivesrc3 = src2.getIVEImage();

    double dTime = (HI_DOUBLE)HI_GetTickCount();
    int s32ret= HI_MPI_IVE_And(&handle, &ivesrc1, &ivesrc2, &ivesrc3, HI_TRUE);
    if(0!=s32ret){
        printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
    }
    sleep(1);
    HI_BOOL finish;
    HI_BOOL block = HI_TRUE;
    do
    {
        HI_MPI_IVE_Query(handle, &finish, block);
        // printf("延时\n");
        // usleep(5);
    } while (!finish);
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) /1000000000;
    printf("HI_MPI_IVE_And time:%f\n", dTime);
    printf("end\n");

}

void iveAnd(){
    hisiImage src1;
    hisiImage src2;
    hisiImage src3;
    src1.imread("/mnt/bjwang/f11024.bmp");
    src2.imread("/mnt/bjwang/f21024.bmp");
    src3.create(src1);
    doIVEAnd(src1,src2,src3);
    writeGrayBmpImage("/mnt/bjwang/iveAndImage.bmp", src3);
}
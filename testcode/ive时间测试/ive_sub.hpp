#pragma once
#include "hisiImage.h"
#include"unistd.h"

void SubSample(hisiImage &src1, hisiImage &src2, hisiImage &src3, IVE_SUB_CTRL_S &mode)
{
    IVE_HANDLE handle;
    IVE_IMAGE_S ivesrc1 = src1.getIVEImage();
    IVE_IMAGE_S ivesrc2 = src2.getIVEImage();
    IVE_IMAGE_S ivesrc3 = src3.getIVEImage();

    double t1 = HI_GetTickCount();
    int s32ret = HI_MPI_IVE_Sub(&handle, &ivesrc1, &ivesrc2, &ivesrc3, &mode, HI_TRUE);
    if (0 != s32ret)
    {
        printf("line:%d,HI_MPI_IVE_Sub error:%x\n", __LINE__,s32ret);
        return;
    }
    // sleep(1);
    HI_BOOL finish;
    do
    {
        HI_MPI_IVE_Query(handle, &finish, HI_TRUE);
    } while (!finish);
    t1 = (HI_GetTickCount() - t1) / 1000000000;
    printf("HI_MPI_IVE_Sub time:%f\n", t1);
    
}

void SubSampleABS()
{
    hisiImage src1;
    hisiImage src2;
    src1.imread("/mnt/bjwang/f11024.bmp");
    src2.imread("/mnt/bjwang/f21024.bmp");
    hisiImage src3;
    src3.create(src1);
    IVE_SUB_CTRL_S ctrl;
    ctrl.enMode = IVE_SUB_MODE_ABS;
    SubSample(src1, src2, src3, ctrl);
}
void SubSampleSHIFT(){
    hisiImage src1;
    hisiImage src2;
    src1.imread("/mnt/bjwang/f11024.bmp");
    src2.imread("/mnt/bjwang/f21024.bmp");
    hisiImage src3;
    src3.create(src1,IVE_IMAGE_TYPE_S8C1);
    IVE_SUB_CTRL_S ctrl;
    ctrl.enMode = IVE_SUB_MODE_SHIFT;
    SubSample(src1, src2, src3, ctrl);
    writeGrayBmpImage("/mnt/bjwang/ive_sub_shift.bmp", src3);
} 

void ive_sub()
{
    SubSampleABS();
    SubSampleSHIFT();
}
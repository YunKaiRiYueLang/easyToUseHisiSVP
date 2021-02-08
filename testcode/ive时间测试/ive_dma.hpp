#pragma once
#include "hisiImage.h"

HI_VOID DMASample(hisiImage &src1, hisiImage &dst, IVE_DMA_CTRL_S &ctrl)
{
    IVE_HANDLE handle;
    IVE_DATA_S ivesrc1=src1.getIVEData();
    IVE_DATA_S ivedst=dst.getIVEData();
    double time =(double) HI_GetTickCount();
    int32_t s32ret= HI_MPI_IVE_DMA(&handle, &ivesrc1, &ivedst, &ctrl, HI_TRUE);
    if(0!=s32ret){
        printf("line:%d,HI_MPI_IVE_DMA EOOR:%x\n", __LINE__,s32ret);
    }
    HI_BOOL finish;
    do{
        s32ret = HI_MPI_IVE_Query(handle, &finish, HI_TRUE);
        if(0!=s32ret){
        printf("line:%d,HI_MPI_IVE_Query EOOR:%x\n", __LINE__,s32ret);
        }
        printf("返回值：%x\n 结束：%d\n", s32ret,finish);
        printf("check\n");
    } while (!finish);
    time = (HI_GetTickCount()-time)/1000000000; // second;
    printf("dma time %f\n",time);
}
void DMASampleDirectCopy()
{
    hisiImage src1;
    src1.imread("/mnt/bjwang/f11024.bmp");
    hisiImage dst;
    dst.create(src1);
    IVE_DMA_CTRL_S ctrl;
    memset(&ctrl, 0, sizeof(IVE_DMA_CTRL_S));
    ctrl.enMode = IVE_DMA_MODE_DIRECT_COPY;

    DMASample(src1, dst, ctrl);
}
void ive_dma()
{
    DMASampleDirectCopy();
}
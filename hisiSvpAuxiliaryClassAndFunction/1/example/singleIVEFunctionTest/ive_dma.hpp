#pragma once
#include "solohisiImage.h"

HI_VOID DMASample(hisiImage &src1, hisiImage &dst, IVE_DMA_CTRL_S &ctrl)
{
    IVE_HANDLE handle;
    IVE_DATA_S ivesrc1 = src1.getIVEData();
    IVE_DATA_S ivedst = dst.getIVEData();
    double time = (double)HI_GetTickCount();
    int32_t s32ret = HI_MPI_IVE_DMA(&handle, &ivesrc1, &ivedst, &ctrl, HI_TRUE);
    if (0 != s32ret)
    {
        printf("line:%d,HI_MPI_IVE_DMA EOOR:%x\n", __LINE__, s32ret);
    }
    HI_BOOL finish;
    do
    {
        s32ret = HI_MPI_IVE_Query(handle, &finish, HI_TRUE);
        if (0 != s32ret)
        {
            printf("line:%d,HI_MPI_IVE_Query EOOR:%x\n", __LINE__, s32ret);
        }
        printf("返回值：%x 结束：%d\n", s32ret, finish);
        printf("check\n");
    } while (!finish);
    time = (HI_GetTickCount() - time) / 1000000000; // second;
    printf("dma time %f\n", time);
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
    //或者直接存储。
    writeGrayBmpImage("/mnt/bjwang/DebugDir/bit24.bmp", dst);
}

void DMASampleDirectCopyRoi(hisiImage &src1, hisiImage &dst)
{
    IVE_DMA_CTRL_S ctrl;
    memset(&ctrl, 0, sizeof(IVE_DMA_CTRL_S));
    ctrl.enMode = IVE_DMA_MODE_DIRECT_COPY;
    IVE_DATA_S ivesrc1 = src1.getIVEData();
    IVE_DATA_S ivedst = dst.getIVEData();
    IVE_DATA_S srcRoi;
    srcRoi.u32Height = ivesrc1.u32Height / 2;
    srcRoi.u32Width = 512;
    srcRoi.u32Stride = ivesrc1.u32Stride;
    srcRoi.u64PhyAddr = ivesrc1.u64PhyAddr;
    srcRoi.u64VirAddr = ivesrc1.u64VirAddr;
    IVE_DATA_S dstRoi;
    dstRoi.u32Height = ivedst.u32Height / 2;
    dstRoi.u32Width = 512;
    dstRoi.u32Stride = ivedst.u32Stride;
    dstRoi.u64PhyAddr = ivedst.u64PhyAddr;
    dstRoi.u64VirAddr = ivedst.u64VirAddr;
    IVE_HANDLE handle;
    int32_t s32ret = HI_MPI_IVE_DMA(&handle, &srcRoi, &dstRoi, &ctrl, HI_TRUE);
    if (0 != s32ret)
    {
        printf("line:%d,HI_MPI_IVE_DMA EOOR:%x\n", __LINE__, s32ret);
    }
    HI_BOOL finish;
    do
    {
        s32ret = HI_MPI_IVE_Query(handle, &finish, HI_TRUE);
        if (0 != s32ret)
        {
            printf("line:%d,HI_MPI_IVE_Query EOOR:%x\n", __LINE__, s32ret);
        }
        printf("返回值：%x 结束：%d\n", s32ret, finish);
        printf("check\n");
    } while (!finish);
    writeGrayBmpImage("/mnt/bjwang/DebugDir/topleft.bmp", dst); //实现只存拷贝左上角宽512高1/2原图像的图像到dst中。
    //发现新的bug，必须重启才行。
/***************************************
 * 错误的左上角dma案例
 ***************************************/
    memset((void *)dst.iveImg.au64VirAddr[0], 0, dst.iveImg.au32Stride[0] * dst.iveImg.u32Height);
    srcRoi.u32Height = ivesrc1.u32Height / 2;
    srcRoi.u32Width = 512;
    srcRoi.u32Stride = 512;
    srcRoi.u64PhyAddr = ivesrc1.u64PhyAddr;
    srcRoi.u64VirAddr = ivesrc1.u64VirAddr;
    dstRoi.u32Height = ivedst.u32Height / 2;
    dstRoi.u32Width = 512;
    dstRoi.u32Stride = 512;
    dstRoi.u64PhyAddr = ivedst.u64PhyAddr;
    dstRoi.u64VirAddr = ivedst.u64VirAddr;
    s32ret = HI_MPI_IVE_DMA(&handle, &srcRoi, &dstRoi, &ctrl, HI_TRUE);
    if (0 != s32ret)
    {
        printf("line:%d,HI_MPI_IVE_DMA EOOR:%x\n", __LINE__, s32ret);
    }
    do
    {
        s32ret = HI_MPI_IVE_Query(handle, &finish, HI_TRUE);
        if (0 != s32ret)
        {
            printf("line:%d,HI_MPI_IVE_Query EOOR:%x\n", __LINE__, s32ret);
        }
        printf("返回值：%x 结束：%d\n", s32ret, finish);
        printf("check\n");
    } while (!finish);

    writeGrayBmpImage("/mnt/bjwang/DebugDir/topleft1.bmp", dst); 
    //在已经申请的内存中，因为跨度设置的不合理。
/************************************************************
 * 复制图像中的src一小块到dst图像的相同位置。src dst类型尺寸相同
 * **********************************************************/
    memset((void *)dst.iveImg.au64VirAddr[0], 0, dst.iveImg.au32Stride[0] * dst.iveImg.u32Height);
    srcRoi.u32Height = 500;
    srcRoi.u32Width = 512;
    srcRoi.u32Stride = src1.iveImg.au32Stride[0];
    srcRoi.u64PhyAddr = src1.iveImg.au64PhyAddr[0]+src1.iveImg.au32Stride[0]*100+500;
    srcRoi.u64VirAddr = src1.iveImg.au64VirAddr[0]+src1.iveImg.au32Stride[0]*100+500;
    dstRoi.u32Height = 500;
    dstRoi.u32Width = 512;
    dstRoi.u32Stride = ivedst.u32Stride;
    dstRoi.u64PhyAddr = ivedst.u64PhyAddr+dst.iveImg.au32Stride[0]*100+500;
    dstRoi.u64VirAddr = ivedst.u64VirAddr+dst.iveImg.au32Stride[0]*100+500;
    s32ret = HI_MPI_IVE_DMA(&handle, &srcRoi, &dstRoi, &ctrl, HI_TRUE);
    if (0 != s32ret)
    {
        printf("line:%d,HI_MPI_IVE_DMA EOOR:%x\n", __LINE__, s32ret);
    }
    do
    {
        s32ret = HI_MPI_IVE_Query(handle, &finish, HI_TRUE);
        if (0 != s32ret)
        {
            printf("line:%d,HI_MPI_IVE_Query EOOR:%x\n", __LINE__, s32ret);
        }
        printf("返回值：%x 结束：%d\n", s32ret, finish);
        printf("check\n");
    } while (!finish);

    writeGrayBmpImage("/mnt/bjwang/DebugDir/shiftroi.bmp", dst); 
    //实验到这里就可以说明，就是普通意义上的dma（内存拷贝。没有其他的，
    // 因为复制的是图像块，这里确认好图像块相关数据结构的信息就好）
    printf("%s", __TIME__);
}

void ive_dma()
{

    hisiImage sourceImage;
    sourceImage.imread("/mnt/bjwang/f11024.bmp");
    hisiImage des;
    des.create(sourceImage);
    // DMASampleDirectCopy();
    DMASampleDirectCopyRoi(sourceImage, des);
}
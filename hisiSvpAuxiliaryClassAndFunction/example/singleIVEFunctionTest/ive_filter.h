#ifndef __IVE_FILTER_H__
#define __IVE_FILTER_H__

#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_ive.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "sample_assist.h"

#include "hisiImage.h"
#include "time.h"
/**
 * image type ，kernel，norm  fileName.
 **/
void FilterSample(IVE_IMAGE_TYPE_E enType, HI_S8 *ps8Mask, HI_U8 u8Norm, HI_CHAR *fileName)
{
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDst;
    IVE_FILTER_CTRL_S stCtrl;
    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S)); // 置'0';
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_FILTER_CTRL_S));
    
    stbImageData img;
    if (!readGrayBmpImage(fileName, img))
    {
        printf("无法读取图片");
        return;
    };
    printf("读图成功\n");

    int s32Result = HI_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, img.w, img.h);
    if (0 != s32Result)
    {
        printf("创建图像错误\n");
        return;
    }
    s32Result = HI_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U8C1, img.w, img.h);
    if (0 != s32Result)
    {
        printf("创建图像错误\n");
        return;
    }

    //植入图像数据到ive结构体指向的内存
    unsigned char *p=(unsigned char*)stSrc.au64VirAddr[0];
    for (int i = 0; i < stSrc.u32Height;i++){
        memcpy(p + stSrc.au32Stride[0] * i, img.data+i*img.w, img.w);
    }


    memcpy(stCtrl.as8Mask, ps8Mask, sizeof(HI_S8) * 25);
    stCtrl.u8Norm = u8Norm;

    printf("start\n");
    IVE_HANDLE handle;
    double dTime = (HI_DOUBLE)HI_GetTickCount();
    // for (int i = 0; i < 1000;i++){
     s32Result = HI_MPI_IVE_Filter(&handle, &stSrc, &stDst, &stCtrl, HI_TRUE);
    if (0 != s32Result)
    {
        printf("HI_MPI_IVE_Filter错误: %x\n",s32Result);
        return;
    }
    HI_BOOL finish;
    HI_BOOL block = HI_TRUE;
    do
    {
        HI_MPI_IVE_Query(handle, &finish, block);
        // printf("延时\n");
        // usleep(5);
    } while (!finish);
    // }
   
    // sleep(1);
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) /1000000000;
    printf("filter time:%f\n", dTime);
    printf("end\n");
    stbImageData out;
    out.w = stDst.au32Stride[0];
    out.h = stDst.u32Height;
    out.data = (unsigned char *)stDst.au64VirAddr[0];
    out.c = 1;
    writeGrayBmpImage("/mnt/bjwang/filter.bmp", out);

    IVE_MMZ_FREE(stSrc.au64PhyAddr[0], stSrc.au64VirAddr[0]);
    IVE_MMZ_FREE(stDst.au64PhyAddr[0], stDst.au64VirAddr[0]);
}
void FilterSampleY3x3()
{
    HI_CHAR *fileName = (HI_CHAR *)"./test.bmp";
    HI_S8 mask[25] = {0, 0, 0, 0, 0,
                      0, 1, 2, 1, 0,
                      0, 2, 4, 2, 0,
                      0, 1, 2, 1, 0,
                      0, 0, 0, 0, 0};
    FilterSample(IVE_IMAGE_TYPE_U8C1, mask, 4, fileName);
}
HI_VOID FilterSampleY5x5(HI_VOID){
    HI_CHAR *fileName = (HI_CHAR *)"./test.bmp";
	HI_S8 mask[25]={1,2,3,2,1,
		2,5,6,5,2,
		3,6,8,6,3,
		2,5,6,5,2,
		1,2,3,2,1};
	FilterSample(IVE_IMAGE_TYPE_U8C1,mask,7,fileName); 
}

void ive_filter()
{
    FilterSampleY3x3();
    FilterSampleY5x5();
}

#endif
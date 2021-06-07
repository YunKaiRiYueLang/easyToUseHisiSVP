#ifndef __IVE_THRESH_H__
#define __IVE_THRESH_H__

#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_ive.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"

#include "sample_assist.h"
#include"sample_define.h"


#include "solohisiImage.h"
#include "time.h"


#include"solohisiImage.h"
// 原来的代码入书写方式总是难以阅读。
void ThreshSample(IVE_THRESH_MODE_E enMode,HI_CHAR*fileName){
    /* defined variable */
	IVE_SRC_IMAGE_S stSrc;
	IVE_DST_IMAGE_S stDst;
	IVE_THRESH_CTRL_S stCtrl;
    
    memset(&stSrc,0,sizeof(IVE_SRC_IMAGE_S));
	memset(&stDst,0,sizeof(IVE_DST_IMAGE_S));
	memset(&stCtrl,0,sizeof(IVE_THRESH_CTRL_S));
    stCtrl.enMode = enMode;
	stCtrl.u8LowThr = 100;
	stCtrl.u8MinVal = 0;
	stCtrl.u8MaxVal = 255;
    
    stbImageData img;
    if (!readGrayBmpImage(fileName, img))
    {
        printf("无法读取图片");
        return;
    };
    printf("读图成功\n");

    writeGrayBmpImage("/mnt/bjwang/binaryThreshtest.bmp", img);


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


    IVE_HANDLE handle;
    double dTime = (HI_DOUBLE)HI_GetTickCount();
	s32Result = HI_MPI_IVE_Thresh(&handle,&stSrc,&stDst,&stCtrl,HI_TRUE);
    if (0 != s32Result)
    {
        printf("HI_MPI_IVE_Thresh错误: %x\n",s32Result);
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
	dTime = (HI_DOUBLE)HI_GetTickCount() - dTime;

	printf("Thresh mode %d,time = %g ms\n",enMode, dTime/1000000000);

    stbImageData out;
    out.w = stDst.au32Stride[0];
    out.h = stDst.u32Height;
    out.data = (unsigned char *)stDst.au64VirAddr[0];
    out.c = 1;
    writeGrayBmpImage("/mnt/bjwang/binaryThresh.bmp", out);

    IVE_MMZ_FREE(stSrc.au64PhyAddr[0], stSrc.au64VirAddr[0]);
    IVE_MMZ_FREE(stDst.au64PhyAddr[0], stDst.au64VirAddr[0]);
    // printf("line:%d\n", __LINE__);
} 

void ThreshSampleBinary(){
    IVE_THRESH_MODE_E mode= IVE_THRESH_MODE_BINARY;
    HI_CHAR *fileName = (char *)"./test.bmp";
    ThreshSample(mode,fileName);
}

void ive_thresh(){
    ThreshSampleBinary();

}

#endif
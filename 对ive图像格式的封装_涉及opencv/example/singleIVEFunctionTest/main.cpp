#include <cstdio>
#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_ive.h"

#include "sample_assist.h"
#include "sample_define.h"

#include <string>
using std::string;
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include <unistd.h>

#include "ive_filter.h"
#include "ive_thresh.h"
#include "ive_add.h"
#include "ive_and.hpp"
#include "ive_sub.hpp"
#include "ive_dma.hpp"

#include "hisiImage.h"

void sobelExample();
void SobelSampleBoth3x3();


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("need char to choose which function to be choosed\n");
        printf("1: sobel example\n2:filter example\n3:thresh example\n4: add example\n5: and example\n6: sub example\n7:dma examples\n");
        return 1;
    }
    switch (*argv[1])
    {
    case '0':

        break;
    case '1':
        /* code */
        sobelExample();
        break;
    case '2':
        ive_filter();
        break;
    case '3':
        ive_thresh();
        break;
    case '4':
        iveAddSample();
        break;
    case '5':
        iveAnd();
        break;
    case '6':
        ive_sub();
        break;
    case '7':
        ive_dma();
        break;
    case 'p':
    {
        hisiImage img;
        img.imread("./test.bmp");
        break;
    }
    default:
        break;
    }
    return 0;
}

/******sobel*******/
void sobelExample()
{
    printf(" start Sobel sample\n");

    SobelSampleBoth3x3();
    // SobelSampleBoth5x5();
    // SobelSampleHor3x3();
    // SobelSampleHor5x5();
    // SobelSampleVer3x3();
    // SobelSampleVer5x5();
}
#if 1
void doSobelSample(IVE_SOBEL_OUT_CTRL_E enOutCtrl, HI_S8 *ps8Mask)
{
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDstH;
    IVE_DST_IMAGE_S stDstV;
    IVE_SOBEL_CTRL_S stCtrl;
    string fileName = "./test.bmp";
    unsigned char *data = nullptr;
    stbImageData img;
    memset(&img, 0, sizeof(stbImageData));
    printf("start sobel\n");
    if (!readGrayBmpImage(fileName.c_str(), img))
    {
        printf("无法读取图片");
        return;
    }
    printf("读图成功\n");

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDstH, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stDstV, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_MAG_AND_ANG_CTRL_S));

    int s32Result = HI_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, img.w, img.h);
    if (0 != s32Result)
    {
        printf("创建图像错误\n");
        return;
    }
    if (IVE_SOBEL_OUT_CTRL_VER != enOutCtrl)
    {
        s32Result = HI_CreateIveImage(&stDstH, IVE_IMAGE_TYPE_S16C1, img.w, img.h);
        if (0 != s32Result)
        {
            printf("创建图像错误\n");
            return;
        }
    }

    if (IVE_SOBEL_OUT_CTRL_HOR != enOutCtrl)
    {
        s32Result = HI_CreateIveImage(&stDstV, IVE_IMAGE_TYPE_S16C1, img.w, img.h);
        if (0 != s32Result)
        {
            printf("创建图像错误:%x\n");
            return;
        }
    }

    unsigned char *pData = (HI_U8 *)stSrc.au64VirAddr[0];
    int y;
    for (y = 0; y < stSrc.u32Height; y++, pData += stSrc.au32Stride[0])
    {
        memcpy(pData, img.data, img.w);
        img.data += img.w;
    }

    printf("初始化图像成功");
    printf("line:%d\n", __LINE__);
    stCtrl.enOutCtrl = enOutCtrl;
    memcpy(stCtrl.as8Mask, ps8Mask, sizeof(HI_S8) * 25);
    IVE_HANDLE handle;

    HI_DOUBLE dTime;
    dTime = (HI_DOUBLE)HI_GetTickCount();
    printf("line:%d\n", __LINE__);

    s32Result = HI_MPI_IVE_Sobel(&handle, &stSrc, &stDstH, &stDstV, &stCtrl, HI_TRUE);
    if (0 != s32Result)
    {
        printf("HI_MPI_IVE_Sobel 错误 %x\n", s32Result);
        return;
    }
    HI_BOOL finish = (HI_BOOL)0;
    HI_BOOL block = (HI_BOOL)1;
    printf("line:%d\n", __LINE__);
    do
    {
        HI_MPI_IVE_Query(handle, &finish, block);
        // printf("延时\n");
        // usleep(5);
    } while (!finish);
    // sleep(1);
    dTime = ((HI_DOUBLE)HI_GetTickCount() - dTime) / 1000000000;

    printf("image w: %d h: %d, sobel time: %f\n", img.w, img.h, dTime);
    printf("line:%d\n", __LINE__);
    IVE_MMZ_FREE(stSrc.au64PhyAddr[0], stSrc.au64VirAddr[0]);
    IVE_MMZ_FREE(stDstH.au64PhyAddr[0], stDstH.au64VirAddr[0]);
    IVE_MMZ_FREE(stDstV.au64PhyAddr[0], stDstV.au64VirAddr[0]);
}
#endif

void SobelSampleBoth3x3()
{
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_BOTH;
    HI_S8 mask[25] = {0, 0, 0, 0, 0,
                      0, -1, 0, 1, 0,
                      0, -2, 0, 2, 0,
                      0, -1, 0, 1, 0,
                      0, 0, 0, 0, 0};

    doSobelSample(enOutCtrl, mask);
}

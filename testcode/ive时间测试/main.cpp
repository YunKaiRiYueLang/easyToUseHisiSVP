/*
opencv的sobel函数声明 ：输入，输出 输出图像深度 方向 sobel size
CV_EXPORTS_W void Sobel( InputArray src, OutputArray dst, int ddepth,
                         int dx, int dy, int ksize = 3,
                         double scale = 1, double delta = 0,
                         int borderType = BORDER_DEFAULT );

*/
#include <cstdio>
#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_ive.h"

#include "sample_assist.h"
#include "sample_define.h"

#include <string>
using std::string;

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "readimage.h"

#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include <unistd.h>

#include "ive_filter.h"
#include "ive_thresh.h"
#include "ive_add.h"

#include "hisiImage.h"

void sobelExample();
void SobelSampleBoth3x3();

void stdReadWriteImage();

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("need char to choose which function to be choosed\n");
        return 1;
    }
    switch (*argv[1])
    {
    case '0':
        stdReadWriteImage();

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

/*stb read write***/
void stdReadWriteImage()
{
    string filePath = "test.bmp";
    int x, y, channels_in_file, desired_channels = 3;
    unsigned char *data = stbi_load(filePath.c_str(), &x, &y, &channels_in_file, desired_channels);
    if (!data)
    {
        fprintf(stderr, "fail to read image: %s\n", filePath.c_str());
        return;
    }
    fprintf(stdout, "image: %s, x: %d, y: %d, channels_in_file: %d, desired_channels: %d\n",
            filePath.c_str(), x, y, channels_in_file, desired_channels);

    printf("1\n");

    string storeFilePath = "/mnt/bjwang/stbTest.bmp";
    int width_resize = x;
    int height_resize = y;
    unsigned char *outputPixel = data;
    int ret = stbi_write_bmp(storeFilePath.c_str(), width_resize, height_resize, desired_channels, outputPixel);
    if (ret == 0)
    {
        fprintf(stderr, "fail to write image png: %s\n", storeFilePath.c_str());
        return;
    }
    printf("2\n");

    free(data);
    return;
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

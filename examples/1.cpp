/**
 * @file 1.cpp
 * @author wangbianjiang (1510627880@qq.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-02
 * 
 * @copyright Copyright (c) 2021
 * 1. 不同聚焦点截图并保存为bmp文件
 */

#include "motor.hpp"
#include "string"
#include "tools/hisiImage.h"
#include "vpssFunction.hpp"
#include"iveFunctions.hpp"

using namespace std;

void storeImage1();
bool frameyuv420spTohisiImage(VIDEO_FRAME_S &frame, hisiImage &dst);
static void help()
{
    printf("need one parameter\n");
    printf("default grp 1 chn 1\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        help();
        return -1;
    }
    switch (atoi(argv[1]))
    {
    case 0:
        /* code */
        storeImage1();
        break;

    default:
        break;
    }
}

const int designatedGroup = 1, designatedChannel = 1;
void storeImage1()
{
    /*检查是否成功截图，像素格式,图片尺寸是否正确*/
    int designatedDepth = 1;
    ehisi::HISI_VPSS_SetChnDepth(designatedGroup, designatedChannel, designatedDepth);
    int i32focusValude;
    int i32zoomValue;
    DRV_MOTOR_GetPos(&i32focusValude, &i32zoomValue);
    printf("current i32focusValude :%d\n", i32focusValude);
    int temp = 280;
    VIDEO_FRAME_INFO_S frame;
    memset(&frame, 0, sizeof(VIDEO_FRAME_INFO_S));
    int index = 0;
    while (temp < 333)
    {
        DRV_MOTOR_SetPos(temp, i32zoomValue); //调整焦点值
        sleep(1);
        temp += 10;
        void *data = nullptr;
        ehisi::HISI_VPSS_GetChnFrame(designatedGroup, designatedChannel, frame, &data);
        hisiImage hiImage42sp;
        frameyuv420spTohisiImage(frame.stVFrame, hiImage42sp); //package u8c3像素获取。
        hisiImage hiImageu8c3;
        hiImageu8c3.create(frame.stVFrame.u32Width, frame.stVFrame.u32Height, IVE_IMAGE_TYPE_U8C3_PACKAGE);
        eive::iveCSC(hiImage42sp, hiImageu8c3, 3, 1);

        string path = "/mnt/bjwang/DebugDir/batch";
        path += to_string(index) + ".bmp";
        writeColorBmpImage(path.c_str(), hiImageu8c3);

        int u32Size = ehisi::GetFrameSize(frame.stVFrame.enPixelFormat,
                                          frame.stVFrame.u32Width,
                                          frame.stVFrame.u32Height);
        HI_MPI_VPSS_ReleaseChnFrame(designatedGroup, designatedChannel, &frame);

        HI_MPI_SYS_Munmap(data, u32Size);
        index++;
    }
    designatedDepth = 0;
    ehisi::HISI_VPSS_SetChnDepth(designatedGroup, designatedChannel, designatedDepth);
}

/*yuv420sp only*/
bool frameyuv420spTohisiImage(VIDEO_FRAME_S &frame, hisiImage &dst)
{
    // if (frame.u64VirAddr[0] == 0)
    // {
    //     printf("frame地址为空");
    //     return false;
    // }
    if (frame.enPixelFormat != PIXEL_FORMAT_YUV_SEMIPLANAR_420)
    {
        printf("PIXEL_FORMAT_YUV_SEMIPLANAR_420 needed");
        return false;
    }
    dst.iveImg.enType = IVE_IMAGE_TYPE_YUV420SP;

    memcpy(dst.iveImg.au32Stride, frame.u32Stride, sizeof(HI_U32) * 3);
    memcpy(dst.iveImg.au64PhyAddr, frame.u64PhyAddr, sizeof(HI_U64) * 3);
    memcpy(dst.iveImg.au64VirAddr, frame.u64VirAddr, sizeof(hi_u64) * 3);
    dst.iveImg.u32Height = frame.u32Height;
    dst.iveImg.u32Width = frame.u32Width;
    // printf("w h %d %d\n", frame.u32Width, frame.u32Height);
    return true;
}
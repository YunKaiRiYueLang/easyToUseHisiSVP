#pragma once
#include "debug.hpp"
#include "hi_comm_vpss.h"
#include "hi_type.h"
#include "mpi_sys.h"
#include "mpi_vpss.h"

#include "vector"

namespace ehisi
{
    int GetFrameSize(PIXEL_FORMAT_E enPixFmt, unsigned int u32Width, unsigned int u32Height)
    {
        int i32Size;

        switch (enPixFmt)
        {
        case PIXEL_FORMAT_YVU_PLANAR_422:
        case PIXEL_FORMAT_YVU_SEMIPLANAR_422:
        case PIXEL_FORMAT_YUV_SEMIPLANAR_422:
            i32Size = u32Width * u32Height * 2;
            break;

        case PIXEL_FORMAT_YVU_PLANAR_420:
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420:
        case PIXEL_FORMAT_YUV_SEMIPLANAR_420:
            i32Size = u32Width * u32Height * 3 / 2;
            break;

        case PIXEL_FORMAT_YVU_PLANAR_444:
        case PIXEL_FORMAT_YVU_SEMIPLANAR_444:
        case PIXEL_FORMAT_YUV_SEMIPLANAR_444:
        case PIXEL_FORMAT_ARGB_1555:
        case PIXEL_FORMAT_ARGB_4444:
            i32Size = u32Width * u32Height * 3;
            break;

        default:
            printf("unknow format %d!\n", enPixFmt);
            return -1;
        }

        return i32Size;
    }

    /**
 * @brief 设置指定grp chn 图像深度
 * 
 * @param VpssGrp 
 * @param VpssChn 
 * @param u32Depth 
 * @return int 
 */
    int HISI_VPSS_SetChnDepth(int VpssGrp, int VpssChn, unsigned int u32Depth)
    {
        HI_S32 s32Ret;
        VPSS_CHN_ATTR_S stChnAttr;

        s32Ret = HI_MPI_VPSS_GetChnAttr(VpssGrp, VpssChn, &stChnAttr);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_GetChnAttr", s32Ret);
        stChnAttr.u32Depth = u32Depth;
        s32Ret = HI_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stChnAttr);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_SetChnAttr", s32Ret);

        return HI_SUCCESS;
    }

    /**
 * @brief capture frame
 * 
 * @param group 
 * @param channel 
 * @param frame frame
 * @param data frame data's pointer,void*
 * @return int 
 */
    int HISI_VPSS_GetChnFrame(const int group, const int channel, VIDEO_FRAME_INFO_S &frame, void **data = nullptr)
    {
        int s32Ret;
        s32Ret = HI_MPI_VPSS_GetChnFrame(group, channel, &frame, 2000);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_GetChnFrame", s32Ret);
        int u32Size = GetFrameSize(frame.stVFrame.enPixelFormat,
                                   frame.stVFrame.u32Width,
                                   frame.stVFrame.u32Height);
        *data = (void *)HI_MPI_SYS_Mmap(frame.stVFrame.u64PhyAddr[0], u32Size); //frame内的虚拟地址不可用，重新map
        if (NULL == *data)
        {
            printf("HI_MPI_SYS_Mmap err!\n");
            return -1;
        }

        return HI_SUCCESS;
    }

    /**
 * @brief  release frame ,umap data
 * 
 * @param VpssGrp 
 * @param VpssChn 
 * @param frame 
 * @param data 
 * @return int 
 */
    int HISI_VPSS_ReleaseChnFrame(const int VpssGrp, const int VpssChn, VIDEO_FRAME_INFO_S &frame, void *data = nullptr)
    {
        HI_S32 s32Ret = HI_SUCCESS;
        HI_U32 u32Size = 0;
        u32Size = GetFrameSize(frame.stVFrame.enPixelFormat,
                               frame.stVFrame.u32Width,
                               frame.stVFrame.u32Height);

        s32Ret = HI_MPI_VPSS_ReleaseChnFrame(VpssGrp, VpssChn, &frame);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_ReleaseChnFrame", s32Ret);
        if (data != nullptr)
        {
            s32Ret = HI_MPI_SYS_Munmap(data, u32Size);
            CHECK_HISI_FUNCTION("HI_MPI_SYS_Munmap", s32Ret);
        }

        return HI_SUCCESS;
    }

    /*******************************************************************************
/// @brief Vpss获取通道宽高
/// @param [in] VpssGrp vpss组号
/// @param [in] VpssChn vpss通道号
/// @param [out] pu32Width vpss通道宽
/// @param [out] pu32Height vpss通道高
/// @return int32 0.成功; 非0.失败
*******************************************************************************/

    int HISI_VPSS_GetChnWH(int VpssGrp, int VpssChn, int *pu32Width, int *pu32Height)
    {
        HI_S32 s32Ret;
        VPSS_CHN_ATTR_S pstChnAttr;

        s32Ret = HI_MPI_VPSS_GetChnAttr(VpssGrp, VpssChn, &pstChnAttr);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_GetChnAttr error", s32Ret);
        *pu32Width = pstChnAttr.u32Width;
        *pu32Height = pstChnAttr.u32Height;

        return HI_SUCCESS;
    }

    /**
     * @brief 
     * 
     * @param VpssGrp 
     * @param VpssChn 
     * @param vecData 
     * @param vecFrame 
     * @return int 
     */
    int HISI_VPSS_ReleaseChnFrame(const int VpssGrp, const int VpssChn,
                                  std::vector<void *> &vecData, std::vector<VIDEO_FRAME_INFO_S> &vecFrame)
    {
        if (vecData.empty() || vecFrame.empty())
        {
            errorCode("HISI_VPSS_ReleaseChnFrame 输出向量不能为空", -1);
            return -1;
        }
        int size = vecData.size();
        for (int i = 0; i < size; i++)
        {
            HISI_VPSS_ReleaseChnFrame(VpssGrp, VpssChn, vecFrame[i], vecData[i]);
        }
        return 0;
    }

    /**
     * @brief 
     * 
     * @param VpssGrp 
     * @param VpssChn 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     * @param enable 
     * @return int 
     */
    int HISI_VPSS_SetChnCrop(int VpssGrp, int VpssChn, int x, int y, int w, int h, int enable = 1)
    {
        HI_S32 s32Ret;
        VPSS_CROP_INFO_S stCropInfo;
        if (x % 2 != 0 || y % 2 != 0 || w % 2 != 0 || h % 2 != 0)
        {
            errorCode("HISI_VPSS_SetChnCrop2 int x y w h 必须为偶数", -1);
            return -1;
        }
        int cw, ch;
        HISI_VPSS_GetChnWH(VpssGrp, VpssChn, &cw, &ch);
        if (w > cw || h > ch || x < 0 || y < 0)
        {
            errorCode("HISI_VPSS_SetChnCrop2 x y w h 超出范围", -1);
            printf(" x must >0 ,y must >0,current vpss channel wh %d %d\n", cw, ch);
            return -1;
        }
        s32Ret = HI_MPI_VPSS_GetChnCrop(VpssGrp, VpssChn, &stCropInfo);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_GetChnCrop fail", s32Ret);
        stCropInfo.bEnable = (HI_BOOL)enable;
        stCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
        stCropInfo.stCropRect.s32X = x;
        stCropInfo.stCropRect.s32Y = y;
        stCropInfo.stCropRect.u32Width = w;
        stCropInfo.stCropRect.u32Height = h;

        s32Ret = HI_MPI_VPSS_SetChnCrop(VpssGrp, VpssChn, &stCropInfo);
        CHECK_HISI_FUNCTION("HI_MPI_VPSS_SetChnCrop error", s32Ret);

        return HI_SUCCESS;
    }

}

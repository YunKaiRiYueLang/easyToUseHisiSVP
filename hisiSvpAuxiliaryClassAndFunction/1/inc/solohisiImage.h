#ifndef __SOLO_HISIIMAGE_H__
#define __SOLO_HISIIMAGE_H__

/*******************************hisi mpp头文件*************************************/
#include "hi_comm_ive.h"
#include "hi_comm_sys.h"
#include "hi_ive.h"
#include "mpi_ive.h"
#include "mpi_sys.h"
#include "sample_assist.h"
/********************************************************************/

#define IVE_MMZ_FREE(phy, vir)                                  \
    do                                                          \
    {                                                           \
        if ((0 != (phy)) && (0 != (vir)))                       \
        {                                                       \
            HI_MPI_SYS_MmzFree((phy), (HI_VOID *)(HI_UL)(vir)); \
            (phy) = 0;                                          \
            (vir) = 0;                                          \
        }                                                       \
    } while (0)

typedef struct
{
    int w;
    int h;
    int c;
    unsigned char *data;
} stbImageData;

bool readGrayBmpImage(const char *path, stbImageData &image);
bool writeGrayBmpImage(const char *path, stbImageData &image);
int HI_CreateIveImage2(IVE_IMAGE_S *pstImage, IVE_IMAGE_TYPE_E enType, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32Stride);
//目前只写用到的。涉及大块内存拷贝部分（图像内存），后续使用移动拷贝实现。不用低效拷贝操作
class hisiImage
{
public:
    hisiImage() : iveImg()
    {
        memset(this, 0, sizeof(hisiImage));
    };
    ~hisiImage()
    {
        if (0 != iveImg.u32Width)
        { //需要释放空间
            int i = 0;
            IVE_MMZ_FREE(iveImg.au64PhyAddr[i], iveImg.au64VirAddr[i]);
        }
    };
    void getIVEImage(IVE_IMAGE_S &out)
    {
        // memset(&out, 0, sizeof(IVE_IMAGE_S));
        out.enType = iveImg.enType;
        out.u32Height = iveImg.u32Height;
        out.u32Width = iveImg.u32Width;
        memcpy(out.au64VirAddr, (void *)iveImg.au64VirAddr[0], 3 * sizeof(HI_U64));
        memcpy(out.au64PhyAddr, (void *)iveImg.au64PhyAddr[0], 3 * sizeof(HI_U64));
        memcpy(out.au32Stride, (void *)iveImg.au32Stride[0], 3 * sizeof(HI_U32));
    }
    IVE_IMAGE_S getIVEImage() const
    {
        return iveImg;
    }
    IVE_DATA_S getIVEData() const
    {
        IVE_DATA_S data;
        data.u32Height = iveImg.u32Height;
        data.u32Width = iveImg.u32Width;
        data.u32Stride = iveImg.au32Stride[0];
        data.u64VirAddr = iveImg.au64VirAddr[0];
        data.u64PhyAddr = iveImg.au64PhyAddr[0];
        return data;
    }
    void imread(const char *filePath, hiIVE_IMAGE_TYPE_E type = IVE_IMAGE_TYPE_U8C1)
    {
        stbImageData img;
        if (!readGrayBmpImage(filePath, img))
        {
            printf("无法读取图片");
            return;
        };
        printf("读图成功\n");
        int s32Result = HI_CreateIveImage(&iveImg, IVE_IMAGE_TYPE_U8C1, img.w, img.h);
        if (0 != s32Result)
        {
            printf("创建图像错误\n");
            return;
        }
        unsigned char *pive = (unsigned char *)iveImg.au64VirAddr[0];
        unsigned char *psrc = (unsigned char *)img.data;
        for (int i = 0; i < img.h; i++)
        {
            memcpy(pive, psrc, img.w);
            pive += iveImg.au32Stride[0];
            psrc += img.w;
        }
    };
    void create(int w, int h, hiIVE_IMAGE_TYPE_E type = IVE_IMAGE_TYPE_U8C1)
    {
        int s32Result = HI_CreateIveImage(&iveImg, type, w, h);
        if (0 != s32Result)
        {
            printf("创建图像错误 error code:%x\n", s32Result);
            return;
        }
    };
    bool create(int w, int h, int s, hiIVE_IMAGE_TYPE_E type = IVE_IMAGE_TYPE_U8C1)
    {
        //跨度s由使用者保证。
        int s32ret = HI_CreateIveImage2(&iveImg, type, w, h, s);
        if (0 != s32ret)
        {
            printf("创建图像错误 error code:%x\n", s32ret);
            return false;
        }
        return true;
    }
    void create(const hisiImage &input)
    {
        int s32Result = HI_CreateIveImage(&iveImg, input.iveImg.enType, input.iveImg.au32Stride[0], input.iveImg.u32Height);
        if (0 != s32Result)
        {
            printf("创建图像错误 error code:%x\n", s32Result);
            printf("type %x,stride:%d,height:%d\n", input.iveImg.enType, input.iveImg.au32Stride[0], input.iveImg.u32Height);
            return;
        }
    }
    void create(const hisiImage &input, IVE_IMAGE_TYPE_E type)
    {
        create(input.iveImg.u32Width, input.iveImg.u32Height, type);
    }

public:
    IVE_IMAGE_S iveImg;
};

bool writeGrayBmpImage(const char *path, const hisiImage &src);
bool writeColorBmpImage(const char *path, const hisiImage &src);
#endif

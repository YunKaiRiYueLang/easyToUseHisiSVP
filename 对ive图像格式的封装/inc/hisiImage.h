#ifndef __HISIIMAGE_H__
#define __HISIIMAGE_H__

#include "hi_comm_ive.h"
#include "hi_comm_sys.h"
#include "hi_ive.h"
#include "mpi_ive.h"
#include "mpi_sys.h"
#include "sample_assist.h"

#include "time.h"

#include "commonHeader.h"

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#if !defined(STB_IMAGE_WRITE_IMPLEMENTATION)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

typedef struct
{
    int w;
    int h;
    int c;
    unsigned char *data;
} stbImageData;

bool readGrayBmpImage(const char *path, stbImageData &image)
{
    int x, y, channels_in_file, desired_channels = 1;
    image.data = stbi_load(path, &x, &y, &channels_in_file, desired_channels);
    if (!image.data)
    {
        fprintf(stderr, "fail to read image: %s\n", path);
        return false;
    }
    fprintf(stdout, "image: %s, x: %d, y: %d, channels_in_file: %d, desired_channels: %d\n",
            path, x, y, channels_in_file, desired_channels);
    image.w = x;
    image.h = y;
    image.c = 1;
    printf("%s end\n", __FUNCTION__);
    return true;
}

bool writeGrayBmpImage(const char *path, stbImageData &image)
{
    int ret = stbi_write_bmp(path, image.w, image.h, image.c, image.data);
}
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

class hisiImage
{
public:
    hisiImage() : iveImg()
    {
        memset(this, 0, sizeof(hisiImage));
    };
    hisiImage(int w, int h, void *data, int type = 0)
    { //默认type==0 单通道8bit
        create(w, h, hiIVE_IMAGE_TYPE_E(type));
        unsigned char *p = (unsigned char *)iveImg.au64VirAddr[0];
        unsigned char *psrc = (unsigned char *)data;
        for (int i = 0; i < h; i++)
        {
            memcpy(p, psrc, w);
            p += iveImg.au32Stride[0];
            psrc += w;
        }
    }
    hisiImage(const cv::Mat &cvSrc)
    {
        this->cloneFrom(cvSrc);
    }
    // hisiImage(int width,int height,int channel,hiIVE_IMAGE_TYPE_E type){
    // if(0==width||0==height||0==channel&&(type!=IVE_IMAGE_TYPE_U8C1))
    //
    // }
    ~hisiImage()
    {
        if (0 != iveImg.u32Width)
        { //需要释放空间
            int i = 0;
            // do{
            IVE_MMZ_FREE(iveImg.au64PhyAddr[i], iveImg.au64VirAddr[i]);
            // } while (i<channel);    //attention: need loop or not?
        }
    };
    void getIVEImage(IVE_IMAGE_S &out)
    {
        // memset(&out, 0, sizeof(IVE_IMAGE_S));
        out.enType = iveImg.enType;
        out.u32Height = iveImg.u32Height;
        out.u32Width = iveImg.u32Width;
        memcpy(out.au64VirAddr, iveImg.au64VirAddr, 3 * sizeof(HI_U64));
        memcpy(out.au64PhyAddr, iveImg.au64PhyAddr, 3 * sizeof(HI_U64));
        memcpy(out.au32Stride, iveImg.au32Stride, 3 * sizeof(HI_U32));
    }
    IVE_IMAGE_S getIVEImage() const
    {
        return iveImg;
    }
    IVE_DATA_S getIVEData() const
    {
        IVE_DATA_S ret;
        ret.u32Height = iveImg.u32Height;
        ret.u32Width = iveImg.u32Width;
        ret.u32Stride = iveImg.au32Stride[0];
        ret.u64VirAddr = iveImg.au64VirAddr[0];
        ret.u64PhyAddr = iveImg.au64PhyAddr[0];
        return ret;
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
    inline void create(int w, int h, hiIVE_IMAGE_TYPE_E type = IVE_IMAGE_TYPE_U8C1)
    {
        int s32Result = HI_CreateIveImage(&iveImg, type, w, h);
        if (0 != s32Result)
        {
            printf("创建图像错误 error code:%x\n", s32Result);
            return;
        }
    };
    void create(const hisiImage &input)
    {
        int s32Result = HI_CreateIveImage(&iveImg, input.iveImg.enType, input.iveImg.au32Stride[0], input.iveImg.u32Height);
        if (0 != s32Result)
        {
            printf("创建图像错误 error code:%x\n", s32Result);
            printf("type %x,stride:%d,height:%d\n", input.iveImg.enType, input.iveImg.au32Stride[0], input.iveImg.u32Height);
            return;
        }
        size_t length = iveImg.au32Stride[0] * iveImg.u32Height;
        memset((void *)iveImg.au64VirAddr[0], 0, length);
    }
    void create(const hisiImage &input, IVE_IMAGE_TYPE_E type)
    {
        create(input.iveImg.u32Width, input.iveImg.u32Height, type);
    }
    void create(int w, int h, void *data, int type=0)
    {
        create(w, h, hiIVE_IMAGE_TYPE_E(type));
        unsigned char *p = (unsigned char *)iveImg.au64VirAddr[0];
        unsigned char *psrc = (unsigned char *)data;
        for (int i = 0; i < h; i++)
        {
            memcpy(p, psrc, w);
            p += iveImg.au32Stride[0];
            psrc += w;
        }
    }
    void cloneFrom(const cv::Mat &src)
    {
        IVE_IMAGE_TYPE_E type = IVE_IMAGE_TYPE_U8C1;
        int s32ret = HI_CreateIveImage(&iveImg, type, src.cols, src.rows);
        if (0 != s32ret)
        {
            printf("创建图像错误 error code:%x\n", s32ret);
            return;
        }
        unsigned char *p = (unsigned char *)iveImg.au64VirAddr[0];
        unsigned char *psrc = src.data;
        int width = src.cols;
        for (int i = 0; i < src.rows; i++)
        {
            memcpy(p, psrc, width);
            p += iveImg.au32Stride[0];
            psrc += width;
        }
    }
    void copyTo(cv::Mat &dst)
    {
        dst = cv::Mat::zeros(iveImg.u32Height, iveImg.au32Stride[0], CV_8UC1);
        unsigned char *p = (unsigned char *)iveImg.au64VirAddr[0];
        unsigned char *psrc = dst.data;
        int width = iveImg.au32Stride[0];
        for (int i = 0; i < dst.rows; i++)
        {
            memcpy(psrc, p, width);
            p += iveImg.au32Stride[0];
            psrc += width;
        }
    }

public:
    IVE_IMAGE_S iveImg;
    // int height;
    // int width;
    // int channel;
    // hiIVE_IMAGE_TYPE_E type;
    // HI_U64 au64PhyAddr[3]; /* RW;The physical address of the image */
    // HI_U64 au64VirAddr[3]; /* RW;The virtual address of the image */
    // HI_U32 au32Stride[3];  /* RW;The stride of the image */
};

bool writeColorBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 3, (void *)img.au64VirAddr[0]);
}

bool writeGrayBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 1, (void *)img.au64VirAddr[0]);
}
bool writeiveImage(const char*path,IVE_IMAGE_S src){
    int ret = stbi_write_bmp(path, src.au32Stride[0], src.u32Height, 3, (void *)src.au64VirAddr[0]);
}
#endif

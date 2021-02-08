#ifndef __HISIIMAGE_H__
#define __HISIIMAGE_H__

#include "hi_comm_ive.h"
#include "hi_ive.h"
#include "mpi_ive.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "sample_assist.h"

#include "readimage.h"

#include "hisiImage.h"
#include "time.h"

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#if !defined(STB_IMAGE_WRITE_IMPLEMENTATION)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

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

//目前只写用到的。涉及大块内存拷贝部分（图像内存），后续使用移动拷贝实现。不用低效拷贝操作
class hisiImage
{
public:
    hisiImage() : iveImg(){
                      // memset(this, 0, sizeof(hisiImage));
                  };
    // hisiImage(hisiImage &src1,IVE_IMAGE_TYPE_E type){
    //     create(src1.iveImg.u32Width, src1.iveImg.u32Height, type);
    // }
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
        memcpy(out.au64VirAddr, (void *)iveImg.au64VirAddr[0], 3 * sizeof(HI_U64));
        memcpy(out.au64PhyAddr, (void *)iveImg.au64PhyAddr[0], 3 * sizeof(HI_U64));
        memcpy(out.au32Stride, (void *)iveImg.au32Stride[0], 3 * sizeof(HI_U32));
    }
    IVE_IMAGE_S getIVEImage() const
    {
        /*
        IVE_IMAGE_S out;
        memset(&out, 0, sizeof(IVE_IMAGE_S));
        out.enType = type;
        out.u32Height = height;
        out.u32Width = width;
        memcpy(out.au64VirAddr, au64VirAddr, 3 * sizeof(HI_U64));
        memcpy(out.au64PhyAddr, au64PhyAddr, 3 * sizeof(HI_U64));
        memcpy(out.au32Stride, au32Stride, 3 * sizeof(HI_U32));
        return out;*/
        return iveImg;
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
    }
    void create(const hisiImage &input, IVE_IMAGE_TYPE_E type)
    {
        create(input.iveImg.u32Width, input.iveImg.u32Height, type);
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

bool writeGrayBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 1, (void *)img.au64VirAddr[0]);
}

#endif

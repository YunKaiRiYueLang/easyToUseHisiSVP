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

#define IVE_MMZ_FREE(phy,vir)\
    do{\
        if ((0 != (phy)) && (0 != (vir)))\
        {\
            HI_MPI_SYS_MmzFree((phy),(HI_VOID *)(HI_UL)(vir));\
            (phy) = 0;\
            (vir) = 0;\
        }\
    }while(0)

//目前只写用到的。涉及大块内存拷贝部分（图像内存），后续使用移动拷贝实现。不用低效拷贝操作
class hisiImage{
public:
    hisiImage(){
        memset(this,0, sizeof(hisiImage));
    };
    // hisiImage(int width,int height,int channel,hiIVE_IMAGE_TYPE_E type){
        // if(0==width||0==height||0==channel&&(type!=IVE_IMAGE_TYPE_U8C1))
// 
    // }
    ~hisiImage(){
        if(0!=width){   //需要释放空间
            int i = 0;
            // do{
                IVE_MMZ_FREE(au64PhyAddr[i], au64VirAddr[i]);
            // } while (i<channel);    //attention: need loop or not?
        }
    };
    void getIVEImage(IVE_IMAGE_S &out){
        // memset(&out, 0, sizeof(IVE_IMAGE_S));
        out.enType = type;
        out.u32Height = height;
        out.u32Width = width;
        memcpy(out.au64VirAddr, au64VirAddr, 3 * sizeof(HI_U64));
        memcpy(out.au64PhyAddr, au64PhyAddr, 3 * sizeof(HI_U64));
        memcpy(out.au32Stride, au32Stride, 3 * sizeof(HI_U32));
    }
    IVE_IMAGE_S getIVEImage(){
        IVE_IMAGE_S out;
        memset(&out, 0, sizeof(IVE_IMAGE_S));
        out.enType = type;
        out.u32Height = height;
        out.u32Width = width;
        memcpy(out.au64VirAddr, au64VirAddr, 3 * sizeof(HI_U64));
        memcpy(out.au64PhyAddr, au64PhyAddr, 3 * sizeof(HI_U64));
        memcpy(out.au32Stride, au32Stride, 3 * sizeof(HI_U32));
        return out;
    }
private:
    IVE_IMAGE_S img;
    int height;
    int width;
    int channel;
    hiIVE_IMAGE_TYPE_E type;
    HI_U64 au64PhyAddr[3];   /* RW;The physical address of the image */
    HI_U64 au64VirAddr[3];   /* RW;The virtual address of the image */
    HI_U32 au32Stride[3];    /* RW;The stride of the image */
};

#endif

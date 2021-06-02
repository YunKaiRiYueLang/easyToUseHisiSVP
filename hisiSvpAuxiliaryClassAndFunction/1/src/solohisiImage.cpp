#include "solohisiImage.h"

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#if !defined(STB_IMAGE_WRITE_IMPLEMENTATION)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

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

bool writeGrayBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 1, (void *)img.au64VirAddr[0]);
}

bool writeColorBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 3, (void *)img.au64VirAddr[0]);
}

int HI_CreateIveImage2(IVE_IMAGE_S *pstImage, IVE_IMAGE_TYPE_E enType, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32Stride)
{
    pstImage->enType = enType;
    pstImage->u32Width = u32Width;
    pstImage->u32Height = u32Height;
    pstImage->au32Stride[0] = u32Stride;
    switch (enType)
    {
    case IVE_IMAGE_TYPE_U8C1:
    case IVE_IMAGE_TYPE_S8C1:
    {
        int ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&(pstImage->au64VirAddr[0]),
                                      HI_NULL, HI_NULL, u32Stride * u32Height);
        if (ret != 0)
        {
            printf("error code :%x\n", ret);
            return ret;
        }
        break;
    }

    default:
        return -1;
    }
    return 0;
}
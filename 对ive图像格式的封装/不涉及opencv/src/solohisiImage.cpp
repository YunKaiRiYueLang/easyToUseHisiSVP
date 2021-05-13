#include"solohisiImage.h"

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#if !defined(STB_IMAGE_WRITE_IMPLEMENTATION)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

bool readGrayBmpImage(const char* path,stbImageData &image){
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

bool writeGrayBmpImage(const char* path,stbImageData &image){
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


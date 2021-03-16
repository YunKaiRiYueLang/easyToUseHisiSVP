#if !defined(__READIMAGE_H__)
#define __READIMAGE_H__

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION


#if !defined(STB_IMAGE_WRITE_IMPLEMENTATION)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

typedef struct{
    int w;
    int h;
    int c;
    unsigned char *data;
} stbImageData;

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




#endif // __READIMAGE_H__

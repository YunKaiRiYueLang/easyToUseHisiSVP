#include "imageio.h"

#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#if !defined(STB_IMAGE_WRITE_IMPLEMENTATION)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif
#include "hi_ive.h"

#include "hisiImage.h"

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
bool writeColorBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 3, (void *)img.au64VirAddr[0]);
}

bool writeGrayBmpImage(const char *path, const hisiImage &src)
{
    IVE_IMAGE_S img = src.getIVEImage();
    int ret = stbi_write_bmp(path, img.au32Stride[0], img.u32Height, 1, (void *)img.au64VirAddr[0]);
    if (ret == 0)
    {
        printf("failed to write bmp image %d\n", ret);
        printf("%s\n", path);
    }
}
bool writeiveImage(const char *path, IVE_IMAGE_S src)
{
    int ret = stbi_write_bmp(path, src.au32Stride[0], src.u32Height, 3, (void *)src.au64VirAddr[0]);
    if (ret == 0)
    {
        printf("failed to write bmp image %d\n", ret);
        printf("%s\n", path);
        return false;
    }
    return true;
}
bool writeIveImage_jpg(const char *path, IVE_IMAGE_S src)
{
    int ret = stbi_write_jpg(path, src.au32Stride[0], src.u32Height, 3, (void *)src.au64VirAddr[0], 0);
    if (ret == 0)
    {
        printf("failed to write jpg image %d\n", ret);
        printf("%s\n", path);
        return false;
    }
    return true;
}

/**
 * @brief change ive format image to opencv Mat format;
 * 
 * @param outCvImage final output image in Mat format;
 * @param inputIveImage the input IVE_IMAGE_S format image
 * 
 * @attention dma copy may be used later.
 * @attention outCvImage need be initialed by input image's width height
 */
bool iveImageToMat(Mat &outCvImage, IVE_IMAGE_S &inputIveImage)
{
    if (inputIveImage.u32Width == 0 || inputIveImage.u32Height == 0)
    {
        // error("EMPTY IVE_IMAGE_S IMAGE");
        return false;
    }
    if (outCvImage.rows != inputIveImage.u32Height || outCvImage.cols != inputIveImage.u32Width)
    {
        // error("image size dose not match");
        return false;
    }
    switch (inputIveImage.enType)
    {
    // case IVE_IMAGE_TYPE_S8C1:
    case IVE_IMAGE_TYPE_U8C1:
    {
        uchar *pcv = outCvImage.data;
        uchar *pive = (uchar *)inputIveImage.au64VirAddr[0];
        for (int i = 0; i < inputIveImage.u32Height; i++)
        {
            memcpy(pcv, pive, inputIveImage.u32Width);
            pcv += inputIveImage.u32Width;
            pive += inputIveImage.au32Stride[0];
        }
        return true;
    }
    default:
        // error("NO CORRECT FORMAT");
        return false;
    }
}
/**
 * @brief this does not involve memory alloction;parameter must be prepared before this function
 * 
 * @param input 
 * @param output  
 * @return true 
 * @return false 
 */
bool MatU8C1ImageToIve(const Mat &input, IVE_IMAGE_S &output)
{
    if (input.data == NULL || output.u32Width == 0)
    {
        // error("do not be prepared for input image or output image");
        return false;
    }
    uchar *pInput = input.data;
    uchar *pOutput = (uchar *)output.au64VirAddr[0];
    for (int i = 0; i < input.rows; i++)
    {
        memcpy(pOutput, pInput, input.cols);
        pInput += input.cols;
        pOutput += output.au32Stride[0];
    }
    return true;
}

#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include"hi_ive.h"

// class hisiImage;//类声明。
#include"hisiImage.h"

typedef struct
{
    int w;
    int h;
    int c;
    unsigned char *data;
} stbImageData;

bool readGrayBmpImage(const char *path, stbImageData &image);
bool writeGrayBmpImage(const char *path, stbImageData &image);
bool writeColorBmpImage(const char *path, const hisiImage &src);
bool writeGrayBmpImage(const char *path, const hisiImage &src);
bool writeiveImage(const char *path, IVE_IMAGE_S src);
bool writeIveImage_jpg(const char *path, IVE_IMAGE_S src);
#ifdef OPENCV_CORE_MAT_HPP
bool iveImageToMat(Mat &outCvImage, IVE_IMAGE_S &inputIveImage);
bool MatU8C1ImageToIve(const Mat &input, IVE_IMAGE_S &output);
#endif
#endif
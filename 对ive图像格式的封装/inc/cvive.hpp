/*用于opencv的Mat格式与海思ive格式的转换*/
#pragma once

#include "commonHeader.h"
#include "hi_comm_ive.h"
#include"debug.hpp"



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
        error("EMPTY IVE_IMAGE_S IMAGE");
        return false;
    }
    if (outCvImage.rows != inputIveImage.u32Height || outCvImage.cols != inputIveImage.u32Width)
    {
        error("image size dose not match");
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
        error("NO CORRECT FORMAT");
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
    if(input.data==NULL||output.u32Width==0){
        error("do not be prepared for input image or output image");
        return false;
    }
    uchar *pInput = input.data;
    uchar *pOutput = (uchar*)output.au64VirAddr[0];
    for (int i = 0; i < input.rows;i++){
        memcpy(pOutput, pInput, input.cols);
        pInput += input.cols;
        pOutput += output.au32Stride[0];
    }
    return true;
}

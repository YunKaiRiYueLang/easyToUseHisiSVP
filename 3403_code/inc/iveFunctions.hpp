#ifdef hi_3403
bool iveCsc(ot_svp_img &src, ot_svp_img &dst, const int mode, const int needBlock)
// transform hisi yuv (BT 709 :420 sp / 422 sp ) to rgb (0~255) package;
// use guarantees the security of the two parameters
bool yuv2rgbIve(ot_video_frame_info *frame_info, void *rgb);
bool iveCsc(hisiImage &src, hisiImage &dst, const int mode, const int needBlock);
void MediaPatrol(int roiWidth, int roiHeight, uint8_t *yuvBuffer, uint8_t **dstEdgeBuffer, int high_th = 150, int low_th = 50);
#endif

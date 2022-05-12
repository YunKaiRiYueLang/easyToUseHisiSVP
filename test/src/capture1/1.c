#include"hisi_vpss.h"


int main(){
    VPSS_GRP grp=0;
    FRAME_INFO_S frame;
    int ret=HISI_VPSS_GetChnFrame(grp,VPSS_CHN3,&frame);
    if(ret!=HI_SUCCESS){
        printf("error return %x\n",ret);
        return 0;
    }
    printf("w h %d %d\n",frame.u32Width,frame.u32Height);
    HISI_VPSS_ReleaseChnFrame(grp,VPSS_CHN3,&frame);
    
    return 0;

}

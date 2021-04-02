#pragma once

#include <unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <sys/ioctl.h>
#include"cstdio"

#define MT_GET_KEY				0x61
#define MT_RUN_POS				0x62

typedef struct drv_motor_pos_s
{
	int	zm_pos;
	int	af_pos; 	
} DRV_MOTOR_POS_S;

int DRV_MOTOR_SetPos(int af, int zm)
{
   //return;
	DRV_MOTOR_POS_S cur_pos;
	//char *dev  = "/dev/an41908";
	char *dev  = (char *)"/dev/io_dev";
	int  fd = -1;

	fd = open(dev, O_RDWR);
	if(fd < 0){
		printf("az_pos_set: open /dev/io_dev error\n");
		return -1;
	}

	cur_pos.af_pos = af;
	cur_pos.zm_pos = zm;
	ioctl(fd, MT_RUN_POS, &cur_pos);
	// printf("az_pos_set %d %d\n",af,zm);
	close(fd);
	return 1;
}


int DRV_MOTOR_GetPos(int *af, int *zm)
{
	//return;
	DRV_MOTOR_POS_S cur_pos;
	//char *dev  = "/dev/an41908";
	//char *dev  = "/dev/misc/io_dev";
	char *dev  = (char *)"/dev/io_dev";
	int  fd = -1;
	int  ret =  0;
	
	fd = open(dev, O_RDWR);
	if(fd < 0){
		printf("az_pos_get:open /dev/io_dev error\n");
		return -1;
	}

	ret = ioctl(fd, MT_GET_KEY, &cur_pos);
	*af = cur_pos.af_pos;
	*zm = cur_pos.zm_pos;
	//printf("az_pos_get %d %d\n",*af,*zm);
	close(fd);
	
	return ret;
}

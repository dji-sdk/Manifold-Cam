#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include "djicam.h"

#define FRAME_SIZE              (1280*720*3/2)  /*format NV12*/
#define BLOCK_MODE                     1

static unsigned char buffer[FRAME_SIZE+8] = {0};
static unsigned int nframe = 0;
static int mode = 0;

static void print_usage(const char *prog)
{
	printf("Usage: sudo %s [-dgt]\n",prog);
	puts("  -d --display    display vedio stream\n"
		 "  -g --getbuffer  get NV12 format buffer\n"
		 "  -t --transfer   transfer vedio datas to RC\n"
		 "  Note: -d and -g cannot be set at the same time\n");   
}

static void parse_opts(int argc, char **argv)
{
	int c;
	static const struct option lopts[] = {
		{"display", 0,0,'d'},
		{"getbuffer", 0,0,'g'},
		{"transfer",0,0,'t'},
		{NULL,      0,0, 0 },
	};

	while((c = getopt_long(argc, argv, "dgt", lopts, NULL)) != -1)
	{    

		switch(c)
		{
			case 'd':
				mode |=  DISPLAY_MODE;
				break;
			case 'g':
				mode |=  GETBUFFER_MODE;
				break;
			case 't':
				mode |=  TRANSFER_MODE;
				break;
			default:
				print_usage(argv[0]);
				exit(0);
		}

	}

}

int main(int argc, char **argv)
{
	int ret;
	if(0 != geteuid())
	{
		printf("Please run ./test as root!\n");
		print_usage(argv[0]);
		return -1;
	}
	parse_opts(argc, argv); /*get parameters*/
	if(0 == mode || 3 == mode || 7 == mode) /*invalid mode*/
	{
		print_usage(argv[0]);
		return -1;
	}
	ret = manifold_cam_init(mode);
	if(-1 == ret)
	{
		printf("manifold init error \n");
		return -1;
	}

	while(1) /*Ctrl+c to break out*/
	{    
		if(mode & GETBUFFER_MODE)
		{
#if BLOCK_MODE				
			ret = manifold_cam_read(buffer, &nframe, CAM_BLOCK); /*blocking read*/	
			if(ret >= 0)
				printf("frame_size = %d, nframe = %d\n", ret, nframe);			
			else if(ret < 0)
				break;
#else
            		ret = manifold_cam_read(buffer, &nframe, CAM_NON_BLOCK); /*non_blocking read*/
            		if(ret > 0)
                		printf("frame_size = %d, nframe = %d\n", ret, nframe);	
             		else if(0 == ret)
                		printf("no new frame, ret = %d\n",ret);
             		else
                		break;
#endif
		}

		if(manifold_cam_exit()) /*to check if threads stop working*/
			break; 
		usleep(10000);    
	}

	while(!manifold_cam_exit()) /*make sure all threads exit ok*/
	{
		sleep(1);
	}
	return 0;
}

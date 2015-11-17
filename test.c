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
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
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
        {"display",   0,0,'d'},
        {"getbuffer", 0,0,'g'},
        {"transfer",  0,0,'t'},
        {NULL,        0,0, 0 },
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

static void *get_images_loop(void *data)
{
    int ret;

    while(!manifold_cam_exit()) /*Ctrl+c to break out*/
    {
        if(mode & GETBUFFER_MODE)
        {
#if BLOCK_MODE
            ret = manifold_cam_read(buffer, &nframe, CAM_BLOCK); /*blocking read*/
            if(ret < 0)
            {
                printf("manifold_cam_read error \n");
                break;
            }

#else
            ret = manifold_cam_read(buffer, &nframe, CAM_NON_BLOCK); /*non_blocking read*/
            if(ret < 0)
            {
                printf("manifold_cam_read error \n");
                break;
            }
#endif
        }

        usleep(1000);
    }

    printf("get_images_loop thread exit! \n");

}

int main(int argc, char **argv)
{
    int ret;
    pthread_attr_t attr;
    struct sched_param schedparam;
    pthread_t read_thread;

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

    /*
     * if the cpu usage is high, the scheduling policy of the read thread
     * is recommended setting to FIFO, and also, the priority of the thread should be high enough.
     */
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy((pthread_attr_t *)&attr, SCHED_FIFO);
    schedparam.sched_priority = 90;
    pthread_attr_setschedparam(&attr,&schedparam);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    if (pthread_create(&read_thread, &attr, get_images_loop, NULL) != 0)
    {
        perror ("usbRead_thread create");
        assert(0);
    }

    if(pthread_attr_destroy(&attr) != 0)
    {
        perror("pthread_attr_destroy error");
    }

    pthread_join(read_thread, NULL);/*wait for read_thread exit*/

    while (!manifold_cam_exit())
    {
        sleep(1);
    }

    return 0;
}

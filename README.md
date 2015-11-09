## What's This?

The manifold_cam project realizes to get the H.264 video streams from Zenmuse X3 camera, decode and display the vedio in real time. What's more, it also realizes to transfer stream data to RC.

## Requirements

- GStreamer, FFmpeg, libusb (Install already)
- libdcam.so, Manifold cam driver library 

## Quick Start
 
### 1. Compile with makefile

~~~
 $ make             //compile to the obj file 'test'
 $ make clean       //remove the obj file and intermediate files
 $ sudo make install     //install the libdcam.so to /usr/lib
~~~

### 2. Run it

~~~
 $ sudo ./test [-dgt]
~~~

Usage: 

- -d --display    display vedio stream
- -g --getbuffer  get NV12 format buffer
- -t --transfer   transfer vedio datas to RC

**Note: -d and -g cannot be set at the same time**

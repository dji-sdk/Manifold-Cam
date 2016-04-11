## What's This?

The manifold_cam project implements a functionality to get H.264 video stream from DJI Zenmuse X3 camera, decode and display the video in realtime. What's more, it also allows developers to transfer stream data to the mobile APP.

## Requirements

- GStreamer, FFmpeg, libusb (Install already)
- libdcam.so, Manifold camera driver library 

## Quick Start
 
### 1. Compile with makefile

~~~
 $ sudo make install     //install the libdcam.so to /usr/lib, you need to install the library first
 $ make                  //compile to the obj file 'test'
 $ make clean            //remove the obj file and intermediate files
~~~

### 2. Run it

~~~
 $ sudo ./test [-dgt]
~~~

Usage: 

- -d --display    display video stream
- -g --getbuffer  get NV12 format buffer
- -t --transfer   transfer video datas to RC

## Note
- `d` and `g` cannot be used at the same time
- DJI GO APP is not necessary. If the APP is running, developers should tap `enter camera view` and it's very important that the APP should always be in camera view mode.
- The resolution of image frame is 1280 X 720, and the format is NV12

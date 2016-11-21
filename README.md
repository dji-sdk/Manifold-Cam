## What's This?

The manifold_cam project implements a functionality to get H.264 video stream from DJI Zenmuse X3 camera, decode and display the video in realtime. What's more, it also allows developers to transfer stream data to the mobile APP.

**This package is specially designed for DJI Manifold. It doesn't support any other platform.**

## Requirements

- GStreamer, FFmpeg, libusb (Install already)
- libdcam.so, Manifold camera driver library 

## Quick Start
 
### 0. Connecting Manifold to the Camera

Connect the 8-pins port on the camera to the CAM_IN port through the supplied 8-pins cable. Then, connect
the CAM_OUT port to the 8-pins port on the N1 flight controller on the Matrice 100. Finally, connect the 10-
pins port from the N1 flight controller to the 10 pins port on the gimbal camera.

![](http://i.imgur.com/7qI9I9R.png)

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

## SSH

This program needs a graphical library from `xinit&`, which won't be found when working with SSH.

To solve it:

1. (Optional) Disable lightdm for stability:

   in `/etc/init/lightdm.conf` line 12:
   
   Modify `runlevel [!06]` to `runlevel [!026]`
   
2. Add xinit& to a startup script, such as:

   ```
   echo -e '#!/bin/bash\nxinit&' > /home/ubuntu/pre_x3
   chmod a+x /home/ubuntu/pre_x3
   ```
   
   And add `/home/ubuntu/pre_x3` into `/etc/rc.local`. The `ubuntu` here is the current user, you may need to change it into yours.

3. Add `export DISPLAY=:0` to your .bashrc or remember to set this environment variable to THE USER WHO WILL RUN THE CODE

4. (Maybe a reboot at first) Run the ./test with paramters you want.

> This part is contributed by @groundmelon, check [his repository](https://github.com/groundmelon/m100_x3) for more detail.

## Note
- `d` and `g` cannot be used at the same time
- DJI GO APP is not necessary. If the APP is running, developers should tap `enter camera view` and it's very important that the APP should always be in camera view mode.
- The resolution of image frame is 1280 X 720, and the format is NV12

## Other

The resource of Manifold:

1. The user manual: https://dl.djicdn.com/downloads/manifold/en/Manifold_User_Manual__en__v1.0.pdf
2. The system image: https://dl.djicdn.com/downloads/manifold/manifold_image_v1.0.tar.gz

# Stereo vision thesis readme
---

This README file is for the individual research thesis for
Cranfield University done by Nigel Thornewill von Essen

It is about making a prototype using the raspberry pi 3
to make a stereo vision Advanced Diving assistant. This
system will create the disparity map of the scene and
highlight object that are considered too close to the cameras

---
### What to expect from this project
---
With in this project you can find the code files for both the
stereo vision application and the calibration files. The program
works on the command line on Linux. The calibration stage requires
images with a chessboard pattern that is 9 x 6 to be able to calibrate
the cameras. This step has the most user input.

The second section of the program is the Stereo Vision and is the
section that requires little to no input from the user; where the user
only. This section is where the disparity map and highlighted images
made. There is some functionality to save images and gather information
on the computational time of the program; but this is rather limited.

Lastly the is a configuration file that makes tuning parameters easier,
if the user is not satisfied with any of the parameters. These are
already set to parameters that will give a good result.

---
### Acknowledgments
---
I would like to give thanks to my supervisor Dr Antonios Antoniadis
for giving me guidance on this project. I would also like to thanks
my dad for helping me through for bouncing idea off of.

I would also like to shoutout the O'reilly Learning Opencv 3 book for
giving a concise tool to be able to complete this project.

---
### Dependencies
---
 This program requires, OpenCV and Motion.

***
 OpenCV is the computer vision library and the install instructions
 can be found at this link [OpenCV Install for Linux](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html).

 to make the Files all that is needed is the following command

```
[compiler] sudo apt-get install build-essential
[required] sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
[optional] sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev```
```
next are the make files

 ```
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DOPENCV_EXTRA_MODULES_PATH = <path to opencv_contrib/modules/> \
      -DBUILD_DOCS = ON\
      -DBUILD_EXAMPLES OFF ..
 ```
 There are Some other features that can be toggled for python and java
 compatibility.

 next is the compiling OpenCV is done with the following
 ```
 make -j2 # runs 2 cores in parallel
 ```

 Then to make the documentation is

 ```
 cd ~/opencv/build/doc/
 make -j7 doxygen
 ```

then it should be installed correctly.

---

To get motion just run the following command. This needs to be on both
Pi's as its for streaming the camera information. make sure that The IP
is static on both pi's and change the IP in the ```VideoCapture``` obj-
ects to the IP you set the Pi's to.


```
sudo apt-get install motion
```

Then you can fiddle around with the settings to what you like.

---
### How to run the file
---

once everything is installed you can run the program. run the
motion software on both Pi's, on the rig I was working on, This
was already done and set up so that you just have to turn on both
and the motion software will run on the slave Pi.

on the master Pi, just run go to the directory, you can use the
following command to run the stereo program
```
sudo motion
cd Desktop\Code\thesis
.\Stereo -v
```
this will run everything.

---
There are multiple flags that you can use
```
-f n   #is the full calibration
       #takes n number of photos and calibrates

-t n   #just takes n number of photots

-s     #just calibrates with premade photos

-v     #does the Stereo vision potion (the main section)

-r     #shows rectified images

-p     #is the parameter tuner section

-h     #is the help section of the program

```

please only use only one flag at a time. I have not
set it up to take multiple flags. also flags with n
behind them require a number.

Thank you for reading me have a lovely time.

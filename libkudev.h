/* File Name: libkudev.h
   Author: Alrayan
   Created on: 19 May 2016
   Description: libkudev is made to decide /dev/videoX of
                the camera dynamically.
*/

#ifndef __LIBKUDEV_H_
#define __LIBKUDEV_H_

#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int koeva_udev_init();
int koeva_udev_indexOfTopCamera_V4L2();
int koeva_udev_indexOfBottomCamera_V4L2();

#endif

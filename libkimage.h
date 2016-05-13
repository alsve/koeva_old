/*
 * Authored by: alrayan
 * Date of Creation : May 13th, 2016
 * Bio-informathic Division
 * Departmen of Mehcanical and Engineering
 * Bogor Agricultural University
 */

#ifndef __LIBKIMAGE_H_
#define __LIBKIMAGE_H_

#include <stdio.h>
#include <unistd.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int koeva_cv_init();
int koeva_cv_shutdown();
int koeva_cv_dirInit();
int koeva_cv_cameraInit();
int koeva_cv_dirShutdown();
int koeva_cv_cameraShutdown();
int koeva_cv_captureImage(char* _filename);

#endif

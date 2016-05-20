/* File Name: libkudev.h
   Author: Alrayan
   Created on: 19 May 2016
   Description: libkudev is made to decide /dev/videoX of
                the camera dynamically
   Dependent File: camConfigure, camIndex.conf
*/

#include "libkudev.h"

int kindexBC;
int kindexTC;
const char* kpathCamIndex = "./camIndex.conf";

int koeva_udev_init()
{
        int krc;
        char command[255] = "";
        FILE* kcamIndexFile;

        strcat(command, "./camConfigure");
        krc = system(command);

        if (krc == 127) {
                fprintf(stderr, "FATAL ERROR: camConfigure script not found!");
                for(;;); // Looping program forever
        } else if (krc == -1) {
                fprintf(stderr, "FATAL ERROR: udevadm is not installed!");
                for(;;);
        } else if (krc == -2) {
                fprintf(stderr, "FATAL ERROR: Are cameras already connected?");
        }

        do{
                kcamIndexFile = fopen(kpathCamIndex, "r");
                if (kcamIndexFile == NULL) {
                        fprintf(stderr, "WARNING: Could not open camIndex.conf\n");
                        fprintf(stderr, "WARNING: Retrying to open camIndex.conf");
                        usleep(500 * 1000);
                }
        } while (kcamIndexFile == NULL);

        if (fscanf(kcamIndexFile, "%d", &kindexTC) != EOF);
        if (fscanf(kcamIndexFile, "%d", &kindexBC) != EOF);

        fclose(kcamIndexFile);

        return 0;
}

int koeva_udev_indexOfTopCamera_V4L2()
{
        return kindexTC;
}

int koeva_udev_indexOfBottomCamera_V4L2()
{
        return kindexBC;
}


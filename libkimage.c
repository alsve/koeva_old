#include "libkimage.h"
#include "libkudev.h"

const char* IMAGE_DIR = "koeva_image_directory";

int koeva_cv_dirInit()
{
        DIR* dir = opendir(IMAGE_DIR);
        if (dir) {
                closedir(dir);
                koeva_cv_dirShutdown();
                mkdir(IMAGE_DIR, 0777);
                return 1;
        } else if (ENOENT == errno) {
                mkdir(IMAGE_DIR, 0777);
                return 1;
        }

        
        return 0;
}

int koeva_cv_dirShutdown()
{
        int rc;
        char command[255] = "";
        strcat(command, "sudo rm -r ");
        strcat(command, IMAGE_DIR);
        rc = system(command);

        if (rc != 0) {
                fprintf(stderr, "ERROR: Something wrong with rm -r\n");
        }

        return 0;
}

int koeva_cv_captureImage(char* _filename, kcamera_t _which)
{
        char kpath_to_image[255] = "";
        IplImage* kframe = 0;
        CvCapture* kcamera = 0;

        char ktemp_int[10] = "";

        int kindexC;
        int ki;

        koeva_udev_init();

        switch(_which){
        case 0:
                kindexC = koeva_udev_indexOfTopCamera_V4L2();
                break;
        case 1:
                kindexC = koeva_udev_indexOfBottomCamera_V4L2();
                break;
        }

        kcamera = cvCaptureFromCAM(kindexC);

        for (ki = 0; ki < 10; ki++) {
                cvQueryFrame(kcamera);
                usleep(100 * 1);
        }

        sprintf(ktemp_int, "%d", _which);

        strcat(kpath_to_image, IMAGE_DIR);
        strcat(kpath_to_image, "/");
        strcat(kpath_to_image, _filename);
        strcat(kpath_to_image, "-");
        strcat(kpath_to_image, ktemp_int);
        strcat(kpath_to_image, ".jpg");

        kframe = cvQueryFrame(kcamera);
        
        if (kframe) {
                cvSaveImage(kpath_to_image, kframe, 0);
        } else {
                fprintf(stderr, "ERROR: Frame is not acquired!\n");
        }

        cvReleaseCapture(&kcamera);

        return 0;
}


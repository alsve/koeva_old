#include "libkimage.h"

const char* IMAGE_DIR = "koeva_image_directory";
CvCapture* kcamera0 = 0;
IplImage* kframe0 = 0;

int koeva_cv_init()
{
        koeva_cv_dirInit();
        koeva_cv_cameraInit();
        return 0;
}

int koeva_cv_shutdown()
{
        koeva_cv_dirShutdown();
        koeva_cv_cameraShutdown();
        return 0;
}

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

int koeva_cv_cameraInit()
{
        int ki = 0;

        kcamera0 = cvCaptureFromCAM(1);
        cvSetCaptureProperty(kcamera0,
                                CV_CAP_PROP_FRAME_WIDTH, 1024);
        cvSetCaptureProperty(kcamera0,
                                CV_CAP_PROP_FRAME_HEIGHT, 768);

        if (!kcamera0) {
                fprintf(stderr, "Could not open the camera0!\n");
        }

        //Skip 6 frame for stable image acquisition
        for (ki = 0; ki < 6; ki++) {
                cvQueryFrame(kcamera0);
                sleep(1);
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

int koeva_cv_cameraShutdown()
{
        cvReleaseCapture(&kcamera0);
        return 0;
}

int koeva_cv_captureImage(char* _filename)
{
        char kpath_to_image[255] = "";

        strcat(kpath_to_image, IMAGE_DIR);
        strcat(kpath_to_image, "/");
        strcat(kpath_to_image, _filename);
        strcat(kpath_to_image, ".jpg");

        kframe0 = cvQueryFrame(kcamera0);
        
        if (kframe0) {
                cvSaveImage(kpath_to_image, kframe0, 0);
        } else {
                fprintf(stderr, "ERROR: Frame is not acquired!\n");
        }

        return 0;
}

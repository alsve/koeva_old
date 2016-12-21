/* stream2cam
 * Enabling streaming 2 cameras concurrently
 * Stream will be redirected to gui using cvWindow
 * Author : Alrayan
 * Date : December 21 2016
 * Note : Linux is using FFMPEG to acquire images from USB web camera.
 *        So plugging two cameras at the same USB 2 controller will
 *        make USB Controller bottlenecked and resulting an error.
 *        To prevent this error, you need to plug each of USB web camera to
 *        different USB controller.
 *
 *        1. Check /dev/videoX if it is there or not.
 *        2. Udev assigns each camera to /dev/videoX in numerical order
 *           (e.g. the first plugged camera will be /dev/video0,
 *            the second camera will be /dev/video1, so on, and so on)
 *        3. After you know /dev/videoX node connect to what camera, 
 *           you can assign the node index to cvCaptureFromCAM(_index_)
 *
 *        4. You need to install opencv first in order to compile this
 *        5. Compile using following command where (CC) is C compiler :
 *           (CC) -o stream2cam stream2cam.c `pkg-config --libs opencv`
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
        IplImage* kframe = 0;
        IplImage* kframe2 = 0;
        CvCapture* kcamera = 0;
        CvCapture* kcamera2 = 0;

        kcamera = cvCaptureFromCAM(1);
        kcamera2 = cvCaptureFromCAM(2);

        cvNamedWindow("Stream camera 1", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Stream camera 2", CV_WINDOW_AUTOSIZE);

        while (cvWaitKey(10) != 27) {
                kframe = cvQueryFrame(kcamera);
                kframe2 = cvQueryFrame(kcamera2);

                cvShowImage("Stream camera 1", kframe);
                cvShowImage("Stream camera 2", kframe2);
        }

        cvDestroyAllWindows();
        cvReleaseImage(&kframe);
        cvReleaseImage(&kframe2);

        return 0;
}

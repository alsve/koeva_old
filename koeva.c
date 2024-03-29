/* koeva.c 
 * author : Alrayan
 * date of creation : April 22, 2016
 * library : ardunio-serial (Tod E. Kurt, 2006)
 * License : inherited from MIT license
 */

#include "libkoeva.h"
#include "libkimage.h"

#define KBAUD_RATE 9600

char* kdevice_path = "/dev/ttyUSB0";

void koeva_menu_main()
{
        koeva_lcd_clear();
        koeva_lcd_write(0, 0, "-------KOEVA--------");
        koeva_lcd_write(0, 1, "Insert GBC to tray  ");
        koeva_lcd_write(0, 2, "A:Proceed           ");
        koeva_lcd_write(0, 3, "B:Shutdown device   ");
}

void koeva_menu_imageProcessing()
{
        int ki;
        char kprocBar[21] = "    [          ]    ";
        char* ktemp_charpint = malloc(sizeof(char) * 10);

        koeva_lcd_clear();
        koeva_lcd_write(0, 1, " PROCESSING IMAGE:  ");

        for (ki = 5; ki < 15;ki++) {
                kprocBar[ki] = '*';
                koeva_lcd_write(0, 2, kprocBar);
                usleep(500000);
        }

        sprintf(ktemp_charpint, "%d", koeva_image_getNthEditor());

        koeva_cv_captureImage(ktemp_charpint, 0);
        koeva_cv_captureImage(ktemp_charpint, 1);

        //Increase defect rate (dummy)
        koeva_addDefect(5);
        //Set nth-Tray processed Green Bean Coffee
        koeva_image_setCurrentKprocGBC(66);

        free(ktemp_charpint);
}

void koeva_menu_gradePrint()
{
        int kgrade = koeva_getGrade();
        char* ktemp = malloc(sizeof(char) * 2);
        sprintf(ktemp, "%d", kgrade);
        koeva_lcd_clear();
        koeva_lcd_write(0, 0, "Based on SNI Number:");
        koeva_lcd_write(0, 1, "    1-2907-2008     ");
        koeva_lcd_write(0, 2, "The GBC grade is (");
        koeva_lcd_write(18, 2, ktemp);
        koeva_lcd_write(19, 2, ")");
        koeva_lcd_write(1, 3, "A:Restart Scanning ");

        free(ktemp); 
}

void koeva_menu_nthEditor()
{
        koeva_lcd_clear();
        koeva_lcd_write(0, 0, "-------KOEVA--------");
        koeva_lcd_write(0, 1, "Set tray n-th =     ");
        koeva_lcd_write(0, 2, "A:Enter    B:Clear  ");
        koeva_lcd_write(0, 3, "C:Cancel            ");
}

void koeva_menu_summary()
{
        unsigned int kprocGBC = koeva_image_getSumKprocGBC();
        unsigned int kprocTray = koeva_image_getNthEditor();
        char* ktemp = malloc(sizeof(char) * 5);

        memset(ktemp, 0, 5);
        sprintf(ktemp, "%d", kprocGBC);

        koeva_lcd_clear();
        koeva_lcd_write(0, 0, "Processed GBC :");
        koeva_lcd_write(16, 0, ktemp);

        memset(ktemp, 0, 5);
        sprintf(ktemp, "%d", kprocTray);

        koeva_lcd_write(0, 1, "Processed tray:");
        koeva_lcd_write(16, 1, ktemp);
        koeva_lcd_write(0, 2, "A:NextTray B:Evaluat");
        koeva_lcd_write(0, 3, "C:Re-scan  D:Restart");

        free(ktemp);
}

void koeva_menu_confirmation() {
        koeva_lcd_clear();
        koeva_lcd_write(0, 1, "   Are you sure?    ");
        koeva_lcd_write(0, 2, "  A:Yes     B:No    ");
}

int koeva_main(void)
{
        char kwhere = 'a';
        char kinput;

        int kquit = 0;
        int kselectedInt = 0;
        int ktemp = 0;

        while (kquit != 1) {
                switch(kwhere){
                case 'a':
                        koeva_cv_dirInit();
                        koeva_menu_main();
                        koeva_serial_flush();
                        kinput = koeva_getSelectedFrom("AB");
                        if (kinput == 'A') kwhere = 'b';
                        else kwhere = 'q';
                        break;
                case 'b':
                        // Image processing goes here!
                        koeva_image_incNthEditor();
                        koeva_menu_imageProcessing();
                        kwhere = 's';
                        break;
                case 'c':
                        koeva_evaluate();
                        koeva_menu_gradePrint();
                        koeva_serial_flush();
                        kinput = koeva_getSelectedFrom("A");
                        if (kinput == 'A') {
                                kwhere = 'a';
                                koeva_paramReset();
                        }
                        else kwhere = 'q';
                        break;
                //Case d is only for quality checker users.
                case 'd':
                        koeva_menu_nthEditor();
                        koeva_serial_flush();
                        do {
                                koeva_lcd_write(16, 1, "   ");
                                kinput = koeva_lcd_getInteger(16, 1, "ABC", 2, &kselectedInt);
                                if (kinput == 'A' && kselectedInt > 0) {
                                        koeva_image_setNthEditor(kselectedInt - 1);
                                        kwhere = 'b';
                                        ktemp = 0;
                                } else if (kinput == 'A' && kselectedInt < 0) ktemp = 0;
                                else if (kinput == 'B') ktemp = 32;
                                else if (kinput == 'C') {
                                        kwhere = 's';
                                        ktemp = 0;
                                } else kwhere = 'q';
                        } while (ktemp == 32);
                        break;
                case 's':
                        koeva_menu_summary();
                        koeva_serial_flush();
                        kinput = koeva_getSelectedFrom("ABCD");
                        if (kinput == 'A') {
                                kwhere = 'b';
                        }
                        else if (kinput == 'B') kwhere = 'c';
                        else if (kinput == 'C') {
                                kwhere = 'b';
                                koeva_image_decNthEditor();
                        } else if (kinput == 'D') {
                                koeva_serial_flush();
                                koeva_menu_confirmation();
                                kinput = koeva_getSelectedFrom("AB");
                                if (kinput == 'A') {
                                        kwhere = 'a';
                                        koeva_paramReset();
                                } else kwhere = 's';
                        }
                        else kwhere = 'q';
                        break;
                case 'q':
                        kquit = 1;
                        koeva_cv_dirShutdown();
                        break;
                default:
                        printf("WARNING: Koeva, Where are we?\n");
                        koeva_die("Fall into eternity...");
                }
        }
        return 0;
}

int main(void)
{
        koeva_lcd_init(kdevice_path, KBAUD_RATE);
        koeva_rpiReady();
        koeva_isLcdReady();
        koeva_main();
        koeva_shutdown();
        return 0;
}

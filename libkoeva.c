/* KOEVA_arduino_serial 
 * author : Alrayan
 * date of creation : April 22, 2016
 * library : ardunio-serial (Tod E. Kurt, 2006)
 * License : inherited from MIT license
 */

#include "libkoeva.h"

#define KSLEEP_BETWEEN 100000

void koeva_die(char* _msg)
{
        fprintf(stderr, "ERROR: %s\n", _msg);
        exit(1);
}

int koeva_lcd_init(char* _ttyUSB, int _baudrate)
{
        if (access(_ttyUSB, F_OK) == 0) {
                printf("Device path: %s\n", _ttyUSB);
                strcpy(kttyUSB, _ttyUSB);
        }

        kbrate = _baudrate;
        klcd_state = koeva_serialport_init(kttyUSB, kbrate);

        if (klcd_state == -1)
                koeva_die("Tidak dapat membuka port LCD!");

        koeva_serialport_flush(klcd_state);

        return 0;
}

int koeva_lcd_write(unsigned int _posX, unsigned int _posY, char* _kstring)
{
        char posX[3];
        char posY[3];
        char kstring[MAX_BUFF] = "";

        if (klcd_state == -1)
                koeva_die("Port LCD tidak terbuka!");

        if(_posX > 20 || _posY > 4)
                koeva_die("posX or posY is not in allowable value!");

        sprintf(posX, "%d", _posX);
        if (strlen(posX) == 1)
                sprintf(posX, "0%d", _posX);

        sprintf(posY, "%d", _posY);
        if (strlen(posY) == 1)
                sprintf(posY, "0%d", _posY);

        strcat(kstring, posX);
        strcat(kstring, " ");
        strcat(kstring, posY);
        strcat(kstring, " ");
        strcat(kstring, _kstring);
        strcat(kstring, ";");

        printf("%s\n", kstring);

        krc = koeva_serialport_write(klcd_state, kstring);

        if (krc == -1)
                koeva_die("Error in writing!");

        usleep(KSLEEP_BETWEEN);

        return 0;
}

int koeva_lcd_clear()
{
        koeva_serialport_write(klcd_state, "c;");
        usleep(KSLEEP_BETWEEN);
        return 0;
}

int koeva_rpiReady()
{
        ksumDefect = 0;
        kgrade = 0;
        knthImage = 0;
        koeva_serialport_write(klcd_state, "R");
        printf("WARNING: Raspberry is ready!\n"
               "WARNING: Waiting for LCD and keypad...\n");

        return 0;
}

int koeva_isLcdReady()
{
        char* temp = "";
        int timeout = 10;
        koeva_serialport_read_until(klcd_state, temp, 'R', 1, timeout);
        printf("WARNING: LCD and keypad ready!\n");

        sleep(4);

        return 0;
}

int* koeva_getKlcdState()
{
        return &klcd_state;
}

char koeva_getSelectedFrom(char* _selection)
{
        int ki;
        int kj;
        char kb[1];  // read expects an array, so we give it a 1-byte array
        char kselected;        
        char* kselection = malloc(sizeof(char) * 5);
        
        strcpy(kselection, _selection);
        int kselen = strlen(kselection);

        kj = 0;

        do { 
                int kn = read(klcd_state, kb, 1);  // read a char at a time
                if( kn==-1) return 0;    // couldn't read
                if( kn==0 ) {
                    usleep( 1 * 1000 );  // wait 1 msec try again
                    continue;
                }

                for (ki = 0; ki < kselen; ki++) {
                        if (kb[0] == kselection[ki]) {
                                kj = 10;
                                break;
                        }
                }
        } while( kj != 10 );

        kselected = kb[0];
        
        return kselected;
}

void koeva_intNullFormatter(int _kint, int _buffSize, char* _dest)
{
        int kinlen;
        int i;
        char* ktemp = malloc(sizeof(char) * (_buffSize + 1));
        char* ktemp2 = malloc(sizeof(char) * (_buffSize + 1));
        sprintf(ktemp, "%d", _kint);

        kinlen = strlen(ktemp);

        for (i = 0; i < (kinlen - _buffSize); i++) {
                strcat(ktemp2, "0");
        }

        strcat(ktemp2, ktemp);
        strcat(_dest, ktemp2);

        free(ktemp);
        free(ktemp2);
}

int koeva_shutdown()
{
        koeva_lcd_clear();
        koeva_lcd_write(0, 1, "  SHUTTING DOWN...  ");
        sleep(1);
        koeva_lcd_clear();
        return koeva_serialport_close(klcd_state);
}

int koeva_addDefect(unsigned int _add)
{
        ksumDefect += _add;        
        return 0;
}

int koeva_evaluate()
{
        if (ksumDefect < 12) {
                kgrade = 1;
        } else if (ksumDefect < 26) {
                kgrade = 2;
        } else if (ksumDefect < 45) {
                kgrade = 3;
        } else if (ksumDefect < 81) {
                kgrade = 4;
        } else if (ksumDefect < 151) {
                kgrade = 5;
        } else kgrade = 6;

        return kgrade;
}

int koeva_getGrade()
{
        return kgrade;
}

int koeva_paramReset()
{
        kgrade = 0;
        ksumDefect = 0;

        return 0;
}

void koeva_image_setNthEditor(int _nth)
{
        knthImage = _nth;
}

void koeva_image_incNthEditor()
{
        kmaxImage = (knthImage + 1) < kmaxImage ? kmaxImage : (knthImage + 1);

        knthImage++;
}

void koeva_image_decNthEditor()
{
        knthImage--;
}

unsigned int koeva_image_getNthEditor()
{
        return knthImage;
}

int koeva_getStringFrom(char _terminator, int _buff_max, char* _dst)
{
        char kb[1];  // read expects an array, so we give it a 1-byte array
        int  ki = 0;
        
        char* kdst = malloc(sizeof(char) * _buff_max);

        do { 
                int n = read(klcd_state, kb, 1);  // read a char at a time

                if( n==-1) return -1;    // couldn't read
                if( n==0 ) {
                        usleep( 1 * 1000 );  // wait 1 msec try again
                        continue;
                }

                kdst[ki] = kb[0];
                ki++;
        } while( kb[0] != _terminator && ki < _buff_max);

        kdst[ki] = 0;  // null terminate the string

        strcpy(_dst, kdst);

        free(kdst);

        return 0;
}

int koeva_getIntFrom(char _terminator, int _maxDigit, int* _dst)
{
        int ki;
        int kii;
        int kj;
        char kb[1];  // read expects an array, so we give it a 1-byte array
        char* kdst = malloc(sizeof(char) * _maxDigit);
        char* kselection = malloc(sizeof(char) * 11);
        
        strcpy(kselection, "1234567890");

        kii= 0;
        kj = 0;

        do { 
                int kn = read(klcd_state, kb, 1);  // read a char at a time
                if( kn == -1) return 0;    // couldn't read
                if( kn == 0) {
                    usleep( 1 * 1000 );  // wait 1 msec try again
                    continue;
                }

                for (ki = 0; ki < 11; ki++) {
                        if (kb[0] == kselection[ki] && kii < _maxDigit) {
                                kdst[kii] = kb[0];
                                kii++;
                        } else if (kb[0] == _terminator) {
                                kj = 10;
                                break;
                        }
                }

        } while(kj != 10);
        
        *_dst = atoi(kdst);
        free(kdst);

        return 0;
}

char koeva_lcd_getInteger(int _posX, int _posY, char* _selection, int _maxDigit, int* _dst)
{
        //Return type : char
        //Return mean : selected terminator from _selection
        //Param @ _posX : starting column position of writing
        //Param @ _posY : starting row position of writing int
        //Param @ _selection : an array of terminator char
        //Param @ _maxDigit : maximum digits that could be an input
        //Param @ _dst : Passing by pointer destinated integer which will be altered

        int ki;
        int kii;
        int kj;
        int ktempInteger;
        char kb[1];  // read expects an array, so we give it a 1-byte array
        char kselected;
        char* kdst = malloc(sizeof(char) * _maxDigit);
        char* kselection = malloc(sizeof(char) * 11);

        char* kselectionChar = malloc(sizeof(char) * 5);
        
        strcpy(kselectionChar, _selection);
        int kselen = strlen(kselectionChar);
        
        strcpy(kselection, "1234567890");

        kii= 0;
        kj = 0;

        do { 
                int kn = read(klcd_state, kb, 1);  // read a char at a time
                if( kn == -1) return 0;    // couldn't read
                if( kn == 0) {
                    usleep( 1 * 1000 );  // wait 1 msec try again
                    continue;
                }

                for (ki = 0; ki < 11; ki++) {
                        if (kb[0] == kselection[ki] && kii < _maxDigit) {
                                kdst[kii] = kb[0];
                                kdst[kii + 1] = 0;
                                ktempInteger = koeva_kmaxImage_nthImage(atoi(kdst));
                                sprintf(kdst, "%d", ktempInteger);
                                koeva_lcd_write(_posX, _posY, kdst);
                                kii++;
                        }
                }

                for (ki = 0; ki < kselen; ki++) {
                        if (kb[0] == kselectionChar[ki]) {
                                kj = 10;
                                break;
                        }
                }

        } while(kj != 10);
        
        *_dst = atoi(kdst);
        free(kdst);

        kselected = kb[0];

        return kselected;
}

int koeva_kmaxImage_nthImage(unsigned int _nthImage)
{
        // If param _nthImage is greater than kmaxImage

        if (kmaxImage < _nthImage) {
                return kmaxImage;
        } else return _nthImage;
}

int koeva_serial_flush()
{
        return tcflush(klcd_state, TCIOFLUSH);
}

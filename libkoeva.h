/* arduino-send-string-test
 * author : Alrayan
 * date of creation : April 22, 2016
 * library : ardunio-serial (Tod E. Kurt, 2006)
 * 
 */

#ifndef __LIBKOEVA_H_
#define __LIBKOEVA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "libkserial.h"

#define MAX_BUFF 256

int krc;
int klcd_state;
int kbrate;
int kverbose;

unsigned int ksumDefect;
unsigned int kgrade;
unsigned int knthImage;
unsigned int kmaxImage;

char kttyUSB[MAX_BUFF];

void koeva_die(char* _msg);
void koeva_intNullFormatter(int _kint, int _buffSize, char* _dest);
void koeva_image_setNthEditor(int _nth);
void koeva_image_incNthEditor();
void koeva_image_decNthEditor();
int koeva_lcd_init(char* _ttyUSB, int _baudrate);
int koeva_lcd_write(unsigned int _posX, unsigned int _posY, char* kstring);
int koeva_getIntFrom(char _terminator, int _maxDigit, int* _dst);
int koeva_getStringFrom(char _terminator, int _buff_max, char* _dst);
int koeva_lcd_clear();
int koeva_rpiReady();
int koeva_isLcdReady();
int koeva_shutdown();
int koeva_evaluate();
int koeva_getGrade();
int koeva_addDefect();
int koeva_paramReset();
int* koeva_getKlcdState();
int koeva_serial_flush();
int koeva_kmaxImage_nthImage(unsigned int _nthImage);
unsigned int koeva_image_getNthEditor();

char koeva_getSelectedFrom(char* _selection);
char koeva_lcd_getInteger(int _posX, int _posY, char* _selection, int _maxDigit, int* _dst);

#endif

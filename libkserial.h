//
// arduino-serial-lib -- simple library for reading/writing serial ports
//
// 2006-2013, Tod E. Kurt, http://todbot.com/blog/
//


#ifndef __LIBKSERIAL_H_
#define __LIBKSERIAL_H_

#include <stdint.h>   // Standard types 
#include <stdio.h>    // Standard input/output definitions 
#include <unistd.h>   // UNIX standard function definitions 
#include <fcntl.h>    // File control definitions 
#include <errno.h>    // Error number definitions 
#include <termios.h>  // POSIX terminal control definitions 
#include <string.h>   // String function definitions 
#include <sys/ioctl.h>

int koeva_serialport_init(const char* serialport, int baud);
int koeva_serialport_close(int fd);
int koeva_serialport_writebyte( int fd, uint8_t b);
int koeva_serialport_write(int fd, const char* str);
int koeva_serialport_read_until(int fd, char* buf, char until, int buf_max,int timeout);
int koeva_serialport_flush(int fd);

#endif


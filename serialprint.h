/*
 * serial.h
 */ 

#ifndef SERIAL_H
#define SERIAL_H

void serial_start(int prescaler);

void serial_push(unsigned char data);

void print(char* pr);
#endif


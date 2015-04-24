/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the µVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

//#include <stdio.h>
#include <rt_misc.h>
#include <stdio.h>
//#pragma import(__use_no_semihosting_swi)


extern void sendchar (char c);
extern char getkey(void);

typedef void (*WriteFunction)(char);
typedef char (*ReadFunction)(void);

struct __FILE { 
	void * handle;
};

FILE __stdout={(void*)sendchar};
FILE __stdin={(void*)getkey};


int fputc(int c, FILE *f) {
	((WriteFunction)f->handle)(c);
	return c;
}


int fgetc(FILE *f) {
  return ((ReadFunction)f->handle)();
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

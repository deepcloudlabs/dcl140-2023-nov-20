/*******************************************************************************
This module contains declarations and definitions for module utils.c

Note that modules linking with utils.c need to link with rt library (-lrt).
/******************************************************************************/

/* Reentrant function that takes a character string, prepends a fractional
time to it, and then prints it out. */
void printWithTime(char *);

/* Reentrant function that pauses for a time that can include a fraction of a
second. */
void fractSleep(double);

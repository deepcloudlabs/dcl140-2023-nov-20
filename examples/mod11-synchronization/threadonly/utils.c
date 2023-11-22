/*******************************************************************************
This module implements some utility functions used by other modules.

Modules using this module must compile with -lrt to get nanosleep(3R).

/******************************************************************************/

#include <stdio.h>		/* Standard IO defs. */
#include <sys/time.h>		/* Needed for gettimeofday() defs. */
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"		/* Forward refs and defs for this module. */

#define STDOUT_FD 1

static int numTimeChars = -1;	/* Stores size of built time string. */

/******************************************************************************/
void printWithTime(char * string) {
/*******************************************************************************
Reentrant routine which prepends a relative high-resolution time to a string
and then prints it.
/******************************************************************************/
  char timeBuffer[20];	/* Where time string is built. */
  struct timeval time;	/* Where current time is retrieved. */

  gettimeofday(&time, NULL);	/* Get current time. */

  /* Build time string. */
  sprintf (timeBuffer,"%.4ld,%.6ld ", time.tv_sec % 10000, time.tv_usec);

  /* Initialize time string size the first time through. */
  if (numTimeChars == -1) {
    numTimeChars = strlen(timeBuffer);
  }

  /* Write the time followed by the input string to stdout. */
  write (STDOUT_FD, timeBuffer, numTimeChars);
  write (STDOUT_FD, string, strlen(string));
}


/******************************************************************************/
void fractSleep(double time) {
/*******************************************************************************
Routine to delay for fractions of a second.
/******************************************************************************/

  /* Place where nanosleep arg is built from input arg. */
  struct timespec timeSpec;

  /* Truncate fraction in int conversion. */
  timeSpec.tv_sec = (time_t)time;

  /* Load fraction. */
  timeSpec.tv_nsec = (int)((time - timeSpec.tv_sec) * 1000000000);

  /* Delay. */
  nanosleep(&timeSpec, NULL);
}

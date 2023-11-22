/*
 *  timer.c : contains two timing functions to measure process time
 */

#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include "shell.h"

/* Storage for baseline times. */
static clock_t old;

/* Save a baseline of user and system CPU times, plus wallclock time. */
void set_timer(void) {
  struct tms tmbuf;

  /* Fill in code. */
}


/* Get second set of times, and take delta to get wallclock time.  Display. */
void stop_timer(void) {
  struct tms tmbuf;
  clock_t new;
  double ticks;

  /* Get delta times and print them out. */
  /* Fill in code. */
}


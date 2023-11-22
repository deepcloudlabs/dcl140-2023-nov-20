#!/bin/csh

# This file runs two instances of proc_sem, and demonstrates
# how the semaphore coordinates multiple threads in multiple
# processes.

# Output of both instances are merged and sorted by time
# into sortedSem.dat

rm -f /tmp/semaphore.dat
./proc_sem >sem1.dat &
./proc_sem >sem2.dat &
echo "Waiting for proc 1 to finish..."
wait
echo "Waiting for proc 2 to finish..."
wait
echo "Sorting..."
cat sem1.dat sem2.dat | sort >sortedSem.dat

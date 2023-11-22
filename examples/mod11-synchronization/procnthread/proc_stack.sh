#!/bin/csh

# This file runs two instances of proc_stack, and demonstrates
# how a mutexes and condition variables coordinate multiple
# threads in multiple processes.

# Output of both instances are merged and sorted by time
# into sortedStack.dat

rm -f /tmp/proc_stack.dat
./proc_stackMain >stack1.dat &
./proc_stackMain >stack2.dat &
echo "Waiting for proc 1 to finish..."
wait
echo "Waiting for proc 2 to finish..."
wait
echo "Sorting..."
cat stack1.dat stack2.dat | sort -k 5,5 >sortedStack.dat

#!/bin/bash
trap "echo hello mars" INT
trap "echo hello sun" QUIT
trap "echo hello moon" TSTP
while (( 1 ))
do
   echo -n "Input: "
   read string

done
print "Bye..."

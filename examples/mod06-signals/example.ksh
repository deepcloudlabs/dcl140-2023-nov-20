#!/bin/bash
trap "exec xcalc" INT
trap "exec xcalc" QUIT
trap "exec xcalc" TSTP
while (( 1 ))
do
   echo -n "Input: "
   read string

done
print "Bye..."

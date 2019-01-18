#!/bin/bash

i=1
return=0
while [ $return -eq 0 ]
do
#	output=` (curl http://localhost:8080/index.html 2>&1)  ; return=$?`
#	echo "Return is:$return: output is $output"
#
	utime=`{ time curl http://localhost:8080/index.html > /dev/null; } 2>&1  | egrep -i user`
	echo "$?:$i: $utime"
	i=$((i+1))
done
echo $return:$output

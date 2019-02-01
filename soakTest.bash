#!/bin/bash

i=1
ret="0"
while [ $ret = 0 ]
do
#
	utime=`{ time (curl http://localhost:8080/index.html > soakTest.response.out 2>&1) 2>/dev/null; echo $? > soakTest.return.out; } 2>&1  | egrep -i user`
	ret=`cat soakTest.return.out`
	response=`head -4 soakTest.response.out | tail -1 | sed 's/.*curl//'`
	echo "$ret:$i: $utime: $response"
	i=$((i+1))
done

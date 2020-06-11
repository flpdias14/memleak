#!/bin/bash
rm -f size.log valgrind-out.txt


gcc -g -o xmemleak main.c

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./xmemleak &

pid_of_mem_leak=`ps -ef | grep [x]memleak | awk '{print $2}'`




function log_memory {
    ps --no-headers -o size -p $pid_of_mem_leak >> size.log
}    

function interrupt()
{
    log_memory
    kill -9 $pid_of_mem_leak
    echo -e "\n\nEnded."
    log_memory
    exit
}

trap interrupt INT

while [ True ]
do
log_memory

done 
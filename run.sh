#!/bin/bash

function log_memory 
{
    ps --no-headers -o size -p $pid_of_mem_leak >> size.log
}    

function interrupt
{
    log_memory
    kill -9 $pid_of_mem_leak
    echo -e "\n\nEnded."
    log_memory
    exit
}

trap interrupt INT

valgrind --version
t=`echo $?` 
if [ $t == '0' ];
then
    rm -f size.log valgrind-out.txt


    gcc -g -o xmemleak main.c

    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./xmemleak &

    pid_of_mem_leak=`ps -ef | grep [x]memleak | awk '{print $2}'`    

    while [ True ]
    do
    log_memory

done 
else
    echo "no"
fi
#!/bin/bash
pid_of_mem_leak=0
# log memory size in kilobytes
function log_memory 
{
    ps --no-headers -o size -p $pid_of_mem_leak >> size.log
}    

function kill_process
{
    kill $pid_of_mem_leak > /dev/null
}

function restore_aplication
{
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./xmemleak --restore &>> output.log &
    pid_of_mem_leak=`ps -ef | grep [x]memleak | awk '{print $2}'` 
}

function rejuvenate 
{
    kill_process
    restore_aplication
}
function interrupt
{
    log_memory
    kill_process
    echo -e "\n\nEnded."
    log_memory
    exit
}

trap interrupt INT

valgrind --version
t=`echo $?` 
if [ $t == '0' ];
then
    rm -f size.log valgrind-out.txt output.log time_to_rejuvenate.log
    touch time_to_rejuvenate.log

    gcc -Wall -pedantic -g -o xmemleak main.c

    # start application
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./xmemleak  &>> output.log & 

    pid_of_mem_leak=`ps -ef | grep [x]memleak | awk '{print $2}'`    
    cont=0
    while [ True ]
    do
    log_memory
    last=`tail -n 1 size.log`
    if [ $last -gt 2097152 ]; # 2GB = 2097152 KB
    then
       START=$(date +%s.%N)
       rejuvenate
       END=$(date +%s.%N)
       DIFF=$( echo "scale=3; (${END} - ${START})*1000/1" | bc )
       echo "${DIFF}" >> time_to_rejuvenate.log

    fi
    sleep 10
    done
else
    echo "no"
fi
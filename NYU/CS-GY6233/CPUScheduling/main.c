#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"

int is_NULL_PCB(struct PCB inpcb)
{
    if (inpcb.process_id == 0 && inpcb.arrival_timestamp == 0 && inpcb.total_bursttime == 0 && inpcb.execution_starttime == 0 && inpcb.execution_endtime == 0 && inpcb.remaining_bursttime == 0 && inpcb.process_priority == 0)
        return 1;
    else
        return 0;
}

void setup_pcb(struct PCB *inpcb, int process_id, int arrival_timestamp, int total_bursttime, int execution_starttime, int execution_endtime, int remaining_bursttime, int process_priority)
{
    (*inpcb).process_id = process_id;
    (*inpcb).arrival_timestamp = arrival_timestamp;
    (*inpcb).total_bursttime = total_bursttime;
    (*inpcb).execution_starttime = execution_starttime;
    (*inpcb).execution_endtime = execution_endtime;
    (*inpcb).remaining_bursttime = remaining_bursttime;
    (*inpcb).process_priority = process_priority;
}

void printPCB(struct PCB *process)
{
    printf("------------------\n");
    printf("Process ID:   %d\n", (*process).process_id);
    printf("Timestamp:    %d\n", (*process).arrival_timestamp);
    printf("Total Burst:  %d\n", (*process).total_bursttime);
    printf("Start Time:   %d\n", (*process).execution_starttime);
    printf("End Time:     %d\n", (*process).execution_endtime);
    printf("Remain Busrt: %d\n", (*process).remaining_bursttime);
    printf("Priorty:      %d\n", (*process).process_priority);
    printf("------------------\n");
}

void printQueue(struct PCB ready_queue[QUEUEMAX], int length)
{
    int i;

    for (i = 0; i < length; i++)
        printPCB(&ready_queue[i]);
}

int main(int argc, char** argv)
{
    struct PCB ready_queue[QUEUEMAX];
    //struct PCB current_process;
    //struct PCB new_process;
    struct PCB process1;
    struct PCB process2;
    struct PCB process3;

    struct PCB returned_pcb;
    //struct PCB nullpcb;

    int queue_cnt = 3;
    int timestamp = 2;
    //int time_quantum = 5;

    setup_pcb(&process1,1,1,4,0,0,4,23);
    setup_pcb(&process2,2,1,4,0,0,4,22);
    setup_pcb(&process3,3,1,4,0,0,4,24);
    
    //setup_pcb(&current_process,1,1,4,1,5,4,8);
    //setup_pcb(&new_process,2,2,3,0,0,3,6);
    //setup_pcb(&nullpcb,0,0,0,0,0,0,0);

    ready_queue[0] = process1;
    ready_queue[2] = process2;
    ready_queue[1] = process3;

    printQueue(ready_queue, 3);

    returned_pcb = handle_process_completion_pp(ready_queue,&queue_cnt,timestamp);
    printf("============================\n");
    printPCB(&returned_pcb);
    printf("QueueCnt: %d\n", queue_cnt);
    printQueue(ready_queue, 2);
    //returned_pcb = handled_process_arrival_pp(ready_queue,&queue_cnt,current_process,new_process,timestamp);
    //returned_pcb = handle_process_arrival_rr(ready_queue,&queue_cnt,nullpcb,new_process,timestamp, 5);
    
    /*
     * quantum = 5, total = 3, timestamp = 2 :: end = 5
     * quantum = 2, total = 3, timestamp = 2 :: end = 4
     */
    return 0;
}

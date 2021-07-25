#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"

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

void setup_mem(struct MEMORY_BLOCK *inmb, int start_address, int end_address, int segment_size, int process_id)
{
    (*inmb).start_address = start_address;
    (*inmb).end_address = end_address;
    (*inmb).segment_size = segment_size;
    (*inmb).process_id = process_id;
}

void printMEM(struct MEMORY_BLOCK *inmb)
{
    printf("------------------\n");
    printf("Start addr:   %d\n", (*inmb).start_address);
    printf("End addr:     %d\n", (*inmb).end_address);
    printf("Segment Size: %d\n", (*inmb).segment_size);
    printf("process_id:   %d\n", (*inmb).process_id);
    printf("------------------\n");
}

void printMap(struct MEMORY_BLOCK map[MAPMAX], int length)
{
    int i;

    for (i = 0; i < length; i++)
        printMEM(&map[i]);
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

void setupRCB(struct RCB *inRCB, int request_id, int arrival_timestamp, int cylinder, int address, int process_id)
{
    (*inRCB).request_id = request_id;
    (*inRCB).arrival_timestamp = arrival_timestamp;
    (*inRCB).cylinder = cylinder;
    (*inRCB).address = address;
    (*inRCB).process_id = process_id;
}

void printRCB(struct RCB *request)
{
    printf("------------------\n");
    printf("Request ID:   %d\n", (*request).request_id);
    printf("Arrival Time: %d\n", (*request).arrival_timestamp);
    printf("Cylinder:     %d\n", (*request).cylinder);
    printf("Address:      %d\n", (*request).address);
    printf("Process ID:   %d\n", (*request).process_id);
    printf("------------------\n");
}

void printRCBQueue(struct RCB request_queue[QUEUEMAX], int length){
    int i;

    for (i = 0; i < length; i++)
        printRCB(&request_queue[i]);
}


int main(int argc, char** argv)
{
    int queue_cnt = 3;
    int current_cylinder = 58;
    int scan_direction = 1;
    struct RCB request_queue[QUEUEMAX];
    struct RCB request0;
    struct RCB request1;
    struct RCB request2;
    struct RCB request3;

    setupRCB(&request0,1,52,57,57,1);
    setupRCB(&request1,2,51,56,56,2);
    setupRCB(&request2,3,53,55,55,3);

    printRCB(&request0);
    //printRCB(&request1);

    request_queue[0] = request0;
    request_queue[1] = request1;
    request_queue[2] = request2;

    printRCBQueue(request_queue,queue_cnt);
    printf("======================\n");
    //struct MEMORY_BLOCK returned_mem = next_fit_allocate(10,map,&map_cnt,32,42);
    //struct RCB returned_req = handle_request_arrival_fcfs(request_queue, &queue_cnt, request1, request0, 2);
    struct RCB returned_req = handle_request_completion_look(request_queue, &queue_cnt, current_cylinder, scan_direction);
    printf("======================\n");
    printRCBQueue(request_queue,queue_cnt);
    printf("Returned request\n");
    printRCB(&returned_req);

    /*
    printMap(map1e,map1e_cnt);
    struct MEMORY_BLOCK returned_mem = best_fit_allocate(10,map1e,&map1e_cnt,32);
    printMap(map1e, map1e_cnt);
    */

    return 0;
}

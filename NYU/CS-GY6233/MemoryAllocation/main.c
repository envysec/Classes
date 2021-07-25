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

int main(int argc, char** argv)
{
    int map1e_cnt = 1;
    int map_cnt = 5;

    struct MEMORY_BLOCK map[MAPMAX];
    struct MEMORY_BLOCK map1e[MAPMAX];
    struct MEMORY_BLOCK block0;
    struct MEMORY_BLOCK block1;
    struct MEMORY_BLOCK block2;
    struct MEMORY_BLOCK block3;
    struct MEMORY_BLOCK block4;
    
    struct MEMORY_BLOCK block1e;
    struct MEMORY_BLOCK block2e;
    
    setup_mem(&block0, 0, 7, 8, 12);
    setup_mem(&block1, 8, 15, 8, 5);
    setup_mem(&block2, 16, 23, 8, 13);
    setup_mem(&block3, 24, 27, 4, 0);
    setup_mem(&block4, 28, 29, 2, 11);

    setup_mem(&block1e, 0, 1023, 1024, 0);

    map[0] = block0;
    map[1] = block1;
    map[2] = block2;
    map[3] = block3;
    map[4] = block4;

    map1e[0] = block1e;

    printMap(map,map_cnt);
    printf("======================\n");
    //struct MEMORY_BLOCK returned_mem = next_fit_allocate(10,map,&map_cnt,32,42);
    release_memory(block2, map, &map_cnt);
    printf("======================\n");
    printMap(map,map_cnt);

    /*
    printMap(map1e,map1e_cnt);
    struct MEMORY_BLOCK returned_mem = best_fit_allocate(10,map1e,&map1e_cnt,32);
    printMap(map1e, map1e_cnt);
    */

    return 0;
}

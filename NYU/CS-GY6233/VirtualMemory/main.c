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

void setupPTE(struct PTE *inPTE, int is_valid, int frame_number, int arrival_timestamp, int last_access_timestamp, int reference_count)
{
    (*inPTE).is_valid = is_valid;
    (*inPTE).frame_number = frame_number;
    (*inPTE).arrival_timestamp = arrival_timestamp;
    (*inPTE).last_access_timestamp = last_access_timestamp;
    (*inPTE).reference_count = reference_count;
}

void printPTE(struct PTE *inPTE)
{
    printf("------------------\n");
    printf("Valid: %d\n", (*inPTE).is_valid);
    printf("Frame Number: %d\n", (*inPTE).frame_number);
    printf("Arrival Time: %d\n", (*inPTE).arrival_timestamp);
    printf("Last Access:  %d\n", (*inPTE).last_access_timestamp);
    printf("Ref Count:    %d\n", (*inPTE).reference_count);
    printf("------------------\n");

}

void printTable(struct PTE page_table[TABLEMAX], int length)
{
    int i;

    for (i = 0; i < length; i++)
	printPTE(&page_table[i]);
}

void printPool(int frame_pool[TABLEMAX], int length)
{
    int i;
    
    printf("---\n");
    for (i = 0; i < length; i++)
	printf("%d\n", frame_pool[i]);
    printf("---\n");
}
int main(int argc, char** argv)
{
    int table_cnt = 8;
    int page_number = 0;
    int frame_pool[] = {};
    int frame_cnt = 0;
    int current_timestamp = 12;
    int reference_string[] = {0,3,2,6,3,4,5,2,6,4,5};
    int reference_cnt = 11;

    struct PTE PTE10;
    struct PTE PTE11;
    struct PTE PTE12;
    struct PTE PTE13;
    struct PTE PTE14;
    struct PTE PTE15;
    struct PTE PTE16;
    struct PTE PTE17;
    struct PTE PTE20;
    struct PTE PTE21;
    struct PTE PTE22;
    struct PTE PTE23;
    struct PTE PTE24;
    struct PTE PTE25;
    struct PTE PTE26;
    struct PTE PTE27;
    struct PTE page_table[TABLEMAX];

    ///*
    setupPTE(&PTE10,0,-1,-1,-1,-1);
    setupPTE(&PTE11,0,-1,-1,-1,-1);
    setupPTE(&PTE12,1,10,3,3,1);
    setupPTE(&PTE13,0,-1,-1,-1,-1);
    setupPTE(&PTE14,0,-1,-1,-1,-1);
    setupPTE(&PTE15,1,20,2,4,2);
    setupPTE(&PTE16,0,-1,-1,-1,-1);
    setupPTE(&PTE17,1,30,1,1,1);
    //*/

    ///*
    setupPTE(&PTE20,0,-1,-1,-1,-1);
    setupPTE(&PTE21,0,-1,-1,-1,-1);
    setupPTE(&PTE22,0,-1,-1,-1,-1);
    setupPTE(&PTE23,0,-1,-1,-1,-1);
    setupPTE(&PTE24,0,-1,-1,-1,-1);
    setupPTE(&PTE25,0,-1,-1,-1,-1);
    setupPTE(&PTE26,0,-1,-1,-1,-1);
    setupPTE(&PTE27,0,-1,-1,-1,-1);
    //*/

    page_table[0] = PTE10;
    page_table[1] = PTE11;
    page_table[2] = PTE12;
    page_table[3] = PTE13;
    page_table[4] = PTE14;
    page_table[5] = PTE15;
    page_table[6] = PTE16;
    page_table[7] = PTE17;
    
    /*
    page_table[0] = PTE20;
    page_table[1] = PTE21;
    page_table[2] = PTE22;
    page_table[3] = PTE23;
    page_table[4] = PTE24;
    page_table[5] = PTE25;
    page_table[6] = PTE26;
    page_table[7] = PTE27;
    */
    ///*
    int return_value = process_page_access_lfu(page_table,&table_cnt,page_number,frame_pool,&frame_cnt,current_timestamp);
    printTable(page_table, table_cnt);
    printf("Table count: %d\n", table_cnt);
    printf("frame_cnt:   %d\n", frame_cnt);
    printf("%d\n",return_value);
    //*/
    /* 
    printTable(page_table, table_cnt);
    int return_value = count_page_faults_lfu(page_table, table_cnt, reference_string, reference_cnt, frame_pool, frame_cnt);
    printTable(page_table, table_cnt);
    printf("%d\n", return_value);
    */
    return 0;
}

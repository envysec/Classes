#include <stdio.h>
#include "oslabs.h"

/*
 * if page in memory (PTE.valid = 1)
 *   modify last_access_timestamp and reference_count
 *   return frame number
 * else
 *   if free frames (frame_pool != empty)
 *     frame is removed from process frame pool
 *     frame number inserted into PTE
 *     Update any other PTE fields
 *     return frame number associated with PTE
 *   else
 *     Select from all pages that are currently in memory (valid = 1) with smallest arrival_timestamp
 *     Mark PTE as invalid and update arrival_time, last_access, and ref_count to 0
 *     Sets the frame_number of PTE of new page to new free frame
 *     Sets arrival_time, last_access, and ref_count accordingly
 *     return frame number
 */
int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        (page_table[page_number].reference_count)++;
        return page_table[page_number].frame_number;
    }
    else
    {
        if (*frame_cnt != 0)
	{
            (*frame_cnt)--;
	    page_table[page_number].is_valid = 1;
	    page_table[page_number].frame_number = frame_pool[*frame_cnt];
	    page_table[page_number].arrival_timestamp = current_timestamp;
	    page_table[page_number].last_access_timestamp = current_timestamp;
	    page_table[page_number].reference_count = 1;
	    return page_table[page_number].frame_number;
	}
	else
	{
	    int i, drop_index = -1; 
	    int min_time = 1000000;

            for (i = 0; i < *table_cnt; i++)
	    {
                if (page_table[i].is_valid && (page_table[i].arrival_timestamp < min_time))
		{
		    drop_index = i;
		    min_time = page_table[i].arrival_timestamp;
		}
	    }
           
	    page_table[page_number].is_valid = 1;
	    page_table[page_number].frame_number = page_table[drop_index].frame_number;
	    page_table[page_number].arrival_timestamp = current_timestamp;
	    page_table[page_number].last_access_timestamp = current_timestamp;
	    page_table[page_number].reference_count = 1;

	    page_table[drop_index].is_valid = 0;
	    page_table[drop_index].frame_number = -1;
            page_table[drop_index].arrival_timestamp = -1;
	    page_table[drop_index].last_access_timestamp = -1;
	    page_table[drop_index].reference_count = -1;

	    return page_table[page_number].frame_number;
	}
    }
}

/*
 *  start with int timestamp = 1
 *  If referenced page in memory (valid bit == 1)
 *    modifies last_access_timestamp && reference_count
 *    increment timestamp
 *  else
 *    If frame_pool not empty
 *      remove frame from frame_pool
 *      frame number inserted into PTE
 *
 */
int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int i, current_timestamp = 1;
    int num_faults = 0;

    for (i = 0; i < reference_cnt; i++)
    {
	int page_reference = reference_string[i];
        current_timestamp++;

	if (page_table[page_reference].is_valid)
	{
	    page_table[page_reference].last_access_timestamp = current_timestamp;
	    (page_table[page_reference].reference_count)++;
	    current_timestamp++;
	}
	else
	{
	    if (frame_cnt != 0)
	    {
		frame_cnt--;
		page_table[page_reference].is_valid = 1;
		page_table[page_reference].frame_number = frame_pool[frame_cnt];
		page_table[page_reference].arrival_timestamp = current_timestamp;
		page_table[page_reference].last_access_timestamp = current_timestamp;
		page_table[page_reference].reference_count = 1;
		num_faults++;
	    }
	    else
	    {
		int j, drop_index = -1; 
		int min_time = 1000000;

		for (j = 0; j < table_cnt; j++)
		{
                    
		    if (page_table[j].is_valid && (page_table[j].arrival_timestamp < min_time))
		    {
			drop_index = j;
			min_time = page_table[j].arrival_timestamp;
		    }
		}
                
		page_table[page_reference].is_valid = 1;
		page_table[page_reference].frame_number = page_table[drop_index].frame_number;
		page_table[page_reference].arrival_timestamp = current_timestamp;
		page_table[page_reference].last_access_timestamp = current_timestamp;
		page_table[page_reference].reference_count = 1;

		page_table[drop_index].is_valid = 0;
		page_table[drop_index].frame_number = -1;
		page_table[drop_index].arrival_timestamp = -1;
		page_table[drop_index].last_access_timestamp = -1;
		page_table[drop_index].reference_count = -1;
                
		num_faults++;
	    }
	}   
    }
    return num_faults;
}

/*
 *  If in memory (valid == 1)
 *    update last_access_timestamp and reference_count
 *    return frame number
 *  else
 *    If frame pool != empty
 *      remove frame from frame pool (take frame and decrement count)
 *      update PTE fields
 *      return frame number
 *    else
 *      select from pages in memory (valid == 1) that has smallest last_access_timestamp
 *      Mark page invalid and update PTE fields
 *      Insert frame number into new PTE and update all fields
 *      return frame number
 */
int process_page_access_lru(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        (page_table[page_number].reference_count)++;
        return page_table[page_number].frame_number;
    }
    else
    {
        if (*frame_cnt != 0)
	{
            (*frame_cnt)--;
	    page_table[page_number].is_valid = 1;
	    page_table[page_number].frame_number = frame_pool[*frame_cnt];
	    page_table[page_number].arrival_timestamp = current_timestamp;
	    page_table[page_number].last_access_timestamp = current_timestamp;
	    page_table[page_number].reference_count = 1;
	    return page_table[page_number].frame_number;
	}
	else
	{
	    int i, drop_index = -1; 
	    int min_time = 1000000;

            for (i = 0; i < *table_cnt; i++)
	    {
                if (page_table[i].is_valid && (page_table[i].last_access_timestamp < min_time))
		{
		    drop_index = i;
		    min_time = page_table[i].last_access_timestamp;
		}
	    }
           
	    page_table[page_number].is_valid = 1;
	    page_table[page_number].frame_number = page_table[drop_index].frame_number;
	    page_table[page_number].arrival_timestamp = current_timestamp;
	    page_table[page_number].last_access_timestamp = current_timestamp;
	    page_table[page_number].reference_count = 1;

	    page_table[drop_index].is_valid = 0;
	    page_table[drop_index].frame_number = -1;
            page_table[drop_index].arrival_timestamp = -1;
	    page_table[drop_index].last_access_timestamp = -1;
	    page_table[drop_index].reference_count = -1;

	    return page_table[page_number].frame_number;
	}
    }
}

/*
 *
 */
int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int i, current_timestamp = 1;
    int num_faults = 0;

    for (i = 0; i < reference_cnt; i++)
    {
	int page_reference = reference_string[i];
        current_timestamp++;

	if (page_table[page_reference].is_valid)
	{
	    page_table[page_reference].last_access_timestamp = current_timestamp;
	    (page_table[page_reference].reference_count)++;
	    current_timestamp++;
	}
	else
	{
	    if (frame_cnt != 0)
	    {
		frame_cnt--;
		page_table[page_reference].is_valid = 1;
		page_table[page_reference].frame_number = frame_pool[frame_cnt];
		page_table[page_reference].arrival_timestamp = current_timestamp;
		page_table[page_reference].last_access_timestamp = current_timestamp;
		page_table[page_reference].reference_count = 1;
		num_faults++;
	    }
	    else
	    {
		int j, drop_index = -1; 
		int min_time = 1000000;

		for (j = 0; j < table_cnt; j++)
		{
                    
		    if (page_table[j].is_valid && (page_table[j].last_access_timestamp < min_time))
		    {
			drop_index = j;
			min_time = page_table[j].last_access_timestamp;
		    }
		}
                
		page_table[page_reference].is_valid = 1;
		page_table[page_reference].frame_number = page_table[drop_index].frame_number;
		page_table[page_reference].arrival_timestamp = current_timestamp;
		page_table[page_reference].last_access_timestamp = current_timestamp;
		page_table[page_reference].reference_count = 1;

		page_table[drop_index].is_valid = 0;
		page_table[drop_index].frame_number = -1;
		page_table[drop_index].arrival_timestamp = -1;
		page_table[drop_index].last_access_timestamp = -1;
		page_table[drop_index].reference_count = -1;
                
		num_faults++;
	    }
	}   
    }
    return num_faults;
}

/*
 *
 */
int process_page_access_lfu(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        (page_table[page_number].reference_count)++;
        return page_table[page_number].frame_number;
    }
    else
    {
        if (*frame_cnt != 0)
	{
            (*frame_cnt)--;
	    page_table[page_number].is_valid = 1;
	    page_table[page_number].frame_number = frame_pool[*frame_cnt];
	    page_table[page_number].arrival_timestamp = current_timestamp;
	    page_table[page_number].last_access_timestamp = current_timestamp;
	    page_table[page_number].reference_count = 1;
	    return page_table[page_number].frame_number;
	}
	else
	{
	    int i, drop_index = -1; 
	    int min_reference = 1000000;
	    int min_time = 1000000;

		for (i = 0; i < *table_cnt; i++)
		{
                    
		    if (page_table[i].is_valid && (page_table[i].reference_count < min_reference))
		    {
			drop_index = i;
			min_reference = page_table[i].reference_count;
			min_time = page_table[i].arrival_timestamp;
		    } 
		    else if (page_table[i].is_valid && (page_table[i].reference_count == min_reference))
		    {
                        if (page_table[i].arrival_timestamp < min_time)
			{
		          drop_index = i;
			  min_reference = page_table[i].reference_count;
			  min_time = page_table[i].arrival_timestamp;
			}
		    }
		    else
			continue;
		}
           
	    page_table[page_number].is_valid = 1;
	    page_table[page_number].frame_number = page_table[drop_index].frame_number;
	    page_table[page_number].arrival_timestamp = current_timestamp;
	    page_table[page_number].last_access_timestamp = current_timestamp;
	    page_table[page_number].reference_count = 1;

	    page_table[drop_index].is_valid = 0;
	    page_table[drop_index].frame_number = -1;
            page_table[drop_index].arrival_timestamp = -1;
	    page_table[drop_index].last_access_timestamp = -1;
	    page_table[drop_index].reference_count = -1;

	    return page_table[page_number].frame_number;
	}
    }
}

/*
 *
 */
int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int i, current_timestamp = 1;
    int num_faults = 0;

    for (i = 0; i < reference_cnt; i++)
    {
	int page_reference = reference_string[i];
        current_timestamp++;

	if (page_table[page_reference].is_valid)
	{
	    page_table[page_reference].last_access_timestamp = current_timestamp;
	    (page_table[page_reference].reference_count)++;
	    current_timestamp++;
	}
	else
	{
	    if (frame_cnt != 0)
	    {
		frame_cnt--;
		page_table[page_reference].is_valid = 1;
		page_table[page_reference].frame_number = frame_pool[frame_cnt];
		page_table[page_reference].arrival_timestamp = current_timestamp;
		page_table[page_reference].last_access_timestamp = current_timestamp;
		page_table[page_reference].reference_count = 1;
		num_faults++;
	    }
	    else
	    {
		int j, drop_index = -1; 
		int min_reference = 1000000;
                int min_time = 1000000;

		for (j = 0; j < table_cnt; j++)
		{
                    
		    if (page_table[j].is_valid && (page_table[j].reference_count < min_reference))
		    {
			drop_index = j;
			min_reference = page_table[j].reference_count;
			min_time = page_table[j].arrival_timestamp;
		    } 
		    else if (page_table[j].is_valid && (page_table[j].reference_count == min_reference))
		    {
                        if (page_table[j].arrival_timestamp < min_time)
			{
		          drop_index = j;
			  min_reference = page_table[j].reference_count;
			  min_time = page_table[j].arrival_timestamp;
			}
		    }
		    else
			continue;
		}
                
		page_table[page_reference].is_valid = 1;
		page_table[page_reference].frame_number = page_table[drop_index].frame_number;
		page_table[page_reference].arrival_timestamp = current_timestamp;
		page_table[page_reference].last_access_timestamp = current_timestamp;
		page_table[page_reference].reference_count = 1;

		page_table[drop_index].is_valid = 0;
		page_table[drop_index].frame_number = -1;
		page_table[drop_index].arrival_timestamp = -1;
		page_table[drop_index].last_access_timestamp = -1;
		page_table[drop_index].reference_count = -1;
                
		num_faults++;
	    }
	}   
    }
    return num_faults;
}

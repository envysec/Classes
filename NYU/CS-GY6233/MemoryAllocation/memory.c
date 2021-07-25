#include "oslabs.h"

int find_target_index(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int map_cnt)
{
    int i;
    
    for (i = 0; i < map_cnt; i++)
    {
	if (memory_map[i].process_id == freed_block.process_id)
	    return i;
    }

    return -1;
}
int find_candidate_next(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int map_cnt, int last_address)
{
    int i;
    int last_address_index = -1;

    for (i = 0; i < map_cnt; i++)
    {
        if (memory_map[i].start_address == last_address)
	    last_address_index = i;
    }

    if (last_address_index == -1)
	return -1;

    for (i = last_address_index + 1; i < map_cnt; i++)
    {
        if ((memory_map[i].process_id) == 0 && (memory_map[i].segment_size >= request_size))
	    return i;
    }

    return -1;
}

int find_candidate_worst(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int map_cnt)
{
    //If map is empty
    if (map_cnt == 0)
        return -1;

    //If has one block
    if (map_cnt == 1)
    {
	//If block is free
        if (memory_map[0].process_id == 0)
	{
            return 0;
	}
	//If block is not free
	else
	{
            return -1;
	}
    }
    else
    {
        int i;
	int candidate_index = -1;
	int dist = -1;
        
	//Iterate through memory block to find worst fit
	for (i = 0; i < map_cnt; i++)
	{
            //If block is free and at least the size requested
            if ((memory_map[i].process_id == 0) && (memory_map[i].segment_size >= request_size))
	    {
		//Check if worst candidate
                if ((memory_map[i].segment_size - request_size) > dist)
		{
		    //Update distance and index
                    dist = memory_map[i].segment_size - request_size;
		    candidate_index = i;
		}
	    }
	}
	return candidate_index;
    }
}

/*
 *  If empty map
 *      return -1
 *  If 1 map entry
 *      if free
 *          return block index
 *      else
 *          return -1
 *  else
 *      set dist -1
 *      loop
 *          if free
 *              min(distance)
 *          else
 *              skip
 *      if dist == -1
 *          return -1
 *      else
 *          return block index
 *      
 */
int find_candidate_best(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int map_cnt)
{
    //If map is empty
    if (map_cnt == 0)
        return -1;

    //If has one block
    if (map_cnt == 1)
    {
	//If block is free
        if (memory_map[0].process_id == 0)
	{
            return 0;
	}
	//If block is not free
	else
	{
            return -1;
	}
    }
    else
    {
        int i;
	int candidate_index = -1;
	int dist = 2000000000;
        
	//Iterate through memory block to find best fit
	for (i = 0; i < map_cnt; i++)
	{
            //If block is free and at least the size requested
            if ((memory_map[i].process_id == 0) && (memory_map[i].segment_size >= request_size))
	    {
		//Check if better candidate
                if ((memory_map[i].segment_size - request_size) < dist)
		{
		    //Update distance and index
                    dist = memory_map[i].segment_size - request_size;
		    candidate_index = i;
		}
	    }
	}
	return candidate_index;
    }
}

/* 
 *  If free block found
 *      If block size == requested size
 *          return pointer to memory block
 *      If block size > requested size
 *          Split block into allocated piece + free block
 *          return pointer of allocated block
 *      else
 *          return NULLBLOCK
 *   else
 *      return NULLBLOCK
 */
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK allocated_block;
    struct MEMORY_BLOCK remaining_block;

    int best_fit_index = find_candidate_best(request_size, memory_map, *map_cnt);

    if (best_fit_index == -1)
    {
	//Establish NULLBLOCK
        allocated_block.start_address = 0;
	allocated_block.end_address = 0;
	allocated_block.segment_size = 0;
	allocated_block.process_id = 0;

	//Return NULLBLOCK
	return allocated_block;
    }
    else
    {
	if (request_size == memory_map[best_fit_index].segment_size)
	{
            memory_map[best_fit_index].process_id = process_id;
	    allocated_block = memory_map[best_fit_index];
	}
	else
	{
	    //Establish allocated block
            allocated_block.start_address = memory_map[best_fit_index].start_address;
	    allocated_block.end_address = memory_map[best_fit_index].start_address + request_size - 1;
	    allocated_block.segment_size = request_size;
	    allocated_block.process_id = process_id;

	    //Establish remaining free block
	    remaining_block.start_address = memory_map[best_fit_index].start_address + request_size;
	    remaining_block.end_address = memory_map[best_fit_index].end_address;
	    remaining_block.segment_size = memory_map[best_fit_index].segment_size - request_size;
	    remaining_block.process_id = 0;

	    int i;

	    //Shift right
	    for (i = *map_cnt; i > best_fit_index + 1; i--)
	        memory_map[i] = memory_map[i-1];
        
	    //Splits free block into allocated + remaining free block
	    memory_map[best_fit_index + 1] = remaining_block;
	    memory_map[best_fit_index] = allocated_block;
            (*map_cnt)++;
        }
	//Return allocated block
	return allocated_block;
    }
}

/*
 *  If free
 *      If free size == request size
 *          return allocated block
 *      If free size > request size
 *          split block to allocated + free remaining
 *          return allocated block
 *      else
 *          return NULLBLOCK
 */
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK allocated_block;
    struct MEMORY_BLOCK remaining_block;

    if ((*map_cnt) == 0)
    {
	//Establish NULLBLOCK
        allocated_block.start_address = 0;
	allocated_block.end_address = 0;
	allocated_block.segment_size = 0;
	allocated_block.process_id = 0;

	//Return NULLBLOCK
	return allocated_block;
    }
    
    int i;

    for (i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0)
	{
            if (memory_map[i].segment_size == request_size)
	    {
                //Establish allocated block
	        memory_map[i].process_id = process_id;

		//Return allocated block
		return memory_map[i];
	    }
	    else if (memory_map[i].segment_size > request_size)
	    {
	        //Establish allocated block
                allocated_block.start_address = memory_map[i].start_address;
	        allocated_block.end_address = memory_map[i].start_address + request_size - 1;
	        allocated_block.segment_size = request_size;
	        allocated_block.process_id = process_id;

	        //Establish remaining free block
	        remaining_block.start_address = memory_map[i].start_address + request_size;
	        remaining_block.end_address = memory_map[i].end_address;
	        remaining_block.segment_size = memory_map[i].segment_size - request_size;
	        remaining_block.process_id = 0;

	        int j;

	        //Shift right
	        for (j = *map_cnt; j > i + 1; j--)
	            memory_map[i] = memory_map[i-1];
        
	        //Splits free block into allocated + remaining free block
	        memory_map[i + 1] = remaining_block;
	        memory_map[i] = allocated_block;
                (*map_cnt)++;
            
		//Return allocated block
                return allocated_block;		
	    }
	    else
	    {
                continue;
	    }
	}
    }

    //Establish NULLBLOCK
    allocated_block.start_address = 0;
    allocated_block.end_address = 0;
    allocated_block.segment_size = 0;
    allocated_block.process_id = 0;

    //Return NULLBLOCK
    return allocated_block;
}

/*
 *
 */
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK allocated_block;
    struct MEMORY_BLOCK remaining_block;

    int worst_fit_index = find_candidate_worst(request_size, memory_map, *map_cnt);

    if (worst_fit_index == -1)
    {
	//Establish NULLBLOCK
        allocated_block.start_address = 0;
	allocated_block.end_address = 0;
	allocated_block.segment_size = 0;
	allocated_block.process_id = 0;

	//Return NULLBLOCK
	return allocated_block;
    }
    else
    {
	if (request_size == memory_map[worst_fit_index].segment_size)
	{
	    //Establish allocated block
            memory_map[worst_fit_index].process_id = process_id;
	    allocated_block = memory_map[worst_fit_index];
	}
	else
	{
	    //Establish allocated block
            allocated_block.start_address = memory_map[worst_fit_index].start_address;
	    allocated_block.end_address = memory_map[worst_fit_index].start_address + request_size - 1;
	    allocated_block.segment_size = request_size;
	    allocated_block.process_id = process_id;

	    //Establish remaining free block
	    remaining_block.start_address = memory_map[worst_fit_index].start_address + request_size;
	    remaining_block.end_address = memory_map[worst_fit_index].end_address;
	    remaining_block.segment_size = memory_map[worst_fit_index].segment_size - request_size;
	    remaining_block.process_id = 0;

	    int i;

	    //Shift right
	    for (i = *map_cnt; i > worst_fit_index + 1; i--)
	        memory_map[i] = memory_map[i-1];
        
	    //Splits free block into allocated + remaining free block
	    memory_map[worst_fit_index + 1] = remaining_block;
	    memory_map[worst_fit_index] = allocated_block;
            (*map_cnt)++;
        }
	//Return allocated block
	return allocated_block;
    }
}

/*
 *  
 */
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id, int last_address)
{
    struct MEMORY_BLOCK allocated_block;
    struct MEMORY_BLOCK remaining_block;

    int next_address_index = find_candidate_next(request_size, memory_map, *map_cnt, last_address);

    if (next_address_index == -1)
    {
	//Establish NULLBLOCK
        allocated_block.start_address = 0;
	allocated_block.end_address = 0;
	allocated_block.segment_size = 0;
	allocated_block.process_id = 0;

	//Return NULLBLOCK
	return allocated_block;
    }
    
    if (memory_map[next_address_index].segment_size == request_size)
    {
	//Establish allocated block
        memory_map[next_address_index].process_id = process_id;
	return memory_map[next_address_index];
    }
    else if (memory_map[next_address_index].segment_size > request_size)
    {
	//Establish allocated block
        allocated_block.start_address = memory_map[next_address_index].start_address;
	allocated_block.end_address = memory_map[next_address_index].start_address + request_size - 1;
	allocated_block.segment_size = request_size;
	allocated_block.process_id = process_id;
        
	//Establish remaining block
	remaining_block.start_address = memory_map[next_address_index].start_address + request_size;
	remaining_block.end_address = memory_map[next_address_index].end_address;
	remaining_block.segment_size = memory_map[next_address_index].segment_size - request_size;
	remaining_block.process_id = 0;

	int i;
        
	//Shift right
	for (i = *map_cnt; i > next_address_index; i--)
            memory_map[i] = memory_map[i-1];
    
	//Splits free block into allocated + remaining free block
	memory_map[next_address_index + 1] = remaining_block;
	memory_map[next_address_index] = allocated_block;
	(*map_cnt)++;

	//Return allocated block
	return allocated_block;
    }
    else
    {
        allocated_block.start_address = 0;
	allocated_block.end_address = 0;
	allocated_block.segment_size = 0;
	allocated_block.process_id = 0;

	return allocated_block;
    }
}

/*
 *  If free
 *      return
 *  else
 *      If right and left free
 *          Create new block
 *          start_addr = left.start_addr
 *          end_addr = right.end_addr
 *          segment_size = left.segment_size + target.segment_size + right.segment_size
 *          process_id = 0
 *          shift left
 *      If right free
 *          Create new block
 *          start_addr = target.start_addr
 *          end_addr = right.end_addr
 *          segment_size = target.segment_size + right.segment_size
 *          process_id = 0
 *          shift left
 *      If left free
 *          Create new block
 *          start_addr = left.start_addr
 *          end_addr = target.end_addr
 *          segment_size = left.segment_size + target.segment_size
 *          process_id = 0
 *          shift left
 *      else
 *          process_id = 0
 */
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt)
{
    struct MEMORY_BLOCK new_block;

    int target_index = find_target_index(freed_block, memory_map, *map_cnt);

    //Checks if block is already free
    if (freed_block.process_id == 0)
	return;

    //Checks if block cannot be found
    if (target_index == -1)
	return;

    //Checks if both adjacent blocks are free
    if (memory_map[target_index - 1].process_id == 0 && memory_map[target_index + 1].process_id == 0)
    {
	//Establish new block
        new_block.start_address = memory_map[target_index - 1].start_address;
	new_block.end_address = memory_map[target_index + 1].end_address;
	new_block.segment_size = memory_map[target_index - 1].segment_size + memory_map[target_index].segment_size + memory_map[target_index + 1].segment_size;
	new_block.process_id = 0;

	//Updates memory map with newly freed block
	memory_map[target_index - 1] = new_block;
	
	int i;
	
	//Shift left
	for (i = target_index + 2; i <= *map_cnt; i++)
            memory_map[i - 2] = memory_map[i];

	//Update map count
	(*map_cnt) = (*map_cnt) - 2;
    }
    //Checks if only the memory block below is free
    else if (memory_map[target_index - 1].process_id == 0)
    {
	//Establish new block
        new_block.start_address = memory_map[target_index - 1].start_address;
	new_block.end_address = memory_map[target_index].end_address;
	new_block.segment_size = memory_map[target_index - 1].segment_size + memory_map[target_index].segment_size;
	new_block.process_id = 0;

	//Updates memory map with newly freed block
	memory_map[target_index - 1] = new_block;

        int i;

	//Shift left
	for (i = target_index + 1; i <= *map_cnt; i++)
	    memory_map[i - 1] = memory_map[i];

	//Update map count
	(*map_cnt)--;
    }
    //Checks if only the memory above is free
    else if (memory_map[target_index + 1].process_id == 0)
    {
	//Establish new block
        new_block.start_address = memory_map[target_index].start_address;
	new_block.end_address = memory_map[target_index + 1].end_address;
	new_block.segment_size = memory_map[target_index].segment_size + memory_map[target_index + 1].segment_size;
	new_block.process_id = 0;

	//Updates memory map with newly freed block
	memory_map[target_index] = new_block;

	int i;

	//Shift left
	for (i = target_index + 2; i <= *map_cnt; i++)
            memory_map[i - 1] = memory_map[i];

	//Update map count
	(*map_cnt)--;
    }
    else
    {
	//Updates memory map with newly freed block
        memory_map[target_index].process_id = 0;
    }
}

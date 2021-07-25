#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"

int find_next_request_sstf(struct RCB request_queue[QUEUEMAX], int queue_cnt, int current_cylinder)
{
    int i, dist = 1000000;
    int next_request_index = 0;

    for (i = 1; i < queue_cnt; i++)
    {
        if (abs(request_queue[i].cylinder - current_cylinder) < dist)
        {
            dist = abs(request_queue[i].cylinder - current_cylinder);
	    next_request_index = i;
	}
	else if (abs(request_queue[i].cylinder - current_cylinder) == dist)
	{
	    if(request_queue[i].arrival_timestamp < request_queue[next_request_index].arrival_timestamp)
		next_request_index = i;
	}
	else
	    continue;
    }

    return next_request_index;
}

int find_next_request_fcfs(struct RCB request_queue[QUEUEMAX], int queue_cnt)
{
    int i, next_request_index = 0;
    int min = request_queue[next_request_index].arrival_timestamp;
    
    for (i = 1; i < queue_cnt; i++)
    {
        if (request_queue[i].arrival_timestamp < min)
	{
	    min = request_queue[i].arrival_timestamp;
	    next_request_index = i;
	}
    }
    return next_request_index;
}

int isNULLRCB(struct RCB current_request)
{
    if (current_request.request_id == 0 && current_request.arrival_timestamp == 0 && current_request.cylinder == 0 && current_request.address == 0 && current_request.process_id == 0)
	return 1;
    return 0;
}

/*
 *  If disk free
 *      return new RCB
 *  else
 *      add new RCB to queue
 *      increment queue_cnt
 *      return current RCB
 */
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (isNULLRCB(current_request))
	return new_request;
    else
    {
	request_queue[*queue_cnt] = new_request;
	(*queue_cnt)++;
	return current_request;
    }
}

/*
 *  If queue empty
 *      return NULLRCB
 *  else
 *      find earliest arrival time
 *      remove request from queue
 *      update queue
 *      update queue_cnt
 *      return RCB
 */
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt)
{
    struct RCB next_request;

    if (*queue_cnt == 0)
    {
	next_request.request_id = 0;
	next_request.arrival_timestamp = 0;
	next_request.cylinder = 0;
	next_request.address = 0;
	next_request.process_id = 0;
	return next_request;
    }
    else
    {
        int next_request_index = find_next_request_fcfs(request_queue, *queue_cnt);
        int i;

        next_request.request_id = request_queue[next_request_index].request_id;
	next_request.arrival_timestamp = request_queue[next_request_index].arrival_timestamp;
	next_request.cylinder = request_queue[next_request_index].cylinder;
	next_request.address = request_queue[next_request_index].address;
	next_request.process_id = request_queue[next_request_index].process_id;

	for (i = next_request_index; i < *queue_cnt; i++)
            request_queue[i] = request_queue[i + 1];

	(*queue_cnt)--;

	return next_request;
    }

}

/*
 *  if free
 *    return new request
 *  else
 *    add RCB to queue
 *    update queue count
 *    return current
 */
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (isNULLRCB(current_request))
        return new_request;
    else
    {
	request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
	return current_request;
    }
}

/*
 *  If queue empty
 *    return NULLRCB
 *  else
 *    find closest cylinder in queue
 *    if equal distance pick eariler arrival
 *    remove request from queue
 *    update queue
 *    update count
 *    return request
 */
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder)
{
    struct RCB next_request;

    if (*queue_cnt == 0)
    {
	next_request.request_id = 0;
	next_request.arrival_timestamp = 0;
	next_request.cylinder = 0;
	next_request.address = 0;
	next_request.process_id = 0;
	return next_request;
    }
    else
    {
        int next_request_index = find_next_request_sstf(request_queue, *queue_cnt, current_cylinder);
        int i;

	next_request.request_id = request_queue[next_request_index].request_id;
	next_request.arrival_timestamp = request_queue[next_request_index].arrival_timestamp;
	next_request.cylinder = request_queue[next_request_index].cylinder;
	next_request.address = request_queue[next_request_index].address;
	next_request.process_id = request_queue[next_request_index].process_id;

	for (i = next_request_index; i < *queue_cnt; i++)
	    request_queue[i] = request_queue[i + 1];

	(*queue_cnt)--;
        
	return next_request;
    }
}

/*
 *  If free
 *    return new request
 *  else
 *    add new request to queue
 *    update count
 *    return current request
 */
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (isNULLRCB(current_request))
	return new_request;
    else
    {
	request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
	return current_request;
    }
}

/*
 *  If empty queue
 *    return NULLPCB
 *  else
 *    If queue cylinder == current cylinder
 *      If multiple
 *        earlier arrival
 *    else if direction == 1 
 *      find closest cylinder > current cylinder
 *      if none
 *        find closest < current cylinder
 *    else
 *      find closest cylinder < current cylinder
 *      if none
 *        find closest > current cylinder
 */
int find_next_request_look(struct RCB request_queue[QUEUEMAX], int queue_cnt, int current_cylinder, int scan_direction)
{
    int i, next_request_index = -1;

    for (i = 0; i < queue_cnt; i++)
    {
	if (request_queue[i].cylinder == current_cylinder)
	{
	    if (next_request_index > -1 && (request_queue[i].arrival_timestamp >= request_queue[next_request_index].arrival_timestamp))
		continue;
	    else
		next_request_index = i;
        }
    }

    if (next_request_index > -1)
        return next_request_index;

    int nDist = 1000000;
    int sDist = -1000000;

    for (i = 0; i < queue_cnt; i++)
    {
        if (scan_direction == 1)
	{
	    if (request_queue[i].cylinder - current_cylinder < nDist && request_queue[i].cylinder - current_cylinder > 0)
                next_request_index = i;
	}
	else if (scan_direction == 0)
	{
            if (request_queue[i].cylinder - current_cylinder > sDist && request_queue[i].cylinder - current_cylinder < 0)
		next_request_index = i;
	}
	else
	    return -1;
    }

    if (next_request_index == -1)
        next_request_index = find_next_request_sstf(request_queue, queue_cnt, current_cylinder);

    return next_request_index;
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction)
{
    struct RCB next_request;

    if (*queue_cnt == 0)
    {
	next_request.request_id = 0;
	next_request.arrival_timestamp = 0;
	next_request.cylinder = 0;
	next_request.address = 0;
        next_request.process_id = 0;

	return next_request;
    }
    else
    {
	int next_request_index = find_next_request_look(request_queue, *queue_cnt, current_cylinder, scan_direction);
	int i;

	next_request.request_id = request_queue[next_request_index].request_id;
	next_request.arrival_timestamp = request_queue[next_request_index].arrival_timestamp;
	next_request.cylinder = request_queue[next_request_index].cylinder;
	next_request.address = request_queue[next_request_index].address;
	next_request.process_id = request_queue[next_request_index].process_id;

	for (i = next_request_index; i < *queue_cnt; i++)
            request_queue[i] = request_queue[i + 1];

	(*queue_cnt)--;

	return next_request;
    }
}


#include "stdio.h"
#include "nlist.h"
#include "log.h"


void init_list(nlist *list)
{
	list->tail = NULL;
	list->tailpred = (nnode *)list;
	list->head = (nnode *)&list->tail;
}

void AddTail(nlist *list, nnode *node)
{
	nnode *last = list->tailpred;
	list->tailpred = node;

	if ( node->next || node->prev )
		ypa_log_out("_AddTail(): Node added twice!\n");

	node->prev = last;
	node->next = (nnode *)&list->tail;

	last->next = node;
}

void AddHead(nlist *list, nnode *node)
{
	nnode *first = list->head;

	list->head = node;
	if ( node->next || node->prev )
		ypa_log_out("_AddHead(): Node added twice!\n");

	node->prev = (nnode *)list;
	node->next = first;

	first->prev = node;
}

nnode * RemHead(nlist *list)
{
	nnode *head_elm = list->head;
	nnode *next_elm = list->head->next;
	if ( next_elm )
	{
		list->head = next_elm;
		next_elm->prev = (nnode *)list;

		head_elm->next = NULL;
		head_elm->prev = NULL;
		return head_elm;
	}
	return NULL;
}

void Remove(nnode *node)
{
	if ( !node->next || !node->prev )
		ypa_log_out("_Remove(): Node not in list!\n");

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->next = 0;
	node->prev = 0;
}

int listCnt(nlist *list)
{
	nnode *v1 = list->head;
	int num = 0;

	while (v1->next)
	{
		v1 = v1->next;
		num++;
	}
	return num;
}

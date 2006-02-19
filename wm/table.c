#include <stdlib.h>
#include "xcb_wm.h"

typedef struct node node;
struct node {
	node *next;
	CARD32 key;
	void *value;
};

struct Table {
	node *head;
};

Table *AllocTable()
{
	return calloc(1, sizeof(Table));
}

void FreeTable(Table *table)
{
	free(table);
}

int TablePut(Table *table, CARD32 key, void *value)
{
	node *record = malloc(sizeof(node));
	if(!record)
		return 0;
	record->next = table->head;
	record->key = key;
	record->value = value;
	table->head = record;
	return 1;
}

void *TableGet(Table *table, CARD32 key)
{
	node *cur;
	for(cur = table->head; cur; cur = cur->next)
		if(cur->key == key)
			return cur->value;
	return 0;
}

void *TableRemove(Table *table, CARD32 key)
{
	node **cur;
	for(cur = &table->head; *cur; cur = &(*cur)->next)
		if((*cur)->key == key)
		{
			node *tmp = *cur;
			void *ret = tmp->value;
			*cur = (*cur)->next;
			free(tmp);
			return ret;
		}
	return 0;
}

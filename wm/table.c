#include <stdlib.h>
#include "xcb_wm.h"

typedef struct node node;
struct node {
	node *next;
	uint32_t key;
	void *value;
};

struct table_t {
	node *head;
};

table_t *alloc_table()
{
	return calloc(1, sizeof(table_t));
}

void free_table(table_t *table)
{
	free(table);
}

int table_put(table_t *table, uint32_t key, void *value)
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

void *table_get(table_t *table, uint32_t key)
{
	node *cur;
	for(cur = table->head; cur; cur = cur->next)
		if(cur->key == key)
			return cur->value;
	return 0;
}

void *table_remove(table_t *table, uint32_t key)
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

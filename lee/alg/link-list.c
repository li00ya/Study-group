#include "util/string.h"
#include "util/memory.h"
#include "api/mode.h"

#include <stdlib.h>

typedef struct list_node
{
	int32_t data;
	struct list_node *next;
}list_node_t;

static struct list_node *create_node(int32_t data)
{
	struct list_node *new = malloc(sizeof(struct list_node));

	if (NULL == new) {
		return NULL;
	}
	new->data = data;
	new->next = NULL;

	return new;
}

static struct list_node *add_node_tail(struct list_node *header, struct list_node *new)
{
	struct list_node *p = header;

	while (p->next != NULL) {
		p = p->next;
	}
	p->next = new;

	return header;
}

static struct list_node *add_node(struct list_node *header, struct list_node *new)
{
	new->next = header;
	header = new;

	return header;
}

static struct list_node *reserve(struct list_node *h)
{
	struct list_node *next, *tmp;

	next = h->next;
	h->next = NULL;

	while (next != NULL) {
		tmp = next;
		next = next->next;
		tmp->next = h;
		h = tmp;
	}

	return h;
}

static struct list_node *reserve_data(struct list_node *h, int32_t *cnt)
{
	struct list_node *next, *cur;
	int32_t idx;

	cur = h;

	if (cur->next != NULL) {
		reserve_data(cur->next, cnt);
		*cnt += 1;
	} else {
		*cnt = 0;
	}

	for (idx = *cnt; idx > 0; idx--) {
		next = cur->next;
		cur->data = cur->data ^ next->data;
		next->data = cur->data ^ next->data;
		cur->data = cur->data ^ next->data;
		cur = next;
	}

	return h;
}

static void show_link(int8_t *title,  struct list_node *header)
{
	struct list_node *p = header;

	while (p) {
		util_puts("%s addr %p, link data %d\n", title, p, p->data);
		p = p->next;
	}
}

static struct list_node *FindFirstCommonNode(struct list_node *pHead1, struct list_node *pHead2)
{
	struct list_node* ps = pHead1;
	struct list_node* pl = pHead2;
	int32_t length1 = 1;
	int32_t length2 = 1;

	while ((ps = ps->next)) length1++;
	while ((pl = pl->next)) length2++;

	if (length1 < length2) {
		ps = pHead1;
		pl = pHead2;
		length1 = length2 - length1;
	} else {
		ps = pHead2;
		pl = pHead1;
		length1 = length1 - length2;
	}

	while (length1-- > 0) pl = pl->next;

	while (ps && pl) {
		if (ps == pl) {
			break;
		}

		ps = ps->next;
		pl = pl->next;
	}

	return ps;
}

int32_t _proc(int32_t argc, int8_t *argv[])
{
	struct list_node *header = create_node(0);
	struct list_node *h1, *tmp;
	int32_t num;

	header = add_node(header, create_node(2));
	h1 = header;
	header = add_node(header, create_node(3));
	header = add_node_tail(header, create_node(1));
	util_puts("第一个链表：\n");
	show_link("header", header);

	h1 = add_node(h1, create_node(5));
	h1 = add_node(h1, create_node(6));
	h1 = add_node(h1, create_node(7));
	util_puts("第二个链表：\n");
	show_link("h1", h1);

	util_puts("获取两个链表的第一个公共节点：\n");
	tmp = FindFirstCommonNode(h1, header);
	show_link("tmp", tmp);

	util_puts("********* reserve link **********\n");
	header = reserve_data(header, &num);
	util_puts("链表不动，数据反转：\n");
	show_link("header", header);
	header = reserve(header);
	util_puts("链表反转\n");
	show_link("header", header);

	return 0;
}

static int32_t _usage(void)
{
	util_puts("link option:\n\n");
	util_puts("\t-t [type]	link | array\n");

	return 0;
}

static lee_module_t _module = {
	.name = "link",
	.usage = _usage,
	.proc = _proc,
};

static void PREP_INIT _constructor(void)
{
	lee_module_register(LEE_TYPE_ALG, LEE_ALG_LINK, &_module);
}

static void PREP_EXIT _destructor(void)
{
	lee_module_unregister(LEE_TYPE_ALG, LEE_ALG_LINK);
}

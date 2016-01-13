#ifndef NLIST_H_INCLUDED
#define NLIST_H_INCLUDED


struct nnode
{
    nnode *next;
    nnode *prev;
};

struct nlist
{
    nnode *head;
    nnode *tail;
    nnode *tailpred;
};

void init_list(nlist *list);

void AddTail(nlist *list, nnode *node);
void AddHead(nlist *list, nnode *node);
nnode * RemHead(nlist *list);
void Remove(nnode *node);
int listCnt(nlist *list);

#endif // NLIST_H_INCLUDED

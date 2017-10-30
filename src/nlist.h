#ifndef NLIST_H_INCLUDED
#define NLIST_H_INCLUDED


struct nnode
{
    nnode *next;
    nnode *prev;

    nnode()
    {
        next = NULL;
        prev = NULL;
    }
};

struct nlist
{
    nnode *head;
    nnode *tail;
    nnode *tailpred;

    nlist()
    {
        tail = NULL;
        tailpred = (nnode *)this;
        head = (nnode *)&this->tail;
    }
};

void init_list(nlist *list);

void AddTail(nlist *list, nnode *node);
void AddHead(nlist *list, nnode *node);
nnode * RemHead(nlist *list);
void Remove(nnode *node);
int listCnt(nlist *list);

#endif // NLIST_H_INCLUDED

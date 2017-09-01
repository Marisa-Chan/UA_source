#ifndef LISTNODE_H_INCLUDED
#define LISTNODE_H_INCLUDED

#include <list>

template <typename T> struct ListNode
{
    typename std::list<T> *pList;
    typename std::list<T>::iterator it;
    bool assigned;

    ListNode()
    {
        _Init();
    };

    void _Init()
    {
        assigned = false;
    };

    void RemoveFromAnyList()
    {
        if (assigned && pList)
            pList->erase(it);

        assigned = false;
    };

    bool PushBack(std::list<T> *l, T val)
    {
        if (!l)
            return false;

        RemoveFromAnyList();

        it = l->insert(l->end(), val);
        pList = l;
        assigned = true;

        return true;
    };

    bool PushFront(std::list<T> *l, T val)
    {
        if (!l)
            return false;

        RemoveFromAnyList();

        it = l->insert(l->begin(), val);
        pList = l;
        assigned = true;

        return true;
    };

};



#endif // LISTNODE_H_INCLUDED

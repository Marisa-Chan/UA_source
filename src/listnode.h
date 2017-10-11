#ifndef LISTNODE_H_INCLUDED
#define LISTNODE_H_INCLUDED

#include <list>

template <typename T> class RefList: public std::list<T>
{
public:
    typedef typename std::list<T> _T_interList;
    typedef typename std::list<T>::iterator _T_interListIter;

    struct ListNode
    {
        _T_interList *pList;
        _T_interListIter it;
        bool assigned;

        ListNode()
        {
            assigned = false;
            pList = NULL;
        };

        ListNode(_T_interList &_lst, _T_interListIter _it)
        {
            pList = &_lst;
            it = _it;
            assigned = true;
        };

        void RemoveFromAnyList()
        {
            if (assigned && pList)
                pList->erase(it);

            assigned = false;
            pList = NULL;
        };
    };

    ListNode refPushBack(const T &val)
    {
        return ListNode(this, insert(this->end(), val));
    }

    ListNode refPushFront(const T &val)
    {
        return ListNode(this, insert(this->begin(), val));
    }
};

#endif // LISTNODE_H_INCLUDED

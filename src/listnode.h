#ifndef LISTNODE_H_INCLUDED
#define LISTNODE_H_INCLUDED

#include <list>

template <typename T> class RefList: protected std::list<T>
{
public:
    class Node;
    
    typedef RefList<T> _T_List;
    typedef typename std::list<T>::iterator _T_interListIter;
    typedef Node& (* _T_RefNodeCallBack)(T&);
    
    class Node
    {
    friend class RefList;
    public:
        Node() : _pList(NULL) {};
        Node(_T_List *lst, _T_interListIter it)
        {
            _pList = lst;
            _it = it;
        };
        
        void Detach()
        {
            if (_pList)
            {
                _pList->erase(_it);
                _pList = NULL;
            }
        }
        
        operator bool() const
        {
            return (_pList != NULL);
        }
        
        T *PList()
        {
            return _pList;
        }
        
        void *PObj()
        {
            if (_pList)
                return _pList->_o;
            return NULL;
        }
        
    protected:
        _T_List *_pList;
        _T_interListIter _it;
    };
    
public:
    using std::list<T>::begin;
    using std::list<T>::end;
    using std::list<T>::insert;
    using std::list<T>::size;
    
    RefList(void *O, int LType = 0) : _o(O), _refNodeCallBack(NULL), ListType(LType) {};
    RefList(void *O, _T_RefNodeCallBack RefNodeCallBack, int LType = 0) : _o(O), _refNodeCallBack(RefNodeCallBack), ListType(LType) {};
    
    Node push_back(T c)
    {
        return Node(this, insert(this->end(), c));
    }
    
    Node push_front(T c)
    {
        return Node(this, insert(this->front(), c));
    }
    
    void unsafe_clear()
    {
        std::list<T>::clear();
    }
    
    void clear()
    {
        if (_refNodeCallBack)
        {
            while(this->begin() != this->end())
            {
                _refNodeCallBack(this->front())._pList = NULL;
                this->erase(this->begin());
            }
        }
        else
            std::list<T>::clear();
    }
    
public:
    void * const _o; // Pointer to object that contain this list
    const int ListType; // Identify list by this
    
protected:
    _T_RefNodeCallBack _refNodeCallBack;
};

#endif // LISTNODE_H_INCLUDED

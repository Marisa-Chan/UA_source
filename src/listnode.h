#ifndef LISTNODE_H_INCLUDED
#define LISTNODE_H_INCLUDED

#include <list>
#include <vector>

template <typename T> class RefList: protected std::list<T>
{
public:
    class Node;
    typedef std::vector<T> SafeCopy;
    
    typedef std::list<T> _T_Base;
    typedef RefList<T> _T_List;
    typedef typename _T_Base::iterator _T_interListIter;
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
        
        ~Node()
        {
            Detach();
        }
        
        Node(Node && b)
        {
            _pList = b._pList;
            _it = b._it;
            b._pList = NULL;
        }
        
        Node& operator=(Node && b)
        {
            _pList = b._pList;
            _it = b._it;
            b._pList = NULL;
            return *this;
        }
        
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
        
        operator _T_interListIter() const
        {
            return _it;
        }
        
        _T_List *PList() const
        {
            return _pList;
        }
        
        void *PObj() const
        {
            if (_pList)
                return _pList->_o;
            return NULL;
        }
        
        _T_interListIter iter() const
        {
            return _it;
        }
        
        inline bool IsListType(int type) const
        {
            if (!_pList)
                return false;
            return _pList->ListType == type;
        }
        
    protected:
        _T_List *_pList;
        _T_interListIter _it;
    };
    
public:
    using _T_Base::begin;
    using _T_Base::end;
    using _T_Base::rbegin;
    using _T_Base::rend;
    using _T_Base::size;
    using _T_Base::front;
    using _T_Base::back;
    using _T_Base::erase;
    using typename _T_Base::iterator;
    using typename _T_Base::reverse_iterator;
    using _T_Base::empty;
    
    
    RefList(void *O, int LType = 0) : _o(O), ListType(LType), _refNodeCallBack(NULL) {};
    RefList(void *O, _T_RefNodeCallBack RefNodeCallBack, int LType = 0) : _o(O), ListType(LType), _refNodeCallBack(RefNodeCallBack){};
    ~RefList()
    {
        clear();
    }
    
    Node push_back(T c)
    {
        return Node(this, _T_Base::insert(end(), c));
    }
    
    Node push_front(T c)
    {
        return Node(this, _T_Base::insert(begin(), c));
    }
    
    Node insert(_T_interListIter iter, T c)
    {
        return Node(this, _T_Base::insert(iter, c));
    }
    
    void unsafe_clear()
    {
        _T_Base::clear();
    }
    
    void clear()
    {
        if (_refNodeCallBack)
        {
            while(!empty())
            {
                _refNodeCallBack(front())._pList = NULL;
                erase(begin());
            }
        }
        else
            _T_Base::clear();
    }
    
    const SafeCopy safe_iter() const
    {
        SafeCopy tmp;
        tmp.reserve(_T_Base::size());
        tmp.assign(_T_Base::begin(), _T_Base::end());
        return tmp;
    }
    
public:
    void * const _o; // Pointer to object that contain this list
    const int ListType; // Identify list by this
    
protected:
    _T_RefNodeCallBack _refNodeCallBack;
};

#endif // LISTNODE_H_INCLUDED

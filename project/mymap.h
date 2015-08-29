// -*- C++ -*-
//===----------------------------- map ------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_MAP
#define _LIBCPP_MAP

/*

    map synopsis

namespace std
{

template <class Key, class T, class Compare = less<Key>,
          class Allocator = allocator<pair<const Key, T>>>
class map
{
public:
    // types:
    typedef Key                                      key_type;
    typedef T                                        mapped_type;
    typedef pair<key_type, mapped_type>        value_type;
    typedef Compare                                  key_compare;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef implementation-defined                   iterator;
    typedef implementation-defined                   const_iterator;
    typedef std::reverse_iterator<iterator>          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

    class value_compare
        : public binary_function<value_type, value_type, bool>
    {
        friend class map;
    protected:
        key_compare comp;

        value_compare(key_compare c);
    public:
        bool operator()(const value_type& x, const value_type& y) const;
    };

    // construct/copy/destroy:
    map()
        noexcept(
            is_nothrow_default_constructible<allocator_type>::value &&
            is_nothrow_default_constructible<key_compare>::value &&
            is_nothrow_copy_constructible<key_compare>::value);
    explicit map(const key_compare& comp);
    map(const key_compare& comp, const allocator_type& a);
    template <class InputIterator>
        map(InputIterator first, InputIterator last,
            const key_compare& comp = key_compare());
    template <class InputIterator>
        map(InputIterator first, InputIterator last,
            const key_compare& comp, const allocator_type& a);
    map(const map& m);
    map(map&& m)
        noexcept(
            is_nothrow_move_constructible<allocator_type>::value &&
            is_nothrow_move_constructible<key_compare>::value);
    explicit map(const allocator_type& a);
    map(const map& m, const allocator_type& a);
    map(map&& m, const allocator_type& a);
    map(initializer_list<value_type> il, const key_compare& comp = key_compare());
    map(initializer_list<value_type> il, const key_compare& comp, const allocator_type& a);
    template <class InputIterator>
        map(InputIterator first, InputIterator last, const allocator_type& a)
            : map(first, last, Compare(), a) {}  // C++14
    map(initializer_list<value_type> il, const allocator_type& a)
        : map(il, Compare(), a) {}  // C++14
   ~map();

    map& operator=(const map& m);
    map& operator=(map&& m)
        noexcept(
            allocator_type::propagate_on_container_move_assignment::value &&
            is_nothrow_move_assignable<allocator_type>::value &&
            is_nothrow_move_assignable<key_compare>::value);
    map& operator=(initializer_list<value_type> il);

    // iterators:
          iterator begin() noexcept;
    const_iterator begin() const noexcept;
          iterator end() noexcept;
    const_iterator end()   const noexcept;

          reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
          reverse_iterator rend() noexcept;
    const_reverse_iterator rend()   const noexcept;

    const_iterator         cbegin()  const noexcept;
    const_iterator         cend()    const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend()   const noexcept;

    // capacity:
    bool      empty()    const noexcept;
    size_type size()     const noexcept;
    size_type max_size() const noexcept;

    // element access:
    mapped_type& operator[](key_type& k);
    mapped_type& operator[](key_type&& k);

          mapped_type& at(key_type& k);
    const mapped_type& at(key_type& k) const;

    // modifiers:
    template <class... Args>
        pair<iterator, bool> emplace(Args&&... args);
    template <class... Args>
        iterator emplace_hint(const_iterator position, Args&&... args);
    pair<iterator, bool> insert(const value_type& v);
    template <class P>
        pair<iterator, bool> insert(P&& p);
    iterator insert(const_iterator position, const value_type& v);
    template <class P>
        iterator insert(const_iterator position, P&& p);
    template <class InputIterator>
        void insert(InputIterator first, InputIterator last);
    void insert(initializer_list<value_type> il);

    iterator  erase(const_iterator position);
    size_type erase(key_type& k);
    iterator  erase(const_iterator first, const_iterator last);
    void clear() noexcept;

    void swap(map& m)
        noexcept(
            __is_nothrow_swappable<key_compare>::value &&
            (!allocator_type::propagate_on_container_swap::value ||
             __is_nothrow_swappable<allocator_type>::value));

    // observers:
    allocator_type get_allocator() const noexcept;
    key_compare    key_comp()      const;
    value_compare  value_comp()    const;

    // map operations:
          iterator find(key_type& k);
    const_iterator find(key_type& k) const;
    template<typename K>
        iterator find(const K& x);              // C++14
    template<typename K>
        const_iterator find(const K& x) const;  // C++14
    template<typename K>
      size_type count(const K& x) const;

    size_type      count(key_type& k) const;
          iterator lower_bound(key_type& k);
    const_iterator lower_bound(key_type& k) const;
    template<typename K>
        iterator lower_bound(const K& x);              // C++14
    template<typename K>
        const_iterator lower_bound(const K& x) const;  // C++14

          iterator upper_bound(key_type& k);
    const_iterator upper_bound(key_type& k) const;
    template<typename K>
        iterator upper_bound(const K& x);              // C++14
    template<typename K>
        const_iterator upper_bound(const K& x) const;  // C++14

    pair<iterator,iterator>             equal_range(key_type& k);
    pair<const_iterator,const_iterator> equal_range(key_type& k) const;
    template<typename K>
        pair<iterator,iterator>             equal_range(const K& x);        // C++14
    template<typename K>
        pair<const_iterator,const_iterator> equal_range(const K& x) const;  // C++14
};

template <class Key, class T, class Compare, class Allocator>
bool
operator==(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator< (const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator!=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator> (const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator>=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator<=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void
swap(map<Key, T, Compare, Allocator>& x, map<Key, T, Compare, Allocator>& y)
    noexcept(noexcept(x.swap(y)));

template <class Key, class T, class Compare = less<Key>,
          class Allocator = allocator<pair<const Key, T>>>
class multimap
{
public:
    // types:
    typedef Key                                      key_type;
    typedef T                                        mapped_type;
    typedef pair<key_type,mapped_type>         value_type;
    typedef Compare                                  key_compare;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;

    typedef implementation-defined                   iterator;
    typedef implementation-defined                   const_iterator;
    typedef std::reverse_iterator<iterator>          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

    class value_compare
        : public binary_function<value_type,value_type,bool>
    {
        friend class multimap;
    protected:
        key_compare comp;
        value_compare(key_compare c);
    public:
        bool operator()(const value_type& x, const value_type& y) const;
    };

    // construct/copy/destroy:
    multimap()
        noexcept(
            is_nothrow_default_constructible<allocator_type>::value &&
            is_nothrow_default_constructible<key_compare>::value &&
            is_nothrow_copy_constructible<key_compare>::value);
    explicit multimap(const key_compare& comp);
    multimap(const key_compare& comp, const allocator_type& a);
    template <class InputIterator>
        multimap(InputIterator first, InputIterator last, const key_compare& comp);
    template <class InputIterator>
        multimap(InputIterator first, InputIterator last, const key_compare& comp,
                 const allocator_type& a);
    multimap(const multimap& m);
    multimap(multimap&& m)
        noexcept(
            is_nothrow_move_constructible<allocator_type>::value &&
            is_nothrow_move_constructible<key_compare>::value);
    explicit multimap(const allocator_type& a);
    multimap(const multimap& m, const allocator_type& a);
    multimap(multimap&& m, const allocator_type& a);
    multimap(initializer_list<value_type> il, const key_compare& comp = key_compare());
    multimap(initializer_list<value_type> il, const key_compare& comp,
             const allocator_type& a);
    template <class InputIterator>
        multimap(InputIterator first, InputIterator last, const allocator_type& a)
            : multimap(first, last, Compare(), a) {} // C++14
    multimap(initializer_list<value_type> il, const allocator_type& a)
        : multimap(il, Compare(), a) {} // C++14
    ~multimap();

    multimap& operator=(const multimap& m);
    multimap& operator=(multimap&& m)
        noexcept(
            allocator_type::propagate_on_container_move_assignment::value &&
            is_nothrow_move_assignable<allocator_type>::value &&
            is_nothrow_move_assignable<key_compare>::value);
    multimap& operator=(initializer_list<value_type> il);

    // iterators:
          iterator begin() noexcept;
    const_iterator begin() const noexcept;
          iterator end() noexcept;
    const_iterator end()   const noexcept;

          reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
          reverse_iterator rend() noexcept;
    const_reverse_iterator rend()   const noexcept;

    const_iterator         cbegin()  const noexcept;
    const_iterator         cend()    const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend()   const noexcept;

    // capacity:
    bool      empty()    const noexcept;
    size_type size()     const noexcept;
    size_type max_size() const noexcept;

    // modifiers:
    template <class... Args>
        iterator emplace(Args&&... args);
    template <class... Args>
        iterator emplace_hint(const_iterator position, Args&&... args);
    iterator insert(const value_type& v);
    template <class P>
        iterator insert(P&& p);
    iterator insert(const_iterator position, const value_type& v);
    template <class P>
        iterator insert(const_iterator position, P&& p);
    template <class InputIterator>
        void insert(InputIterator first, InputIterator last);
    void insert(initializer_list<value_type> il);

    iterator  erase(const_iterator position);
    size_type erase(key_type& k);
    iterator  erase(const_iterator first, const_iterator last);
    void clear() noexcept;

    void swap(multimap& m)
        noexcept(
            __is_nothrow_swappable<key_compare>::value &&
            (!allocator_type::propagate_on_container_swap::value ||
             __is_nothrow_swappable<allocator_type>::value));

    // observers:
    allocator_type get_allocator() const noexcept;
    key_compare    key_comp()      const;
    value_compare  value_comp()    const;

    // map operations:
          iterator find(key_type& k);
    const_iterator find(key_type& k) const;
    template<typename K>
        iterator find(const K& x);              // C++14
    template<typename K>
        const_iterator find(const K& x) const;  // C++14
    template<typename K>
      size_type count(const K& x) const;

    size_type      count(key_type& k) const;
          iterator lower_bound(key_type& k);
    const_iterator lower_bound(key_type& k) const;
    template<typename K>
        iterator lower_bound(const K& x);              // C++14
    template<typename K>
        const_iterator lower_bound(const K& x) const;  // C++14

          iterator upper_bound(key_type& k);
    const_iterator upper_bound(key_type& k) const;
    template<typename K>
        iterator upper_bound(const K& x);              // C++14
    template<typename K>
        const_iterator upper_bound(const K& x) const;  // C++14

    pair<iterator,iterator>             equal_range(key_type& k);
    pair<const_iterator,const_iterator> equal_range(key_type& k) const;
    template<typename K>
        pair<iterator,iterator>             equal_range(const K& x);        // C++14
    template<typename K>
        pair<const_iterator,const_iterator> equal_range(const K& x) const;  // C++14
};

template <class Key, class T, class Compare, class Allocator>
bool
operator==(const multimap<Key, T, Compare, Allocator>& x,
           const multimap<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator< (const multimap<Key, T, Compare, Allocator>& x,
           const multimap<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator!=(const multimap<Key, T, Compare, Allocator>& x,
           const multimap<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator> (const multimap<Key, T, Compare, Allocator>& x,
           const multimap<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator>=(const multimap<Key, T, Compare, Allocator>& x,
           const multimap<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator<=(const multimap<Key, T, Compare, Allocator>& x,
           const multimap<Key, T, Compare, Allocator>& y);

// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void
swap(multimap<Key, T, Compare, Allocator>& x,
     multimap<Key, T, Compare, Allocator>& y)
    noexcept(noexcept(x.swap(y)));

}  // std

*/

#include <__config>
#include <__tree>
#include <iterator>
#include <memory>
#include <utility>
#include <functional>
#include <initializer_list>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Key, class _CP, class _Compare, bool = is_empty<_Compare>::value
#if __has_feature(is_final)
                                                        && !__is_final(_Compare)
#endif
         >
class __map_value_compare
    : private _Compare
{
public:
    _LIBCPP_INLINE_VISIBILITY
    __map_value_compare()
        _NOEXCEPT_(is_nothrow_default_constructible<_Compare>::value)
        : _Compare() {}
    _LIBCPP_INLINE_VISIBILITY
    __map_value_compare(_Compare c)
        _NOEXCEPT_(is_nothrow_copy_constructible<_Compare>::value)
        : _Compare(c) {}
    _LIBCPP_INLINE_VISIBILITY
    const _Compare& key_comp() const _NOEXCEPT {return *this;}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _CP& __x, const _CP& __y) const
        {return static_cast<const _Compare&>(*this)(__x.__cc.first, __y.__cc.first);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _CP& __x, const _Key& __y) const
        {return static_cast<const _Compare&>(*this)(__x.__cc.first, __y);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _Key& __x, const _CP& __y) const
        {return static_cast<const _Compare&>(*this)(__x, __y.__cc.first);}

#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value, bool>::type
    operator () ( const _K2& __x, const _CP& __y ) const
        {return static_cast<const _Compare&>(*this) (__x, __y.__cc.first);}

    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value, bool>::type
    operator () (const _CP& __x, const _K2& __y) const
        {return static_cast<const _Compare&>(*this) (__x.__cc.first, __y);}
#endif
};

template <class _Key, class _CP, class _Compare>
class __map_value_compare<_Key, _CP, _Compare, false>
{
    _Compare comp;

public:
    _LIBCPP_INLINE_VISIBILITY
    __map_value_compare()
        _NOEXCEPT_(is_nothrow_default_constructible<_Compare>::value)
        : comp() {}
    _LIBCPP_INLINE_VISIBILITY
    __map_value_compare(_Compare c)
        _NOEXCEPT_(is_nothrow_copy_constructible<_Compare>::value)
        : comp(c) {}
    _LIBCPP_INLINE_VISIBILITY
    const _Compare& key_comp() const _NOEXCEPT {return comp;}

    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _CP& __x, const _CP& __y) const
        {return comp(__x.__cc.first, __y.__cc.first);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _CP& __x, const _Key& __y) const
        {return comp(__x.__cc.first, __y);}
    _LIBCPP_INLINE_VISIBILITY
    bool operator()(const _Key& __x, const _CP& __y) const
        {return comp(__x, __y.__cc.first);}

#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value, bool>::type
    operator () ( const _K2& __x, const _CP& __y ) const
        {return comp (__x, __y.__cc.first);}

    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value, bool>::type
    operator () (const _CP& __x, const _K2& __y) const
        {return comp (__x.__cc.first, __y);}
#endif
};

template <class _Allocator>
class __map_node_destructor
{
    typedef _Allocator                          allocator_type;
    typedef allocator_traits<allocator_type>    __alloc_traits;
    typedef typename __alloc_traits::value_type::value_type value_type;
public:
    typedef typename __alloc_traits::pointer    pointer;
private:
    typedef typename value_type::value_type::first_type     first_type;
    typedef typename value_type::value_type::second_type    second_type;

    allocator_type& __na_;

    __map_node_destructor& operator=(const __map_node_destructor&);

public:
    bool __first_constructed;
    bool __second_constructed;

    _LIBCPP_INLINE_VISIBILITY
    explicit __map_node_destructor(allocator_type& __na) _NOEXCEPT
        : __na_(__na),
          __first_constructed(false),
          __second_constructed(false)
        {}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    __map_node_destructor(__tree_node_destructor<allocator_type>&& __x) _NOEXCEPT
        : __na_(__x.__na_),
          __first_constructed(__x.__value_constructed),
          __second_constructed(__x.__value_constructed)
        {
            __x.__value_constructed = false;
        }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY
    void operator()(pointer __p) _NOEXCEPT
    {
        if (__second_constructed)
            __alloc_traits::destroy(__na_, _VSTD::addressof(__p->__value_.__cc.second));
        if (__first_constructed)
            __alloc_traits::destroy(__na_, _VSTD::addressof(__p->__value_.__cc.first));
        if (__p)
            __alloc_traits::deallocate(__na_, __p, 1);
    }
};

template <class _Key, class _Tp, class _Compare, class _Allocator>
    class map;
template <class _Key, class _Tp, class _Compare, class _Allocator>
    class multimap;
template <class _TreeIterator> class __map_const_iterator;

#if __cplusplus >= 201103L

template <class _Key, class _Tp>
union __value_type
{
    typedef _Key                                     key_type;
    typedef _Tp                                      mapped_type;
    typedef pair<key_type, mapped_type>        value_type;
    typedef pair<key_type, mapped_type>              __nc_value_type;

    value_type __cc;
    __nc_value_type __nc;

    template <class ..._Args>
    _LIBCPP_INLINE_VISIBILITY
    __value_type(_Args&& ...__args)
        : __cc(std::forward<_Args>(__args)...) {}

    _LIBCPP_INLINE_VISIBILITY
    __value_type(const __value_type& __v)
        : __cc(__v.__cc) {}

    _LIBCPP_INLINE_VISIBILITY
    __value_type(__value_type& __v)
        : __cc(__v.__cc) {}

    _LIBCPP_INLINE_VISIBILITY
    __value_type(__value_type&& __v)
        : __nc(std::move(__v.__nc)) {}

    _LIBCPP_INLINE_VISIBILITY
    __value_type& operator=(const __value_type& __v)
        {__nc = __v.__cc; return *this;}

    _LIBCPP_INLINE_VISIBILITY
    __value_type& operator=(__value_type&& __v)
        {__nc = std::move(__v.__nc); return *this;}

    _LIBCPP_INLINE_VISIBILITY
    ~__value_type() {__cc.~value_type();}
};

#else

template <class _Key, class _Tp>
struct __value_type
{
    typedef _Key                                     key_type;
    typedef _Tp                                      mapped_type;
    typedef pair<key_type, mapped_type>        value_type;

    value_type __cc;

    _LIBCPP_INLINE_VISIBILITY
    __value_type() {}

    template <class _A0>
    _LIBCPP_INLINE_VISIBILITY
    __value_type(const _A0& __a0)
        : __cc(__a0) {}

    template <class _A0, class _A1>
    _LIBCPP_INLINE_VISIBILITY
    __value_type(const _A0& __a0, const _A1& __a1)
        : __cc(__a0, __a1) {}
};

#endif

template <class _TreeIterator>
class _LIBCPP_TYPE_VIS_ONLY __map_iterator
{
    _TreeIterator __i_;

    typedef typename _TreeIterator::__pointer_traits             __pointer_traits;
    typedef const typename _TreeIterator::value_type::value_type::first_type __key_type;
    typedef typename _TreeIterator::value_type::value_type::second_type      __mapped_type;
public:
    typedef bidirectional_iterator_tag                           iterator_category;
    typedef pair<__key_type, __mapped_type>                      value_type;
    typedef typename _TreeIterator::difference_type              difference_type;
    typedef value_type&                                          reference;
    typedef typename __pointer_traits::template
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
            rebind<value_type>
#else
            rebind<value_type>::other
#endif
                                                                 pointer;

    _LIBCPP_INLINE_VISIBILITY
    __map_iterator() _NOEXCEPT {}

    _LIBCPP_INLINE_VISIBILITY
    __map_iterator(_TreeIterator __i) _NOEXCEPT : __i_(__i) {}

    _LIBCPP_INLINE_VISIBILITY
    reference operator*() const {return __i_->__cc;}
    _LIBCPP_INLINE_VISIBILITY
    pointer operator->() const {return pointer_traits<pointer>::pointer_to(__i_->__cc);}

    _LIBCPP_INLINE_VISIBILITY
    __map_iterator& operator++() {++__i_; return *this;}
    _LIBCPP_INLINE_VISIBILITY
    __map_iterator operator++(int)
    {
        __map_iterator __t(*this);
        ++(*this);
        return __t;
    }

    _LIBCPP_INLINE_VISIBILITY
    __map_iterator& operator--() {--__i_; return *this;}
    _LIBCPP_INLINE_VISIBILITY
    __map_iterator operator--(int)
    {
        __map_iterator __t(*this);
        --(*this);
        return __t;
    }

    friend _LIBCPP_INLINE_VISIBILITY
    bool operator==(const __map_iterator& __x, const __map_iterator& __y)
        {return __x.__i_ == __y.__i_;}
    friend 
    _LIBCPP_INLINE_VISIBILITY
    bool operator!=(const __map_iterator& __x, const __map_iterator& __y)
        {return __x.__i_ != __y.__i_;}

    template <class, class, class, class> friend class _LIBCPP_TYPE_VIS_ONLY map;
    template <class, class, class, class> friend class _LIBCPP_TYPE_VIS_ONLY multimap;
    template <class> friend class _LIBCPP_TYPE_VIS_ONLY __map_const_iterator;
};

template <class _TreeIterator>
class _LIBCPP_TYPE_VIS_ONLY __map_const_iterator
{
    _TreeIterator __i_;

    typedef typename _TreeIterator::__pointer_traits             __pointer_traits;
    typedef const typename _TreeIterator::value_type::value_type::first_type __key_type;
    typedef typename _TreeIterator::value_type::value_type::second_type      __mapped_type;
public:
    typedef bidirectional_iterator_tag                           iterator_category;
    typedef pair<__key_type, __mapped_type>                      value_type;
    typedef typename _TreeIterator::difference_type              difference_type;
    typedef const value_type&                                    reference;
    typedef typename __pointer_traits::template
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
            rebind<const value_type>
#else
            rebind<const value_type>::other
#endif
                                                                 pointer;

    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator() _NOEXCEPT {}

    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator(_TreeIterator __i) _NOEXCEPT : __i_(__i) {}
    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator(
            __map_iterator<typename _TreeIterator::__non_const_iterator> __i)
                _NOEXCEPT
                : __i_(__i.__i_) {}

    _LIBCPP_INLINE_VISIBILITY
    reference operator*() const {return __i_->__cc;}
    _LIBCPP_INLINE_VISIBILITY
    pointer operator->() const {return pointer_traits<pointer>::pointer_to(__i_->__cc);}

    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator& operator++() {++__i_; return *this;}
    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator operator++(int)
    {
        __map_const_iterator __t(*this);
        ++(*this);
        return __t;
    }

    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator& operator--() {--__i_; return *this;}
    _LIBCPP_INLINE_VISIBILITY
    __map_const_iterator operator--(int)
    {
        __map_const_iterator __t(*this);
        --(*this);
        return __t;
    }

    friend _LIBCPP_INLINE_VISIBILITY
    bool operator==(const __map_const_iterator& __x, const __map_const_iterator& __y)
        {return __x.__i_ == __y.__i_;}
    friend _LIBCPP_INLINE_VISIBILITY
    bool operator!=(const __map_const_iterator& __x, const __map_const_iterator& __y)
        {return __x.__i_ != __y.__i_;}

    template <class, class, class, class> friend class _LIBCPP_TYPE_VIS_ONLY map;
    template <class, class, class, class> friend class _LIBCPP_TYPE_VIS_ONLY multimap;
    template <class, class, class> friend class _LIBCPP_TYPE_VIS_ONLY __tree_const_iterator;
};

template <class _Key, class _Tp, class _Compare = less<_Key>,
          class _Allocator = allocator<pair<_Key, _Tp> > >
class _LIBCPP_TYPE_VIS_ONLY map
{
public:
    // types:
    typedef _Key                                     key_type;
    typedef _Tp                                      mapped_type;
    typedef pair<key_type, mapped_type>        value_type;
    typedef pair<key_type, mapped_type>              __nc_value_type;
    typedef _Compare                                 key_compare;
    typedef _Allocator                               allocator_type;
    typedef value_type&                              reference;
    typedef const value_type&                        const_reference;

    class _LIBCPP_TYPE_VIS_ONLY value_compare
        : public binary_function<value_type, value_type, bool>
    {
        friend class map;
    protected:
        key_compare comp;

        _LIBCPP_INLINE_VISIBILITY value_compare(key_compare c) : comp(c) {}
    public:
        _LIBCPP_INLINE_VISIBILITY
        bool operator()(const value_type& __x, const value_type& __y) const
            {return comp(__x.first, __y.first);}
    };

private:

    typedef _VSTD::__value_type<key_type, mapped_type>             __value_type;
    typedef __map_value_compare<key_type, __value_type, key_compare> __vc;
    typedef typename allocator_traits<allocator_type>::template
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
            rebind_alloc<__value_type>
#else
            rebind_alloc<__value_type>::other
#endif
                                                           __allocator_type;
    typedef __tree<__value_type, __vc, __allocator_type>   __base;
    typedef typename __base::__node_traits                 __node_traits;
    typedef allocator_traits<allocator_type>               __alloc_traits;

    __base __tree_;

public:
    typedef typename __alloc_traits::pointer               pointer;
    typedef typename __alloc_traits::const_pointer         const_pointer;
    typedef typename __alloc_traits::size_type             size_type;
    typedef typename __alloc_traits::difference_type       difference_type;
    typedef __map_iterator<typename __base::iterator>      iterator;
    typedef __map_const_iterator<typename __base::const_iterator> const_iterator;
    typedef _VSTD::reverse_iterator<iterator>               reverse_iterator;
    typedef _VSTD::reverse_iterator<const_iterator>         const_reverse_iterator;

    _LIBCPP_INLINE_VISIBILITY
    explicit map(const key_compare& __comp = key_compare())
        _NOEXCEPT_(
            is_nothrow_default_constructible<allocator_type>::value &&
            is_nothrow_default_constructible<key_compare>::value &&
            is_nothrow_copy_constructible<key_compare>::value)
        : __tree_(__vc(__comp)) {}

    _LIBCPP_INLINE_VISIBILITY
    explicit map(const key_compare& __comp, const allocator_type& __a)
        : __tree_(__vc(__comp), __a) {}

    template <class _InputIterator>
    _LIBCPP_INLINE_VISIBILITY
        map(_InputIterator __f, _InputIterator __l,
            const key_compare& __comp = key_compare())
        : __tree_(__vc(__comp))
        {
            insert(__f, __l);
        }

    template <class _InputIterator>
    _LIBCPP_INLINE_VISIBILITY
        map(_InputIterator __f, _InputIterator __l,
            const key_compare& __comp, const allocator_type& __a)
        : __tree_(__vc(__comp), __a)
        {
            insert(__f, __l);
        }

#if _LIBCPP_STD_VER > 11
    template <class _InputIterator>
    _LIBCPP_INLINE_VISIBILITY 
    map(_InputIterator __f, _InputIterator __l, const allocator_type& __a)
        : map(__f, __l, key_compare(), __a) {}
#endif

    _LIBCPP_INLINE_VISIBILITY
    map(const map& __m)
        : __tree_(__m.__tree_)
        {
            insert(__m.begin(), __m.end());
        }

    _LIBCPP_INLINE_VISIBILITY
    map& operator=(const map& __m)
        {
#if __cplusplus >= 201103L
            __tree_ = __m.__tree_;
#else
            __tree_.clear();
            __tree_.value_comp() = __m.__tree_.value_comp();
            __tree_.__copy_assign_alloc(__m.__tree_);
            insert(__m.begin(), __m.end());
#endif
            return *this;
        }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY
    map(map&& __m)
        _NOEXCEPT_(is_nothrow_move_constructible<__base>::value)
        : __tree_(_VSTD::move(__m.__tree_))
        {
        }

    map(map&& __m, const allocator_type& __a);

    _LIBCPP_INLINE_VISIBILITY
    map& operator=(map&& __m)
        _NOEXCEPT_(is_nothrow_move_assignable<__base>::value)
        {
            __tree_ = _VSTD::move(__m.__tree_);
            return *this;
        }

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    map(initializer_list<value_type> __il, const key_compare& __comp = key_compare())
        : __tree_(__vc(__comp))
        {
            insert(__il.begin(), __il.end());
        }

    _LIBCPP_INLINE_VISIBILITY
    map(initializer_list<value_type> __il, const key_compare& __comp, const allocator_type& __a)
        : __tree_(__vc(__comp), __a)
        {
            insert(__il.begin(), __il.end());
        }

#if _LIBCPP_STD_VER > 11
    _LIBCPP_INLINE_VISIBILITY 
    map(initializer_list<value_type> __il, const allocator_type& __a)
        : map(__il, key_compare(), __a) {}
#endif

    _LIBCPP_INLINE_VISIBILITY
    map& operator=(initializer_list<value_type> __il)
        {
            __tree_.__assign_unique(__il.begin(), __il.end());
            return *this;
        }

#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    explicit map(const allocator_type& __a)
        : __tree_(__a)
        {
        }

    _LIBCPP_INLINE_VISIBILITY
    map(const map& __m, const allocator_type& __a)
        : __tree_(__m.__tree_.value_comp(), __a)
        {
            insert(__m.begin(), __m.end());
        }

    _LIBCPP_INLINE_VISIBILITY
          iterator begin() _NOEXCEPT {return __tree_.begin();}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator begin() const _NOEXCEPT {return __tree_.begin();}
    _LIBCPP_INLINE_VISIBILITY
          iterator end() _NOEXCEPT {return __tree_.end();}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator end() const _NOEXCEPT {return __tree_.end();}

    _LIBCPP_INLINE_VISIBILITY
          reverse_iterator rbegin() _NOEXCEPT {return reverse_iterator(end());}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator rbegin() const _NOEXCEPT
        {return const_reverse_iterator(end());}
    _LIBCPP_INLINE_VISIBILITY
          reverse_iterator rend() _NOEXCEPT
            {return       reverse_iterator(begin());}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator rend() const _NOEXCEPT
        {return const_reverse_iterator(begin());}

    _LIBCPP_INLINE_VISIBILITY
    const_iterator cbegin() const _NOEXCEPT {return begin();}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator cend() const _NOEXCEPT {return end();}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator crbegin() const _NOEXCEPT {return rbegin();}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator crend() const _NOEXCEPT {return rend();}

    _LIBCPP_INLINE_VISIBILITY
    bool      empty() const _NOEXCEPT {return __tree_.size() == 0;}
    _LIBCPP_INLINE_VISIBILITY
    size_type size() const _NOEXCEPT {return __tree_.size();}
    _LIBCPP_INLINE_VISIBILITY
    size_type max_size() const _NOEXCEPT {return __tree_.max_size();}

    mapped_type& operator[](key_type& __k);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    mapped_type& operator[](key_type&& __k);
#endif

          mapped_type& at(key_type& __k);
    const mapped_type& at(key_type& __k) const;

    _LIBCPP_INLINE_VISIBILITY
    allocator_type get_allocator() const _NOEXCEPT {return __tree_.__alloc();}
    _LIBCPP_INLINE_VISIBILITY
    key_compare    key_comp()      const {return __tree_.value_comp().key_comp();}
    _LIBCPP_INLINE_VISIBILITY
    value_compare  value_comp()    const {return value_compare(__tree_.value_comp().key_comp());}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class ..._Args>
        pair<iterator, bool>
        emplace(_Args&& ...__args);

    template <class ..._Args>
        iterator
        emplace_hint(const_iterator __p, _Args&& ...__args);

#endif  // _LIBCPP_HAS_NO_VARIADICS

    template <class _Pp,
              class = typename enable_if<is_constructible<value_type, _Pp>::value>::type>
        _LIBCPP_INLINE_VISIBILITY
        pair<iterator, bool> insert(_Pp&& __p)
            {return __tree_.__insert_unique(_VSTD::forward<_Pp>(__p));}

    template <class _Pp,
              class = typename enable_if<is_constructible<value_type, _Pp>::value>::type>
        _LIBCPP_INLINE_VISIBILITY
        iterator insert(const_iterator __pos, _Pp&& __p)
            {return __tree_.__insert_unique(__pos.__i_, _VSTD::forward<_Pp>(__p));}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY
    pair<iterator, bool>
        insert(const value_type& __v) {return __tree_.__insert_unique(__v);}

    _LIBCPP_INLINE_VISIBILITY
    iterator
        insert(const_iterator __p, const value_type& __v)
            {return __tree_.__insert_unique(__p.__i_, __v);}

    template <class _InputIterator>
        _LIBCPP_INLINE_VISIBILITY
        void insert(_InputIterator __f, _InputIterator __l)
        {
            for (const_iterator __e = cend(); __f != __l; ++__f)
                insert(__e.__i_, *__f);
        }

#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    void insert(initializer_list<value_type> __il)
        {insert(__il.begin(), __il.end());}

#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    iterator erase(const_iterator __p) {return __tree_.erase(__p.__i_);}
    _LIBCPP_INLINE_VISIBILITY
    size_type erase(key_type& __k)
        {return __tree_.__erase_unique(__k);}
    _LIBCPP_INLINE_VISIBILITY
    iterator  erase(const_iterator __f, const_iterator __l)
        {return __tree_.erase(__f.__i_, __l.__i_);}
    _LIBCPP_INLINE_VISIBILITY
    void clear() _NOEXCEPT {__tree_.clear();}

    _LIBCPP_INLINE_VISIBILITY
    void swap(map& __m)
        _NOEXCEPT_(__is_nothrow_swappable<__base>::value)
        {__tree_.swap(__m.__tree_);}

    _LIBCPP_INLINE_VISIBILITY
    iterator find(key_type& __k)             {return __tree_.find(__k);}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator find(key_type& __k) const {return __tree_.find(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,iterator>::type
    find(const _K2& __k)                           {return __tree_.find(__k);}
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,const_iterator>::type
    find(const _K2& __k) const                     {return __tree_.find(__k);}
#endif

    _LIBCPP_INLINE_VISIBILITY
    size_type      count(key_type& __k) const
        {return __tree_.__count_unique(__k);}
    _LIBCPP_INLINE_VISIBILITY
    iterator lower_bound(key_type& __k)
        {return __tree_.lower_bound(__k);}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator lower_bound(key_type& __k) const
        {return __tree_.lower_bound(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,iterator>::type
    lower_bound(const _K2& __k)       {return __tree_.lower_bound(__k);}

    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,const_iterator>::type
    lower_bound(const _K2& __k) const {return __tree_.lower_bound(__k);}
#endif

    _LIBCPP_INLINE_VISIBILITY
    iterator upper_bound(key_type& __k)
        {return __tree_.upper_bound(__k);}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator upper_bound(key_type& __k) const
        {return __tree_.upper_bound(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,iterator>::type
    upper_bound(const _K2& __k)       {return __tree_.upper_bound(__k);}
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,const_iterator>::type
    upper_bound(const _K2& __k) const {return __tree_.upper_bound(__k);}
#endif

    _LIBCPP_INLINE_VISIBILITY
    pair<iterator,iterator> equal_range(key_type& __k)
        {return __tree_.__equal_range_unique(__k);}
    _LIBCPP_INLINE_VISIBILITY
    pair<const_iterator,const_iterator> equal_range(key_type& __k) const
        {return __tree_.__equal_range_unique(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,pair<iterator,iterator>>::type
    equal_range(const _K2& __k)       {return __tree_.__equal_range_unique(__k);}
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,pair<const_iterator,const_iterator>>::type
    equal_range(const _K2& __k) const {return __tree_.__equal_range_unique(__k);}
#endif

private:
    typedef typename __base::__node                    __node;
    typedef typename __base::__node_allocator          __node_allocator;
    typedef typename __base::__node_pointer            __node_pointer;
    typedef typename __base::__node_const_pointer      __node_const_pointer;
    typedef typename __base::__node_base_pointer       __node_base_pointer;
    typedef typename __base::__node_base_const_pointer __node_base_const_pointer;
    typedef __map_node_destructor<__node_allocator> _Dp;
    typedef unique_ptr<__node, _Dp> __node_holder;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    __node_holder __construct_node();
    template <class _A0>
        __node_holder __construct_node(_A0&& __a0);
    __node_holder __construct_node_with_key(key_type&& __k);
#ifndef _LIBCPP_HAS_NO_VARIADICS
    template <class _A0, class _A1, class ..._Args>
        __node_holder __construct_node(_A0&& __a0, _A1&& __a1, _Args&& ...__args);
#endif  // _LIBCPP_HAS_NO_VARIADICS
#endif
    __node_holder __construct_node_with_key(key_type& __k);

    __node_base_pointer&
        __find_equal_key(__node_base_pointer& __parent, key_type& __k);
    __node_base_const_pointer
        __find_equal_key(__node_base_const_pointer& __parent, key_type& __k) const;
};

// Find place to insert if __k doesn't exist
// Set __parent to parent of null leaf
// Return reference to null leaf
// If __k exists, set parent to node of __k and return reference to node of __k
template <class _Key, class _Tp, class _Compare, class _Allocator>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_base_pointer&
map<_Key, _Tp, _Compare, _Allocator>::__find_equal_key(__node_base_pointer& __parent,
                                                       key_type& __k)
{
    __node_pointer __nd = __tree_.__root();
    if (__nd != nullptr)
    {
        while (true)
        {
            if (__tree_.value_comp().key_comp()(__k, __nd->__value_.__cc.first))
            {
                if (__nd->__left_ != nullptr)
                    __nd = static_cast<__node_pointer>(__nd->__left_);
                else
                {
                    __parent = static_cast<__node_base_pointer>(__nd);
                    return __parent->__left_;
                }
            }
            else if (__tree_.value_comp().key_comp()(__nd->__value_.__cc.first, __k))
            {
                if (__nd->__right_ != nullptr)
                    __nd = static_cast<__node_pointer>(__nd->__right_);
                else
                {
                    __parent = static_cast<__node_base_pointer>(__nd);
                    return __parent->__right_;
                }
            }
            else
            {
                __parent = static_cast<__node_base_pointer>(__nd);
                return __parent;
            }
        }
    }
    __parent = static_cast<__node_base_pointer>(__tree_.__end_node());
    return __parent->__left_;
}

// Find __k
// Set __parent to parent of null leaf and
//    return reference to null leaf iv __k does not exist.
// If __k exists, set parent to node of __k and return reference to node of __k
template <class _Key, class _Tp, class _Compare, class _Allocator>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_base_const_pointer
map<_Key, _Tp, _Compare, _Allocator>::__find_equal_key(__node_base_const_pointer& __parent,
                                                       key_type& __k) const
{
    __node_const_pointer __nd = __tree_.__root();
    if (__nd != nullptr)
    {
        while (true)
        {
            if (__tree_.value_comp().key_comp()(__k, __nd->__value_.__cc.first))
            {
                if (__nd->__left_ != nullptr)
                    __nd = static_cast<__node_pointer>(__nd->__left_);
                else
                {
                    __parent = static_cast<__node_base_pointer>(__nd);
                    return const_cast<const __node_base_const_pointer&>(__parent->__left_);
                }
            }
            else if (__tree_.value_comp().key_comp()(__nd->__value_.__cc.first, __k))
            {
                if (__nd->__right_ != nullptr)
                    __nd = static_cast<__node_pointer>(__nd->__right_);
                else
                {
                    __parent = static_cast<__node_base_pointer>(__nd);
                    return const_cast<const __node_base_const_pointer&>(__parent->__right_);
                }
            }
            else
            {
                __parent = static_cast<__node_base_pointer>(__nd);
                return __parent;
            }
        }
    }
    __parent = static_cast<__node_base_pointer>(__tree_.__end_node());
    return const_cast<const __node_base_const_pointer&>(__parent->__left_);
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Key, class _Tp, class _Compare, class _Allocator>
map<_Key, _Tp, _Compare, _Allocator>::map(map&& __m, const allocator_type& __a)
    : __tree_(_VSTD::move(__m.__tree_), __a)
{
    if (__a != __m.get_allocator())
    {
        const_iterator __e = cend();
        while (!__m.empty())
            __tree_.__insert_unique(__e.__i_,
                    _VSTD::move(__m.__tree_.remove(__m.begin().__i_)->__value_));
    }
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_holder
map<_Key, _Tp, _Compare, _Allocator>::__construct_node()
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.first));
    __h.get_deleter().__first_constructed = true;
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.second));
    __h.get_deleter().__second_constructed = true;
    return __h;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class _A0>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_holder
map<_Key, _Tp, _Compare, _Allocator>::__construct_node(_A0&& __a0)
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_), _VSTD::forward<_A0>(__a0));
    __h.get_deleter().__first_constructed = true;
    __h.get_deleter().__second_constructed = true;
    return __h;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_holder
map<_Key, _Tp, _Compare, _Allocator>::__construct_node_with_key(key_type&& __k)
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.first), _VSTD::move(__k));
    __h.get_deleter().__first_constructed = true;
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.second));
    __h.get_deleter().__second_constructed = true;
    return __h;
}

#ifndef _LIBCPP_HAS_NO_VARIADICS

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class _A0, class _A1, class ..._Args>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_holder
map<_Key, _Tp, _Compare, _Allocator>::__construct_node(_A0&& __a0, _A1&& __a1, _Args&& ...__args)
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_),
                             _VSTD::forward<_A0>(__a0), _VSTD::forward<_A1>(__a1),
                             _VSTD::forward<_Args>(__args)...);
    __h.get_deleter().__first_constructed = true;
    __h.get_deleter().__second_constructed = true;
    return __h;
}

#endif  // _LIBCPP_HAS_NO_VARIADICS

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Key, class _Tp, class _Compare, class _Allocator>
typename map<_Key, _Tp, _Compare, _Allocator>::__node_holder
map<_Key, _Tp, _Compare, _Allocator>::__construct_node_with_key(key_type& __k)
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.first), __k);
    __h.get_deleter().__first_constructed = true;
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.second));
    __h.get_deleter().__second_constructed = true;
    return _VSTD::move(__h);  // explicitly moved for C++03
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
_Tp&
map<_Key, _Tp, _Compare, _Allocator>::operator[](key_type& __k)
{
    __node_base_pointer __parent;
    __node_base_pointer& __child = __find_equal_key(__parent, __k);
    __node_pointer __r = static_cast<__node_pointer>(__child);
    if (__child == nullptr)
    {
        __node_holder __h = __construct_node_with_key(__k);
        __tree_.__insert_node_at(__parent, __child, static_cast<__node_base_pointer>(__h.get()));
        __r = __h.release();
    }
    return __r->__value_.__cc.second;
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Key, class _Tp, class _Compare, class _Allocator>
_Tp&
map<_Key, _Tp, _Compare, _Allocator>::operator[](key_type&& __k)
{
    __node_base_pointer __parent;
    __node_base_pointer& __child = __find_equal_key(__parent, __k);
    __node_pointer __r = static_cast<__node_pointer>(__child);
    if (__child == nullptr)
    {
        __node_holder __h = __construct_node_with_key(_VSTD::move(__k));
        __tree_.__insert_node_at(__parent, __child, static_cast<__node_base_pointer>(__h.get()));
        __r = __h.release();
    }
    return __r->__value_.__cc.second;
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Key, class _Tp, class _Compare, class _Allocator>
_Tp&
map<_Key, _Tp, _Compare, _Allocator>::at(key_type& __k)
{
    __node_base_pointer __parent;
    __node_base_pointer& __child = __find_equal_key(__parent, __k);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__child == nullptr)
        throw out_of_range("map::at:  key not found");
#endif  // _LIBCPP_NO_EXCEPTIONS
    return static_cast<__node_pointer>(__child)->__value_.__cc.second;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
const _Tp&
map<_Key, _Tp, _Compare, _Allocator>::at(key_type& __k) const
{
    __node_base_const_pointer __parent;
    __node_base_const_pointer __child = __find_equal_key(__parent, __k);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__child == nullptr)
        throw out_of_range("map::at:  key not found");
#endif  // _LIBCPP_NO_EXCEPTIONS
    return static_cast<__node_const_pointer>(__child)->__value_.__cc.second;
}

#if !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class ..._Args>
pair<typename map<_Key, _Tp, _Compare, _Allocator>::iterator, bool>
map<_Key, _Tp, _Compare, _Allocator>::emplace(_Args&& ...__args)
{
    __node_holder __h = __construct_node(_VSTD::forward<_Args>(__args)...);
    pair<iterator, bool> __r = __tree_.__node_insert_unique(__h.get());
    if (__r.second)
        __h.release();
    return __r;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class ..._Args>
typename map<_Key, _Tp, _Compare, _Allocator>::iterator
map<_Key, _Tp, _Compare, _Allocator>::emplace_hint(const_iterator __p,
                                                   _Args&& ...__args)
{
    __node_holder __h = __construct_node(_VSTD::forward<_Args>(__args)...);
    iterator __r = __tree_.__node_insert_unique(__p.__i_, __h.get());
    if (__r.__i_.__ptr_ == __h.get())
        __h.release();
    return __r;
}

#endif  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const map<_Key, _Tp, _Compare, _Allocator>& __x,
           const map<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return __x.size() == __y.size() && _VSTD::equal(__x.begin(), __x.end(), __y.begin());
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator< (const map<_Key, _Tp, _Compare, _Allocator>& __x,
           const map<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return _VSTD::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const map<_Key, _Tp, _Compare, _Allocator>& __x,
           const map<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return !(__x == __y);
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator> (const map<_Key, _Tp, _Compare, _Allocator>& __x,
           const map<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return __y < __x;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const map<_Key, _Tp, _Compare, _Allocator>& __x,
           const map<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return !(__x < __y);
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const map<_Key, _Tp, _Compare, _Allocator>& __x,
           const map<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return !(__y < __x);
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(map<_Key, _Tp, _Compare, _Allocator>& __x,
     map<_Key, _Tp, _Compare, _Allocator>& __y)
    _NOEXCEPT_(_NOEXCEPT_(__x.swap(__y)))
{
    __x.swap(__y);
}

template <class _Key, class _Tp, class _Compare = less<_Key>,
          class _Allocator = allocator<pair<const _Key, _Tp> > >
class _LIBCPP_TYPE_VIS_ONLY multimap
{
public:
    // types:
    typedef _Key                                     key_type;
    typedef _Tp                                      mapped_type;
    typedef pair<key_type, mapped_type>        value_type;
    typedef pair<key_type, mapped_type>              __nc_value_type;
    typedef _Compare                                 key_compare;
    typedef _Allocator                               allocator_type;
    typedef value_type&                              reference;
    typedef const value_type&                        const_reference;

    class _LIBCPP_TYPE_VIS_ONLY value_compare
        : public binary_function<value_type, value_type, bool>
    {
        friend class multimap;
    protected:
        key_compare comp;

        _LIBCPP_INLINE_VISIBILITY
        value_compare(key_compare c) : comp(c) {}
    public:
        _LIBCPP_INLINE_VISIBILITY
        bool operator()(const value_type& __x, const value_type& __y) const
            {return comp(__x.first, __y.first);}
    };

private:

    typedef _VSTD::__value_type<key_type, mapped_type>             __value_type;
    typedef __map_value_compare<key_type, __value_type, key_compare> __vc;
    typedef typename allocator_traits<allocator_type>::template
#ifndef _LIBCPP_HAS_NO_TEMPLATE_ALIASES
            rebind_alloc<__value_type>
#else
            rebind_alloc<__value_type>::other
#endif
                                                                    __allocator_type;
    typedef __tree<__value_type, __vc, __allocator_type>            __base;
    typedef typename __base::__node_traits                          __node_traits;
    typedef allocator_traits<allocator_type>                        __alloc_traits;

    __base __tree_;

public:
    typedef typename __alloc_traits::pointer               pointer;
    typedef typename __alloc_traits::const_pointer         const_pointer;
    typedef typename __alloc_traits::size_type             size_type;
    typedef typename __alloc_traits::difference_type       difference_type;
    typedef __map_iterator<typename __base::iterator>      iterator;
    typedef __map_const_iterator<typename __base::const_iterator> const_iterator;
    typedef _VSTD::reverse_iterator<iterator>               reverse_iterator;
    typedef _VSTD::reverse_iterator<const_iterator>         const_reverse_iterator;

    _LIBCPP_INLINE_VISIBILITY
    explicit multimap(const key_compare& __comp = key_compare())
        _NOEXCEPT_(
            is_nothrow_default_constructible<allocator_type>::value &&
            is_nothrow_default_constructible<key_compare>::value &&
            is_nothrow_copy_constructible<key_compare>::value)
        : __tree_(__vc(__comp)) {}

    _LIBCPP_INLINE_VISIBILITY
    explicit multimap(const key_compare& __comp, const allocator_type& __a)
        : __tree_(__vc(__comp), __a) {}

    template <class _InputIterator>
        _LIBCPP_INLINE_VISIBILITY
        multimap(_InputIterator __f, _InputIterator __l,
            const key_compare& __comp = key_compare())
        : __tree_(__vc(__comp))
        {
            insert(__f, __l);
        }

    template <class _InputIterator>
        _LIBCPP_INLINE_VISIBILITY
        multimap(_InputIterator __f, _InputIterator __l,
            const key_compare& __comp, const allocator_type& __a)
        : __tree_(__vc(__comp), __a)
        {
            insert(__f, __l);
        }

#if _LIBCPP_STD_VER > 11
    template <class _InputIterator>
    _LIBCPP_INLINE_VISIBILITY 
    multimap(_InputIterator __f, _InputIterator __l, const allocator_type& __a)
        : multimap(__f, __l, key_compare(), __a) {}
#endif

    _LIBCPP_INLINE_VISIBILITY
    multimap(const multimap& __m)
        : __tree_(__m.__tree_.value_comp(),
          __alloc_traits::select_on_container_copy_construction(__m.__tree_.__alloc()))
        {
            insert(__m.begin(), __m.end());
        }

    _LIBCPP_INLINE_VISIBILITY
    multimap& operator=(const multimap& __m)
        {
#if __cplusplus >= 201103L
            __tree_ = __m.__tree_;
#else
            __tree_.clear();
            __tree_.value_comp() = __m.__tree_.value_comp();
            __tree_.__copy_assign_alloc(__m.__tree_);
            insert(__m.begin(), __m.end());
#endif
            return *this;
        }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY
    multimap(multimap&& __m)
        _NOEXCEPT_(is_nothrow_move_constructible<__base>::value)
        : __tree_(_VSTD::move(__m.__tree_))
        {
        }

    multimap(multimap&& __m, const allocator_type& __a);

    _LIBCPP_INLINE_VISIBILITY
    multimap& operator=(multimap&& __m)
        _NOEXCEPT_(is_nothrow_move_assignable<__base>::value)
        {
            __tree_ = _VSTD::move(__m.__tree_);
            return *this;
        }

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    multimap(initializer_list<value_type> __il, const key_compare& __comp = key_compare())
        : __tree_(__vc(__comp))
        {
            insert(__il.begin(), __il.end());
        }

    _LIBCPP_INLINE_VISIBILITY
    multimap(initializer_list<value_type> __il, const key_compare& __comp, const allocator_type& __a)
        : __tree_(__vc(__comp), __a)
        {
            insert(__il.begin(), __il.end());
        }

#if _LIBCPP_STD_VER > 11
    _LIBCPP_INLINE_VISIBILITY 
    multimap(initializer_list<value_type> __il, const allocator_type& __a)
        : multimap(__il, key_compare(), __a) {}
#endif

    _LIBCPP_INLINE_VISIBILITY
    multimap& operator=(initializer_list<value_type> __il)
        {
            __tree_.__assign_multi(__il.begin(), __il.end());
            return *this;
        }

#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    explicit multimap(const allocator_type& __a)
        : __tree_(__a)
        {
        }

    _LIBCPP_INLINE_VISIBILITY
    multimap(const multimap& __m, const allocator_type& __a)
        : __tree_(__m.__tree_.value_comp(), __a)
        {
            insert(__m.begin(), __m.end());
        }

    _LIBCPP_INLINE_VISIBILITY
          iterator begin() _NOEXCEPT {return __tree_.begin();}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator begin() const _NOEXCEPT {return __tree_.begin();}
    _LIBCPP_INLINE_VISIBILITY
          iterator end() _NOEXCEPT {return __tree_.end();}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator end() const _NOEXCEPT {return __tree_.end();}

    _LIBCPP_INLINE_VISIBILITY
          reverse_iterator rbegin() _NOEXCEPT {return reverse_iterator(end());}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator rbegin() const _NOEXCEPT
        {return const_reverse_iterator(end());}
    _LIBCPP_INLINE_VISIBILITY
          reverse_iterator rend() _NOEXCEPT {return reverse_iterator(begin());}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator rend() const _NOEXCEPT
        {return const_reverse_iterator(begin());}

    _LIBCPP_INLINE_VISIBILITY
    const_iterator cbegin()  const _NOEXCEPT {return begin();}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator cend() const _NOEXCEPT {return end();}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator crbegin() const _NOEXCEPT {return rbegin();}
    _LIBCPP_INLINE_VISIBILITY
    const_reverse_iterator crend() const _NOEXCEPT {return rend();}

    _LIBCPP_INLINE_VISIBILITY
    bool empty() const _NOEXCEPT {return __tree_.size() == 0;}
    _LIBCPP_INLINE_VISIBILITY
    size_type size() const _NOEXCEPT {return __tree_.size();}
    _LIBCPP_INLINE_VISIBILITY
    size_type max_size() const _NOEXCEPT {return __tree_.max_size();}

    _LIBCPP_INLINE_VISIBILITY
    allocator_type get_allocator() const _NOEXCEPT {return __tree_.__alloc();}
    _LIBCPP_INLINE_VISIBILITY
    key_compare    key_comp() const {return __tree_.value_comp().key_comp();}
    _LIBCPP_INLINE_VISIBILITY
    value_compare  value_comp() const
        {return value_compare(__tree_.value_comp().key_comp());}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
#ifndef _LIBCPP_HAS_NO_VARIADICS

    template <class ..._Args>
        iterator
        emplace(_Args&& ...__args);

    template <class ..._Args>
        iterator
        emplace_hint(const_iterator __p, _Args&& ...__args);

#endif  // _LIBCPP_HAS_NO_VARIADICS

    template <class _Pp,
              class = typename enable_if<is_constructible<value_type, _Pp>::value>::type>
        _LIBCPP_INLINE_VISIBILITY
        iterator insert(_Pp&& __p)
            {return __tree_.__insert_multi(_VSTD::forward<_Pp>(__p));}

    template <class _Pp,
              class = typename enable_if<is_constructible<value_type, _Pp>::value>::type>
        _LIBCPP_INLINE_VISIBILITY
        iterator insert(const_iterator __pos, _Pp&& __p)
            {return __tree_.__insert_multi(__pos.__i_, _VSTD::forward<_Pp>(__p));}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    _LIBCPP_INLINE_VISIBILITY
    iterator insert(const value_type& __v) {return __tree_.__insert_multi(__v);}

    _LIBCPP_INLINE_VISIBILITY
    iterator insert(const_iterator __p, const value_type& __v)
            {return __tree_.__insert_multi(__p.__i_, __v);}

    template <class _InputIterator>
        _LIBCPP_INLINE_VISIBILITY
        void insert(_InputIterator __f, _InputIterator __l)
        {
            for (const_iterator __e = cend(); __f != __l; ++__f)
                __tree_.__insert_multi(__e.__i_, *__f);
        }

#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    void insert(initializer_list<value_type> __il)
        {insert(__il.begin(), __il.end());}

#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS

    _LIBCPP_INLINE_VISIBILITY
    iterator erase(const_iterator __p) {return __tree_.erase(__p.__i_);}
    _LIBCPP_INLINE_VISIBILITY
    size_type erase(key_type& __k) {return __tree_.__erase_multi(__k);}
    _LIBCPP_INLINE_VISIBILITY
    iterator  erase(const_iterator __f, const_iterator __l)
        {return __tree_.erase(__f.__i_, __l.__i_);}
    _LIBCPP_INLINE_VISIBILITY
    void clear() {__tree_.clear();}

    _LIBCPP_INLINE_VISIBILITY
    void swap(multimap& __m)
        _NOEXCEPT_(__is_nothrow_swappable<__base>::value)
        {__tree_.swap(__m.__tree_);}

    _LIBCPP_INLINE_VISIBILITY
    iterator find(key_type& __k)             {return __tree_.find(__k);}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator find(key_type& __k) const {return __tree_.find(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,iterator>::type
    find(const _K2& __k)                           {return __tree_.find(__k);}
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,const_iterator>::type
    find(const _K2& __k) const                     {return __tree_.find(__k);}
#endif

    _LIBCPP_INLINE_VISIBILITY
    size_type      count(key_type& __k) const
        {return __tree_.__count_multi(__k);}
    _LIBCPP_INLINE_VISIBILITY
    iterator lower_bound(key_type& __k)
        {return __tree_.lower_bound(__k);}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator lower_bound(key_type& __k) const
            {return __tree_.lower_bound(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,iterator>::type
    lower_bound(const _K2& __k)       {return __tree_.lower_bound(__k);}

    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,const_iterator>::type
    lower_bound(const _K2& __k) const {return __tree_.lower_bound(__k);}
#endif

    _LIBCPP_INLINE_VISIBILITY
    iterator upper_bound(key_type& __k)
            {return __tree_.upper_bound(__k);}
    _LIBCPP_INLINE_VISIBILITY
    const_iterator upper_bound(key_type& __k) const
            {return __tree_.upper_bound(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,iterator>::type
    upper_bound(const _K2& __k)       {return __tree_.upper_bound(__k);}
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,const_iterator>::type
    upper_bound(const _K2& __k) const {return __tree_.upper_bound(__k);}
#endif

    _LIBCPP_INLINE_VISIBILITY
    pair<iterator,iterator>             equal_range(key_type& __k)
            {return __tree_.__equal_range_multi(__k);}
    _LIBCPP_INLINE_VISIBILITY
    pair<const_iterator,const_iterator> equal_range(key_type& __k) const
            {return __tree_.__equal_range_multi(__k);}
#if _LIBCPP_STD_VER > 11
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,pair<iterator,iterator>>::type
    equal_range(const _K2& __k)       {return __tree_.__equal_range_multi(__k);}
    template <typename _K2>
    _LIBCPP_INLINE_VISIBILITY
    typename enable_if<__is_transparent<_Compare, _K2>::value,pair<const_iterator,const_iterator>>::type
    equal_range(const _K2& __k) const {return __tree_.__equal_range_multi(__k);}
#endif

private:
    typedef typename __base::__node                    __node;
    typedef typename __base::__node_allocator          __node_allocator;
    typedef typename __base::__node_pointer            __node_pointer;
    typedef typename __base::__node_const_pointer      __node_const_pointer;
    typedef __map_node_destructor<__node_allocator> _Dp;
    typedef unique_ptr<__node, _Dp> __node_holder;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    __node_holder __construct_node();
    template <class _A0>
        __node_holder
         __construct_node(_A0&& __a0);
#ifndef _LIBCPP_HAS_NO_VARIADICS
    template <class _A0, class _A1, class ..._Args>
        __node_holder __construct_node(_A0&& __a0, _A1&& __a1, _Args&& ...__args);
#endif  // _LIBCPP_HAS_NO_VARIADICS
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Key, class _Tp, class _Compare, class _Allocator>
multimap<_Key, _Tp, _Compare, _Allocator>::multimap(multimap&& __m, const allocator_type& __a)
    : __tree_(_VSTD::move(__m.__tree_), __a)
{
    if (__a != __m.get_allocator())
    {
        const_iterator __e = cend();
        while (!__m.empty())
            __tree_.__insert_multi(__e.__i_,
                    _VSTD::move(__m.__tree_.remove(__m.begin().__i_)->__value_));
    }
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
typename multimap<_Key, _Tp, _Compare, _Allocator>::__node_holder
multimap<_Key, _Tp, _Compare, _Allocator>::__construct_node()
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.first));
    __h.get_deleter().__first_constructed = true;
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_.__cc.second));
    __h.get_deleter().__second_constructed = true;
    return __h;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class _A0>
typename multimap<_Key, _Tp, _Compare, _Allocator>::__node_holder
multimap<_Key, _Tp, _Compare, _Allocator>::__construct_node(_A0&& __a0)
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_), _VSTD::forward<_A0>(__a0));
    __h.get_deleter().__first_constructed = true;
    __h.get_deleter().__second_constructed = true;
    return __h;
}

#ifndef _LIBCPP_HAS_NO_VARIADICS

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class _A0, class _A1, class ..._Args>
typename multimap<_Key, _Tp, _Compare, _Allocator>::__node_holder
multimap<_Key, _Tp, _Compare, _Allocator>::__construct_node(_A0&& __a0, _A1&& __a1, _Args&& ...__args)
{
    __node_allocator& __na = __tree_.__node_alloc();
    __node_holder __h(__node_traits::allocate(__na, 1), _Dp(__na));
    __node_traits::construct(__na, _VSTD::addressof(__h->__value_),
                             _VSTD::forward<_A0>(__a0), _VSTD::forward<_A1>(__a1),
                             _VSTD::forward<_Args>(__args)...);
    __h.get_deleter().__first_constructed = true;
    __h.get_deleter().__second_constructed = true;
    return __h;
}

#endif  // _LIBCPP_HAS_NO_VARIADICS
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

#if !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class ..._Args>
typename multimap<_Key, _Tp, _Compare, _Allocator>::iterator
multimap<_Key, _Tp, _Compare, _Allocator>::emplace(_Args&& ...__args)
{
    __node_holder __h = __construct_node(_VSTD::forward<_Args>(__args)...);
    iterator __r = __tree_.__node_insert_multi(__h.get());
    __h.release();
    return __r;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
template <class ..._Args>
typename multimap<_Key, _Tp, _Compare, _Allocator>::iterator
multimap<_Key, _Tp, _Compare, _Allocator>::emplace_hint(const_iterator __p,
                                                        _Args&& ...__args)
{
    __node_holder __h = __construct_node(_VSTD::forward<_Args>(__args)...);
    iterator __r = __tree_.__node_insert_multi(__p.__i_, __h.get());
    __h.release();
    return __r;
}

#endif  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const multimap<_Key, _Tp, _Compare, _Allocator>& __x,
           const multimap<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return __x.size() == __y.size() && _VSTD::equal(__x.begin(), __x.end(), __y.begin());
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator< (const multimap<_Key, _Tp, _Compare, _Allocator>& __x,
           const multimap<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return _VSTD::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const multimap<_Key, _Tp, _Compare, _Allocator>& __x,
           const multimap<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return !(__x == __y);
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator> (const multimap<_Key, _Tp, _Compare, _Allocator>& __x,
           const multimap<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return __y < __x;
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const multimap<_Key, _Tp, _Compare, _Allocator>& __x,
           const multimap<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return !(__x < __y);
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const multimap<_Key, _Tp, _Compare, _Allocator>& __x,
           const multimap<_Key, _Tp, _Compare, _Allocator>& __y)
{
    return !(__y < __x);
}

template <class _Key, class _Tp, class _Compare, class _Allocator>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(multimap<_Key, _Tp, _Compare, _Allocator>& __x,
     multimap<_Key, _Tp, _Compare, _Allocator>& __y)
    _NOEXCEPT_(_NOEXCEPT_(__x.swap(__y)))
{
    __x.swap(__y);
}

_LIBCPP_END_NAMESPACE_STD

#endif  // _LIBCPP_MAP

#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP

#include "class-integer.hpp"
#include "class-matrix.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
class Hash
{
public:
    unsigned int operator()(Integer lhs) const
    {
        int val = lhs.val;
        return std::hash<int>()(val);
    }
};
class Equal
{
public:
    bool operator()(const Integer& lhs, const Integer& rhs) const { return lhs.val == rhs.val; }
};

namespace sjtu {
template<class T> class double_list
{
public:
    /**
     * elements
     * add whatever you want
     */
    struct list_node
    {
        T* pval = nullptr;
        list_node *next = nullptr, *pre = nullptr;
        list_node(){};
        list_node(const T& v) { pval = new T(v); }
        ~list_node()
        {
            if (pval != nullptr) delete pval;
        }
    };
    list_node *pbegin = nullptr, *pend = nullptr;
    int size = 0;

    // --------------------------
    /**
     * the follows are constructors and destructors
     * you can also add some if needed.
     */
    double_list(){};
    double_list(const double_list<T>& other)
    {
        // Here it is deep copy
        list_node* now = pbegin;
        while (now != nullptr) {
            list_node* temp = now;
            now = now->next;
            delete temp;
        }
        size = 0;
        now = other.pbegin;
        pbegin = pend = nullptr;
        while (now != nullptr) {
            insert_tail(*now->pval);
            now = now->next;
        }
    }
    ~double_list()
    {
        list_node* now = pbegin;
        while (now != nullptr) {
            list_node* temp = now;
            now = now->next;
            delete temp;
        }
        size = 0;
        pbegin = pend = nullptr;
    }

    class iterator
    {
    public:
        /**
         * elements
         * add whatever you want
         */

        // --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
         */
        list_node* p = nullptr;
        double_list* plist = nullptr;
        iterator(){};
        iterator(const iterator& t) { p = t.p, plist = t.plist; }
        iterator(list_node* p1, double_list* p2)
            : p(p1)
            , plist(p2){};
        ~iterator(){};
        /**
         * iter++
         */
        iterator operator++(int)
        {
            if (plist != nullptr) {
                if (p == nullptr) std::cerr << "end()++ ", throw "end()++";
                iterator temp = *this;
                p = p->next;
                return temp;
            }
            else
                std::cerr << "Iterator is not bounded to a double_list. ",
                    throw "Iterator is not bounded to a double_list.";
        }
        /**
         * ++iter
         */
        iterator& operator++()
        {
            if (plist != nullptr) {
                if (p == nullptr) std::cerr << "end()++  ", throw "end()++";
                p = p->next;
                return *this;
            }
            else
                std::cerr << "Iterator is not bounded to a double_list. ",
                    throw "Iterator is not bounded to a double_list.";
        }
        /**
         * iter--
         */
        iterator operator--(int)
        {
            if (plist != nullptr) {
                if (p == plist->pbegin) p = nullptr, std::cerr << "begin()--  ", throw "begin()--";
                iterator temp = *this;
                if (p != nullptr)
                    p = p->pre;
                else
                    p = plist->pend;
                return temp;
            }
            else
                std::cerr << "Iterator is not bounded to a double_list.  ",
                    throw "Iterator is not bounded to a double_list.";
        }
        /**
         * --iter
         */
        iterator& operator--()
        {
            if (plist != nullptr) {
                if (p == plist->pbegin) p = nullptr, std::cerr << "begin()--  ", throw "begin()--";
                if (p != nullptr)
                    p = p->pre;
                else
                    p = plist->pend;
                return *this;
            }
            else
                std::cerr << "Iterator is not bounded to a double_list.  ",
                    throw "Iterator is not bounded to a double_list.";
        }
        /**
         * if the iter didn't point to a value
         * throw " invalid"
         */
        T& operator*() const
        {
            if (plist == nullptr || p == nullptr)
                std::cerr << "Iterator point to null but is asked to dereference.  ",
                    throw "Iterator point to null but is asked to dereference.";
            return *p->pval;
        }
        /**
         * other operation
         */
        T* operator->() const noexcept { return p->pval; }
        bool operator==(const iterator& rhs) const { return plist == rhs.plist && p == rhs.p; }
        bool operator!=(const iterator& rhs) const { return !(plist == rhs.plist && p == rhs.p); }
    };
    /**
     * return an iterator to the beginning
     */
    iterator begin() { return iterator(pbegin, this); }
    /**
     * return an iterator to the ending
     * in fact, it returns the iterator point to nothing,
     * just after the last element.
     */
    iterator end() { return iterator(nullptr, this); }
    /**
     * if the iter didn't point to anything, do nothing,
     * otherwise, delete the element pointed by the iter
     * and return the iterator point at the same "index"
     * e.g.
     * 	if the origin iterator point at the 2nd element
     * 	the returned iterator also point at the
     *  2nd element of the list after the operation
     *  or nothing if the list after the operation
     *  don't contain 2nd elememt.
     */
    iterator erase(iterator pos)
    {
        if (pos.plist != this || pos.p == nullptr) return pos;
        iterator res = iterator(pos.p->next, pos.plist);
        --size;
        if (!size) {
            pbegin = pend = nullptr;
            delete pos.p;
        }
        else if (pos.p == pbegin) {
            pbegin = pos.p->next;
            pbegin->pre = nullptr;
            delete pos.p;
        }
        else if (pos.p == pend) {
            pend = pos.p->pre;
            pend->next = nullptr;
            delete pos.p;
        }
        else {
            pos.p->pre->next = pos.p->next;
            pos.p->next->pre = pos.p->pre;
            delete pos.p;
        }
        return res;
    }

    /**
     * the following are operations of double list
     */
    void insert_head(const T& val)
    {
        list_node* pnew = new list_node(val);
        ++size;
        if (size == 1) {
            pbegin = pend = pnew;
        }
        else {
            pnew->next = pbegin;
            pbegin->pre = pnew;
            pbegin = pnew;
        }
    }
    void insert_tail(const T& val)
    {
        list_node* pnew = new list_node(val);
        if (pend != nullptr) {
            pnew->pre = pend, pend->next = pnew;
            ++size;
            pend = pend->next;
        }
        else {
            ++size;
            pbegin = pend = pnew;
        }
    }
    void delete_head()
    {
        if (pbegin == nullptr) return;
        --size;
        list_node* temp = pbegin;
        if (size == 0) {
            pend = pbegin = nullptr;
        }
        else {
            pbegin = pbegin->next;
            pbegin->pre = nullptr;
        }
        delete temp;
    }
    void delete_tail()
    {
        if (pend == nullptr) return;
        --size;
        list_node* temp = pend;
        if (size == 0) {
            pend = pbegin = nullptr;
        }
        else {
            pend = pend->pre;
            pend->next = nullptr;
        }
        delete temp;
    }
    /**
     * if didn't contain anything, return true,
     * otherwise false.
     */
    bool empty() { return size == 0; }
};

template<class Key, class T, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
class hashmap
{
public:
    using value_type = pair<const Key, T>;
    /**
     * elements
     * add whatever you want
     */

    struct map_node
    {
        map_node* pt = nullptr;
        size_t key_hash = 0;
        value_type* pv = nullptr;
        map_node(size_t hash, const Key& key, const T& val)
        {
            pt = nullptr, key_hash = hash;
            pv = new value_type(key, val);
        }
        map_node(const map_node& other)
        {
            pt = other.pt, key_hash = other.key_hash;
            pv = new value_type(*other.pv);
        }
        ~map_node()
        {
            pt = nullptr, key_hash = 0;
            delete pv;
        }
    };
    const int prime[9] = {149, 379, 1361, 10067, 100189, 1000159, 10000019, 100000007, 1000000007};
    int capacity_level = 0, capacity = 149;
    const int max_level = 8;
    const double max_ratio = 0.5;
    int size = 0;
    map_node** map;
    Hash h = Hash();
    Equal e = Equal();

    // --------------------------

    /**
     * the follows are constructors and destructors
     * you can also add some if needed.
     */
    hashmap() { map = new map_node*[capacity](); }
    hashmap(const hashmap& other)
    {
        capacity_level = other.capacity_level, capacity = other.capacity;
        h = other.h, e = other.e, size = other.size;
        map = new map_node*[capacity]();
        for (int i = 0; i < capacity; ++i) {
            map_node* now = other.map[i];
            if (now != nullptr) {
                map[i] = copy(now);
                now = now->pt;
            }
            else
                continue;
            map_node* here = map[i];
            while (now != nullptr) {
                here->pt = copy(now);
                here = here->pt;
                now = now->pt;
            }
        }
    }
    ~hashmap()
    {
        for (int i = 0; i < capacity; ++i) {
            map_node* now = map[i];
            while (now != nullptr) {
                map_node* temp = now;
                now = now->pt;
                delete temp;
            }
        }
        delete[] map;
    }
    hashmap& operator=(const hashmap& other)
    {
        for (int i = 0; i < capacity; ++i) {
            map_node* now = map[i];
            while (now != nullptr) {
                map_node* temp = now;
                now = now->pt;
                delete temp;
            }
        }
        delete[] map;
        capacity_level = other.capacity_level, capacity = other.capacity;
        h = other.h, e = other.e, size = other.size;
        map = new map_node*[capacity]();
        for (int i = 0; i < capacity; ++i) {
            map_node* now = other.map[i];
            if (now != nullptr) {
                map[i] = copy(now);
                now = now->pt;
            }
            else
                continue;
            map_node* here = map[i];
            while (now != nullptr) {
                here->pt = copy(now);
                here = here->pt;
                now = now->pt;
            }
        }
        return *this;
    }

    class iterator
    {
    public:
        /**
         * elements
         * add whatever you want
         */
        map_node* pt;
        // --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
         */
        iterator() { pt = nullptr; }
        iterator(const iterator& t) { pt = t.pt; }
        iterator(map_node* p) { pt = p; }
        ~iterator(){};

        /**
         * if point to nothing
         * throw
         */
        value_type& operator*() const
        {
            if (pt != nullptr) {
                return *pt->pv;
            }
            std::cerr << "dereference null pointer in hashmap iterator   ", throw "Fuck";
        }

        /**
         * other operation
         */
        value_type* operator->() const noexcept { return pt->pv; }
        bool operator==(const iterator& rhs) const { return pt == rhs.pt; }
        bool operator!=(const iterator& rhs) const { return pt != rhs.pt; }
    };

    map_node* copy(const map_node* other)
    {
        map_node* res = new map_node(*other);
        return res;
    }
    void clear()
    {
        for (int i = 0; i < capacity; ++i) {
            map_node* now = map[i];
            while (now != nullptr) {
                map_node* temp = now;
                now = now->pt;
                delete temp;
            }
        }
        delete[] map;
        capacity_level = 0, capacity = 149, size = 0;
        map = new map_node*[capacity]();
    }
    /**
     * you need to expand the hashmap dynamically
     */
    void expand()
    {
        int old_capacity = capacity;
        capacity = prime[++capacity_level];
        map_node** newmap = new map_node*[capacity]();
        for (int i = 0; i < old_capacity; ++i) {
            map_node* now = map[i];
            while (now != nullptr) {
                value_type val = *now->pv;
                size_t hash = h(val.first);
                int map_loc = hash % capacity;
                map_node* here = new map_node(hash, val.first, val.second);
                here->pt = newmap[map_loc];
                newmap[map_loc] = here;
                map_node* temp = now;
                now = now->pt;
                delete temp;
            }
        }
        delete[] map;
        map = newmap;
    }

    /**
     * the iterator point at nothing
     */
    iterator end() const { return iterator(); }
    /**
     * find, return a pointer point to the value
     * not find, return the end (point to nothing)
     */
    iterator find(const Key& key) const
    {
        size_t hash = h(key);
        int map_loc = hash % capacity;
        map_node* now = map[map_loc];
        // std::cout << "???" << std::endl;
        while (now != nullptr) {
            if (now->pv == nullptr) std::cout << "OK!" << std::endl;
            if (e(key, now->pv->first)) {
                return iterator(now);
            }
            now = now->pt;
        }
        return iterator();
    }
    /**
     * already have a value_pair with the same key
     * -> just update the value, return false
     * not find a value_pair with the same key
     * -> insert the value_pair, return true
     */
    sjtu::pair<iterator, bool> insert(const value_type& value_pair)
    {
        ++size;
        // expand();
        if (capacity_level < max_level && 1.0 * size / capacity >= max_ratio) {
            expand();
        }
        size_t hash = h(value_pair.first);
        int map_loc = hash % capacity;
        sjtu::pair<iterator, bool> res;
        map_node* now = map[map_loc];
        while (now != nullptr) {
            if (e(now->pv->first, value_pair.first)) {
                res.second = 1;
                res.first = iterator(now);
                now->pv->second = value_pair.second;
                return res;
            }
            now = now->pt;
        }
        map_node* new_node = new map_node(hash, value_pair.first, value_pair.second);
        res.first = iterator(new_node);
        new_node->pt = map[map_loc];
        map[map_loc] = new_node;
        res.second = 0;
        return res;
    }
    /**
     * the value_pair exists, remove and return true
     * otherwise, return false
     */
    bool remove(const Key& key)
    {
        size_t hash = h(key);
        int map_loc = hash % capacity;
        map_node* now = map[map_loc];
        --size;
        if (e(key, now->pv->first)) {
            map[map_loc] = now->pt;
            delete now;
            return 1;
        }
        map_node* last = now;
        now = now->pt;
        while (now != nullptr) {
            if (e(key, now->pv->first)) {
                last->pt = now->pt;
                delete now;
                return 1;
            }
            last = now;
            now = now->pt;
        }
        return 0;
    }
};

template<class Key, class T, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
class linked_hashmap
{
public:
    typedef pair<const Key, T> value_type;
    /**
     * elements
     * add whatever you want
     */
    struct linked_node
    {
        value_type* pv;
        linked_node* next;
        typename double_list<linked_node*>::iterator it;
        linked_node() { pv = nullptr, next = nullptr; }
        linked_node(value_type val)
        {
            pv = new value_type(val);
            next = nullptr;
        }
        linked_node(const linked_node& other)
        {
            pv = new value_type(*other.pv);
            next = other.next, it = other.it;
        }
        ~linked_node()
        {
            delete pv;
            pv = next = nullptr;
        }
    };

    const int prime[9] = {149, 379, 1361, 10067, 100189, 1000159, 10000019, 100000007, 1000000007};
    int capacity_level = 0, capacity = 149;
    const int max_level = 8;
    const double max_ratio = 0.5;
    int sz = 0;
    linked_node** map;
    double_list<linked_node*> list;
    Hash h = Hash();
    Equal e = Equal();

    // --------------------------

    class const_iterator;
    class iterator
    {
    public:
        /**
         * elements
         * add whatever you want
         */

        typename double_list<linked_node*>::iterator it;

        // --------------------------

        iterator(){};
        iterator(const iterator& other)
            : it(other.it){};
        ~iterator(){};

        /**
         * iter++
         */
        iterator operator++(int)
        {
            iterator temp = *this;
            ++it;
            return temp;
        }
        /**
         * ++iter
         */
        iterator& operator++()
        {
            ++it;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int)
        {
            iterator temp = *this;
            --it;
            return temp;
        }
        /**
         * --iter
         */
        iterator& operator--()
        {
            --it;
            return *this;
        }

        /**
         * if the iter didn't point to a value
         * throw "star invalid"
         */
        value_type& operator*() const { return *((*it)->pv); }
        value_type* operator->() const noexcept { return (*it)->pv; }

        /**
         * operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator& rhs) const { return it == rhs.it; }
        bool operator!=(const iterator& rhs) const { return it != rhs.it; }
        bool operator==(const const_iterator& rhs) const { return it == rhs.it; }
        bool operator!=(const const_iterator& rhs) const { return it != rhs.it; }
    };

    class const_iterator
    {
    public:
        /**
         * elements
         * add whatever you want
         */
        typename double_list<linked_node*>::iterator it;

        // --------------------------

        const_iterator(){};
        const_iterator(const iterator& other)
            : it(other.it){};

        /**
         * iter++
         */
        const_iterator operator++(int)
        {
            const_iterator temp = *this;
            ++it;
            return temp;
        }
        /**
         * ++iter
         */
        const_iterator& operator++()
        {
            ++it;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int)
        {
            const_iterator temp = *this;
            --it;
            return temp;
        }
        /**
         * --iter
         */
        const_iterator& operator--()
        {
            --it;
            return *this;
        }

        /**
         * if the iter didn't point to a value
         * throw
         */
        const value_type& operator*() const { return *((*it)->pv); }
        const value_type* operator->() const noexcept { return (*it)->pv; }

        /**
         * operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator& rhs) const { return it = rhs.it; }
        bool operator!=(const iterator& rhs) const { return it != rhs.it; }
        bool operator==(const const_iterator& rhs) const { return it == rhs.it; }
        bool operator!=(const const_iterator& rhs) const { return it != rhs.it; }
    };

    linked_hashmap() {}
    linked_hashmap(const linked_hashmap& other) {}
    ~linked_hashmap() {}
    linked_hashmap& operator=(const linked_hashmap& other) {}

    /**
     * return the value connected with the Key(O(1))
     * if the key not found, throw
     */
    T& at(const Key& key) {}
    const T& at(const Key& key) const {}
    T& operator[](const Key& key) {}
    const T& operator[](const Key& key) const {}

    /**
     * return an iterator point to the first
     * inserted and existed element
     */
    iterator begin() {}
    const_iterator cbegin() const {}
    /**
     * return an iterator after the last inserted element
     */
    iterator end() {}
    const_iterator cend() const {}
    /**
     * if didn't contain anything, return true,
     * otherwise false.
     */
    bool empty() const {}

    void clear() {}

    size_t size() const {}
    /**
     * insert the value_piar
     * if the key of the value_pair exists in the map
     * update the value instead of adding a new element，
     * then the order of the element moved from inner of the
     * list to the head of the list
     * and return false
     * if the key of the value_pair doesn't exist in the map
     * add a new element and return true
     */
    pair<iterator, bool> insert(const value_type& value) {}
    /**
     * erase the value_pair pointed by the iterator
     * if the iterator points to nothing
     * throw
     */
    void remove(iterator pos) {}
    /**
     * return how many value_pairs consist of key
     * this should only return 0 or 1
     */
    size_t count(const Key& key) const {}
    /**
     * find the iterator points at the value_pair
     * which consist of key
     * if not find, return the iterator
     * point at nothing
     */
    iterator find(const Key& key) {}
};

class lru
{
    using lmap = sjtu::linked_hashmap<Integer, Matrix<int>, Hash, Equal>;
    using value_type = sjtu::pair<const Integer, Matrix<int>>;

public:
    lru(int size) {}
    ~lru() {}
    /**
     * save the value_pair in the memory
     * delete something in the memory if necessary
     */
    void save(const value_type& v) const {}
    /**
     * return a pointer contain the value
     */
    Matrix<int>* get(const Integer& v) const {}
    /**
     * just print everything in the memory
     * to debug or test.
     * this operation follows the order, but don't
     * change the order.
     */
    void print() {}
};
}   // namespace sjtu

#endif
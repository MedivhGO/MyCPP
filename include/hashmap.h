/*
* Assignment 2: HashMap template interface (STARTER CODE)
*      TODO: write a comment here.
*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>             // for cout
#include <iomanip>              // for setw, setprecision, setfill, right
#include <sstream>              // for istringstream
#include <vector>

#include "hashmap_iterator.h"

// add any other includes that are necessary

/*
* Template class for a HashMap
*
* K = key type
* M = mapped type
* H = hash function type used to hash a key; if not provided, defaults to std::hash<K>
*
* Notes: When dealing with the Stanford libraries, we often call M the value
* (and maps store key/value pairs).
*
* However, we name it M for mapped type to avoid confusion with value_type.
* value_type is what the container is storing, which is a std::pair<const K, M>.
*
* All STL containers have a value_type and STL algorithms may use the value_type alias, so
* we try our best to follow that convention.
*
* Example:
*      HashMap<std::string, int>
*      This means K = key = std::string,
*                 M = mapped = int,
*                 value_type = std::pair<const std::string, int>.
*
* Concept requirements:
*      - H is function type that with function prototype size_t hash(const K& key).
*           The const and reference are not required, but key cannot be modified in function.
*      - K and M must be regular (copyable, default constructible, and equality comparable).
*/
template <typename K, typename M, typename H = std::hash<K>>
class HashMap {
public:
    /*
    * Alias for std::pair<const K, M>, used by the STL (such as in std::inserter)
    * As noted above, value_type is not the same as the mapped_type!
    *
    * Usage:
    *      HashMap::value_type val = {3, "Avery"};
    *      map.insert(val);
    */
    using value_type = std::pair<const K, M>;

    /*
     * Alias for the iterator type. Recall that it's impossible for an external client
     * to figure out the type of this iterator (you would've never guessed what the template
     * parameters are here), which is why the aliases are crucial.
     *
     * Usage:
     *      HashMap::iterator iter = map.begin();
     */
    using iterator = HashMapIterator<HashMap, false>;

    /*
     * Alias for the const_iterator type. Recall that it's impossible for an external client
     * to figure out the type of this iterator (you would've never guessed what the template
     * parameters are here), which is why the aliases are crucial.
     *
     * Usage:
     *      const auto& cmap = map;
     *      HashMap::iterator iter = cmap.begin();
     *
     * Notes: recall that you cannot modify the element a const_iterator is pointing to.
     * Also, a const_iterator is not a const iterator!
     */
    using const_iterator = HashMapIterator<HashMap, true>;

    /*
     * Declares that the HashMapIterator class are friends of the HashMap class.
     * This allows the HashMapIterators to see the private members, which is
     * important because the iterator needs to know what element it is pointing to.
     */
    friend class HashMapIterator<HashMap, false>;
    friend class HashMapIterator<HashMap, true>;

    /*
    * Default constructor
    * Creates an empty HashMap with default number of buckets and hash function.
    *
    * Usage:
    *      HashMap map;
    *      HashMap map{};
    *
    * Complexity: O(B), B = number of buckets
    */
    HashMap();

    /*
    * Constructor with bucket_count and hash function as parameters.
    *
    * Creates an empty HashMap with a specified initial bucket_count and hash funciton.
    * If no hash function provided, default value of H is used.
    *
    * Usage:
    *      HashMap(10) map;
    *      HashMap map(10, [](const K& key) {return key % 10; });
    *      HashMap map{10, [](const K& key) {return key % 10; }};
    *
    * Complexity: O(B), B = number of buckets
    *
    * Notes : what is explicit? Explicit specifies that a constructor
    * cannot perform implicit conversion on the parameters, or use copy-initialization.
    * That's good, as nonsense like the following won't compile:
    *
    * HashMap<int, int> map(1.0);  // double -> int conversion not allowed.
    * HashMap<int, int> map = 1;   // copy-initialization, does not compile.
    */
    explicit HashMap(size_t bucket_count, const H& hash = H());

    /*
    * Destructor.
    *
    * Usage: (implicitly called when HashMap goes out of scope)
    *
    * Complexity: O(N), N = number of elements
    */
    ~HashMap();

    /*
    * Returns the number of (K, M) pairs in the map.
    *
    * We declare this function inline since it is short and
    * the compiler can optimize by doing a direct inline substitution.
    *
    * Parameters: none
    * Return value: size_t
    *
    * Usage:
    *      if (map.size() < 3) { ... }
    *
    * Complexity: O(1) (inlined because function is short)
    */
    inline size_t size() const;

    /*
    * Returns whether the HashMap is empty.
    *
    * Parameters: none
    * Return value: bool
    *
    * Usage:
    *      if (map.empty()) { ... }
    *
    * Complexity: O(1) (inlined because function is short)
    */
    inline bool empty() const;

    /*
    * Returns the load_factor, defined as size/bucket_count.
    *
    * Parameters: none
    * Return value: float
    *
    * Usage:
    *      float load_factor = map.load_factor();
    *
    * Complexity: O(1) (inlined because function is short)
    *
    * Notes: our minimal implementation does not automatically rehash when the load
    * factor is too high. If you want as an extension, you can implement automatic rehashing.
    */
    inline float load_factor() const;

    /*
    * Returns the number of buckets.
    *
    * Parameters: none
    * Return value: size_t - number of buckets
    *
    * Usage:
    *      size_t buckets = map.bucket_count();
    *
    * Complexity: O(1) (inlined because function is short)
    *
    * Notes: our minimal implementation does not automatically rehash when the load
    * factor is too high. If you want, you can implement automatic rehashing.
    *
    * What is noexcept? It's a guarantee that this function does not throw
    * exceptions, allowing the compiler to optimize this function further.
    * A noexcept function that throws an exception will automatically
    * terminate the program.
    */
    inline size_t bucket_count() const;

    /*
    * Returns whether or not the HashMap contains the given key.
    *
    * Parameters: const l-value reference to type K, the given key
    * Return value: bool
    *
    * Usage:
    *      if (map.contains("Avery")) { map.at("Avery"); ... }
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: Recall that when using a std::map, you use the map.count(key) function
    * (returns 0 or 1) to check if key exists. In C++20, map.contains(key) will be available.
    * Since contains feels more natural to students who've used the Stanford libraries
    * and will be available in the future, we will implement map.contains(key).
    */
    bool contains(const K& key) const;

    /*
    * Returns a l-value reference to the mapped value given a key.
    * If no such element exists, throws exception of type std::out_of_range.
    *
    * Parameters: key of type K.
    * Return value: l-value reference to type V, the mapped value of key.
    *
    * Usage:
    *      map.at(3) = "Avery";            // assuming {3, "Avery"} is in the map.
    *      std::string s = map.at(3);      // s = "Avery"
    *
    * Exceptions: std::out_of_range if key is not in the map.
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: recall that operator[], which you will implement, does not throw exceptions,
    * if a key is not found. Instead, it will create a K/M pair for that key with a default
    * mapped value. This function is also not const-correct, which you will fix in milestone 2.
    */
    M& at(const K& key);

    const M& at(const K& key) const;

    /*
    * Removes all K/M pairs the HashMap.
    *
    * Parameters: none
    * Return value: none
    *
    * Usage:
    *      map.clear();
    *
    * Complexity: O(N), N = number of elements
    *
    * Notes: clear removes all the elements in the HashMap and frees the memory associated
    * with those elements, but the HashMap should still be in a valid state and is
    * ready to be inserted again, as if it were a newly constructed HashMap with no elements.
    * The number of buckets should stay the same.
    */
    void clear();

    /*
     * Finds the element with the given key, and returns an iterator to that element.
     * If an element is not found, an iterator to end() is returned.
     *
     * Parameters: const l-value reference to type K, the key we are looking for.
     * Return value: iterator to the K/M element with given key.
     *
     * Usage:
     *      auto iter = map.find(4);
     *      iter->second = "Hello"; // sets whatever 4 was mapped to to "Hello".
     *
     * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
     */
    iterator find(const K& key);

    const_iterator find(const K& key) const;

    /*
    * Inserts the K/M pair into the HashMap, if the key does not already exist.
    * If the key exists, then the operation is a no-op.
    *
    * Parameters: const l-value reference to value_type (K/M pair)
    * Return value:
    *          pair<iterator, bool>, where:
    *              iterator - iterator to the value_type element with the given key
    *                         this element may have been just added, or may have already existed.
    *              bool - true if the element was successfully added,
    *                      false if the element already existed.
    *
    * Usage:
    *      HashMap<int, std::string> map;
    *      auto [iter1, insert1] = map.insert({3, "Avery"}); // inserts {3, "Avery"}, iter1 points to that element, insert1 = true
    *      auto [iter2, insert2] = map.insert({3, "Anna"});  // no-op, iter2 points to {3, "Avery"}, insert2 = false
    *
    * Complexity: O(1) amortized average case
    */
    std::pair<iterator, bool> insert(const value_type& value);

    /*
    * Erases a K/M pair (if one exists) corresponding to given key from the HashMap.
    * This is a no-op if the key does not exist.
    *
    * Parameters: const l-value reference to K, key to be removed.
    * Return value: true if K/M pair was found and removed, false if key was not found.
    *
    * Usage:
    *      map.erase(3);           // assuming K = int, erases element with key 3, returns true
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: a call to erase should maintain the order of existing iterators,
    * other than iterators to the erased K/M element.
    */
    bool erase(const K& key);

    /*
    * Erases the K/M pair that pos points to.
    * Behavior is undefined if pos is not a valid and dereferencable iterator.
    *
    * Parameters: const_iterator pos, iterator to element to be removed
    * Return value: the iterator immediately following pos, which may be end().
    *
    * Usage:
    *       auto iter = map.find(3);
    *       auto next = map.erase(iter);    // erases element that iter is pointing to
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: a call to erase should maintain the order of existing iterators,
    * other than iterators to the erased K/M element.
    */
    iterator erase(const_iterator pos);

    /*
    * Resizes the array of buckets, and rehashes all elements. new_buckets could
    * be larger than, smaller than, or equal to the original number of buckets.
    *
    * Parameters: new_buckets - the new number of buckets. Must be greater than 0.
    * Return value: none
    *
    * Usage:
    *      map.rehash(30)
    *
    * Exceptions: std::out_of_range if new_buckets = 0.
    *
    * Complexity: O(N) amortized average case, O(N^2) worst case, N = number of elements
    *
    * Notes: our minimal HashMap implementation does not support automatic rehashing, but
    * std::unordered_map will automatically rehash, even if you rehash to
    * a very small number of buckets. For this reason, std::unordered_map.rehash(0)
    * is allowed and forces an unconditional rehash. We will not require this behavior.
    * If you want, you could implement this.
    *
    * Previously, this function was part of the assignment. However, it's a fairly challenging
    * linked list problem, and students had a difficult time finding an elegant solution.
    * Instead, we will ask short answer questions on this function instead.
    */
    void rehash(size_t new_buckets);

    /*
     * Returns an iterator to the first element.
     * This overload is used when the HashMap is non-const.
     *
     * Usage:
     *      auto iter = map.begin();
     */
    iterator begin();

    /*
     * Returns a const_iterator to the first element.
     * This overload is used when the HashMap is const.
     *
     * Usage:
     *      auto iter = cmap.begin();
     */    
    const_iterator begin() const;

    /*
     * Returns an iterator to one past the last element.
     * This overload is used when the HashMap is non-const.
     *
     * Usage:
     *      while (iter != map.end()) {...}
     */
    iterator end();

    const_iterator end() const;


    /*
    * Function that will print to std::cout the contents of the hash table as
    * linked lists, and also displays the size, number of buckets, and load factor.
    *
    * Parameters: none
    * Return value: none
    *
    * Usage:
    *      map.debug();
    *
    * Complexity: O(N), N = number of elements.
    *
    * Notes: debug will not compile if either K or V does not support operator<< for std::ostream.
    * this function will crash if your linked list logic is incorrect (eg. forgot to reset the
    * last node's next to nullptr). Check where the source of the compiler error comes from
    * before complaining to us that our starter code doesn't work!
    *
    * Tip: place map.debug() in various places in the test cases to figure out which operation
    * is failing. Super useful when we debugged our code.
    */
    void debug() const;

    /* EXTRA CONSTURCTORS */

    /*
     * Range constructor
     * Creates a HashMap with the elements in the range [first, last).
     *
     * Requirements: InputIt must be iterators to a container whose elements are pair<K, M>.
     *
     * Usage:
     *      std::vector<std::pair<char, int>> vec {{'a', 3}, {'b', 5}, {'c', 7}};
     *      HashMap<char, int> map{vec.begin(), vec.end()};
     *
     * Complexity: O(N), where N = std::distance(first, last);
     */
    template <typename InputIt>
    HashMap(InputIt first, InputIt last, size_t bucket_count = kDefaultBuckets, const H& hash = H());

    /*
     * Initializer list constructor
     * Creates a HashMap with the elements in the initializer list init
     *
     * Requirements: init must be an initializer_list whose elements are pair<K, M>.
     *
     * Usage:
     *      HashMap<char, int> map{{'a', 3}, {'b', 5}, {'c', 7}};
     *
     * Complexity: O(N), where N = init.size();
     *
     * Notes: you may want to do some research on initializer_lists. The most important detail you need
     * to know is that they are very limited, and have three functions: init.begin(), init.end(), and init.size().
     * There are no other ways to access the elements in an initializer_list.
     * As a result, you probably want to leverage the range constructor you wrote in the previous function!
     *
     * Also, you should check out the delegating constructor note in the .cpp file.
     */
    HashMap(std::initializer_list<value_type> init, size_t bucket_count = kDefaultBuckets, const H& hash = H());

    /*
     * Indexing operator
     * Retrieves a reference to the mapped value corresponding to this key.
     * If no such key exists, a key/mapped value pair will be added to the HashMap.
     * The mapped value will have the default value for type M.
     *
     * Usage:
     *      HashMap<int, std::string> map;
     *      map[3] = "Avery"; // creates the pair {3, "Avery"}
     *      auto name = map[3]; // name is now "Avery"
     *      auto name2 = map[4]; // creates the pair {4, ""}, name2 is now ""
     *
     * Complexity: O(1) average case amortized plus complexity of K and M's constructor
     */
    M& operator[](const K& key); // 由于 [] 运算符会为 hashmap 增加kv，所以const对象不能进行调用， 因此不写 const 版本。

    /* Milestone 2 headers (you need to declare these) */
    // TODO: declare headers for copy constructor/assignment, move constructor/assignment

    HashMap(const HashMap& m);

    HashMap(HashMap&& m);

    HashMap& operator=(const HashMap& m);
    
    HashMap& operator=(HashMap&& m);

private:
    /*
    * node structure represented a node in a linked list.
    * Each node consists of a value_type (K/M pair) and a next pointer.
    *
    * This is implemented in the private section as clients should not be dealing
    * with anything related to the node struct.
    *
    * Usage;
    *      HashMap<K, M, H>::node n;
    *      n->value = {3, 4};
    *      n->next = nullptr;
    */
    struct node {
        value_type value;
        node* next;

        /*
        * Constructor with default values, so even if you forget to set next to nullptr it'll be fine.
        *
        * Usage:
        *      node* new_node = node({key, mapped}, next_ptr);
        */
        node(const value_type& value = value_type(), node* next = nullptr) :
            value(value), next(next) {}
    };

    /*
    * Type alias for a pair of node*'s.
    *
    * This is used in find_node.
    *
    * Usage:
    *      auto& [prev, curr] = node_pair{nullptr, new node()};
    */
    using node_pair = std::pair<typename HashMap::node*, typename HashMap::node*>;

    /*
    * Finds the node N with given key, and returns a node_pair consisting of
    * the node whose's next is N, and N. If node is not found, {nullptr, nullptr}
    * is returned. If node found is the first in the list, {nullptr, node} is returned.
    *
    * Example given list: front -> [A] -> [B] -> [C] -> /
    * where A, B, C, D are pointers, then
    *
    * find_node(A_key) = {nullptr, A}
    * find_node(B_key) = {A, B}
    * find_node(C_key) = {B, C}
    * find_node(D_key) = {nullptr, nullptr}
    *
    * Usage:
    *      auto& [prev, curr] = find_node(3);
    *      if (prev == nullptr) { ... }
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: this function is necessary because when erasing, we need to change the
    * next pointer of the node before the one we are erasing.
    *
    * Hint: on the assignment, you should NOT need to call this function.
    */
    node_pair find_node(const K& key) const;

    /*
    * Finds the first bucket in _buckets_array that is non-empty.
    *
    * Hint: on the assignment, you should NOT need to call this function.
    */
    size_t first_not_empty_bucket() const;

    /*
    * Creates an iterator that points to the element curr->value.
    *
    * Hint: on the assignment, you should NOT need to call this function.
    */
    iterator make_iterator(node* curr);

    const_iterator make_const_iterator(node* curr) const;

    /* Private member variables */

    /*
    * instance variable: _size, the number of elements, which are K/M pairs.
    * Don't confuse this with the number of buckets!
    */
    size_t _size;

    /*
    * instance variable: _hash_function, a function (K -> size_t) that is used
    * to hash K's to determine which bucket they should be inserted/found.
    *
    * Remember to mod the output of _hash_function by _bucket_count!
    *
    * Usage:
    *      K element = // something;
    *      size_t index = _hash_function(element) % _bucket_count;
    *
    */
    H _hash_function;

    /*
    * The array (vector) of buckets. Each bucket is a linked list,
    * and the item stored in the bucket is the front pointer of that linked list.
    *
    * Usage:
    *      node* ptr = _buckets_array[index];          // _buckets_array is array of node*
    *      const auto& [key, mapped] = ptr->value;     // each node* contains a value that is a pair
    */
    std::vector<node*> _buckets_array;

    /*
    * A constant for the default number of buckets for the default constructor.
    */
    static const size_t kDefaultBuckets = 10;

    /*
     * A private type alias used by the iterator class so it can traverse
     * the buckets.
     */
    using bucket_array_type = decltype(_buckets_array);

};

/*
* Ask compiler to put the template implementation here.
*/
/*
* Assignment 2: HashMap template implementation (STARTER CODE)
*      TODO: write a comment here.
*/

// #include "hashmap.h"

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() :
        HashMap{kDefaultBuckets} { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash) :
        _size{0},
        _hash_function{hash},
        _buckets_array(bucket_count, nullptr) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(const HashMap<K, M, H>& m)
    : _size{0}, _hash_function(m._hash_function) {
    _buckets_array.resize(m.bucket_count());
    for (auto [key, val] : m) {
        insert({key, val});
    }
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap<K, M, H>&& m)
    : _size{std::move(m._size)},
      _buckets_array{m.bucket_count(), nullptr},
      _hash_function{std::move(m._hash_function)} {
    for (size_t i = 0; i < m.bucket_count(); ++i) {
        _buckets_array[i] = std::move(m._buckets_array[i]);
        m._buckets_array[i] = nullptr;
    }
    m._size = 0;
}

template <typename K, typename M, typename H>
HashMap<K, M, H>& HashMap<K, M, H>::operator=(const HashMap<K, M, H>& m) {
    if (this == &m) {
        return *this;
    }
    clear();
    _buckets_array.resize(m.bucket_count());
    _hash_function = m._hash_function;
    for (auto [key, val] : m) {
        insert({key, val});
    }
    return *this;
}

template <typename K, typename M, typename H>
HashMap<K, M, H>& HashMap<K, M, H>::operator=(HashMap<K, M, H>&& m) {
    if (this == &m) {
        return *this;
    }
    clear();
    _size = std::move(m._size);
    _hash_function = std::move(m._hash_function);
    _buckets_array.resize(m.bucket_count());
    for (size_t i = 0; i < m.bucket_count(); ++i) {
        _buckets_array[i] = std::move(m._buckets_array[i]);
        m._buckets_array[i] = nullptr;
    }
    m._size = 0;
    return *this;
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
    clear();
}

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const {
    return _size;
}

template <typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const {
    return size() == 0;
}

template <typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const {
    return static_cast<float>(size())/bucket_count();
};

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const{
    return _buckets_array.size();
};

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key) {
    auto [prev, node_found] = find_node(key);
    if (node_found == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found");
    }
    return node_found->value.second;
}

template <typename K, typename M, typename H>
const M& HashMap<K, M, H>::at(const K& key) const{
    auto [prev, node_found] = find_node(key);
    if (node_found == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found");
    }
    return node_found->value.second;
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const {
    return find_node(key).second != nullptr;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() {
    for (auto& curr : _buckets_array) {
        while (curr != nullptr) {
            auto will_del = curr;
            curr = curr->next;
            delete will_del;
        }
    }
    _size = 0;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::find(const K& key) {
    return make_iterator(find_node(key).second);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::find(const K& key) const {
    return make_const_iterator(find_node(key).second);
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::iterator, bool> HashMap<K, M, H>::insert(const value_type& value) {
    const auto& [key, mapped] = value;
    auto [prev, node_to_edit] = find_node(key);
    size_t index = _hash_function(key) % bucket_count();

    if (node_to_edit != nullptr) {
        return {make_iterator(node_to_edit), false};
    }
    // 将新建的 node 插入到该桶的第一个位置
    auto temp = new node(value, _buckets_array[index]);
    _buckets_array[index] = temp;

    ++_size;
    return {make_iterator(temp), true};
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
    size_t index = _hash_function(key) % bucket_count();
    node* curr = _buckets_array[index];
    node* prev = nullptr; // if first node is the key, return {nullptr, front}
    while (curr != nullptr) {
        const auto& [found_key, found_mapped] = curr->value;
        if (found_key == key) {
            return {prev, curr};
        }
        prev = curr;
        curr = curr->next;
    }
    return {nullptr, nullptr}; // key not found at all.
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::begin() {
    size_t index = first_not_empty_bucket();
    if (index == bucket_count()) {
        return end();
    }
    return make_iterator(_buckets_array[index]);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::begin() const {
    // This is called the static_cast/const_cast trick, which allows us to reuse
    // the non-const version of find to implement the const version.
    // The idea is to cast this so it's pointing to a non-const HashMap, which
    // calls the overload above (and prevent infinite recursion).
    // Also note that we are calling the conversion operator in the iterator class!
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->begin());
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::end() {
    return make_iterator(nullptr);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::end() const {
    return make_const_iterator(nullptr);
}

template <typename K, typename M, typename H>
size_t HashMap<K, M, H>::first_not_empty_bucket() const {
    auto isNotNullptr = [ ](const auto& v){
        return v != nullptr;
    };

    auto found = std::find_if(_buckets_array.begin(), _buckets_array.end(), isNotNullptr);
    return found - _buckets_array.begin();
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::make_iterator(node* curr) {
    if (curr == nullptr) {
        return {&_buckets_array, curr, bucket_count()};
    }
    size_t index = _hash_function(curr->value.first) % bucket_count();
    return {&_buckets_array, curr, index};
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::make_const_iterator(node* curr) const {
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->make_iterator(curr));
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {
        return false;
    }
    size_t index = _hash_function(key) % bucket_count();
    (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
    --_size;
    return true;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::erase(typename HashMap<K, M, H>::const_iterator pos) {
    erase(pos++->first);
    return make_iterator(pos._node); // unfortunately we need a regular iterator, not a const_iterator
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::debug() const {
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
              << "Printing debug information for your HashMap implementation\n"
              << "Size: " << size() << std::setw(15) << std::right
              << "Buckets: " << bucket_count() << std::setw(20) << std::right
              << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

    for (size_t i = 0; i < bucket_count(); ++i) {
        std::cout << "[" << std::setw(3) << i << "]:";
        node* curr = _buckets_array[i];
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            // next line will not compile if << not supported for K or M
            std::cout <<  " -> " << key << ":" << mapped;
            curr = curr->next;
        }
        std::cout <<  " /" <<  '\n';
    }
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ');
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
    if (new_bucket_count == 0) {
        throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
    }

    std::vector<node*> new_buckets_array(new_bucket_count, nullptr);
    for (auto& curr : _buckets_array) { // short answer question is asking about this 'curr'
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            size_t index = _hash_function(key) % new_bucket_count;
            // curr 存储的是当前桶的第一个元素的地址
            auto temp = curr;                     // temp 为将要更换桶位置的节点地址
            curr = temp->next;                    // curr 为 temp 的下一个节点地址
            temp->next = new_buckets_array[index];// temp 节点的下一个节点是新桶的第一个节点的地址
            new_buckets_array[index] = temp;      // 将 temp 做为新桶的第一个节点
        }
    }
    _buckets_array = std::move(new_buckets_array);
}

template <typename K, typename M, typename H>
template <typename InputIt>
HashMap<K, M, H>::HashMap(InputIt first, InputIt last, size_t bucket_count, const H& hash) :
        HashMap(bucket_count, hash) {
    for (auto iter = first; iter != last; ++iter) {
        insert(*iter);
    }
}

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(std::initializer_list<value_type> init, size_t bucket_count, const H& hash) :
        HashMap{init.begin(), init.end(), bucket_count, hash} {
}

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::operator[](const K& key) {
    return insert({key, {}}).first->second;
}

template <typename K, typename M, typename H>
bool operator==(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {
    return lhs.size() == rhs.size() && std::is_permutation(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename K, typename M, typename H>
bool operator!=(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {
    return !(lhs == rhs);
}

template <typename K, typename M, typename H>
std::ostream& operator<<(std::ostream& os, const HashMap<K, M, H>& rhs) {
    std::ostringstream oss("{", std::ostringstream::ate);
    for (const auto& [key, value] : rhs) {
        oss << key << ":" << value << ", ";
    }
    std::string s = oss.str();
    os << s.substr(0, s.length()-2) << "}";
    return os;
}

/* Begin Milestone 2: Special Member Functions */

/* end student code */


#endif // HASHMAP_H


# Simple Hash Table

This respository contains a simple implementation of a hash table with a nested binary search tree for improved key lookup. The implementation provides classical methods such as:

* Insertion
* Lookup
* Deletion

Operating on the hash table

and further

* exits

Operating on the nested binary search tree in the hash table.

In summary the implementation provides options for directly looking up a key by indexing into the hash table using hash(key) and by looking up keys that are stored in a binary search tree in order of their hashed key value.

This repository contains C code written by a hobbyist self-taught programmer key concepts from the books System Design Interview volume one and two.

You '''will''' not find full fledged flawless implementations but instead working proof of concepts utilizing features such as mutexes, multithreading, networking, sockets, custom protocols and APIs.

**Thus you will find, as per disclaimer, among other things lack of return checks, different use of returns (int, bools), large source files, repetive code, lack of project structure**

The code is not flawless and does not account for all edge cases.

Finally this project is not an example of how to structure C projects. In this repository each of the features/concepts have been implemented in a single source file with a single header and are thus built indepedently.


I hope you find this useful.


## Installation  and usage

The program can be used by using a compiler of your choice or utilizing the makefile

For example:

```bash
make hashtable
```
    
## Feedback

If you have any feedback, please reach out to me.


## Acknowledgements

 - [System Design Book](https://www.google.com/search?hl=da&q=System%20Design%20book)



## Usage/Examples

See the implementation or the below example for usage.

Methods operating on the nested binary search tree follow the pattern of hashtable_bst_method while methods operating on the hashtable follow the pattern of hashtable_method e.g. hashtable_insert

Calling a method on the hashtable will also call the appropiate method on the binary search tree.

```bash
/* 
[**************************************************************************************************************************************************]
                                                            MAIN
[**************************************************************************************************************************************************]
*/

/**
 * @brief Main
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv) {

    hashtable_t *table = hashtable_create(1000, &hashtable_hash);

    /* Insertion */
    hashtable_insert(table, "A", "Value");
    hashtable_insert(table, "E", "Value");
    hashtable_insert(table, "B", "Value");
    hashtable_insert(table, "G", "Value");
    hashtable_insert(table, "J", "Value");



    /* Lookup */
    hashtable_bucket_item *n = hashtable_lookup(table, "A");

    /* Key brute search using a binary search tree */
    bool exists = hashtable_exists(table, "A");

    /* Removal*/
    hashtable_remove(table, "A");
  

    exists = hashtable_exists(table, "A");


}

```


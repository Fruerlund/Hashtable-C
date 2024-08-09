/**
 * @file hashtable.h
 * @author Fruerlund
 * @brief 
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HASTABLE_H
#define HASHTABLE_H

#include "common-defines.h"



/* 
[**************************************************************************************************************************************************]
                                                            STRUCTUES AND PROTOTYPES
[**************************************************************************************************************************************************]
*/

/**
 * @brief Function protoype for hashing methods
 * 
 */
typedef uint32_t (*hashtable_hashmethod)(char *key);



/**
 * @brief Describes the linked list entry.
 * 
 */
typedef struct hashtable_bucket_item {

    char *key;
    char *value;
    LIST_ENTRY(hashtable_bucket_item) entries;

} hashtable_bucket_item;



/**
 * @brief Describes the bucket in a hash table. Each bucket holds a linked list.
 * 
 */
typedef struct hashtable_bucket_t {

    LIST_HEAD(hashtable_list, hashtable_bucket_item) list;

} hashtable_bucket_t;


/**
 * @brief
 */
typedef struct hashtable_bst_node_t {

    uint32_t hash;
    void *key;
    void *value;
    uint8_t type;
    struct hashtable_bst_node_t *left;
    struct hashtable_bst_node_t *right;
    struct hashtable_bst_node_t *parent;

} hashtable_bst_node_t;


/**
 * @brief
 */
typedef struct hashtable_bst_t {

    uint32_t depth;
    uint32_t size;
    struct hashtable_bst_node_t *root;

} hashtable_bst_t;


/**
 * @brief Describes the hash table. The hash table holds buckets with linked lists.
 * 
 * 
 * Our table has the following layout utilizing linked lists for handling collisions
 * 
 * TABLE:
 * 
 *      BUCKET -> LINKED LIST -> ENTRY 1, ENTRY 2
 *      BUCKET -> LINKED LIST -> ENTRY 1, ENTRY 2
 *      BUCKET -> LINKED LIST -> ENTRY 1, ENTRY 2
 *      BUCKET -> LINKED LIST -> ENTRY 1, ENTRY 2
 */

typedef struct hashtable_t {

    uint32_t size;                          /*      Holds the size of the hash table                */
    uint32_t count;                         /*      Holds the number of current elements            */
    hashtable_bucket_t **buckets;           /*      Array of hashtable buckets                      */
    hashtable_bst_t *bst;                   /*      Binary search tree representation of buckets    */
    hashtable_hashmethod hashmethod;        /*      Pointer to the method responsible for hashing   */

} hashtable_t;



typedef enum remove_methods_t {
    INORDER,
    PREOREDER
} remove_methods_t;

remove_methods_t remove_method = INORDER;

/* 
[**************************************************************************************************************************************************]
                                                            METHODS / FUNCTIONS
[**************************************************************************************************************************************************]
*/

uint32_t hashtable_hash(char *key);
hashtable_bst_node_t * hashtable_bst_search(hashtable_t *table, char *k);
int hashtable_bst_insert(hashtable_bst_t *tree, char *key, uint32_t value, void *data);
void hashtable_bst_destroy(hashtable_bst_t *tree);
hashtable_bst_node_t * hashtable_bst_create_node(char *key, uint32_t value, void *data, uint8_t type);
hashtable_bst_t * hashtable_bst_create(void);
bool hashtable_bst_empty(hashtable_bst_t *tree);
bool hashtable_bst_remove(hashtable_t *table, char *key);
void hashtable_bst_destroy_node(hashtable_bst_node_t *n);

#endif
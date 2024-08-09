/**
 * @file hashtable.c
 * @author Fruerlund
 * @brief 
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 * 
 * 
*/


#include "./include/hashtable.h"

/* 
[**************************************************************************************************************************************************]
                                                            GLOBAL VARIABLES AND DEFINES
[**************************************************************************************************************************************************]
*/



/* 
[**************************************************************************************************************************************************]
                                                            BINARY SEARCH TREE
[**************************************************************************************************************************************************]
*/

/**
 * @brief Searches the binary search tree by iteratively walking the tree.
 * 
 * @param table 
 * @param k 
 * @return hashtable_bst_node_t* 
 */
hashtable_bst_node_t * hashtable_bst_search(hashtable_t *table, char *k) {

    hashtable_bst_t *tree = table->bst;

    if( hashtable_bst_empty(tree) ) {
        return NULL;
    }

    uint32_t hash = table->hashmethod(k);

    hashtable_bst_node_t *n = tree->root;

    while(true) {

        if(n->hash == hash) {
            /* Found */
            return n;
        }

        if( hash > n->hash && n->right != NULL ) {
            n = n->right;
            continue;
        }

        if ( hash < n->hash && n->left != NULL ) {
            n = n->left;
            continue;
        }

        /* Not found */
        break;
    }

    return NULL;
}


/**
 * @brief Inserts a new node into the binary search tree by iteratively walking the tree.
 * 
 * @param tree 
 * @param key 
 * @param hash 
 * @param data 
 * @return int 
 */
int hashtable_bst_insert(hashtable_bst_t *tree, char *key, uint32_t hash, void *data) {

    hashtable_bst_node_t *n = hashtable_bst_create_node(key, hash, data, 1);
    if(n == NULL) {
        return EXIT_FAILURE;
    }

    /* If tree is empty, we are the root node. */
    if( hashtable_bst_empty(tree) ) {
        tree->root = n;
        tree->size++;
        return EXIT_SUCCESS;
    }

    /* If tree is not empty, start at root and walk */
    else {
        hashtable_bst_node_t *current = tree->root;
        while(true) {
            
            /* Walking right */
            if( n->key > current->key) {
                if(current->right != NULL) {
                    current = current->right;
                    continue;
                }
                else {
                    current->right = n;
                    n->parent = current;
                    break;
                }
            }

            /* Walking left */
            else {
                if(current->left != NULL) {
                    current = current->left;
                    continue;
                }
                else {
                    current->left = n;
                    n->parent = current;
                    break;
                }
            }
            
            break; // Should never hit
        }
    }

    tree->size++;
}




/**
 * @brief Finds the biggest node in a tree.
 * 
 * @param node 
 * @return hashtable_bst_node_t* 
 */
hashtable_bst_node_t * hashtable_bst_findmax(hashtable_bst_node_t *node) {

    hashtable_bst_node_t *current = node;

    while(true) {

        if( current->right != NULL ) {
            current = current->right;
            continue;
        }
        else {
            break;
        }
    }

    return current;
}



/**
 * @brief Finds the smallest node in a tree.
 * 
 * @param node 
 * @return hashtable_bst_node_t* 
 */
hashtable_bst_node_t * hashtable_bst_findmin(hashtable_bst_node_t *node) {
    
    hashtable_bst_node_t *current = node;

    while(true) {
        if( current->left != NULL ) {
            current = current->left;
            continue;
        }
        else {
            break;
        }
    }

    return current;
}


/**
 * @brief Deletes a node from the binary search tree
 * 
 * TO DO: Optimize.
 * 
 * @param table 
 * @param key 
 * @return true 
 * @return false 
 */
bool hashtable_bst_remove(hashtable_t *table, char *key) {

    if( hashtable_bst_empty(table->bst) ) {
        return EXIT_FAILURE;
    }

    uint32_t hash = table->hashmethod(key);
    hashtable_bst_node_t *n = hashtable_bst_search(table, key);

    /**
     * Case 1: Node has no children.
    */
    if( n->left == NULL && n->right == NULL) {
        if(n->parent != NULL) {
            if( n->parent->left == n) {
                n->parent->left = NULL;
            }
            
            if(n->parent->right == n) {
                n->parent->right = NULL;
            }
        }
        else {
            table->bst->root = NULL;
        }

    }
    
    /*
     * Case 2: Node has a two children.
        Inorder successor uses the minimum node value of the right subtree, which becomes the root node.
        Preorder successor uses the maximum node value of the left subtree, which becomes the root node.
     */
    else if(n->left != NULL && n->right != NULL) {


       switch(remove_method) {

        // TO DO: Redo.
        
        case INORDER:

            /* Find minimum node of the right subtree */
            hashtable_bst_node_t *sucessor = hashtable_bst_findmin(n->right);
            sucessor->parent->left = NULL;
            sucessor->parent = NULL;
            
            /* If our node has a parent, we need to update that one*/
            if(n->parent != NULL) {

                sucessor->parent = n->parent;

                if(n->left != NULL) {
                    sucessor->left = n->left;
                    n->left->parent = sucessor;
                }
                if(n->right != NULL) {
                    sucessor->right = n->right;
                    n->right->parent = sucessor;
                }

            }
            else {

                /* If node doesn't have a parent we are deleting the top root*/
                sucessor->right = table->bst->root->right;
                sucessor->right->parent = sucessor;

                if(table->bst->root->left != NULL) {
                    sucessor->left = table->bst->root->left;
                    table->bst->root->left->parent = sucessor;
                }

                table->bst->root = sucessor;

            }


            break;


        case PREOREDER:

            /* Find maximum node of the left subtree */
            hashtable_bst_node_t *leftmax = hashtable_bst_findmax(n->left);
            leftmax->parent->right = NULL;
            leftmax->parent = NULL;

            /* If our node has a parent, we need to update that one*/
            if(n->parent != NULL) {

                leftmax->parent = n->parent; /* 70 Parent is 100*/

                if(n->left != NULL) {
                    leftmax->left = n->left;
                    n->left->parent = leftmax;
                }
                if(n->right != NULL) {
                    leftmax->right = n->right;
                    n->right->parent = leftmax;
                }

                n->parent->left = leftmax;
            }
            else {

                /* If node doesn't have a parent we are deleting the top root*/
                leftmax->left = table->bst->root->left;
                leftmax->left->parent = leftmax;

                if(table->bst->root->right != NULL) {
                    leftmax->right = table->bst->root->right;
                    table->bst->root->right->parent = leftmax;
                }

                table->bst->root = leftmax;

            }

            break;
        
        default:
            break;
       }

    }
    /*
    * Case 3: Node has a single child. 
    */
    else {
        hashtable_bst_node_t *child;
        if(n->left != NULL) {
            child = n->left;
        }
        else {
            child = n->right;
        }
        
        if( n->parent != NULL) {
            if( n->parent->left == n) {
                n->parent->left = child;
            }
            
            if(n->parent->right == n) {
                n->parent->right = child;
            }
        }
        else {
            table->bst->root = child;
        }
    }

    hashtable_bst_destroy_node(n);
    table->bst->size--;
    return EXIT_SUCCESS;
}


/**
 * @brief Creates a binary search tree node to be stored as a leaf.
 * 
 * @param key 
 * @param hash 
 * @param data 
 * @param type 
 * @return hashtable_bst_node_t* 
 */
hashtable_bst_node_t * hashtable_bst_create_node(char *key, uint32_t hash, void *data, uint8_t type) {

    hashtable_bst_node_t *n = (hashtable_bst_node_t *)malloc(sizeof(hashtable_bst_node_t));
    if(n == NULL) {
        perror("Failed to allocate memory for bst node\n");
        return NULL;
    }

    n->key = key;
    n->hash = hash;
    n->value = data;
    n->type = type;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;

    return n;
}


/**
 * @brief Destroys a binary search tree node
 * 
 * @param n 
 */
void hashtable_bst_destroy_node(hashtable_bst_node_t *n) {

    n->parent = NULL;
    n->left = NULL;
    n->right = NULL;
    free(n);

}


/**
 * @brief Destroys a binary search tree by iteratively deleting all nodes
 * 
 * @param tree 
 */
void hashtable_bst_destroy(hashtable_bst_t *tree) {

    /* TO DO */

}




/**
 * @brief Creates a binary search tree to be nested in the hashtable
 * 
 * @return hashtable_bst_t* 
 */
hashtable_bst_t * hashtable_bst_create(void) {

    hashtable_bst_t *t = (hashtable_bst_t *)malloc(sizeof(hashtable_bst_t));
    if(t == NULL) {
        perror("Failed to allocate memory for bst\n");
        return NULL;
    }

    t->depth = 0;
    t->root = NULL;
    t->size = 0;

    return t;
}


/**
 * @brief Returns true if the binary search tree is empty, otherwise false.
 * 
 * @param tree 
 * @return true 
 * @return false 
 */
bool hashtable_bst_empty(hashtable_bst_t *tree) {

    if(tree->size == 0) {
        return true;
    }

    return false;
}


/* 
[**************************************************************************************************************************************************]
                                                            HASH TABLE
[**************************************************************************************************************************************************]
*/
/**
 * @brief Creates a hash bucket initializing the linked list in the bucket.
 * 
 * @return hashtable_bucket_t* 
 */
hashtable_bucket_t *hashbucket_create() {

    hashtable_bucket_t *bucket = (hashtable_bucket_t *) malloc (sizeof(hashtable_bucket_t ));
    
    if(bucket == NULL) {
        // printf("[ERROR]: Failed to create bucket in hash table\n");
    }

    LIST_INIT(&bucket->list);
    return bucket;

}



/**
 * @brief Deletes the contents of the linked list in the hash bucket.
 * 
 * @param bucket 
 */
void hashbucket_delete(hashtable_bucket_t *bucket) {

    hashtable_bucket_item *h1, *h2;
    h1 = LIST_FIRST(&bucket->list);
    while( h1 != NULL ) {
        h2 = LIST_NEXT(h2, entries);
        free(h1);
        free(h1->key);
        free(h1->value);
        h1 = h2;
    }
}





/**
 * @brief Creates a hash table by allocating memory for a specified number of buckets.
 * 
 * @param size 
 * @param hashmethod 
 * @return hashtable_t* 
 */
hashtable_t * hashtable_create(uint32_t size, hashtable_hashmethod hashmethod) {

    hashtable_t *table = NULL;

    table = (hashtable_t *) malloc (sizeof(hashtable_t));
    
    /* Failed to allocate new hash table */
    if(table == NULL) {
        exit(EXIT_FAILURE);
    }

    table->size = size;
    table->count = 0;
    table->buckets = calloc((size_t)table->size, sizeof(hashtable_bucket_t *));

    /* Binary Search Tree */
    table->bst = hashtable_bst_create();
    for(uint32_t i = 0; i < size; i++) {
        table->buckets[i] = hashbucket_create();
    }

    table->hashmethod = hashmethod;

    return table;

}



/**
 * @brief Deletes a hash table by freeing allocated buckets and finally the table itself.
 * 
 * @param table 
 */
void hashtable_delete(hashtable_t *table) {

    /* Iterate over all buckets, then over each entry in the nested list */
    for(uint32_t i = 0; i < table->size; i++) {
        if(table->buckets[i] != NULL) {
            hashbucket_delete(table->buckets[i]);
            free(table->buckets[i]);
        }   
    }

    /* Destroy bst and all of its nodes */
    hashtable_bst_destroy(table->bst);

    free(table->bst);
    free(table->buckets);
    free(table);
}



/**
 * @brief Implements a simple DJB2_HASH algorithm for use in this hash table. See resource at: https://stackoverflow.com/questions/7666509/hash-function-for-string
 * 
 * @param key 
 * @return uint32_t 
 */
uint32_t hashtable_hash(char *key) {    

    uint32_t hash = 5381;
    int c;
    while (c = *key++)
        hash = ((hash << 5) + hash) + c; 
    return hash;


}



/**
 * @brief Performs a lookup in the hash table
 * 
 * @param table 
 * @param key 
 * @return hashtable_bucket_item* 
 */
hashtable_bucket_item *hashtable_lookup(hashtable_t *table, char *key) {

    hashtable_bucket_item *result = NULL;

    /* Calculate hash/index  */
    uint32_t hash = table->hashmethod(key);

    /* Get bucket at index */
    uint32_t index = hash % table->size;
    hashtable_bucket_t *bucket = table->buckets[index];

    /* Check if list in bucket is empty or not */
    if(LIST_EMPTY(&bucket->list)) {
        return result;
    }

    /* If list in bucket is not empty, it may have multiple entries due to collisions, thus we iterate over each list entry to find our entry */
    hashtable_bucket_item *n1 = LIST_FIRST(&bucket->list);
    while (n1 != NULL) {
        if( strcmp(key, n1->key) == 0) {
            result = n1;
            return result;
        }
        n1 = LIST_NEXT(n1, entries);
    }
    

    return result;
}



/**
 * @brief Inserts a key, value pair into a bucket placed the hash table. In case of collision the new kvp is stored as a linked list entry.
 * 
 * @param table 
 * @param key 
 * @param value 
 * @return true 
 * @return false 
 */
bool hashtable_insert(hashtable_t *table, char *key, char *value) {

    /* Check if key already exists */
    if(hashtable_lookup(table, key) != NULL) {
        // perror("[ERROR]: Key already exists in hash table\n");
        return false;
    }

    /* TODO: Check if our hash table will overspill, if so perform resizing*/

    /* Calculate hash/index  */
    uint32_t hash = table->hashmethod(key);

    /* Get bucket at index */
    uint32_t index = hash % table->size;
    hashtable_bucket_t *bucket = table->buckets[index];

    /* Insert new item into list in bucket */
    hashtable_bucket_item *n1 = malloc(sizeof(struct hashtable_bucket_item));
    if(n1 == NULL) {
        return false;
    }

    /* DEBUG: Check if for a collision in bucket */
    if(LIST_EMPTY(&bucket->list)) {
        // perror("[DEBUG]: Collision\n");
    }

    n1->key = strdup(key);
    n1->value = strdup(value);
    LIST_INSERT_HEAD(&bucket->list, n1, entries);

    table->count++;

    /* Insert key into binary search tree */
    hashtable_bst_insert(table->bst, key, hash, (void *)value);

    return true;

}


/**
 * @brief Uses the nested binary search tree to iteratively look for a given key.
 * 
 * @param table 
 * @param key 
 * @return true 
 * @return false 
 */
bool hashtable_exists(hashtable_t *table, char *key) {

    hashtable_bst_node_t *n = hashtable_bst_search(table, key);

    if(n != NULL) {
        return true;
    }

    return false;

}


/**
 * @brief Removes a key, value pair from the hash table by direct indexing into array.
 * 
 * @param table 
 * @param key 
 * @return true 
 * @return false 
 */
bool hashtable_remove(hashtable_t *table, char *key) {

     /* Check if key  exists */
    if(hashtable_lookup(table, key) == NULL) {
        // perror("[ERROR]: Key doesn't exists in hash table\n");
        return false;
    }

    /* Calculate hash/index  */
    uint32_t hash = table->hashmethod(key);

    /* Get bucket at index */
    uint32_t index = hash % table->size;
    hashtable_bucket_t *bucket = table->buckets[index];

    /* If list in bucket is not empty we iterate over each list entry */
    hashtable_bucket_item *n1 = LIST_FIRST(&bucket->list);
    while (n1 != NULL) {
        if( strcmp(key, n1->key) == 0) {
            LIST_REMOVE(n1, entries);
            free(n1);
            break;
        }
        n1 = LIST_NEXT(n1, entries);
    }

    /* Remove from BST */
    hashtable_bst_remove(table, key);

    table->count--;
    return true;
    
}




#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL; 

// malloc_fptr = malloc;
// free_fptr   = free;

// (void)(malloc_fptr);
// (void)(free_fptr);


// Creates a new linked_list.
// PRECONDITION: Register malloc() and free() functions via the
//               linked_list_register_malloc() and 
//               linked_list_register_free() functions.
// POSTCONDITION: An empty linked_list has its head point to NULL.
// Returns a new linked_list on success, NULL on failure.
//
struct linked_list * linked_list_create(void)
{
    struct linked_list *ll = malloc_fptr(sizeof(struct linked_list));
    if (!ll) return (struct linked_list*)NULL;
    ll->head = NULL;
    ll->size = 0;
    return ll;
}


// Deletes a linked_list and frees all memory assoicated with it.
// \param ll : Pointer to linked_list to delete
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete(struct linked_list * ll)
{
    if(!ll) return false; 

    struct node *cur = ll->head;
    while(cur)
    {
        struct node* next = cur->next;
        free_fptr(cur);
        cur = next;
    }
    free_fptr(ll);
    return true;
}

// Returns the size of a linked_list.
// \param ll : Pointer to linked_list.
// Returns size on success, SIZE_MAX on failure.
//
// size_t linked_list_size(struct linked_list * ll)
// {
//     if(!ll) return SIZE_MAX;
//     size_t size = 0;
//     struct node *cur = ll->head;
//     while(cur)
//     {
//         size++;
//         cur = cur->next;
//     }
//     return size;
// }


size_t linked_list_size(struct linked_list * ll)
{
    if(!ll) return SIZE_MAX;
    return ll->size;
}

// Inserts an element at the end of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_end(struct linked_list * ll,
                            unsigned int data)
{
    if (!ll) return false;
    struct node* cur = ll->head;
    struct node* node = malloc_fptr(sizeof(struct node));

    if(!node) return false;
    node->data = data;
    node->next = NULL;

    if(!cur) ll->head = node;
    else
    {
        while(cur->next)
            cur=cur->next;
        cur->next = node;
    }
    ll->size++;
    return true;
}

// Inserts an element at the front of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_front(struct linked_list * ll,
                              unsigned int data)
{
    if(!ll) return false;
    struct node *node = malloc_fptr(sizeof(struct node));
    if(!node) return false;
    node->data = data;
    node->next = ll->head;

    ll->head = node;
    ll->size++;
    return true;
}


// Inserts an element at a specified index in the linked_list.
// \param ll    : Pointer to linked_list.
// \param index : Index to insert data at.
// \param data  : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert(struct linked_list * ll,
                        size_t index,
                        unsigned int data)
{
    if(!ll) return false;
    if(index < 0 || index > ll->size) return false;

    if(index == 0) return linked_list_insert_front(ll, data);
    else if(index == ll->size ) return linked_list_insert_end(ll, data);
    else
    {
        struct node *cur = ll->head;
        struct node *prev = NULL;
        size_t cur_idx = 0;
        while(cur && cur_idx < index)
        {
            prev = cur;
            cur = cur->next;
            cur_idx++;
        }
        struct node* new = malloc_fptr(sizeof(struct node));
        if(!new) return false;
        new->data = data;
        new->next = cur;
        prev->next = new;
        ll->size++;
        return true;
    }
    return false;
}


// Finds the first occurrence of data and returns its index.
// \param ll   : Pointer to linked_list.
// \param data : Data to find.
// Returns index of the first index with that data, SIZE_MAX otherwise.
//
size_t linked_list_find(struct linked_list * ll,
                        unsigned int data)
{
    if(!ll) return SIZE_MAX;
    struct node *cur = ll->head;
    size_t idx = 0;
    while(cur)
    {
        if(cur->data == data) return idx;;
        cur= cur->next;
        idx ++;
    }
    return SIZE_MAX;
}


// Removes a node from the linked_list at a specific index.
// \param ll    : Pointer to linked_list.
// \param index : Index to remove node.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_remove(struct linked_list * ll,
                        size_t index)
{
    if(!ll) return false;
    if(index < 0 || index > ll->size) return false;

    struct node* cur = ll->head;
    size_t idx = 0;
    struct node* prev = NULL;
    if(index == 0) ll->head = cur->next;
    else
    {
        while(cur && idx<index)
        {
            prev = cur;
            cur = cur->next;
            idx++;  
        }

        if(!cur) return false;
        prev->next = cur->next;
        cur->next = NULL;
    }
    

    free_fptr(cur);
    ll->size--;
    return true;
}


// Creates an iterator struct at a particular index.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// Returns pointer to an iterator on success, NULL otherwise.
//
struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index)
{
    if(!ll || index > ll->size) return NULL;

    struct node *cur = ll->head;
    size_t idx = 0;
    while(cur && idx<index)
    {
        cur = cur->next;
        idx++;
    }
    if(!cur) return NULL;

    struct iterator *it = malloc_fptr(sizeof(struct iterator));
    if(!it) return NULL;
    it->ll = ll;
    it->current_index = idx;
    it->current_node = cur;
    it->data = cur->data;
    return it;
}

// Deletes an iterator struct.
// \param iterator : Iterator to delete.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete_iterator(struct iterator * iter)
{
    if(!iter) return false;
    free_fptr(iter);
    return true;
}

// Iterates to the next node in the linked_list.
// \param iterator: Iterator to iterate on.
// Returns TRUE when next node is present, FALSE once end of list is reached.
//
bool linked_list_iterate(struct iterator * iter)
{
    if(!iter) return false;
    struct iterator *it = iter;
    if(!it->current_node) return false;
    
    it->current_node = it->current_node->next;
    if(!it->current_node) return false;
   
    it->current_index++;
    it->data = it->current_node->data;
    return true;
    
}

// Registers malloc() function.
// \param malloc : Function pointer to malloc()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_malloc(void * (*malloc)(size_t))
{
    if(!malloc) return false;
    malloc_fptr = malloc;
    return true;
}

// Registers free() function.
// \param free : Function pointer to free()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_free(void (*free)(void*))
{
    if(!free) return false;
    free_fptr = free;
    return true;
}

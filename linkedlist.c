#include "linkedlist.h"
#include <stdlib.h>

linkedlist_t* listCreate() {
    linkedlist_t* result;
    result = (linkedlist_t*) malloc(sizeof(linkedlist_t));
    if (result == NULL) return NULL;
    result->current = result->first = result->last = NULL;
    result->length = 0;
    return result;
}

listnode_t* listInsert(linkedlist_t* l, void* v) {
    return listInsertAt(l, l->length, v);
}

void listDestroy(linkedlist_t* l) {
    listnode_t* current;
    for (current = l->first; current != NULL; current = current->next) {
        free(current->previous);
    }
    free(l);
}

listnode_t* listCreateNode(void* v) {
    listnode_t* result;
    result = (listnode_t*) malloc(sizeof(listnode_t));
    if (result == NULL) return NULL;
    result->next = result->previous = NULL;
    result->value = v;
    return result;
}

void listDestroyNode(listnode_t* n) {
    if (n == NULL) return;
    if (n->previous != NULL) {
        n->previous->next = n->next;
    }
    if (n->next != NULL) {
        n->next->previous = n->previous;
    }
    free(n);
}

void* listNext(linkedlist_t* l) {
    void* result;
    if (l->current == NULL) return NULL;
    result = l->current->value;
    l->current = l->current->next;
    return result;
}

void* listNextCyclic(linkedlist_t* l) {
    void* result;
    if (l->current == NULL) return NULL;
    result = l->current->value;
    l->current = (l->current->next == NULL) ? l->first : l->current->next;
    return result;
}

void* listPreviousCyclic(linkedlist_t* l) {
    void* result;
    if (l->current == NULL) return NULL;
    result = l->current->value;
    l->current = (l->current->previous == NULL) ? l->last : l->current->previous;
    return result;
}

void* listPrevious(linkedlist_t* l) {
    void* result;
    if (l->current == NULL) return NULL;
    result = l->current->value;
    l->current = l->current->previous;
    return result;
}

void listReset(linkedlist_t* l) {
    listResetToFirst(l);
}

void listResetToFirst(linkedlist_t* l) {
    l->current = l->first;
}

void listResetToLast(linkedlist_t* l) {
    l->current = l->last;
}

void listTraverse(linkedlist_t* l, void (*action)(void*)) {
    void* elem;
    listReset(l);
    for (elem = listNext(l); elem != NULL; elem = listNext(l)) {
        action(elem);
    }
    listReset(l);
}

void* listContains(linkedlist_t* l, void* value, int(*equality)(void*, void*)) {
    void* elem;
    listnode_t* previousCurrent;
    previousCurrent = l->current; // save current traversal pointer
    listReset(l);
    for (elem = listNext(l); elem != NULL; elem = listNext(l)) {
        if (equality(elem, value)) {
            break;
        }
    }
    l->current = previousCurrent; // recover internal traversal pointer
    return elem; // either NULL or breaked with a value
}

void listSwapNodes(listnode_t* n1, listnode_t* n2) {
    void* t;
    if ((n1 == NULL) || (n2 == NULL)) return;
    t = n1->value;
    n1->value = n2->value;
    n2->value = t;
}

void listSort(linkedlist_t* l, int(*comparator)(void*, void*)) {
    listnode_t *c, *limit;
    int swapped = 1;
    if ((l == NULL) || (l->length <= 1)) return;
    for (limit = l->last; ((limit != NULL) && (swapped)); limit = limit->previous) {
        swapped = 0;
        for (c = l->first; c != limit; c = c->next) {
            if (comparator(c->value, c->next->value) == 1) {
                listSwapNodes(c, c->next);
                swapped = 1;
            }
        }
    }
}

listnode_t* listGetNode(linkedlist_t* l, int i) {
    listnode_t* c;
    int counter = 0;
    if (l == NULL) return NULL;
    if ((i < 0) || (i >= l->length)) return NULL;
    for (c = l->first, counter = 0; c != NULL; c = c->next, counter++) {
        if (counter == i) {
            return c;
        }
    }
    return NULL;
}

void* listRemoveAt(linkedlist_t* l, int i) {
    void* result;
    listnode_t* node;
    if ((node = listGetNode(l, i)) == NULL) return NULL;
    result = node->value;
    if (node == l->first) {
        l->first = node->next;
    }
    if (node == l->last) {
        l->last = node->previous;
    }
    listDestroyNode(node);
    l->length--;
    return result;
}

listnode_t* listInsertAt(linkedlist_t* l, int i, void* v) {
    listnode_t *node, *previousNode;
    if (l == NULL) return NULL;
    if ((i < 0)||(i > l->length)) return NULL;
    node = listCreateNode(v);
    if (node == NULL) return NULL;
    if (l->length == 0) {
        l->first = l->last = node;
        l->length++;
        l->current = node;
        return node;
    }
    if (i == 0) {
        l->first->previous = node;
        node->next = l->first;
        l->first = node;
        l->length++;
        return node;
    }
    if (i == l->length) {
        node->previous = l->last;
        l->last->next = node;
        l->last = node;
        l->length++;
        return node;
    }
    previousNode = listGetNode(l,i);
    node->next = previousNode;
    if (previousNode->previous != NULL) previousNode->previous->next = node;
    node->previous = previousNode->previous;
    previousNode->previous = node;
    l->length++;
    return node;
}

void* listRemove(linkedlist_t* l) {
    return listRemoveAt(l,0);
}

void* listGet(linkedlist_t* l, int i) {
    listnode_t* n = listGetNode(l,i);
    if (n == NULL) return NULL;
    return n->value;
}

void listSet(linkedlist_t* l, int i, void* v) {
    listnode_t* n = listGetNode(l,i);
    if (n == NULL) return;
    n->value = v;
}

linkedlist_t* listCreateAllSearchResult(linkedlist_t* l, void* key, int matches(void*,void*)) {
    linkedlist_t* results = NULL;
    void * currentValue = NULL;
    if (l == NULL)  return NULL;
    if (key == NULL) return NULL;
    if (matches == NULL) return NULL;
    results = listCreate();
    if (results == NULL) {
//        fprintf(stderr,"ERROR: out of memory\n");
        return NULL;
    }
    listReset(l);
    while( (currentValue=listNext(l)) != NULL ) {
        if (matches(key,currentValue) != 0) {
            listInsert(results,currentValue);
        }
    }
    return results;
}

void listRemoveItem(linkedlist_t* l, void* value, int(*equality)(void*,void*)) {
    void* elem;
    listnode_t* previousCurrent;
    int index = 0;
    if (l == NULL) return;
    if (value == NULL) return;
    if (equality == NULL) return;
    previousCurrent = l->current; // save current traversal pointer
    listReset(l);
    index = 0;
    for (elem = listNext(l); elem != NULL; elem = listNext(l)) {
        if (equality(elem, value)) {
            listRemoveAt(l,index);
            break;
        }
        index++;
    }
    l->current = previousCurrent; // recover internal traversal pointer
    return; // either NULL or breaked with a value
}
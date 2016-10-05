#ifndef _LINKEDLIST_H
#define	_LINKEDLIST_H

typedef struct _listnode_t {
    void* value;
    struct _listnode_t* next;
    struct _listnode_t* previous;
} listnode_t;

typedef struct {
    listnode_t* first;
    listnode_t* last;
    listnode_t* current;
    long length;
} linkedlist_t;

linkedlist_t* listCreate();
listnode_t* listInsert(linkedlist_t* l, void* v);
void listDestroy(linkedlist_t* l);
void* listNext(linkedlist_t* l);
void* listPrevious(linkedlist_t* l);
void listReset(linkedlist_t* l);
listnode_t* listCreateNode(void* v);
void listDestroyNode(listnode_t* n);
void listTraverse(linkedlist_t* l, void (*action)(void*));
void listPrint(linkedlist_t* l);
void* listContains(linkedlist_t* l, void* value, int(*equality)(void*,void*));
void* listPrevious(linkedlist_t* l);
void listResetToFirst(linkedlist_t* l);
void listResetToLast(linkedlist_t* l);
void* listNextCyclic(linkedlist_t* l);
void* listPreviousCyclic(linkedlist_t* l);
void listSwapNodes(listnode_t* n1, listnode_t* n2);
void listSort(linkedlist_t* l, int(*comparator)(void*,void*));
listnode_t* listGetNode(linkedlist_t* l, int i);
void* listRemoveAt(linkedlist_t* l,int i);
listnode_t* listInsertAt(linkedlist_t* l, int i, void* v);
void* listRemove(linkedlist_t* l);
void* listGet(linkedlist_t* l, int i);
void listSet(linkedlist_t* l, int i, void* v);
linkedlist_t* listCreateAllSearchResult(linkedlist_t* l, void* key, int matches(void*,void*));
void listRemoveItem(linkedlist_t* l, void* value, int(*equality)(void*,void*));


#endif	/* _LINKEDLIST_H */


// Universal Double Ended Linked List Header file

#include <stdint.h>

#define UDELL_PASS 1          // don't use this to test for success
#define UDELL_FAIL 0          // use this to test for failure

#define UDELL_NOLIMIT -1      // used for max links
#define UDELL_NOLIST 0        // used to mark an unowned link

#define UDELL_ERROR_BASE      1000
#define UDELL_ENOERROR        UDELL_ERROR_BASE + 0
#define UDELL_ESIZE           UDELL_ERROR_BASE + 1
#define UDELL_EHEAD           UDELL_ERROR_BASE + 2
#define UDELL_ETAIL           UDELL_ERROR_BASE + 3
#define UDELL_EEMPTY          UDELL_ERROR_BASE + 4
#define UDELL_EINVPTR         UDELL_ERROR_BASE + 5
#define UDELL_ELISTFULL       UDELL_ERROR_BASE + 6
#define UDELL_EINVINPUT       UDELL_ERROR_BASE + 7
#define UDELL_EBADLIST        UDELL_ERROR_BASE + 8

struct udellLink;
struct udellList;

typedef struct udellLink
{
  struct udellLink *pPrev;    // prev link
  struct udellLink *pNext;    // next link
  struct udellList *pList;    // a pointer to the list that contains this link
  uint32_t payloadSize;       // max amount of data that can be held
  uint32_t dataSize;          // actual size of data held (optional)
  void     *pData;            // pointer to actual data
}udellLink;

typedef struct udellList
{
  struct udellLink *pHead;     // address of first link (0 if empty)
  struct udellLink *pTail;     // address of final link (0 if empty)
  int32_t           maxLen;    // the maximuim number of links allowed
  uint32_t          len;       // number of links
}udellList;

// Initializes a link
int LinkInit(udellLink *link, void *data, uint32_t payloadSize, uint32_t dataSize);

// Takes a ptr to a link and returns a pointer to the data
void* LinkGetData(udellLink *link);

// Takes a ptr to a link and returns the list it belongs to
udellList* LinkGetList(udellLink *link);

// Same as LinkGetData(), but interprets the data as an int and returns it - for debugging
uint32_t LinkGetDataInt32(udellLink *link);

// Takes a ptr to a link and returns the payload size
uint32_t LinkGetPayloadSize(udellLink *link);

// Takes a ptr to a link and returns the data size
uint32_t LinkGetDataSize(udellLink *link);

// Takes a link pointer and returns a pointer to the next link
udellLink* LinkGetNext(udellLink *link);

// Takes a link pointer and returns a pointer to the prev link
udellLink* LinkGetPrev(udellLink *link);

// Initializes a list
int ListInit(udellList *list);

// Set the max links available to a list
int ListSetMaxLinks(udellList *list, int32_t max);

// Get the max links available to a list
int ListGetMaxLinks(udellList *list);

// Takes a ptr to a list and a link, and adds the link to the list's begining
int ListInsertFirst(udellList *list, udellLink *link);

// Takes a ptr to a list and a link, and adds the link to the list's end
int ListInsertLast(udellList *list, udellLink *link);

// Takes a ptr to a list and 2 links, the second link is added to the list after the first link
int ListInsertAfter(udellList *list, udellLink *reflink, udellLink *addlink);

// Takes a ptr to a list and 2 links, the second link is added to the list before the first link
int ListInsertBefore(udellList *list, udellLink *reflink, udellLink *addlink);

// Takes a ptr to a list and a link, and removes the link
int ListRemoveLink(udellList *list, udellLink *link);

// Takes a pointer to a list, and returns a link pointer to the head of the list
udellLink* ListGetHead(udellList *list);

// Takes a pointer to a list, and returns a link pointer to the tail of the list
udellLink* ListGetTail(udellList *list);

// Takes a pointer to a list and returns the number of links
int ListGetLen(udellList *list);


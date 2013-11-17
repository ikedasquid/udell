// Universal Double Ended Linked List Implementation File

#include "udell.h"
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>

// Initializes a link
int LinkInit(udellLink *link, void *data, uint32_t payloadSize, uint32_t dataSize)
{
  // the pointer needs to point to a valid link
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  if (dataSize > payloadSize)
  {
    errno = UDELL_ESIZE;
    return UDELL_FAIL;
  }
  
  link->pPrev = 0;
  link->pNext = 0;
  link->pData = data;
  link->payloadSize = payloadSize;
  link->dataSize = dataSize;
  link->pList = UDELL_NOLIST;
  
  return UDELL_PASS;
}

// Initializes a list
int ListInit(udellList *list)
{
  // the pointer needs to point to a valid list
  if (list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  list->pHead = 0;
  list->pTail = 0;
  list->len = 0;
  list->maxLen = UDELL_NOLIMIT;
  
  return UDELL_PASS;
}

int ListSetMaxLinks(udellList *list, int32_t max)
{
  // the pointer needs to point to a valid list
  if (list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // UDELL_NOLIMIT is -1, a max below this makes no sense
  if (max < UDELL_NOLIMIT)
  {
    errno = UDELL_EINVINPUT;
    return UDELL_FAIL;
  }
  
  // fail if the list is currently larger than the specified limit
  if (list->len >= max)
  {
    errno = UDELL_ESIZE;
    return UDELL_FAIL;
  }
  
  list->maxLen = max;
  return UDELL_PASS;
}

int ListGetMaxLinks(udellList *list)
{
  // the pointer needs to point to a valid list
  if (list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  return list->maxLen;
}

// Takes a ptr to a link and returns the list it belongs to
udellList* LinkGetList(udellLink *link)
{
  // the pointer needs to point to a valid link
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // if the link does not belong to a list, return fail
  if (link->pList == UDELL_NOLIST)
  {
    errno = UDELL_EEMPTY;
    return UDELL_FAIL;
  }
  
  return link->pList;
}
  
// Takes a ptr to a list and a link, and adds the link to the list's begining
int ListInsertFirst(udellList *list, udellLink *link)
{
  // both pointers need to point to valid data
  if (link == UDELL_FAIL || list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // if the list is full, fail
  if ( (list->maxLen != UDELL_NOLIMIT) && (list->len + 1 > list->maxLen) )
  {
    errno = UDELL_ELISTFULL;
    return UDELL_FAIL;
  }
  
  // check empty list condition (special case)
  if (list->len == 0)
  {
    list->pHead = link;
    list->pTail = link;
    list->len = 1;
    link->pPrev = 0;
    link->pNext = 0;
  }
  
  // otherwise, move the first link down and insert
  else
  {
    list->pHead->pPrev = link;
    link->pNext = list->pHead;
    list->pHead = link;
    link->pPrev = 0;
    list->len += 1;
  }
  
  // assign this link to the list
  link->pList = list;
  
  return UDELL_PASS;
}

// Takes a ptr to a list and a link, and adds the link to the list's end
int ListInsertLast(udellList *list, udellLink *link)
{
  // both pointers need to point to valid data
  if (link == UDELL_FAIL || list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // if the list is full, fail
  if ( (list->maxLen != UDELL_NOLIMIT) && (list->len + 1 > list->maxLen) )
  {
    errno = UDELL_ELISTFULL;
    return UDELL_FAIL;
  }
  
  // check empty list condition first (special case)
  if (list->len == 0)
  {
    list->pHead = link;
    list->pTail = link;
    list->len = 1;
    link->pPrev = 0;
    link->pNext = 0;
  }
  
  // otherwise, insert after the last link
  else
  {
    list->pTail->pNext = link;
    link->pPrev = list->pTail;
    list->pTail = link;
    link->pNext = 0;
    list->len += 1;
  }
  
  // assign this link to the list
  link->pList = list;
  
  return UDELL_PASS;
}

// Takes a ptr to a list and 2 links, the second link is added to the list after the first link
int ListInsertAfter(udellList *list, udellLink *reflink, udellLink *addlink)
{
  // all pointers need to point to valid data
  if (reflink == UDELL_FAIL || addlink == UDELL_FAIL || list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // this call makes no sense if the list is empty
  if (list->len == 0)
  {
    errno = UDELL_EEMPTY;
    return UDELL_FAIL;
  }
  
  // if the reflink isn't part of this list, fail
  if (reflink->pList != list)
  {
    errno = UDELL_EBADLIST;
    return UDELL_FAIL;
  }
  
  // if the list is full, fail
  if ( (list->maxLen != UDELL_NOLIMIT) && (list->len + 1 > list->maxLen) )
  {
    errno = UDELL_ELISTFULL;
    return UDELL_FAIL;
  }
  
  // otherwise, insert after the link
  else
  {
    // connect the "next" chain (this automatically handles the case where reflink was the tail)
    addlink->pNext = reflink->pNext;
    reflink->pNext = addlink;
    
    // connect the "prev" chain
    addlink->pPrev = reflink;
    
    // only update prev to the addlink if it's not the tail
    if (addlink->pNext != 0)
    {
      addlink->pNext->pPrev = addlink;
    }
    // if it is the tail, then update the list instead
    else
    {
      list->pTail = addlink;
    }
    
    list->len += 1;
  }
  
  // assign this link to the list
  addlink->pList = list;
  
  return UDELL_PASS;
}

// Takes a ptr to a list and 2 links, the second link is added to the list before the first link
int ListInsertBefore(udellList *list, udellLink *reflink, udellLink *addlink)
{
  // all pointers need to point to valid data
  if (reflink == UDELL_FAIL || addlink == UDELL_FAIL || list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // this call makes no sense if the list is empty
  if (list->len == 0)
  {
    errno = UDELL_EEMPTY;
    return UDELL_FAIL;
  }
  
  // if the reflink isn't part of this list, fail
  if (reflink->pList != list)
  {
    errno = UDELL_EBADLIST;
    return UDELL_FAIL;
  }
  
  // if the list is full, fail
  if ( (list->maxLen != UDELL_NOLIMIT) && (list->len + 1 > list->maxLen) )
  {
    errno = UDELL_ELISTFULL;
    return UDELL_FAIL;
  }
  
  // otherwise, insert after the link
  else
  {
    // connect the "prev" chain (this automatically handles the case where reflink was the head)
    addlink->pPrev = reflink->pPrev;
    reflink->pPrev = addlink;
    
    // connect the "next" chain
    addlink->pNext = reflink;
    
    // only update next to the addlink if it's not the head
    if (addlink->pPrev != 0)
    {
      addlink->pPrev->pNext = addlink;
    }
    // if it is the head, then update the list instead
    else
    {
      list->pHead = addlink;
    }
    
    list->len += 1;
  }
  
  // assign this link to the list
  addlink->pList = list;
  
  return UDELL_PASS;
}

// Takes a ptr to a list and a link, and removes the link
int ListRemoveLink(udellList *list, udellLink *link)
{
  // both pointers need to point to valid data
  if (link == UDELL_FAIL || list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // this call makes no sense if the list is empty
  if (list->len == 0)
  {
    errno = UDELL_EEMPTY;
    return UDELL_FAIL;
  }
  
  // don't alter the link if it doesn't belong to this list
  if (link->pList != list)
  {
    errno = UDELL_EBADLIST;
    return UDELL_FAIL;
  }
  
  // if the list has only this link, handle as a special case
  if (list->len == 1)
  {
    list->len = 0;
    list->pHead = 0;
    list->pTail = 0;
    // note: link is still in the init state - no need to re-init
    return UDELL_PASS;
  }
  
  // otherwise, delete!
  else
  {
    // if not the head, then connect the next chain
    if (link->pPrev != 0)
    {
      link->pPrev->pNext = link->pNext;
    }
    // if this link is the head, update the list's head
    // and mark the next link as such
    else
    {
      list->pHead = link->pNext;
      link->pNext->pPrev = 0;
    }
    
    // if not the tail, then connect the prev chain
    if (link->pNext != 0)
    {
      link->pNext->pPrev = link->pPrev;
    }
    // if this link is the tail, update the list's tail
    // and mark the prev link as such
    else
    {
      list->pTail = link->pPrev;
      link->pPrev->pNext = 0;
    }
    
    // zero out this links pointers
    link->pPrev = 0;
    link->pNext = 0;
    link->pList = UDELL_NOLIST;
    
    list->len -= 1;
  }
  
  return UDELL_PASS;
}

// Takes a ptr to a link and returns a pointer to the data
void* LinkGetData(udellLink *link)
{
  // the link pointer needs to point to valid data
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  return link->pData;
}

// Same as LinkGetData(), but interprets the data as an int and returns it - for debugging
uint32_t LinkGetDataInt32(udellLink *link)
{
  // the pointer needs to point to valid data
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // correctly cast and follow the pointer to the data
  return *(uint32_t*)(link->pData);
}

// Takes a ptr to a link and returns the payload size
uint32_t LinkGetPayloadSize(udellLink *link)
{
  // the pointer needs to point to valid data
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  return link->payloadSize;
}

// Takes a ptr to a link and returns the data size
uint32_t LinkGetDataSize(udellLink *link)
{
  // the pointer needs to point to valid data
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  return link->dataSize;
}

// Takes a link pointer and returns a pointer to the next link
udellLink* LinkGetNext(udellLink *link)
{
  // the pointer needs to point to valid data
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  // if the end of the list has been reached, fail
  if (link->pNext == 0)
  {
    errno = UDELL_ETAIL;
    return UDELL_FAIL;
  }

  return link->pNext;
}

// Takes a link pointer and returns a pointer to the prev link
udellLink* LinkGetPrev(udellLink *link)
{
  // the pointer needs to point to valid data
  if (link == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
    
  // if the head of the list has been reached, fail
  if (link->pPrev == 0)
  {
    errno = UDELL_EHEAD;
    return UDELL_FAIL;
  }
    
  return link->pPrev;
}

// Takes a pointer to a list, and returns a link pointer to the head of the list
udellLink* ListGetHead(udellList *list)
{
  // the pointer needs to point to valid data
  if (list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
    
  // if the list contains no links this makes no sense, fail
  if (list->len == 0)
  {
    errno = UDELL_EEMPTY;
    return UDELL_FAIL;
  }
  
  return list->pHead;
}

// Takes a pointer to a list, and returns a link pointer to the tail of the list
udellLink* ListGetTail(udellList *list)
{
  // the pointer needs to point to valid data
  if (list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
    
  // if the list contains no links this makes no sense, fail
  if (list->len == 0)
  {
    errno = UDELL_EEMPTY;
    return UDELL_FAIL;
  }

  return list->pTail;
}

// Takes a pointer to a list and returns the number of links
int ListGetLen(udellList *list)
{
  // the pointer needs to point to valid data
  if (list == UDELL_FAIL)
  {
    errno = UDELL_EINVPTR;
    return UDELL_FAIL;
  }
  
  return list->len;
}


int main(int argc, char* argv[])
{
  
  int i, retval;
  udellLink link[100];
  udellList list;
  
  udellLink* pLink;
  int *ptr;
  
  int arrayData[100];
  
  for (i=0;i<15;i++)
    arrayData[i] = i;
    
  for (i=0;i<15;i++)
  {
    retval = LinkInit(&link[i], (void*)&arrayData[i], sizeof(int), sizeof(int));
    printf("LinkInit retval: %d    errno %d\n", retval, errno);
  }
  
  printf("data begins at: %x\n", (unsigned int)arrayData);
  
  pLink = &(link[0]);
  printf("first link data location: %x\n", (unsigned int)pLink->pData);
  printf("first link data value: %d\n", *(int*)pLink->pData);
  
  ptr = (int*)LinkGetData(&link[0]);
  printf("LinkGetData: [%x] %d\n", (unsigned int)ptr, *ptr);
  
  retval = ListInit(&list);
  printf("ListInit retval: %d  errno %d\n", retval, errno);
  
  retval = ListGetLen(&list);
  printf("ListGetLen retval: %d  errno %d\n", retval, errno);
  
  pLink = ListGetHead(&list);
  printf("ListGetHead retval: %d  errno %d\n", (unsigned int)pLink, errno);
  
  pLink = ListGetTail(&list);
  printf("ListGetTail retval: %d  errno %d\n", (unsigned int)pLink, errno);
  
  ListSetMaxLinks(&list, 3);
  
  for (i=0;i<5;i++)
  {
    retval = ListInsertLast(&list, &link[i]);
    printf("LinkInsertLast retval: %d  errno %d\n", retval, errno);
  }
  
  retval = ListRemoveLink(&list, &link[0]);
  printf("ListRemoveLink retval: %d  errno %d\n", retval, errno);
  
  for (i=0;i<7;i++)
  {
    printf("Link %d [%x] %d\n", i, (unsigned int)&link[i], LinkGetDataInt32(&link[i]));
    printf("  prev [%x]\n", (unsigned int)link[i].pPrev);
    printf("  next [%x]\n", (unsigned int)link[i].pNext);
  }
  
  pLink = ListGetHead(&list);
  printf("ListGetHead retval: %d  errno %d  loc: %x data: %d\n", (unsigned int)pLink, errno, (unsigned int)pLink, LinkGetDataInt32(pLink));
  printf("first link addr: %x\n", (unsigned int)pLink);
  
  for (i=0;i<7;i++)
  {
    pLink = LinkGetNext(pLink);
    printf("LinkGetNext retval: %x  errno %d  data: %d\n", (unsigned int)pLink, errno, LinkGetDataInt32(pLink));
  }
  
  pLink = ListGetTail(&list);
  printf("ListGetTail retval: %d  errno %d  loc: %x data: %d\n", (unsigned int)pLink, errno, (unsigned int)pLink, LinkGetDataInt32(pLink));
  printf("last link addr: %x\n", (unsigned int)pLink);
  
  for (i=0;i<7;i++)
  {
    pLink = LinkGetPrev(pLink);
    printf("LinkGetNext retval: %x  errno %d  data: %d\n", (unsigned int)pLink, errno, LinkGetDataInt32(pLink));
  }
  
  return 0;
}

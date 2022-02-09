#include "os_cirbuf.h"

int osCirBufCreate(osCirBuf *cb, uint8_t *bytes, uint32_t size)
{
  if( ( size == 0 ) || ( cb == NULL ) || (bytes == NULL) )
    return -1;
  cb->ptr = bytes;
  cb->bufsize = size;
  cb->last = ((uint32_t)cb->ptr + cb->bufsize - 1u);
  if( osCirBufClear(cb) == -1 )
    return -1;
  return 1;
}

int osCirBufClear(osCirBuf *cb)
{
  if( cb == NULL )
	  return -1;
  if( cb->ptr == NULL )
	  return -1;
  if( cb->bufsize == 0 )
	 return -1;
  memset(cb->ptr, 0x00, cb->bufsize);
  cb->head = cb->ptr;
  cb->tail = cb->ptr;
  cb->used = 0;
  return 0;
}

int osCirBufRead(osCirBuf *cb, uint8_t *bytes, uint32_t len)
{
  uint32_t counter;
  if( ( len == 0 ) || ( cb == NULL ) || ( cb->ptr == NULL ) ||
      ( cb->bufsize == 0 )  || (bytes == NULL) )
    return -1;
  /* check if there is data */
  if (cb->used == 0)
    return 0;
  if (len > cb->used)
    len = cb->used;
  cb->used -= len;
  for (counter = 0; counter < len; counter++)
  {
    bytes[counter] = *(cb->head++);
    /* wrap if needed */
    if ( (uint32_t)cb->head > cb->last )
      cb->head = cb->ptr;
  }
  return len;
}

int osCirBufWrite(osCirBuf *cb, uint8_t *bytes, uint32_t len)
{
  int freesize;
  uint32_t counter;
  if( ( len == 0 ) || ( cb == NULL ) || ( cb->ptr == NULL ) ||
      ( cb->bufsize == 0 )  || (bytes == NULL) )
    return -1;
  /* check if there is space */
  freesize = cb->bufsize - cb->used;
  if ( len > freesize )
    return 0;
  cb->used += len;
  for (counter = 0; counter < len; counter++)
  {
    *(cb->tail++) = bytes[counter];
    /* wrap if needed */
    if ( (uint32_t)cb->tail > cb->last )
      cb->tail = cb->ptr;
  }
  return len;
}

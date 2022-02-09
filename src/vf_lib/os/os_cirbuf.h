#ifndef OS_CIRBUF_H
#define OS_CIRBUF_H

#include "os_common.h"

typedef struct
{
  uint8_t   *ptr;
  uint8_t   *head;
  uint8_t   *tail;
  uint32_t  bufsize;
  uint32_t  last;
  uint32_t  used;
}osCirBuf;

int osCirBufCreate(osCirBuf *cb, uint8_t *bytes, uint32_t size);
int osCirBufClear(osCirBuf *cb);
/* len on success, -1 on fail */
int osCirBufWrite(osCirBuf *cb, uint8_t *bytes, uint32_t len);
/* len on success, -1 on fail */
int osCirBufRead(osCirBuf *cb, uint8_t *bytes, uint32_t len);

#endif

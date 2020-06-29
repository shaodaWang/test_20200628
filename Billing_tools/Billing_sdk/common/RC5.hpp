/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C.        */
/* Copyright (C) 1995 RSA Data Security, Inc.                        */


#ifndef __RC5_ENCODE_DECODE__
#define __RC5_ENCODE_DECODE__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BSCP_MAX_CMDLEN 20*1024 

#define ENCODE_PASSWORD_STR "afwjerouiqw9rasl" 

#define DECODE_PASSWORD_STR ENCODE_PASSWORD_STR


int SEND_RC5ENCODE(char *pBuffer, int nSize, char *pEncodeBuf);

int RECV_RC5DECODE(char *pEncodeBuf, int nSize, char *pDecodeBuf);


#endif //__RC5_ENCODE_DECODE__


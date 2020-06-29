#include "stdafx.h"
#include "RC5.hpp"


#define w        32             /* word size in bits                 */
#define r        12             /* number of rounds                  */  
#define b        16             /* number of bytes in key            */
#define c         4             /* number  words in key = ceil(8*b/w)*/
#define t        26             /* size of table S = 2*(r+1) words   */
unsigned long S[t];             /* expanded key table				 */
unsigned long P = 0xb7e15163;	/* magic constants					 */
unsigned long Q = 0x9e3779b9;   /* magic constants					 */

/* Rotation operators. x must be unsigned, to get logical right shift*/
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

static bool bRC5Init = false;

/* 2 WORD input pt/output ct  */
static void RC5_ENCRYPT(unsigned long *pt, unsigned long *ct) 
{
	unsigned long A = pt[0]+S[0];
	unsigned long B = pt[1]+S[1];
	for (int i=1; i<=r; i++) 
  { 
		A = ROTL(A^B,B) + S[2*i]; 
		B = ROTL(B^A,A) + S[2*i+1]; 
  }
	
	ct[0] = A; 
	ct[1] = B;  
} 


/* 2 WORD input ct/output pt  */
static void RC5_DECRYPT(unsigned long *ct, unsigned long *pt) 
{ 
	unsigned long B = ct[1];
	unsigned long A = ct[0];
	for (int i=r; i>0; i--) 
  { 
		B = ROTR(B-S[2*i+1], A)^A; 
		A = ROTR(A-S[2*i], B)^B; 
  }
	
	pt[1] = B - S[1]; 
	pt[0] = A - S[0];  
} 


/* secret input key K[0...b-1]  */
static void RC5_SETUP(char *K) 
{ 
	int i;
  unsigned long j, k, u=w/8, A, B, L[c]; 

  /* Initialize L, then S, then mix key into S */
  for (i=b-1,L[c-1]=0; i!=-1; i--)
  {
	  L[i/u] = (L[i/u]<<8) + K[i];
  }

  for (S[0]=P,i=1; i<t; i++) 
  {
	  S[i] = S[i-1] + Q;
  }

  /* 3*t > 3*c */
  for (A=B=i=j=k=0; k<3*t; k++,i=(i+1)%t,j=(j+1)%c)   
  { 
	  A = S[i] = ROTL(S[i]+(A+B), 3);  
	  B = L[j] = ROTL(L[j]+(A+B), (A+B)); 
  } 
}


/*===================================================================
* 函数:
* 参数:		pBuffer－待加密数据缓冲
			nSize－加密前数据大小
			pEncodeBuf－加密后数据缓冲
* 功能:		执行加密
* 说明:		返回加密后数据大小
* 时间:		2007-9-15
* 作者:		罗燕华
====================================================================*/
int SEND_RC5ENCODE(char *pBuffer, int nSize, char *pEncodeBuf)
{
  unsigned long pt[2];
	unsigned long ct[2] = {0, 0};
  int i;

	memset(pEncodeBuf, 0, BSCP_MAX_CMDLEN);
	
	if (!bRC5Init) 
	{
    RC5_SETUP(ENCODE_PASSWORD_STR);
		bRC5Init = true;
	}
	
  for (i=0; i<nSize/8; i++) 
	{
    memcpy(pt, (char *)&pBuffer[i*8], 8);
    RC5_ENCRYPT(pt, ct);
    memcpy(&pEncodeBuf[i*8], (char *)ct, 8);  
  }
	
  if (nSize % 8) 
	{
    memcpy(pt, (char *)&pBuffer[i*8], nSize % 8);    
    RC5_ENCRYPT(pt,ct);
    memcpy(&pEncodeBuf[i*8], (char *)ct, 8);          
    nSize = (nSize/8 + 1) * 8;    
  }    
	
  return nSize;
}


/*===================================================================
* 函数:
* 参数:		pEncodeBuf－输入的加密缓冲
			nSize－加密数据的长度
			pDecodeBuf－解密后的数据缓冲
* 功能:		执行解密
* 说明:		返回解密前数据大小,失败返回-1
* 时间:		2007-9-15
====================================================================*/
int RECV_RC5DECODE(char *pEncodeBuf, int nSize, char *pDecodeBuf)
{
  if (NULL == pDecodeBuf) 
	{
		return -1;
  }

  if (!bRC5Init) 
	{
    RC5_SETUP(ENCODE_PASSWORD_STR);  
    bRC5Init = true;
  }

  if (strcmp(DECODE_PASSWORD_STR, ENCODE_PASSWORD_STR))
	{
    return -1;
  }

	unsigned long pt[2];
	unsigned long ct[2] = {0, 0};
  for (int i=0; i<nSize/8; i++) 
	{        
    memcpy(pt, (char *)&pEncodeBuf[i*8], 8);
    RC5_DECRYPT(pt, ct);
    memcpy(&pDecodeBuf[i*8], (char *)ct, 8);      
  }

  return nSize;
}


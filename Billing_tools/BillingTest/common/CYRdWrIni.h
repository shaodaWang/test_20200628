#ifndef _CYRDWRINI_H_
#define _CYRDWRINI_H_
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


//namespace cy_pt{
#define FALSE 0
#define TRUE  1
#define MAX_PATH 260
#define MAX_LINE 1024


typedef int BOOL;

#define Min(a,b) (((a)<(b))?(a):(b))


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

BOOL CYCopyFile(const char* lpExistingFileName, // name of an existing file
			  const char* lpNewFileName,      // name of new file
			  BOOL bFailIfExists         // operation if file exists
			  );

int	CYGetLine(char * pFileBuffer ,char* pLine, unsigned int dwOffset, unsigned int dwSize);
int	CYGetLineOriginal(char * pFileBuffer ,char* pLine, unsigned int dwOffset, unsigned int dwSize);
BOOL CYIsComment(const char* pLine);
BOOL CYIsSection(const char* pLine) ;
BOOL CYIsSectionName(const char* pLine, const char* pSection);
BOOL CYIsKey(char* pLine , const char* pKeyName, char** pValue, unsigned int* dwValLen );  
unsigned int CYGetString( const char* lpAppName,const char* lpKeyName,   
                char* lpReturnedString, unsigned int nSize,const char* lpFileName); 

int CYGetFileSize(const char* lpFileName);
int CYReadIniFile(const char* lpFileName,char * pFileBuffer ,int nBufferSize) ;  

/************************************************************************  
**������GetPrivateProfileString  
**���ܣ���ƽ̨��ȡINI�ļ���ĳ����/�����ļ�ֵ���ַ���  
**������  
		lpAppName[in]         - points to section name  
		lpKeyName[in]         - points to key name  
		lpDefault[in]         - points to default string  
		lpReturnedString[out] - points to destination buffer  
		nSize[in]             - size of destination buffer "lpReturnedString"(in characters)  
		lpFileName[in]        - points to initialization filename     
**���أ�The return value is the number of characters copied to the buffer,   
		not including the terminating null character.          
**��ע��  
1). ���INI�ļ�û������ĵ����ݣ�����Ĭ��ֵlpDefault  
************************************************************************/   
unsigned int CYGetPrivateProfileString(   
    const char* lpAppName,   
    const char* lpKeyName,   
    const char* lpDefault,    
    char*  lpReturnedString,   
    unsigned int   Size,   
    const char* lpFileName );
/************************************************************************  
**������GetPrivateProfileInt  
**���ܣ� retrieves an integer associated with a key in the  
		specified section of the given initialization file  
**������  
		LPCTSTR lpAppName,  // address of section name  
		LPCTSTR lpKeyName,  // address of key name  
		INT nDefault,       // return value if key name is not found  
		LPCTSTR lpFileName  // address of initialization filename  
**���أ�  
		The return value is the integer equivalent of the string following   
		the specified key name in the specified initialization file. If the   
		key is not found, the return value is the specified default value.   
		If the value of the key is less than zero, the return value is zero.     
************************************************************************/
int CYGetPrivateProfileInt(   
  const char* lpAppName,   
  const char* lpKeyName,    
  int nDefault,   
  const char* lpFileName );
 
void CYWriteLine(FILE* hOutput , const char* pLine);  

/************************************************************************  
**������WritePrivateProfileString  
**���ܣ���ƽ̨�����£���ָ��INI�ļ�ָ������д���ַ�������  
**������  
		lpAppName[in]  
		Pointer to a null-terminated string containing section name. If  
		the section does not exit, it is created.  
		lpKeyName[in]  
		Pointer to a null-terminated string containing key name. If the  
		key does not exit in the specified section pointed to by the lpAppName   
		parameter, it's created. If this parameter is NULL, the ertire section,   
		including all keys within the section, is deleted. When deleting a   
		section, leave the comments intact.  
		lpString[in]  
		pointer to a null-terminated string to be written to the file.   
		If this parameter is NULL, the key pointed to by the lpKeyName   
		parameter is deleted.  
		lpFileName[in]  
		Pointer to a null-terminated string that names the initialization file.   
**���أ�  
		FALSE - fail  
		TRUE  - success  
**��ע��  
		1). �Ƚ�Ҫ�޸ĵ�INI�ļ���ȫ�����ݶ�ȡ��ȫ���ڴ�g_pData��  
		2). ��g_pData�ж�λ������Ҫ�޸ĵ�λ�ã����������ݺ������޸ĵ�����д��һ��ʱini�ļ�  
		3). �����ʱini�ļ�����ԭ����ini�ļ�����ɾ����ʱini�ļ�  
		4). ��Ҫ��API������  
		creat��lseek��close��remove��open��mmap 
		5). ���lpKeyName == NULL, ɾ��������, ���lpString == NULL, ɾ����  
************************************************************************/   
BOOL CYWritePrivateProfileString(   
  const char* lpAppName,   
  const char* lpKeyName,   
  const char* lpString,   
  const char* lpFileName);  
/************************************************************************  
**������WritePrivateProfileInt  
**���ܣ���ƽ̨�����£���ָ��INI�ļ�ָ������д����������  
**�������ο�WritePrivateProfileString����  
**���أ�  
		FALSE - fail  
		TRUE  - success  
**��ע��  
************************************************************************/   
BOOL CYWritePrivateProfileInt(   
    const char* lpAppName,   
    const char* lpKeyName,   
    int     Value,   
    const char* lpFileName) ; 

void CYWriteValue(FILE* fdOutput, const char* pAppName, const char* pKeyName, const char* pString);   


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif //_RDWRINI_H_


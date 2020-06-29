

#include "CYRdWrIni.h"
 

#if defined(_MSC_VER)

#include <Windows.h>

#else

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#endif


/************************************************************************  
**函数：CopyFile  
**功能：The CopyFile function copies an existing file to a new file. 

**参数：  
		lpExistingFileName[in] -Pointer to a null-terminated string
							    that specifies the name of an existing file. 
		lpNewFileName[in]      - Pointer to a null-terminated string that specifies the name of the new file. 
		bFailIfExists[in]      - Specifies how this operation is to proceed if a file of the same name as that 
							      specified by lpNewFileName already exists.
								   If this parameter is TRUE and the new file already exists, the function fails. 
									If this parameter is FALSE and the new file already exists, 
									the function overwrites the existing file and succeeds. 
**返回：  
		TRUE - success  
		FALSE - fail 
**备注：
************************************************************************/  

BOOL CYCopyFile(const char* lpExistingFileName, // name of an existing file
			  const char* lpNewFileName,      // name of new file
			  BOOL bFailIfExists         // operation if file exists
			  )
{

#if defined(_MSC_VER) //windows

	return CopyFileA(  lpExistingFileName,  lpNewFileName,
		bFailIfExists);


#else //linux

	int fdIn = 0, fdOut = 0;
	struct stat statinfo;
	unsigned int dwSize =0;
	void *pSrc =NULL, *pDst = NULL;
	
	memset(&statinfo,0, sizeof(statinfo));
	if (!lpExistingFileName || !lpExistingFileName)
	{
		return FALSE;
	}
	
	fdIn = open(lpExistingFileName, O_RDONLY);
	if (fdIn<0)
	{
		perror("CopyFile open");
		return FALSE;
	}
	// get file info
	if (fstat(fdIn, &statinfo) < 0) 
	{
		perror("CopyFile fstat ");
	}
	dwSize = statinfo.st_size;
	//printf("statinfo.st_size = %d\n ", statinfo.st_size);
	if (bFailIfExists)
	{
		fdOut = open(lpNewFileName, O_RDWR | O_CREAT | O_EXCL,S_IRWXU);
		if (fdOut== -1)
		{
			perror("CopyFile new file fail");
			return FALSE;
		}
	}
	else
	{
		fdOut = open(lpNewFileName, O_RDWR | O_CREAT |O_TRUNC,S_IRWXU);
		if (fdOut== -1)
		{
			perror("CopyFile new file fail ");
			return FALSE;
		}
	}
	//printf("CopyFile lseek\n");
	if (lseek(fdOut, dwSize - 1, SEEK_SET) == -1) 
	{
		perror("CopyFile lseek ");
		return FALSE;
	}
	
	if (write(fdOut, "", 1) != 1) 
	{
		perror("the file isn't writable ");
		return FALSE;
	}
	//printf("CopyFile write\n");
	if ((pSrc = mmap(NULL, dwSize, PROT_READ, MAP_SHARED, fdIn, 0)) == MAP_FAILED)
	{
		perror("mmap ");
		return FALSE;	
	}
	//printf("CopyFile mmap 1\n");
	if ((pDst = mmap(NULL, dwSize, PROT_READ|PROT_WRITE, MAP_SHARED, fdOut, 0)) == MAP_FAILED) 
	{
		perror("mmap ");
		return FALSE;
	}
	//printf("CopyFile mmap 2\n");
	//dose copy the file
	memcpy(pDst,pSrc, dwSize);
	close(fdOut);
	close(fdIn);

	return TRUE;
#endif

}

/************************************************************************  
**函数：GetLine  
**功能：获取在file中从dwOffset位置开始的一行数据并保存到pLine，同时把偏移量dwOffset  
		移到下一行行首  
**参数：  
		pLine[out]   - 接收一行数据(不包括\r\n)  
		dwOffset[in] - 要读取的那一行的开始位置  
		dwSize[in]   - INI文件大小  
**返回：  
		正确 - 下一行行首的位置  
		错误 - 0  
**备注：
************************************************************************/   
int	CYGetLine(char * szFileBuff ,char* pLine, unsigned int dwOffset, unsigned int dwSize)
{
	int len = 0;   
    int len2 = 0;

	char* findstrPointer;

    // Look for the end of the line.    
    while ( dwOffset + len < dwSize   
        && '\r' != szFileBuff[dwOffset+len] && '\n' != szFileBuff[dwOffset+len])   
    {   
        if( szFileBuff[dwOffset+len]==0 )   
            break;   
        pLine[len] = szFileBuff[dwOffset+len] ;   
        ++len;   
    }   
	
    pLine[len] = 0 ;   
    // Now push the internal offset past the newline.    
    // (We assume \r\n pairs are always in this order)    
    if (dwOffset + len + len2 < dwSize && '\r' == szFileBuff[dwOffset+len+len2])   
        ++len2;   
    if (dwOffset + len + len2+1 < dwSize && '\n' == szFileBuff[dwOffset+len+len2])   
        ++len2;   
    if (2 >= len + len2 && (dwOffset +2 >=dwSize) )   
        return 0;

	
	//remove comment ";" "#" from line
	findstrPointer = strchr(pLine,';');
 	if(findstrPointer) findstrPointer[0] = '\0';

	findstrPointer = strchr(pLine,'#');
	if(findstrPointer) findstrPointer[0] = '\0';

    dwOffset += len + len2;   
    return dwOffset;   
}

int	CYGetLineOriginal(char * szFileBuff ,char* pLine, unsigned int dwOffset, unsigned int dwSize)
{
	int len = 0;   
	int len2 = 0;

	// Look for the end of the line.    
	while ( dwOffset + len < dwSize   
		&& '\r' != szFileBuff[dwOffset+len] && '\n' != szFileBuff[dwOffset+len])   
	{   
		if( szFileBuff[dwOffset+len]==0 )   
			break;   
		pLine[len] = szFileBuff[dwOffset+len] ;   
		++len;   
	}   

	pLine[len] = 0 ;   
	// Now push the internal offset past the newline.    
	// (We assume \r\n pairs are always in this order)    
	if (dwOffset + len + len2 < dwSize && '\r' == szFileBuff[dwOffset+len+len2])   
		++len2;   
	if (dwOffset + len + len2+1 < dwSize && '\n' == szFileBuff[dwOffset+len+len2])   
		++len2;   
	if (2 >= len + len2 && (dwOffset +2 >=dwSize) )   
		return 0;

	dwOffset += len + len2;   
	return dwOffset;   
}



/************************************************************************  
**函数：IsComment  
**功能：判断是不是注释行  
**参数：  
		pLine[in] - INI的一行数据  
**返回：  
		1 - 注释行  
		0 - 不是注释行  
**备注：  
		1). 空行也视为注释行  
************************************************************************/ 

BOOL CYIsComment(const char* pLine)
{
	if (!pLine || 0 == strlen(pLine) 
		|| ';' == *pLine || '#'== *pLine )   
        return TRUE;   
    return FALSE;  
}

/************************************************************************  
**函数：IsSection  
**功能：判断是不是段名  
**参数：  
		pLine[in] - INI的一行数据  
**返回：  
		1 - 是段名  
		0 - 不是  
************************************************************************/   
BOOL CYIsSection(const char* pLine)   
{   
    if (pLine && '[' == *pLine)   
        return TRUE;   
    return FALSE;   
}   

/************************************************************************  
**函数：IsSectionName  
**功能：判断是INI文件的一行(pLine)是不是要找的段名(pSection)  
**参数：  
pLine[in]    - INI文件的一行数据  
pSection[in] - 要找的段名  
**返回：  
1 - 是  
0 - 不是  
**备注：  
************************************************************************/   
BOOL CYIsSectionName(const char* pLine, const char* pSection)   
{   
    if (CYIsSection(pLine))   
    {   
        unsigned int len = strlen(pSection);   
        if (strlen(pLine) - 2 == len && 0 == strncmp(pLine+1, pSection, len))   
            return TRUE;   
    }   
    return FALSE;   
}  


/************************************************************************  
**函数：IsKey  
**功能：判断INI文件中一行的数据是不是要找的键名,如果是并读取键值  
**参数：  
		pLine[in] - INI文件某行数据  
		pKeyName[in] - 要寻找的键名  
		pValue[out] - 键值  
		dwValLen[out] - 键值pValue大小(in bytes)  
**返回：  
		1 - 是，同时pValue返回键值  
		0 - 不是，pValue为NULL   
**备注：  
************************************************************************/   
BOOL CYIsKey(char* pLine , const char* pKeyName, char** pValue, unsigned int* dwValLen )   
{   
    char* pEqual = NULL;   
    unsigned int length = 0, len = 0;   
	
    if(!pLine || !pKeyName)   
        return FALSE;   
	
    // pLine是不是注释行    
    if (CYIsComment( pLine ))   
        return FALSE;   
	
    // 寻找"="号    
    pEqual = strchr(pLine, '=' );   
    if (!pEqual)   
        return FALSE;   
	
    // 寻找键名最后一字符的位置    
    //while (pEqual - 1 >= pLine && iswspace(*(pEqual-1)))   
	while(pEqual - 1 >= pLine && ((*(pEqual-1)) == "")) 
        --pEqual;   
    // Badly formed file.    
    if (pEqual - 1 < pLine)   
        return FALSE;   
	
    // 键名长度    
    length = pEqual - pLine;   
	
    len = strlen(pKeyName);   
    //if (len == length && 0 == _wcsnicmp(pLine, pKeyName, len))    
    if ( 0 == strncmp(pLine, pKeyName, len))   
    {   
        *pValue = strchr(pLine, '=' );   
        ++(*pValue);   
        *dwValLen = strlen(pLine) - ((*pValue) - pLine);   
		
        // 去掉紧跟在"="号后的所有空格    
        //while (0 < *dwValLen && iswspace(**pValue))   
		while (0 < *dwValLen && ((**pValue) == ""))   
        {   
            ++(*pValue);   
            --(*dwValLen);   
        }   
        while (0 < *dwValLen && ((*pValue)[*dwValLen-1] == ""))   
        {   
            --(*dwValLen);   
        }   
		while (0 < *dwValLen && ((*pValue)[*dwValLen-1] == '\t'))   
		{   
			--(*dwValLen);   
		} 
		while (0 < *dwValLen && ((*pValue)[*dwValLen-1] == ' '))   
		{   
			--(*dwValLen);   
		} 
        // If the string is surrounded by quotes, remove them.    
        if ("" == (**pValue))   
        {   
            ++(*pValue);   
            --(*dwValLen);   
            if ('"' == (*pValue)[*dwValLen-1])   
            {   
                --(*dwValLen);   
            }   
        }   
        return TRUE;   
    }   
    else   
    {   
        *pValue = NULL;   
        return FALSE;   
    }   
}


int CYGetFileSize(const char* lpFileName)
{

	FILE* pFile =0;
	int filesize = 0;

	if (!lpFileName)   
	{
		return -1;
	}

	pFile = fopen(lpFileName, "r");
	if(pFile <= 0)
	{
		return -2;      
	}

	fseek(pFile,0,SEEK_END);
	filesize = ftell(pFile);

	fclose(pFile);
	return filesize; //file size + '\0'

}


/************************************************************************  
**函数：ReadIniFile  
**功能：打开文件，并将文件数据拷贝到一缓冲区pFileBuffer中   
**参数：  
		lpFileName[in] - INI文件名，如果没有路径，默认路径当前应用程序路劲 
**返回：  
		非0 - 该INI文件的大小(in bytes)  
		0   - 失败  
**备注：  

************************************************************************/   
int CYReadIniFile(const char* lpFileName,char * pFileBuffer ,int nBufferSize)   
{   
    FILE* fdFile =0; 
    BOOL  bRet = FALSE; // bUnicode - 标志INI文件是不是Unicode文件    
 	
    if (!lpFileName)   
    {   
        return -1;   
    }   
	
    // ReadIniFile the file.    
	fdFile = fopen(lpFileName, "r");
	if(fdFile < 0)
	{
		return -2;   
	}

	bRet = fread(pFileBuffer, sizeof(char), nBufferSize,fdFile);
	fclose(fdFile);   

	return bRet;

}   


/************************************************************************  
**函数：GetString  
**功能：读INI文件  
**参数：  
		lpAppName[in]         - 字段名  
		lpKeyName[in]         - 键名  
		lpReturnedString[out] - 键值  
		nSize[in]             - 键值缓冲区大小(in characters )  
		lpFileName[in]        - 完整的INI文件名  
**返回：Returns the number of bytes read.  
**备注：  
************************************************************************/   
unsigned int CYGetString( const char* lpAppName,const char* lpKeyName,   
                char* lpReturnedString, unsigned int nSize,const char* lpFileName)   
{     
    unsigned int nReadSize , cchCopied;   
    unsigned int dwOffset = 0;   
    char pLine[MAX_LINE] = {0} ;   
    unsigned int dwValLen = 0;
	int  nFileSize = 0;
	char* pFileBuff = NULL;
	
    if (!lpAppName || !lpFileName)   
        return 0;

	nFileSize = CYGetFileSize(lpFileName);
	if (nFileSize <= 0)
	{
		return 0;
	}
	pFileBuff = (char*)malloc(nFileSize+sizeof(char));
	if(!pFileBuff)
	{
		return 0;
	}
	memset(pFileBuff,0,nFileSize+sizeof(char));

	
    //打开文件，将文件数据拷贝到缓冲区pFileBuffer中，返回INI文件大小    
    if (0 == (nReadSize = CYReadIniFile(lpFileName,pFileBuff,nFileSize)))   
    {   
     //   printf("GetString, Could not ReadIniFile INI file: %s\n", lpFileName);   
        return 0;   
    }   
	
    cchCopied = 0;   
    while ( 0 != (dwOffset = CYGetLine( pFileBuff ,pLine , dwOffset , nReadSize )))   
    {   
        //RETAILMSG(1,(_T("%s\n"),szLine));    
        // 是不是注释行    
        if (CYIsComment(pLine))   
            continue;   
		
        // 是不是段名    
        if (CYIsSection(pLine))   
        {   
            // 是不是我们要找的段名    
            if (CYIsSectionName(pLine,lpAppName))   
            {   
                // 寻找我们要的键名    
                while ( 0 != (dwOffset = CYGetLine( pFileBuff,pLine , dwOffset , nReadSize)))   
                {   
                    char* pValue=NULL;   
					
                    if (CYIsSection(pLine))   
                        break;
					// 是不是注释行    
					if (CYIsComment(pLine))   
						continue;   
					
                    if (CYIsKey(pLine , lpKeyName, &pValue, &dwValLen))   
                    {   
                        cchCopied = Min(dwValLen, nSize-1);   
                        memcpy(lpReturnedString, pValue, cchCopied);   
                        lpReturnedString[cchCopied] = 0;   
                        // We're done.    
                        break;                         
                    }   
                }   
                break;   
            }   
        }   
    }

	if(pFileBuff)
	{
		free(pFileBuff);
	}
	
    return cchCopied;   
}  

/************************************************************************  
**函数：GetPrivateProfileString  
**功能：跨平台下读取INI文件中某段名/键名的键值的字符串  
**参数：  
		lpAppName[in]         - points to section name  
		lpKeyName[in]         - points to key name  
		lpDefault[in]         - points to default string  
		lpReturnedString[out] - points to destination buffer  
		nSize[in]             - size of destination buffer "lpReturnedString"(in characters)  
		lpFileName[in]        - points to initialization filename     
**返回：The return value is the number of characters copied to the buffer,   
		not including the terminating null character.          
**备注：  
1). 如果INI文件没有你关心的数据，返回默认值lpDefault  
************************************************************************/   
unsigned int CYGetPrivateProfileString(   
							  const char* lpAppName,   
							  const char* lpKeyName,   
							  const char* lpDefault,    
							  char*  lpReturnedString,   
							  unsigned int   Size,   
							  const char* lpFileName )   
{   
    unsigned int dwRc = 0, dwReturn = 0;   
    if(!lpAppName || !lpKeyName || !lpReturnedString || !lpFileName || Size<=0 )   
        return 0;   
	
    dwRc = CYGetString(lpAppName,lpKeyName,lpReturnedString,Size,lpFileName);   
    if(dwRc != 0)   
    {   
        dwReturn = dwRc;   
    }   
    else   
    {   
        if(lpDefault)   
        {   
            memcpy(lpReturnedString, lpDefault, Size);   
            lpReturnedString[Size-1] = NULL;   
        }   
        else   
            *lpReturnedString = 0;   
        dwReturn = strlen(lpReturnedString);   
    }   
	  
    return dwReturn;   
}   

/************************************************************************  
**函数：GetPrivateProfileInt  
**功能： retrieves an integer associated with a key in the  
		specified section of the given initialization file  
**参数：  
		LPCTSTR lpAppName,  // address of section name  
		LPCTSTR lpKeyName,  // address of key name  
		INT nDefault,       // return value if key name is not found  
		LPCTSTR lpFileName  // address of initialization filename  
**返回：  
		The return value is the integer equivalent of the string following   
		the specified key name in the specified initialization file. If the   
		key is not found, the return value is the specified default value.   
		If the value of the key is less than zero, the return value is zero.   
**备注：  
************************************************************************/   
int CYGetPrivateProfileInt(   
						  const char* lpAppName,   
						  const char* lpKeyName,    
						  int nDefault,   
						  const char* lpFileName )   
{   
    char szRet[80] ={0}; 
	unsigned int cch =0;
	
    if(!lpAppName || !lpKeyName || !lpFileName )   
        return 0;   
	
    cch = CYGetString(lpAppName, lpKeyName, szRet, sizeof(szRet)/sizeof(char), lpFileName);   
	
    if (cch)   
        return atoi(szRet);   
    else   
        return nDefault;       
}  

/************************************************************************  
**函数：WriteLine  
**功能：向文件写入一行数据(包括回车换行符)  
**参数：  
		hOutput[in] - 已打开的文件句柄  
		pLine[in]   - 要写入的一行数据  
**返回：NONE  
**备注：  
		1). 写入一行数据，也包括写入行末的"\r\n"两个字符  
		3). 注意不要将结束符也写入文件  
************************************************************************/   
void CYWriteLine(FILE* hOutput , const char* pLine)   
{   
	if (pLine)   
	{   
		fwrite( pLine, sizeof(char),strlen(pLine)*sizeof(char),hOutput);   
		fwrite( "\r\n",sizeof(char) ,2*sizeof(char),hOutput);   
	}   
}   

/************************************************************************  
**函数：WritePrivateProfileString  
**功能：跨平台环境下，向指定INI文件指定段名写入字符串数据  
**参数：  
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
**返回：  
		FALSE - fail  
		TRUE  - success  
**备注：  
		1). 先将要修改的INI文件的全部数据读取到pFileBuffer中  
		2). 在pFileBuffer中定位到我们要修改的位置，将其它数据和我们修改的数据写入一临时ini文件  
		3). 最后将临时ini文件覆盖原来的ini文件，再删除临时ini文件  
		4). 主要的API函数：  
		creat、lseek、close、remove、open、mmap 
		5). 如果lpKeyName == NULL, 删除整个段, 如果lpString == NULL, 删除健  
************************************************************************/   
BOOL CYWritePrivateProfileString(   
							   const char* lpAppName,   
							   const char* lpKeyName,   
							   const char* lpString,   
							   const char* lpFileName)   
{   
    unsigned int dwSize, dwOffset;                // dwSize - ini文件大小, dwOffset - 偏移量    
    BOOL  bReadLine = TRUE;   
    BOOL  bWrote = FALSE;   
    char pszLine[MAX_LINE] = {0};         // 存储一行的数据    
    char pszIniFileTemp[MAX_PATH] = {0};  // 临时ini文件的名称(包括路径)    
    FILE*  fdOutputFile ;   
    char* pValue;   
    unsigned int dwValLen; 
	char *pFileBuff = NULL;
	int oldFileSize = 0;
	
    dwOffset = 0;   
	if (!lpFileName) {  
        return FALSE; 
	}

	oldFileSize = CYGetFileSize(lpFileName);
	pFileBuff = (char*)malloc(oldFileSize+sizeof(char));
	if(!pFileBuff)
	{
		return 0;
	}
	memset(pFileBuff,0,oldFileSize+sizeof(char));
	
    // 读取INI文件内容到pFileBuffer内存中    
    dwSize = CYReadIniFile(lpFileName,pFileBuff,oldFileSize);   
	
	memset(pszIniFileTemp,0,sizeof(pszIniFileTemp));
    strcpy(pszIniFileTemp,lpFileName);
    strcat(pszIniFileTemp,".tmp");
    
    fdOutputFile = fopen(pszIniFileTemp,"w");
	
    if (-1 == fdOutputFile)   
    {   
     //   perror("Could not open output file");   
        return FALSE;   
    }   
	
    // 将所有数据写入临时ini文件    
    for (;;)   
    {   
        // The bReadLine flag is used to not read a new line after we break    
        // out of the inner loop. We've already got a line to process.    
        if (bReadLine)   
        {   
            dwOffset = CYGetLineOriginal(pFileBuff, pszLine , dwOffset , dwSize );   
            if (!dwOffset)     
			{
				//printf("dwOffset = %d \n", dwOffset);
                break;
			}   
        }   
        bReadLine = TRUE;   
        // Skip past comments.    
        if (CYIsComment(pszLine))   
        {   
            CYWriteLine(fdOutputFile , pszLine);   
            continue;   
        }   
        // Found a section name.    
        if (CYIsSection(pszLine))   
        {   
            // It's the section we want.    
            if (CYIsSectionName(pszLine , lpAppName))   
            {   
                // 以下如果lpKeyName为NULL，删除整个段    
                if (lpKeyName)   
                    CYWriteLine(fdOutputFile , pszLine);   
				
                // Process the whole section.    
                while (0 != (dwOffset = CYGetLineOriginal( pFileBuff,pszLine , dwOffset , dwSize )))   
                {   
                    // Reached the end of the section.    
                    if (CYIsSection(pszLine))   
                    {   
                        bReadLine = FALSE;   
                        // This line will be written in the outer loop.    
                        break;   
                    }   
                    // When deleting a section, leave the comments intact.    
                    else if (CYIsComment(pszLine))   
                    {   
                        CYWriteLine(fdOutputFile , pszLine);   
                        continue;   
                    }   
                    // Got the value we want.    
                    if (!bWrote && CYIsKey(pszLine , lpKeyName, &pValue, &dwValLen))   
                    {   
                        bWrote = TRUE;   
                        // 如果lpString为NULL，删除健lpKeyName    
                        if(lpString)   
                            CYWriteValue(fdOutputFile , NULL, lpKeyName, lpString);   
                    }   
                    else   
                    {   
                        if (lpKeyName)   
                            CYWriteLine(fdOutputFile , pszLine);   
                    }   
					
                    if(dwOffset >= dwSize)   
                        break ;   
                }   
				
                // 如果在段名lpAppName下键名lpKeyName不存在，则新建键名lpKeyName和键值lpString    
                if (!bWrote)   
                {   
                    bWrote = TRUE;   
                    CYWriteValue(fdOutputFile, NULL, lpKeyName, lpString);   
                }   
            }   
            else   
                CYWriteLine(fdOutputFile , pszLine);   
        }   
        else   
            CYWriteLine(fdOutputFile , pszLine);   
		
        if(dwOffset ==0)    
            break;   
    }   
	
    // 如果指定的段名lpAppName不存在，则新建段名lpAppName及键名lpKeyName和键值lpString    
    if (!bWrote && lpKeyName && lpString)   
    {   
	//	printf("create key! lpAppName = %s, lpKeyName = %s , lpString =%s \n",lpAppName,lpKeyName,lpString);
        CYWriteValue(fdOutputFile , lpAppName, lpKeyName, lpString);   
    }   
	
    // 用临时ini文件覆盖原来的ini文件并删除临时ini文件    
    if (fdOutputFile)   
    {   
        fseek(fdOutputFile, 0, SEEK_END);   
        fclose(fdOutputFile);   
        CYCopyFile(pszIniFileTemp, lpFileName, FALSE);   
        if(remove(pszIniFileTemp) !=0)
		{
			perror("remove file fail ");
		}
		
    }   
	
    // 释放ReadIniFile函数的全局内存    
    if (NULL != pFileBuff)   
    {   
        free( pFileBuff ) ;   
        pFileBuff = NULL ;   
    }   
	
    return TRUE;   
}   

/************************************************************************  
**函数：WritePrivateProfileInt  
**功能：跨平台环境下，向指定INI文件指定段名写入整型数据  
**参数：参考WritePrivateProfileString函数  
**返回：  
		FALSE - fail  
		TRUE  - success  
**备注：  
************************************************************************/   
BOOL CYWritePrivateProfileInt(   
							const char* lpAppName,   
							const char* lpKeyName,   
							int     Value,   
							const char* lpFileName)   
{      
    char ValBuf[16]={0};       
	
    sprintf( ValBuf, "%d", Value);       
    return( CYWritePrivateProfileString(lpAppName, lpKeyName, ValBuf, lpFileName) );    
}   

/************************************************************************  
**函数：WriteValue  
**功能：向指定INI文件中写入段名、键名和键值  
**参数：  
		m_hOutput[in]  
		pointer to the handle of ini file.  
		pAppName[in]  
		Pointer to a null-terminated string containing the name of the section  
		in which data is written. If this parameter is NULL, the WriteValue  
		function just wirte the pKeyName and pString.  
		pKeyName[in]  
		Pointer to a null-terminated string containing the name of the key in  
		which data is writtern.   
		pString[in]  
		Pointer to a null-terminated string to be written to the file.   
**返回：NONE  
**备注：  
		1). 要成功写入INI文件，键名pKeyName和键值pString都不能为NULL。  
		2). 如果段名pAppName为NULL，则只写入键名pKeyName和键值pString。  
		3). 注意往INI文件写入字符串时，不要写入结束符。  
************************************************************************/   
void CYWriteValue(FILE* fdOutput, const char* pAppName, const char* pKeyName, const char* pString)   
{   
 
    if (pKeyName && pString)   
    {   
        if (pAppName)       // 写入段名    
        {   
			fwrite("[",sizeof(char), sizeof(char),fdOutput);   
			fwrite( pAppName,sizeof(char), strlen(pAppName)*sizeof(char),fdOutput);   
			fwrite( "]\r\n",sizeof(char) ,3*sizeof(char),fdOutput);   									
        }   
		// 写入健名和键值   
		fwrite( pKeyName,sizeof(char), strlen(pKeyName)*sizeof(char),fdOutput);   
		fwrite( "=", sizeof(char),sizeof(char),fdOutput);   
		fwrite( pString,sizeof(char) ,strlen(pString)*sizeof(char),fdOutput);   
		fwrite( "\r\n",sizeof(char)  ,2*sizeof(char),fdOutput);  
    }   
}   




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
**������CopyFile  
**���ܣ�The CopyFile function copies an existing file to a new file. 

**������  
		lpExistingFileName[in] -Pointer to a null-terminated string
							    that specifies the name of an existing file. 
		lpNewFileName[in]      - Pointer to a null-terminated string that specifies the name of the new file. 
		bFailIfExists[in]      - Specifies how this operation is to proceed if a file of the same name as that 
							      specified by lpNewFileName already exists.
								   If this parameter is TRUE and the new file already exists, the function fails. 
									If this parameter is FALSE and the new file already exists, 
									the function overwrites the existing file and succeeds. 
**���أ�  
		TRUE - success  
		FALSE - fail 
**��ע��
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
**������GetLine  
**���ܣ���ȡ��file�д�dwOffsetλ�ÿ�ʼ��һ�����ݲ����浽pLine��ͬʱ��ƫ����dwOffset  
		�Ƶ���һ������  
**������  
		pLine[out]   - ����һ������(������\r\n)  
		dwOffset[in] - Ҫ��ȡ����һ�еĿ�ʼλ��  
		dwSize[in]   - INI�ļ���С  
**���أ�  
		��ȷ - ��һ�����׵�λ��  
		���� - 0  
**��ע��
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
**������IsComment  
**���ܣ��ж��ǲ���ע����  
**������  
		pLine[in] - INI��һ������  
**���أ�  
		1 - ע����  
		0 - ����ע����  
**��ע��  
		1). ����Ҳ��Ϊע����  
************************************************************************/ 

BOOL CYIsComment(const char* pLine)
{
	if (!pLine || 0 == strlen(pLine) 
		|| ';' == *pLine || '#'== *pLine )   
        return TRUE;   
    return FALSE;  
}

/************************************************************************  
**������IsSection  
**���ܣ��ж��ǲ��Ƕ���  
**������  
		pLine[in] - INI��һ������  
**���أ�  
		1 - �Ƕ���  
		0 - ����  
************************************************************************/   
BOOL CYIsSection(const char* pLine)   
{   
    if (pLine && '[' == *pLine)   
        return TRUE;   
    return FALSE;   
}   

/************************************************************************  
**������IsSectionName  
**���ܣ��ж���INI�ļ���һ��(pLine)�ǲ���Ҫ�ҵĶ���(pSection)  
**������  
pLine[in]    - INI�ļ���һ������  
pSection[in] - Ҫ�ҵĶ���  
**���أ�  
1 - ��  
0 - ����  
**��ע��  
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
**������IsKey  
**���ܣ��ж�INI�ļ���һ�е������ǲ���Ҫ�ҵļ���,����ǲ���ȡ��ֵ  
**������  
		pLine[in] - INI�ļ�ĳ������  
		pKeyName[in] - ҪѰ�ҵļ���  
		pValue[out] - ��ֵ  
		dwValLen[out] - ��ֵpValue��С(in bytes)  
**���أ�  
		1 - �ǣ�ͬʱpValue���ؼ�ֵ  
		0 - ���ǣ�pValueΪNULL   
**��ע��  
************************************************************************/   
BOOL CYIsKey(char* pLine , const char* pKeyName, char** pValue, unsigned int* dwValLen )   
{   
    char* pEqual = NULL;   
    unsigned int length = 0, len = 0;   
	
    if(!pLine || !pKeyName)   
        return FALSE;   
	
    // pLine�ǲ���ע����    
    if (CYIsComment( pLine ))   
        return FALSE;   
	
    // Ѱ��"="��    
    pEqual = strchr(pLine, '=' );   
    if (!pEqual)   
        return FALSE;   
	
    // Ѱ�Ҽ������һ�ַ���λ��    
    //while (pEqual - 1 >= pLine && iswspace(*(pEqual-1)))   
	while(pEqual - 1 >= pLine && ((*(pEqual-1)) == "")) 
        --pEqual;   
    // Badly formed file.    
    if (pEqual - 1 < pLine)   
        return FALSE;   
	
    // ��������    
    length = pEqual - pLine;   
	
    len = strlen(pKeyName);   
    //if (len == length && 0 == _wcsnicmp(pLine, pKeyName, len))    
    if ( 0 == strncmp(pLine, pKeyName, len))   
    {   
        *pValue = strchr(pLine, '=' );   
        ++(*pValue);   
        *dwValLen = strlen(pLine) - ((*pValue) - pLine);   
		
        // ȥ��������"="�ź�����пո�    
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
**������ReadIniFile  
**���ܣ����ļ��������ļ����ݿ�����һ������pFileBuffer��   
**������  
		lpFileName[in] - INI�ļ��������û��·����Ĭ��·����ǰӦ�ó���·�� 
**���أ�  
		��0 - ��INI�ļ��Ĵ�С(in bytes)  
		0   - ʧ��  
**��ע��  

************************************************************************/   
int CYReadIniFile(const char* lpFileName,char * pFileBuffer ,int nBufferSize)   
{   
    FILE* fdFile =0; 
    BOOL  bRet = FALSE; // bUnicode - ��־INI�ļ��ǲ���Unicode�ļ�    
 	
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
**������GetString  
**���ܣ���INI�ļ�  
**������  
		lpAppName[in]         - �ֶ���  
		lpKeyName[in]         - ����  
		lpReturnedString[out] - ��ֵ  
		nSize[in]             - ��ֵ��������С(in characters )  
		lpFileName[in]        - ������INI�ļ���  
**���أ�Returns the number of bytes read.  
**��ע��  
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

	
    //���ļ������ļ����ݿ�����������pFileBuffer�У�����INI�ļ���С    
    if (0 == (nReadSize = CYReadIniFile(lpFileName,pFileBuff,nFileSize)))   
    {   
     //   printf("GetString, Could not ReadIniFile INI file: %s\n", lpFileName);   
        return 0;   
    }   
	
    cchCopied = 0;   
    while ( 0 != (dwOffset = CYGetLine( pFileBuff ,pLine , dwOffset , nReadSize )))   
    {   
        //RETAILMSG(1,(_T("%s\n"),szLine));    
        // �ǲ���ע����    
        if (CYIsComment(pLine))   
            continue;   
		
        // �ǲ��Ƕ���    
        if (CYIsSection(pLine))   
        {   
            // �ǲ�������Ҫ�ҵĶ���    
            if (CYIsSectionName(pLine,lpAppName))   
            {   
                // Ѱ������Ҫ�ļ���    
                while ( 0 != (dwOffset = CYGetLine( pFileBuff,pLine , dwOffset , nReadSize)))   
                {   
                    char* pValue=NULL;   
					
                    if (CYIsSection(pLine))   
                        break;
					// �ǲ���ע����    
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
**������GetPrivateProfileString  
**���ܣ���ƽ̨�¶�ȡINI�ļ���ĳ����/�����ļ�ֵ���ַ���  
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
**��ע��  
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
**������WriteLine  
**���ܣ����ļ�д��һ������(�����س����з�)  
**������  
		hOutput[in] - �Ѵ򿪵��ļ����  
		pLine[in]   - Ҫд���һ������  
**���أ�NONE  
**��ע��  
		1). д��һ�����ݣ�Ҳ����д����ĩ��"\r\n"�����ַ�  
		3). ע�ⲻҪ��������Ҳд���ļ�  
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
		1). �Ƚ�Ҫ�޸ĵ�INI�ļ���ȫ�����ݶ�ȡ��pFileBuffer��  
		2). ��pFileBuffer�ж�λ������Ҫ�޸ĵ�λ�ã����������ݺ������޸ĵ�����д��һ��ʱini�ļ�  
		3). �����ʱini�ļ�����ԭ����ini�ļ�����ɾ����ʱini�ļ�  
		4). ��Ҫ��API������  
		creat��lseek��close��remove��open��mmap 
		5). ���lpKeyName == NULL, ɾ��������, ���lpString == NULL, ɾ����  
************************************************************************/   
BOOL CYWritePrivateProfileString(   
							   const char* lpAppName,   
							   const char* lpKeyName,   
							   const char* lpString,   
							   const char* lpFileName)   
{   
    unsigned int dwSize, dwOffset;                // dwSize - ini�ļ���С, dwOffset - ƫ����    
    BOOL  bReadLine = TRUE;   
    BOOL  bWrote = FALSE;   
    char pszLine[MAX_LINE] = {0};         // �洢һ�е�����    
    char pszIniFileTemp[MAX_PATH] = {0};  // ��ʱini�ļ�������(����·��)    
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
	
    // ��ȡINI�ļ����ݵ�pFileBuffer�ڴ���    
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
	
    // ����������д����ʱini�ļ�    
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
                // �������lpKeyNameΪNULL��ɾ��������    
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
                        // ���lpStringΪNULL��ɾ����lpKeyName    
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
				
                // ����ڶ���lpAppName�¼���lpKeyName�����ڣ����½�����lpKeyName�ͼ�ֵlpString    
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
	
    // ���ָ���Ķ���lpAppName�����ڣ����½�����lpAppName������lpKeyName�ͼ�ֵlpString    
    if (!bWrote && lpKeyName && lpString)   
    {   
	//	printf("create key! lpAppName = %s, lpKeyName = %s , lpString =%s \n",lpAppName,lpKeyName,lpString);
        CYWriteValue(fdOutputFile , lpAppName, lpKeyName, lpString);   
    }   
	
    // ����ʱini�ļ�����ԭ����ini�ļ���ɾ����ʱini�ļ�    
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
	
    // �ͷ�ReadIniFile������ȫ���ڴ�    
    if (NULL != pFileBuff)   
    {   
        free( pFileBuff ) ;   
        pFileBuff = NULL ;   
    }   
	
    return TRUE;   
}   

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
							const char* lpFileName)   
{      
    char ValBuf[16]={0};       
	
    sprintf( ValBuf, "%d", Value);       
    return( CYWritePrivateProfileString(lpAppName, lpKeyName, ValBuf, lpFileName) );    
}   

/************************************************************************  
**������WriteValue  
**���ܣ���ָ��INI�ļ���д������������ͼ�ֵ  
**������  
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
**���أ�NONE  
**��ע��  
		1). Ҫ�ɹ�д��INI�ļ�������pKeyName�ͼ�ֵpString������ΪNULL��  
		2). �������pAppNameΪNULL����ֻд�����pKeyName�ͼ�ֵpString��  
		3). ע����INI�ļ�д���ַ���ʱ����Ҫд���������  
************************************************************************/   
void CYWriteValue(FILE* fdOutput, const char* pAppName, const char* pKeyName, const char* pString)   
{   
 
    if (pKeyName && pString)   
    {   
        if (pAppName)       // д�����    
        {   
			fwrite("[",sizeof(char), sizeof(char),fdOutput);   
			fwrite( pAppName,sizeof(char), strlen(pAppName)*sizeof(char),fdOutput);   
			fwrite( "]\r\n",sizeof(char) ,3*sizeof(char),fdOutput);   									
        }   
		// д�뽡���ͼ�ֵ   
		fwrite( pKeyName,sizeof(char), strlen(pKeyName)*sizeof(char),fdOutput);   
		fwrite( "=", sizeof(char),sizeof(char),fdOutput);   
		fwrite( pString,sizeof(char) ,strlen(pString)*sizeof(char),fdOutput);   
		fwrite( "\r\n",sizeof(char)  ,2*sizeof(char),fdOutput);  
    }   
}   


/** $Id$ $DateTime$
 *  @file  bsrLog.cpp
 *  @brief ʵ����־����
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <direct.h>
#include <Windows.h>
#include "bsrLog.h"

#pragma warning(disable: 4996)

/*****************************************************************************/
// д��־�Ĺ��ܺ���

struct LogHandle_st;

/// д��־��غ���
struct LogFunctions
{
	void (*fCreateLog)( LogHandle_st* );
	void (*fDestoryLog)( LogHandle_st* );
	void (*fPrintLog)( LogHandle_st*, const char*);
};

/// ��־����Ľṹ����
struct LogHandle_st
{
	FILE*		pfile;		/**< ΪNULLʱ��дOutputDebugString */
	char		szFName[MAX_PATH];
	unsigned short  level;
	unsigned short  mode;
	LogFunctions		function;
	CRITICAL_SECTION	cs;
};

/**
 * @fn void TraceLogCreate( LogHandle_st* pLog )
 * @brief      дOutputDebugString�Ĵ�����������OutputDebugString�ǲ���Ҫ�ģ�����ֱ�ӷ���NULL
 * @return void
 */
void
TraceLogCreate( LogHandle_st* pLog )
{
	return;
}

/**
 * @fn void TraceLogDestory( LogHandle_st* )
 * @brief      ��־�������������ٺ�������Ϊû�д���������������Ҳ��ֱ�ӷ���
 */
void 
TraceLogDestory( LogHandle_st* )
{
	return;
}

/**
 * @fn void TraceLogPrint( void*, const char* szLog )
 * @brief      д��־����
 * @param[in]  szLog ��־����
 */
void 
TraceLogPrint( LogHandle_st* pLog, const char* szLog )
{
	if ( pLog != NULL )
	{
		OutputDebugStringA( (char*)pLog->szFName );
		OutputDebugStringA( " " );
	}
	if ( szLog != NULL )
	{
		OutputDebugStringA( szLog );
	}
}

/**
 * @fn void FileLogCreate( LogHandle_st* pLog )
 * @brief     ������־�ļ����� 
 * @param[in]  pLog ��־���
 * @return void
 */
void FileLogCreate( LogHandle_st* pLog )
{
	if ( pLog->pfile )
	{
		pLog->function.fDestoryLog( pLog );
	}

	// �ļ���
	char path[MAX_PATH];
  char szDisk[4];
  char szPath[MAX_PATH];
  	
  ::GetModuleFileNameA( NULL, path, MAX_PATH );
  _splitpath( path, szDisk, szPath, NULL, NULL );
  sprintf( path, "%s%s\\log", szDisk, szPath );
	mkdir( path );
	sprintf( path, "%s\\%s.log", path, pLog->szFName );

	// �ļ��򿪷�ʽ
	char mode[4];
	if ( pLog->mode & LOGMODE_APPEND )
	{
		strcpy( mode, "at" );
	}
	else
	{
		strcpy( mode, "wt" );
	}

	pLog->pfile = fopen( path, mode );
	return;
}

/**
 * @fn void FileLogDestory( LogHandle_st* pLog )
 * @brief      �ر���־�ļ�
 * @param[in]  pLog ��־�ļ����
 */
void 
FileLogDestory( LogHandle_st* pLog )
{
	if ( pLog )
	{
		if ( pLog->pfile )
		{
			fclose( pLog->pfile );
			pLog->pfile = NULL;
		}
	}
}

/**
 * @fn void FileLogPrint( LogHandle_st* pLog, const char* szLog )
 * @brief      ��ӡ��־
 * @param[in]  pLog ��־�ļ����
 * @param[in]  szLog ��־����
 */
void 
FileLogPrint( LogHandle_st* pLog, const char* szLog )
{
	if ( NULL == pLog )
	{
		return;
	}

	if ( pLog->mode & LOGMODE_REOPEN )
	{
		pLog->function.fCreateLog( pLog );
	}

	if ( pLog->pfile )
	{
		fprintf( pLog->pfile, szLog );
	}	

	if ( pLog->mode & LOGMODE_REOPEN )
	{
		pLog->function.fDestoryLog( pLog );
	}	
}

/*****************************************************************************/
// ��־�Ľӿں���

/**
 * @fn LogHandle logCreate( const char* szFileName, unsigned short level, unsigned short mode )
 * @brief      ����־�ļ�
 * @param[in]  szModuleName ģ����
 * @param[in]  module ģ���ʶ
 * @return LogHandle
 * @retval  NULL ʧ��
 * @retval  ����ֵ �ɹ�
 */
LogHandle 
logCreate( const char* szFileName, unsigned short level, unsigned short mode )
{
	/// 1.������־���
	LogHandle_st* pLog = new LogHandle_st;

  do 
  {
	  if ( NULL == pLog )
	  {
		  break;
	  }
	  pLog->pfile =  NULL;
	  strncpy( pLog->szFName, szFileName, sizeof(pLog->szFName)-1 );
	  pLog->level = level;
	  pLog->mode = mode;
	  InitializeCriticalSection( &pLog->cs );

	  /// 3.�õ���־�ļ���ģʽ
	  if ( pLog->mode & LOGMODE_TOFILE )
	  {
		  pLog->function.fCreateLog = FileLogCreate;
		  pLog->function.fDestoryLog = FileLogDestory;
		  pLog->function.fPrintLog = FileLogPrint;
	  }
	  else
	  {
		  pLog->function.fCreateLog = TraceLogCreate;
		  pLog->function.fDestoryLog = TraceLogDestory;
		  pLog->function.fPrintLog = TraceLogPrint;
	  }

	  if ( pLog->mode & LOGMODE_REOPEN )	// ��������´�ģʽ����ǰ���ô�
	  {
		  break;
	  }

	  pLog->function.fCreateLog( pLog );
  } while (0);


	return (LogHandle)pLog;
}

/**
 * @fn void logDestory( LogHandle hLog )
 * @brief      ������־�������Դ
 * @param[in]  hLog ��־���
 */
void 
logDestory( LogHandle hLog )
{
	LogHandle_st* pLog = (LogHandle_st*)hLog;
	if ( pLog != NULL )
	{
		pLog->function.fDestoryLog( pLog );
		DeleteCriticalSection( &pLog->cs );
		delete pLog;		
	}
}

/**
 * @fn void logPrintBase( LogHandle hLog, int level, const char* fmt, va_list &arglist )
 * @brief      д��־�Ļ�������
 * @param[in]  hLog ��־���
 * @param[in]  level ��־����
 * @param[in]  fmt ��־��ʽ
 * @param[in]  arglist ��־����
 */
void 
logPrintBase( LogHandle hLog, int level, const char* fmt, va_list &arglist )
{
	LogHandle_st* pLog = (LogHandle_st*)hLog;
	if ( NULL == pLog )
	{
		return;
	}

	EnterCriticalSection( &pLog->cs );

	do 
	{
		if ( pLog->level & level )	// ����д��־
		{
			char szLog[MAX_LOG_LINE_SIZE];

			time_t t = time(NULL);
			struct tm* ptm = localtime(&t);
			if ( NULL == ptm )
			{
				break;
			}

			int len = 0;
			/// ������
			if ( pLog->mode & LOGMODE_DATE )
			{
				len = sprintf( szLog, "%.04d-%.02d-%.02d ", 1900 + ptm->tm_year, 1 + ptm->tm_mon, ptm->tm_mday );
			}

			/// ��ʱ��
			if ( pLog->mode & LOGMODE_TIME )
			{
				len += sprintf( &szLog[len], "%.02d:%.02d:%.02d ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
			}

			/// �� �� ��
			len += sprintf( &szLog[len], ": " );

			/// ����־����
			len += vsnprintf( &szLog[len], MAX_LOG_LINE_SIZE - len - 2, fmt, arglist );

			/// �ӻ��з�
			sprintf( &szLog[len], "\n" );

			pLog->function.fPrintLog( pLog, szLog );
		}

	} while (0);

	LeaveCriticalSection( &pLog->cs );
}

/**
 * @fn void logPrint( LogHandle hLog, int level, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  level ��־����
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint( LogHandle hLog, int level, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );
	
	logPrintBase( hLog, level, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void logPrint1( LogHandle hLog, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint1( LogHandle hLog, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );

	logPrintBase( hLog, LOGLEVEL_1, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void logPrint2( LogHandle hLog, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint2( LogHandle hLog, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );

	logPrintBase( hLog, LOGLEVEL_2, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void logPrint3( LogHandle hLog, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint3( LogHandle hLog, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );

	logPrintBase( hLog, LOGLEVEL_3, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void logPrint4( LogHandle hLog, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint4( LogHandle hLog, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );

	logPrintBase( hLog, LOGLEVEL_4, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void logPrint5( LogHandle hLog, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint5( LogHandle hLog, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );

	logPrintBase( hLog, LOGLEVEL_5, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void logPrint6( LogHandle hLog, const char* fmt, ... )
 * @brief      д��־����
 * @param[in]  hLog ��־���
 * @param[in]  fmt ��־��ʽ
 */
void 
logPrint6( LogHandle hLog, const char* fmt, ... )
{
	va_list arglist;
	va_start( arglist, fmt );

	logPrintBase( hLog, LOGLEVEL_6, fmt, arglist );

	va_end( arglist );
}

/**
 * @fn void log( const char* fmt, ... )
 * @brief      
 * @param[in]  
 * @param[out] 
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
void log( const char* fmt, ... )
{
#ifdef _DEBUG
	char szLog[1024];

	FILE* pf = fopen( "d:\\tst.log", "at" );
	if ( NULL == pf )
	{
		return;
	}

	va_list arglist;
	va_start( arglist, fmt );

	vsnprintf( szLog, sizeof(szLog), fmt, arglist );

	va_end( arglist );

	fprintf( pf, "%s\n", szLog ); 
	fclose(pf);
#endif
}

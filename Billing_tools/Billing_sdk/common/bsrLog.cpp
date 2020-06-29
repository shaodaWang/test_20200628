/** $Id$ $DateTime$
 *  @file  bsrLog.cpp
 *  @brief 实现日志功能
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
// 写日志的功能函数

struct LogHandle_st;

/// 写日志相关函数
struct LogFunctions
{
	void (*fCreateLog)( LogHandle_st* );
	void (*fDestoryLog)( LogHandle_st* );
	void (*fPrintLog)( LogHandle_st*, const char*);
};

/// 日志句柄的结构定义
struct LogHandle_st
{
	FILE*		pfile;		/**< 为NULL时，写OutputDebugString */
	char		szFName[MAX_PATH];
	unsigned short  level;
	unsigned short  mode;
	LogFunctions		function;
	CRITICAL_SECTION	cs;
};

/**
 * @fn void TraceLogCreate( LogHandle_st* pLog )
 * @brief      写OutputDebugString的创建函数，但OutputDebugString是不需要的，所以直接返回NULL
 * @return void
 */
void
TraceLogCreate( LogHandle_st* pLog )
{
	return;
}

/**
 * @fn void TraceLogDestory( LogHandle_st* )
 * @brief      日志操作函数的销毁函数，因为没有创建任务东西，所以也是直接返回
 */
void 
TraceLogDestory( LogHandle_st* )
{
	return;
}

/**
 * @fn void TraceLogPrint( void*, const char* szLog )
 * @brief      写日志函数
 * @param[in]  szLog 日志内容
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
 * @brief     创建日志文件函数 
 * @param[in]  pLog 日志句柄
 * @return void
 */
void FileLogCreate( LogHandle_st* pLog )
{
	if ( pLog->pfile )
	{
		pLog->function.fDestoryLog( pLog );
	}

	// 文件名
	char path[MAX_PATH];
  char szDisk[4];
  char szPath[MAX_PATH];
  	
  ::GetModuleFileNameA( NULL, path, MAX_PATH );
  _splitpath( path, szDisk, szPath, NULL, NULL );
  sprintf( path, "%s%s\\log", szDisk, szPath );
	mkdir( path );
	sprintf( path, "%s\\%s.log", path, pLog->szFName );

	// 文件打开方式
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
 * @brief      关闭日志文件
 * @param[in]  pLog 日志文件句柄
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
 * @brief      打印日志
 * @param[in]  pLog 日志文件句柄
 * @param[in]  szLog 日志内容
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
// 日志的接口函数

/**
 * @fn LogHandle logCreate( const char* szFileName, unsigned short level, unsigned short mode )
 * @brief      打开日志文件
 * @param[in]  szModuleName 模块名
 * @param[in]  module 模块标识
 * @return LogHandle
 * @retval  NULL 失败
 * @retval  其它值 成功
 */
LogHandle 
logCreate( const char* szFileName, unsigned short level, unsigned short mode )
{
	/// 1.分配日志句柄
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

	  /// 3.得到日志文件打开模式
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

	  if ( pLog->mode & LOGMODE_REOPEN )	// 如果是重新打开模式，则当前不用打开
	  {
		  break;
	  }

	  pLog->function.fCreateLog( pLog );
  } while (0);


	return (LogHandle)pLog;
}

/**
 * @fn void logDestory( LogHandle hLog )
 * @brief      销毁日志句柄及资源
 * @param[in]  hLog 日志句柄
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
 * @brief      写日志的基本函数
 * @param[in]  hLog 日志句柄
 * @param[in]  level 日志级别
 * @param[in]  fmt 日志格式
 * @param[in]  arglist 日志内容
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
		if ( pLog->level & level )	// 可以写日志
		{
			char szLog[MAX_LOG_LINE_SIZE];

			time_t t = time(NULL);
			struct tm* ptm = localtime(&t);
			if ( NULL == ptm )
			{
				break;
			}

			int len = 0;
			/// 加日期
			if ( pLog->mode & LOGMODE_DATE )
			{
				len = sprintf( szLog, "%.04d-%.02d-%.02d ", 1900 + ptm->tm_year, 1 + ptm->tm_mon, ptm->tm_mday );
			}

			/// 加时间
			if ( pLog->mode & LOGMODE_TIME )
			{
				len += sprintf( &szLog[len], "%.02d:%.02d:%.02d ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
			}

			/// 加 ： 号
			len += sprintf( &szLog[len], ": " );

			/// 加日志内容
			len += vsnprintf( &szLog[len], MAX_LOG_LINE_SIZE - len - 2, fmt, arglist );

			/// 加换行符
			sprintf( &szLog[len], "\n" );

			pLog->function.fPrintLog( pLog, szLog );
		}

	} while (0);

	LeaveCriticalSection( &pLog->cs );
}

/**
 * @fn void logPrint( LogHandle hLog, int level, const char* fmt, ... )
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  level 日志级别
 * @param[in]  fmt 日志格式
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
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  fmt 日志格式
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
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  fmt 日志格式
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
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  fmt 日志格式
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
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  fmt 日志格式
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
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  fmt 日志格式
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
 * @brief      写日志函数
 * @param[in]  hLog 日志句柄
 * @param[in]  fmt 日志格式
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
 * @retval  0 成功
 * @retval  其它值 错误码
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

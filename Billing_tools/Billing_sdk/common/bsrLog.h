/** $Id$ $DateTime$
 *  @file  bsrLog.h
 *  @brief 本日志机制，可以实现：可以选择设定模块、级别、日志模块，记日志时，可以选择是否记时间等
 *  @version 0.0.1
 *  @since 0.0.1
 */


#ifndef __BSR_LOG_H__
#define __BSR_LOG_H__

/** @defgroup grLog Log_Module
 *  @{
 */

typedef void*		LogHandle;

/// 日志的总控制宏，其它的控制宏都要用它。
#ifdef _DEBUG
#define BSRLOG(x)	//	x
#else
#define BSRLOG(x)		
#endif

#define MAX_LOG_LINE_SIZE		1024

typedef enum{
	LOGMODE_TOFILE		= 0x01,		/**< 写到文件中 */
	LOGMODE_APPEND		= 0x02,		/**< 以添加的方式打开日志文件 */
	LOGMODE_DATE			= 0x04,		/**< 记文本日志时，记录日期 */
	LOGMODE_TIME			= 0x08,		/**< 记文本日志时，记录时间 */
	LOGMODE_REOPEN		= 0x12,		/**< 每次记日志，都重新关闭打开(0x10)， 但要是REOPEN，则必须是APPEND，所以是(0x12)*/
}ELOG_MODE;

#define LOGMODE_DEFAULT	(LOGMODE_TOFILE | LOGMODE_TIME)

typedef enum{
	LOGLEVEL_1	= 0x01,
	LOGLEVEL_2	= 0x02,
	LOGLEVEL_3	= 0x04,
	LOGLEVEL_4	= 0x08,
	LOGLEVEL_5	= 0x10,
	LOGLEVEL_6	= 0x20,
	LOGLEVEL_7  = 0x40,
	LOGLEVEL_8  = 0x80
}ELOG_LEVEL;

LogHandle logCreate( const char* szFileName, unsigned short level, unsigned short mode );
void logDestory( LogHandle hLog );

void logPrint( LogHandle hLog, int level, const char* fmt, ... );
void logPrint1( LogHandle hLog, const char* fmt, ... );
void logPrint2( LogHandle hLog, const char* fmt, ... );
void logPrint3( LogHandle hLog, const char* fmt, ... );
void logPrint4( LogHandle hLog, const char* fmt, ... );
void logPrint5( LogHandle hLog, const char* fmt, ... );
void logPrint6( LogHandle hLog, const char* fmt, ... );

void log( const char* fmt, ... );

/** @}*/	// end of grLog

#endif	// end of __BSR_LOG_H__

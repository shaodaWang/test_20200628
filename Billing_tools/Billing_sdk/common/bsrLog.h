/** $Id$ $DateTime$
 *  @file  bsrLog.h
 *  @brief ����־���ƣ�����ʵ�֣�����ѡ���趨ģ�顢������־ģ�飬����־ʱ������ѡ���Ƿ��ʱ���
 *  @version 0.0.1
 *  @since 0.0.1
 */


#ifndef __BSR_LOG_H__
#define __BSR_LOG_H__

/** @defgroup grLog Log_Module
 *  @{
 */

typedef void*		LogHandle;

/// ��־���ܿ��ƺ꣬�����Ŀ��ƺ궼Ҫ������
#ifdef _DEBUG
#define BSRLOG(x)	//	x
#else
#define BSRLOG(x)		
#endif

#define MAX_LOG_LINE_SIZE		1024

typedef enum{
	LOGMODE_TOFILE		= 0x01,		/**< д���ļ��� */
	LOGMODE_APPEND		= 0x02,		/**< ����ӵķ�ʽ����־�ļ� */
	LOGMODE_DATE			= 0x04,		/**< ���ı���־ʱ����¼���� */
	LOGMODE_TIME			= 0x08,		/**< ���ı���־ʱ����¼ʱ�� */
	LOGMODE_REOPEN		= 0x12,		/**< ÿ�μ���־�������¹رմ�(0x10)�� ��Ҫ��REOPEN���������APPEND��������(0x12)*/
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

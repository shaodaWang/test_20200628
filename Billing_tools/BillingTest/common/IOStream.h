/********************************************************************
    created date:  2009/09/16
    filename:      /src/PFBilling/Common/IOStream.h
    author:        PFBilling

    description:   定义收发包用到的IO流
********************************************************************/
#ifndef __IO_STREAM_H__
#define __IO_STREAM_H__

#include "stdafx.h"

class InputStream
{
public:
    InputStream(char* pbuf) : m_pbuf(pbuf){};
    ~InputStream(){};

public:
    void Read(INT &val)
    {
        memcpy(&val, m_pbuf, sizeof(INT));
        val = ntohl(val);
        m_pbuf += sizeof(INT);
    };

    void Read(UINT &val)
    {
        memcpy(&val, m_pbuf, sizeof(UINT));
        val = ntohl(val);
        m_pbuf += sizeof(UINT);
    };

	void Read(SHORT &val)
    {
        memcpy(&val, m_pbuf, sizeof(SHORT));
        val = ntohs(val);
        m_pbuf += sizeof(SHORT);
    };

    void Read(USHORT &val)
    {
        memcpy(&val, m_pbuf, sizeof(USHORT));
        val = ntohs(val);
        m_pbuf += sizeof(USHORT);
    };

    void Read(long long &val)
    {
        memcpy(&val, m_pbuf, sizeof(long long));
        INT  tmpH;
		UINT tmpL;
        memcpy(&tmpH, &val, sizeof(INT));
        memcpy(&tmpL, (UINT*)(&val)+1, sizeof(UINT));

        val = 0;

        val = ntohl(tmpH);
        val = val << 32;
        val += ntohl(tmpL);

        m_pbuf += sizeof(long long);
    };

	void Read(unsigned long long &val)
	{
		memcpy(&val, m_pbuf, sizeof(unsigned long long));
		UINT tmpL, tmpH;
		memcpy(&tmpH, &val, sizeof(UINT));
		memcpy(&tmpL, (UINT*)(&val)+1, sizeof(UINT));

		val = 0;

		val = ntohl(tmpH);
		val = val << 32;
		val += ntohl(tmpL);

		m_pbuf += sizeof(unsigned long long);
	};

    void Read(UCHAR &val)
    {
        memcpy(&val, m_pbuf, sizeof(UCHAR));
        m_pbuf += sizeof(UCHAR);
    };

	void Read(char &val)
    {
        memcpy(&val, m_pbuf, sizeof(char));
        m_pbuf += sizeof(char);
    };

	void Read(char* val, int size)
    {
        memcpy(val, m_pbuf, size);
        m_pbuf += size;
    };

public:
	char * GetBuffer(void)
	{
		return m_pbuf;
	}

private:
    char *m_pbuf;
};


class OutputStream
{
public:
    OutputStream(char* pbuf) : m_pbuf(pbuf), m_nSize(0){};
    ~OutputStream(){};

public:
    void Write(INT val)
    {
        val = htonl(val);
        memcpy(m_pbuf, &val, sizeof(INT));
        m_pbuf += sizeof(INT);
		m_nSize+= sizeof(INT);
    };

	void Write(UINT val)
	{
		val = htonl(val);
		memcpy(m_pbuf, &val, sizeof(UINT));
		m_pbuf += sizeof(UINT);
		m_nSize += sizeof(UINT);
	};

	void Write(SHORT val)
    {
        val = htons(val);
        memcpy(m_pbuf, &val, sizeof(SHORT));
        m_pbuf += sizeof(SHORT);
		m_nSize += sizeof(SHORT);
    };

    void Write(USHORT val)
    {
        val = htons(val);
        memcpy(m_pbuf, &val, sizeof(USHORT));
        m_pbuf += sizeof(USHORT);
		m_nSize += sizeof(USHORT);
    };

	void Write(CHAR val)
	{
		memcpy(m_pbuf, &val, sizeof(CHAR));
		m_pbuf += sizeof(CHAR);
		m_nSize += sizeof(CHAR);
	};

	void Write(UCHAR val)
	{
		memcpy(m_pbuf, &val, sizeof(UCHAR));
		m_pbuf += sizeof(UCHAR);
		m_nSize += sizeof(UCHAR);
	}
	
	void Write(char* val, int size) 
    {
        memcpy(m_pbuf, val, size);
        m_pbuf += size;
		m_nSize += size;
    };    

    void Write(const char* val, int size) 
    {
        memcpy(m_pbuf, val, size);
        m_pbuf += size;
		m_nSize += size;
    };

    void Write(long long val)
    {
        int tmpL, tmpH;
        memcpy(&tmpH, &val, sizeof(int));
        memcpy(&tmpL, (int*)(&val)+1, sizeof(int));

        val = 0;

        val = htonl(tmpH);
        val = val << 32;
        val += htonl(tmpL);

        memcpy(m_pbuf, &val, sizeof(long long));
        m_pbuf += sizeof(long long);
		m_nSize += sizeof(long long);
    };

	void Write(unsigned long long val)
	{
		UINT tmpL, tmpH;
		memcpy(&tmpH, &val, sizeof(UINT));
		memcpy(&tmpL, (UINT*)(&val)+1, sizeof(UINT));

		val = 0;

		val = htonl(tmpH);
		val = val << 32;
		val += htonl(tmpL);

		memcpy(m_pbuf, &val, sizeof(unsigned long long));
		m_pbuf += sizeof(unsigned long long);
		m_nSize += sizeof(unsigned long long);
	};

	//这是为了最后，填写长度那个字段（变长）
	USHORT WritePacketSize(UCHAR* pbuf)
	{
		USHORT uSize = (char*)m_pbuf - (char*)pbuf - 6;
		uSize = htons(uSize);
		memcpy(pbuf + 2, &uSize, sizeof(USHORT));
		return ntohs(uSize);
	}

public:
	char * GetBuffer(void)
	{
		return m_pbuf;
	}
	unsigned int GetBufferSize() {
		return m_nSize;
	}
private:
    char *m_pbuf;
	unsigned int m_nSize;
};
#endif//__IO_STREAM_H__


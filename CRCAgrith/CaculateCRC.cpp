#include "StdAfx.h"
#include "CaculateCRC.h"

CaculateCRC::CaculateCRC(void)
{
}

CaculateCRC::~CaculateCRC(void)
{
}


unsigned long CaculateCRC::Reflect(unsigned long ref, char ch)
{
    unsigned long value = 0;
	
    // Swap bit 0 for bit 7, bit 1 for bit 6, etc.
    for(int i = 1; i < (ch + 1); i++)
    {
        if (ref & 1)
            value |= 1 << (ch - i);
        ref >>= 1;
    }
    return value;
}

void CaculateCRC::initTable()
{
	unsigned long ulPolynomial = 0x04C11DB7;
	
    // 256 values representing ASCII character codes.
    for (int i = 0; i <= 0xFF; i++)
    {
        m_Table[i] = Reflect(i, 8) << 24;
        for (int j = 0; j < 8; j++)
            m_Table[i] = (m_Table[i] << 1) ^ (m_Table[i] & (1 << 31) ? ulPolynomial : 0);
        m_Table[i] = Reflect(m_Table[i], 32);
    }
}
void CaculateCRC::TurnHexToByte(unsigned char *Obj, unsigned char *Source, unsigned int nLen)
{
	//字节串转为字节串内码Hex值的字符串
    char ch;
	unsigned int i=0;
	for(i=0;i<nLen;i++)
	{
		ch=Source[i]&0x0f;
		if(ch<=9 && ch>=0)
			Obj[i*2+1]=ch+0x30;
		else
			Obj[i*2+1]=ch+0x41-10;
		ch=(Source[i]&0xf0)>>4;
		if(ch<=9 && ch>=0)
			Obj[i*2]=ch+0x30;
		else
			Obj[i*2]=ch+0x41-10;
	}
	Obj[i*2]='\0';
	
}

unsigned long CaculateCRC::CRC32(LPBYTE pucData, UINT nDataLen/*, LPBYTE output*/)
{
	int i = 0;
	unsigned char* pData = new unsigned char[nDataLen * 2 + 1];
	unsigned long DataLen = nDataLen * 2;
	
	TurnHexToByte(pData, (unsigned char*)pucData, nDataLen);
	
	unsigned long CRCResult = 0xFFFFFFFF;
		
	while (DataLen--)
        CRCResult = (CRCResult >> 8) ^ m_Table[(CRCResult & 0xFF) ^ pData[i++]];
	
	CRCResult ^= 0xFFFFFFFF;
	
	/*
	output[0]=(BYTE)(CRCResult>>24);
    output[1]=(BYTE)(CRCResult>>16);
	output[2]=(BYTE)(CRCResult>>8);
	output[3]=(BYTE)CRCResult;
	*/

	if (NULL != pData)
	{
		delete []pData;
		pData = NULL;
	}
	return CRCResult;
}

int CaculateCRC::TurnByteToHex(unsigned char *Obj, unsigned char *Source,unsigned int nLen)
{
	//字节串内码Hex值的字符串转为字节串
	unsigned char ch=0x00;
	unsigned int i = 0;
	for(i=0;i<nLen;i++)
	{
		if(Source[i*2]>=0x61 && Source[i*2]<=0x66)
		{
			ch=(unsigned char)(Source[2*i]+10-0x61);
			Obj[i]=(unsigned char)(ch<<4);
		}
		else if(Source[i*2]>=0x30 && Source[i*2]<=0x39)
		{
			ch=(unsigned char)(Source[2*i]-0x30);
			Obj[i]=(unsigned char)(ch<<4);
		}
		else if(Source[i*2]>=0x41 && Source[i*2]<=0x46)
		{
			ch=(unsigned char)(Source[2*i]+10-0x41);
			Obj[i]=(unsigned char)(ch<<4);
		}
		else
			return 0;
		
		if(Source[2*i+1]>=0x61 && Source[2*i+1]<=0x66)
		{
			ch=(unsigned char)(Source[2*i+1]+10-0x61);
			ch&=0x0f;
			Obj[i]|=ch;
		}
		else if(Source[2*i+1]>=0x30 && Source[2*i+1]<=0x39)
		{
			ch=(unsigned char)(Source[2*i+1]-0x30);
			ch&=0x0f;
			Obj[i]|=ch;
		}
		else if(Source[2*i+1]>=0x41 && Source[2*i+1]<=0x46)
		{
			ch=(unsigned char)(Source[2*i+1]+10-0x41);
			ch&=0x0f;
			Obj[i]|=ch;
		}
		else 
			return 0;
	}
	return 1;
}

void CaculateCRC::AddPause(unsigned char *Obj, unsigned char *Source, unsigned int nLen)
{
	int j=0;
	memset(Obj, 0x00, sizeof(unsigned char )*1024);
	for(int i=0; i<nLen; i++)
	{
		Obj[j] = Source[i];
		i++;
		Obj[j+1] = Source[i];
		j++;
		if(i != nLen - 1)
		{
			Obj[j+1] = ' ';
		}
		j = j + 2;
	}
}
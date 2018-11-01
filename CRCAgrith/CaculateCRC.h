#pragma once

typedef unsigned char * LPBYTE;
typedef unsigned int UINT;
typedef unsigned char BYTE;
class CaculateCRC
{
public:
	CaculateCRC(void);
	unsigned long Reflect(unsigned long ref, char ch);
	void initTable();
	unsigned long CRC32(LPBYTE input, UINT len/*, LPBYTE output*/);//CRC32
	void TurnHexToByte(unsigned char *Obj, unsigned char *Source, unsigned int nLen);
	int TurnByteToHex(unsigned char *Obj, unsigned char *Source,unsigned int nLen);
	void AddPause(unsigned char *Obj, unsigned char *Source, unsigned int nLen);
public:
	~CaculateCRC(void);
public:
	unsigned long m_Table[256];
};

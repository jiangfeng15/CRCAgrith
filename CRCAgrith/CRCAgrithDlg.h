// CRCAgrithDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CCRCAgrithDlg �Ի���
class CCRCAgrithDlg : public CDialog
{
// ����
public:
	CCRCAgrithDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CRCAGRITH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CString m_szInput;

	CComboBox m_comboxCrc;

	CString m_szCrcOutput;
public:
	afx_msg void OnBnClickedCancel();
	unsigned short CRC16CCITT(unsigned char *message, unsigned int len); 
	void TurnHexToByte(unsigned char *Obj, unsigned char *Source, unsigned int Len);
	int TurnByteToHex(unsigned char *Obj, unsigned char *Source,unsigned int nLen);

public:
	CString m_szRes;
};

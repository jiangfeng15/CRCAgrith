// CRCAgrithDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CCRCAgrithDlg 对话框
class CCRCAgrithDlg : public CDialog
{
// 构造
public:
	CCRCAgrithDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CRCAGRITH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

// CRCAgrithDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CRCAgrith.h"
#include "CRCAgrithDlg.h"
#include "CaculateCRC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCRCAgrithDlg 对话框




CCRCAgrithDlg::CCRCAgrithDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCRCAgrithDlg::IDD, pParent)
	, m_szInput(_T(""))
	, m_szCrcOutput(_T(""))
	, m_szRes(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCRCAgrithDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_szInput);
	DDX_Control(pDX, IDC_COMBO1, m_comboxCrc);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_szCrcOutput);
	DDX_Text(pDX, IDC_EDIT_RES, m_szRes);
}

BEGIN_MESSAGE_MAP(CCRCAgrithDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCRCAgrithDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCRCAgrithDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCRCAgrithDlg 消息处理程序

BOOL CCRCAgrithDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MINIMIZE);
	
	// TODO: 在此添加额外的初始化代码
	m_comboxCrc.ResetContent();
	m_comboxCrc.InsertString(0,_T("CRC16CCITT"));
	m_comboxCrc.InsertString(1,_T("CRC32"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCRCAgrithDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCRCAgrithDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCRCAgrithDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCRCAgrithDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	unsigned short crc16res;
	CString szCRCType(_T(""));
	int nSelType, nInput;
	nSelType = m_comboxCrc.GetCurSel();
	if(nSelType == CB_ERR)
	{
		AfxMessageBox(_T("请选择CRC算法！"));
		return ;
	}
	m_comboxCrc.GetLBText(nSelType, szCRCType);
	unsigned char cInBuff[1024]={0x00}, cTemp[1024]={0x00};
	m_szInput.Remove(_T(' '));
	nInput = m_szInput.GetLength();
	if(nInput % 2 != 0)
	{
		AfxMessageBox(_T("输入的字节码长度不正确，请检查！"));
		return ;
	}
	strcpy((char *)cTemp, (CStringA)m_szInput);
	TurnByteToHex(cInBuff, cTemp, nInput/2);
	if(szCRCType == _T("CRC16CCITT"))
	{
		crc16res = CRC16CCITT(cInBuff, nInput/2);
		m_szCrcOutput.Format(_T("%04x"), crc16res);
	}
	else if(szCRCType == _T("CRC32"))
	{
		CaculateCRC clsCaculateCRC;
		unsigned long crc32res;
		clsCaculateCRC.initTable();
		crc32res = clsCaculateCRC.CRC32(cInBuff, nInput/2);
		m_szCrcOutput.Format(_T("%08x"), crc32res);
	}
	else
	{
	}
	m_szCrcOutput.MakeUpper();
	m_szRes = m_szInput + _T(" ") + m_szCrcOutput;
	UpdateData(FALSE);
	//OnOK();
}

void CCRCAgrithDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
 
void CCRCAgrithDlg::TurnHexToByte(unsigned char *Obj, unsigned char *Source, unsigned int Len)
{
	//字节串转为字节串内码Hex值的字符串
    char ch;
	unsigned int i=0;
	for(i=0;i<Len;i++)
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


int CCRCAgrithDlg::TurnByteToHex(unsigned char *Obj, unsigned char *Source,unsigned int nLen)
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

unsigned short CCRCAgrithDlg::CRC16CCITT(unsigned char *message, unsigned int len) 
{
    int i, j;
    unsigned short crc_reg = 0;
    unsigned short current;
        
    for (i = 0; i < len; i++) 
    {
        current = message[i];
        for (j = 0; j < 8; j++) 
        { 
            if ((crc_reg ^ current) & 0x0001)
                crc_reg = (crc_reg >> 1) ^ 0x8408;
            else 
                crc_reg >>= 1; 
            current >>= 1;            
        }
    }
    return crc_reg;
}

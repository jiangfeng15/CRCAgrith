// CRCAgrithDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CRCAgrith.h"
#include "CRCAgrithDlg.h"
#include "CaculateCRC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCRCAgrithDlg �Ի���




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


// CCRCAgrithDlg ��Ϣ�������

BOOL CCRCAgrithDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MINIMIZE);
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_comboxCrc.ResetContent();
	m_comboxCrc.InsertString(0,_T("CRC16CCITT"));
	m_comboxCrc.InsertString(1,_T("CRC32"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCRCAgrithDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CCRCAgrithDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCRCAgrithDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	unsigned short crc16res;
	CString szCRCType(_T(""));
	int nSelType, nInput;
	nSelType = m_comboxCrc.GetCurSel();
	if(nSelType == CB_ERR)
	{
		AfxMessageBox(_T("��ѡ��CRC�㷨��"));
		return ;
	}
	m_comboxCrc.GetLBText(nSelType, szCRCType);
	unsigned char cInBuff[1024]={0x00}, cTemp[1024]={0x00};
	m_szInput.Remove(_T(' '));
	nInput = m_szInput.GetLength();
	if(nInput % 2 != 0)
	{
		AfxMessageBox(_T("������ֽ��볤�Ȳ���ȷ�����飡"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
 
void CCRCAgrithDlg::TurnHexToByte(unsigned char *Obj, unsigned char *Source, unsigned int Len)
{
	//�ֽڴ�תΪ�ֽڴ�����Hexֵ���ַ���
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
	//�ֽڴ�����Hexֵ���ַ���תΪ�ֽڴ�
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

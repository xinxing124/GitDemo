﻿
// GitDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GitDemo.h"
#include "GitDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGitDemoDlg 对话框




CGitDemoDlg::CGitDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGitDemoDlg::IDD, pParent)
	, m_dbSpeed(0)
	, m_dbVoltage_1(0)
	, m_dbVoltage_2(0)
	, m_ulTriggerNumber_1(0)
	,m_pfLog_1(NULL)
	, m_ulSaveNumber_1(0)
	, m_bDirectoryExists_1(false)
	, m_TriggerNumber_1(0)
	, m_SaveNumber_1(0)
	//,m_strLeftPath("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGitDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_WND1, m_ImageWnd_1);
	DDX_Text(pDX, IDC_EDIT3, m_dbSpeed);
	DDX_Text(pDX, IDC_EDIT1, m_dbVoltage_1);
	DDX_Text(pDX, IDC_EDIT2, m_dbVoltage_2);
	DDX_Text(pDX, IDC_EDIT4, m_TriggerNumber_1);
	DDX_Text(pDX, IDC_EDIT5, m_SaveNumber_1);
}

BEGIN_MESSAGE_MAP(CGitDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveData)
	//ON_MESSAGE(WM_COMM_RXCHAR, &CGitDemoDlg::OnReceiveData)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CGitDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGitDemoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGitDemoDlg 消息处理程序

BOOL CGitDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: 在此添加额外的初始化代码
	struct DeviceList
	{
		int Index;
		char SN[128];
	};
	DeviceList dl[3];
	int iServerCount=SapManager::GetServerCount();

	for(int i=0;i<iServerCount;i++)
	{
		int iAcqDeviceNum=SapManager::GetResourceCount(i,SapManager::ResourceAcqDevice);
		int iAcqNum=SapManager::GetResourceCount(i,SapManager::ResourceAcq);
		if(iAcqDeviceNum>0||iAcqNum>0)
		{
			//char ServerName[128];
			//char DeviceName[128];
			//memset(ServerName,0x00,128);
			//SapManager::GetServerName(i,ServerName,sizeof(ServerName));
			//SapManager::GetServerSerialNumber(i,ServerName);
			SapManager::GetServerSerialNumber(i,dl[i].SN);
			dl[i].Index=i;
		}
	}
	
		//m_AcqDevice_1	= new SapAcqDevice(SapLocation(dl[1].Index,0),"C:\\T_Linea_M2048-7um_12080124_3M.ccf");
	m_AcqDevice_1	= new SapAcqDevice(SapLocation(dl[1].Index,0),"C:\\T_Linea_M2048-7um_Default_Default.ccf");
		//m_AcqDevice_1	= new SapAcqDevice(SapLocation(dl[1].Index,0),"C:\\Program Files\\Teledyne DALSA\\Sapera\\Examples\\NET\\GitDemo\\GitDemo\\T_Linea_M2048-7um_12080124_3M.ccf");
		//m_AcqDevice_1	= new SapAcqDevice(SapLocation(dl[1].Index,0),m_configFileName_1);


	//m_AcqDevice_1	= new SapAcqDevice(SapLocation(m_serverName_1,m_resourceIndex_1),m_configFileName_1);
	m_Buffers_1	= new SapBufferWithTrash(2, m_AcqDevice_1);
	//m_Bayer     = new SapColorConversion(m_Buffers_1);
	m_Xfer_1		= new SapAcqDeviceToBuf(m_AcqDevice_1, m_Buffers_1, XferCallback_1, this);
	m_View_1      = new SapView(m_Buffers_1);
	
    // Attach sapview to image viewer 1
    m_ImageWnd_1.AttachSapView(m_View_1);

   if (!CreateObjects_1()) { EndDialog(TRUE); return FALSE; }

	
   //m_ImageWnd_1.AttachEventHandler(this);
   m_ImageWnd_1.CenterImage();
   m_ImageWnd_1.Reset();

   FitToWindow(m_View_1);
   m_ImageWnd_1.Refresh();

   CString m_strLeftPath;
   m_strLeftPath="D:\\Image";
   memcpy(m_cSavePath_1,m_strLeftPath.GetBuffer(),m_strLeftPath.GetLength());

    SYSTEMTIME time;
	::GetLocalTime(&time);
   	//if(m_pfLog_1 == NULL)
	//{	
	//	str.Format("L%d%02d%02d%02d%02d%02d%03d.log",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	//	m_pfLog_1 = _tfopen(str,_T("wb"));
	//	//m_pfLog_1 = fopen("D:\\Leftlog.log","wb");
	//}

	//if(m_SerialPortOne.InitPort(this,1,9600,'N',8,1,EV_RXCHAR|EV_CTS,512))
	//{
	//	m_SerialPortOne.StartMonitoring();
	//}
	
	if(m_SerialPortTwo.InitPort(this,3,9600,'N',8,1,EV_RXCHAR|EV_CTS,512))
	{
		m_SerialPortTwo.StartMonitoring();
		//SetTimer(1,500,NULL);
	}
	SetTimer(1,500,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGitDemoDlg::FitToWindow(SapView *pView)
{

	int viewWidth;
	int viewHeight;

	
	BOOL	m_bLockAspectRatio=false;
	DWORD	scaleHeight;
	DWORD	scaleWidth;
	float	scaleHeightFactor;
	float	scaleWidthFactor;

	pView->GetViewArea( &viewWidth, &viewHeight);

	scaleWidthFactor = 100.0f * viewWidth/pView->GetBuffer()->GetWidth();
	scaleHeightFactor= 100.0f * viewHeight/pView->GetBuffer()->GetHeight();

	//if( m_bLockAspectRatio)
	//{
		if( scaleWidthFactor < scaleHeightFactor)
		{
			scaleHeightFactor= scaleWidthFactor;
		}
		else
		{
			scaleWidthFactor= scaleHeightFactor;
		}
	//}

	scaleWidth = (DWORD)floor(pView->GetBuffer()->GetWidth()  * scaleWidthFactor/100 + 0.5f);
	scaleHeight= (DWORD)floor(pView->GetBuffer()->GetHeight() * scaleHeightFactor/100 + 0.5f);

	scaleWidthFactor = 100.0f * scaleWidth / pView->GetBuffer()->GetWidth();
	scaleHeightFactor= 100.0f * scaleHeight / pView->GetBuffer()->GetHeight();
	//scaleWidthFactor=(float)23.584;
	//scaleHeightFactor=(float)25.5859;
	pView->SetScalingMode( scaleWidthFactor/100.0f, scaleHeightFactor/100.0f);
	UpdateData( FALSE);

	//	int viewWidth;
	//int viewHeight;

	//m_pView->GetViewArea( &viewWidth, &viewHeight);

	//m_scaleWidthFactor = 100.0f * viewWidth/m_pView->GetBuffer()->GetWidth();
	//m_scaleHeightFactor= 100.0f * viewHeight/m_pView->GetBuffer()->GetHeight();

	//if( m_bLockAspectRatio)
	//{
	//	if( m_scaleWidthFactor < m_scaleHeightFactor)
	//	{
	//		m_scaleHeightFactor= m_scaleWidthFactor;
	//	}
	//	else
	//	{
	//		m_scaleWidthFactor= m_scaleHeightFactor;
	//	}
	//}

	//m_scaleWidth = (DWORD)floor(m_pView->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	//m_scaleHeight= (DWORD)floor(m_pView->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);

	//m_scaleWidthFactor = 100.0f * m_scaleWidth / m_pView->GetBuffer()->GetWidth();
	//m_scaleHeightFactor= 100.0f * m_scaleHeight / m_pView->GetBuffer()->GetHeight();

	//UpdateData( FALSE);
}

void CGitDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGitDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGitDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CGitDemoDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	static BYTE RBufOne[25]={0};
	static int RPosOne=0;
	static BYTE RBufTwo[25]={0};
	static int RPosTwo=0;
	INT16 revint16[4],rint16[8];
	int iTemp;
	switch((UINT)lParam)
	{
	case 1:
		RBufOne[RPosOne++]=(UINT)wParam;
		if(RPosOne>13){
			for(int i=3,j=0;i<11;j++,i+=2)
				revint16[j] =RBufOne[i] * 256+ RBufOne[i + 1];
			iTemp=(int)(revint16[0]/100.0);
			m_dbVoltage_1=iTemp/10.0;
			//iTemp=(int)(revint16[1]/100.0);
			//pThis->m_dbSpeed =iTemp/10.0;
			iTemp=(int)(revint16[2]/100.0);
			m_dbVoltage_2 =iTemp/10.0;
			RPosOne=0;
		}
		break;
	case 3:
		RBufTwo[RPosTwo++]=(UINT)wParam;
		if(RPosTwo>20){
			for(int m=3,n=0;m<19;n++,m+=2)
				rint16[n] =RBufTwo[m] * 256+ RBufTwo[m + 1];
			iTemp=(int)(rint16[4]+rint16[5]);
			m_dbSpeed =iTemp/1.0;
			RPosTwo=0;
		}
		break;
	}
	m_dbVoltage_1=0.0001;
	m_dbVoltage_2=0.0002;
	UpdateData(false);
	return 0;
}

void CGitDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	//byte sendvoltage[] = {0x01,0x03,0x02,0x58,0x00,0x04,0xC4,0x62};
	//byte sendfrequency[]={0x01,0x03,0x00,0x00,0x00,0x08,0x44,0x0C};
	////m_SerialPortOne.WriteToPort((char*)sendvoltage,8);
	//m_SerialPortTwo.WriteToPort((char*)sendfrequency,8);
	//KillTimer(1);

	m_TriggerNumber_1=m_ulTriggerNumber_1;
   m_SaveNumber_1=m_ulSaveNumber_1;
   UpdateData(false);
	CDialogEx::OnTimer(nIDEvent);
}


void CGitDemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	//KillTimer(1);

	DestroyObjects_1();

	// Delete all objects
   if (m_View_1)			delete m_View_1;
	if (m_Xfer_1)			delete m_Xfer_1;
	if (m_Buffers_1)		delete m_Buffers_1;
	if (m_AcqDevice_1)	delete m_AcqDevice_1;

	if(m_pfLog_1)
	{
		fclose(m_pfLog_1);
		m_pfLog_1 = NULL;
	}
}

BOOL CGitDemoDlg::CreateObjects_1()
{
	CWaitCursor wait;

	// Create acquisition object
	if (m_AcqDevice_1 && !*m_AcqDevice_1 && !m_AcqDevice_1->Create())
   {
      DestroyObjects_1();
      return FALSE;
   }

	// Create buffer object
	if ((m_Buffers_1 && !*m_Buffers_1))
	{
		if( !m_Buffers_1->Create())
		{
			DestroyObjects_1();
			return FALSE;
		}
		// Clear all buffers
		m_Buffers_1->Clear();
		
	}
	//if(m_iBayerEnable == 1)
	//{
	//	bool bSet = m_Bayer->SetOutputFormat(SapFormatRGB888);
	//	if(bSet == FALSE)
	//		return false;

	//	bSet = m_Bayer->Enable(1, 0);
	//	if(bSet == FALSE)
	//		return false;
	//	
	//	/*创建贝尔滤波*/
	//	if(!m_Bayer->Create())
	//	{
	//		return false;
	//	}

	//	SapBuffer* bayerBuffer = m_Bayer->GetOutputBuffer();
 //       if (bayerBuffer && *bayerBuffer)
	//		m_View_1->SetBuffer(bayerBuffer);
 //       else
	//		m_View_1->SetBuffer(m_Buffers_1);

	//	// Set alignment 
	//	m_Bayer->SetAlign(m_align[m_iBayerType]);

	//	// Set interpolation method
	//	m_Bayer->SetMethod(SapBayer::Method1);
	//}

	// Create view object
	if (m_View_1 && !*m_View_1 && !m_View_1->Create())
   {
      DestroyObjects_1();
      return FALSE;
   }

   // Set next empty with trash cycle mode for transfer
   if (m_Xfer_1 && m_Xfer_1->GetPair(0))
   {
	   //if (!m_Xfer_1->GetPair(0)->SetCycleMode(SapXferPair::EventEndOfFrame))
	  if (!m_Xfer_1->GetPair(0)->SetCycleMode(SapXferPair::CycleNextWithTrash))
      {
         DestroyObjects_1();
         return FALSE;
      }
   }

	// Create transfer object
	if (m_Xfer_1 && !*m_Xfer_1 && !m_Xfer_1->Create())
   {
      DestroyObjects_1();
      return FALSE;
   }

	return TRUE;
}

BOOL CGitDemoDlg::DestroyObjects_1()
{
	// Destroy transfer object
	if (m_Xfer_1 && *m_Xfer_1) m_Xfer_1->Destroy();

	// Destroy view object
	if (m_View_1 && *m_View_1) m_View_1->Destroy();

	//if(m_iBayerEnable == 1)
	//{
	//	if (m_Bayer && *m_Bayer) m_Bayer->Destroy();
	//}

	// Destroy buffer object
	if (m_Buffers_1 && *m_Buffers_1) m_Buffers_1->Destroy();

	// Destroy acquisition object
	if (m_AcqDevice_1 && *m_AcqDevice_1) m_AcqDevice_1->Destroy();

	return TRUE;
}

void CGitDemoDlg::XferCallback_1(SapXferCallbackInfo * pInfo)
{
	CGitDemoDlg *pDlg= (CGitDemoDlg *) pInfo->GetContext();

		int iImageType = 0;
    void *pImageData_1 = NULL;
    BOOL iSucceed = 0;
    SapBuffer *pBuffer_1 = NULL;
	char ch_ImageFileName_1[4096];
	int iCount = 0;
	time_t start;
	time_t end;
	double timeSpace;
    //pDlg->m_View_1->Show();

	pBuffer_1 = pDlg->m_Buffers_1;

   pDlg->m_ulTriggerNumber_1++;
   iSucceed = (pBuffer_1->GetAddress(&pImageData_1));
   if(iSucceed)
   {
	   if(!pDlg->m_bDirectoryExists_1)
		{
			//判断目录是否存在,如果不存在就创建目录
			pDlg->m_bDirectoryExists_1=true;
			ZLBCreateDirectory(pDlg->m_cSavePath_1);
		}
		start = clock();
		sprintf_s(ch_ImageFileName_1,1024,"%s\\%08d.bmp",pDlg->m_cSavePath_1,++pDlg->m_ulSaveNumber_1);
		pBuffer_1->Save(ch_ImageFileName_1,"-format bmp");
		end = clock();
		timeSpace =(double)(end - start);
		if(pDlg->m_pfLog_1 != NULL)
		{
			pDlg->m_ulSaveNumber_1++;
			SYSTEMTIME time;
			::GetLocalTime(&time);
			fprintf(pDlg->m_pfLog_1,"L:%d-%d-%d %d:%d:%d.%d tcount=%d acount=%d time=%f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,pDlg->m_ulTriggerNumber_1,pDlg->m_ulSaveNumber_1,timeSpace);
		}

		pBuffer_1->ReleaseAddress(&pImageData_1);
   }

}


void CGitDemoDlg::OnBnClickedOk()
{
	BOOL bAcqNoGrab	= m_Xfer_1 && *m_Xfer_1 && !m_Xfer_1->IsGrabbing();
	BOOL bAcqGrab		= m_Xfer_1 && *m_Xfer_1 && m_Xfer_1->IsGrabbing();
	BOOL bNoGrab		= !m_Xfer_1 || !m_Xfer_1->IsGrabbing();
	if( m_Xfer_1->Grab())
	{

	// Acquisition Control
	GetDlgItem(IDCANCEL)->EnableWindow(bAcqNoGrab);
	GetDlgItem(IDOK)->EnableWindow(bAcqGrab);
	}
	//CDialogEx::OnOK();
}


void CGitDemoDlg::OnBnClickedCancel()
{
	BOOL bAcqNoGrab	= m_Xfer_1 && *m_Xfer_1 && !m_Xfer_1->IsGrabbing();
	BOOL bAcqGrab		= m_Xfer_1 && *m_Xfer_1 && m_Xfer_1->IsGrabbing();
	BOOL bNoGrab		= !m_Xfer_1 || !m_Xfer_1->IsGrabbing();
	if( m_Xfer_1->Freeze())
	{
		if (CAbortDlg(this, m_Xfer_1).DoModal() != IDOK) 
		{
			m_Xfer_1->Abort();
		}
		// Acquisition Control
		GetDlgItem(IDCANCEL)->EnableWindow(bAcqNoGrab);
		GetDlgItem(IDOK)->EnableWindow(bAcqGrab);
	}
	KillTimer(1);
	CDialogEx::OnCancel();
}

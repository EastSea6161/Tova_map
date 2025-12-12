
// ImTasLicenseCreaterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ImTasLicenseCreater.h"
#include "ImTasLicenseCreaterDlg.h"
#include "afxdialogex.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>    
#include <fstream>
#include <iostream>

#include "..\QBicCommon\QBicMd5.h"
#include "..\QBicCommon\QBicIPAddress.h"

#include "LicenseVersionInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// CImTasLicenseCreaterDlg 대화 상자
CImTasLicenseCreaterDlg::CImTasLicenseCreaterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImTasLicenseCreaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImTasLicenseCreaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImTasLicenseCreaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CImTasLicenseCreaterDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON1, &CImTasLicenseCreaterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CImTasLicenseCreaterDlg 메시지 처리기

BOOL CImTasLicenseCreaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
    CString strLicenseFileID(_T(""));
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);

    try
    {
    	int nLicenseFileID = GetLastLicenseFileID();
        if (nLicenseFileID < 0)
            throw 1;

        strLicenseFileID.Format(_T("%d"), nLicenseFileID + 1);
        SetDlgItemText(IDC_EDIT2, strLicenseFileID);
    }
    catch (KExceptionPtr ex)
    {
        pEdit->EnableWindow(TRUE);         
        AfxMessageBox(_T("라이센스 정보 로딩 중 오류가 발생하였습니다."));
    }
    catch (...)
    {
        pEdit->EnableWindow(TRUE);         
        AfxMessageBox(_T("라이센스 정보 로딩 중 오류가 발생하였습니다."));
    }

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CImTasLicenseCreaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImTasLicenseCreaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImTasLicenseCreaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImTasLicenseCreaterDlg::OnBnClickedOk()
{
    CString strMacAddress(_T(""));
    int nLicenseFileID;
    CString strUserName; 
    CString strOrganization;
    CTime   timeStart;
    CTime   timeEnd;

    // 1.0 데이터 체크
    if ( InputDataCheck(strMacAddress, nLicenseFileID, strUserName, strOrganization, timeStart, timeEnd) == false )
        return;

    QBicTime oStartDay(timeStart);
    QBicTime oEndDay  (timeEnd);
    QBicTime oToDay;

    if (oEndDay < oToDay)
    {
        AfxMessageBox(_T("종료 시간 정보가 오늘 이전입니다."));
        return;
    }

    int nDayInterval = (int)oEndDay.DayInterval(oStartDay);    

    CString strCheck(_T(""));
    strCheck.AppendFormat(_T("License Version Type : %d"), GxLicenseVersion);
    strCheck.AppendFormat(_T("\nLicense 기간 : %d 일(%s~%s)"), nDayInterval, oStartDay.ToYmdString(), oEndDay.ToYmdString());
    strCheck.Append(_T("\n\n진행하시겠습니까 ?"));

    if ( AfxMessageBox(strCheck, MB_OKCANCEL) == IDCANCEL)
        return;

    CString strAppPath = GetAppPath();
    if (strAppPath.GetLength() < 1)
    {
        AfxMessageBox(_T("경로 정보를 알수 없습니다."));
        return;
    }

    CString strLicenseFilePath(_T(""));
    strLicenseFilePath.Format(_T("%s\\ImTasLicense\\%d"), strAppPath, nLicenseFileID);

    try
    {   
        CString strLicenseDir(_T(""));
        strLicenseDir.Format(_T("%s\\ImTasLicense"), strAppPath);
        QBicDir::CreateDirectory(strLicenseDir);
        QBicDir::CreateDirectory(strLicenseFilePath);
    }
    catch (KExceptionPtr ex)
    {
        AfxMessageBox(ex->GetErrorMessage());
        return;
    }
    catch (...)
    {
        AfxMessageBox(_T("디렉토리 생성 중 오류가 발생하였습니다"));
        return;
    }

    CString strLicenseFileName(_T(""));
    strLicenseFileName.Format(_T("%s\\TOVA.lic"), strLicenseFilePath);
    
    #pragma pack(push, 1)
    std::ofstream ofs(strLicenseFileName.GetBuffer(), std::ios::binary);
    if (!ofs.is_open())
    {
        AfxMessageBox(_T("라이센스 파일 생성 중 오류가 발생하였습니다."));
        return;
    }

    //★ 난수 발생 -> Write
    std::srand((unsigned)time(0)); 
    for (int i=0; i<100000; i++) {        
        int nRandomVariable = std::rand();
        ofs.write(reinterpret_cast<char*>( &nRandomVariable ), sizeof(int) );         
    }

    CString strResult(_T(""));
    QBicMd5 oQBicMd5;

    if (oQBicMd5.GetMd5String(strMacAddress, strResult) == false) {
        AfxMessageBox(_T("암호화 처리 중 오류가 발생하였습니다."));
        return;
    }
    
    // step 1 : write version no
    ofs.seekp(802);
    int nVersion(GxLicenseVersion);
    ofs.write(reinterpret_cast<char*>( &nVersion ), sizeof(int) );  
    
    // write 라이센스 시작일자    
    int nStartYear  = timeStart.GetYear();
    int nStartMonth = timeStart.GetMonth();
    int nStartDay   = timeStart.GetDay();
    ofs.seekp(912);      
    ofs.write(reinterpret_cast<char*>( &nStartYear  ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nStartMonth ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nStartDay   ), sizeof(int) );  
    
    // write 라이센스 종료일자
    int nEndYear   = timeEnd.GetYear();
    int nEndMonth  = timeEnd.GetMonth();
    int nEndDay    = timeEnd.GetDay();
    ofs.seekp(1100);      
    ofs.write(reinterpret_cast<char*>( &nEndYear  ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nEndMonth ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nEndDay   ), sizeof(int) );  

    // write 프로그램 실행일자
    // 중요 : 1200~5005Byte는 사용하면 안된다.
    int nRunYear  = nStartYear;
    int nRunMonth = nStartMonth;
    int nRunDay   = nStartDay;    
    ofs.seekp(1300);      
    ofs.write(reinterpret_cast<char*>( &nRunYear  ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nRunMonth ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nRunDay   ), sizeof(int) );  

    int nHour     = 0;
    int nMinute   = 0;
    int nSec      = 0;

    ofs.seekp(2300);      
    ofs.write(reinterpret_cast<char*>( &nHour   ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nMinute ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nSec    ), sizeof(int) );  
    // 중요 : 1200~5005Byte는 사용하면 안된다.

    char* szOut = NULL;
    USES_CONVERSION;
    szOut= T2A (strResult.GetBuffer());
    int nByte = strlen(szOut) + 1;    
    // write mac-address
    ofs.seekp(7802);
    ofs.write(reinterpret_cast<char*>( &nByte ), sizeof(int) );   
    ofs.write(szOut, nByte);     
    ofs.close();   
    
    try
    {
    	WriteDBInfo(strMacAddress, nLicenseFileID, strUserName, strOrganization, timeStart, timeEnd, GxLicenseVersion);
        AfxMessageBox(_T("생성하였습니다."));
    }
    catch (KExceptionPtr ex)
    {
        AfxMessageBox(_T("데이터베이스 입력 중 오류가 발생하였습니다."));
    }
    catch (...)
    {
        AfxMessageBox(_T("데이터베이스 입력 중 오류가 발생하였습니다."));
    }
        
    #pragma  pack(pop)     
}

void CImTasLicenseCreaterDlg::OnBnClickedButton1()
{
    CString strMacAddress(_T(""));
    int nLicenseFileID;
    CString strUserName; 
    CString strOrganization;
    CTime   timeStart;
    CTime   timeEnd;

    // 1.0 데이터 체크
    if ( InputDataCheck(strMacAddress, nLicenseFileID, strUserName, strOrganization, timeStart, timeEnd) == false )
        return;

    CString strAppPath = GetAppPath();
    if (strAppPath.GetLength() < 1)
    {
        AfxMessageBox(_T("경로 정보를 알수 없습니다."));
        return;
    }
    
    CString strLicenseFilePath(_T(""));
    strLicenseFilePath.Format(_T("%s\\ImTasLicense\\%d"), strAppPath, nLicenseFileID);

    CString strLicenseFileName(_T(""));
    strLicenseFileName.Format(_T("%s\\TOVA.lic"), strLicenseFilePath);

    #pragma pack(push, 1)
    std::ifstream ifs(strLicenseFileName.GetBuffer(), std::ios::binary);

    if (!ifs.is_open())
    {
        AfxMessageBox(_T("라이센스 파일을 오픈중 오류가 발생하였습니다."));
        return;
    }

    CString strResult(_T(""));
    QBicMd5 oQBicMd5;    
    if (oQBicMd5.GetMd5String(strMacAddress, strResult) == false)
    {
        AfxMessageBox(_T("암호화 처리 중 오류가 발생하였습니다."));
        return;
    }

    // TODO : 향후 추가 필요
    // step 1 : read version no
    ifs.seekg(802);
    int nVersion(1);
    ifs.read(reinterpret_cast<char*>( &nVersion ), sizeof(int) );  

    // read 라이센스 시작일자    
    int nStartYear  = timeStart.GetYear();
    int nStartMonth = timeStart.GetMonth();
    int nStartDay   = timeStart.GetDay();
    ifs.seekg(912);      
    ifs.read(reinterpret_cast<char*>( &nStartYear  ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nStartMonth ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nStartDay   ), sizeof(int) );  

    // read 라이센스 종료일자
    int nEndYear  = timeEnd.GetYear();
    int nEndMonth = timeEnd.GetMonth();
    int nEndDay   = timeEnd.GetDay();
    ifs.seekg(1100);      
    ifs.read(reinterpret_cast<char*>( &nEndYear  ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nEndMonth ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nEndDay   ), sizeof(int) );  

    // read 프로그램 실행일자
    int nRunYear  = nStartYear;
    int nRunMonth = nStartMonth;
    int nRunDay   = nStartDay;
    ifs.seekg(1300);      
    ifs.read(reinterpret_cast<char*>( &nRunYear  ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nRunMonth ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nRunDay   ), sizeof(int) );  

    int nHour     = 0;
    int nMinute   = 0;
    int nSec      = 0;

    if (nVersion > 1)
    {
        ifs.read(reinterpret_cast<char*>( &nHour   ), sizeof(int) );  
        ifs.read(reinterpret_cast<char*>( &nMinute ), sizeof(int) );  
        ifs.read(reinterpret_cast<char*>( &nSec    ), sizeof(int) );  
    }    

    ifs.seekg(2300);    

    char szOut[1024] = {0};
    int nByte(0); 
    // read mac-address
    ifs.seekg(7802);
    ifs.read(reinterpret_cast<char*>( &nByte ), sizeof(int) );  

    ifs.read(szOut, nByte);     
    ifs.close();   

    USES_CONVERSION;
    CString strBinaryMacAddress = A2T(szOut);
    
    CString strMsg(_T(""));
    strMsg.AppendFormat(_T("License Version : %d"), nVersion);
    strMsg.AppendFormat(_T("\n시작일자:%04d년 %02d월 %02d일"), nStartYear, nStartMonth, nStartDay);
    strMsg.AppendFormat(_T("\n종료일자:%04d년 %02d월 %02d일"), nEndYear, nEndMonth, nEndDay);
    
    // 메세지
    if (strResult.CompareNoCase(strBinaryMacAddress) == 0)
    {
        strMsg.Append(_T("\n라이센스 정보가 일치합니다."));      
    }
    else
    {
        strMsg.Append(_T("\nError..라이센스 정보가 일치하지 않습니다."));
    }

    AfxMessageBox(strMsg);

#pragma  pack(pop)  
}

bool CImTasLicenseCreaterDlg::InputDataCheck
                              ( CString& ar_strMacAddress,   int& ar_nLicenseFileID, CString& ar_strUserName, 
                                CString& ar_strOrganization, CTime& ar_timeStart,    CTime& ar_timeEnd
                              )
{
    // MacAddress
    CString strMacAddress(_T(""));
    int nStrLength = GetDlgItemText(IDC_EDIT1, strMacAddress);

    if (nStrLength < 1)
    {
        AfxMessageBox(_T("Mac Address 정보를 입력해 주세요.. !!"));
        GetDlgItem(IDC_EDIT1)->SetFocus();
        return false;
    }    
    ar_strMacAddress = strMacAddress.TrimRight();

    // 라이센스 파일 ID
    CString strLicenseFileID(_T(""));
    nStrLength = GetDlgItemText(IDC_EDIT2, strLicenseFileID);
    if (nStrLength < 1)
    {
        AfxMessageBox(_T("License 파일 ID 정보를 입력해 주세요.. !!"));
        GetDlgItem(IDC_EDIT2)->SetFocus();
        return false;
    }        
    ar_nLicenseFileID = _ttoi(strLicenseFileID);

    // 사용자 이름
    CString strUserName(_T(""));
    nStrLength = GetDlgItemText(IDC_EDIT3, strUserName);
    if (nStrLength < 1)
    {
        AfxMessageBox(_T("사용자 정보를 입력해 주세요.. !!"));
        GetDlgItem(IDC_EDIT3)->SetFocus();
        return false;
    }        
    ar_strUserName = strUserName;

    // 사용자 조직
    CString strOrganization(_T(""));
    nStrLength = GetDlgItemText(IDC_EDIT4, strOrganization);
    if (nStrLength < 1)
    {
        AfxMessageBox(_T("조직 정보를 입력해 주세요.. !!"));
        GetDlgItem(IDC_EDIT4)->SetFocus();
        return false;
    }        
    ar_strOrganization = strOrganization;

    // 시작일자
    CDateTimeCtrl* pStartDateCtrl = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER1);
    pStartDateCtrl->GetTime(ar_timeStart);

    CDateTimeCtrl* pEndDateCtrl   = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER2);
    pEndDateCtrl->GetTime(ar_timeEnd);

    if (ar_timeStart > ar_timeEnd)
    {
        AfxMessageBox(_T("종료 시간 정보가 시작시간 보다 늦습니다."));
        return false;
    }
    
    return true;
}

CString CImTasLicenseCreaterDlg::GetAppPath()
{
    TCHAR    szAppPath[_MAX_PATH];
    VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szAppPath, _MAX_PATH));		

    CString  strAppPath(szAppPath);

    int nIndex  = strAppPath.ReverseFind(_T('\\'));
    if (nIndex > 0) 
    {
        strAppPath = strAppPath.Left(nIndex);
    }
    else 
    {
        strAppPath.Empty();
    }       

    return strAppPath;
}


KDBaseConnectionPtr CImTasLicenseCreaterDlg::GetLicenseDBConnection()
{
    CString strApplicationDirectory = GetAppPath();
    KDBaseConnectionPtr spDBaseConnection = KDBaseConnectionPtr(new KSQLiteConnection());
    try
    {
        KConnectionString oCon;
        oCon.m_strUrl = strApplicationDirectory + _T("\\ImTasLicense.db");
        if (spDBaseConnection->Connection(oCon))
        {
            return spDBaseConnection;
        }
        else
        {
            spDBaseConnection.reset();
        }
    }
    catch (...)
    {
        spDBaseConnection.reset();
    }

    return spDBaseConnection;
}

void CImTasLicenseCreaterDlg::WriteDBInfo( CString ar_strMacAddress, int ar_nLicenseFileID, CString ar_strUserName, CString ar_strOrganization, CTime& ar_timeStart, CTime& ar_timeEnd, int ar_version )
{
    try
    {
        CString strStartTime(_T(""));
        CString strEndTime  (_T(""));
        strStartTime.Format(_T("%04d-%02d-%02d"), ar_timeStart.GetYear(), ar_timeStart.GetMonth(), ar_timeStart.GetDay());
        strEndTime.Format(_T("%04d-%02d-%02d"),   ar_timeEnd.GetYear(),   ar_timeEnd.GetMonth(),   ar_timeEnd.GetDay());

        KDBaseConnectionPtr spDBConnection = GetLicenseDBConnection();
        CString strSQL(_T(""));

        strSQL.Format(_T("INSERT OR REPLACE INTO ImTasLicense(License_ID, License_Start_Date, License_End_Date, Mac_Address, UserName, Organization, License_Version) Values('%d','%s','%s', '%s', '%s', '%s', '%d')"),
                                                              ar_nLicenseFileID,strStartTime, strEndTime, ar_strMacAddress, ar_strUserName, ar_strOrganization, ar_version);
        
        int nResult = spDBConnection->ExecuteUpdate(strSQL);
        if ( nResult == 1)
        {
            throw 1;
        }
    }
    catch (KExceptionPtr ex)
    {
    	throw ex;
    }
    catch (...)
    {
    	ThrowException(_T("At WriteDBInfo")); 
    }
}

int CImTasLicenseCreaterDlg::GetLastLicenseFileID()
{
    try
    {
        int nLicenseID(-1);
    	KDBaseConnectionPtr spDBConnection = GetLicenseDBConnection();
        CString strSQL(_T("Select IFNULL(Max(License_ID), 0) AS LicenseID From ImTasLicense"));

        KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
        if (spResultSet->Next())
        {
            nLicenseID = spResultSet->GetValueInt(0);
        }

        return nLicenseID;
    }
    catch (KExceptionPtr ex)
    {
    	throw ex;
    }
    catch (...)
    {
    	ThrowException(_T("At GetLastLicenseFileID")); 
    }
}


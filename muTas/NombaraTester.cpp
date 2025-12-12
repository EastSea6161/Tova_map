// NombaraTester.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NombaraTester.h"
#include "afxdialogex.h"
#include "Launcher.h"
#include "MapView.h"
#include "PopupMessenger.h"
// KNombaraTester 대화 상자입니다.

IMPLEMENT_DYNAMIC(KNombaraTester, KDialogEx)

KNombaraTester::KNombaraTester(KMapView* pMapView, CWnd* pParent /*=NULL*/)
	: KDialogEx(KNombaraTester::IDD, pParent)
{
    m_pMapView = pMapView;
}

KNombaraTester::~KNombaraTester()
{
}

void KNombaraTester::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KNombaraTester, KDialogEx)
    ON_WM_COPYDATA()
    ON_BN_CLICKED(IDOK, &KNombaraTester::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON1, &KNombaraTester::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &KNombaraTester::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &KNombaraTester::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &KNombaraTester::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &KNombaraTester::OnBnClickedButton5)
    ON_BN_CLICKED(IDCANCEL, &KNombaraTester::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL KNombaraTester::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
    try
    {
        int     nMsgCode = pCopyDataStruct->dwData;
        CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

        m_nResultCode = nMsgCode;
        AddStatusMessage(strMsg);

        TxLogDebug((LPCTSTR)strMsg);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }   

    return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void KNombaraTester::AddStatusMessage(CString a_strMsg)
{
    QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
    if(spProgressWindow)
    {
        spProgressWindow->SetStatus(a_strMsg);
        AfxMessageBox(a_strMsg);
    }
}

// KNombaraTester 메시지 처리기입니다.


void KNombaraTester::OnBnClickedOk()
{
    QBicEnvelope oEnvelope(0, 0, 100, 100);
    oEnvelope.Offset(100);
    oEnvelope.Offset(-100);
    oEnvelope.Offset(-40);
    oEnvelope.Offset(40);

    oEnvelope.Offset(-50);

    oEnvelope.Offset(50);
    QBicPoint oA1(0, 0);
    QBicPoint oA2(100, 0);

    QBicPoint oC(0, 0);
    QBicPoint oD(50, 50);
    QBicPoint oE(100, 100);

    QBicPoint oF(200, 20);
    QBicPoint oG(-60, 20);

    QBicPoint oCP;
    double dDist1, dDist2;
    int nResult(0);
    QBicGeometry::GetShortestPoint(oA1, oA2, oC, oCP,  dDist1, dDist2);
    nResult = QBicGeometry::GetIntersectPoint(oA1, oA2, oC, oCP, dDist1, dDist2);

    QBicGeometry::GetShortestPoint(oA1, oA2, oD, oCP, dDist1, dDist2);
    nResult = QBicGeometry::GetIntersectPoint(oA1, oA2, oD, oCP, dDist1, dDist2);

    QBicGeometry::GetShortestPoint(oA1, oA2, oE, oCP, dDist1, dDist2);
    nResult = QBicGeometry::GetIntersectPoint(oA1, oA2, oE, oCP, dDist1, dDist2);

    QBicGeometry::GetShortestPoint(oA1, oA2, oF, oCP, dDist1, dDist2);
    nResult = QBicGeometry::GetIntersectPoint(oA1, oA2, oF, oCP, dDist1, dDist2);

    QBicGeometry::GetShortestPoint(oA1, oA2, oG, oCP, dDist1, dDist2);
    nResult = QBicGeometry::GetIntersectPoint(oA1, oA2, oG, oCP, dDist1, dDist2);

    int m =99;
    return;

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

    QBicSimpleProgressThread::ExecuteThread(ReceivedThreadCaller, this, false, nLang);

    AfxMessageBox(_T("성공적으로 완료되었습니다."));
}

unsigned __stdcall KNombaraTester::ReceivedThreadCaller(void* p)
{
    QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
    QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
    pProgressWindow->SetStatus(_T("calculating statistics..."));

    KNombaraTester* pDlg = (KNombaraTester*)pParameter->GetParameter();
    pDlg->Run();   

    return 0;
}

void KNombaraTester::Run()
{
    m_nResultCode = DLL_UNEXPECTED_ERROR;
    CString strWindowName;
    GetWindowText(strWindowName);

    AddStatusMessage(_T("> 통계 분석 Start ..."));

    KLauncher::Run64(strWindowName, 0);
}

// #include <log4cxx/logger.h>
// #include <log4cxx/basicconfigurator.h>
// #include <log4cxx/propertyconfigurator.h>
// #include <log4cxx/helpers/exception.h>
// 
// using namespace log4cxx;
// using namespace log4cxx::helpers;

void KNombaraTester::OnBnClickedButton1()
{
    /*PropertyConfigurator::configure(log4cxx::File("log4cxx.conf"));
    LoggerPtr rootlog = Logger::getRootLogger(); 
    LoggerPtr logger = Logger::getLogger("");

    setlocale(LC_ALL, "");

    LOG4CXX_DEBUG(logger, _T("정수호 : nombara"));
    return;*/
    CString strProvider   = _T("SQLOLEDB");
    CString strDataSource = _T("192.168.40.26");
    CString strDBName     = _T("BigData");
    CString strUID        = _T("sa");
    CString strPWD        = _T("w00rinara");

    CString strConnectionString = _T("");
    strConnectionString.Format(_T("Provider=%s;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s;"), strProvider, strDataSource, strDBName, strUID, strPWD);

    CString strSelect = _T("Select * From BigData.TxMaster.Link_Vertex");

    KDBaseConPtr spDBaseConnection(new KAdoConnection(strConnectionString));
    KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSelect);

    TxLogDebugStartMsg();
    while(spResultSet->Next())
    {
        CString strID    = spResultSet->GetValueString(0);
        CString strValue = spResultSet->GetValueString(1);
                
        CString strOut(_T(""));
        strOut.Format(_T("ID:%s, Value:%s"), strID, strValue);

        //LOG4CXX_DEBUG(QBicLog4cxx::GetRootLogger(), LOG4CXX_STR(strOut));
        //TxLogDebug((LPCTSTR)strOut);
    }
    TxLogDebugEndMsg();
    return;

    TxLogDebugStartMsg();
    strSelect = _T("Insert Into BigData.dbo.TestTable(ID, Value) Values(1, 77)");
    spDBaseConnection->ExecuteUpdate(strSelect);

    strSelect = _T("Delete From BigData.dbo.TestTable");
    spDBaseConnection->BeginTransaction();
    spDBaseConnection->ExecuteUpdate(strSelect);
    spDBaseConnection->Commit();

    CString strInsert(_T("Insert Into BigData.dbo.TestTable(ID, Value, V_Float) Values(?, ?, ?)"));
    KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strInsert); 

    for (int i=0; i<1000000; i++)
    {
        spPrepareStatement->BindInt(1, i);
        spPrepareStatement->BindInt(2, i);
        spPrepareStatement->BindDouble(3, 99.0);
        spPrepareStatement->ExecuteUpdate();
        spPrepareStatement->Reset();
    }

    TxLogDebugEndMsg();
}

#include <boost/iostreams/device/mapped_file.hpp> // for mmap
void KNombaraTester::OnBnClickedButton2()
{    
    TxLogDebugStartMsg();    
    char buffer[1024];

    boost::iostreams::mapped_file mmap("C:\\QBicDev\\SMT_201309\\test.txt", boost::iostreams::mapped_file::readonly);

    auto pMMapFile    = mmap.const_data();
    auto pMMapFileEnd = pMMapFile + mmap.size();

    uintmax_t m_numLines = 0;

    auto pFirstPos = pMMapFile;
    while (pMMapFile && pMMapFile!=pMMapFileEnd)
    {
        memset(buffer, '\0', 1024);

        if ((pMMapFile = static_cast<const char*>(memchr(pMMapFile, '\n', pMMapFileEnd-pMMapFile))))
        {
            size_t nxCount = pMMapFile-pFirstPos;
            if (nxCount < 1024)
                memcpy(buffer, pFirstPos, nxCount);
            else
                memcpy(buffer, pFirstPos, 1024);

            m_numLines++; 
            pMMapFile++;

            pFirstPos  = pMMapFile;
        }
        else
        {
            size_t nxCount = pMMapFileEnd-pFirstPos;
            if (nxCount < 1024)
                memcpy(buffer, pFirstPos, nxCount);
            else
                memcpy(buffer, pFirstPos, 1024);
        }
    }

    mmap.close();
    TxLogDebugEndMsg();
}


void KNombaraTester::OnBnClickedButton3()
{
    


        
//     {
//         TxLogDebugStartMsg();
//         std::ifstream inFile;
//         inFile.open("C:\\QBicDev\\SMT_201309\\tra_col_link_20130930.txt");
// 
//         if (!inFile.is_open()) 
//         {
//             AfxMessageBox(_T("Not Opened"));
//             return;
//         }
// 
//         char buf[500];
//         memset(buf, 0, 500);
// 
//         while (!inFile.getline(buf, sizeof(buf)).eof()) 
//         {
//         }
// 
//         TxLogDebugEndMsg();
//     }
// 
//     {
//         TxLogDebugStartMsg();
//         FILE *fp = fopen("C:\\QBicDev\\SMT_201309\\tra_col_link_20130930.txt", "r");
// 
//         if (NULL == fp)
//         {
//             AfxMessageBox(_T("Not Opened"));
//             return;
//         }
// 
//         char buf[512];
//         while (NULL != fgets(buf, sizeof(buf), fp)) {
// 
//         }
//         fclose(fp);
// 
//         TxLogDebugEndMsg();
//     }
}


void KNombaraTester::OnBnClickedButton4()
{
    KPopupMessenger oMessenger;
    oMessenger.Show(_T("Hello\nWorld\nHaHaHa"), m_pMapView);
    return;

    double dWidthCellDist (1000.0);
    double dHeightCellDist(1000.0);
    double dMinX, dMinY, dMaxX, dMaxY;
    m_pMapView->GetFullExtent(dMinX, dMinY, dMaxX, dMaxY);

    double dWidth  = dMaxX - dMinX;
    double dHeight = dMaxY - dMinY;

    int nWidthCount  = (int) ( dWidth  / dWidthCellDist  + 0.5 );
    int nHeightCount = (int) ( dHeight / dHeightCellDist + 0.5 );

    int m = 0;
    //             680489, 655572
    //188535, 39752

    //             680489, 655572
    //188000, 39000

}

BOOL KNombaraTester::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
    //SetBackgroundColor(RGB(255, 0, 0));

    return TRUE; 
}


void KNombaraTester::OnBnClickedButton5()
{
    
}


void KNombaraTester::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}

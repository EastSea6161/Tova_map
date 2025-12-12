// StationODCalculatorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StationODCalculatorDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "BulkDBaseDemarcation.h"

#include "StationODVirtualRecord.h"


const int NONERROR		= 0;
const int BUSERROR		= 1;
const int RAILERROR		= 2;
const int BUSRAILERROR	= 3;

const int REPORTCTL_COLUMN_WIDTH = 70;

const CString HEADER_ORIGIN      = _T("기점");
const CString HEADER_DESTINATION = _T("종점");

// KStationODCalculatorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KStationODCalculatorDlg, KResizeDialogEx)

KStationODCalculatorDlg::KStationODCalculatorDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KStationODCalculatorDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_pRecordArray(nullptr)
{
    m_pVectorRecord = nullptr;
    m_pMatrixRecord = nullptr;

    m_strFile   = _T("");
    m_strErrMsg = _T(""); 

    m_bExportThreadOK = false;
    m_bEnableExport   = false;
}

KStationODCalculatorDlg::~KStationODCalculatorDlg()
{
	if (m_pRecordArray != nullptr)
	{
		delete m_pRecordArray;
		m_pRecordArray = nullptr;
	}

	CloseWindowProc();
}

void KStationODCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}

BEGIN_MESSAGE_MAP(KStationODCalculatorDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KStationODCalculatorDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON2, &KStationODCalculatorDlg::OnBnClickedButtonSearch)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KStationODCalculatorDlg::OnBnClickedRadioType)
    ON_BN_CLICKED(IDC_BUTTON3, &KStationODCalculatorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDCANCEL, &KStationODCalculatorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void KStationODCalculatorDlg::ActivatedFrameWndNotifyProcess()
{
	try
	{
		if(nullptr != m_spImTasMultiIDFind)
		{
			if(!m_spImTasMultiIDFind->IsUserClosed())
			{
				if(!m_spImTasMultiIDFind->IsWindowVisible())
					m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KStationODCalculatorDlg::DeActivatedFrameWndNotifyProcess()
{
	try
	{
		if(nullptr != m_spImTasMultiIDFind)
		{
			if(!m_spImTasMultiIDFind->IsUserClosed())
			{
				if(m_spImTasMultiIDFind->IsWindowVisible())
					m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


// KStationODCalculatorDlg 메시지 처리기입니다.

void KStationODCalculatorDlg::ModelessOKFeedback( UINT nDialogID )
{
	if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			m_mapStnOD.clear();
            m_setTransit.clear();

			CString strIDs = m_spImTasMultiIDFind->GetSelectedID();
            SetDlgItemText(IDC_EDIT1, strIDs);
			
            std::set<CString> setID  = m_spImTasMultiIDFind->GetSelectedIDSet();
            for (auto iter = setID.begin(); iter != setID.end(); ++iter) {
                __int64 nxLineID = _ttoi64(*iter);
                SetViewTransit(nxLineID);
            }
            
            OnBnClickedButtonSearch();
		}
	}
}

void KStationODCalculatorDlg::ModelessCancleFeedback( UINT nDialogID )
{

}

BOOL KStationODCalculatorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KBulkDBaseDemarcation::AllZoneID(m_pTarget, m_setAllZoneID);
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	ResizeComponent();

	KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AutoColumnSizeFalse(m_wndReport);

    KBulkDBaseTransit::TransitAllIdCaptionType(m_pTarget, m_vecLineIDCaption, m_mapLineData);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KStationODCalculatorDlg::SetViewTransit( Integer a_nxTransitID )
{
	try
	{
        auto iFind = m_mapLineData.find(a_nxTransitID);
        if (iFind == m_mapLineData.end())
            return;

        m_setTransit.insert(a_nxTransitID);

        CString strCaption(_T(""));
        for (auto iter = m_setTransit.begin(); iter != m_setTransit.end(); ++iter) {
            __int64 nxTransitID = *iter;
            auto iFind = m_mapLineData.find(nxTransitID);

            if (iFind == m_mapLineData.end())
                continue;

            if (strCaption.GetLength() == 0) {
                strCaption.AppendFormat(_T("%I64d"), nxTransitID);
            }
            else {
                strCaption.AppendFormat(_T(",%I64d"), nxTransitID);
            }
        }
        GetDlgItem(IDC_EDIT1)->SetWindowText(strCaption);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KStationODCalculatorDlg::OnBnClickedButtonFind()
{
	try
	{
		CWaitCursor cw;

        CString strSelect(_T("")); 
        GetDlgItemText(IDC_EDIT1, strSelect);

        m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
        m_spImTasMultiIDFind->SetModelessReceiver(this);

        m_spImTasMultiIDFind->SetIDCaption(m_vecLineIDCaption);

        m_spImTasMultiIDFind->SetSelectedID(strSelect);
        m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
        m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KStationODCalculatorDlg::OnBnClickedButtonSearch()
{
	try
	{
        m_bEnableExport = false;
		if (m_setTransit.size() == 0) {
            m_wndReport.ResetContent();
			AfxMessageBox(_T("선택된 노선이 없습니다."));
			return;
		}
		
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(SearchStationThreadCaller, this, false, nLang);
		if (m_nReadOutResult != NONERROR)
		{            
            m_wndReport.ResetContent();
            m_wndReport.GetColumns()->Clear();
			AfxMessageBox(_T("결과 파일 읽기에 실패하였습니다."));
			return;
		}

		if (m_mapStnOD.size() == 0)
		{
            GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));           
            m_wndReport.ResetContent();
            m_wndReport.GetColumns()->Clear();
			AfxMessageBox(_T("선택한 노선에 대한 정보가 없습니다."));
			return;
		}

		m_wndReport.ResetContent();
		m_wndReport.GetColumns()->Clear();

		if (IsDlgButtonChecked(IDC_RADIO1)) {
			MatrixReport();
		}
		else {
			VectorReport();
		}

        m_bEnableExport = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

unsigned __stdcall KStationODCalculatorDlg::SearchStationThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("결과 데이터 파일 읽는 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Reading Output Files..."));
	}

	KStationODCalculatorDlg* pDlg = (KStationODCalculatorDlg*)pParameter->GetParameter();;

	pDlg->ReadOutputFile();

	return 0;
}

void KStationODCalculatorDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KStationODCalculatorDlg::ReadOutputFile()
{
	m_nReadOutResult = NONERROR;
    m_mapStnOD.clear();
    m_mapRouteNodeSeq.clear();

    for (auto iter = m_setTransit.begin(); iter != m_setTransit.end(); ++iter) {
        __int64 nxID = *iter;

        auto iFind = m_mapRouteNodeSeq.find(nxID);
        if (iFind == m_mapRouteNodeSeq.end()) {
            std::map<int, TStationODInfo> mapNodeSeq; {
                KMultiModalResultInfo::GetStationODSQLData(m_pTarget, nxID, mapNodeSeq);
            }
            
            m_mapRouteNodeSeq[nxID] = mapNodeSeq;
        }
    }

	bool bReadBus(false);
	bool bReadRail(false);
	bool bReadBusRail(false);
	try
	{
        for (auto iter = m_setTransit.begin(); iter != m_setTransit.end(); ++iter) {
            __int64 nxID = *iter;

            int nMode(0);
            auto iFind = m_mapLineData.find(nxID);
            if (iFind != m_mapLineData.end()) {
                nMode = iFind->second.nMode;
            }
            else {
                continue;
            }

            //★ 노선의 유형
            if (nMode == 3) {
                bReadBus     = true;
                bReadBusRail = true;
            }

            if(nMode == 4) {
                bReadRail    = true;
                bReadBusRail = true;
            }
        }
		
		CString strDataFileName(_T(""));
		CString strSavedPath(_T(""));

		int nZoneCount = (int)m_setAllZoneID.size();
		int nIndex = 1;

        for (auto iter = m_setAllZoneID.begin(); iter != m_setAllZoneID.end(); ++iter) 
        {
            __int64 nxZoneID = *iter;
            
            if (bReadBus)
            {
                strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_BusPathAssign.dat"), nxZoneID);
                strSavedPath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);
                
                if (ReadBusPathFile(strSavedPath) == false) {
                    m_nReadOutResult = BUSERROR;

                    TxLogDebug(_T("Exception : ") + strSavedPath);
                    return false;
                }
            }

            if (bReadRail)
            {
                strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_RailPathAssign.dat"), nxZoneID);
                strSavedPath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);

                if (ReadRailPathFile(strSavedPath) == false) {
                    m_nReadOutResult = RAILERROR;
                    TxLogDebug(_T("Exception : ") + strSavedPath);
                    return false;
                }
            }

            if (bReadBusRail)
            {
                strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_BusRailPathAssign.dat"), nxZoneID);
                strSavedPath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);

                if (ReadBusRailPathFile(strSavedPath) == false) {
                    m_nReadOutResult = BUSRAILERROR;
                    TxLogDebug(_T("Exception : ") + strSavedPath);
                    return false;
                }
            }

            CString strMsg(_T(""));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strMsg.Format(_T("모형 실행 결과 데이터 읽는 중 : %d / %d "), nIndex++, nZoneCount);
			}
			else {
				strMsg.Format(_T("Reading Output File : %d / %d "), nIndex++, nZoneCount);
			}
            AddStatusMessage(strMsg);
        }
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return true;
}

bool KStationODCalculatorDlg::ReadBusPathFile(CString strFileName)
{
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);
			while(! ifs.eof())
			{
				Integer nxOZoneID;
				Integer nxDZoneID;
				int		nPathID;

				ifs.read( reinterpret_cast<char*>( &(nxOZoneID)), sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDZoneID)), sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathID)),	  sizeof(int) );

				if(ifs.eof())
                    break;

				for (int i= 0; i< BUSPATH_ASSIGNSET_COUNT; i++)
				{
					for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
					{
						Integer nxTransitID;
						int		nStartStation;
						int		nEndStation;
						double  dTrip;

						ifs.read( reinterpret_cast<char*>( &(nxTransitID)),   sizeof(Integer));
						ifs.read( reinterpret_cast<char*>( &(nStartStation)), sizeof(int));
						ifs.read( reinterpret_cast<char*>( &(nEndStation)),   sizeof(int));
						ifs.read( reinterpret_cast<char*>( &(dTrip)),         sizeof(double));

						if(0 == nxTransitID)
							continue;

                        auto iter = m_setTransit.find(nxTransitID);
						if (iter != m_setTransit.end())
						{
                            auto iFind = m_mapRouteNodeSeq.find(nxTransitID);
                            if (iFind == m_mapRouteNodeSeq.end())
                                continue;

                            std::map<int, TStationODInfo>& mapSeqStn = iFind->second;
                            __int64 nxSStnID(0); {
                                auto iter = mapSeqStn.find(nStartStation);
                                if (iter == mapSeqStn.end())
                                    continue;

                                TStationODInfo& oInfo = iter->second;
                                nxSStnID = oInfo.TNodeID;
                            }

                            __int64 nxEStnID(0); {
                                auto iter = mapSeqStn.find(nEndStation);
                                if (iter == mapSeqStn.end())
                                    continue;

                                TStationODInfo& oInfo = iter->second;
                                nxEStnID = oInfo.TNodeID;
                            }

                            if (nxEStnID == 0 || nxSStnID == 0)
                                continue;

							KODKey oKey(nxSStnID, nxEStnID);
                            double& dExsitTrip = m_mapStnOD[oKey]; {
                                dExsitTrip += dTrip;
                            }
						}
					}
				}

				TBusSubData oBusSubData;
				ifs.read( reinterpret_cast<char*>( &oBusSubData), sizeof(TBusSubData) );
				nCount++;
			}
		}
		ifs.close();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool KStationODCalculatorDlg::ReadRailPathFile( CString strFileName )
{
	TxLogDebugStartMsg();

	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOZoneID;
				Integer nxDZoneID;
				int		nPathID;

				ifs.read( reinterpret_cast<char*>( & nxOZoneID),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( & nxDZoneID),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( & nPathID),          sizeof(int) );
				if(ifs.eof())break;

				int nBoardingCnt(0); //탑승횟수
				ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

				if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int i= 0; i< nBoardingCnt; i++)
				{
					int nDuplicateCnt(0); //중복 노선 갯수
					ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

					if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					for (int k= 0; k< nDuplicateCnt; k++)
					{
						Integer nxTransitID(0);
						int		nStartStation(0);
						int		nEndStation(0);
						double	dTrip(0);

						ifs.read( reinterpret_cast<char*>( &nxTransitID),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &nStartStation),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &nEndStation),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &dTrip),            sizeof(double) );

						if(0 == nxTransitID)
							continue;

                        auto iter = m_setTransit.find(nxTransitID);
                        if (iter != m_setTransit.end())
                        {
                            auto iFind = m_mapRouteNodeSeq.find(nxTransitID);
                            if (iFind == m_mapRouteNodeSeq.end())
                                continue;

                            std::map<int, TStationODInfo>& mapSeqStn = iFind->second;
                            __int64 nxSStnID(0); {
                                auto iter = mapSeqStn.find(nStartStation);
                                if (iter == mapSeqStn.end())
                                    continue;

                                TStationODInfo& oInfo = iter->second;
                                nxSStnID = oInfo.TNodeID;
                            }

                            __int64 nxEStnID(0); {
                                auto iter = mapSeqStn.find(nEndStation);
                                if (iter == mapSeqStn.end())
                                    continue;

                                TStationODInfo& oInfo = iter->second;
                                nxEStnID = oInfo.TNodeID;
                            }

                            if (nxEStnID == 0 || nxSStnID == 0)
                                continue;

                            KODKey oKey(nxSStnID, nxEStnID);
                            double& dExsitTrip = m_mapStnOD[oKey]; {
                                dExsitTrip += dTrip;
                            }
                        }
					}
				}

				TRailSubData oRailSubData = {};
				ifs.read( reinterpret_cast<char*>( &oRailSubData), sizeof(TRailSubData));
				nCount++;
			}
		}

		ifs.close();
	}
	catch (...)
	{
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}

bool KStationODCalculatorDlg::ReadBusRailPathFile( CString strFileName )
{	
    CString strError(_T("File Protocol Error :")); {
        strError.Append(strFileName);
    }

	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{  
			int nCount(0);

			while(! ifs.eof())
			{
				int nRailIndex = -1;

				Integer nxOZoneID;
				Integer nxDZoneID;
				int		nPathID;

				ifs.read( reinterpret_cast<char*>( &nxOZoneID),             sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &nxDZoneID),				sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &nPathID),               sizeof(int) );
				if(ifs.eof())break;

				for (int i= 0; i< BUSRAILPATH_ASSIGNSET_COUNT; i++)
				{
					for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
					{
						Integer nxTransitID(0);
						int		nStartStation(0);
						int		nEndStation(0);
						double	dTrip(0);

						ifs.read( reinterpret_cast<char*>( &nxTransitID),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &nStartStation),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &nEndStation),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &dTrip),            sizeof(double) );

						if(0 == nxTransitID)
							continue;

						if(nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
						{
							nRailIndex = i;
						}

                        auto iter = m_setTransit.find(nxTransitID);
                        if (iter != m_setTransit.end())
                        {
                            auto iFind = m_mapRouteNodeSeq.find(nxTransitID);
                            if (iFind == m_mapRouteNodeSeq.end())
                                continue;

                            std::map<int, TStationODInfo>& mapSeqStn = iFind->second;
                            __int64 nxSStnID(0); {
                                auto iter = mapSeqStn.find(nStartStation);
                                if (iter == mapSeqStn.end())
                                    continue;

                                TStationODInfo& oInfo = iter->second;
                                nxSStnID = oInfo.TNodeID;
                            }

                            __int64 nxEStnID(0); {
                                auto iter = mapSeqStn.find(nEndStation);
                                if (iter == mapSeqStn.end())
                                    continue;

                                TStationODInfo& oInfo = iter->second;
                                nxEStnID = oInfo.TNodeID;
                            }

                            if (nxEStnID == 0 || nxSStnID == 0)
                                continue;

                            KODKey oKey(nxSStnID, nxEStnID);
                            double& dExsitTrip = m_mapStnOD[oKey]; {
                                dExsitTrip += dTrip;
                            }
                        }
					}
				}

				TBusSubData oBusSubData = {};
				ifs.read( reinterpret_cast<char*>( &oBusSubData), sizeof(TBusSubData) );

				if (nRailIndex != -1)
				{
					int nRailCount(0);
					ifs.read( reinterpret_cast<char*>( &(nRailCount)), sizeof(int) );

					if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
                        TxLogDebug(strError);
						throw 1;
					}

					for (int m= 0; m< nRailCount; m++)
					{
						Integer nxNoneUseOriginID;
						Integer nxNoneUseDestinationID;
						int     nNoneUsePathID;

						ifs.read( reinterpret_cast<char*>( &(nxNoneUseOriginID)),       sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(nxNoneUseDestinationID)),  sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),          sizeof(int) );

						int nBoardingCnt(0); //탑승횟수
						ifs.read( reinterpret_cast<char*>( &nBoardingCnt), sizeof(int) );
						if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
                            TxLogDebug(strError);
							throw 1;
						}

						std::vector< TIndexResultTransitInfo > vecResultRailInfo;
						for (int i= 0; i< nBoardingCnt; i++)
						{
							int nDuplicateCnt(0); //중복 노선 갯수
							ifs.read( reinterpret_cast<char*>( &nDuplicateCnt), sizeof(int) );
							if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
                                TxLogDebug(strError);
								throw 1;
							}

							for (int k= 0; k< nDuplicateCnt; k++)
							{
								Integer nxTransitID(0);
								int		nStartStation(0);
								int		nEndStation(0);
								double	dTrip(0);
								
                                ifs.read( reinterpret_cast<char*>( &nxTransitID),   sizeof(Integer) );
                                ifs.read( reinterpret_cast<char*>( &nStartStation), sizeof(int) );
                                ifs.read( reinterpret_cast<char*>( &nEndStation),   sizeof(int) );
                                ifs.read( reinterpret_cast<char*>( &dTrip),         sizeof(double) );

								if(0 == nxTransitID)
									continue;

                                auto iter = m_setTransit.find(nxTransitID);
                                if (iter != m_setTransit.end())
                                {
                                    auto iFind = m_mapRouteNodeSeq.find(nxTransitID);
                                    if (iFind == m_mapRouteNodeSeq.end())
                                        continue;

                                    std::map<int, TStationODInfo>& mapSeqStn = iFind->second;
                                    __int64 nxSStnID(0); {
                                        auto iter = mapSeqStn.find(nStartStation);
                                        if (iter == mapSeqStn.end())
                                            continue;

                                        TStationODInfo& oInfo = iter->second;
                                        nxSStnID = oInfo.TNodeID;
                                    }

                                    __int64 nxEStnID(0); {
                                        auto iter = mapSeqStn.find(nEndStation);
                                        if (iter == mapSeqStn.end())
                                            continue;

                                        TStationODInfo& oInfo = iter->second;
                                        nxEStnID = oInfo.TNodeID;
                                    }

                                    if (nxEStnID == 0 || nxSStnID == 0)
                                        continue;

                                    KODKey oKey(nxSStnID, nxEStnID);
                                    double& dExsitTrip = m_mapStnOD[oKey]; {
                                        dExsitTrip += dTrip;
                                    }
                                }
							}
						}

						TRailSubData oRailSubData = {};
						ifs.read( reinterpret_cast<char*>( &oRailSubData), sizeof(TRailSubData) ); // 맨 마지막의 RailSubData 저장
					}
				}
				nCount++;
			}
		}
		ifs.close();
	}
	catch (...)
	{
        TxLogDebug(strError);
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}

void KStationODCalculatorDlg::ResizeComponent( void )
{
	try
	{
		SetResize(IDC_STATIC1,       				  SZ_TOP_LEFT,     SZ_TOP_LEFT);
		//SetResize(IDC_EDIT1,       			      SZ_TOP_LEFT,     SZ_TOP_RIGHT);
		//SetResize(IDC_BUTTON1,       			      SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
		//SetResize(IDC_BUTTON2,       				  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);

		SetResize(IDC_STATIC2,       				  SZ_TOP_LEFT,     SZ_TOP_LEFT);
		SetResize(IDC_RADIO1,       				  SZ_TOP_LEFT,     SZ_TOP_LEFT);
		SetResize(IDC_RADIO2,       				  SZ_TOP_LEFT,     SZ_TOP_LEFT);

		SetResize(IDC_STATIC_HBar1,       			  SZ_TOP_LEFT,  SZ_TOP_RIGHT);
		SetResize(IDC_REPORT,       				  SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC_HBar2,       			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

		SetResize(IDC_BUTTON3,       				  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDOK,       						  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,       					  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KStationODCalculatorDlg::OnBnClickedRadioType( UINT nID )
{
	OnBnClickedButtonSearch();
}

void KStationODCalculatorDlg::VectorReport( void )
{/*
	//InitVector Header
	int               nItemIndex(0);
	CXTPReportColumn* pColumn = nullptr;

	//#0
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, HEADER_ORIGIN, REPORTCTL_COLUMN_WIDTH));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

	//#1
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, HEADER_DESTINATION, REPORTCTL_COLUMN_WIDTH));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

	//#2
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Value"), REPORTCTL_COLUMN_WIDTH));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

	//InitVector Data
	if (m_pRecordArray != nullptr)
	{
		delete m_pRecordArray;
		m_pRecordArray= nullptr;
	}

	std::vector<int> vecKey;	//사용된 전체 Key 값을 조회함

	std::map<int ,TStationODInfo>::iterator bIter = m_mapSeqStationInfo.begin();
	
	while (bIter != m_mapSeqStationInfo.end())
	{
		vecKey.push_back(bIter->first);

		++bIter;
	}

	m_pRecordArray = new KSDoubleRecordArray(m_wndReport.GetColumns()->GetCount());
	std::map<KIntKey, double>::iterator fIter;

	int nSize = vecKey.size();
	double dValue;

	for (int i = 0; i < nSize; ++i)
	{
		for (int j = 0; j < nSize; ++j)
		{
			KSDoubleRecord* pRecord = m_pRecordArray->AddNewRecord();

			pRecord->SetAt(0, vecKey[i]);
			pRecord->SetAt(1, vecKey[j]);

			KIntKey oIntKey(vecKey[i], vecKey[j]);
			fIter = m_mapStationData.find(oIntKey);

			if (fIter == m_mapStationData.end())
			{
				dValue = 0.0;
			}
			else
			{
				dValue = fIter->second;
			}

			pRecord->SetAt(2, dValue);
		}
	}

	m_pVectorRecord = new KStationVectorVRecord(m_pRecordArray, m_mapSeqStationInfo);
	int nRecord     = m_pRecordArray->GetRecordCount() + 1; //m_mapStationData.size()(RecordData) + 1(Sum)

	m_wndReport.SetVirtualMode(m_pVectorRecord, nRecord);
	m_wndReport.Populate();
    */
}

void KStationODCalculatorDlg::MatrixReport( void )
{
    //★ Origin Key와 Dest Key 생성
    std::set<__int64> setOriginKey, setDestKey; 

    for (auto iter = m_mapRouteNodeSeq.begin(); iter != m_mapRouteNodeSeq.end(); ++iter) {
        std::map<int, TStationODInfo>& mapInfo = iter->second;

        for (auto iter = mapInfo.begin(); iter != mapInfo.end(); ++iter) {
            TStationODInfo& oInfo = iter->second;
            if (oInfo.TStationYN == 1) {
                setOriginKey.insert(oInfo.TNodeID);
                setDestKey.insert(oInfo.TNodeID);
            }
        }
    }
    /* 값이 있는 정류장만 표현할 경우
    for (auto iter = m_mapStnOD.begin(); iter != m_mapStnOD.end(); ++iter)
    {
        KODKey oKey = iter->first; {
            setOriginKey.insert(oKey.OriginID);
            setDestKey.insert(oKey.DestinationID);
        }        
    }
    */

	int               nItemIndex(0);
	CXTPReportColumn* pColumn = nullptr;

	//#0
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("O/D"), REPORTCTL_COLUMN_WIDTH));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	
	//#1
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Sum"), REPORTCTL_COLUMN_WIDTH));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

	//#n : 컬럼 추가
    for (auto iter = setDestKey.begin(); iter != setDestKey.end(); ++iter) {
        CString strColumn(_T("")); {
            strColumn.Format(_T("%I64d"), *iter);
        }
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, strColumn, REPORTCTL_COLUMN_WIDTH));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetAlignment(DT_CENTER);
    }
    
	//InitMatrix Data
	m_vecOrigin.clear();	
    for (auto iter = setOriginKey.begin(); iter != setOriginKey.end(); ++iter) {
        m_vecOrigin.push_back(*iter);
    }

    m_vecDest.clear();
    for (auto iter = setDestKey.begin(); iter != setDestKey.end(); ++iter) {
        m_vecDest.push_back(*iter);
    }

	//CalcField
	KSDoubleRecord oRecordSum_Oi(m_vecOrigin.size() + 1 );
	KSDoubleRecord oRecordSum_Di(m_vecDest.size() + 1 );

	GetStatisticsSum_Oi(oRecordSum_Oi);
	GetStatisticsSum_Di(oRecordSum_Di);

	m_pMatrixRecord = new KStationMatrixVRecord(m_vecOrigin, m_vecDest, m_mapStnOD, oRecordSum_Oi, oRecordSum_Di);
	int nRecord     = m_vecOrigin.size() + 1; //m_mapStationData.size()(RecordData) + 1(Sum)

	m_wndReport.SetVirtualMode(m_pMatrixRecord, nRecord);
    m_wndReport.SetFreezeColumnsCount(1);
	m_wndReport.Populate();    
}

void KStationODCalculatorDlg::GetStatisticsSum_Oi( KSDoubleRecord& a_oRecordSum_Oi )
{
    double dTotalValue(0.0);

    size_t nOZoneCounts = m_vecOrigin.size();
    size_t nDZoneCounts = m_vecDest.size();

    for (size_t i = 0; i < nOZoneCounts; ++i)
    {
        double dValSum_Oi(0.0);
        for (size_t j = 0; j < nDZoneCounts; ++j)
        {
            KODKey oKey(m_vecOrigin[i], m_vecDest[j]);
            auto iter = m_mapStnOD.find( oKey );

            if (iter == m_mapStnOD.end())
                continue;

            dValSum_Oi += iter->second;
        }
        a_oRecordSum_Oi.SetAt((i + 1), dValSum_Oi);
        dTotalValue += dValSum_Oi;
    }

    a_oRecordSum_Oi.SetAt(0, dTotalValue);
}

void KStationODCalculatorDlg::GetStatisticsSum_Di( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dTotalValue(0.0);

    size_t nOZoneCounts = m_vecOrigin.size();
    size_t nDZoneCounts = m_vecDest.size();

	for (size_t i = 0; i < nDZoneCounts; ++i)
	{
		double dValSum_Oi(0.0);
		for (size_t j = 0; j < nOZoneCounts; ++j)
		{
			KODKey oKey(m_vecOrigin[j], m_vecDest[i]);
			auto iter = m_mapStnOD.find( oKey );

			if (iter == m_mapStnOD.end())
				continue;

			dValSum_Oi += iter->second;
		}
		a_oRecordSum_Oi.SetAt((i + 1), dValSum_Oi);
		dTotalValue += dValSum_Oi;
	}

	a_oRecordSum_Oi.SetAt(0, dTotalValue);
}

void KStationODCalculatorDlg::OnBnClickedButton3()
{
    try
    {
        if ( m_bEnableExport == false )
        {
            AfxMessageBox(_T("파일 내보내기를 수행 할 데이터가 존재하지 않습니다."));
            return;
        }

        CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);
        fileDlg.m_ofn.lpstrTitle = _T("Export Data");

        if (fileDlg.DoModal() != IDOK)
            return;

        CString strExportFile = fileDlg.GetPathName();
        strExportFile.Replace(_T(".txt"), _T(""));
        m_strFile.Format(_T("%s.txt"), strExportFile);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

        QBicSimpleProgressThread::ExecuteThread(ExportDataThreadCaller, this, false, nLang);

        if (m_bExportThreadOK)
            AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
        else
            AfxMessageBox(m_strErrMsg);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        AfxMessageBox(ex->GetErrorMessage());
    }
    catch (...)
    {
        TxLogDebugException();
        AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
    }
}

unsigned __stdcall KStationODCalculatorDlg::ExportDataThreadCaller( void* pParam )
{
    QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

    QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("파일로 내보내는 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Export File..."));
	}

    KStationODCalculatorDlg* pDlg = (KStationODCalculatorDlg*)pParameter->GetParameter();
    pDlg->ExportData();

    return 0;
}

void KStationODCalculatorDlg::ExportData()
{
    m_bExportThreadOK = false;

    UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
    CStdioFileEx outFile(m_strFile, nOpenMode);
    outFile.SetCodePage(65001);

    try
    {
        CString strLine(_T(""));
        CXTPReportColumns* pColumns = m_wndReport.GetColumns();

        int nColumnCount = pColumns->GetCount(); 

        for (int i=0; i<nColumnCount; i++)
        {
            CXTPReportColumn* pColumn = pColumns->GetAt(i);

            CString strCaption = pColumn->GetCaption();
            if (i == 0)
            {
                outFile.WriteString(strCaption);
            }
            else
            {
                strLine.Format(_T(", %s"), strCaption);
                outFile.WriteString(strLine);
            }
        }

        outFile.WriteString(_T("\r\n"));

        CXTPReportRecords* pRecords = m_wndReport.GetRecords();
        int                nCount   = pRecords->GetCount();
                
        for (int i=0; i<nCount; i++)
        {
            CXTPReportRecord*           pRecord = pRecords->GetAt(i);
            CXTPReportRecordItem*       pItem   = nullptr;

            strLine = _T("");
            for (int j=0; j<nColumnCount; j++)
            {
                pItem = pRecord->GetItem(j);
                
                if (IsDlgButtonChecked(IDC_RADIO1) == BST_CHECKED)
                {
                    if (j == 0)
                    {
                        strLine.AppendFormat(_T("%s"), m_pMatrixRecord->GetVirtualCaption(i, j));
                    }
                    else
                    {
                        strLine.AppendFormat(_T(", %s"), m_pMatrixRecord->GetVirtualCaption(i, j));
                    }
                }
                else
                {
                    if (j == 0)
                    {
                        strLine.AppendFormat(_T("%s"), m_pVectorRecord->GetVirtualCaption(i, j));
                    }
                    else
                    {
                        strLine.AppendFormat(_T(", %s"), m_pVectorRecord->GetVirtualCaption(i, j));
                    }
                }                
            }

            strLine.Append(_T("\r\n"));
            outFile.WriteString(strLine);
        }		

        outFile.Close();
        m_bExportThreadOK = true;
    }
    catch (KExceptionPtr ex)
    {
        outFile.Close();
        TxExceptionPrint(ex);
        m_strErrMsg = ex->GetErrorMessage();
    }
    catch (...)
    {
        outFile.Close();
        TxLogDebugException();
        m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
    }
}

/*
if (IsDlgButtonChecked(IDC_RADIO1))
{
MatrixReport();
}
else
{
VectorReport();
}
*/

void KStationODCalculatorDlg::OnBnClickedCancel()
{
	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}

void KStationODCalculatorDlg::CloseWindowProc()
{
	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();
}

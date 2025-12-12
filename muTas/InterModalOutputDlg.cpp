// InterModalOutputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterModalOutputDlg.h"
#include "afxdialogex.h"
#include "Target.h"

enum
{
	_0_COLUMN_TABLE_NAME = 0,	
	_1_TYPE_NAME,
	_2_COLUMN_NAME
};

// KInterModalOutputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterModalOutputDlg, KResizeDialogEx)

KInterModalOutputDlg::KInterModalOutputDlg(KTarget* a_pTarget, CString strDescription, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterModalOutputDlg::IDD, pParent)
	,m_pTarget(a_pTarget)
	,m_strDescription(strDescription)
{
	m_strNodeGroupName     = _T("Node");
	m_strLinkGroupName     = _T("Link") ;
	m_strLineGroupName     = _T("Line");
	m_strTransitGroupName  = _T("LineInfo");
	m_strODGroupName       = _T("Mode OD");
	m_strTerminalGroupName = _T("Terminal");

	KIOTable* pNodeTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
	if (nullptr != pNodeTable) {
		m_strNodeGroupName = pNodeTable->Caption();
	}
	KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);
	if (nullptr != pLinkTable) {
		m_strLinkGroupName = pLinkTable->Caption();
	}
	KIOTable* pTransitNodeTable = m_pTarget->Tables()->FindTable(TABLE_TRANSITNODES);
	if (nullptr != pTransitNodeTable) {
		m_strLineGroupName = pTransitNodeTable->Caption();
	}
	KIOTable* pTransitTable = m_pTarget->Tables()->FindTable(TABLE_TRANSIT);
	if (nullptr != pTransitTable) {
		m_strTransitGroupName = pTransitTable->Caption();
	}
	KIOTable* pODTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
	if (nullptr != pODTable) {
		m_strODGroupName = pODTable->Caption();
	}
	KIOTable* pTerminalTable = m_pTarget->Tables()->FindTable(TABLE_TERMINAL_SCHEDULE);
	if (nullptr != pTerminalTable) {
		m_strTerminalGroupName = pTerminalTable->Caption();
	}
}

KInterModalOutputDlg::~KInterModalOutputDlg()
{
}

void KInterModalOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,		 m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KInterModalOutputDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterModalOutputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterModalOutputDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInterModalOutputDlg 메시지 처리기입니다.


BOOL KInterModalOutputDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	KReportCtrlSetting::Default(m_wndReportCtrl, FALSE);

	InitReportHeader();
	UpdateReportData();

	CButton* pEditBox	= (CButton*)GetDlgItem(IDC_EDIT_TEXT);
	pEditBox->SetWindowTextW(m_strDescription);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterModalOutputDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TABLE_NAME, _T(""), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	//pColumn->SetGroupable(TRUE);

	m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_TYPE_NAME, _T("Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("출력 항목"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Out Field Name"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("결과 저장 컬럼명"));
	}

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(TRUE);
	pHeader->AllowColumnSort(TRUE);
}

void KInterModalOutputDlg::UpdateReportData()
{
	struct TOutputCol{
		CString strColInfo;
		CString strFileName;
	};
	TOutputCol oTOutputCol;
	oTOutputCol.strColInfo =_T("");oTOutputCol.strFileName =_T("");

	/************************************************************************/
	/* Node                                                                 */
	/************************************************************************/
	std::vector<TOutputCol> vecColInfo_Node; {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTOutputCol.strColInfo =_T("순승차(인)");oTOutputCol.strFileName =_T("IM_BoardNet");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("동일 수단간 환승승차(인)");oTOutputCol.strFileName =_T("IM_BoardTransfer");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("총승차(인)");oTOutputCol.strFileName =_T("IM_BoardTot");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("순하차(인)");oTOutputCol.strFileName =_T("IM_AlightNet");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("동일 수단간 환승하차(인)");oTOutputCol.strFileName =_T("IM_AlightTransfer");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("총하차(인)");oTOutputCol.strFileName =_T("IM_AlightTot");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("PR 승용차 도착 통행량");oTOutputCol.strFileName =_T("IM_PR_Auto");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("PR 버스 출발 통행량");oTOutputCol.strFileName =_T("IM_PR_Bus");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("PR 도시철도 출발 통행량");oTOutputCol.strFileName =_T("IM_PR_Rail");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("PR 버스+도시철도 출발 통행량");oTOutputCol.strFileName =_T("IM_PR_Transit");
			vecColInfo_Node.push_back(oTOutputCol);
		}
		else {
			oTOutputCol.strColInfo =_T("Board Net");oTOutputCol.strFileName =_T("IM_BoardNet");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Board Transfer");oTOutputCol.strFileName =_T("IM_BoardTransfer");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Board Total");oTOutputCol.strFileName =_T("IM_BoardTot");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Alight Net");oTOutputCol.strFileName =_T("IM_AlightNet");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Alight Transfer");oTOutputCol.strFileName =_T("IM_AlightTransfer");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Alight Total");oTOutputCol.strFileName =_T("IM_AlightTot");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Park and Ride Access PassengerCar");oTOutputCol.strFileName =_T("IM_PR_PassengerCar");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Park and Ride Egress Bus");oTOutputCol.strFileName =_T("IM_PR_Bus");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Park and Ride Egress Rail");oTOutputCol.strFileName =_T("IM_PR_Rail");
			vecColInfo_Node.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Park and Ride Egress Transit");oTOutputCol.strFileName =_T("IM_PR_Transit");
			vecColInfo_Node.push_back(oTOutputCol);
		}
	}
	/************************************************************************/
	/* Link                                                                 */
	/************************************************************************/
	std::vector<TOutputCol> vecColInfo_Link; {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTOutputCol.strColInfo =_T("통행시간(분)");oTOutputCol.strFileName =_T("IM_AvgTime");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("V/C");oTOutputCol.strFileName =_T("IM_V/C");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("총교통량(PCU)");oTOutputCol.strFileName =_T("IM_TotVol");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("노선버스 교통량(PCU)");oTOutputCol.strFileName =_T("IM_BusInitVol");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("대중교통 노선승객 통행량(인)");oTOutputCol.strFileName =_T("IM_Trip");
			vecColInfo_Link.push_back(oTOutputCol);
		}
		else {
			oTOutputCol.strColInfo =_T("Average Time");oTOutputCol.strFileName =_T("IM_AvgTime");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("V/C");oTOutputCol.strFileName =_T("IM_V/C");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Total Volume");oTOutputCol.strFileName =_T("IM_TotVol");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Bus Initial Volume");oTOutputCol.strFileName =_T("IM_BusInitVol");
			vecColInfo_Link.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Trip");oTOutputCol.strFileName =_T("IM_Trip");
			vecColInfo_Link.push_back(oTOutputCol);
		}
	}
	/************************************************************************/
	/* Line Info                                                            */
	/************************************************************************/
	std::vector<TOutputCol> vecColInfo_Transit; {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTOutputCol.strColInfo =_T("대중교통 노선승객 통행량(인)");oTOutputCol.strFileName =_T("IM_Trip");
			vecColInfo_Transit.push_back(oTOutputCol);
		}
		else {
			oTOutputCol.strColInfo =_T("Trip");oTOutputCol.strFileName =_T("IM_Trip");
			vecColInfo_Transit.push_back(oTOutputCol);
		}
	}
	/************************************************************************/
	/* Line                                                                 */
	/************************************************************************/
	std::vector<TOutputCol> vecColInfo_Line; {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTOutputCol.strColInfo =_T("순승차(인)");oTOutputCol.strFileName =_T("IM_BoardNet");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("동일 수단간 환승승차(인)");oTOutputCol.strFileName =_T("IM_BoardTransfer");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("총승차(인)");oTOutputCol.strFileName =_T("IM_BoardTot");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("순하차(인)");oTOutputCol.strFileName =_T("IM_AlightgNet");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("동일 수단간 환승하차(인)");oTOutputCol.strFileName =_T("IM_AlightTransfer");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("총하차(인)");oTOutputCol.strFileName =_T("IM_AlightTot");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("구간 재차인원(인)");oTOutputCol.strFileName =_T("IM_Passenger");
			vecColInfo_Line.push_back(oTOutputCol);
		}
		else {
			oTOutputCol.strColInfo =_T("Board Net");oTOutputCol.strFileName =_T("IM_BoardNet");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Board Transfer");oTOutputCol.strFileName =_T("IM_BoardTransfer");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Board Total");oTOutputCol.strFileName =_T("IM_BoardTot");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Alighting Net");oTOutputCol.strFileName =_T("IM_AlightgNet");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Alighting Transfer");oTOutputCol.strFileName =_T("IM_AlightTransfer");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Alighting Total");oTOutputCol.strFileName =_T("IM_AlightTot");
			vecColInfo_Line.push_back(oTOutputCol);
			oTOutputCol.strColInfo =_T("Passenger");oTOutputCol.strFileName =_T("IM_Passenger");
			vecColInfo_Line.push_back(oTOutputCol);
		}
	}
	/************************************************************************/
	/* Terminal                                                             */
	/************************************************************************/
	std::vector<TOutputCol> vecColInfo_Terminal; {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTOutputCol.strColInfo =_T("대중교통 노선승객 통행량(인)");oTOutputCol.strFileName =_T("IM_Trip");
			vecColInfo_Terminal.push_back(oTOutputCol);
		}
		else {
			oTOutputCol.strColInfo =_T("Trip");oTOutputCol.strFileName =_T("IM_Trip");
			vecColInfo_Terminal.push_back(oTOutputCol);
		}
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	/************************************************************************/
	/* Node                                                                 */
	/************************************************************************/
	for (size_t i= 0; i< vecColInfo_Node.size(); i++) {
		TOutputCol &oTOutputCol = vecColInfo_Node[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strColInfo));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strFileName));
		pItem->SetAlignment(DT_CENTER);
	}
	
	/************************************************************************/
	/* Link                                                                 */
	/************************************************************************/
	for (size_t i= 0; i< vecColInfo_Link.size(); i++) {
		TOutputCol &oTOutputCol = vecColInfo_Link[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strColInfo));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strFileName));
		pItem->SetAlignment(DT_CENTER);
	}
	/************************************************************************/
	/* Line Info                                                            */
	/************************************************************************/
	for (size_t i= 0; i< vecColInfo_Transit.size(); i++) {
		TOutputCol &oTOutputCol = vecColInfo_Transit[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strColInfo));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strFileName));
		pItem->SetAlignment(DT_CENTER);
	}

	/************************************************************************/
	/* Line                                                                 */
	/************************************************************************/
	for (size_t i= 0; i< vecColInfo_Line.size(); i++) {
		TOutputCol &oTOutputCol = vecColInfo_Line[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strColInfo));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strFileName));
		pItem->SetAlignment(DT_CENTER);
	}
	/************************************************************************/
	/* Terminal                                                             */
	/************************************************************************/
	for (size_t i= 0; i< vecColInfo_Terminal.size(); i++) {
		TOutputCol &oTOutputCol = vecColInfo_Terminal[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTerminalGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strColInfo));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTOutputCol.strFileName));
		pItem->SetAlignment(DT_CENTER);
	}

	/************************************************************************/
	/* Mode OD                                                              */
	/************************************************************************/
	for (size_t i = 0; i < m_vecMode.size(); i++) {
		TOutputName& oInfo = m_vecMode[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strODGroupName));
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.TCaption));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.TName));
		pItem->SetAlignment(DT_CENTER);
	}

	m_wndReportCtrl.Populate();
}


void KInterModalOutputDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_TEXT, m_strDescription);
	if (m_strDescription == _T(""))
	{
		AfxMessageBox(_T("이름을 입력해 주세요."));
		return;
	}

	KResizeDialogEx::OnOK();
}


void KInterModalOutputDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KInterModalOutputDlg::GetDescription( CString& a_strDescription )
{
	a_strDescription = m_strDescription;
}

// MAOutputDesignDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MAOutputDesignDlg.h"
#include "afxdialogex.h"
#include "MultiModalInfo.h"
#include "Target.h"


// KMAOutputDesignDlg 대화 상자입니다.

enum
{
	_0_COLUMN_TABLE_NAME = 0,	
	_1_TYPE_NAME,
	_2_COLUMN_NAME
};

IMPLEMENT_DYNAMIC(KMAOutputDesignDlg, CXTResizeDialog)

KMAOutputDesignDlg::KMAOutputDesignDlg(KTarget* a_pTarget, std::list<KMultiModalInfo> a_lstMultinfo, TAssignResultInfo& a_oMAssignResultInfo, int a_nModel, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KMAOutputDesignDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_lstMultiModalinfo(a_lstMultinfo),
	m_bUseBus(false),
	m_bUseSubway(false),
	m_bUseBusSubway(false),
	m_nModeType(a_nModel),
	m_oMAssignResultInfo(a_oMAssignResultInfo)
{
	m_strNodeGroupName     = _T("Node");
	m_strLinkGroupName     = _T("Link") ;
	m_strAttriGroupName    = _T("Attribute");
	m_strLineGroupName     = _T("Line");
	m_strTransitGroupName  = _T("LineInfo");
	m_strODGroupName       = _T("O/D");

	KIOTable* pNodeTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
	if (nullptr != pNodeTable) {
		m_strNodeGroupName = pNodeTable->Caption();
	}
	KIOTable* pLinkTable = m_pTarget->Tables()->FindTable(TABLE_LINK);
	if (nullptr != pLinkTable) {
		m_strLinkGroupName = pLinkTable->Caption();
	}
	KIOTable* pParaODTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
	if (nullptr != pParaODTable) {
		m_strAttriGroupName = pParaODTable->Caption();
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
}

KMAOutputDesignDlg::~KMAOutputDesignDlg()
{

}

void KMAOutputDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,		 m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO1,		 m_cboOverwriteInfo);
}


BEGIN_MESSAGE_MAP(KMAOutputDesignDlg, CXTResizeDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KMAOutputDesignDlg::OnCbnSelchangeCombo)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_NEW, IDC_RADIO_OVERWRITE, &KMAOutputDesignDlg::OnRadioClicked)
	ON_BN_CLICKED(IDOK, &KMAOutputDesignDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KMAOutputDesignDlg::OnBnClickedCancel)
END_MESSAGE_MAP()



BOOL KMAOutputDesignDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	KReportCtrlSetting::Default(m_wndReportCtrl);
	m_wndReportCtrl.GetColumns()->Clear();
	m_wndReportCtrl.ResetContent();

	try
	{
		m_nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);
		KDBaseAssignment::GetMulitimodalAssignResultInfo(m_pTarget, m_vecMultiAssignInfoResult);

		int     nCboIndex = -1;
		CString strMsg;

		for (int i=0; i<(int)m_vecMultiAssignInfoResult.size(); i++)
		{
			TAssignResultInfo& oAssignResultInfo = m_vecMultiAssignInfoResult[i];
			strMsg.Format(_T("%s(%s~%s)"), oAssignResultInfo.TstrDescription, oAssignResultInfo.TstrStartDate, oAssignResultInfo.TstrEndDate);
			nCboIndex = m_cboOverwriteInfo.AddString(strMsg);
			m_cboOverwriteInfo.SetItemData(nCboIndex, (DWORD_PTR)oAssignResultInfo.TnRunSeq);
		}		

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
	
	GetInputModeColumn();
	InitReportColumn();
	UpdateReportRecord();

	//button 초기화
	InitButtonInfo();

// 	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
// 	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.SetGroupRowsBold(TRUE);    
	m_wndReportCtrl.Populate();   


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KMAOutputDesignDlg::InitReportColumn()
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

void KMAOutputDesignDlg::UpdateReportRecord( void )
{
	CString strRunSeq;
	strRunSeq.Format(_T("(%d)"), m_nRunSeq);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	int nNo = 0;
	/************************************************************************/
	/* Node                                                                 */
	/************************************************************************/
	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Board Net")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("순승차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BoardNet") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Board Transfer")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("동일 수단간 환승승차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BoardTransfer") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Board Total")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("총승차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BoardTot") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Alight Net")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("순하차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AlightNet") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Alight Transfer")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("동일 수단간 환승하차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AlightTransfer") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strNodeGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Alight Total")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("총하차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AlightTot") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	/************************************************************************/
	/* Link                                                                 */
	/************************************************************************/
	if (0 == m_nModeType)
	{
		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Time")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("통행시간(분)"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgTime") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C")));
		pItem->SetAlignment(DT_CENTER);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Total Volume")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("총교통량(PCU)"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("TotVol") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		// addrow
		for (int i=0; i<m_oColumnCollection.ColumnCount(); i++)
		{
			KIOColumn* pColumn = m_oColumnCollection.GetColumn(i);
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));

			CString strMode = pColumn->Caption();
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strMode));
			pItem->SetAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strMode.Append(_T("(PCU)"));
				CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
				pItemTxt->SetValue(strMode);
			}

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pColumn->Caption() + strRunSeq));
			pItem->SetAlignment(DT_CENTER);
		}

		//addrow
		if (m_bUseBus == true || m_bUseBusSubway == true)
		{
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus Initial Volume")));
			pItem->SetAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
				pItemTxt->SetValue(_T("노선버스 교통량(PCU)"));
			}

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BusInitVol") + strRunSeq));
			pItem->SetAlignment(DT_CENTER);
		}
	}
	
	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLinkGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Trip")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("대중교통 노선승객 통행량(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Trip") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	/************************************************************************/
	/* OD                                                                   */
	/************************************************************************/
	if (m_bUseBus == false && m_bUseSubway == false && m_bUseBusSubway == true)
	{
		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strODGroupName));
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스(통행)"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strODGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Rail")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("도시철도(통행)"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Rail") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strODGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus+Rail")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스+도시철도(통행)"));
		}
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus+Rail") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	/************************************************************************/
	/* OD_Parameter                                                         */
	/************************************************************************/
	if (m_bUseBus == true)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Bus Time")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스 평균통행시간(분)"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgBusTime") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	if (m_bUseSubway = true)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Subway Time")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("도시철도 평균통행시간(분)"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgSubwayTime") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	if (m_bUseSubway == true)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Bus+Subway Time")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스+도시철도 평균통행시간(분)"));
		}
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AvgBus+SubwayTime") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	if (m_bUseBus == true)
	{
		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus Direct")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스 직승 통행량"));
		}

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus0T") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus One Transfer")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스 1회 환승 통행량"));
		}
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus1T") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);

		//addrow
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strAttriGroupName));

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus Two Transfer")));
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("버스 2회 환승 통행량"));
		}
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus2T") + strRunSeq));
		pItem->SetAlignment(DT_CENTER);
	}

	/************************************************************************/
	/* Line Info                                                            */
	/************************************************************************/
	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Trip")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("대중교통 노선승객 통행량(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Trip") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Route Length")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("총 VKT(인·Km)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Route_Len") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Route_Len_PT")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("평균 VKT(인·Km)"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Route Length per Trip") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max Board Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("최대승차인원"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max_Brd_PSG") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Board Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("평균승차인원"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Avg_Brd_PSG") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("최대재차인원"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max_PSG") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("평균재차인원"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Avg_PSG") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max Standing Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("최대입석인원"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max_Std_PSG") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Standing Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("평균입석인원"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Avg_Std_PSG") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max Crowdedness")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("최대혼잡도"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Max_Crowd") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	// addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Average Crowdedness")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("평균혼잡도"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Avg_Crowd") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Passenger per Km")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("노선Km당 평균이용객수"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("PSG_per_Km") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Passenger per Frequency")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("운행횟수당 평균이용객수"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("PSG_per_Freq") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strTransitGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Passenger per Km Frequency")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("Km당운행횟수당 평균이용객수"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("PSG_per_Km_Freq") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	/************************************************************************/
	/* Line                                                                 */
	/************************************************************************/
	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Board Net")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("순승차(인)"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BoardNet") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Board Transfer")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("동일 수단간 환승승차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BoardTransfer") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Board Total")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("총승차(인)"));
	}

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("BoardTot") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Alighting Net")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("순하차(인)"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AlightgNet") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));

	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Alighting Transfer")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("동일 수단간 환승하차(인)"));
	}
		
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AlightTransfer") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Alighting Total")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("총하차(인)"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("AlightTot") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

	//addrow
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(m_strLineGroupName));
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Passenger")));
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
		pItemTxt->SetValue(_T("구간 재차인원(인)"));
	}
	
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Passenger") + strRunSeq));
	pItem->SetAlignment(DT_CENTER);

// 	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
// 	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.SetGroupRowsBold(TRUE);    
	m_wndReportCtrl.Populate();   

}

void KMAOutputDesignDlg::GetInputModeColumn()
{
	m_oColumnCollection.Clear();

	std::list<KMultiModalInfo>::iterator iter = m_lstMultiModalinfo.begin();
	std::list<KMultiModalInfo>::iterator end  = m_lstMultiModalinfo.end();

	while(iter != end)
	{
		KMultiModalInfo oMInfo = *iter;
		if (oMInfo.TransitHighwayType() == 0)
		{
			KIOColumn* pColumn = oMInfo.ModeColumn();
			if (pColumn != nullptr)
			{
				m_oColumnCollection.AddNewColumn(pColumn);
			}
		}
		else if(oMInfo.TransitHighwayType() == 1)
		{
			m_bUseBus = true;
		}
		else if (oMInfo.TransitHighwayType() == 2)
		{
			m_bUseSubway = true;
		}
		else if (oMInfo.TransitHighwayType() == 3)
		{
			m_bUseSubway = true;
		}
		++iter;
	}
}

void KMAOutputDesignDlg::InitButtonInfo( void )
{
	CButton* pOVerRadio = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	CButton* pNewRadio	= (CButton*)GetDlgItem(IDC_RADIO_NEW);
	CButton* pEditBox	= (CButton*)GetDlgItem(IDC_EDIT_TEXT);

	if (m_oMAssignResultInfo.TbUseData == false)
	{
		if (m_vecMultiAssignInfoResult.size() > 0)
		{
			pOVerRadio->SetCheck(BST_CHECKED);
			m_cboOverwriteInfo.SetCurSel(0);
			pEditBox->EnableWindow(TRUE);
			OnCbnSelchangeCombo();
		}
		else
		{
			pNewRadio->SetCheck(BST_CHECKED);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CString strDescription = _T("");
			strDescription.Format(_T("Multi_Modal(%d)"), m_nRunSeq);
			pEditBox->SetWindowTextW(strDescription);
			pOVerRadio->EnableWindow(FALSE);
		}
	}
	else
	{
		if (m_oMAssignResultInfo.TbUseOverWrite == true)
		{
			pOVerRadio->SetCheck(BST_CHECKED);

			int nComboCount = m_cboOverwriteInfo.GetCount();
			for (int i = 0; i < nComboCount; i++)
			{
				int nComboData = m_cboOverwriteInfo.GetItemData(i);
				if (nComboData == m_oMAssignResultInfo.TnRunSeq)
				{
					m_cboOverwriteInfo.SetCurSel(i);
				}
			}
			
			pEditBox->SetWindowTextW(m_oMAssignResultInfo.TstrDescription);
		}
		else
		{
			pNewRadio->SetCheck(BST_CHECKED);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			pEditBox->SetWindowTextW(m_oMAssignResultInfo.TstrDescription);
		}
	}
}


void KMAOutputDesignDlg::OnCbnSelchangeCombo()
{
	int nCurSel = m_cboOverwriteInfo.GetCurSel();

	if (nCurSel < 0 )
	{
		nCurSel = 0;
	}
	m_cboOverwriteInfo.SetCurSel(nCurSel);

	int nRunSeq = m_cboOverwriteInfo.GetItemData(nCurSel);

	for (int i = 0; i < (int)m_vecMultiAssignInfoResult.size(); i++)
	{
		TAssignResultInfo& oMAssignInfo = m_vecMultiAssignInfoResult[i];
		if (nRunSeq == oMAssignInfo.TnRunSeq)
		{
			CString strDescription = _T("");
			strDescription.Format(_T("Multi_Modal(%d)"),m_nRunSeq);
			SetDlgItemText(IDC_EDIT_TEXT, strDescription);
		}

	}
}

void KMAOutputDesignDlg::OnRadioClicked( UINT nID )
{
	CButton* pEditBox	= (CButton*)GetDlgItem(IDC_EDIT_TEXT);
	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO_NEW, IDC_RADIO_OVERWRITE);
	if (nCheckedRadioBtn == IDC_RADIO_NEW)
	{
		m_cboOverwriteInfo.SetCurSel(-1);
		m_cboOverwriteInfo.EnableWindow(FALSE);
		CString strDescription = _T("");
		strDescription.Format(_T("Multi_Modal(%d)"),m_nRunSeq);
		pEditBox->SetWindowTextW(strDescription);
	}
	else
	{
		m_cboOverwriteInfo.EnableWindow(TRUE);
		OnCbnSelchangeCombo();
	}
}

void KMAOutputDesignDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	m_oMAssignResultInfo.TbUseData = true;
	m_oMAssignResultInfo.TnType	   = 5;

	CString strDescription = _T("");
	GetDlgItemText(IDC_EDIT_TEXT, strDescription);

	int nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO_NEW, IDC_RADIO_OVERWRITE);
	if (nCheckedRadioBtn == IDC_RADIO_NEW)
	{
		m_oMAssignResultInfo.TbUseOverWrite = false;
		m_oMAssignResultInfo.TstrDescription = strDescription;
	} else {
		int nCursel = m_cboOverwriteInfo.GetCurSel();
		if (nCursel < 0)
		{
			m_oMAssignResultInfo.TbUseOverWrite = false;
			m_oMAssignResultInfo.TstrDescription = strDescription;
		}
		else
		{
			int nRunSeq = m_cboOverwriteInfo.GetItemData(nCursel);
			for (int i = 0; i < (int)m_vecMultiAssignInfoResult.size(); i++)
			{
				TAssignResultInfo& oMAinfo = m_vecMultiAssignInfoResult[i];
				if (nRunSeq == oMAinfo.TnRunSeq)
				{
					m_oMAssignResultInfo = oMAinfo;
					m_oMAssignResultInfo.TstrDescription = strDescription;
					m_oMAssignResultInfo.TbUseOverWrite	 = true;
				}
			}
		}
	}

	CXTResizeDialog::OnOK();
}


void KMAOutputDesignDlg::OnBnClickedCancel()
{
	CXTResizeDialog::OnCancel();
}

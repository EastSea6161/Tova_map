// PrepareChartTransitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PrepareChartTransitDlg.h"
#include "afxdialogex.h"
#include "Target.h"

enum
{
	_0_COLUMN_SELECT_BOARD     = 0,
	_1_COLUMN_BOARD_NAME       = 1,
	_2_COLUMN_SELECT_ALIGHT    = 2,
	_3_COLUMN_ALIGHT_NAME      = 3
};

enum
{
	_0_A_COLUMN_SELECT     = 0,
	_1_A_COLUMN_MODELNAME  = 1
};

enum
{
	_0_B_COLUMN_SEQ     = 0,
	_1_B_COLUMN_LINK    = 1,
	_2_B_COLUMN_YN      = 2,
	_3_B_COLUMN_BUTTON  = 3
};

enum
{
	ASSIGN_MODEL_TYPE_MULTI = 5,
	ASSIGN_MODEL_TYPE_INTER = 6
};

enum
{
	LINEFIELD_PASSENGER       = 0,
	LINEFIELD_BOARD_NET       = 1,
	LINEFIELD_BOARD_TRANSFER  = 2,
	LINEFIELD_BOARD_TOTAL     = 3,
	LINEFIELD_ALIGHT_NET      = 4,
	LINEFIELD_ALIGHT_TRANSFER = 5,
	LINEFIELD_ALIGHT_TOTAL    = 6
};

IMPLEMENT_DYNAMIC(KPrepareChartTransitDlg, KResizeDialogEx)

KPrepareChartTransitDlg::KPrepareChartTransitDlg(KTarget* a_pTarget, Integer a_nxTransitID, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPrepareChartTransitDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nxDefaultTransitID(a_nxTransitID)
	, m_pTableLine(nullptr)
{
	try
	{
		m_pTableLine = m_pTarget->Tables()->FindTable(TABLE_TRANSITNODES);	
	}
	catch (KExceptionPtr ex)
	{
		m_pTableLine = nullptr;
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		m_pTableLine = nullptr;
		TxLogDebugException();
	}
}

KPrepareChartTransitDlg::~KPrepareChartTransitDlg()
{
}

void KPrepareChartTransitDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,       m_wndReportCtrl);
	DDX_Control(pDX, IDC_REPORT_A,     m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_B,     m_wndReportBCtrl);
	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);
	DDX_Control(pDX, IDC_COMBO1,       m_cboLineNo);
	DDX_Control(pDX, IDC_COMBO2,       m_cboAssignResult);
}


BEGIN_MESSAGE_MAP(KPrepareChartTransitDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL,                 &KPrepareChartTransitDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_ALL,            &KPrepareChartTransitDlg::OnBnClickedCheckAll)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,         &KPrepareChartTransitDlg::OnReportItemClick)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KPrepareChartTransitDlg::OnCbnSelchangeComboLineNo)
	ON_BN_CLICKED(IDC_BUTTON_CHART, &KPrepareChartTransitDlg::OnBnClickedButtonChart)
	ON_BN_CLICKED(IDC_CHECK_SHOWLABELS, &KPrepareChartTransitDlg::OnCheckShowLabels)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KPrepareChartTransitDlg::OnCbnSelchangeComboMultiModal)
END_MESSAGE_MAP()


// KPrepareChartTransitDlg 메시지 처리기입니다.
BOOL KPrepareChartTransitDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWaitCursor wc;

	CString strTargetName = m_pTarget->Caption();
	SetDlgItemText(IDC_STATIC_TARGET, strTargetName);
	CheckDlgButton(IDC_CHECK_SHOWLABELS, BST_UNCHECKED);

	KReportCtrlSetting::Default(m_wndReportCtrl);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);
	KReportCtrlSetting::Default(m_wndReportACtrl);
	KReportCtrlSetting::Default(m_wndReportBCtrl);
	InitReportHeader();
	InitReportAHeader();
	InitReportBHeader();
	
	LoadAssignModelInfo(m_vecModelInfo);
	InitComboAssignModelSelect(m_vecModelInfo);

	std::vector<TChartTransitInfo>    vecTransitlInfo;
	LoadTransitData(vecTransitlInfo);
	InitComboData(vecTransitlInfo);

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPrepareChartTransitDlg::LoadAssignModelInfo(std::vector<TMultiModalInfo> &a_vecMultiModalInfo)
{
	a_vecMultiModalInfo.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strQuery(_T(""));
	strQuery.Format(_T(" SELECT Description, ")
		_T(" line_board_net,  line_board_Transfer, line_board_Total, ")
		_T(" line_alight_net, line_alight_Transfer, line_alight_Total, ")
		_T(" line_passenger ")
		_T(" FROM %s WHERE type in (%d, %d) ORDER BY run_seq "),TABLE_ASSIGN_MODEL, ASSIGN_MODEL_TYPE_MULTI, ASSIGN_MODEL_TYPE_INTER);
	try
	{        
		KResultSetPtr spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);
		while ( spResultSet->Next() )
		{
			TMultiModalInfo oTMultiModalInfo;
			oTMultiModalInfo.strDescript             = spResultSet->GetValueString(0);
			oTMultiModalInfo.strFieldBoardNet        = spResultSet->GetValueString(1);
			oTMultiModalInfo.strFieldBoardTransfer   = spResultSet->GetValueString(2);
			oTMultiModalInfo.strFieldBoardTot        = spResultSet->GetValueString(3);
			oTMultiModalInfo.strFieldAlightNet       = spResultSet->GetValueString(4);
			oTMultiModalInfo.strFieldAlightTransfer  = spResultSet->GetValueString(5);
			oTMultiModalInfo.strFieldAlightTot       = spResultSet->GetValueString(6);

			oTMultiModalInfo.strFieldPassenger       = spResultSet->GetValueString(7);

			a_vecMultiModalInfo.push_back(oTMultiModalInfo);
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("ModelData 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 
}


void KPrepareChartTransitDlg::InitComboAssignModelSelect(std::vector<TMultiModalInfo> &a_vecMultiModalInfo)
{
	try
	{
		m_cboAssignResult.ResetContent();

		for (size_t i= 0; i< a_vecMultiModalInfo.size(); i++)
		{
			TMultiModalInfo &oTMultiModalInfo = a_vecMultiModalInfo[i];

			int nCur = m_cboAssignResult.AddString(oTMultiModalInfo.strDescript);
			m_cboAssignResult.SetItemData(nCur, (DWORD_PTR)&oTMultiModalInfo);
		}

		if(a_vecMultiModalInfo.size() > 0)
		{
			m_cboAssignResult.SetCurSel(0);
			UpdateReportData();
			/*UpdateTotalMinMaxData();*/
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

}


void KPrepareChartTransitDlg::InitFieldBoardAlight()
{
	try
	{
		m_mapPassengerField.clear();
		int nCurSel = m_cboAssignResult.GetCurSel();
		if(nCurSel < 0)
			return;

		TMultiModalInfo *pTMultiModalInfo = (TMultiModalInfo*)m_cboAssignResult.GetItemData(nCurSel);

		CString &strFieldPassenger        = pTMultiModalInfo->strFieldPassenger;
		CString &strFieldBoardNet         = pTMultiModalInfo->strFieldBoardNet;
		CString &strFieldBoardTransfer    = pTMultiModalInfo->strFieldBoardTransfer;
		CString &strFieldBoardTot         = pTMultiModalInfo->strFieldBoardTot;
		CString &strFieldAlightNet        = pTMultiModalInfo->strFieldAlightNet;
		CString &strFieldAlightTransfer   = pTMultiModalInfo->strFieldAlightTransfer;
		CString &strFieldAlightTot        = pTMultiModalInfo->strFieldAlightTot;

		if (m_pTableLine == nullptr)
			throw 1;

		const KIOColumns* pIOColumns = m_pTableLine->Columns();
		int nColumnCount = pIOColumns->ColumnCount();
		int nCur(-1);
		for (int i=0; i<nColumnCount; i++)
		{
			KIOColumn* pIOColumn    = pIOColumns->GetColumn(i);
			CString strColumnName   = pIOColumn->Name();

			if (strColumnName.CompareNoCase(strFieldBoardNet)       == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_BOARD_NET,       pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldBoardTransfer)  == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_BOARD_TRANSFER,  pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldBoardTot)       == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_BOARD_TOTAL,     pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldAlightNet)      == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_ALIGHT_NET,      pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldAlightTransfer) == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_ALIGHT_TRANSFER, pIOColumn));
			}
			else if (strColumnName.CompareNoCase(strFieldAlightTot)      == 0)
			{
				m_mapPassengerField.insert(std::make_pair(LINEFIELD_ALIGHT_TOTAL,    pIOColumn));
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPrepareChartTransitDlg::InitReportHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT_BOARD,   _T("Select"),        40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_BOARD_NAME,     _T("Board"),         50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("승차"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_SELECT_ALIGHT,   _T("Select"),       40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_ALIGHT_NAME,     _T("Alight"),       50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("하차"));
	}

	m_wndReportCtrl.Populate();
}


void KPrepareChartTransitDlg::InitReportAHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_SELECT,       _T("선택"),        60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_1_A_COLUMN_MODELNAME,    _T("Model"),       100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	m_wndReportACtrl.Populate();
}


void KPrepareChartTransitDlg::InitReportBHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_SEQ,       _T("Seq"),        60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("순서"));
	}

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_LINK,      _T("Node ID"),     75));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("노드 ID"));
	}

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_YN,        _T("Station"),    25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("정차 여부"));
    }

	m_wndReportBCtrl.Populate();
}


void KPrepareChartTransitDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_B,       			  SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CHART,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,						  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR5,				  SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHARTCONTROL,       		  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_SHOWLABELS,       	  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}


void KPrepareChartTransitDlg::LoadTransitData(std::vector<TChartTransitInfo> &a_vecTransitlInfo)
{
	a_vecTransitlInfo.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strQuery(_T(""));
	strQuery.Format(_T(" SELECT transit_Id, name FROM %s ORDER BY transit_Id "),TABLE_TRANSIT);
	try
	{        
		KResultSetPtr spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);
		while ( spResultSet->Next() )
		{
			TChartTransitInfo oTTransitInfo;
			oTTransitInfo.nxID       = spResultSet->GetValueInt64 (0);
			oTTransitInfo.strName    = spResultSet->GetValueString(1);

			a_vecTransitlInfo.push_back(oTTransitInfo);
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("대중교통 관련 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 
}


void KPrepareChartTransitDlg::LoadTransitLineData(Integer a_nxTransitID, std::vector<TChartDetailLineInfo> &a_vecTransitLineInfo)
{
	a_vecTransitLineInfo.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString strQuery(_T(""));
	strQuery.Format(_T(" SELECT Link_ID, Seq, Station_YN ")
					_T(" FROM %s WHERE Transit_ID = %I64d order by seq "),TABLE_TRANSITLINKS, a_nxTransitID);

	strQuery.Format(_T(" SELECT TBL1.Link_ID, FNode_Id, TNode_Id, Seq, Station_YN ")
						_T(" FROM ")
						_T(" (SELECT Link_ID, Seq, Station_YN FROM %s WHERE Transit_ID = %I64d order by seq) ")
						_T(" TBL1, ")
						_T(" (SELECT Link_ID, FNode_Id, TNode_Id FROM %s) ")
						_T(" TBL2 ")
						_T(" WHERE TBL1.Link_ID = TBL2.Link_ID "),TABLE_TRANSITLINKS, a_nxTransitID, TABLE_LINK);
	try
	{        
		KResultSetPtr spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);
		while ( spResultSet->Next() )
		{
			TChartDetailLineInfo oTTransitLineInfo;
			oTTransitLineInfo.nxLinkID       = spResultSet->GetValueInt64(0);
			oTTransitLineInfo.nxFNodeID      = spResultSet->GetValueInt64(1);
			oTTransitLineInfo.nxTNodeID      = spResultSet->GetValueInt64(2);
			oTTransitLineInfo.nSeq           = spResultSet->GetValueInt  (3);
			oTTransitLineInfo.nStationYN     = spResultSet->GetValueInt  (4);

			a_vecTransitLineInfo.push_back(oTTransitLineInfo);
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("대중교통 관련 정보 조회 중 오류가 발생하였습니다."));
		throw 1;
	} 
}


void KPrepareChartTransitDlg::LoadTransitLineByStn( __int64 nxTransitID, std::vector<TCharStnInfo>& vecTransitStnInfo )
{
    //★ 노드 타입으로 정리
    vecTransitStnInfo.clear();

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  T1.Transit_ID, T1.Seq, T1.Station_YN, T1.Link_ID, T2.fnode_id, T2.tnode_id "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Transit_Links T1, Link T2 "));
        strSQL.Append(_T(" Where "));
        strSQL.AppendFormat(_T("  T1.Transit_ID = '%I64d' And "), nxTransitID);
        strSQL.Append(_T("  T1.Link_ID = T2.Link_ID "));
    }

    KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
    while (spResult->Next()) {
        __int64 nxID      = spResult->GetValueI64(0);
        
        int     nSeq      = spResult->GetValueInt(1);
        int     nStnYN    = spResult->GetValueInt(2);
        __int64 nxLinkID  = spResult->GetValueI64(3);
        __int64 nxFNodeID = spResult->GetValueI64(4);
        __int64 nxTNodeID = spResult->GetValueI64(5);

        if (nSeq == 1) {
            TCharStnInfo oInfo; {
                oInfo.TNodeID    = nxFNodeID;
                oInfo.TStationYN = 1;
            }

            vecTransitStnInfo.push_back(oInfo);
        }
        
        TCharStnInfo oInfo; {
            oInfo.TNodeID    = nxTNodeID;
            oInfo.TStationYN = nStnYN;
        }
        vecTransitStnInfo.push_back(oInfo);
    }
}

void KPrepareChartTransitDlg::InitReportAData()
{
	CXTPReportRecord    * pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	try
	{
		for (size_t i= 0; i< m_vecModelInfo.size(); i++)
		{
			TMultiModalInfo &oTMultiModalInfo = m_vecModelInfo[i];

			pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&oTMultiModalInfo);		

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTMultiModalInfo.strDescript));
			pItem->SetAlignment(xtpColumnTextCenter);
		}

		if (m_nxDefaultTransitID > 0 && m_vecModelInfo.size() > 0)//특정 노선 우클릭으로 들어 왔다면, 디폴트로 하나는 체크 한다.
		{
			pRecord = m_wndReportACtrl.GetRecords()->GetAt(0);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);
			pItem->SetChecked(TRUE);

			if(m_vecModelInfo.size() == 1)
			{
				//CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
			}
		}

		m_wndReportACtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPrepareChartTransitDlg::InitComboData(std::vector<TChartTransitInfo> &a_vecTransitlInfo)
{
	m_cboLineNo.ResetContent();

	CString strDisplay(_T(""));
	int nDefaultCur = -1;
	for (size_t i= 0; i< a_vecTransitlInfo.size(); i++)
	{
		Integer &nxTransitID = a_vecTransitlInfo[i].nxID;
		CString &strName     = a_vecTransitlInfo[i].strName;

		strDisplay.Format(_T("%I64d : %s"), nxTransitID, strName);
		int nCur = m_cboLineNo.AddString(strDisplay);
		m_cboLineNo.SetItemData(nCur, (DWORD_PTR)nxTransitID);

		if(m_nxDefaultTransitID == nxTransitID)
		{
			nDefaultCur = nCur;
		}
	}
	m_cboLineNo.SetCurSel(nDefaultCur);

	if(nDefaultCur > -1)
	{
		OnCbnSelchangeComboLineNo();
	}
}


void KPrepareChartTransitDlg::UpdateReportData()
{
	InitFieldBoardAlight();
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		int nCur = m_cboAssignResult.GetCurSel();
		if(nCur< 0)
		{
			m_wndReportCtrl.Populate();
			return;
		}

		TMultiModalInfo *pTMultiModalInfo = (TMultiModalInfo*)m_cboAssignResult.GetItemData(nCur);
		pTMultiModalInfo->strFieldBoardNet;
		pTMultiModalInfo->strFieldBoardTransfer;
		pTMultiModalInfo->strFieldBoardTot;
		pTMultiModalInfo->strFieldAlightNet;
		pTMultiModalInfo->strFieldAlightTransfer;
		pTMultiModalInfo->strFieldAlightTot;

		CXTPReportRecord    * pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		CString strDisplayName(_T(""));
		for (int i= 0; i< 3; i++)
		{
			int nBoardIdx(0);
			int nAlightIdx(0);
			if (0 == i)
			{
				nBoardIdx  = LINEFIELD_BOARD_TOTAL;
				nAlightIdx = LINEFIELD_ALIGHT_TOTAL;
			}
			else if(1 == i)
			{
				nBoardIdx  = LINEFIELD_BOARD_TRANSFER;
				nAlightIdx = LINEFIELD_ALIGHT_TRANSFER;
			}
			else
			{
				nBoardIdx  = LINEFIELD_BOARD_NET;
				nAlightIdx = LINEFIELD_ALIGHT_NET;
			}

			CString strBoardFieldName(_T(""));
			CString strAlightFieldName(_T(""));

			GetFieldCaption(nBoardIdx,  strBoardFieldName);
			GetFieldCaption(nAlightIdx, strAlightFieldName);

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			if (strBoardFieldName.Compare(_T("")) == 0)
			{
				pItem->HasCheckbox(FALSE);
			}
			else
			{
				pItem->HasCheckbox(TRUE);
			}
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)nBoardIdx);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strBoardFieldName));
			pItem->SetAlignment(DT_LEFT);
						
			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			if (strAlightFieldName.Compare(_T("")) == 0)
			{
				pItem->HasCheckbox(FALSE);
			}
			else
			{
				pItem->HasCheckbox(TRUE);
			}
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)nAlightIdx);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strAlightFieldName));
			pItem->SetAlignment(DT_LEFT);
		}

		m_wndReportCtrl.Populate();

		UpdateCheckAll();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPrepareChartTransitDlg::GetFieldCaption(int a_nKeyIndex, CString &a_strCaption)
{
	try
	{
		AutoType iter  = m_mapPassengerField.find(a_nKeyIndex);
		AutoType itEnd = m_mapPassengerField.end();

		if(iter != itEnd)
		{
			KIOColumn* pIOColumn = iter->second;
			a_strCaption = pIOColumn->Caption();
		}
		else
		{
			a_strCaption = _T("");
		}
	}
	catch(...)
	{
		a_strCaption = _T("");
	}
}


void KPrepareChartTransitDlg::UpdateReportBData(Integer a_nxTransitID)
{
	try
	{
		m_wndReportBCtrl.ResetContent(TRUE);

        //★ 노드 기준 : LoadTransitLineByStn(__int64 nxTransitID, std::vector<TCharStnInfo>& vecTransitStnInfo);
		//★ 링크 기준
             // std::vector<TChartDetailLineInfo>  vecTransitLineInfo;
		     // LoadTransitLineData(a_nxTransitID, vecTransitLineInfo);

        std::vector<TCharStnInfo> vecTransitStnInfo; {
            LoadTransitLineByStn(a_nxTransitID, vecTransitStnInfo);
        }

		CXTPReportRecord    * pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		CString strCaption(_T(""));
		for (size_t i= 0; i< vecTransitStnInfo.size(); i++)
		{
			TCharStnInfo& oInfo = vecTransitStnInfo[i];

			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

            strCaption.Format(_T("%d"), i+1);
			pItem = pRecord->AddItem(new CXTPReportRecordItem()); {
                pItem->SetCaption(strCaption);
                pItem->SetAlignment(xtpColumnTextCenter);
            }
			
			strCaption.Format(_T("%I64d"), oInfo.TNodeID);
			pItem = pRecord->AddItem(new CXTPReportRecordItem()); {
                pItem->SetCaption(strCaption);
                pItem->SetAlignment(xtpColumnTextCenter);

                pItem->SetItemData((DWORD_PTR)oInfo.TNodeID);
            }

			int nYN = oInfo.TStationYN; {
                strCaption = (nYN == 1) ? _T("v") : _T("");
            }		

            pItem = pRecord->AddItem(new CXTPReportRecordItem()); {
                pItem->SetCaption(strCaption);
                pItem->SetAlignment(xtpColumnTextCenter);
            }
		}

		m_wndReportBCtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPrepareChartTransitDlg::OnBnClickedCheckAll()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ALL);    

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	int nCount       = pRecords->GetCount();
	for (int i= 0; i< nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = nullptr;

		pItem   = pRecord->GetItem(_0_COLUMN_SELECT_BOARD);
		pItem->SetChecked(bCheck);

		pItem   = pRecord->GetItem(_2_COLUMN_SELECT_ALIGHT);
		pItem->SetChecked(bCheck);
	}	

	m_wndReportCtrl.RedrawControl();
}


void KPrepareChartTransitDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_COLUMN_SELECT_BOARD == nIndex || _2_COLUMN_SELECT_ALIGHT == nIndex )
	{
		UpdateCheckAll();
	}
}


void KPrepareChartTransitDlg::UpdateCheckAll()
{
	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	int nCount       = pRecords->GetCount();

	int nTotalCount(0);
	int nCheckedCount(0);
	for (int i= 0; i< nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = nullptr;

		pItem   = pRecord->GetItem(_0_COLUMN_SELECT_BOARD);
		if (pItem->IsChecked() == BST_CHECKED)
			nCheckedCount++;
		if (pItem->GetHasCheckbox() == TRUE)
			nTotalCount++;

		pItem   = pRecord->GetItem(_2_COLUMN_SELECT_ALIGHT);
		if (pItem->IsChecked() == BST_CHECKED)
			nCheckedCount++;
		if (pItem->GetHasCheckbox() == TRUE)
			nTotalCount++;
	}

	if (0 == nTotalCount)
	{
		return;
	}

	UINT nCheck;
	if (nTotalCount == nCheckedCount)
		nCheck = BST_CHECKED;
	else
		nCheck = BST_UNCHECKED;

	CheckDlgButton(IDC_CHECK_ALL, nCheck);
}


void KPrepareChartTransitDlg::OnCbnSelchangeComboLineNo()
{
	UpdateData(TRUE);
	int     nCur        = m_cboLineNo.GetCurSel();
	Integer nxTransitID = (Integer)m_cboLineNo.GetItemData(nCur);
	UpdateReportBData(nxTransitID);
}


void KPrepareChartTransitDlg::OnCheckShowLabels() 
{
	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_SHOWLABELS);

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle =  (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->GetLabel()->SetVisible(bChecked);
	}
}

void KPrepareChartTransitDlg::OnBnClickedButtonChart()
{
    UpdateData(TRUE);

    try
    {
        int nCurSel = m_cboAssignResult.GetCurSel();
        if(nCurSel < 0)
            return;

        TMultiModalInfo *pTMultiModalInfo = (TMultiModalInfo*)m_cboAssignResult.GetItemData(nCurSel);

        // 1
        int nCur = m_cboLineNo.GetCurSel();		
        if(nCur < 0) {
            AfxMessageBox(_T("노선을 선택해 주세요"));
            m_cboLineNo.SetCurSel(nCur);
            return;
        }

        Integer nxTransitID = (Integer)m_cboLineNo.GetItemData(nCur);

        class TSelInfo {
        public:
            TSelInfo() {
                Key     = -1;
                Board   = true;
                TColumn = nullptr;
            }
        public:
            int  Key;
            bool Board; //★ Board, Alight Flag
        public:
            KIOColumn* TColumn; 
        };

        std::vector<TSelInfo> vecSelInfo;

        std::vector<int>  vecKeyIndex;
        std::vector<bool> vecIsAlight;

        // 2
        CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();

        for (int i= 0; i< pRecords->GetCount(); i++) {
            CXTPReportRecord*     pRecord = pRecords->GetAt(i);
            CXTPReportRecordItem* pItem   = nullptr;

            //★ Board
            pItem = pRecord->GetItem(_0_COLUMN_SELECT_BOARD);
            if (pItem->IsChecked() == TRUE) {
                int nKeyIndex = (int)pItem->GetItemData();

                //★ 안전하게
                auto iter = m_mapPassengerField.find(nKeyIndex);
                if (iter == m_mapPassengerField.end())
                    continue;

                TSelInfo oInfo; {
                    oInfo.Key     = nKeyIndex;
                    oInfo.Board   = true;
                    oInfo.TColumn = iter->second;
                }
                vecSelInfo.push_back(oInfo);
            }

            //★ Alight
            pItem = pRecord->GetItem(_2_COLUMN_SELECT_ALIGHT);
            if (pItem->IsChecked() == TRUE)
            {
                int nKeyIndex = (int)pItem->GetItemData();

                //★ 안전하게
                auto iter = m_mapPassengerField.find(nKeyIndex);
                if (iter == m_mapPassengerField.end())
                    continue;

                TSelInfo oInfo; {
                    oInfo.Key     = nKeyIndex;
                    oInfo.Board   = false;
                    oInfo.TColumn = iter->second;
                }
                vecSelInfo.push_back(oInfo);
            }
        }	


        if (vecSelInfo.size() < 1) {
            AfxMessageBox(_T("승/하차 관련 항목을 선택해 주세요."));
            return;
        }

        CWaitCursor wc;

        // 3
        KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();

        CString strSQL(_T("")); {
            strSQL.AppendFormat(_T(" Select node_id, seq, %s as Passenger "), pTMultiModalInfo->strFieldPassenger);
            for (size_t i=0; i<vecSelInfo.size(); i++) {
                TSelInfo& oInfo = vecSelInfo[i];

                KIOColumn* pIOColumn = oInfo.TColumn;
                strSQL.AppendFormat(_T(" ,%s "), pIOColumn->Name());
            }

            strSQL.AppendFormat(_T(" From %s "), TABLE_TRANSITNODES);
            strSQL.AppendFormat(_T(" Where Transit_id = '%I64d' Order By Transit_id, Seq "), nxTransitID);
        }
        
        std::map<int, TChartData> mapData; 
        for (size_t i=0; i<vecSelInfo.size(); i++) {
            TSelInfo&  oInfo     = vecSelInfo[i];
            KIOColumn* pIOColumn = oInfo.TColumn;

            TChartData& oChartData = mapData[oInfo.Key]; {
                oChartData.TCaption = pIOColumn->Caption();
            }
        }

        KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
        while (spResult->Next()) {
            __int64 nxNodeID = spResult->GetValueI64(0);
            int     nSeq     = spResult->GetValueInt(1);
            double  dPass    = spResult->GetValueDbl(2);

            for (size_t i=0; i<vecSelInfo.size(); i++) {
                double dValue = spResult->GetValueDbl(3+ (int)i);
                TSelInfo& oInfo = vecSelInfo[i];

                //★ 첫번째 정류장의 재차량을 첫번째 정류장의 Boarding 값에 더한다.
                //★ 명지대학교 한혁(2016-11-01)
                if (nSeq == 1 && oInfo.Board == true) {
                    dValue += dPass;
                }

                TChartData& oChartData = mapData[oInfo.Key]; {
                    std::vector<double>& vecValue = oChartData.TData;
                    vecValue.push_back(dValue);
                }
            }
        }
        
        //★ 차트 생성
        CXTPChartContent* pContent = CreateChart(mapData); {
            m_wndChartControl.SetContent(pContent);
        }

        OnCheckShowLabels();
    }
    catch(KExceptionPtr ex)
    {
        TxLogDebugException();
        AfxMessageBox(ex->GetErrorMessage());
    }
    catch(...)
    {
        TxLogDebugException();
    }	
}

/*
void KPrepareChartTransitDlg::OnBnClickedButtonChart()
{
	UpdateData(TRUE);

	try
	{
		// 1
		int nCur = m_cboLineNo.GetCurSel();		
		if(nCur < 0) {
			AfxMessageBox(_T("노선을 선택해 주세요"));
			m_cboLineNo.SetCurSel(nCur);
			return;
		}

        Integer nxTransitID = (Integer)m_cboLineNo.GetItemData(nCur);

        class TSelInfo {
        public:
            TSelInfo() {
                Key     = -1;
                Board   = true;
                TColumn = nullptr;
            }
        public:
            int  Key;
            bool Board; //★ Board, Alight Flag
        public:
            KIOColumn* TColumn; 
        };

        std::vector<TSelInfo> vecSelInfo;
		
        std::vector<int>  vecKeyIndex;
        std::vector<bool> vecIsAlight;
		
        // 2
		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();

		for (int i= 0; i< pRecords->GetCount(); i++) {
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = nullptr;

            //★ Board
			pItem = pRecord->GetItem(_0_COLUMN_SELECT_BOARD);
			if (pItem->IsChecked() == TRUE) {
				int nKeyIndex = (int)pItem->GetItemData();

                //★ 안전하게
                auto iter = m_mapPassengerField.find(nKeyIndex);
                if (iter == m_mapPassengerField.end())
                    continue;

                TSelInfo oInfo; {
                    oInfo.Key     = nKeyIndex;
                    oInfo.Board   = true;
                    oInfo.TColumn = iter->second;
                }
                vecSelInfo.push_back(oInfo);
			}

            //★ Alight
			pItem = pRecord->GetItem(_2_COLUMN_SELECT_ALIGHT);
			if (pItem->IsChecked() == TRUE)
			{
				int nKeyIndex = (int)pItem->GetItemData();

                //★ 안전하게
                auto iter = m_mapPassengerField.find(nKeyIndex);
                if (iter == m_mapPassengerField.end())
                    continue;

                TSelInfo oInfo; {
                    oInfo.Key     = nKeyIndex;
                    oInfo.Board   = false;
                    oInfo.TColumn = iter->second;
                }
                vecSelInfo.push_back(oInfo);
			}
		}	


		if (vecSelInfo.size() < 1) {
			AfxMessageBox(_T("승/하차 관련 항목을 선택해 주세요."));
			return;
		}

		CWaitCursor wc;

		// 3
		KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();
		
        CString strSQL(_T("")); {
		    strSQL.Append(_T(" Select link_id, seq "));
            for (size_t i=0; i<vecSelInfo.size(); i++) {
                TSelInfo& oInfo = vecSelInfo[i];
                
                KIOColumn* pIOColumn = oInfo.TColumn;
                strSQL.AppendFormat(_T(" ,%s "), pIOColumn->Name());
            }

            strSQL.AppendFormat(_T(" From %s "), TABLE_TRANSITLINKS); //★ 주석
            strSQL.AppendFormat(_T(" Where Transit_id = '%I64d' Order By Transit_id, Seq "), nxTransitID);
        }
		
        //★ 승차는 마지막에 0을 추가
        //★ 하차는 맨앞에 0을 추가
        std::map<int, TChartData> mapData; 
        for (size_t i=0; i<vecSelInfo.size(); i++) {
            TSelInfo&  oInfo     = vecSelInfo[i];
            KIOColumn* pIOColumn = oInfo.TColumn;

            TChartData& oChartData = mapData[oInfo.Key]; {
                oChartData.TCaption = pIOColumn->Caption();
            }
            
            //★ 하차는 맨앞에 0을 추가
            if (oInfo.Board == false) {
                std::vector<double>& vecValue = oChartData.TData;
                vecValue.push_back(0);
            }
        }

        //★ 하차는 이미 첫번째에 0이 추가되었다.
        KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
        while (spResult->Next()) {
            __int64 nxLinkID = spResult->GetValueI64(0);
            int     nSeq     = spResult->GetValueInt(1);

            for (size_t i=0; i<vecSelInfo.size(); i++) {
                double dValue = spResult->GetValueDbl(2+ (int)i);
                TSelInfo& oInfo = vecSelInfo[i];
                TChartData& oChartData = mapData[oInfo.Key]; {
                    std::vector<double>& vecValue = oChartData.TData;
                    vecValue.push_back(dValue);
                }
            }
        }

        //★ 승차 정보 마지막에 0값을 추가
        for (size_t i=0; i<vecSelInfo.size(); i++) {
            TSelInfo& oInfo = vecSelInfo[i];
            if (oInfo.Board == true) {
                TChartData& oChartData = mapData[oInfo.Key]; {
                    std::vector<double>& vecValue = oChartData.TData;
                    vecValue.push_back(0);
                }
            }            
        }

        //★ 차트 생성
		CXTPChartContent* pContent = CreateChart(mapData); {
            m_wndChartControl.SetContent(pContent);
        }
		
		OnCheckShowLabels();
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}	
}
*/
CXTPChartContent* KPrepareChartTransitDlg::CreateChart( std::map<int, TChartData>& mapChartData )
{
    CXTPChartContent* pChartContent = new CXTPChartContent(); {
        KChartCtrlSetting::DefaultContent(pChartContent, _T(""));
        pChartContent->SetPanelDirection(xtpChartPanelVertical);
    }

    CXTPChartLegend* pLegend = pChartContent->GetLegend(); {
        pLegend->SetVerticalAlignment(xtpChartLegendNear);  //default : xtpChartLegendNear
        pLegend->SetHorizontalAlignment(xtpChartLegendNear); //default : xtpChartLegendFarOutside
        pLegend->SetOffset(CPoint(30, 0));
    }

    CXTPChartSeriesCollection* pSeriesCollection = pChartContent->GetSeries();
    CXTPChartSeries*           pSeries           = nullptr;

    try
    {
        //★ 2D 다이아 그램 추가
        CXTPChartDiagram2D* pChartDiagram = (CXTPChartDiagram2D*)pChartContent->GetPanels()->Add(new CXTPChartDiagram2D()); {
            KChartCtrlSetting::DefaultDiagram2D(pChartDiagram, _T(""), _T(""));
            pChartDiagram->SetRotated(FALSE);
        }

        for (auto iter = mapChartData.begin(); iter != mapChartData.end(); ++iter) {
            TChartData& oInfo = iter->second;

            std::vector<double>& vecData =oInfo.TData;

            //★ ChartSeries
            pSeries = pSeriesCollection->Add(new CXTPChartSeries()); {
                KChartCtrlSetting::DefaultSeriesArea(pSeries, oInfo.TCaption);
            }

            double dAddValue  = 0.0;
            for (size_t i= 0; i<vecData.size(); i++) {                
                int nSeq = (int)i + 1;
                //★ 누적                
                dAddValue += vecData[i];

                CXTPChartSeriesPoint* pChartPoint = new CXTPChartSeriesPoint(nSeq, dAddValue); {
                    pSeries->GetPoints()->Add(pChartPoint);
                }				
            }

            pSeries->SetDiagram(pChartDiagram);
        }
    }
    catch (...)
    {
        return nullptr;
    }

    return pChartContent;
}

void KPrepareChartTransitDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KPrepareChartTransitDlg::OnCbnSelchangeComboMultiModal()
{
	UpdateReportData();
}

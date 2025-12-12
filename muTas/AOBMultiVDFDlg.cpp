// AOBMultiVDFDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AOBMultiVDFDlg.h"
#include "afxdialogex.h"
//^#include "DBaseConnector.h"
//^^#include "SEDData.h"

#include "Target.h"
#include "ABOMultiFnManagerDlg.h"

#include "CustomReportRecordItem.h"

#include "Mode.h"
#define DATA_CHECK		_T("v")
#define DATA_UNCHECK	_T("")


// KAOBMultiVDFDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAOBMultiVDFDlg, KDialogEx)

KAOBMultiVDFDlg::KAOBMultiVDFDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KAOBMultiVDFDlg::IDD, pParent)
	, m_ChkAlphaBeta(FALSE)
	, m_ChkabToll(FALSE)
	, m_ChkInitialTime(FALSE)
	, m_RadioFunction(0)
	,m_nComboLinkAllAlphaCursor(0)
	,m_nComboLinkAllBetaCursor(0)
	,m_nComboLinkAllACursor(0)
	,m_nComboLinkAllBCursor(0)
	,m_nComboLinkAllTCursor(0)
	,m_pTarget(a_pTarget)
{

}

KAOBMultiVDFDlg::~KAOBMultiVDFDlg()
{
}

void KAOBMultiVDFDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndMatch);
	DDX_Control(pDX, IDC_REPORT2, m_wndVOT);
	DDX_Control(pDX, IDC_REPORT_FN, m_wndFunction);
	DDX_Control(pDX, IDC_REPORT_EL, m_wndEachLink);

	DDX_Check(pDX, IDC_CHECK1, m_ChkAlphaBeta);
	DDX_Check(pDX, IDC_CHECK2, m_ChkabToll);
	DDX_Check(pDX, IDC_CHECK3, m_ChkInitialTime);
	DDX_Radio(pDX, IDC_RADIO2, m_RadioFunction);
	DDX_Control(pDX, IDC_COMBO_ALPHA, m_comboLinkAlpha);
	DDX_Control(pDX, IDC_COMBO_BETA, m_comboLinkBeta);
	DDX_Control(pDX, IDC_COMBO_A, m_comboLinkA);
	DDX_Control(pDX, IDC_COMBO_B, m_comboLinkB);
	//DDX_Control(pDX, IDC_COMBO_T, m_comboLinkT);
}


BEGIN_MESSAGE_MAP(KAOBMultiVDFDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnMatchDataClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT2, OnVOTDataClick)
	ON_BN_CLICKED(IDC_CHECK1, &KAOBMultiVDFDlg::OnBnClickedAlphaBeta)
	ON_BN_CLICKED(IDC_CHECK2, &KAOBMultiVDFDlg::OnBnClickedAB)
	ON_BN_CLICKED(IDC_CHECK3, &KAOBMultiVDFDlg::OnBnClickedInitialTime)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO2, IDC_RADIO3, &KAOBMultiVDFDlg::OnBnClickedRadio)
	ON_BN_CLICKED(ID_BUTTON_SETBPRFN, &KAOBMultiVDFDlg::OnBnClickedButtonSetbprfn)
END_MESSAGE_MAP()


// KAOBMultiVDFDlg 메시지 처리기입니다.


BOOL KAOBMultiVDFDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAOBMultiVDFDlg::SetInitData()
{
	InitVOTReport();
	UpdateVOTReport();

	InitMatchReport();
	UpdateMatchReport();

	InitComboboxEachLink();

	SetFunctionColumnName();
	CheckRadioButton();
}

void KAOBMultiVDFDlg::SetInitEachLinkData()
{
	InitComboboxEachLink();

	UpdateData(TRUE);

	GetDlgItem(IDC_COMBO_ALPHA)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_BETA)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_A)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_B)->EnableWindow(false);
	GetDlgItem(IDC_REPORT_EL)->EnableWindow(TRUE);


	switch(m_RadioFunction)
	{
	case 1:
		GetDlgItem(IDC_COMBO_ALPHA)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_BETA)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_A)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_B)->EnableWindow(true);
		GetDlgItem(IDC_REPORT_EL)->EnableWindow(TRUE);

		if (m_ChkAlphaBeta == TRUE)
		{
			GetDlgItem(IDC_COMBO_ALPHA)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_BETA)->EnableWindow(false);
		}

		if (m_ChkabToll == TRUE)
		{
			GetDlgItem(IDC_COMBO_A)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_B)->EnableWindow(false);
		}

		if (m_ChkInitialTime == TRUE)
		{
		}
		return;
	default:
		return;
	}
	UpdateData(FALSE);
}

void KAOBMultiVDFDlg::InitVOTReport( void )
{
	m_wndVOT.GetColumns()->Clear();
	m_wndVOT.ResetContent();

	KReportCtrlSetting::Default(m_wndVOT, TRUE);

	CXTPReportColumn* pColumn = NULL;
	
	pColumn = m_wndVOT.AddColumn(new CXTPReportColumn(0, _T(""), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("설정항목"));
    }

	for (int i = 0; i < m_nModeNumInVDF; ++i)
	{
		CString strColName;
		strColName.Format(_T("VDF%d"), (i+1));

		pColumn = m_wndVOT.AddColumn(new CXTPReportColumn(i+1, strColName, 1));
		pColumn->SetHeaderAlignment(DT_CENTER);
		//pColumn->SetAlignment(xtpColumnIconCenter);
	}

	m_wndVOT.AllowEdit(TRUE);
	m_wndVOT.EditOnClick(TRUE);
}


void KAOBMultiVDFDlg::UpdateVOTReport( void )
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	pRecord = m_wndVOT.AddRecord(new CXTPReportRecord);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText( _T("Standard Initial Time") ));
	pItem->SetEditable(FALSE);
	pItem->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("최초 배정 지체함수"));
    }
	
	for (int i = 0; i < m_nModeNumInVDF; ++i)
	{
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->HasCheckbox(TRUE);
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		if ( i == 0)
		{
			pItem->SetChecked(TRUE);
		}
	}
	pRecord = m_wndVOT.AddRecord(new CXTPReportRecord);
	pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Value of Time(V/M)")));
	pItem->SetAlignment(DT_CENTER);
	pItem->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pItem->SetCaption(_T("VOT(원/분)"));
    }

	for (int i = 0; i < m_nModeNumInVDF; ++i)
	{
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.5f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_RIGHT);
	}

	m_wndVOT.Populate();
}


void KAOBMultiVDFDlg::OnVOTDataClick( NMHDR * a_pNotifyStruct, LRESULT * a_result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;

	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	int nSelectedColIndex = pItemNotify->pColumn->GetItemIndex();
	int nSelectedRowIndex = pItemNotify->pRow->GetIndex();

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	int nColumnCount = pRecord->GetItemCount();

	// 'target', 'status' 필드 는 제외 
	if( nSelectedColIndex == 0 )
	{
		return;
	}

	if (nSelectedRowIndex == 1)
	{
		return;
	}

	CXTPReportRecordItem* pItem = pRecord->GetItem(nSelectedColIndex);

	for(int i= 1; i< nColumnCount; i++)
	{
		BOOL BCheck = FALSE;
		if(i == nSelectedColIndex)
		{
			BCheck = TRUE;
		}
		else 
		{
			BCheck = FALSE;
		}
		pItem = pRecord->GetItem(i);
		pItem->SetChecked(BCheck);
	}
	m_wndVOT.Populate();
}


void KAOBMultiVDFDlg::SetModeNumInVDF( int a_nModeNumInVDF )
{
	m_nModeNumInVDF = a_nModeNumInVDF;
}


void KAOBMultiVDFDlg::InitMatchReport( void )
{
	m_wndMatch.GetColumns()->Clear();
	m_wndMatch.ResetContent();

	KReportCtrlSetting::Default(m_wndMatch, TRUE);

	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndMatch.AddColumn(new CXTPReportColumn(0, _T("Class"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	for (int i = 0; i < m_nModeNumInVDF; ++i)
	{
		CString strColName;
		strColName.Format(_T("VDF%d"), (i+1));

		pColumn = m_wndMatch.AddColumn(new CXTPReportColumn(i+1, strColName, 1));
		pColumn->SetHeaderAlignment(DT_CENTER);
		//pColumn->SetAlignment(xtpColumnIconCenter);
	}
}


void KAOBMultiVDFDlg::UpdateMatchReport( void )
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	std::list<KAInputInfo*>::iterator itInput, itEnd = m_InputList.end();

	int nInputIndex = 0;

	for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput  = *itInput;

		if (!pInput->Selected())
		{
			continue;
		}

		KMode* pMode = pInput->Mode();
		CString strColName = pMode->ModeName();
		
		pRecord = m_wndMatch.AddRecord(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText( strColName ));
		pItem->SetAlignment(DT_CENTER);

		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->HasCheckbox(TRUE);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			if (i == nInputIndex)
			{
				pItem->SetChecked(TRUE);
			}

			if (nInputIndex > m_nModeNumInVDF || nInputIndex == m_nModeNumInVDF)
			{
				if (i == m_nModeNumInVDF - 1)
				{
					pItem->SetChecked(TRUE);
				}
			}

		}
		nInputIndex ++;

	}
	m_wndMatch.Populate();
}


void KAOBMultiVDFDlg::SetInputList( std::list<KAInputInfo*>& inputList )
{
	m_InputList = inputList;
}


void KAOBMultiVDFDlg::OnMatchDataClick( NMHDR * a_pNotifyStruct, LRESULT * a_result )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;
	
	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	int nSelectedColIndex = pItemNotify->pColumn->GetItemIndex();
	int nSelectedRowIndex = pItemNotify->pRow->GetIndex();

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	int nColumnCount = pRecord->GetItemCount();

	// 'target', 'status' 필드 는 제외 
	if( nSelectedColIndex == 0 )
	{
		return;
	}

	for(int i= 1; i< nColumnCount; i++)
	{
		BOOL BCheck = FALSE;
		if(i == nSelectedColIndex)
		{
			BCheck = TRUE;
		}
		else 
		{
			BCheck = FALSE;
		}
		CXTPReportRecordItem* pItem = pRecord->GetItem(i);
		pItem->SetChecked(BCheck);
	}

	m_wndMatch.Populate();
}


void KAOBMultiVDFDlg::ChangedEachLinkparameter( void )
{
	UpdateData(TRUE);

	GetDlgItem(IDC_COMBO_ALPHA)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_BETA)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_A)->EnableWindow(true);
	GetDlgItem(IDC_COMBO_B)->EnableWindow(true);
	GetDlgItem(IDC_REPORT_EL)->EnableWindow(TRUE);

	if (m_ChkAlphaBeta == TRUE)
	{
		GetDlgItem(IDC_COMBO_ALPHA)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_BETA)->EnableWindow(false);
	}

	if (m_ChkabToll == TRUE)
	{
		GetDlgItem(IDC_COMBO_A)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_B)->EnableWindow(false);
	}

	if (m_ChkInitialTime == TRUE)
	{
	}
	InitEachLinkReport();
	UpdateData(FALSE);
}


void KAOBMultiVDFDlg::SetFunctionColumnName( void )
{
	UpdateData(TRUE);
	m_vecInitFunctionColumn.clear();
	m_vecInitEachLinkColumn.clear();

	if (m_ChkAlphaBeta == TRUE && m_ChkabToll == TRUE && m_ChkInitialTime == TRUE)
	{
		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strAlphaColName;
			strAlphaColName.Format(_T("VDF%d_α"), (i+1));
			m_vecInitFunctionColumn.push_back(strAlphaColName);
			m_vecInitEachLinkColumn.push_back(strAlphaColName);


			CString strBetaColName;
			strBetaColName.Format(_T("VDF%d_β"), (i+1));
			m_vecInitFunctionColumn.push_back(strBetaColName);
			m_vecInitEachLinkColumn.push_back(strBetaColName);

			CString strAColName;
			strAColName.Format(_T("VDF%d_a"), (i+1));
			m_vecInitFunctionColumn.push_back(strAColName);
			m_vecInitEachLinkColumn.push_back(strAColName);

			CString strBColName;
			strBColName.Format(_T("VDF%d_b"), (i+1));
			m_vecInitFunctionColumn.push_back(strBColName);
			m_vecInitEachLinkColumn.push_back(strBColName);

		}


		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strTimeColName;
			strTimeColName.Format(_T("VDF%d_t"), (i+1));
			m_vecInitFunctionColumn.push_back(strTimeColName);
			m_vecInitEachLinkColumn.push_back(strTimeColName);
		}
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == TRUE && m_ChkInitialTime == FALSE)
	{
		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strAlphaColName;
			strAlphaColName.Format(_T("VDF%d_α"), (i+1));
			m_vecInitFunctionColumn.push_back(strAlphaColName);
			m_vecInitEachLinkColumn.push_back(strAlphaColName);


			CString strBetaColName;
			strBetaColName.Format(_T("VDF%d_β"), (i+1));
			m_vecInitFunctionColumn.push_back(strBetaColName);
			m_vecInitEachLinkColumn.push_back(strBetaColName);

			CString strAColName;
			strAColName.Format(_T("VDF%d_a"), (i+1));
			m_vecInitFunctionColumn.push_back(strAColName);
			m_vecInitEachLinkColumn.push_back(strAColName);

			CString strBColName;
			strBColName.Format(_T("VDF%d_b"), (i+1));
			m_vecInitFunctionColumn.push_back(strBColName);
			m_vecInitEachLinkColumn.push_back(strBColName);
		}

		//m_vecInitFunctionColumn.push_back(_T("t"));
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == FALSE && m_ChkInitialTime == TRUE)
	{
		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strAlphaColName;
			strAlphaColName.Format(_T("VDF%d_α"), (i+1));
			m_vecInitFunctionColumn.push_back(strAlphaColName);
			m_vecInitEachLinkColumn.push_back(strAlphaColName);


			CString strBetaColName;
			strBetaColName.Format(_T("VDF%d_β"), (i+1));
			m_vecInitFunctionColumn.push_back(strBetaColName);
			m_vecInitEachLinkColumn.push_back(strBetaColName);
		}

		m_vecInitFunctionColumn.push_back(_T("a"));
		m_vecInitFunctionColumn.push_back(_T("b"));

		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strTimeColName;
			strTimeColName.Format(_T("VDF%d_t"), (i+1));
			m_vecInitFunctionColumn.push_back(strTimeColName);
			m_vecInitEachLinkColumn.push_back(strTimeColName);
		}

	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == TRUE && m_ChkInitialTime == TRUE)
	{
		m_vecInitFunctionColumn.push_back(_T("α"));
		m_vecInitFunctionColumn.push_back(_T("β"));

		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strAColName;
			strAColName.Format(_T("VDF%d_a"), (i+1));
			m_vecInitFunctionColumn.push_back(strAColName);
			m_vecInitEachLinkColumn.push_back(strAColName);

			CString strBColName;
			strBColName.Format(_T("VDF%d_b"), (i+1));
			m_vecInitFunctionColumn.push_back(strBColName);
			m_vecInitEachLinkColumn.push_back(strBColName);

			CString strTimeColName;
			strTimeColName.Format(_T("VDF%d_t"), (i+1));
			m_vecInitFunctionColumn.push_back(strTimeColName);
			m_vecInitEachLinkColumn.push_back(strTimeColName);
		}
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == FALSE && m_ChkInitialTime == FALSE)
	{
		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strAlphaColName;
			strAlphaColName.Format(_T("VDF%d_α"), (i+1));
			m_vecInitFunctionColumn.push_back(strAlphaColName);
			m_vecInitEachLinkColumn.push_back(strAlphaColName);


			CString strBetaColName;
			strBetaColName.Format(_T("VDF%d_β"), (i+1));
			m_vecInitFunctionColumn.push_back(strBetaColName);
			m_vecInitEachLinkColumn.push_back(strBetaColName);
		}

		m_vecInitFunctionColumn.push_back(_T("a"));
		m_vecInitFunctionColumn.push_back(_T("b"));
		//m_vecInitFunctionColumn.push_back(_T("t"));

	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == TRUE && m_ChkInitialTime == FALSE)
	{
		m_vecInitFunctionColumn.push_back(_T("α"));
		m_vecInitFunctionColumn.push_back(_T("β"));
		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strAColName;
			strAColName.Format(_T("VDF%d_a"), (i+1));
			m_vecInitFunctionColumn.push_back(strAColName);
			m_vecInitEachLinkColumn.push_back(strAColName);

			CString strBColName;
			strBColName.Format(_T("VDF%d_b"), (i+1));
			m_vecInitFunctionColumn.push_back(strBColName);
			m_vecInitEachLinkColumn.push_back(strBColName);
		}
		//m_vecInitFunctionColumn.push_back(_T("t"));
	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == FALSE && m_ChkInitialTime == TRUE)
	{
		m_vecInitFunctionColumn.push_back(_T("α"));
		m_vecInitFunctionColumn.push_back(_T("β"));
		m_vecInitFunctionColumn.push_back(_T("a"));
		m_vecInitFunctionColumn.push_back(_T("b"));
		
		for (int i = 0; i < m_nModeNumInVDF; ++i)
		{
			CString strTimeColName;
			strTimeColName.Format(_T("VDF%d_t"), (i+1));
			m_vecInitFunctionColumn.push_back(strTimeColName);
			m_vecInitEachLinkColumn.push_back(strTimeColName);
		}

	}
	else
	{
		m_vecInitFunctionColumn.push_back(_T("α"));
		m_vecInitFunctionColumn.push_back(_T("β"));
		m_vecInitFunctionColumn.push_back(_T("a"));
		m_vecInitFunctionColumn.push_back(_T("b"));
		//m_vecInitFunctionColumn.push_back(_T("t"));
	}

	InitFunctionTypeReport();
	InitEachLinkReport();

}


void KAOBMultiVDFDlg::InitFunctionTypeReport()
{
	m_wndFunction.GetColumns()->Clear();
	m_wndFunction.ResetContent();

	KReportCtrlSetting::Default(m_wndFunction, TRUE);

	CXTPReportColumn* pColumn = NULL;

	int nIndex = 0;

	pColumn = m_wndFunction.AddColumn(new CXTPReportColumn(nIndex, _T("Fn. No."), 70));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("함수번호"));
    }

	size_t nCount = m_vecInitFunctionColumn.size();

	for (size_t i = 0; i < nCount ; i++)
	{
		++nIndex;
		CString strColumnName = m_vecInitFunctionColumn[i];
		pColumn = m_wndFunction.AddColumn(new CXTPReportColumn(nIndex, strColumnName, 70));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
	}

	m_wndFunction.Populate();
}


void KAOBMultiVDFDlg::UpdateFunctionTypeReport()
{
	m_wndFunction.ResetContent();
	int nRecordCount = m_vecFunctionRows.size();

	for (int i = 0; i < nRecordCount; ++i)
	{
		std::vector<CString> row = m_vecFunctionRows[i];
		int nColumnCount = row.size();

		CXTPReportRecord* pReportRecord = m_wndFunction.AddRecord(new CXTPReportRecord);

		__int64 nxNo = _ttoi64(row[0]);
		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNo));
		pRecordItem->SetEditable(FALSE);

		for (int nIndex = 1; nIndex < nColumnCount; nIndex++)
		{
			double dValue = _ttof(row[nIndex]);
			pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%g")));
		}
	}
	m_wndFunction.Populate();
}


void KAOBMultiVDFDlg::InitEachLinkReport()
{
	m_wndEachLink.ResetContent();
	m_wndEachLink.GetColumns()->Clear();
	
	KReportCtrlSetting::Default(m_wndEachLink, TRUE);

	CXTPReportColumn* pColumn = NULL;

	int nIndex = 0;

	size_t nCount = m_vecInitEachLinkColumn.size();

	for (size_t i = 0; i < nCount ; i++)
	{
		++nIndex;
		CString strColumnName = m_vecInitEachLinkColumn[i];
		pColumn = m_wndEachLink.AddColumn(new CXTPReportColumn(nIndex, strColumnName, 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

		CXTPReportRecordItemEditOptions* pEditOptions   = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

		KIOTables* pTables = m_pTarget->Tables();
		KIOTable* pTable = pTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns = pTable->Columns();

		int nColumnCount = pColumns->ColumnCount();

		if (strColumnName.Right(1) == "α" || strColumnName.Right(1) == "β" )
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pEditOptions->AddConstraint(_T("선택"), NULL);
			}
			else {
				pEditOptions->AddConstraint(_T("Select"), NULL);
			}
		}
		else
		{
			pEditOptions->AddConstraint(_T("0.0"), NULL);
		}

		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
			}

		}
	}
	
	UpdateEachLinkReport();
	
	m_wndEachLink.Populate();
}


void KAOBMultiVDFDlg::UpdateEachLinkReport( void )
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	int i		= -1;	
	int nIndex  = m_wndEachLink.GetColumns()->GetCount();
	pRecord		= m_wndEachLink.AddRecord(new CXTPReportRecord);

	while( i < nIndex)
	{
		CXTPReportRecordItem* pItem  = pRecord->AddItem(new CCustomReportRecordItem);
		//pItem->SetItemData(-1);
		pItem->SetAlignment(DT_CENTER);
		i++;
	}

	m_wndEachLink.Populate();
 	m_wndEachLink.ExpandAll();
}


void KAOBMultiVDFDlg::OnBnClickedAlphaBeta()
{
	SetFunctionColumnName();
	CheckRadioButton();
	m_vecFunctionRows.clear();
}


void KAOBMultiVDFDlg::OnBnClickedAB()
{
	SetFunctionColumnName();
	CheckRadioButton();
	m_vecFunctionRows.clear();
}


void KAOBMultiVDFDlg::OnBnClickedInitialTime()
{
	SetFunctionColumnName();
	CheckRadioButton();
	m_vecFunctionRows.clear();
}


void KAOBMultiVDFDlg::CheckRadioButton()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_ALPHA)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_BETA)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_A)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_B)->EnableWindow(false);
	GetDlgItem(IDC_REPORT_EL)->EnableWindow(FALSE);
	//GetDlgItem(IDC_COMBO_T)->EnableWindow(false);
	CButton* pFnBtn = (CButton*)GetDlgItem(ID_BUTTON_SETBPRFN);
	pFnBtn->EnableWindow(FALSE);

	switch(m_RadioFunction)
	{
	case 0:
		pFnBtn->EnableWindow(TRUE);
		return;
	
	case 1:
		ChangedEachLinkparameter();
		return;
	
	default:
		return;
	}
	UpdateData(FALSE);
}


void KAOBMultiVDFDlg::OnBnClickedRadio( UINT nID )
{
	CheckRadioButton();
}


void KAOBMultiVDFDlg::InitComboboxEachLink( void )
{
	int nAlphaIndex(0);
	int nBetaIndex (0);
	int nAIndex	   (0);
	int nBIndex	   (0);

	try
	{
		m_comboLinkAlpha.ResetContent();
		m_comboLinkBeta.ResetContent();
		m_comboLinkA.ResetContent();
		m_comboLinkB.ResetContent();
		//m_comboLinkT.ResetContent();

		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("선택"));
		}
		else {
			nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("Select"));
		}
		m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("--- Select ---"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("선택"));
		}
		else {
			nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("Select"));
		}
		m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkA.InsertString(-1, _T("--- 0.0 ---"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("0.0"));
		}
		else {
			nCursorIndex = m_comboLinkA.InsertString(-1, _T("0.0"));
		}
		m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkB.InsertString(-1, _T("--- 0.0 ---"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("0.0"));
		}
		else {
			nCursorIndex = m_comboLinkB.InsertString(-1, _T("0.0"));
		}
		m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		//nCursorIndex = m_comboLinkT.InsertString(-1, _T("--- Select ---"));
		//m_comboLinkT.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		KIOTables* pTables = m_pTarget->Tables();
		KIOTable* pTable = pTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns = pTable->Columns();

		int nColumnCount = pColumns->ColumnCount();

		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				nCursorIndex = m_comboLinkAlpha.InsertString(-1, pColumn->Caption());
				m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				nCursorIndex = m_comboLinkBeta.InsertString(-1, pColumn->Caption());
				m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				nCursorIndex = m_comboLinkA.InsertString(-1, pColumn->Caption());
				m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				nCursorIndex = m_comboLinkB.InsertString(-1, pColumn->Caption());
				m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				/*nCursorIndex = m_comboLinkT.InsertString(-1, pColumn->Caption());
				m_comboLinkT.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);*/

				CString strColumnName = pColumn->Name();
				if (strColumnName.CompareNoCase(COLUMN_LINK_ALPHA) == 0)
				{
					nAlphaIndex = nCursorIndex;
					m_comboLinkAlpha.SetCurSel(nAlphaIndex);
				}
				else if (strColumnName.CompareNoCase(COLUMN_LINK_BETA) == 0)
				{
					nBetaIndex = nCursorIndex;
					m_comboLinkBeta.SetCurSel(nBetaIndex);
				}
				else if (strColumnName.CompareNoCase(COLUMN_LINK_A) == 0)
				{
					nAIndex = nCursorIndex;
					m_comboLinkA.SetCurSel(nAIndex);
				}
				else if (strColumnName.CompareNoCase(COLUMN_LINK_B) == 0)
				{
					nBIndex = nCursorIndex;
					m_comboLinkB.SetCurSel(nBIndex);
				}
			}
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	if (m_nComboLinkAllACursor == 0)
		m_comboLinkAlpha.SetCurSel(nAlphaIndex);
	else
		m_comboLinkAlpha.SetCurSel(m_nComboLinkAllAlphaCursor);

	if (m_nComboLinkAllBetaCursor == 0)
		m_comboLinkBeta.SetCurSel(nBetaIndex);
	else
		m_comboLinkBeta.SetCurSel(m_nComboLinkAllBetaCursor);

	if (m_nComboLinkAllACursor == 0)
		m_comboLinkA.SetCurSel(nAIndex);
	else
		m_comboLinkA.SetCurSel(m_nComboLinkAllACursor);

	if (m_nComboLinkAllBCursor == 0)
		m_comboLinkB.SetCurSel(nBIndex);
	else
		m_comboLinkB.SetCurSel(m_nComboLinkAllBCursor);
	//m_comboLinkT.SetCurSel(m_nComboLinkAllTCursor);
}


void KAOBMultiVDFDlg::OnBnClickedButtonSetbprfn()
{
	//m_vecFunctionRows.clear();
	KABOMultiFnManagerDlg oDlg(m_pTarget);
	oDlg.SetColumnInfo(m_vecInitFunctionColumn);
	oDlg.SetFnData(m_vecFunctionRows);
	if(oDlg.DoModal() == IDOK)
	{
		oDlg.GetImportData(m_vecFunctionRows);
	}

	UpdateFunctionTypeReport();

}


bool KAOBMultiVDFDlg::GetMultiVDFInfo( TAOBMultiVDF& TMultiVDF )
{
	
	return true;

}


bool KAOBMultiVDFDlg::GetMatchingCtrl(std::map<int, int>& a_mapVDFMatching)
{
	std::map<int, int> mapVDFMating;

	CXTPReportRecords*	  pRecords			= NULL;
	CXTPReportRecord*     pRecord			= NULL;
	CXTPReportRecordItem* pItem				= NULL;
	CXTPReportColumn*	  pColumn			= NULL;

	pRecords = m_wndMatch.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; i++)
	{
		//Mode 이름 가져오기
		pRecord   = pRecords->GetAt(i);
		pItem     = pRecord->GetItem(0);
		pColumn   = m_wndMatch.GetColumns()->GetAt(0);

		CString strModeName	= pItem->GetCaption(pColumn);
		int nModeID = 0;

		//Mode 이름으로 Mode ID 매칭
		std::list<KAInputInfo*>::iterator itInput, itEnd = m_InputList.end();
		for(itInput = m_InputList.begin(); itInput != itEnd; ++itInput)
		{
			KAInputInfo* pInput  = *itInput;

			if (!pInput->Selected())
			{
				continue;
			}

			KMode* pMode = pInput->Mode();
			CString strModeCaption = pMode->ModeName();
			if (strModeName == strModeCaption)
			{
				nModeID = pMode->ModeID();
			}
		}

		//Mode에 따른 VDF 값 
		int nItemCount = 1; 
		for (int nIndex = 0; nIndex < m_nModeNumInVDF; nIndex++)
		{
			pItem		= pRecord->GetItem(nItemCount);
			pColumn		= m_wndMatch.GetColumns()->GetAt(nItemCount);
			if (TRUE == pItem->IsChecked())
			{
				mapVDFMating.insert(std::make_pair(nModeID, nItemCount));
			}
			nItemCount++;
		}

	}

	//std::list<int> lstSelectValue; 
	// nombara :  기존 리스트 -> std::set으로 변경

	std::set<int> setSelectValue;
	std::map<int, int>::iterator iter = mapVDFMating.begin();
	std::map<int, int>::iterator end  = mapVDFMating.end();
	while(iter != end)
	{
		int nSelectValue = iter->second;
		//lstSelectValue.push_back(nSelectValue);
		setSelectValue.insert(nSelectValue);
		++iter;
	}	
	//lstSelectValue.unique();

	int nSelectCount = setSelectValue.size();
	if (nSelectCount != m_nModeNumInVDF)
	{		
		return false;
	}
	
	a_mapVDFMatching = mapVDFMating;
	return true;
	
}


bool KAOBMultiVDFDlg::GetValueofTimeInfo( int& a_nStandardVDF, std::map<int, double>& a_mapVDFValue )
{
	int nStandardVDF = 0;
	std::map<int, double> mapVDFMValue;

	CXTPReportRecords*				pRecords		= NULL;
	CXTPReportRecord*				pRecord			= NULL;
	CXTPReportRecordItem*			pItem			= NULL;
	CXTPReportColumn*				pColumn			= NULL;
	CXTPReportRecordItemNumber*		pItemNumber		= NULL;

	pRecords = m_wndVOT.GetRecords();
	int nRecordCount = pRecords->GetCount();

	pRecord = pRecords->GetAt(0);
		//기준이되는 T0의 값 가져오기
	int nItemCount = 1; 
	for (int nIndex = 0; nIndex < m_nModeNumInVDF; nIndex++)
	{
		pItem		= pRecord->GetItem(nItemCount);
		pColumn		= m_wndMatch.GetColumns()->GetAt(nItemCount);
		//CString strModeCheck = pItem->GetCaption(pColumn);
		if (TRUE == pItem->IsChecked())
		{
			nStandardVDF = nItemCount; 
		}
		nItemCount++;
	}

	//두번째 값 가져오기
	pRecord = pRecords->GetAt(1);
	nItemCount = 1; 
	for (int nIndex = 0; nIndex < m_nModeNumInVDF; nIndex++)
	{
		pItemNumber			= (CXTPReportRecordItemNumber*)pRecord->GetItem(nItemCount);
		double dValue	=  pItemNumber->GetValue();
		mapVDFMValue.insert(std::make_pair(nItemCount, dValue));
		nItemCount++;
	}

	if (nStandardVDF != 0 && mapVDFMValue.size() == m_nModeNumInVDF)
	{
		a_mapVDFValue	= mapVDFMValue;
		a_nStandardVDF	= nStandardVDF;
		return true;
	}
	else
	{
		return false;
	}


}


bool KAOBMultiVDFDlg::GetChangeParmeter( bool& a_bAlBeta, bool& a_bABToll, bool& a_bInitionTime )
{
	if (m_ChkAlphaBeta == TRUE)
	{
		a_bAlBeta = true;
	}
	else
	{
		a_bAlBeta = false;
	}

	if (m_ChkabToll == TRUE)
	{
		a_bABToll = true;
	}
	else
	{
		a_bABToll = false;
	}

	if (m_ChkInitialTime == TRUE)
	{
		a_bInitionTime = true;
	}
	else
	{
		a_bInitionTime = false;
	}

	if (m_ChkAlphaBeta == FALSE && m_ChkabToll == FALSE && m_ChkInitialTime == FALSE)
	{
		return false;
	}

	return true;
}


bool KAOBMultiVDFDlg::GetFunctionType( KEMAssignmentBPRFunctionType& a_emBPRFunction )
{
	switch(m_RadioFunction)
	{
	case 0:
		a_emBPRFunction = KEMAssignmentBPRFunctionEachFn;
		return true;

	case 1:
		a_emBPRFunction = KEMAssignmentBPRFunctionEachLink;
		return true;

	default:
		return false;
	}
}


bool KAOBMultiVDFDlg::GetFunctionTypeValue( std::vector<std::vector<CString>>& a_vecFunctionType )
{

	std::vector<std::vector<CString>> vecOuterResult;
	
	if (m_ChkAlphaBeta == TRUE && m_ChkabToll == TRUE && m_ChkInitialTime == TRUE)
	{
		vecOuterResult = m_vecFunctionRows;
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == TRUE && m_ChkInitialTime == FALSE)
	{
		vecOuterResult = m_vecFunctionRows;
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == FALSE && m_ChkInitialTime == TRUE)
	{
		int nOutCount = m_vecFunctionRows.size();
		for (int nOutIndex = 0 ; nOutIndex < nOutCount ; nOutIndex++)
		{
			std::vector<CString>	vecInnerResult;
			std::vector<CString>	vecInRow = m_vecFunctionRows[nOutIndex];		
			vecInnerResult.push_back(vecInRow[0]);
			int nInIndex = 1;
			int nATollindex = m_nModeNumInVDF * 2;
			int nBTollindex = (m_nModeNumInVDF * 2) + 1;
			for (int i = 0; i < m_nModeNumInVDF ; i++)
			{
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
				vecInnerResult.push_back(vecInRow[nATollindex]);
				vecInnerResult.push_back(vecInRow[nBTollindex]);
			}

			nInIndex = nBTollindex + 1;
			for (int i= 0; i < m_nModeNumInVDF; i ++ )
			{
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
			}
			vecOuterResult.push_back(vecInnerResult);
		}
	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == TRUE && m_ChkInitialTime == TRUE)
	{
		int nOutCount = m_vecFunctionRows.size();
		for (int nOutIndex = 0 ; nOutIndex < nOutCount ; nOutIndex++)
		{
			std::vector<CString>	vecInnerResult;
			std::vector<CString>	vecInRow = m_vecFunctionRows[nOutIndex];		
			vecInnerResult.push_back(vecInRow[0]);
			int nAlphaTollindex = 1;
			int nBetaTollindex = 2;
			int nInIndex = 3;
			for (int i = 0; i < m_nModeNumInVDF ; i++)
			{
				vecInnerResult.push_back(vecInRow[nAlphaTollindex]);
				vecInnerResult.push_back(vecInRow[nBetaTollindex]);
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
			}

			for (int i= 0; i < m_nModeNumInVDF; i ++ )
			{
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
			}

			vecOuterResult.push_back(vecInnerResult);
		}
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == FALSE && m_ChkInitialTime == FALSE)
	{
		int nOutCount = m_vecFunctionRows.size();
		for (int nOutIndex = 0 ; nOutIndex < nOutCount ; nOutIndex++)
		{
			std::vector<CString>	vecInnerResult;
			std::vector<CString>	vecInRow = m_vecFunctionRows[nOutIndex];		
			vecInnerResult.push_back(vecInRow[0]);
			int nInIndex = 1;
			int nATollindex = (m_nModeNumInVDF * 2) + 1;
			int nBTollindex = (m_nModeNumInVDF * 2) + 2;
			for (int i = 0; i < m_nModeNumInVDF ; i++)
			{
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
				vecInnerResult.push_back(vecInRow[nATollindex]);
				vecInnerResult.push_back(vecInRow[nBTollindex]);
			}
			vecOuterResult.push_back(vecInnerResult);
		}
	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == TRUE && m_ChkInitialTime == FALSE)
	{
		int nOutCount = m_vecFunctionRows.size();
		for (int nOutIndex = 0 ; nOutIndex < nOutCount ; nOutIndex++)
		{
			std::vector<CString>	vecInnerResult;
			std::vector<CString>	vecInRow = m_vecFunctionRows[nOutIndex];		
			vecInnerResult.push_back(vecInRow[0]);
			int nAlphaTollindex = 1;
			int nBetaTollindex = 2;
			int nInIndex = 3;
			for (int i = 0; i < m_nModeNumInVDF ; i++)
			{
				vecInnerResult.push_back(vecInRow[nAlphaTollindex]);
				vecInnerResult.push_back(vecInRow[nBetaTollindex]);
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
			}

			vecOuterResult.push_back(vecInnerResult);
		}
	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == FALSE && m_ChkInitialTime == TRUE)
	{
		int nOutCount = m_vecFunctionRows.size();
		for (int nOutIndex = 0 ; nOutIndex < nOutCount ; nOutIndex++)
		{
			std::vector<CString>	vecInnerResult;
			std::vector<CString>	vecInRow = m_vecFunctionRows[nOutIndex];
			vecInnerResult.push_back(vecInRow[0]);
			int nAlphaTollindex  = 1;
			int nBetaTollindex   = 2;
			int nATollindex		 = 3;
			int nBTollindex		 = 4;
			int nInIndex = 5;
			for(int i = 0; i <m_nModeNumInVDF; i++)
			{
				vecInnerResult.push_back(vecInRow[nAlphaTollindex]);
				vecInnerResult.push_back(vecInRow[nBetaTollindex]);
				vecInnerResult.push_back(vecInRow[nATollindex]);
				vecInnerResult.push_back(vecInRow[nBTollindex]);
			}

			for (int i= 0; i < m_nModeNumInVDF; i ++ )
			{
				vecInnerResult.push_back(vecInRow[nInIndex]);
				nInIndex++;
			}

			vecOuterResult.push_back(vecInnerResult);

		}
	}
	else
	{
		vecOuterResult = m_vecFunctionRows;
	}

	if (vecOuterResult.size() == 0)
	{
		return false;
	}

	a_vecFunctionType = vecOuterResult;

	return true;
}


bool KAOBMultiVDFDlg::GetEachLInkValue( std::vector<CString>& a_vecLinkColumn )
{
	std::vector<CString> vecLinkColumn;
	std::vector<CString> vecLinkReportColumn;

	if (ReadEachLinkReport(vecLinkReportColumn) == false)
	{
		return false;
	}
	
	CString strAlphaColumnName = _T("");
	CString strBetaColumnName  = _T("");

	if (m_ChkAlphaBeta == FALSE)
	{
		CString strAlphaCaption = _T("");
		m_comboLinkAlpha.GetLBText(m_comboLinkAlpha.GetCurSel(),strAlphaCaption);
		strAlphaColumnName = GetColumnName(strAlphaCaption);
		if (strAlphaColumnName == _T(""))
		{
			return false;
		}

		CString strBetaCaption = _T("");
		m_comboLinkBeta.GetLBText(m_comboLinkBeta.GetCurSel(),strBetaCaption);
		strBetaColumnName = GetColumnName(strBetaCaption);
		if (strBetaColumnName == _T("") )
		{
			return false;
		}
	}

	CString strATollCaption = _T("");
	m_comboLinkA.GetLBText(m_comboLinkA.GetCurSel(),strATollCaption);
	CString strATollColumnName = GetColumnName(strATollCaption);

	CString strBTollCaption = _T("");
	m_comboLinkB.GetLBText(m_comboLinkB.GetCurSel(),strBTollCaption);
	CString strBTollColumnName = GetColumnName(strBTollCaption);


	CString strColumnName = _T("");
	if (m_ChkAlphaBeta == TRUE && m_ChkabToll == TRUE && m_ChkInitialTime == TRUE)
	{
		vecLinkColumn = vecLinkReportColumn;
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == TRUE && m_ChkInitialTime == FALSE)
	{
		vecLinkColumn = vecLinkReportColumn;
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == FALSE && m_ChkInitialTime == TRUE)
	{
		int nIndex = 0;
		for (int i = 0; i < m_nModeNumInVDF; i++)
		{
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
			vecLinkColumn.push_back(strATollColumnName);
			vecLinkColumn.push_back(strBTollColumnName);
		}

		for (int i = 0; i< m_nModeNumInVDF; i++)
		{
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex;
		}

	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == TRUE && m_ChkInitialTime == TRUE)
	{
		int nIndex = 0;
		for (int i = 0; i < m_nModeNumInVDF; i++)
		{
			vecLinkColumn.push_back(strAlphaColumnName);
			vecLinkColumn.push_back(strBetaColumnName);
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
		}

		for (int i = 0; i< m_nModeNumInVDF; i++)
		{
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex;
		}
	}
	else if (m_ChkAlphaBeta == TRUE && m_ChkabToll == FALSE && m_ChkInitialTime == FALSE)
	{
		int nIndex = 0;
		for (int i = 0; i < m_nModeNumInVDF; i++)
		{
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
			vecLinkColumn.push_back(strATollColumnName);
			vecLinkColumn.push_back(strBTollColumnName);
		}
	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == TRUE && m_ChkInitialTime == FALSE)
	{
		int nIndex = 0;
		for (int i = 0; i < m_nModeNumInVDF; i++)
		{
			vecLinkColumn.push_back(strAlphaColumnName);
			vecLinkColumn.push_back(strBetaColumnName);
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
		}
	}
	else if (m_ChkAlphaBeta == FALSE && m_ChkabToll == FALSE && m_ChkInitialTime == TRUE)
	{
		for (int i = 0; i< m_nModeNumInVDF; i++)
		{
			vecLinkColumn.push_back(strAlphaColumnName);
			vecLinkColumn.push_back(strBetaColumnName);
			vecLinkColumn.push_back(strATollColumnName);
			vecLinkColumn.push_back(strBTollColumnName);
		}
		
		int nIndex = 0;
		for (int i = 0; i< m_nModeNumInVDF; i++)
		{
			strColumnName = vecLinkReportColumn[nIndex];
			vecLinkColumn.push_back(strColumnName);
			nIndex++;
		}
	}
	else
	{
		vecLinkColumn.push_back(strAlphaColumnName);
		vecLinkColumn.push_back(strBetaColumnName);
		vecLinkColumn.push_back(strATollColumnName);
		vecLinkColumn.push_back(strBTollColumnName);
	}

	a_vecLinkColumn = vecLinkColumn;
	return true;
}


bool KAOBMultiVDFDlg::ReadEachLinkReport(std::vector<CString>& a_vecLinkReportName)
{

	m_wndEachLink.Populate();

	std::vector<CString> vecLinkReport;

	CXTPReportRecords*	  pRecords			= NULL;
	CXTPReportRecord*     pRecord			= NULL;
	//CXTPReportRecordItem* pItem				= NULL;

	pRecords = m_wndEachLink.GetRecords();
	int nRecordCount = pRecords->GetCount();

	int nCount = m_vecInitEachLinkColumn.size();

	int nIndex = 1;
	for (int i =0; i < nCount ; i++)
	{
		CString strColumnName = _T("");
		CString strIndexName = m_vecInitEachLinkColumn[i];
		
		pRecord = pRecords->GetAt(0);
		CXTPReportColumn*		pColumn	= m_wndEachLink.GetColumns()->GetAt(i);
		CXTPReportRecordItem*	pItem	= pRecord->GetItem(nIndex);
		CString strColumnCaption		= pItem->GetCaption(pColumn);
		strColumnName = GetColumnName(strColumnCaption);

		if (strIndexName.Right(1) == _T("α") || strIndexName.Right(1) == _T("β"))
		{
			if (strColumnName == _T(""))
			{
				return false;
			}
		}
	
		vecLinkReport.push_back(strColumnName);
		nIndex++;
	}

	a_vecLinkReportName = vecLinkReport;
	return true;
}


CString KAOBMultiVDFDlg::GetColumnName( CString a_strColumCaption )
{
	CString strColumnName = _T("");

	KIOTables* pTables = m_pTarget->Tables();
	KIOTable* pTable = pTables->FindTable(TABLE_LINK);

	const KIOColumns* pColumns = pTable->Columns();

	int nColumnCount = pColumns->ColumnCount();

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() == KEMIODataTypeDouble)
		{
			if (pColumn->Caption() == a_strColumCaption)
			{
				strColumnName = pColumn->Name();

				return strColumnName;
			}
		}
	}
	return strColumnName;
}


bool KAOBMultiVDFDlg::GetAOBMultiVDFInfo( TAOBMultiVDF& a_oAOBMultiVDF )
{
	bool bCheckData = false;
	TAOBMultiVDF oAOBmultiVDFInfo;
	if (GetMatchingCtrl(oAOBmultiVDFInfo.TmapVDFModeMating) == false)
	{
		AfxMessageBox(_T("수단별 통행지체함수(VDF) 적용 설정에 선택되지 않은 항목이 존재합니다."));
		return false;
	}
	
	if (GetValueofTimeInfo(oAOBmultiVDFInfo.TnStandardVDF, oAOBmultiVDFInfo.TmapVDFValue) == false)
	{
		AfxMessageBox(_T("최초 배정 지체함수가 설정되지 않았습니다."));
		return false;
	}
	
	if (GetChangeParmeter(oAOBmultiVDFInfo.TbPamAlBeta, oAOBmultiVDFInfo.TbPamabToll, oAOBmultiVDFInfo.TbpamIntialTime) == false)
	{
		AfxMessageBox(_T("통행지체함수 관련 계수 개별 항목을 설정해 주세요."));
		return false;
	}

	CString strAlphaCaption = _T("");
	CString strBetaCaption = _T("");
	CString strATollCaption = _T("");
	CString strBTollCaption = _T("");

	CString strAlphaColumnName(_T(""));
	CString strBetaColumnName(_T(""));
	CString strATollColumnName(_T(""));
	CString strBTollColumnName(_T(""));

	GetFunctionType(oAOBmultiVDFInfo.TemEachFunctionType);
	switch(oAOBmultiVDFInfo.TemEachFunctionType)
	{
	case KEMAssignmentBPRFunctionEachFn:
		if (GetFunctionTypeValue(oAOBmultiVDFInfo.TvecFucntionType) == false)
		{
			AfxMessageBox(_T("함수 형태별 VDF에 설정한 함수가 존재하지 않습니다."));
			return false;
		}
		
		a_oAOBMultiVDF = oAOBmultiVDFInfo;
		return true;

	case KEMAssignmentBPRFunctionEachLink:
		if (GetEachLInkValue(oAOBmultiVDFInfo.TvecEachLinkColumn) == false)
		{
			AfxMessageBox(_T("개별 링크별 VDF가 설정되지 않았습니다.."));
			return false;
		}

		m_comboLinkAlpha.GetLBText(m_comboLinkAlpha.GetCurSel(),strAlphaCaption);
		strAlphaColumnName = GetColumnCaption(strAlphaCaption);
		m_comboLinkBeta.GetLBText(m_comboLinkBeta.GetCurSel(),strBetaCaption);
		strBetaColumnName = GetColumnCaption(strBetaCaption);
		m_comboLinkA.GetLBText(m_comboLinkA.GetCurSel(),strATollCaption);
		strATollColumnName = GetColumnCaption(strATollCaption);
		m_comboLinkB.GetLBText(m_comboLinkB.GetCurSel(),strBTollCaption);
		strBTollColumnName = GetColumnCaption(strBTollCaption);

		oAOBmultiVDFInfo.TstrAlpha = strAlphaCaption;
		oAOBmultiVDFInfo.TstrBeta	 = strBetaCaption;
		oAOBmultiVDFInfo.TA		 = strATollCaption;
		oAOBmultiVDFInfo.TB		 = strBTollCaption;

		a_oAOBMultiVDF = oAOBmultiVDFInfo;
		return true;

	default:
		return false;
	}

	return true;
}


void KAOBMultiVDFDlg::SetComboBoxInfo( TAOBMultiVDF& a_TAOBMultiVDF)
{
	if (a_TAOBMultiVDF.TstrAlpha == _T(""))
	{
		return;
	}

	int nIndex = m_comboLinkAlpha.GetCount();
	for (int i = 0; i < nIndex ; i++)
	{
		CString strComboName = _T("");
		m_comboLinkAlpha.GetLBText(i,strComboName);

		CString strColumnCaption = a_TAOBMultiVDF.TstrAlpha;
		if (strColumnCaption == strComboName)
		{
			m_comboLinkAlpha.SetCurSel(i);
		}
	}

	if (a_TAOBMultiVDF.TstrBeta == _T(""))
	{
		return;
	}

	nIndex = m_comboLinkBeta.GetCount();
	for (int i = 0; i < nIndex ; i++)
	{
		CString strComboName = _T("");
		m_comboLinkBeta.GetLBText(i,strComboName);

		CString strColumnCaption = a_TAOBMultiVDF.TstrBeta;
		if (strColumnCaption == strComboName)
		{
			m_comboLinkBeta.SetCurSel(i);
		}
	}

	if (a_TAOBMultiVDF.TA != _T(""))
	{
		nIndex = m_comboLinkA.GetCount();
		for (int i = 0; i < nIndex ; i++)
		{
			CString strComboName = _T("");
			m_comboLinkA.GetLBText(i,strComboName);

			CString strColumnCaption = a_TAOBMultiVDF.TA;
			if (strColumnCaption == strComboName)
			{
				m_comboLinkA.SetCurSel(i);
			}
		}
	}

	if (a_TAOBMultiVDF.TB != _T(""))
	{
		nIndex = m_comboLinkB.GetCount();
		for (int i = 0; i < nIndex ; i++)
		{
			CString strComboName = _T("");
			m_comboLinkB.GetLBText(i,strComboName);

			CString strColumnCaption = a_TAOBMultiVDF.TB;
			if (strColumnCaption == strComboName)
			{
				m_comboLinkB.SetCurSel(i);
			}
		}
	}
	UpdateData(FALSE);
}


CString KAOBMultiVDFDlg::GetColumnCaption( CString a_strColumnName )
{
	CString strColumnCaption = _T("");

	KIOTables* pTables = m_pTarget->Tables();
	KIOTable* pTable = pTables->FindTable(TABLE_LINK);

	const KIOColumns* pColumns = pTable->Columns();

	int nColumnCount = pColumns->ColumnCount();

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(pColumn->DataType() == KEMIODataTypeDouble)
		{
			if (pColumn->Name() == a_strColumnName)
			{
				strColumnCaption = pColumn->Caption();

				return strColumnCaption;
			}
		}
	}
	return strColumnCaption;
}
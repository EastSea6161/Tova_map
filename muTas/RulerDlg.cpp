// RulerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RulerDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"

#include "DBaseRuler.h"

// KRulerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KRulerDlg, KResizeDialogEx)

KRulerDlg::KRulerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KRulerDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_bIsSingleSelect(true)
{

}

KRulerDlg::~KRulerDlg()
{
	if (m_bIsSingleSelect)
	{
		m_pMapView->ClearRulerSelection();
	}
	else
	{
		m_pMapView->ClearLinkViaPathThemes();
	}

	m_pMapView->ClearRulerSelection();
	m_pMapView->ResetScreenDrawEdit();
}

void KRulerDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboColumn);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KRulerDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL,          &KRulerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_MAP,    &KRulerDlg::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &KRulerDlg::OnBnClickedButtonDelete)
	ON_CBN_SELCHANGE(IDC_COMBO1,     &KRulerDlg::OnCbnSelchangeCombo1)
		
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KRulerDlg::OnBnClickedRadio)
END_MESSAGE_MAP()


// KRulerDlg 메시지 처리기입니다.

void KRulerDlg::MapSelectionFeedback( Integer a_nxID )
{
	try
	{
		AddReportData(a_nxID);
		DrawLinkThemes();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KRulerDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
	m_vecLinkID.clear();

	int nSize = a_vecSelection.size();
	
	for (int i = 0; i < nSize; ++i)
	{
		m_vecLinkID.push_back(a_vecSelection[i]);
	}

	InitReportData();
}

void KRulerDlg::InitResizeCtrl( void )
{
	SetResize(IDC_REPORT,            SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_MID,        CXTPResizePoint(0, 0),  CXTPResizePoint(1, 0));
	SetResize(IDC_STATIC_BOTTOM,     CXTPResizePoint(0, 1),  CXTPResizePoint(1, 1));
    SetResize(IDC_STATIC_BOTTOM2,    CXTPResizePoint(0, 1),  CXTPResizePoint(1, 1));
	SetResize(IDCANCEL,              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_DELETE,     SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_INFO,       CXTPResizePoint(0, 1),  CXTPResizePoint(0, 1));
	SetResize(IDC_STATIC_SELECT,     SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SUM1,       SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_AVG1,       SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MIN,        SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MAX,        SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_STD,        SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_SUM_RESULT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_AVG_RESULT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MIN_RESULT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MAX_RESULT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_STD_RESULT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_FIELD,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO1,            SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BUTTON_MAP,        CXTPResizePoint(1, 0),  CXTPResizePoint(1, 0));
    
    //SetResize(IDC_STATIC_BOUNDARY,   SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);   
    //SetResize(IDC_STATIC_BOUNDARY2,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);   
}

void KRulerDlg::InitCombo( void )
{
	try
	{
		m_cboColumn.ResetContent();

		KIOTable*   pTable   = m_pTarget->Tables()->FindTable(TABLE_LINK);
		KIOColumns* pColumns = pTable->Columns();
		KIOColumn*  pColumn  = nullptr;

		int nColumn = pColumns->ColumnCount();
		int nIndex;

		for (int i = 0; i < nColumn; ++i)
		{
			pColumn = pColumns->GetColumn(i);

			if (pColumn->DataType() != KEMIODataTypeDouble)
				continue;

			nIndex = m_cboColumn.InsertString(-1, pColumn->Caption());
			m_cboColumn.SetItemData(nIndex, (DWORD_PTR)pColumn);
		}

		m_cboColumn.SetCurSel(0);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}			
}

BOOL KRulerDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton->SetCheck(BST_CHECKED);

		InitResizeCtrl();
		InitCombo();

		KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AllowColumnsort(m_wndReport);

		InitReportHeader();
		InitReportData();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
		KResizeDialogEx::OnCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		KResizeDialogEx::OnCancel();
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KRulerDlg::OnBnClickedCancel()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);

	if (m_pMapView == nullptr)
		return;

	if (pButton->GetCheck() == BST_CHECKED)
	{
		m_pMapView->ClearRulerSelection();
	}
	else
	{
		m_pMapView->ClearLinkViaPathThemes();
	}

	m_pMapView->ResetScreenDrawEdit();

	KResizeDialogEx::OnCancel();
}

void KRulerDlg::OnBnClickedButtonMap()
{
	try
	{
		if (m_bIsSingleSelect)
		{
			if (m_pMapView != nullptr)
				m_pMapView->RulerSelectLink(this);
		}
		else
		{
			if (m_pMapView != nullptr)
				m_pMapView->SelectLinkPath(this);
		}	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KRulerDlg::OnBnClickedButtonDelete()
{
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	int nRecord = m_wndReport.GetRecords()->GetCount();

	if (nRecord == 0)
		return;

	m_vecLinkID.clear();

	for (int i = 0; i <nRecord; ++i)
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);
		pItem   = pRecord->GetItem(0);

		if (!pItem->IsChecked())
		{
			m_vecLinkID.push_back((Integer)pItem->GetItemData());
		}
	}

	InitReportData();

	if (m_bIsSingleSelect)
	{
		DrawLinkThemes();
	}
	else
	{
		if (m_pMapView != nullptr)
		{
			m_pMapView->ResetScreenDrawEdit();
			m_pMapView->MapRefresh();

			m_pMapView->SelectLinkPath(this);
		}
	}
}

void KRulerDlg::InitReportHeader( void )
{
	m_wndReport.GetColumns()->Clear();
	
	CXTPReportColumn* pColumn = nullptr;
	int               nIndex  = 0;

	//Selct
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	//Sequence
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Seq"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(TRUE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("순서"));
	}

	//ID
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("ID"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(TRUE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("링크 ID"));
	}

	//Value
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Value"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(TRUE);
	pColumn->SetAlignment(DT_RIGHT);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("측정 기준 값"));
	}
}

void KRulerDlg::InitReportData( void )
{
	m_wndReport.ResetContent();
	GetRulerData();
			
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	TRulerInfo            oRulerInfo;
	
	int nSize = m_vecRulerInfo.size();
	
	for (int i = 0; i < nSize; ++i)
	{
		oRulerInfo = m_vecRulerInfo[i];

		//Select
		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->HasCheckbox(TRUE);
		pItem->SetItemData((DWORD_PTR)oRulerInfo.nxLinkID);

		if (!m_bIsSingleSelect)
		{
			pItem->SetChecked(TRUE);
			pItem->SetEditable(FALSE);
		}

		//Seq
		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)(i + 1), _T("%.0f")));

		//ID
		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oRulerInfo.nxLinkID, _T("%.0f")));

		//Value
		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oRulerInfo.dValue, _T("%.3f")));
	}

	m_wndReport.Populate();
	UpdateInformation();
}

void KRulerDlg::GetRulerData()
{
	m_vecRulerInfo.clear();

	int nSize = m_vecLinkID.size();

	for (int i = 0; i < nSize; ++i)
	{
		TRulerInfo oRulerInfo = {};
		oRulerInfo.nxLinkID   = m_vecLinkID[i];

		m_vecRulerInfo.push_back(oRulerInfo);
	}

	KIOColumn* pColumn = (KIOColumn*)m_cboColumn.GetItemData(m_cboColumn.GetCurSel());
	KDBaseRuler::GetRulerInfo(m_pTarget, pColumn, m_vecRulerInfo);
}

void KRulerDlg::AddReportData( Integer a_nxLinkID )
{
	m_vecLinkID.push_back(a_nxLinkID);

	TRulerInfo oRulerInfo = {};
	oRulerInfo.nxLinkID = a_nxLinkID;

	KIOColumn* pColumn = (KIOColumn*)m_cboColumn.GetItemData(m_cboColumn.GetCurSel());
	KDBaseRuler::GetRulerInfo(m_pTarget, pColumn, oRulerInfo);
	
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	int                   nRecord = m_wndReport.GetRecords()->GetCount();

	pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

	//Select
	pItem = pRecord->AddItem(new CXTPReportRecordItem);
	pItem->HasCheckbox(TRUE);
	pItem->SetItemData((DWORD_PTR)oRulerInfo.nxLinkID);
	pItem->SetAlignment(xtpColumnIconCenter);

	//Seq
	pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)(nRecord + 1), _T("%.0f")));
	//ID
	pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oRulerInfo.nxLinkID, _T("%.0f")));
	//Value
	pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oRulerInfo.dValue, _T("%.3f")));

	m_wndReport.Populate();
	UpdateInformation();
}

void KRulerDlg::DrawLinkThemes()
{
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	Integer               nxID;

	int nRecord = m_wndReport.GetRecords()->GetCount();
	
	COLORREF clrDefult     = RGB(255, 0, 0);
	double   dDefaultWidth = 10.0;
	CString  strValue;

	std::vector<TDrawLinkInfo> vecDrawLinkInfo;
	
	for (int i = 0; i < nRecord; ++i)
	{
		pRecord  = m_wndReport.GetRecords()->GetAt(i);
		nxID     = (Integer)pRecord->GetItem(0)->GetItemData();
		strValue.Format(_T("%I64d"), nxID);

		TDrawLinkInfo oDrawLinkInfo;
		oDrawLinkInfo.TLinkID   = nxID;
		oDrawLinkInfo.TLabel    = strValue;
		oDrawLinkInfo.TColorRGB = clrDefult;
		oDrawLinkInfo.TWidth    = dDefaultWidth;

		vecDrawLinkInfo.push_back(oDrawLinkInfo);
	}

	m_pMapView->RulerSelectionLink(vecDrawLinkInfo, false, true);
}


void KRulerDlg::OnCbnSelchangeCombo1()
{
	InitReportData();
}

void KRulerDlg::UpdateInformation( void )
{
	int     nRecord = m_wndReport.GetRecords()->GetCount();
	CString strValue;

	if (nRecord == 0)
	{
		strValue = _T("0.0");
		SetDlgItemText(IDC_STATIC_AVG_RESULT, strValue);
		SetDlgItemText(IDC_STATIC_SUM_RESULT, strValue);
		SetDlgItemText(IDC_STATIC_MIN_RESULT, strValue);
		SetDlgItemText(IDC_STATIC_MAX_RESULT, strValue);
		SetDlgItemText(IDC_STATIC_STD_RESULT, strValue);
		return;
	}

	double dSum(0.0), dAvg(0.0), dMin(0.0), dMax(0.0);

	CXTPReportRecord*           pRecord = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);

		if (i == 0)
		{
			dMin = pNumber->GetValue();
			dMax = pNumber->GetValue();
		}

		dMin = dMin > pNumber->GetValue() ? pNumber->GetValue() : dMin;
		dMax = dMax < pNumber->GetValue() ? pNumber->GetValue() : dMax;

		dSum += pNumber->GetValue();
	}

	//Update Sum
	strValue.Format(_T("%.3f"), dSum);
	SetDlgItemText(IDC_STATIC_SUM_RESULT, strValue);

	//Update Avg
	dAvg = dSum / nRecord;
	strValue.Format(_T("%.3f"), dAvg);
	SetDlgItemText(IDC_STATIC_AVG_RESULT, strValue);

	//Update Min
	strValue.Format(_T("%.3f"), dMin);
	SetDlgItemText(IDC_STATIC_MIN_RESULT, strValue);

	//Update Max
	strValue.Format(_T("%.3f"), dMax);
	SetDlgItemText(IDC_STATIC_MAX_RESULT, strValue);

	//Calculate Standard devaition(Std.)
	double dDeviation(0.0);

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);

		dDeviation += pow((pNumber->GetValue() - dAvg), 2);
	}

	//분산 = 편차 제곱의 합 / 원소개수
	double dVariance = dDeviation / nRecord;
	
	//표준편차 = 분산의 제곱근
	double dStd      = sqrt(dVariance);
	
	//Update standard devaition(Std.)
	strValue.Format(_T("%.3f"), dStd);
	SetDlgItemText(IDC_STATIC_STD_RESULT, strValue);
}

void KRulerDlg::OnBnClickedRadio( UINT nID )
{
	try
	{
        if (m_pMapView != nullptr)
            m_pMapView->ClearRulerSelection();

		m_vecLinkID.clear();
		InitReportData();

		switch (nID)
		{
		case IDC_RADIO1:
			m_bIsSingleSelect = true;
			break;
		case IDC_RADIO2:
			m_bIsSingleSelect = false;
			break;
		default:
			ThrowException(_T("정의되지 않은 타입입니다."));
			break;
		}
        		
		OnBnClickedButtonMap();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

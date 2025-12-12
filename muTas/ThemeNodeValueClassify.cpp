// ThemeNodeValueClassify.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeNodeValueClassify.h"
#include "afxdialogex.h"

#include "Target.h"
#include "MapView.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

#include "DBaseConnector.h"
#include "KPointMarkerSelectDlg.h"
#include "ThemeStyleSave.h"

#include "BulkDBaseNode.h"

#include "PointStyleValueEdit.h"

#include "ThemeFunctionDlg.h"
#include "ExpressionChecker.h"
#include "ParseExpression.h"
#include "KBulkDBase.h"
#include "TThemeType.h"
#include "DefineNames.h"

// KThemeNodeValueClassify 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeNodeValueClassify, KDialogEx)

KThemeNodeValueClassify::KThemeNodeValueClassify(CWnd* pParent /*=NULL*/)
	: KDialogEx(KThemeNodeValueClassify::IDD, pParent)
	, m_bCheckedFilter(FALSE)
	, m_nRealDigit(0)
	, m_dMinCalculateVal(0.0)
	, m_dMaxCalculateVal(0.0)
{
	m_oQbicDefaultMarkerFont.CreatePointFont(120, _T("QBicF"));   

	m_strPreSelectedColumnName = _T("");
	m_nPreSelectedStyleID      = -1;
	m_nPreSelectedClass        = -1;

	m_nSelectedMarkerIndex   = 48;
	m_clrSelectedMarkerColor = RGB(51, 102, 255);

	m_nOtherMarkerIndex      = 53;
	m_clrOtherMarkerColor    = RGB(234, 234, 234);

	m_strFunction = _T("");
	m_pExpressionChecker   = new KExpressionChecker;
	m_pCategoryFieldRecord = nullptr;
}

KThemeNodeValueClassify::~KThemeNodeValueClassify()
{
	int nCntFilter = m_vecFilterInfo.size();

	for (int i = 0; i < nCntFilter; ++i)
	{
		TFilterInfo* pFilterInfo = m_vecFilterInfo[i];
		delete pFilterInfo;
	}
	m_vecFilterInfo.clear();

	m_pNodeTable->Unregister(this);

	if (nullptr != m_pExpressionChecker)
	{
		QBicDelete(m_pExpressionChecker);
	}

	if (nullptr != m_pCategoryFieldRecord)
	{
		QBicDelete(m_pCategoryFieldRecord);
	}
}

void KThemeNodeValueClassify::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_cboCategory);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cboStyle);
	DDX_Control(pDX, IDC_COMBO_CLASS, m_cboClass);
	DDX_Control(pDX, IDC_COMBO_DIGIT, m_cboDigit);
	DDX_Control(pDX, IDC_BUTTON_MARKER, m_btnMarker);
	DDX_Control(pDX, IDC_BUTTON_MARKER_OTHER, m_btnMarkerOther);
}


BEGIN_MESSAGE_MAP(KThemeNodeValueClassify, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_MARKER, &KThemeNodeValueClassify::OnBnClickedButtonMarker)	
	ON_BN_CLICKED(IDC_RADIO1, &KThemeNodeValueClassify::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &KThemeNodeValueClassify::OnBnClickedRadio)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD, &KThemeNodeValueClassify::OnCbnSelchangeComboField)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &KThemeNodeValueClassify::OnCbnSelchangeComboStyle)	
	ON_CBN_SELCHANGE(IDC_COMBO_CLASS, &KThemeNodeValueClassify::OnCbnSelchangeComboClass)
	ON_BN_CLICKED(IDC_CHECK1, &KThemeNodeValueClassify::OnBnClickedCheck1)

	ON_NOTIFY(NM_CLICK, IDC_REPORT,   &KThemeNodeValueClassify::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KThemeNodeValueClassify::OnReportItemValueChanged)
	//XTP_NM_REPORT_SELCHANGED
	ON_CBN_SELCHANGE(IDC_COMBO_DIGIT, &KThemeNodeValueClassify::OnCbnSelchangeComboDigit)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_SAVE, &KThemeNodeValueClassify::OnBnClickedButtonStyleSave)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_EDIT, &KThemeNodeValueClassify::OnBnClickedButtonStyleEdit)

	ON_BN_CLICKED(IDC_BTN_FILTER1,   &KThemeNodeValueClassify::OnBnClickedBtnFilter)
	ON_BN_CLICKED(IDC_CHECK_FILTER, &KThemeNodeValueClassify::OnBnClickedFilter)

	ON_BN_CLICKED(IDC_BUTTON1, &KThemeNodeValueClassify::OnBnClickedButtonFormula)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO4, &KThemeNodeValueClassify::OnBnClickedRadioDrawData)
	ON_BN_CLICKED(IDC_CHECK_ALL, &KThemeNodeValueClassify::OnBnClickedCheckAll)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KThemeNodeValueClassify::OnReportCheckItem)

	ON_BN_CLICKED(IDC_CHECK3, &KThemeNodeValueClassify::OnBnClickedCheckAllOtherValues)
	ON_BN_CLICKED(IDC_BUTTON_MARKER_OTHER, &KThemeNodeValueClassify::OnBnClickedButtonMarkerOther)
END_MESSAGE_MAP()


// KThemeNodeValueClassify 메시지 처리기입니다.

void KThemeNodeValueClassify::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        CString strSubjectName(a_strSubjectName);

        if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
        {
			m_strFunction = _T("");
			m_mapCalculateResult.clear();

			if (nullptr != m_pCategoryFieldRecord)
			{
				QBicDelete(m_pCategoryFieldRecord);
			}

			m_mapNodeIdType.clear();

			m_dMinCalculateVal = 0.0;
			m_dMaxCalculateVal = 0.0;
			SetDlgItemText(IDC_STATIC_LOW_VALUE,  _T("0.0"));
			SetDlgItemText(IDC_STATIC_HIGH_VALUE, _T("0.0"));

			GetCategoryField       (m_vecColumnCategory);
			LoadInitialCategory    (m_vecColumnCategory);
			InitExpressionChecker  (m_vecColumnCategory);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

BOOL KThemeNodeValueClassify::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));

	m_btnMarker.SetFont(&m_oQbicDefaultMarkerFont);
	m_btnMarkerOther.SetFont(&m_oQbicDefaultMarkerFont);
	CString strSymbol;
	strSymbol.AppendChar(m_nSelectedMarkerIndex);  
	SetDlgItemText(IDC_BUTTON_MARKER, strSymbol);

	strSymbol = _T("");
	strSymbol.AppendChar(m_nOtherMarkerIndex);  
	SetDlgItemText(IDC_BUTTON_MARKER_OTHER, strSymbol);

	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

	SetDlgItemText(IDC_STATIC_LOW_VALUE,  _T("0.0"));
	SetDlgItemText(IDC_STATIC_HIGH_VALUE, _T("0.0"));

    KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, FALSE);
    	
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Select"), 40, FALSE));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Symbol"), 5));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기호/색상"));
    }
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Size"),   8));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("크기"));
    }
// 	CXTPReportInplaceButton* pSpinBtn = pColumn->GetEditOptions()->AddSpinButton(TRUE);
// 	pSpinBtn->SetSpinButtonMin(0);
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("<=  Range  <"),  20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("범위"));
    }
	
	m_wndReportCtrl.Populate();

	m_pNodeTable->Register(this);

	CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
	ControlRadioDrawData();

	CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
	ControlCheckOutRange();
	SetDlgItemText(IDC_EDIT_OUT_SIZE,  _T("5.0"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KThemeNodeValueClassify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KThemeNodeValueClassify::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KThemeNodeValueClassify::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDCtl == IDC_BUTTON_MARKER || nIDCtl == IDC_BUTTON_MARKER_OTHER)
	{
		// IDC_BUTTON_MARKER -> OwnerDraw True 설정할 것
		CDC dc;
		dc.Attach(lpDrawItemStruct ->hDC);
		RECT rect;
		rect= lpDrawItemStruct ->rcItem;
		dc.FillSolidRect(&rect,RGB(255,255,255));
		UINT state=lpDrawItemStruct->itemState;

		dc.DrawFocusRect(&rect);

		dc.SetBkColor(RGB(255, 255, 255));
		if (nIDCtl == IDC_BUTTON_MARKER)
		{
			dc.SetTextColor(m_clrSelectedMarkerColor);
		}
		else
		{
			dc.SetTextColor(m_clrOtherMarkerColor);
		}

		//dc.SetBkMode( TRANSPARENT );

		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH );
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);
		dc.DrawText(buffer, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		dc.Detach();
	}
	else
	{
		KDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}	
}

void KThemeNodeValueClassify::SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView)
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

	CWaitCursor cw;
	InitFitlerData();
	LoadInitialData();
}

void KThemeNodeValueClassify::SetNodeTable(KIOTable* a_pNodeTable)
{
	m_pNodeTable    = a_pNodeTable;
}

void KThemeNodeValueClassify::LoadInitialData()
{
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	CheckDlgButton(IDC_CHECK1, BST_CHECKED);

	m_dMinCalculateVal = 0.0;
	m_dMaxCalculateVal = 0.0;
	SetDlgItemText(IDC_STATIC_LOW_VALUE,  _T("0.0"));
	SetDlgItemText(IDC_STATIC_HIGH_VALUE, _T("0.0"));

	GetCategoryField       (m_vecColumnCategory);
	LoadInitialCategory    (m_vecColumnCategory);
	InitExpressionChecker  (m_vecColumnCategory);

	LoadInitialStyle();	
	LoadInitialClass();
	LoadInitialDigit();
	LoadReportCtrlData();
}


void KThemeNodeValueClassify::GetCategoryField(std::vector<KIOColumn*> &a_vecColumnCategory)
{
	a_vecColumnCategory.clear();

	try
	{
		KIOTables*         pIOTables  = m_pTarget->Tables();
		KIOTable*          pIOTable   = pIOTables->FindTable(TABLE_NODE);
		const KIOColumns*  pIOColumns = pIOTable->Columns();    
		int nColumnCount = pIOColumns->ColumnCount();

		KIOColumn*         pIOColumn  = nullptr;

		for ( int i=0; i<nColumnCount; i++ )
		{
			pIOColumn = pIOColumns->GetColumn(i);

			//Type Column 추가 금지.
			CString strColumnName = pIOColumn->Name();

			if (strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
				continue;

			if (strColumnName.CompareNoCase(COLUMN_NODE_X) == 0)
				continue;

			if (strColumnName.CompareNoCase(COLUMN_NODE_Y) == 0)
				continue;

			bool bEnabled = !pIOColumn->PrimaryKey();
			bEnabled = bEnabled && (pIOColumn->DataType() == KEMIODataTypeDouble);
			bEnabled = bEnabled && pIOColumn->CodeType() != KEMIOCodeTypeIsMultiCode;
			if ( bEnabled )
			{
				a_vecColumnCategory.push_back(pIOColumn);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecColumnCategory.clear();
	}
}


void KThemeNodeValueClassify::InitExpressionChecker(std::vector<KIOColumn*> &a_vecColumnCategory)
{
	try
	{
		if (nullptr != m_pExpressionChecker)
		{
			QBicDelete(m_pExpressionChecker);
		}

		m_pExpressionChecker = new KExpressionChecker;

		size_t nxCount = a_vecColumnCategory.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KIOColumn* pIOColumn = a_vecColumnCategory[i];
			m_pExpressionChecker->AddField(pIOColumn->Caption());
		}

		m_pExpressionChecker->AddFunction(_T("SQRT"));
		m_pExpressionChecker->AddFunction(_T("EXP"));
		m_pExpressionChecker->AddFunction(_T("LN"));
		m_pExpressionChecker->AddFunction(_T("LOG"));
		m_pExpressionChecker->AddFunction(_T("ABS"));

		m_pExpressionChecker->AddOperator(_T("+"));
		m_pExpressionChecker->AddOperator(_T("-"));
		m_pExpressionChecker->AddOperator(_T("*"));
		m_pExpressionChecker->AddOperator(_T("/"));
		m_pExpressionChecker->AddOperator(_T("^"));
		m_pExpressionChecker->AddOperator(_T("("));
		m_pExpressionChecker->AddOperator(_T(")"));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::LoadCategoryFieldRecord(std::vector<KIOColumn*> &a_vecColumnCategory)
{
	try
	{
		KIOColumns oIOColumnsCategory;

		size_t nxCount = a_vecColumnCategory.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KIOColumn* pIOColumn  = a_vecColumnCategory[i];

			oIOColumnsCategory.AddNewColumn(pIOColumn);
		}

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

		if (nullptr != m_pCategoryFieldRecord)
		{
			QBicDelete(m_pCategoryFieldRecord);
		}
		m_pCategoryFieldRecord = new KIDKeyDoubleRecords(nxCount);

		KBulkDBase::BulkNodeSelect(pIOTable, oIOColumnsCategory, *m_pCategoryFieldRecord);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::LoadInitialCategory(std::vector<KIOColumn*> &a_vecColumnCategory)
{
	m_cboCategory.ResetContent();
	int nPreSelectedIndex = -1;

	try
	{
		size_t     nxCount    = a_vecColumnCategory.size();
		KIOColumn* pIOColumn  = nullptr;

		for (size_t i= 0; i< nxCount; i++)
		{
			pIOColumn  = a_vecColumnCategory[i];

			int nIndex = m_cboCategory.AddString(pIOColumn->Caption());
			m_cboCategory.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			if (m_strPreSelectedColumnName.CompareNoCase(pIOColumn->Name()) == 0)
			{
				nPreSelectedIndex = nIndex;
			}
		}

		CButton* pButtonApply = (CButton*)GetDlgItem(IDC_BUTTON_APPLY);
		if (nxCount > 0)
		{
			if ( nPreSelectedIndex >= 0)
			{
				m_cboCategory.SetCurSel(nPreSelectedIndex);
			}
			else
			{
				m_cboCategory.SetCurSel(0);
			}		
			pButtonApply->EnableWindow(TRUE);
		}    
		else
		{
			pButtonApply->EnableWindow(FALSE);
		}

		if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
		{
			FieldSelectChangeAction();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KThemeNodeValueClassify::LoadInitialStyle()
{
	m_cboStyle.ResetContent();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
		return;

	std::vector<TNodeTemplate> vecNodeTemplate;
	KProjectDBase::NodeTemplate(spDBaseConnection, 1, vecNodeTemplate);

	int nPreSelectedIndex = -1;
	for (size_t i=0; i<vecNodeTemplate.size(); i++)
	{
		TNodeTemplate oTemplate = vecNodeTemplate[i];
		CString strName; strName.Format(_T("%d:%s"), oTemplate.TID, oTemplate.TName);
		int nIndex = m_cboStyle.AddString(strName);
		m_cboStyle.SetItemData(nIndex, (DWORD_PTR)oTemplate.TID);

		if (oTemplate.TID == m_nPreSelectedStyleID)
		{
			nPreSelectedIndex = nIndex;
		}
	}

	if (m_cboStyle.GetCount() > 0)
	{
		if (nPreSelectedIndex >= 0)
		{
			m_cboStyle.SetCurSel(nPreSelectedIndex);
		}
		else
		{
			m_cboStyle.SetCurSel(0);
		}			
	}
	if (m_cboStyle.GetCount() == 0)
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	}	

	StyleRadioSelectChangeAction();
}

void KThemeNodeValueClassify::OnBnClickedRadio()
{
	StyleRadioSelectChangeAction();
	LoadReportCtrlData();
}

void KThemeNodeValueClassify::StyleRadioSelectChangeAction()
{
	if (m_cboStyle.GetCount() == 0)
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
	}

	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MARKER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DIGIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CLASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MARKER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DIGIT)->EnableWindow(TRUE);
	}
}


void KThemeNodeValueClassify::OnCbnSelchangeComboField()
{
	int nCurSelectedIndex = m_cboCategory.GetCurSel();
	if (nCurSelectedIndex < 0)
	{
		return;
	}

	KIOColumn* pIOColumn  = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);
	if (pIOColumn == NULL)
	{
		AfxMessageBox(_T("관련 컬럼 정보를 찾을 수 없습니다."));
		return;
	}

	m_strPreSelectedColumnName = pIOColumn->Name();
	LoadReportCtrlData(false);	
	FieldSelectChangeAction();
}

void KThemeNodeValueClassify::FieldSelectChangeAction()
{
	try
	{
		int nCurSelectedIndex = m_cboCategory.GetCurSel();
		if (nCurSelectedIndex < 0)
			return;

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);
		KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		CString strSQL;
		//strSQL.Format(_T("Select min(%s), max(%s) From %s Where %s is not null "), pIOColumn->Name(), pIOColumn->Name(), pTable->Name(), pIOColumn->Name());
// 		strSQL.Format(_T("Select min(%s), max(%s) From %s Where %s is not null And %s < 9e999"),	//9e999 -> Infinity(무한대)를 제외함
// 			pIOColumn->Name(), pIOColumn->Name(), pIOTable->Name(), pIOColumn->Name(), pIOColumn->Name());

		strSQL.Format(_T(" Select min(%s), max(%s) From (Select ifnull(%s, 0) as %s From %s Where %s is null OR %s < 9e999) "), 
			pIOColumn->Name(), pIOColumn->Name(), pIOColumn->Name(), pIOColumn->Name(), pIOTable->Name(), pIOColumn->Name(), pIOColumn->Name());//  null 데이터 0 처리

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return;
		}

		if (pResult->Next())
		{
			CString strTemp;
			double dMinValue = pResult->GetValueDouble(0);

			if (dMinValue >= 0)
			{
				dMinValue -= 0.05;
				if (dMinValue < 0.0)
					dMinValue = 0.0;
			}
			else
			{
				dMinValue -= 0.05;
			}

			double dMaxValue = pResult->GetValueDouble(1);
			dMaxValue += 0.06;

			SetEditControlMinMax(dMinValue, dMaxValue);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KThemeNodeValueClassify::OnCbnSelchangeComboStyle()
{
	int nCurSelectedIndex = -99;
	int nStyleID          = -99;

	if ( m_cboStyle.GetCount() == 0)
	{
		return;
	}

	nCurSelectedIndex = m_cboStyle.GetCurSel();
	nStyleID          = (int)m_cboStyle.GetItemData(nCurSelectedIndex);
	m_nPreSelectedStyleID = nStyleID;

	LoadReportCtrlData();
	ClearNodeThemes();
}

void KThemeNodeValueClassify::StyleSelectChangeAction()
{
	LoadReportCtrlData();
}

void KThemeNodeValueClassify::OnCbnSelchangeComboClass()
{
	LoadReportCtrlData();
}

void KThemeNodeValueClassify::LoadInitialClass()
{
	m_cboClass.ResetContent();

	if (m_nPreSelectedClass < 0)
	{
		m_nPreSelectedClass = 5;
	}
	
	int nPreIndex = 0;

	CString strName; 
	for (int i=2; i<101; i++)
	{
		strName.Format(_T("%d"), i);
		int nIndex = m_cboClass.AddString(strName);
		m_cboClass.SetItemData(nIndex, (DWORD_PTR)i);

		if (i == m_nPreSelectedClass)
		{
			nPreIndex = nIndex;
		}
	}

	m_cboClass.SetCurSel(nPreIndex);
	m_nPreSelectedClass = m_cboClass.GetItemData(nPreIndex);
}

void KThemeNodeValueClassify::OnBnClickedCheck1()
{
	LoadReportCtrlData(false);
}

void KThemeNodeValueClassify::LoadInitialDigit()
{
	m_cboDigit.ResetContent();
	int nIndex = -1;
	
	nIndex = m_cboDigit.AddString(_T("1.0e+0"));
	m_cboDigit.SetItemData(nIndex, 0);

	nIndex = m_cboDigit.AddString(_T("1.0e+1"));
	m_cboDigit.SetItemData(nIndex, 1);

	nIndex = m_cboDigit.AddString(_T("1.0e+2"));
	m_cboDigit.SetItemData(nIndex, 2);

	nIndex = m_cboDigit.AddString(_T("1.0e+3"));
	m_cboDigit.SetItemData(nIndex, 3);

	nIndex = m_cboDigit.AddString(_T("1.0e+4"));
	m_cboDigit.SetItemData(nIndex, 4);

	nIndex = m_cboDigit.AddString(_T("1.0e-1"));
	m_cboDigit.SetItemData(nIndex, -1);

	nIndex = m_cboDigit.AddString(_T("1.0e-2"));
	m_cboDigit.SetItemData(nIndex, -2);

	nIndex = m_cboDigit.AddString(_T("1.0e-3"));
	m_cboDigit.SetItemData(nIndex, -3);

	nIndex = m_cboDigit.AddString(_T("1.0e-4"));
	m_cboDigit.SetItemData(nIndex, -4);

	m_cboDigit.SetCurSel(2);
}

void KThemeNodeValueClassify::OnCbnSelchangeComboDigit()
{
	LoadReportCtrlData(false);
}

void KThemeNodeValueClassify::OnBnClickedButtonMarker()
{
	//TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KPointMarkerSelectDlg dlg;
	dlg.SetInitialData(m_nSelectedMarkerIndex, m_clrSelectedMarkerColor);

	if (IDOK == dlg.DoModal())
	{
		int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
		if (nSelectMarkerIndex > 0)
		{
			CString strSymbol;
			strSymbol.AppendChar(nSelectMarkerIndex);     
			m_nSelectedMarkerIndex = nSelectMarkerIndex;
			SetDlgItemText(IDC_BUTTON_MARKER, strSymbol);

			m_clrSelectedMarkerColor = dlg.GetSelectedMarkerColor();

			GetDlgItem(IDC_BUTTON_MARKER)->RedrawWindow();
			
			CXTPReportRecord*     pRecord  = nullptr;
			CXTPReportRecordItem* pItem    = nullptr;
			CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
			size_t nxCount      = pRecords->GetCount();

			for (size_t i= 0; i< nxCount; i++)
			{
				 pRecord = pRecords->GetAt(i);
				 pItem   = pRecord->GetItem(1);
				 pItem->SetTextColor(m_clrSelectedMarkerColor);
				 pItem->SetCaption  (strSymbol);
                 pItem->SetItemData((DWORD_PTR)nSelectMarkerIndex);
			}
			if (nxCount > 0)
			{
				m_wndReportCtrl.RedrawControl();
			}
		}
	}
}

void KThemeNodeValueClassify::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;
		
	int nIndex = pItemNotify->pColumn->GetItemIndex();
	
	if ( 1 == nIndex )
	{
		KPointMarkerSelectDlg dlg;
		int      nSymbol = pItemNotify->pItem->GetItemData();
		COLORREF oColor  = pItemNotify->pItem->GetTextColor();
		dlg.SetInitialData(nSymbol, oColor);

		if (IDOK == dlg.DoModal())
		{
			int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
			if (nSelectMarkerIndex > 0)
			{
				CString strSymbol;
				strSymbol.AppendChar(nSelectMarkerIndex);        
				pItemNotify->pItem->SetCaption(strSymbol);
				pItemNotify->pItem->SetItemData(nSelectMarkerIndex);

				pItemNotify->pItem->SetTextColor(dlg.GetSelectedMarkerColor());
			}
		}
		m_wndReportCtrl.RedrawControl();
	}

	if ( 3 == nIndex )
	{
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
		m_strPreValue   = pItemText->GetValue();
		m_strPreCaption = pItemText->GetCaption(NULL);
	}
}

void KThemeNodeValueClassify::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nRowIndex    = pItemNotify->pRow->GetIndex();
	int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

	int nRecordCount = m_wndReportCtrl.GetRecords()->GetCount();

	CXTPReportRecordItemText* pItemText;
	
	if ( 2 == nColumnIndex)
	{
		CXTPReportRecordItemNumber* pItemSize  = (CXTPReportRecordItemNumber*)pItemNotify->pItem;
		double dSize              = pItemSize->GetValue();
		if (dSize < 0.0)
		{
			pItemSize->SetValue(0.0);
			m_wndReportCtrl.RedrawControl();
		}
	} 
	else if ( 3 == nColumnIndex )
	{
		pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
		CString strValue   = pItemText->GetValue();
		CString strCaption = pItemText->GetCaption(NULL);

		strValue.Replace(_T(" "), _T(""));
		strCaption.Replace(_T(" "), _T(""));

		std::vector<CString> vecValue;
		CString strTok;
		int     nTokenPos = 0; 

		if (nRowIndex == 0)
		{
// 			while (AfxExtractSubString(strTok, strValue, nTokenPos++, '~'))
// 			{
// 				vecValue.push_back(strTok);
// 			}
			while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '~'))
			{
				vecValue.push_back(strTok);
			}
		}
		else if (nRowIndex == (nRecordCount-1)) // last
		{
			while (AfxExtractSubString(strTok, strValue, nTokenPos++, '~'))
			{
				vecValue.push_back(strTok);
			}
		}
		else
		{
			while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '~'))
			{
				vecValue.push_back(strTok);
			}
		}		

		if (vecValue.size() != 2)
		{
			pItemText->SetValue(m_strPreValue);
			pItemText->SetCaption(m_strPreCaption);
			m_wndReportCtrl.RedrawControl();
			return;			
		}

		if (nRowIndex == 0)
		{
// 			if (QBicStringChecker::IsNumeric(vecValue[0]) == false)
// 			{
// 				AfxMessageBox(_T("숫자를 입력해 주세요."));
// 				pItemText->SetValue(m_strPreValue);
// 				pItemText->SetCaption(m_strPreCaption);
// 				m_wndReportCtrl.RedrawControl();
// 				return;
// 			}

			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("숫자를 입력해 주세요."));
				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_wndReportCtrl.RedrawControl();
				return;
			}
		}
		else if (nRowIndex == (nRecordCount-1)) // last
		{
			if (QBicStringChecker::IsNumeric(vecValue[0]) == false)
			{
				AfxMessageBox(_T("숫자를 입력해 주세요."));
				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_wndReportCtrl.RedrawControl();
				return;
			}
		}
		else
		{
			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("숫자를 입력해 주세요."));
				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_wndReportCtrl.RedrawControl();
				return;
			}
		}

		double dMin = 0.0;
		double dMax = 0.0;

		if (nRowIndex == 0)
		{
			//dMax = _ttof(vecValue[1]);
			dMax = _ttof(strValue);
		}
		else if (nRowIndex == (nRecordCount-1)) // last
		{
			dMin = _ttof(vecValue[0]);
		}
		else
		{
			dMin = _ttof(vecValue[0]);
			dMax = _ttof(strValue);
		}

		int nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);
		CString strMax = QBicStringFormat::FormatString(dMax, nDisplayDecimalPoint);
		CString strMin = QBicStringFormat::FormatString(dMin, nDisplayDecimalPoint);
		
		if (nRowIndex == 0)
		{
			strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);

			pItemText->SetCaption(strCaption);
		}
		else if (nRowIndex == (nRecordCount-1)) // last
		{
			strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);

			pItemText->SetCaption(strCaption);
		}
		else
		{
			if (dMin > dMax)
			{
				CString strOut;
				//strOut.Format(_T("%.1f 보다 큰값을 입력해주세요"), dMin);
				strOut.Format(_T("%s보다 큰 숫자를 입력해 주세요."), strMin);
				AfxMessageBox(strOut);

				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_wndReportCtrl.RedrawControl();
				return;
			}
			else
			{
				//strCaption.Format(_T("%.1f~%.1f"), dMin, dMax);
				strCaption.Format(_T("%s~%s"), strMin, strMax);

				pItemText->SetCaption(strCaption);
			}	
		}
	}

	RecalRange();
}

void KThemeNodeValueClassify::RecalRange()
{
	int nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
		
	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CXTPReportRecordItemText* pItemText    = NULL;

	double dPreValue = -99999999999999.0;

	for (int i=0; i<nRecordCount; i++)
	{
		pRecord   = pRecords->GetAt(i);
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(3);

		CString strCaption = pItemText->GetCaption(NULL);
		std::vector<CString> vecValue;
		CString strTok;
		int     nTokenPos = 0; 
		while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '~'))
		{
			vecValue.push_back(strTok);
		}

		double dMinValue = _ttof(vecValue[0]);
		double dMaxValue = _ttof(vecValue[1]);

		if (i == 0)
		{
			dPreValue = _ttof(vecValue[1]);
		}
		else
		{
			if (dPreValue != dMinValue)
			{
				dMinValue = dPreValue;
			}
		}

		if (dPreValue > dMaxValue)
		{
			dMaxValue = dPreValue;				
		}

		CString strMax = QBicStringFormat::FormatString(dMaxValue, nDisplayDecimalPoint);
		CString strMin = QBicStringFormat::FormatString(dMinValue, nDisplayDecimalPoint);

		CString strValue;
// 		strValue.Format(_T("%.1f"), dMaxValue);		
// 		strCaption.Format(_T("%.1f~%.1f"), dMinValue, dMaxValue);

		strValue.Format(_T("%s"), strMax);		

		if (0 == i)
		{
			strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);

			pItemText->SetValue(strValue);
		}
		else if (nRecordCount- 1 == i) //last
		{
			strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);

			pItemText->SetValue(strCaption);
		}
		else
		{
			strCaption.Format(_T("%s~%s"), strMin, strMax);

			pItemText->SetValue(strValue);
		}

		pItemText->SetCaption(strCaption);		
		dPreValue = dMaxValue;
	}

	m_wndReportCtrl.RedrawControl();
}


void KThemeNodeValueClassify::Clear()
{
	ClearNodeThemes();
}

void KThemeNodeValueClassify::ClearNodeThemes()
{
	m_pMapView->ClearNodeThemes();
}

void KThemeNodeValueClassify::LoadReportCtrlData(bool a_bStlyeReset)
{
	Clear();

	try
	{
		if (m_cboCategory.GetCount() == 0)
		{
			throw 1;
		}

		int nCurSelectedFieldIndex = m_cboCategory.GetCurSel();
		if (nCurSelectedFieldIndex < 0)
		{
			m_cboCategory.SetFocus();
			throw 1;
		}

		KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedFieldIndex);
		if (pIOColumn == NULL)
		{
			m_cboCategory.SetFocus();
			throw 1;
		}

		int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
		if (nCheckedRadioBtn == IDC_RADIO1)
		{
			LoadSavedStyle(a_bStlyeReset);
		}
		else
		{
			LoadUserStyle(a_bStlyeReset);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		m_wndReportCtrl.ResetContent();
		return;
	}

}

void KThemeNodeValueClassify::LoadSavedStyle(bool a_bStlyeReset)
{
	int nCurSelectedFieldIndex = m_cboCategory.GetCurSel();
	if (nCurSelectedFieldIndex < 0)
	{
		m_cboCategory.SetFocus();
		return;
	}

	KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedFieldIndex);
	if (pIOColumn == NULL)
	{
		m_cboCategory.SetFocus();
		return;
	}

	int nCurSelectedStyleIndex = m_cboStyle.GetCurSel();
	if (nCurSelectedStyleIndex < 0)
	{
		m_cboStyle.SetFocus();
		return;
	}

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	int nStyleID = m_cboStyle.GetItemData(nCurSelectedStyleIndex);
	std::vector<TNodeTemplateValue> vecTemplateValue;
	KProjectDBase::NodeTemplateValue(spDBaseConnection, nStyleID, vecTemplateValue);

	if (vecTemplateValue.size() < 1)
	{
		m_wndReportCtrl.ResetContent();
		return;
	}

	m_nRealDigit = 0;
#pragma region 자릿수 구하기
	int nLength(0);
	int nDigit(0);
	int nMaxDigit(0);
	for (size_t i= 0; i< vecTemplateValue.size(); i++)
	{
		CString &strMinVal = vecTemplateValue[i].TMin;
		nLength            = strMinVal.GetLength();

		int nFind   = strMinVal.Find(_T("."));
		if (nFind > 0)
		{
			nDigit = nLength - (nFind+1);
		}

		if (nMaxDigit < nDigit)
		{
			nMaxDigit = nDigit;
		}

		CString &strMaxVal = vecTemplateValue[i].TMax;
		nLength            = strMaxVal.GetLength();

		nFind   = strMaxVal.Find(_T("."));
		if (nFind > 0)
		{
			nDigit = nLength - (nFind+1);
		}

		if (nMaxDigit < nDigit)
		{
			nMaxDigit = nDigit;
		}
	}
	m_nRealDigit = -1 * nMaxDigit;
#pragma endregion 자릿수 구하기

	if (a_bStlyeReset)
	{
		ResetSavedStlye(vecTemplateValue);
	}
	else
	{
		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		size_t nxCount   = pRecords->GetCount();

		if (nxCount != vecTemplateValue.size())
		{
			ResetSavedStlye(vecTemplateValue);
		}
		else
		{
			RefreshOnlySavedRange(vecTemplateValue);
		}
	}
}

void KThemeNodeValueClassify::ResetSavedStlye(std::vector<TNodeTemplateValue> &a_vecTemplateValue)
{
	try
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		{
			bCheck = TRUE;
		}

		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		m_wndReportCtrl.ResetContent();

		size_t nxCount = a_vecTemplateValue.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TNodeTemplateValue& oTemplateValue = a_vecTemplateValue[i];

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			//select
			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);

			// 심볼
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			CString strSymbol;
			strSymbol.AppendChar(oTemplateValue.TSymbol);        
			pItem->SetCaption( strSymbol);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);
			pItem->SetFont(&m_oQbicDefaultMarkerFont); 
			pItem->SetItemData(oTemplateValue.TSymbol);
			pItem->SetTextColor(RGB(oTemplateValue.TR, oTemplateValue.TG, oTemplateValue.TB));

			// Size
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTemplateValue.TSize, _T("%.1f")));         
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnTextCenter);

			// Value
			CString strCaption, strValue;
			strValue.Format(_T("%s"), oTemplateValue.TMax);

			if (0 == i) // first
			{
				strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, oTemplateValue.TMax);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
				pItem->SetEditable(TRUE);
			}
			else if (nxCount-1 == i)
			{
				strCaption.Format(_T("%s~%s"), oTemplateValue.TMin, THEME_TEXT_ABOVE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
				pItem->SetEditable(FALSE);
			}
			else
			{
				strCaption.Format(_T("%s~%s"), oTemplateValue.TMin, oTemplateValue.TMax);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));		
				pItem->SetEditable(TRUE);
			}	

			pItem->SetCaption(strCaption);
		}

		m_wndReportCtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::RefreshOnlySavedRange(std::vector<TNodeTemplateValue> &a_vecTemplateValue)
{
	try
	{
		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItemText* pItem    = nullptr;
		int nCount          = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(3);

			TNodeTemplateValue& oTemplateValue = a_vecTemplateValue[i];

			// Value
			CString strCaption, strValue;
			strValue.Format(_T("%s"), oTemplateValue.TMax);

			if (0 == i) //first
			{
				strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW,    oTemplateValue.TMax);
				pItem->SetValue(strValue);
			}
			else if (nCount-1 == i) //last
			{
				strCaption.Format(_T("%s~%s"), oTemplateValue.TMin, THEME_TEXT_ABOVE);
				pItem->SetValue(strCaption);
			}
			else
			{
				strCaption.Format(_T("%s~%s"), oTemplateValue.TMin, oTemplateValue.TMax);
				pItem->SetValue(strValue);
			}			

			pItem->SetCaption(strCaption);
		}
		if (nCount > 0)
		{
			m_wndReportCtrl.RedrawControl();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::LoadUserStyle(bool a_bStlyeReset)
{
	try
	{
		KIOColumn* pIOColumn = nullptr;

		if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
		{
			int nCurSelectedFieldIndex = m_cboCategory.GetCurSel();
			if (nCurSelectedFieldIndex < 0)
			{
				m_cboCategory.SetFocus();
				return;
			}

			pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedFieldIndex);
			if (pIOColumn == NULL)
			{
				m_cboCategory.SetFocus();
				return;
			}
		}

		int nCurSelectedClassIndex = m_cboClass.GetCurSel();
		int nBreakCount            = m_cboClass.GetItemData(nCurSelectedClassIndex);

		int nCurSelectedDigitIndex = m_cboDigit.GetCurSel();
		int nDigit                 = m_cboDigit.GetItemData(nCurSelectedDigitIndex);

		std::vector<double> vecData;
		if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
		{
			CString strQueryFilter = _T("");
			//CString strQueryFilter = GenQueryFilter();
			//KBulkDBaseNode::AllNodeValue(m_pTarget, pIOColumn->Name(), vecData);
			KBulkDBaseNode::SrchNodeValue(m_pTarget, pIOColumn->Name(), vecData, strQueryFilter);
		}
		else
		{
			if (m_strFunction.IsEmpty())
			{
				m_wndReportCtrl.ResetContent();
				return;
			}

			GetDataByCalculate(vecData);
		}

		if (vecData.size() < 1)
		{
			m_wndReportCtrl.ResetContent();
			return;
		}

		KDevideDataInterval oDevideDataInterval;
		oDevideDataInterval.SetDataCalculate(vecData);

		m_nRealDigit = 0;
		BOOL bChkOptimizeDevide = IsDlgButtonChecked(IDC_CHECK1); 
		std::vector<TDevideDataInterval> ovecDevideInfo;
		if( TRUE == bChkOptimizeDevide)
		{
			ovecDevideInfo = oDevideDataInterval.GetOptimizeDevide(nBreakCount, nDigit);
		}
		else
		{
			ovecDevideInfo = oDevideDataInterval.GetDevide(nBreakCount, nDigit);
		}
		m_nRealDigit = oDevideDataInterval.LastDevideCount();

		if (a_bStlyeReset)
		{
			ResetUserStyle(ovecDevideInfo);
		}
		else
		{
			CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
			size_t nxCount      = pRecords->GetCount();

			if (nxCount != ovecDevideInfo.size())
			{
				ResetUserStyle(ovecDevideInfo);
			}
			else
			{
				RefreshOnlyUserRange(ovecDevideInfo);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::GetDataByCalculate(std::vector<double> &a_vecData)
{
	try
	{
		a_vecData.clear();

		AutoType iter  = m_mapCalculateResult.begin();
		AutoType itEnd = m_mapCalculateResult.end();

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;
			double  dValue   = iter->second;

			a_vecData.push_back(dValue);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecData.clear();
	}
}


void KThemeNodeValueClassify::GetDrawDataByCalculate( std::vector<TNodeIDValue> &ar_vecData, std::vector<TNodeIDValue> &ar_vecOutFilterData )
{
	try
	{
		ar_vecData.clear();
		ar_vecOutFilterData.clear();

		std::set<int> setFilterCheckType;
		GenFilter(setFilterCheckType);

		size_t nxCntFilter = m_vecFilterInfo.size();

		AutoType iter  = m_mapCalculateResult.begin();
		AutoType itEnd = m_mapCalculateResult.end();

		TNodeIDValue oTNodeIDValue;

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;
			double  dValue   = iter->second;

			bool bNoneData(false);
			if (m_bCheckedFilter && nxCntFilter != 0)
			{    
				int     nNodeType(-1);
				AutoType itFind = m_mapNodeIdType.find(nxNodeID);
				if (itFind != m_mapNodeIdType.end())
					nNodeType = itFind->second;

				if (setFilterCheckType.find(nNodeType) != setFilterCheckType.end())
					bNoneData = false;
				else
					bNoneData = true;
			}
			else
			{
				bNoneData = false;
			}

			oTNodeIDValue.TNodeID = nxNodeID;
			oTNodeIDValue.TValue  = dValue;

			if (!bNoneData)
			{
				ar_vecData.push_back(oTNodeIDValue);
			}
			else
			{
				ar_vecOutFilterData.push_back(oTNodeIDValue);
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ar_vecData.clear();
		ar_vecOutFilterData.clear();
	}
}


void KThemeNodeValueClassify::GetMinDecreaseMaxIncrease(int a_nDecimalPoint, double &a_dMinDecrease, double &a_dMaxIncrease)
{
	try
	{
		a_dMinDecrease = 0.5;
		a_dMaxIncrease = 0.6;

		for (int i= 0; i< a_nDecimalPoint; i++)
		{
			a_dMinDecrease *= 0.1;
			a_dMaxIncrease *= 0.1;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


int KThemeNodeValueClassify::GetDisplayDecimalPoint(int a_nRealDigit)
{
	int nDecimalPoint(1);

	try
	{
		if (a_nRealDigit >= 0)
		{
			nDecimalPoint = 1;
		}
		else
		{
			nDecimalPoint = a_nRealDigit * (-1);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return nDecimalPoint;
}


void KThemeNodeValueClassify::ResetUserStyle(std::vector<TDevideDataInterval> &a_ovecDevideInfo)
{
	try
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		{
			bCheck = TRUE;
		}

		double dMinDecrease(0.0);
		double dMaxIncrease(0.0);
		int    nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);
		GetMinDecreaseMaxIncrease(nDisplayDecimalPoint, dMinDecrease, dMaxIncrease);

		KMunsellColor oMunSellColor(5, false);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		m_wndReportCtrl.ResetContent();

		size_t nxCount = a_ovecDevideInfo.size();
		for ( size_t i=0; i< nxCount; i++ )
		{
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			//select
			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);

			// 심볼
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			CString strSymbol;
			strSymbol.AppendChar(m_nSelectedMarkerIndex);        
			pItem->SetCaption( strSymbol);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);
			pItem->SetFont(&m_oQbicDefaultMarkerFont); 
			pItem->SetItemData(m_nSelectedMarkerIndex);
			pItem->SetTextColor(m_clrSelectedMarkerColor);

			// Size
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(8.0, _T("%.1f")));         
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnTextCenter);

			// Value
			if (0 == i)//first
			{
				a_ovecDevideInfo[i].TMinValue = 0.0;
			}
			if (nxCount-1 == i)//last
			{
				a_ovecDevideInfo[i].TMaxValue = 0.0;
			}

			CString strMax = QBicStringFormat::FormatString(a_ovecDevideInfo[i].TMaxValue, nDisplayDecimalPoint);
			CString strMin = QBicStringFormat::FormatString(a_ovecDevideInfo[i].TMinValue, nDisplayDecimalPoint);
			CString strValue, strCaption;
			strValue.Format(_T("%s"), strMax);

			if (0 == i) //first
			{
				strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
				pItem->SetEditable(TRUE);
			} 
			else if (nxCount-1 == i) //last
			{
				strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
				pItem->SetEditable(FALSE);
			}
			else
			{
				strCaption.Format(_T("%s~%s"), strMin, strMax);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));		
				pItem->SetEditable(TRUE);
			}

			pItem->SetCaption(strCaption);
		}

		m_wndReportCtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::RefreshOnlyUserRange(std::vector<TDevideDataInterval> &a_ovecDevideInfo)
{
	try
	{
		double dMinDecrease(0.0);
		double dMaxIncrease(0.0);
		int    nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);
		GetMinDecreaseMaxIncrease(nDisplayDecimalPoint, dMinDecrease, dMaxIncrease);

		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItemText* pItem    = nullptr;
		int nCount          = pRecords->GetCount();

		for ( int i= 0; i< nCount; i++ )
		{
			pRecord = pRecords->GetAt(i);
			pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(3);

			// Value
			if (0 == i)//first
			{
				a_ovecDevideInfo[i].TMinValue = 0.0;
			}
			if (nCount-1 == i)//last
			{
				a_ovecDevideInfo[i].TMaxValue = 0.0;
			}

			CString strMax = QBicStringFormat::FormatString(a_ovecDevideInfo[i].TMaxValue, nDisplayDecimalPoint);
			CString strMin = QBicStringFormat::FormatString(a_ovecDevideInfo[i].TMinValue, nDisplayDecimalPoint);
			CString strValue, strCaption;
			strValue.Format(_T("%s"), strMax);
			
			if (0 == i)
			{
				strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);

				pItem->SetValue(strValue);
			}
			else if (nCount- 1 == i) //last
			{
				strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);

				pItem->SetValue(strCaption);
			}
			else
			{
				strCaption.Format(_T("%s~%s"), strMin, strMax);

				pItem->SetValue(strValue);
			}			

			pItem->SetCaption(strCaption);
		}
		if (nCount > 0)
		{
			m_wndReportCtrl.RedrawControl();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::Draw(bool a_bLabel)
{
	struct TMinMax
	{
		double   TMin;
		double   TMax;

		int      TMarkerIndex;
		COLORREF TColorRGB;
		double   TSize;

		KEMThemeRangeIndex emRangeIndex;
	};

	if (!InvalidateInput())
	{
		ClearNodeThemes();
		return;
	}

	int        nCurSelectedFieldIndex = m_cboCategory.GetCurSel();
	KIOColumn* pIOColumn  = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedFieldIndex);

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CXTPReportRecordItem*       pItemStyle = NULL;
	CXTPReportRecordItemNumber* pItemSize  = NULL;
	CXTPReportRecordItemText*   pItemText  = NULL;

	std::vector<TMinMax> vecTMinMax;
#pragma region vecTMinMax - 데이터 삽입
	for (int i=0; i<nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		if (pItem->IsChecked() == FALSE)
		{
			continue;
		}

		pItemStyle = pRecord->GetItem(1);
		pItemSize  = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		pItemText  = (CXTPReportRecordItemText*  )pRecord->GetItem(3);

		CString strCaption = pItemText->GetCaption(NULL);
		std::vector<CString> vecValue;
		CString strTok;
		int     nTokenPos = 0; 
		while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '~'))
		{
			vecValue.push_back(strTok);
		}

		TMinMax oTMinMax;

		oTMinMax.TMin = _ttof(vecValue[0]);
		oTMinMax.TMax = _ttof(vecValue[1]);
		oTMinMax.TMarkerIndex = pItemStyle->GetItemData();
		oTMinMax.TColorRGB    = pItemStyle->GetTextColor();
		oTMinMax.TSize        = pItemSize->GetValue();

		if (0 == i)
		{
			oTMinMax.emRangeIndex = KEMThemeRangeIndexFirst;
		}
		else if ((nRecordCount-1) == i) //last
		{
			oTMinMax.emRangeIndex = KEMThemeRangeIndexLast;
		}
		else
		{
			oTMinMax.emRangeIndex = KEMThemeRangeIndexNormal;
		}

		vecTMinMax.push_back(oTMinMax);
	}
#pragma endregion vecTMinMax - 데이터 삽입

	CString strQueryFilter = GenQueryFilter();

	std::vector<TNodeIDValue> vecData;
	std::vector<TNodeIDValue> vecNotFilterData;

	if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
	{
		KBulkDBaseNode::SrchNodeValue(m_pTarget, pIOColumn->Name(), vecData, strQueryFilter);

		if (m_bCheckedFilter && m_vecFilterInfo.size() != 0)
		{      
			CString strQueryFilter = GenNotQueryFilter();    
			KBulkDBaseNode::SrchNodeValue(m_pTarget, pIOColumn->Name(), vecNotFilterData, strQueryFilter);
		}
	}
	else
	{
		GetDrawDataByCalculate(vecData, vecNotFilterData);
	}

	size_t nxNodeCount    = vecData.size();
	size_t nxTMinMaxCount = vecTMinMax.size();

	std::vector<TDrawNodeInfo> vecDrawNodeInfo;
	std::vector<TDrawNodeInfo> vecOutDrawNodeInfo;
	TDrawNodeInfo              oTDrawNodeInfo;

	BOOL bIncludeZero = IsDlgButtonChecked(IDC_CHECK_INC_ZERO);
	BOOL bCheckedOutRange(FALSE);
	if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
		bCheckedOutRange = TRUE;

	CString strTemp(_T(""));
	GetDlgItemText(IDC_EDIT_OUT_SIZE, strTemp);
	double dOutSize = _ttof(strTemp);

	for (size_t i=0; i<nxNodeCount; i++)
	{
		TNodeIDValue& oTNodeIDValue = vecData[i];
		oTDrawNodeInfo.TNodeID = oTNodeIDValue.TNodeID;

		double dValue = oTNodeIDValue.TValue;

		bool bOutRange(false);
		if (!bIncludeZero && dValue == 0.0)
		{
			//bOutRange = true;
			continue; // outrange 판단이 아닌 그리지 않겠다로 변경
		}
		else
		{
			if (nxTMinMaxCount < 1)
			{
				bOutRange = true;
			}
			for (size_t k=0; k<nxTMinMaxCount; k++)
			{
				TMinMax& oTMinMax = vecTMinMax[k];

				if ( KEMThemeRangeIndexFirst == oTMinMax.emRangeIndex )
				{
					if (dValue < oTMinMax.TMax)
					{
						oTDrawNodeInfo.TSymbol = oTMinMax.TMarkerIndex;
						oTDrawNodeInfo.TColorRGB       = oTMinMax.TColorRGB;
						oTDrawNodeInfo.TSize           = oTMinMax.TSize;

						bOutRange = false;
						break;
					}
					else
					{
						bOutRange = true;
					}
				}
				else if ( KEMThemeRangeIndexLast == oTMinMax.emRangeIndex )
				{
					if (oTMinMax.TMin <= dValue)
					{
						oTDrawNodeInfo.TSymbol = oTMinMax.TMarkerIndex;
						oTDrawNodeInfo.TColorRGB       = oTMinMax.TColorRGB;
						oTDrawNodeInfo.TSize           = oTMinMax.TSize;

						bOutRange = false;
						break;
					}
					else
					{
						bOutRange = true;
					}
				}
				else // KEMThemeRangeIndexNormal
				{
					if (oTMinMax.TMin <= dValue && dValue < oTMinMax.TMax)
					{
						oTDrawNodeInfo.TSymbol = oTMinMax.TMarkerIndex;
						oTDrawNodeInfo.TColorRGB       = oTMinMax.TColorRGB;
						oTDrawNodeInfo.TSize           = oTMinMax.TSize;

						bOutRange = false;

						break;
					}
					else
					{
						bOutRange = true;
					}
				}
			}
		}

		// 최종 판단 (Out of range 체크박스에 체크가 되어 있을 경우만 OutRange 라고 판단된 데이터를 수집 한다.)
		if (bOutRange)
		{
			if (FALSE == bCheckedOutRange)
			{
				continue;
			}

			oTDrawNodeInfo.TSymbol = m_nOtherMarkerIndex;
			oTDrawNodeInfo.TColorRGB       = m_clrOtherMarkerColor;
			oTDrawNodeInfo.TSize           = dOutSize;
			oTDrawNodeInfo.TLabel          = _T("");

			vecOutDrawNodeInfo.push_back(oTDrawNodeInfo);
		}
		else
		{
			CString strLabel(_T(""));

			if (0.0 == oTDrawNodeInfo.TSize)
			{
				strLabel = _T("");
				oTDrawNodeInfo.TSize = 0.1; //jyk (m_pMapView->DrawNodeThemes 에서 size 0으로 설정 하고 Draw 하면 제법 큰 원이 그려진다. 0.1로 하여 보이지 않게 한다)
			}
			else
			{
				if (dValue > 0.009 || dValue == 0.0 || dValue < -0.009)
				{
					strLabel.Format(_T("%.2f"), dValue);
				}
				else
				{
					//strLabel.Format(_T("%.5f"), dValue);
					strLabel.Format(_T("%g"), dValue);
				}
			}

			oTDrawNodeInfo.TLabel = strLabel;
			vecDrawNodeInfo.push_back(oTDrawNodeInfo);
		}
	}

	if (m_bCheckedFilter && m_vecFilterInfo.size() != 0)
	{    
		if (TRUE == bCheckedOutRange)
		{
			//-jyk-14.04.16 명지대요청: 선택하지 않은 타입은 Outrange가 아닌 그리지 않는것으로 변경
// 			for (size_t i=0; i<vecNotFilterData.size(); i++)
// 			{
// 				TNodeIDValue& oTNodeIDValue = vecNotFilterData[i];
// 				TDrawNodeInfo oTDrawNodeInfo;
// 				
// 				oTDrawNodeInfo.TNodeID         = oTNodeIDValue.TNodeID;
// 				oTDrawNodeInfo.TCharacterIndex = m_nOtherMarkerIndex;
// 				oTDrawNodeInfo.TColorRGB       = m_clrOtherMarkerColor;
// 				oTDrawNodeInfo.TSize           = dOutSize;
// 				oTDrawNodeInfo.TLabel          = _T("");
// 
// 				vecOutDrawNodeInfo.push_back(oTDrawNodeInfo);
// 			}
		}
	}

	std::sort(vecDrawNodeInfo.begin(), vecDrawNodeInfo.end(), Descending);
	size_t nxCount = vecDrawNodeInfo.size();
	for (size_t i=0; i< nxCount; i++)
	{
		vecOutDrawNodeInfo.push_back(vecDrawNodeInfo[i]);
	}

	m_pMapView->DrawNodeThemes(vecOutDrawNodeInfo, a_bLabel);
}


bool KThemeNodeValueClassify::InvalidateInput()
{
	try
	{
		if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
		{
			int nCurSelectedFieldIndex = m_cboCategory.GetCurSel();
			if (nCurSelectedFieldIndex < 0)
			{
				m_cboCategory.SetFocus();
				return false;
			}

			KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedFieldIndex);
			if (pIOColumn == NULL)
			{
				m_cboCategory.SetFocus();
				return false;
			}
		}
		else
		{
			if (m_strFunction.IsEmpty())
			{
				CString strBtnCaption(_T(""));
				GetDlgItemText(IDC_BUTTON1, strBtnCaption);

				CString strMsg(_T(""));
				strMsg.Format(_T("데이터 산출식이 정의되지 않았습니다.\r\n'%s' 버튼을 클릭하여 입력해 주세요."), strBtnCaption);
				AfxMessageBox(strMsg);
				return false;
			}

			if (m_mapCalculateResult.size()< 1)
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("해당 산출식에 대한 결과 값이 존재하지 않습니다.\r\n입력한 산출식 : %s"), m_strFunction);
				AfxMessageBox(strMsg);
				return false;
			}
		}

		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
		{
			CString strTemp(_T(""));
			GetDlgItemText(IDC_EDIT_OUT_SIZE, strTemp);
			if (!QBicStringChecker::IsNumeric(strTemp))
			{
				AfxMessageBox(_T("숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_OUT_SIZE)->SetFocus();
				return false;
			}

			double dOutSize = _ttof(strTemp);

			if (dOutSize < 0.0)
			{
				AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
				GetDlgItem(IDC_EDIT_OUT_SIZE)->SetFocus();
				return false;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KThemeNodeValueClassify::OnBnClickedButtonStyleSave()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	int     nCurSelectedIndex = m_cboStyle.GetCurSel();
	CString strExistStyleName;
	int     nID = 0;
	if (nCurSelectedIndex >= 0)
	{
		m_cboStyle.GetLBText(nCurSelectedIndex, strExistStyleName);
		nID = m_cboStyle.GetItemData(nCurSelectedIndex);
	}

	KThemeStyleSave dlg;
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		dlg.SetExistStyleName(strExistStyleName);
	}
	
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	CXTPReportRecordItemText* pItemText = NULL;
	
	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CString strSQL;
	bool bOverwrite = dlg.IsOverWrite();	

	try
	{
		spDBaseConnection->BeginTransaction();

		if (!bOverwrite)
		{
			CString strNewStyleName = dlg.GetNewStyleName();
			nID = KProjectDBase::NewNodeTemplateID(spDBaseConnection, 1);	
			strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplate(ID,Type,Name) Values('%d','%d','%s')"), nID,1,strNewStyleName.Trim());
			spDBaseConnection->ExecuteUpdate(strSQL);
		}		

		for (int i=0; i<nRecordCount; i++)
		{
			CString      strRGB;
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(1);

			int      nSymbol = pItem->GetItemData();
			COLORREF oColor  = pItem->GetTextColor();
			strRGB.Format(_T("%d,%d,%d"), GetRValue(oColor), GetGValue(oColor), GetBValue(oColor));

			pItem         = pRecord->GetItem(2);
			double dSize  = _ttof(pItem->GetCaption(NULL));

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(3);
			CString strCaption = pItemText->GetCaption(NULL);
			std::vector<CString> vecValue;
			CString strTok;
			int     nTokenPos = 0; 
			while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '~'))
			{
				vecValue.push_back(strTok);
			}

			if (vecValue.size() < 2)
				throw 1;

			if (0 == i) //first
			{
				vecValue[0] = _T("0.0"); // Below 처리
			}
			else if ( (nRecordCount -1) == i) //last
			{
				vecValue[1] = _T("0.0"); // Above 처리
			}

			double dMin = _ttof(vecValue[0]);
			double dMax = _ttof(vecValue[1]);

			//strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplateValue(ID,Seq,Symbol,Size, Min, Max, RGB) Values('%d','%d','%d','%.1f','%.1f','%.1f','%s')"), nID, i, nSymbol, dSize, dMin, dMax, strRGB);
			strSQL.Format(_T("INSERT OR REPLACE INTO NodeTemplateValue(ID,Seq,Symbol,Size, Min, Max, RGB) Values('%d','%d','%d','%.1f','%s','%s','%s')"), nID, i, nSymbol, dSize, vecValue[0], vecValue[1], strRGB);

			spDBaseConnection->ExecuteUpdate(strSQL);
		}
		spDBaseConnection->Commit();

		m_nPreSelectedStyleID = nID;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		AfxMessageBox(_T("저장 중 오류가 발생하였습니다."));
	}	

	LoadInitialStyle();
}


void KThemeNodeValueClassify::OnBnClickedButtonStyleEdit()
{
	KPointStyleValueEdit dlg;
	dlg.DoModal();

	LoadInitialStyle();
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		StyleSelectChangeAction();
	}	
}


bool KThemeNodeValueClassify::Descending( TDrawNodeInfo left, TDrawNodeInfo right )
{
	return left.TSize > right.TSize;
}


void KThemeNodeValueClassify::InitFitlerData( void )
{
	m_vecFilterInfo.clear();

	vector<int>   vecTypeNode;
	map<int, int> mapMatchingCode;

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strQuery;

		//node테이블에서 사용하는 node_type list를 조회
		strQuery.Format(_T("select %s from %s group by %s order by %s asc"), 
			COLUMN_NODE_TYPE, TABLE_NODE, COLUMN_NODE_TYPE, COLUMN_NODE_TYPE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int nTypeNode = spResultSet->GetValueInt(0);
			vecTypeNode.push_back(nTypeNode);
		}

		//define_system_code테이블에서 user_node_tp과, System_node_tp의 매칭코드를 조회
		strQuery.Format(_T("Select user_code_key, system_code_key From %s where user_code_group = %d"),
			TABLE_DEFINE_SYSTEM_CODE, USER_NODECODE_GROUP_KEY);

		spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int nCodeUser   = spResultSet->GetValueInt(0);
			int nCodeSystem = spResultSet->GetValueInt(1);

			mapMatchingCode.insert(std::make_pair(nCodeUser, nCodeSystem));
		}

		int nCntType = vecTypeNode.size();

		for (int i = 0; i < nCntType; ++i)
		{
			TFilterInfo* pFilterInfo = new TFilterInfo;

			int nCodeUser = vecTypeNode[i];
			int nCodeSystem(0);

			AutoType findIter = mapMatchingCode.find(nCodeUser);
			AutoType endIter  = mapMatchingCode.end();

			if (findIter != endIter)
			{
				nCodeSystem = findIter->second;
			}
			else
			{
				nCodeSystem = HIGHWAY_CODE_KEY;
			}

			pFilterInfo->bChecked    = TRUE;
			pFilterInfo->nCodeUser   = nCodeUser;
			pFilterInfo->nCodeSystem = nCodeSystem;

			m_vecFilterInfo.push_back(pFilterInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KThemeNodeValueClassify::OnBnClickedBtnFilter()
{
	KThemeSetFilterDlg oThemeSetFilterDlg(m_pTarget, m_vecFilterInfo, USER_NODECODE_GROUP_KEY, SYSTEM_NODECODE_GROUP_KEY);

	if (oThemeSetFilterDlg.DoModal() == IDOK)
	{
		UpdateTxtFilter();
	}
}

void KThemeNodeValueClassify::OnBnClickedFilter()
{
	m_bCheckedFilter = IsDlgButtonChecked(IDC_CHECK_FILTER);

	if (m_bCheckedFilter)
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FILTER1)->EnableWindow(TRUE);

		UpdateTxtFilter();
	}
	else
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FILTER1)->EnableWindow(FALSE);
	}
}


void KThemeNodeValueClassify::GenFilter(std::set<int> &a_setCheckType)
{
	try
	{
		if (m_bCheckedFilter)
		{
			int nCntFilter = m_vecFilterInfo.size();

			for (int i = 0; i < nCntFilter; ++i)
			{
				TFilterInfo* pFilterInfo = m_vecFilterInfo[i];

				if (!pFilterInfo->bChecked)
					continue;

				a_setCheckType.insert(pFilterInfo->nCodeUser);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		a_setCheckType.clear();
	}
}


CString KThemeNodeValueClassify::GenQueryFilter( void )
{
	CString strQueryFilter(_T(""));

	if (m_bCheckedFilter)
	{
		int nCntFilter = m_vecFilterInfo.size();

		if (nCntFilter == 0)
			return strQueryFilter;

		CString strQuery2(_T(""));
		for (int i = 0; i < nCntFilter; ++i)
		{
			CString strTmp;
			TFilterInfo* pFilterInfo = m_vecFilterInfo[i];

			if (!pFilterInfo->bChecked)
				continue;

			strTmp.Format(_T("%d, "), pFilterInfo->nCodeUser);
			strQuery2.Append(strTmp);
		}

		if (strQuery2.GetLength() != 0)
			strQuery2.Truncate(strQuery2.GetLength() - 2);

		strQueryFilter.Format(_T(" where %s in ( "), COLUMN_NODE_TYPE);
		strQueryFilter.Append(strQuery2);
		strQueryFilter.Append(_T(" ) "));
	}

	return strQueryFilter;
}

CString KThemeNodeValueClassify::GenNotQueryFilter( void )
{
	CString strQueryFilter(_T(""));

	if (m_bCheckedFilter)
	{
		CString strQuery2(_T(""));

		int nCntFilter = m_vecFilterInfo.size();

		if (nCntFilter == 0)
			return strQueryFilter;

		for (int i = 0; i < nCntFilter; ++i)
		{
			CString strTmp;
			TFilterInfo* pFilterInfo = m_vecFilterInfo[i];

			if (!pFilterInfo->bChecked)
				continue;

			strTmp.Format(_T("%d, "), pFilterInfo->nCodeUser);
			strQuery2.Append(strTmp);
		}

		if (strQuery2.GetLength() != 0)
			strQuery2.Truncate(strQuery2.GetLength() - 2);

		strQueryFilter.Format(_T(" where %s not in ( "), COLUMN_NODE_TYPE);
		strQueryFilter.Append(strQuery2);
		strQueryFilter.Append(_T(" ) "));
	}

	return strQueryFilter;
}

void KThemeNodeValueClassify::UpdateTxtFilter( void )
{
	//update idc_edit1;
	CString strFilter(_T(""));

	KCodeManager* pCodeMgr       = m_pTarget->CodeManager();
	KCodeGroup*   pCodeUserGroup = pCodeMgr->FindCodeGroup(USER_NODECODE_GROUP_KEY);

	int nCntFilter = m_vecFilterInfo.size();

	for (int i = 0; i < nCntFilter; ++i)
	{
		TFilterInfo* pFilterInfo = m_vecFilterInfo[i];

		if (!pFilterInfo->bChecked)
			continue;

		CString strCode;
		strCode.Format(_T("%d"), pFilterInfo->nCodeUser);

		strFilter.Append(strCode);
		strFilter.Append(_T(", "));
	}

	if (strFilter.GetLength() != 0)
		strFilter.Truncate(strFilter.GetLength() - 2);

	GetDlgItem(IDC_EDIT1)->SetWindowText(strFilter);
}

BOOL KThemeNodeValueClassify::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_LBUTTONDOWN &&
		pMsg->hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
	{
		OnBnClickedBtnFilter();
		return TRUE;
	}

	return KDialogEx::PreTranslateMessage(pMsg);
}


void KThemeNodeValueClassify::OnBnClickedButtonFormula()
{
	try
	{
		KThemeFunctionDlg oDlg;
		oDlg.Expression(m_strFunction);
		oDlg.FieldList(m_pExpressionChecker->FieldList());
		oDlg.FunctionList(m_pExpressionChecker->FunctionList());
		oDlg.OperatorList(m_pExpressionChecker->OperatorList());

		if (oDlg.DoModal() == IDOK)
		{
			m_strFunction = oDlg.Expression();
			m_strFunction.Trim();

			m_bCalcResult = false;

			// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
			int nLang = 1;
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				nLang = 0;
			}

			QBicSimpleProgressThread::ExecuteThread(CalculateResultThreadCaller, this, false, nLang);
			if (m_bCalcResult)
			{
				LoadReportCtrlData(false);
				SetEditControlMinMax(m_dMinCalculateVal, m_dMaxCalculateVal);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KThemeNodeValueClassify::CalculateResultThreadCaller(void* p)
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("계산 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Calculating..."));
	}

	KThemeNodeValueClassify* pDlg = (KThemeNodeValueClassify*)pParameter->GetParameter();
	pDlg->CalculateResult();

	return 0;
}


bool KThemeNodeValueClassify::CalculateResult()
{
	try
	{
		m_mapCalculateResult.clear();
		m_dMinCalculateVal = 0.0;
		m_dMaxCalculateVal = 0.0;

		if (m_mapNodeIdType.size() < 1)
		{
			KBulkDBaseNode::NodeType( m_pTarget, m_mapNodeIdType );
		}

		if (nullptr == m_pCategoryFieldRecord)
		{
			LoadCategoryFieldRecord(m_vecColumnCategory);
		}
		else
		{
			if (m_pCategoryFieldRecord->GetRecordCount() < 1)
			{
				LoadCategoryFieldRecord(m_vecColumnCategory);
			}
		}

		KIOTables*   pIOTables  = m_pTarget->Tables();
		KIOTable*    pIOTable   = pIOTables->FindTable(TABLE_NODE);

		KParseExpression oParseExpression;
		oParseExpression.SetTable(pIOTable);
		oParseExpression.SetCaptionExpression(m_strFunction);

		std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
		CString              strRealFunction   = oParseExpression.GetRealExpression();
		vector<int>          vecIdx; // vecUsedColumnName와 매칭하는 컬럼의 Index를 가지는 Vector;

		int nCntVecField = (int)vecUsedColumnName.size();
		for (int i= 0; i< nCntVecField; i++)
		{
			CString &strColName = vecUsedColumnName[i];

			for (size_t k= 0; k< m_vecColumnCategory.size(); k++)
			{
				KIOColumn* pColumnCategory   = m_vecColumnCategory[k];
				CString    strColCulateName = pColumnCategory->Name();

				if (strColName.CompareNoCase(strColCulateName) == 0)
				{
					vecIdx.push_back(k);
				}
			}
		}

		if ( vecUsedColumnName.size() != vecIdx.size() )
		{
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
			return false;
		}

		KParser oParser;
		oParser.SetExpr(strRealFunction);
		double* dValueArray = new double[nCntVecField];

		for (int i= 0; i< nCntVecField; i++)
		{
			oParser.DefineVar(vecUsedColumnName[i], &dValueArray[i]);
		}

		AutoType iter  = m_pCategoryFieldRecord->begin();
		AutoType itEnd = m_pCategoryFieldRecord->end();

		double dMaxValue = -999999999999.0;
		double dMinValue = 999999999999.0;

		while (iter != itEnd)
		{
			Integer         nxLinkID = iter->first;
			KSDoubleRecord* pRecord  = iter->second;

			for (int i= 0; i< nCntVecField; i++)
			{
				dValueArray[i] = pRecord->GetAt(vecIdx[i]);
			}

			double dResultVal = oParser.Eval();
			m_mapCalculateResult.insert(std::make_pair(nxLinkID, dResultVal));

			if (dMinValue > dResultVal)
				dMinValue = dResultVal;

			if (dMaxValue < dResultVal)
				dMaxValue = dResultVal;

			++iter;
		}
		delete[] dValueArray;

		if (dMinValue == dMaxValue && dMinValue == 0.0)
		{
			m_dMinCalculateVal = 0.0;
			m_dMaxCalculateVal = 0.0;
		}
		else
		{
			if (dMinValue >= 0)
			{
				dMinValue -= 0.05;
				if (dMinValue < 0.0)
					dMinValue = 0.0;
			}
			else
			{
				dMinValue -= 0.05;
			}

			dMaxValue += 0.06;

			m_dMinCalculateVal = dMinValue;
			m_dMaxCalculateVal = dMaxValue;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	m_bCalcResult = true;
	return true;
}


void KThemeNodeValueClassify::SetEditControlMinMax( double a_dMinVal, double a_dMaxVal )
{
	CString strTemp(_T(""));

	strTemp.Format(_T("%.1f"), a_dMinVal);
	SetDlgItemText(IDC_STATIC_LOW_VALUE,  strTemp);

	strTemp.Format(_T("%.1f"), a_dMaxVal);
	SetDlgItemText(IDC_STATIC_HIGH_VALUE, strTemp);
}


void KThemeNodeValueClassify::OnBnClickedRadioDrawData( UINT nID )
{
	ControlRadioDrawData();
	LoadReportCtrlData(false);
}


void KThemeNodeValueClassify::ControlRadioDrawData()
{
	try
	{
		if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
		{
			GetDlgItem(IDC_COMBO_FIELD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

			OnCbnSelchangeComboField();
		}
		else
		{
			GetDlgItem(IDC_COMBO_FIELD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

			SetEditControlMinMax(m_dMinCalculateVal, m_dMaxCalculateVal);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::OnBnClickedCheckAll()
{
	ControlCheckAll();
}


void KThemeNodeValueClassify::ControlCheckAll()
{
	try
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*           pRecord    = nullptr;
		CXTPReportRecordItem*       pItem      = nullptr;

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem              = pRecord->GetItem(0);
			pItem->SetChecked(bCheck);
		}

		m_wndReportCtrl.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	try
	{
		BOOL bChecked = pItemNotify->pItem->IsChecked();

		if (!bChecked)
		{
			CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::OnBnClickedCheckAllOtherValues()
{
	ControlCheckOutRange();
}


void KThemeNodeValueClassify::ControlCheckOutRange()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
		{
			m_btnMarkerOther.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_OUT_SIZE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC12)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnMarkerOther.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_OUT_SIZE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC12)->ShowWindow(SW_HIDE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueClassify::OnBnClickedButtonMarkerOther()
{
	KPointMarkerSelectDlg dlg;
	dlg.SetInitialData(m_nOtherMarkerIndex, m_clrOtherMarkerColor);

	if (IDOK == dlg.DoModal())
	{
		int nSelectMarkerIndex = dlg.GetSelectedMarkerIndex();
		if (nSelectMarkerIndex > 0)
		{
			CString strSymbol;
			strSymbol.AppendChar(nSelectMarkerIndex);     
			m_nOtherMarkerIndex = nSelectMarkerIndex;
			SetDlgItemText(IDC_BUTTON_MARKER_OTHER, strSymbol);

			m_clrOtherMarkerColor = dlg.GetSelectedMarkerColor();

			GetDlgItem(IDC_BUTTON_MARKER_OTHER)->RedrawWindow();
		}
	}
}
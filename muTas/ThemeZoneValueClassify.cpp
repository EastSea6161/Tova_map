// ThemeZoneValueClassify.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeZoneValueClassify.h"
#include "afxdialogex.h"

#include "Target.h"
#include "MapView.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "ThemeStyleSave.h"

#include "BulkDBaseDemarcation.h"
#include "PolygonStyleValueEdit.h"

#include "ThemeFunctionDlg.h"
#include "ExpressionChecker.h"
#include "ParseExpression.h"
#include "KBulkDBase.h"
#include "TThemeType.h"
#include "DefineNames.h"

// KThemeZoneValueClassify 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeZoneValueClassify, KDialogEx)

KThemeZoneValueClassify::KThemeZoneValueClassify(CWnd* pParent /*=NULL*/)
	: KDialogEx(KThemeZoneValueClassify::IDD, pParent), m_pZoneTable(nullptr), m_pSEDTable(nullptr)
	, m_nRealDigit(0)
	, m_dMinCalculateVal(0.0)
	, m_dMaxCalculateVal(0.0)
{
	m_clrSelectedLineColor = RGB(255, 0, 0);

	m_strFunction = _T("");
	m_pExpressionChecker   = new KExpressionChecker;
	m_pCategoryFieldRecord = nullptr;
}

KThemeZoneValueClassify::~KThemeZoneValueClassify()
{
    try
    {
        m_pZoneTable->Unregister(this);
        m_pSEDTable->Unregister(this);

		if (nullptr != m_pExpressionChecker)
		{
			QBicDelete(m_pExpressionChecker);
		}

		if (nullptr != m_pCategoryFieldRecord)
		{
			QBicDelete(m_pCategoryFieldRecord);
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

void KThemeZoneValueClassify::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_cboCategory);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cboStyle);
	DDX_Control(pDX, IDC_COMBO_CLASS, m_cboClass);
	DDX_Control(pDX, IDC_COMBO_DIGIT, m_cboDigit);
	DDX_Control(pDX, IDC_BUTTON_MARKER, m_btnLineSymbol);
	DDX_Control(pDX, IDC_BUTTON_MARKER_COLOR_OTHER, m_btnOtherSymbolColorPicker);
}


BEGIN_MESSAGE_MAP(KThemeZoneValueClassify, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_MARKER, &KThemeZoneValueClassify::OnBnClickedButtonMarker)	
	ON_BN_CLICKED(IDC_RADIO1, &KThemeZoneValueClassify::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &KThemeZoneValueClassify::OnBnClickedRadio)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD, &KThemeZoneValueClassify::OnCbnSelchangeComboField)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &KThemeZoneValueClassify::OnCbnSelchangeComboStyle)	
	ON_CBN_SELCHANGE(IDC_COMBO_CLASS, &KThemeZoneValueClassify::OnCbnSelchangeComboClass)
	ON_BN_CLICKED(IDC_CHECK1, &KThemeZoneValueClassify::OnBnClickedCheck1)

	ON_NOTIFY(NM_CLICK, IDC_REPORT,   &KThemeZoneValueClassify::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KThemeZoneValueClassify::OnReportItemValueChanged)
	//XTP_NM_REPORT_SELCHANGED

	ON_CBN_SELCHANGE(IDC_COMBO_DIGIT, &KThemeZoneValueClassify::OnCbnSelchangeComboDigit)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_SAVE, &KThemeZoneValueClassify::OnBnClickedButtonStyleSave)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_EDIT, &KThemeZoneValueClassify::OnBnClickedButtonStyleEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &KThemeZoneValueClassify::OnBnClickedButtonFormula)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO4, &KThemeZoneValueClassify::OnBnClickedRadioDrawData)
	ON_BN_CLICKED(IDC_CHECK_ALL, &KThemeZoneValueClassify::OnBnClickedCheckAll)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KThemeZoneValueClassify::OnReportCheckItem)

	ON_BN_CLICKED(IDC_CHECK3, &KThemeZoneValueClassify::OnBnClickedCheckAllOtherValues)
END_MESSAGE_MAP()


void KThemeZoneValueClassify::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        CString strSubjectName(a_strSubjectName);
		
		bool bRefesh(false);

		if(strSubjectName.CompareNoCase(TABLE_ZONE) == 0)
		{
			if (m_bZoneTable)
				bRefesh = true;
		}
		else if (strSubjectName.CompareNoCase(TABLE_SED_VECTOR) == 0)
		{
			if (!m_bZoneTable)
				bRefesh = true;
		}

		if (bRefesh)
		{
			m_strFunction = _T("");
			m_mapCalculateResult.clear();

			if (nullptr != m_pCategoryFieldRecord)
			{
				QBicDelete(m_pCategoryFieldRecord);
			}

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


BOOL KThemeZoneValueClassify::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255,255,255));
	CString strSymbol(_T("■"));
	SetDlgItemText(IDC_BUTTON_MARKER, strSymbol);

	CheckDlgButton(IDC_CHECK_INC_ZERO, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

	SetDlgItemText(IDC_STATIC_LOW_VALUE,  _T("0.0"));
	SetDlgItemText(IDC_STATIC_HIGH_VALUE, _T("0.0"));

	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, FALSE);

	CXTPReportColumn* pColumn = NULL;
	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 40, FALSE));
    pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(1, _T("Color"), 5));
    pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(2, _T("<=  Range  <"),  20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("범위"));
    }

	m_ctrlReport.Populate();

	m_btnOtherSymbolColorPicker.SetColor( RGB(192, 192, 192) );
	m_btnOtherSymbolColorPicker.SetWindowText( _T("") );
	m_btnOtherSymbolColorPicker.ShowText( TRUE );
	m_btnOtherSymbolColorPicker.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
	ControlRadioDrawData();

	CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
	ControlCheckOutRange();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KThemeZoneValueClassify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KThemeZoneValueClassify::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KThemeZoneValueClassify::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDCtl == IDC_BUTTON_MARKER)
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
		dc.SetTextColor(m_clrSelectedLineColor);
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


void KThemeZoneValueClassify::SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView, bool a_bZoneTable)
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;
	m_bZoneTable    = a_bZoneTable;
	
	CWaitCursor cw;
 
	if (m_pSEDTable)
    {        
        m_pSEDTable->Unregister(this);
    }

    if (m_pZoneTable)
    {
        m_pZoneTable->Unregister(this);
    }

    KIOTables*   pIOTables = m_pTarget->Tables();
    m_pZoneTable = pIOTables->FindTable(TABLE_ZONE);
    m_pSEDTable  = pIOTables->FindTable(TABLE_SED_VECTOR);

    m_pZoneTable->Register(this);
    m_pSEDTable->Register(this);
	LoadInitialData();
}

void KThemeZoneValueClassify::LoadInitialData()
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


void KThemeZoneValueClassify::GetCategoryField(std::vector<KIOColumn*> &a_vecColumnCategory)
{
	a_vecColumnCategory.clear();

	try
	{
		KIOTable*  pIOTable  = nullptr;
		if (!m_bZoneTable)
			pIOTable = m_pSEDTable;
		else
			pIOTable = m_pZoneTable;

		const KIOColumns*  pIOColumns = pIOTable->Columns();    
		int nColumnCount = pIOColumns->ColumnCount();
		KIOColumn*         pIOColumn  = nullptr;

		for ( int i=0; i<nColumnCount; i++ )
		{
			pIOColumn = pIOColumns->GetColumn(i);
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


void KThemeZoneValueClassify::InitExpressionChecker(std::vector<KIOColumn*> &a_vecColumnCategory)
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


void KThemeZoneValueClassify::LoadCategoryFieldRecord(std::vector<KIOColumn*> &a_vecColumnCategory)
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

		if (nullptr != m_pCategoryFieldRecord)
		{
			QBicDelete(m_pCategoryFieldRecord);
		}
		m_pCategoryFieldRecord = new KIDKeyDoubleRecords(nxCount);

		if (!m_bZoneTable)
		{
			KBulkDBase::BulkSEDVectorSelect(m_pSEDTable, oIOColumnsCategory, *m_pCategoryFieldRecord);
		}
		else
		{
			KBulkDBase::BulkZoneSelect(m_pZoneTable, oIOColumnsCategory, *m_pCategoryFieldRecord);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeZoneValueClassify::LoadInitialCategory(std::vector<KIOColumn*> &a_vecColumnCategory)
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
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
	{
		FieldSelectChangeAction();
	}
}

void KThemeZoneValueClassify::LoadInitialStyle()
{
	m_cboStyle.ResetContent();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{
		std::vector<TZoneTemplate> vecTemplate;
		KProjectDBase::ZoneTemplate(spDBaseConnection, 1, vecTemplate);

		int nPreSelectedIndex = -1;
		for (size_t i=0; i<vecTemplate.size(); i++)
		{
			TZoneTemplate oTemplate = vecTemplate[i];
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
	}

	if (m_cboStyle.GetCount() == 0)
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	}	

	StyleRadioSelectChangeAction();
}

void KThemeZoneValueClassify::OnBnClickedRadio()
{
	StyleRadioSelectChangeAction();
	LoadReportCtrlData();
}

void KThemeZoneValueClassify::StyleRadioSelectChangeAction()
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

	GetDlgItem(IDC_BUTTON_MARKER)->ShowWindow(SW_HIDE);
}

void KThemeZoneValueClassify::OnCbnSelchangeComboField()
{
	int nCurSelectedIndex = m_cboCategory.GetCurSel();
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

void KThemeZoneValueClassify::FieldSelectChangeAction()
{
	try
	{
		int nCurSelectedIndex = m_cboCategory.GetCurSel();
		if (nCurSelectedIndex < 0)
			return;

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = nullptr;
		if (m_bZoneTable)
		{
			pIOTable  = pIOTables->FindTable(TABLE_ZONE);
		}
		else
		{
			pIOTable  = pIOTables->FindTable(TABLE_SED_VECTOR);
		}
		KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		CString strSQL;
// 		strSQL.Format(_T("Select min(%s), max(%s) From %s Where %s is not null And %s < 9e999"), 
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

void KThemeZoneValueClassify::OnCbnSelchangeComboStyle()
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
	ClearZoneThemes();
}

void KThemeZoneValueClassify::StyleSelectChangeAction()
{
	LoadReportCtrlData();
}

void KThemeZoneValueClassify::OnCbnSelchangeComboClass()
{
	LoadReportCtrlData();
}

void KThemeZoneValueClassify::LoadInitialClass()
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

void KThemeZoneValueClassify::OnBnClickedCheck1()
{
	LoadReportCtrlData(false);
}

void KThemeZoneValueClassify::LoadInitialDigit()
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

void KThemeZoneValueClassify::OnCbnSelchangeComboDigit()
{
	LoadReportCtrlData(false);
}

void KThemeZoneValueClassify::OnBnClickedButtonMarker()
{
	CXTColorDialog dlg(m_clrSelectedLineColor, m_clrSelectedLineColor) ;
	if ( IDOK == dlg.DoModal() )
	{
		m_clrSelectedLineColor = dlg.GetColor();       
		GetDlgItem(IDC_BUTTON_MARKER)->RedrawWindow();
	}
}

void KThemeZoneValueClassify::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( 1 == nIndex )
	{		
		CXTColorDialog dlg(pItemNotify->pItem->GetTextColor(), pItemNotify->pItem->GetTextColor()) ;
		if ( IDOK == dlg.DoModal() )
		{
			pItemNotify->pItem->SetTextColor(dlg.GetColor());       
			m_ctrlReport.RedrawControl();
		}
	}

	if ( 2 == nIndex )
	{
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
		m_strPreValue   = pItemText->GetValue();
		m_strPreCaption = pItemText->GetCaption(NULL);
	}
}

void KThemeZoneValueClassify::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nRowIndex    = pItemNotify->pRow->GetIndex();
	int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

	int nRecordCount = m_ctrlReport.GetRecords()->GetCount();

	CXTPReportRecordItemText* pItemText;

	if ( 2 == nColumnIndex )
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
			m_ctrlReport.RedrawControl();
			return;			
		}

		if (nRowIndex == 0)
		{
// 			if (QBicStringChecker::IsNumeric(vecValue[0]) == false)
// 			{
// 				AfxMessageBox(_T("숫자를 입력해 주세요."));
// 				pItemText->SetValue(m_strPreValue);
// 				pItemText->SetCaption(m_strPreCaption);
// 				m_ctrlReport.RedrawControl();
// 				return;
// 			}

			if (QBicStringChecker::IsNumeric(strValue) == false)
			{
				AfxMessageBox(_T("숫자를 입력해 주세요."));
				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_ctrlReport.RedrawControl();
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
				m_ctrlReport.RedrawControl();
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
				m_ctrlReport.RedrawControl();
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
				m_ctrlReport.RedrawControl();
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

void KThemeZoneValueClassify::RecalRange()
{
	int nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;

	pRecords = m_ctrlReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CXTPReportRecordItemText* pItemText    = NULL;

	double dPreValue = -99999999999999.0;	

	for (int i=0; i<nRecordCount; i++)
	{
		pRecord   = pRecords->GetAt(i);
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);

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

	m_ctrlReport.RedrawControl();
}


void KThemeZoneValueClassify::Clear()
{
	ClearZoneThemes();
}


void KThemeZoneValueClassify::ClearZoneThemes()
{
	m_pMapView->ClearZoneThemes();
}


void KThemeZoneValueClassify::LoadReportCtrlData(bool a_bInit)
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
			LoadSavedStyle();
		}
		else
		{
			LoadUserStyle(a_bInit);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		m_ctrlReport.ResetContent();
		return;
	}
}

void KThemeZoneValueClassify::LoadSavedStyle()
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
	std::vector<TZoneTemplateValue> vecTemplateValue;
	KProjectDBase::ZoneTemplateValue(spDBaseConnection, nStyleID, vecTemplateValue);

	if (vecTemplateValue.size() < 1)
	{
		m_ctrlReport.ResetContent();
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

	BOOL bCheck(FALSE);
	if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
	{
		bCheck = TRUE;
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	m_ctrlReport.ResetContent();
		
	size_t nxCount = vecTemplateValue.size();
	for (size_t i=0; i<nxCount; i++)
	{
		TZoneTemplateValue& oTemplateValue = vecTemplateValue[i];

		pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

		//select
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bCheck);
		pItem->SetAlignment(xtpColumnIconCenter);

		// 심볼
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		CString strSymbol(_T("■"));

		pItem->SetCaption( strSymbol);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);
		pItem->SetTextColor(RGB(oTemplateValue.TR, oTemplateValue.TG, oTemplateValue.TB));
				
		// Value
		CString strCaption, strValue;
// 		strCaption.Format(_T("%.1f~%.1f"), oTemplateValue.TMin, oTemplateValue.TMax);
// 		strValue.Format(_T("%.1f"), oTemplateValue.TMax);
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

	m_ctrlReport.Populate();
}

void KThemeZoneValueClassify::LoadUserStyle(bool a_bInit)
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
		if (!m_bZoneTable)
		{
			KBulkDBaseDemarcation::AllSEDValue(m_pTarget, pIOColumn->Name(), vecData);
		}
		else
		{
			KBulkDBaseDemarcation::AllZoneValue(m_pTarget, pIOColumn->Name(), vecData);
		}
	}
	else
	{
		if (m_strFunction.IsEmpty())
		{
			m_ctrlReport.ResetContent();
			return;
		}
		GetDataByCalculate(vecData);
	}

	if (vecData.size() < 1)
	{
		m_ctrlReport.ResetContent();
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

	KMunsellColor oMunSellColor(5, false);
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();

	size_t nxCount = ovecDevideInfo.size();

	if (!a_bInit)
	{
		if (pRecords->GetCount() != (int)nxCount)
		{
			a_bInit = true;
		}
	}

	double dMinDecrease(0.0);
	double dMaxIncrease(0.0);
	int    nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);
	GetMinDecreaseMaxIncrease(nDisplayDecimalPoint, dMinDecrease, dMaxIncrease);

	if (a_bInit)
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		{
			bCheck = TRUE;
		}

		m_ctrlReport.ResetContent();

		for ( size_t i= 0; i< nxCount; i++ )
		{
			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

			//select
			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);

			// 심볼
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			CString strSymbol(_T("■"));
			pItem->SetCaption( strSymbol);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);
			pItem->SetTextColor(oMunSellColor.GetColor(i)/*m_clrSelectedLineColor*/);

			CString strValue(_T("")), strCaption(_T(""));
			SetClassMinMax(dMinDecrease, dMaxIncrease, nDisplayDecimalPoint, ovecDevideInfo, nxCount, i, strValue, strCaption);

			if (0 == i) //first
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));
				pItem->SetEditable(TRUE);
			} 
			else if (nxCount-1 == i) //last
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
				pItem->SetEditable(FALSE);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strValue));		
				pItem->SetEditable(TRUE);
			}	

			pItem->SetCaption(strCaption);
		}

		m_ctrlReport.Populate();
	}
	else
	{
		for ( size_t i= 0; i< nxCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			// Value
			CString strValue, strCaption;
			SetClassMinMax(dMinDecrease, dMaxIncrease, nDisplayDecimalPoint, ovecDevideInfo, nxCount, i, strValue, strCaption);

			CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);

			if (0 == i)
			{
				pItemText->SetValue(strValue);
			}
			else if (nxCount- 1 == i) //last
			{
				pItemText->SetValue(strCaption);
			}
			else
			{
				pItemText->SetValue(strValue);
			}

			pItemText->SetCaption(strCaption);
		}

		if (nxCount > 0)
		{
			m_ctrlReport.RedrawControl();
		}
	}
}


void KThemeZoneValueClassify::SetClassMinMax(double a_dMinDecrease, double a_dMaxIncrease, int a_nDisplayDecimalPoint, std::vector<TDevideDataInterval> &a_vecDevideInfo, size_t a_nxCount, size_t a_nIdx, CString &a_strValue, CString &a_strCaption)
{
	try
	{
		// Value
		if (0 == a_nIdx)//first
		{
			a_vecDevideInfo[a_nIdx].TMinValue = 0.0;
		}
		if (a_nxCount-1 == a_nIdx)//last
		{
			a_vecDevideInfo[a_nIdx].TMaxValue = 0.0;
		}

		CString strMax = QBicStringFormat::FormatString(a_vecDevideInfo[a_nIdx].TMaxValue, a_nDisplayDecimalPoint);
		CString strMin = QBicStringFormat::FormatString(a_vecDevideInfo[a_nIdx].TMinValue, a_nDisplayDecimalPoint);

		a_strValue.Format(_T("%s"), strMax);

		if (0 == a_nIdx)
		{
			a_strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);
		}
		else if (a_nxCount- 1 == a_nIdx) //last
		{
			a_strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);
		}
		else
		{
			a_strCaption.Format(_T("%s~%s"), strMin, strMax);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeZoneValueClassify::GetDataByCalculate(std::vector<double> &a_vecData)
{
	try
	{
		a_vecData.clear();

		AutoType iter  = m_mapCalculateResult.begin();
		AutoType itEnd = m_mapCalculateResult.end();

		while (iter != itEnd)
		{
			Integer nxZoneID = iter->first;
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


void KThemeZoneValueClassify::GetDrawDataByCalculate( std::vector<TZoneIDValue> &ar_vecData )
{
	try
	{
		ar_vecData.clear();

		AutoType iter  = m_mapCalculateResult.begin();
		AutoType itEnd = m_mapCalculateResult.end();

		TZoneIDValue oTZoneIDValue;

		while (iter != itEnd)
		{
			Integer nxZoneID = iter->first;
			double  dValue   = iter->second;

			oTZoneIDValue.TZoneID = nxZoneID;
			oTZoneIDValue.TValue  = dValue;

			ar_vecData.push_back(oTZoneIDValue);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ar_vecData.clear();
	}
}


void KThemeZoneValueClassify::GetMinDecreaseMaxIncrease(int a_nDecimalPoint, double &a_dMinDecrease, double &a_dMaxIncrease)
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


int KThemeZoneValueClassify::GetDisplayDecimalPoint(int a_nRealDigit)
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


void KThemeZoneValueClassify::Draw(bool a_bLabel)
{
	struct TMinMax
	{
		double   TMin;
		double   TMax;
		COLORREF TColorRGB;

		KEMThemeRangeIndex emRangeIndex;
	};

	if (!InvalidateInput())
	{
		ClearZoneThemes();
		return;
	}

	int nCurSelectedFieldIndex = m_cboCategory.GetCurSel();
	KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedFieldIndex);

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;

	pRecords = m_ctrlReport.GetRecords();
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
		pItemText  = (CXTPReportRecordItemText*  )pRecord->GetItem(2);

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
		oTMinMax.TColorRGB    = pItemStyle->GetTextColor();

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

	COLORREF clrOther   = m_btnOtherSymbolColorPicker.GetColor();

	std::vector<TZoneIDValue> vecData;

	if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
	{
		if (!m_bZoneTable)
		{
			KBulkDBaseDemarcation::AllSEDValue(m_pTarget, pIOColumn->Name(), vecData);
		}
		else
		{
			KBulkDBaseDemarcation::AllZoneValue(m_pTarget, pIOColumn->Name(), vecData);
		}
	}
	else
	{
		GetDrawDataByCalculate(vecData);
	}
	
	size_t nxZoneCount    = vecData.size();
	size_t nxTMinMaxCount = vecTMinMax.size();

	std::vector<TDrawZoneInfo> vecDrawZoneInfo;
	TDrawZoneInfo              oTDrawZoneInfo;

	BOOL bIncludeZero = IsDlgButtonChecked(IDC_CHECK_INC_ZERO);
	BOOL bCheckedOutRange(FALSE);
	if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
		bCheckedOutRange = TRUE;

	for (size_t i=0; i<nxZoneCount; i++)
	{
		TZoneIDValue& oTZoneIDValue = vecData[i];
		oTDrawZoneInfo.TZoneID      = oTZoneIDValue.TZoneID;

		double dValue = oTZoneIDValue.TValue;

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
						oTDrawZoneInfo.TColorRGB = oTMinMax.TColorRGB;

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
						oTDrawZoneInfo.TColorRGB = oTMinMax.TColorRGB;

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
						oTDrawZoneInfo.TColorRGB = oTMinMax.TColorRGB;

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

			oTDrawZoneInfo.TColorRGB = clrOther;
			oTDrawZoneInfo.TLabel    = _T("");
		}
		else
		{
			CString strLabel(_T(""));
			if (dValue > 0.009 || dValue == 0.0 || dValue < -0.009)
			{
				strLabel.Format(_T("%.2f"), dValue);
			}
			else
			{
				//strLabel.Format(_T("%.5f"), dValue);
				strLabel.Format(_T("%g"), dValue);
			}

			oTDrawZoneInfo.TLabel = strLabel;
		}

		vecDrawZoneInfo.push_back(oTDrawZoneInfo);
	}

	m_pMapView->DrawZoneThemes(vecDrawZoneInfo, a_bLabel);
}


bool KThemeZoneValueClassify::InvalidateInput()
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
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KThemeZoneValueClassify::OnBnClickedButtonStyleSave()
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

	pRecords = m_ctrlReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CString strSQL;
	bool bOverwrite = dlg.IsOverWrite();	

	try
	{
		spDBaseConnection->BeginTransaction();

		if (!bOverwrite)
		{
			CString strNewStyleName = dlg.GetNewStyleName();
			nID = KProjectDBase::NewZoneTemplateID(spDBaseConnection, 1);	
			strSQL.Format(_T("INSERT OR REPLACE INTO ZoneTemplate(ID,Type,Name) Values('%d','%d','%s')"), nID,1,strNewStyleName.Trim());
			spDBaseConnection->ExecuteUpdate(strSQL);
		}		

		for (int i=0; i<nRecordCount; i++)
		{
			CString      strRGB;
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(1);

			COLORREF oColor  = pItem->GetTextColor();
			strRGB.Format(_T("%d,%d,%d"), GetRValue(oColor), GetGValue(oColor), GetBValue(oColor));

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
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

			//strSQL.Format(_T("INSERT OR REPLACE INTO ZoneTemplateValue(ID,Seq,Min, Max, RGB) Values('%d','%d','%.1f','%.1f','%s')"), nID, i, dMin, dMax, strRGB);
			strSQL.Format(_T("INSERT OR REPLACE INTO ZoneTemplateValue(ID,Seq,Min, Max, RGB) Values('%d','%d','%s','%s','%s')"), nID, i, vecValue[0], vecValue[1], strRGB);

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


void KThemeZoneValueClassify::OnBnClickedButtonStyleEdit()
{
	KPolygonStyleValueEdit dlg;
	dlg.DoModal();

	LoadInitialStyle();
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		StyleSelectChangeAction();
	}	
}


void KThemeZoneValueClassify::OnBnClickedButtonFormula()
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


unsigned __stdcall KThemeZoneValueClassify::CalculateResultThreadCaller(void* p)
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("계산 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Calculating..."));
	}

	KThemeZoneValueClassify* pDlg = (KThemeZoneValueClassify*)pParameter->GetParameter();
	pDlg->CalculateResult();

	return 0;
}


bool KThemeZoneValueClassify::CalculateResult()
{
	try
	{
		m_mapCalculateResult.clear();
		m_dMinCalculateVal = 0.0;
		m_dMaxCalculateVal = 0.0;

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

		KIOTable* pIOTable = nullptr;
		if (!m_bZoneTable)
			pIOTable = m_pSEDTable;
		else
			pIOTable = m_pZoneTable;

		KParseExpression oParseExpression;
		oParseExpression.SetTable(pIOTable);
		oParseExpression.SetCaptionExpression(m_strFunction);

		std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();
		CString              strRealFunction   = oParseExpression.GetRealExpression();
		std::vector<int>     vecIdx; // vecUsedColumnName와 매칭하는 컬럼의 Index를 가지는 Vector;

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
			Integer         nxZoneID = iter->first;
			KSDoubleRecord* pRecord  = iter->second;

			for (int i= 0; i< nCntVecField; i++)
			{
				dValueArray[i] = pRecord->GetAt(vecIdx[i]);
			}

			double dResultVal = oParser.Eval();
			m_mapCalculateResult.insert(std::make_pair(nxZoneID, dResultVal));

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


void KThemeZoneValueClassify::SetEditControlMinMax( double a_dMinVal, double a_dMaxVal )
{
	CString strTemp(_T(""));

	strTemp.Format(_T("%.1f"), a_dMinVal);
	SetDlgItemText(IDC_STATIC_LOW_VALUE,  strTemp);

	strTemp.Format(_T("%.1f"), a_dMaxVal);
	SetDlgItemText(IDC_STATIC_HIGH_VALUE, strTemp);
}


void KThemeZoneValueClassify::OnBnClickedRadioDrawData( UINT nID )
{
	ControlRadioDrawData();
	LoadReportCtrlData(false);
}


void KThemeZoneValueClassify::ControlRadioDrawData()
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

void KThemeZoneValueClassify::ChangeTable( bool a_bZoneTable )
{
	m_bZoneTable = a_bZoneTable;

	m_strFunction = _T("");
	m_mapCalculateResult.clear();

	if (nullptr != m_pCategoryFieldRecord)
	{
		QBicDelete(m_pCategoryFieldRecord);
	}

	m_dMinCalculateVal = 0.0;
	m_dMaxCalculateVal = 0.0;
	SetDlgItemText(IDC_STATIC_LOW_VALUE,  _T("0.0"));
	SetDlgItemText(IDC_STATIC_HIGH_VALUE, _T("0.0"));

	GetCategoryField       (m_vecColumnCategory);
	LoadInitialCategory    (m_vecColumnCategory);
	InitExpressionChecker  (m_vecColumnCategory);

	LoadReportCtrlData(false);
}


void KThemeZoneValueClassify::OnBnClickedCheckAll()
{
	ControlCheckAll();
}


void KThemeZoneValueClassify::ControlCheckAll()
{
	try
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*           pRecord    = nullptr;
		CXTPReportRecordItem*       pItem      = nullptr;

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem              = pRecord->GetItem(0);
			pItem->SetChecked(bCheck);
		}

		m_ctrlReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeZoneValueClassify::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KThemeZoneValueClassify::OnBnClickedCheckAllOtherValues()
{
	ControlCheckOutRange();
}


void KThemeZoneValueClassify::ControlCheckOutRange()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
		{
			m_btnOtherSymbolColorPicker.ShowWindow(SW_SHOW);
		}
		else
		{
			m_btnOtherSymbolColorPicker.ShowWindow(SW_HIDE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
// ThemeNodeValueScale.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeNodeValueScale.h"
#include "afxdialogex.h"

#include "Target.h"
#include "MapView.h"

#include "KPointMarkerSelectDlg.h"

//#include "CodeManager.h"
//#include "CodeGroup.h"

#include "ThemeFunctionDlg.h"
#include "ExpressionChecker.h"
#include "ParseExpression.h"
#include "KBulkDBase.h"
#include "BulkDBaseNode.h"

// KThemeNodeValueScale 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeNodeValueScale, KDialogEx)

KThemeNodeValueScale::KThemeNodeValueScale(CWnd* pParent /*=NULL*/)
	: KDialogEx(KThemeNodeValueScale::IDD, pParent)
	, m_bCheckedFilter(FALSE)
{
	m_oQbicDefaultMarkerFont.CreatePointFont(120, _T("QBicF"));
	m_nSelectedMarkerIndex   = 48;
	m_clrSelectedMarkerColor = RGB(51, 102, 255);

	m_nOtherMarkerIndex      = 53;
	m_clrOtherMarkerColor    = RGB(234, 234, 234);

	m_strPreSelectedColumnName = _T("");

	m_strFunction = _T("");
	m_pExpressionChecker   = new KExpressionChecker;
	m_pCategoryFieldRecord = nullptr;

	m_dMinCalculateVal = 0.0;
	m_dMaxCalculateVal = 0.0;
}

KThemeNodeValueScale::~KThemeNodeValueScale()
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

void KThemeNodeValueScale::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FIELD,   m_cboCategory);
	DDX_Control(pDX, IDC_BUTTON_MARKER, m_btnMarker);
	DDX_Control(pDX, IDC_BUTTON_MARKER_OTHER, m_btnMarkerOther);
}


BEGIN_MESSAGE_MAP(KThemeNodeValueScale, KDialogEx)
	ON_WM_CTLCOLOR()	
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_MARKER, &KThemeNodeValueScale::OnBnClickedButtonMarker)	
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD, &KThemeNodeValueScale::OnCbnSelchangeComboField)	

	ON_BN_CLICKED(IDC_CHECK1, &KThemeNodeValueScale::OnBnClickedCheckAllOtherValues)
	ON_BN_CLICKED(IDC_BUTTON_MARKER_OTHER, &KThemeNodeValueScale::OnBnClickedButtonMarkerOther)
	ON_BN_CLICKED(IDC_BTN_FILTER1,   &KThemeNodeValueScale::OnBnClickedBtnFilter)
	ON_BN_CLICKED(IDC_CHECK_FILTER, &KThemeNodeValueScale::OnBnClickedFilter)

	ON_BN_CLICKED(IDC_BUTTON1,       &KThemeNodeValueScale::OnBnClickedButtonFormula)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KThemeNodeValueScale::OnBnClickedRadioDrawData)
END_MESSAGE_MAP()


// KThemeNodeValueScale 메시지 처리기입니다.

void KThemeNodeValueScale::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        CString strSubjectName(a_strSubjectName);

        if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
        {
			m_strFunction = _T("");
			m_mapCalculateResult.clear();
			m_dMinCalculateVal = 0.0;
			m_dMaxCalculateVal = 0.0;

			if (nullptr != m_pCategoryFieldRecord)
			{
				QBicDelete(m_pCategoryFieldRecord);
			}

			m_mapNodeIdType.clear();

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


BOOL KThemeNodeValueScale::OnInitDialog()
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

	SetDlgItemText(IDC_EDIT_LOW_SIZE,  _T("5.0"));
	SetDlgItemText(IDC_EDIT_HIGH_SIZE, _T("50.0"));
	SetDlgItemText(IDC_EDIT_OUT_SIZE,  _T("5.0"));

	CString strSymbol;
	strSymbol.AppendChar(m_nSelectedMarkerIndex);  
	SetDlgItemText(IDC_BUTTON_MARKER, strSymbol);

	strSymbol = _T("");
	strSymbol.AppendChar(m_nOtherMarkerIndex);  
	SetDlgItemText(IDC_BUTTON_MARKER_OTHER, strSymbol);

	m_pNodeTable->Register(this);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	ControlRadioDrawData();

	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	ControlCheckOutRange();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH KThemeNodeValueScale::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void KThemeNodeValueScale::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KThemeNodeValueScale::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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

void KThemeNodeValueScale::SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView)
{
	m_pTarget       = a_pTarget;
	m_pMapView      = a_pMapView;

	CWaitCursor cw;

	InitFitlerData();
	
	GetCategoryField       (m_vecColumnCategory);
	LoadInitialCategory    (m_vecColumnCategory);
	InitExpressionChecker  (m_vecColumnCategory);
}

void KThemeNodeValueScale::SetNodeTable(KIOTable* a_pNodeTable)
{
	m_pNodeTable    = a_pNodeTable;
}

void KThemeNodeValueScale::OnBnClickedButtonMarker()
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
		}
	}
}


void KThemeNodeValueScale::GetCategoryField(std::vector<KIOColumn*> &a_vecColumnCategory)
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


void KThemeNodeValueScale::LoadCategoryFieldRecord(std::vector<KIOColumn*> &a_vecColumnCategory)
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


void KThemeNodeValueScale::LoadInitialCategory(std::vector<KIOColumn*> &a_vecColumnCategory)
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

	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		FieldSelectChangeAction();
	}
}


void KThemeNodeValueScale::InitExpressionChecker(std::vector<KIOColumn*> &a_vecColumnCategory)
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


void KThemeNodeValueScale::OnCbnSelchangeComboField()
{
	try
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
		FieldSelectChangeAction();
		ClearNodeThemes();
		
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KThemeNodeValueScale::FieldSelectChangeAction()
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
//     strSQL.Format(_T("Select min(%s), max(%s) From %s Where %s is not null And %s < 9e999"),	//9e999 -> Infinity(무한대)를 제외함
// 		pIOColumn->Name(), pIOColumn->Name(), pIOTable->Name(), pIOColumn->Name(), pIOColumn->Name());

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


void KThemeNodeValueScale::SetEditControlMinMax( double a_dMinVal, double a_dMaxVal )
{
	CString strTemp(_T(""));

	strTemp.Format(_T("%.1f"), a_dMinVal);// '%.0f' 은 버림이 아닌 반올림을 발생 시킨다. 
	SetDlgItemText(IDC_EDIT_LOW_VALUE,  strTemp);

	strTemp.Format(_T("%.1f"), a_dMaxVal);
	SetDlgItemText(IDC_EDIT_HIGH_VALUE, strTemp);
}


void KThemeNodeValueScale::Clear()
{
	ClearNodeThemes();
}


void KThemeNodeValueScale::ClearNodeThemes()
{
	m_pMapView->ClearNodeThemes();
}


void KThemeNodeValueScale::Draw(bool a_bLabel)
{
	/*ClearNodeThemes();*/

	if (!InvalidateInput())
	{
		return;
	}

	TThemeResultSettingInfo oTThemeResultSettingInfo;
	GetThemeResultSettingInfo(oTThemeResultSettingInfo);

	std::vector<TDrawNodeInfo> vecDrawNodeInfo;
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		GetDrawDataByNormal(oTThemeResultSettingInfo, vecDrawNodeInfo);
	}
	else
	{
		GetDrawDataByCalculate(oTThemeResultSettingInfo, vecDrawNodeInfo);
	}

	m_pMapView->DrawNodeThemes(vecDrawNodeInfo, a_bLabel);
}


bool KThemeNodeValueScale::InvalidateInput()
{
	try
	{
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			int nCurSelectedIndex = m_cboCategory.GetCurSel();
			if ( nCurSelectedIndex < 0 )
			{
				AfxMessageBox(_T("컬럼을 선택해 주세요."));
				m_cboCategory.SetFocus();
				return false;
			}    

			KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);
			if (pIOColumn == NULL)
			{
				AfxMessageBox(_T("관련 컬럼 정보를 찾을 수 없습니다."));
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

		CString strTemp;

		GetDlgItemText(IDC_EDIT_LOW_VALUE, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_LOW_VALUE)->SetFocus();
			return false;
		}

		double dLowValue = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_LOW_SIZE, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_LOW_SIZE)->SetFocus();
			return false;
		}

		double dLowSize = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_HIGH_VALUE, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_HIGH_VALUE)->SetFocus();
			return false;
		}

		double dHighValue = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_HIGH_SIZE, strTemp);
		if (!QBicStringChecker::IsNumeric(strTemp))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_HIGH_SIZE)->SetFocus();
			return false;
		}

		double dHighSize = _ttof(strTemp);

		// 	if (dLowValue < 0.0)
		// 	{
		// 		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		// 		GetDlgItem(IDC_EDIT_LOW_VALUE)->SetFocus();
		// 		return false;
		// 	}
		// 
		// 	if (dHighValue < 0.0)
		// 	{
		// 		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		// 		GetDlgItem(IDC_EDIT_HIGH_VALUE)->SetFocus();
		// 		return false;
		// 	}

		if (dLowSize < 0.0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_LOW_SIZE)->SetFocus();
			return false;
		}

		if (dHighSize < 0.0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_HIGH_SIZE)->SetFocus();
			return false;
		}

		if (dHighValue <= dLowValue)
		{
			AfxMessageBox(_T("작은 값을 입력해 주세요"));
			GetDlgItem(IDC_EDIT_LOW_VALUE)->SetFocus();
			return false;
		}

// 		if (dHighSize < dLowSize)
// 		{
// 			AfxMessageBox(_T("작은 값을 입력해 주세요"));
// 			GetDlgItem(IDC_EDIT_LOW_SIZE)->SetFocus();
// 			return false;
// 		}

		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
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


void KThemeNodeValueScale::GetDrawDataByNormal(TThemeResultSettingInfo &a_oTThemeResultSettingInfo, std::vector<TDrawNodeInfo> &a_vecOutDrawInfo)
{
	try
	{
		int nCurSelectedIndex = m_cboCategory.GetCurSel();
		KIOColumn* pIOColumn = (KIOColumn*)m_cboCategory.GetItemData(nCurSelectedIndex);

		BOOL bChecked     = IsDlgButtonChecked(IDC_CHECK1); //out range
		BOOL bIncludeZero = IsDlgButtonChecked(IDC_CHECK_INC_ZERO);

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strQueryFilter = GenQueryFilter();
		CString strSQL;
		strSQL.Format(_T("Select node_id, %s From %s  %s Order By %s Desc"), 
			pIOColumn->Name(), TABLE_NODE, strQueryFilter, pIOColumn->Name());

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			AfxMessageBox(_T("검색 결과가 없습니다."));
			return;
		}

		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		TDrawNodeInfo    oTDrawNodeInfo;

		while(pResult->Next())
		{
			Integer nxNodeID = pResult->GetValueInt64(0);
			double  dValue   = pResult->GetValueDouble(1);
			double  dSize    = 0.0;

			bool    bOutRange(false);
			CString strLabel(_T(""));

			if (!Calculate2Width(bChecked, bIncludeZero, a_oTThemeResultSettingInfo, dValue, bOutRange, dSize, strLabel))
			{
				continue;
			}

			oTDrawNodeInfo.TNodeID         = nxNodeID;
			
			if (!bOutRange)
			{
				oTDrawNodeInfo.TSize           = dSize;	
				oTDrawNodeInfo.TSymbol = m_nSelectedMarkerIndex;
				oTDrawNodeInfo.TColorRGB       = m_clrSelectedMarkerColor;
				oTDrawNodeInfo.TLabel          = strLabel;

				vecDrawNodeInfo.push_back(oTDrawNodeInfo);
			}
			else
			{
				oTDrawNodeInfo.TSize           = a_oTThemeResultSettingInfo.dOutSize;	
				oTDrawNodeInfo.TSymbol = m_nOtherMarkerIndex;
				oTDrawNodeInfo.TColorRGB       = m_clrOtherMarkerColor;
				oTDrawNodeInfo.TLabel          = _T("");

				a_vecOutDrawInfo.push_back(oTDrawNodeInfo);
			}
		}

		size_t nxCntFilter = m_vecFilterInfo.size();
		if (m_bCheckedFilter && nxCntFilter != 0)
		{      
			if (bChecked)
			{
				//-jyk-14.04.16- 명지대요청: 선택하지 않은 타입은 Outrange가 아닌 그리지 않는것으로 변경
// 				CString strQueryFilter = GenNotQueryFilter();    
// 				CString strSQL;
// 				strSQL.Format(_T("Select node_id, %s From %s  %s Order By %s Desc"), 
// 					pIOColumn->Name(), TABLE_NODE, strQueryFilter, pIOColumn->Name());
// 
// 				KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
// 				while(spResult->Next())
// 				{
// 					TDrawNodeInfo    oTDrawNodeInfo;
// 					oTDrawNodeInfo.TNodeID         = spResult->GetValueInt64(0);
// 					oTDrawNodeInfo.TSize           = a_oTThemeResultSettingInfo.dOutSize;
// 					oTDrawNodeInfo.TCharacterIndex = m_nOtherMarkerIndex;
// 					oTDrawNodeInfo.TColorRGB       = m_clrOtherMarkerColor;
// 					oTDrawNodeInfo.TLabel          = _T("");
// 
// 					a_vecOutDrawInfo.push_back(oTDrawNodeInfo);
// 				}
			}
		}

		std::sort(vecDrawNodeInfo.begin(), vecDrawNodeInfo.end(), Descending);
		size_t nxCount = vecDrawNodeInfo.size(); 
		for (size_t i= 0; i< nxCount; i++)
		{
			a_vecOutDrawInfo.push_back(vecDrawNodeInfo[i]);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueScale::GetDrawDataByCalculate(TThemeResultSettingInfo &a_oTThemeResultSettingInfo, std::vector<TDrawNodeInfo> &a_vecOutDrawInfo)
{
	try
	{
		std::set<int> setFilterCheckType;
		GenFilter(setFilterCheckType);

		BOOL bChecked     = IsDlgButtonChecked(IDC_CHECK1); 
		BOOL bIncludeZero = IsDlgButtonChecked(IDC_CHECK_INC_ZERO);

		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		TDrawNodeInfo    oTDrawNodeInfo;

		AutoType iter  = m_mapCalculateResult.begin();
		AutoType itEnd = m_mapCalculateResult.end();

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;
			double  dValue   = iter->second;
			double  dSize(0.0);

			bool bNoneData(false);
			if (m_bCheckedFilter)
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

			if (bNoneData)
			{
				if (bChecked)
				{
					//-jyk-14.04.16- 명지대요청: 선택하지 않은 타입은 Outrange가 아닌 그리지 않는것으로 변경
// 					oTDrawNodeInfo.TNodeID         = nxNodeID;
// 					oTDrawNodeInfo.TSize           = a_oTThemeResultSettingInfo.dOutSize;	
// 					oTDrawNodeInfo.TCharacterIndex = m_nOtherMarkerIndex;
// 					oTDrawNodeInfo.TColorRGB       = m_clrOtherMarkerColor;
// 					oTDrawNodeInfo.TLabel          = _T("");
// 
// 					a_vecOutDrawInfo.push_back(oTDrawNodeInfo);
				}
			}
			else
			{
				bool    bOutRange(false);
				CString strLabel(_T(""));
				if (!Calculate2Width(bChecked, bIncludeZero, a_oTThemeResultSettingInfo, dValue, bOutRange, dSize, strLabel))
				{
					++iter;
					continue;
				}

				oTDrawNodeInfo.TNodeID         = nxNodeID;

				if (!bOutRange)
				{
					oTDrawNodeInfo.TSize           = dSize;	
					oTDrawNodeInfo.TSymbol = m_nSelectedMarkerIndex;
					oTDrawNodeInfo.TColorRGB       = m_clrSelectedMarkerColor;
					oTDrawNodeInfo.TLabel          = strLabel;
					
					vecDrawNodeInfo.push_back(oTDrawNodeInfo);
				}
				else
				{
					oTDrawNodeInfo.TSize           = a_oTThemeResultSettingInfo.dOutSize;	
					oTDrawNodeInfo.TSymbol = m_nOtherMarkerIndex;
					oTDrawNodeInfo.TColorRGB       = m_clrOtherMarkerColor;
					oTDrawNodeInfo.TLabel          = _T("");

					a_vecOutDrawInfo.push_back(oTDrawNodeInfo);
				}
			}

			++iter;
		}

		std::sort(vecDrawNodeInfo.begin(), vecDrawNodeInfo.end(), Descending);

		size_t nxCount = vecDrawNodeInfo.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			a_vecOutDrawInfo.push_back(vecDrawNodeInfo[i]);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KThemeNodeValueScale::GetThemeResultSettingInfo(TThemeResultSettingInfo &a_oTThemeResultSettingInfo)
{
	try
	{
		CString strTemp(_T(""));

		GetDlgItemText(IDC_EDIT_LOW_VALUE, strTemp);
		a_oTThemeResultSettingInfo.dLowValue = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_LOW_SIZE, strTemp);
		a_oTThemeResultSettingInfo.dLowSize = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_HIGH_VALUE, strTemp);
		a_oTThemeResultSettingInfo.dHighValue = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_HIGH_SIZE, strTemp);
		a_oTThemeResultSettingInfo.dHighSize = _ttof(strTemp);

		GetDlgItemText(IDC_EDIT_OUT_SIZE, strTemp);
		a_oTThemeResultSettingInfo.dOutSize = _ttof(strTemp);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KThemeNodeValueScale::Calculate2Width(BOOL a_bChecked, BOOL a_bIncludeZero, TThemeResultSettingInfo &a_oTThemeResultSettingInfo, double a_dValue, bool &a_bOutRange, double &a_dSize, CString &a_strLabel)
{
	try
	{
		double &dLowValue  = a_oTThemeResultSettingInfo.dLowValue;
		double &dHighValue = a_oTThemeResultSettingInfo.dHighValue;
		double &dLowSize   = a_oTThemeResultSettingInfo.dLowSize;
		double &dHighSize  = a_oTThemeResultSettingInfo.dHighSize;

		if (!a_bIncludeZero && a_dValue == 0.0)
		{
			//a_bOutRange = true;
			return false; // outrange 판단이 아닌 그리지 않겠다로 변경
		} 
		else if (dLowValue <= a_dValue && a_dValue <= dHighValue)
		{
			if (dLowValue == dHighValue)
			{
				a_dSize = dLowSize;
			}
			else
			{
				a_dSize = (dHighSize-dLowSize) / (dHighValue-dLowValue) * (a_dValue-dLowValue) + dLowSize;			
			}
		}
		else
		{
			a_bOutRange = true;
		}

		if (true == a_bOutRange)
		{
			if (a_bChecked == TRUE)
			{
				a_dSize = a_oTThemeResultSettingInfo.dOutSize;
			}
			else
			{
				return false;
			}
		}

		if (0.0 == a_dSize)
		{
			a_strLabel = _T("");
			a_dSize = 0.1; //jyk (m_pMapView->DrawNodeThemes 에서 size 0으로 설정 하고 Draw 하면 제법 큰 원이 그려진다. 0.1로 하여 보이지 않게 한다)
		}
		else
		{
			if (a_dValue > 0.009 || a_dValue == 0.0 || a_dValue < -0.009)
			{
				a_strLabel.Format(_T("%.2f"), a_dValue);
			}
			else
			{
				//a_strLabel.Format(_T("%.5f"), a_dValue);
				a_strLabel.Format(_T("%g"), a_dValue);
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


bool KThemeNodeValueScale::Descending( TDrawNodeInfo left, TDrawNodeInfo right )
{
	return left.TSize > right.TSize;
}


void KThemeNodeValueScale::OnBnClickedCheckAllOtherValues()
{
	ControlCheckOutRange();
}


void KThemeNodeValueScale::ControlCheckOutRange()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
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


void KThemeNodeValueScale::OnBnClickedButtonMarkerOther()
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

void KThemeNodeValueScale::InitFitlerData( void )
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

void KThemeNodeValueScale::OnBnClickedBtnFilter()
{
	KThemeSetFilterDlg oThemeSetFilterDlg(m_pTarget, m_vecFilterInfo, USER_NODECODE_GROUP_KEY, SYSTEM_NODECODE_GROUP_KEY);

	if (oThemeSetFilterDlg.DoModal() == IDOK)
	{
		UpdateTxtFilter();
	}
}

void KThemeNodeValueScale::OnBnClickedFilter()
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


void KThemeNodeValueScale::GenFilter(std::set<int> &a_setCheckType)
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
	}
}


CString KThemeNodeValueScale::GenQueryFilter( void )
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


CString KThemeNodeValueScale::GenNotQueryFilter( void )
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

		strQueryFilter.Format(_T(" where %s not in ( "), COLUMN_NODE_TYPE);
		strQueryFilter.Append(strQuery2);
		strQueryFilter.Append(_T(" ) "));
	}

	return strQueryFilter;
}


void KThemeNodeValueScale::UpdateTxtFilter( void )
{
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


BOOL KThemeNodeValueScale::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_LBUTTONDOWN &&
		pMsg->hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
	{
		OnBnClickedBtnFilter();
		return TRUE;
	}

	return KDialogEx::PreTranslateMessage(pMsg);
}


void KThemeNodeValueScale::OnBnClickedButtonFormula()
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
				SetEditControlMinMax(m_dMinCalculateVal, m_dMaxCalculateVal);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KThemeNodeValueScale::CalculateResultThreadCaller(void* p)
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("계산 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Calculating..."));
	}

	KThemeNodeValueScale* pDlg = (KThemeNodeValueScale*)pParameter->GetParameter();
	pDlg->CalculateResult();

	return 0;
}


bool KThemeNodeValueScale::CalculateResult()
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
			Integer         nxNodeID = iter->first;
			KSDoubleRecord* pRecord  = iter->second;

			for (int i= 0; i< nCntVecField; i++)
			{
				dValueArray[i] = pRecord->GetAt(vecIdx[i]);
			}

			double dResultVal = oParser.Eval();
			m_mapCalculateResult.insert(std::make_pair(nxNodeID, dResultVal));

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


void KThemeNodeValueScale::OnBnClickedRadioDrawData( UINT nID )
{
	ControlRadioDrawData();
}


void KThemeNodeValueScale::ControlRadioDrawData()
{
	try
	{
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
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

			ClearNodeThemes();  
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

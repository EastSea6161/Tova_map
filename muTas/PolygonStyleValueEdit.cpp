// PolygonStyleValueEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PolygonStyleValueEdit.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "DefineNames.h"

//#include "StringChecker.h"
//^#include "MunsellColorCircle.h"
// KPolygonStyleValueEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPolygonStyleValueEdit, KDialogEx)

KPolygonStyleValueEdit::KPolygonStyleValueEdit(CWnd* pParent /*=NULL*/)
	: KDialogEx(KPolygonStyleValueEdit::IDD, pParent)
{
	m_nPreSelectedStyleID      = -1;
	m_nRealDigit = 0;
}

KPolygonStyleValueEdit::~KPolygonStyleValueEdit()
{
}

void KPolygonStyleValueEdit::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_cboStyle);
}


BEGIN_MESSAGE_MAP(KPolygonStyleValueEdit, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &KPolygonStyleValueEdit::OnCbnSelchangeComboStyle)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &KPolygonStyleValueEdit::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KPolygonStyleValueEdit::OnReportItemValueChanged)
	ON_EN_SETFOCUS(IDC_EDIT_STYLE_NAME, &KPolygonStyleValueEdit::OnEnSetfocusEditStyleName)
	ON_CBN_SETFOCUS(IDC_COMBO_STYLE, &KPolygonStyleValueEdit::OnCbnSetfocusComboStyle)
	ON_BN_CLICKED(IDC_BUTTON_CONTENTS_ADD, &KPolygonStyleValueEdit::OnBnClickedButtonContentsAdd)
	ON_BN_CLICKED(IDC_BUTTON_CONTENTS_DELETE, &KPolygonStyleValueEdit::OnBnClickedButtonContentsDelete)
	ON_BN_CLICKED(IDOK, &KPolygonStyleValueEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_STYLE_DELETE, &KPolygonStyleValueEdit::OnBnClickedButtonStyleDelete)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KPolygonStyleValueEdit::OnBnClickedRadioEditMethod)
END_MESSAGE_MAP()

BOOL KPolygonStyleValueEdit::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, FALSE);

	CXTPReportColumn* pColumn = NULL;	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Symbol"), 10));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("<=  Range  <"),   30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("범위"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Delete"), 8));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	LoadInitialStyle();
	StyleSelectChangeAction();

	UpdateUIState();

	m_wndReportCtrl.Populate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPolygonStyleValueEdit::UpdateUIState()
{
	CButton* pButton = NULL;

	if (m_cboStyle.GetCount() > 0)
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
		pButton->SetCheck(BST_CHECKED);
		pButton->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STYLE_DELETE)->EnableWindow(TRUE);

		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(BST_UNCHECKED);
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
		pButton->SetCheck(BST_UNCHECKED);

		pButton->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STYLE_DELETE)->EnableWindow(FALSE);

		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(BST_CHECKED);
	}
}

void KPolygonStyleValueEdit::OnCbnSelchangeComboStyle()
{
	StyleSelectChangeAction();
}

void KPolygonStyleValueEdit::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if ( 0 == nIndex )
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetTextColor(), pItemNotify->pItem->GetTextColor()) ;
		if ( IDOK == dlg.DoModal() )
		{
			pItemNotify->pItem->SetTextColor(dlg.GetColor());       
			m_wndReportCtrl.RedrawControl();
		}
	}

	if ( 1 == nIndex )
	{
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;  
		m_strPreValue   = pItemText->GetValue();
		m_strPreCaption = pItemText->GetCaption(NULL);
	}
}

void KPolygonStyleValueEdit::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nRowIndex    = pItemNotify->pRow->GetIndex();
	int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

	int nRecordCount = m_wndReportCtrl.GetRecords()->GetCount();

	CXTPReportRecordItemText* pItemText;

	if ( 1 == nColumnIndex )
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
		else if (nRowIndex == (nRecordCount-1)) //last
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
				//strCaption.Format(_T("%.1f-%.1f"), dMin, dMax);
				strCaption.Format(_T("%s~%s"), strMin, strMax);
				pItemText->SetCaption(strCaption);
			}	
		}
	}

	RecalRange();
}

int KPolygonStyleValueEdit::GetDisplayDecimalPoint(int a_nRealDigit)
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

void KPolygonStyleValueEdit::RecalRange()
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
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);

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
// 		strCaption.Format(_T("%.1f-%.1f"), dMinValue, dMaxValue);

		strValue.Format(_T("%s"), strMax);		

		if (i == 0)
		{
			strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);

			pItemText->SetValue(strValue);
			pItemText->SetEditable(TRUE);
		}
		else if (nRecordCount- 1 == i) //last
		{
			strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);

			pItemText->SetValue(strCaption);
			pItemText->SetEditable(FALSE);
		}
		else
		{
			strCaption.Format(_T("%s~%s"), strMin, strMax);

			pItemText->SetValue(strValue);
			pItemText->SetEditable(TRUE);
		}
		
		pItemText->SetCaption(strCaption);		
		dPreValue = dMaxValue;
	}

	m_wndReportCtrl.RedrawControl();
}

void KPolygonStyleValueEdit::LoadInitialStyle()
{
	m_nRealDigit = 0;
	m_cboStyle.ResetContent();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{
		std::vector<TZoneTemplate> vecLinkTemplate;
		KProjectDBase::ZoneTemplate(spDBaseConnection, 1, vecLinkTemplate);

		int nPreSelectedIndex = -1;
		for (size_t i=0; i<vecLinkTemplate.size(); i++)
		{
			TZoneTemplate oTemplate = vecLinkTemplate[i];
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
}

void KPolygonStyleValueEdit::StyleSelectChangeAction()
{
	int nCurSelectedIndex = -99;
	int nStyleID          = -99;

	if ( m_cboStyle.GetCount() != 0)
	{
		nCurSelectedIndex = m_cboStyle.GetCurSel();
		nStyleID          = (int)m_cboStyle.GetItemData(nCurSelectedIndex);
	}

	std::vector<TZoneTemplateValue> vecTemplateValue;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection != NULL)
	{		
		KProjectDBase::ZoneTemplateValue(spDBaseConnection, nStyleID, vecTemplateValue);
	}

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

	m_wndReportCtrl.ResetContent();

	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	CXTPReportRecordItemNumber* pItemSize = NULL;
	CXTPReportRecordItemText*   pItemText = NULL;

	int nTemplateCount = (int)vecTemplateValue.size();
	for (int i=0; i<nTemplateCount; i++)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetAlignment(DT_CENTER);
		pItem->SetEditable(FALSE);
		CString strSymbol(_T("■"));
		pItem->SetTextColor(RGB(vecTemplateValue[i].TR, vecTemplateValue[i].TG, vecTemplateValue[i].TB));
		pItem->SetCaption(strSymbol);

		pItemText = (CXTPReportRecordItemText*)pRecord->AddItem(new CXTPReportRecordItemText());

		CString strValue;
		//strValue.Format(_T("%.1f"), vecTemplateValue[i].TMax);
		strValue.Format(_T("%s"), vecTemplateValue[i].TMax);
		
		CString strCaption;
		//strCaption.Format(_T("%.1f-%.1f"), vecTemplateValue[i].TMin, vecTemplateValue[i].TMax);
		
		if (0 == i)
		{
			strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW,         vecTemplateValue[i].TMax);
			pItemText->SetValue(strValue);
			pItemText->SetEditable(TRUE);
		}
		else if (nTemplateCount-1 == i)
		{
			strCaption.Format(_T("%s~%s"), vecTemplateValue[i].TMin, THEME_TEXT_ABOVE);
			pItemText->SetValue(strCaption);
			pItemText->SetEditable(FALSE);
		}
		else
		{
			strCaption.Format(_T("%s~%s"), vecTemplateValue[i].TMin, vecTemplateValue[i].TMax);
			pItemText->SetValue(strValue);
			pItemText->SetEditable(TRUE);
		}
		
		pItemText->SetCaption(strCaption);

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->SetEditable(TRUE);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
	}

	m_wndReportCtrl.Populate();
}

void KPolygonStyleValueEdit::OnEnSetfocusEditStyleName()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_UNCHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_CHECKED);
}

void KPolygonStyleValueEdit::OnCbnSetfocusComboStyle()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_CHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_UNCHECKED);

	StyleSelectChangeAction();
}

void KPolygonStyleValueEdit::OnBnClickedButtonContentsAdd()
{
	KMunsellColor oMunSellColor(5, false);
	COLORREF clrColor = oMunSellColor.GetColor(m_wndReportCtrl.GetRecords()->GetCount());

	double   dMin;
	double   dMax;
	PreStyle(dMin, dMax);

	int    nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);

	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecordItemNumber* pItemSize = NULL;
	CXTPReportRecordItemText*   pItemText = NULL;

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pItem = pRecord->AddItem(new CXTPReportRecordItemText);
	pItem->SetAlignment(DT_CENTER);
	pItem->SetEditable(FALSE);

	CString strSymbol(_T("■"));
	pItem->SetTextColor(clrColor);
	pItem->SetCaption(strSymbol);
	
	pItemText = (CXTPReportRecordItemText*)pRecord->AddItem(new CXTPReportRecordItemText());
	CString strValue, strCaption;
	
	CString strMax = QBicStringFormat::FormatString(dMax, nDisplayDecimalPoint);
	CString strMin = QBicStringFormat::FormatString(dMin, nDisplayDecimalPoint);

	strValue.Format(_T("%s"), strMax);

	if (pRecord->GetIndex() == 0)
	{
		strCaption.Format(_T("%s~%s"), THEME_TEXT_BELOW, strMax);

		pItemText->SetValue(strValue);
		pItemText->SetEditable(TRUE);
	}
	else // 항상 last
	{
		strCaption.Format(_T("%s~%s"), strMin, THEME_TEXT_ABOVE);

		pItemText->SetValue(strCaption);
		pItemText->SetEditable(FALSE);
	}

	pItemText->SetCaption(strCaption);
	
	pItem = pRecord->AddItem(new CXTPReportRecordItemText());
	pItem->SetEditable(TRUE);
	pItem->HasCheckbox(TRUE);
	pItem->SetChecked(FALSE);
	pItem->SetAlignment(xtpColumnIconCenter);

	m_wndReportCtrl.Populate();

	if (m_wndReportCtrl.GetRecords()->GetCount() == 1) // Record가 0일때는 연속으로 두개를 생성 해야 한다. (below, above)
	{
		OnBnClickedButtonContentsAdd();
	}
}

void KPolygonStyleValueEdit::OnBnClickedButtonContentsDelete()
{
	CXTPReportRecords*	  pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecordItemNumber* pItemSize = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();
	int nDeleteCount(0);

	for (int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);
		pItem      = pRecord->GetItem(2);

		if (pItem->IsChecked() == TRUE)
		{
			nDeleteCount++;
		}
	}

	int nRestCount = (nRecordCount-nDeleteCount); // 남개 되는 항목이 2개 이상을 유지 시켜야 한다. 
	if (nRestCount< 2)
	{
		AfxMessageBox(_T("최소 2개 이상의 선택 항목이 존재해야 합니다."));
		return;
	}

	for (int i=nRecordCount-1; i>=0; i--)
	{
		pRecord    = pRecords->GetAt(i);
		pItem      = pRecord->GetItem(2);

		if (pItem->IsChecked() == TRUE)
		{
			pRecord->Delete();
		}
	}

	m_wndReportCtrl.Populate();

	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO2)
	{
		nRecordCount = m_wndReportCtrl.GetRecords()->GetCount();
		if (nRecordCount < 1)
		{
			m_nRealDigit = 0;
		}
	}

	RecalRange();
}

void KPolygonStyleValueEdit::Apply()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	CString strStyleName;
	bool    bOverwrite     = false;
	int     nID = 0;

	int  nCurSelectedIndex = m_cboStyle.GetCurSel();
	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		bOverwrite = true;

		if (nCurSelectedIndex < 0)
		{
			GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();
			return;
		}

		nID = m_cboStyle.GetItemData(nCurSelectedIndex);
	}
	else
	{
		GetDlgItemText(IDC_EDIT_STYLE_NAME, strStyleName);
		strStyleName = strStyleName.Trim();

		if (strStyleName.GetLength() == 0)
		{			
			AfxMessageBox(_T("명칭을 입력해 주세요."));
			GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();
			return;
		}
		nID = KProjectDBase::NewZoneTemplateID(spDBaseConnection, 1);
	}

	CXTPReportRecords*	  pRecords = m_wndReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;	
	CXTPReportRecordItemText* pItemText = NULL;
	int nRecordCount = pRecords->GetCount();

	try
	{
		spDBaseConnection->BeginTransaction();
		CString strSQL;	
		if (!bOverwrite)
		{
			strSQL.Format(_T("INSERT OR REPLACE INTO ZoneTemplate(ID,Type,Name) Values('%d','%d','%s')"), nID,1,strStyleName);
			spDBaseConnection->ExecuteUpdate(strSQL);
		}		

		strSQL.Format(_T("Delete From ZoneTemplateValue Where ID = '%d'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);

		for (int i=0; i<nRecordCount; i++)
		{
			CString      strRGB;
			pRecord    = pRecords->GetAt(i);
			pItem      = pRecord->GetItem(0);

			int nSymbol = pItem->GetItemData();
			COLORREF oColor = pItem->GetTextColor();
			strRGB.Format(_T("%d,%d,%d"), GetRValue(oColor), GetGValue(oColor), GetBValue(oColor));
						
			pItemText    = (CXTPReportRecordItemText*)pRecord->GetItem(1);
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

		AfxMessageBox(_T("저장하였습니다."));

		GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STYLE_DELETE)->EnableWindow(TRUE);
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		AfxMessageBox(_T("정보 저장에 실패하였습니다."));
	}

	LoadInitialStyle();
}

void KPolygonStyleValueEdit::OnBnClickedOk()
{
	Apply();
}

void KPolygonStyleValueEdit::OnBnClickedButtonStyleDelete()
{
	int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO2)
	{
		return;
	}

	int     nID = 0;

	int  nCurSelectedIndex = m_cboStyle.GetCurSel();
	if (nCurSelectedIndex < 0)
	{
		GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();
		return;
	}

	nID = m_cboStyle.GetItemData(nCurSelectedIndex);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
	if (spDBaseConnection == NULL)
	{		
		AfxMessageBox(_T("Project.db Not Found"));
		return;
	}

	try
	{
		CString strSQL;

		spDBaseConnection->BeginTransaction();		
		strSQL.Format(_T("Delete From ZoneTemplate Where ID = '%d' and Type='1'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);		

		strSQL.Format(_T("Delete From ZoneTemplateValue Where ID = '%d'"), nID);
		spDBaseConnection->ExecuteUpdate(strSQL);		
		spDBaseConnection->Commit();

		m_nPreSelectedStyleID = 0;
	}
    catch (KExceptionPtr ex)
    {
        spDBaseConnection->RollBack();
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        spDBaseConnection->RollBack();
        TxLogDebugException();
    }

    try
    {
        LoadInitialStyle();
        StyleSelectChangeAction();
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

void KPolygonStyleValueEdit::PreStyle( double& ar_dMin, double& ar_dMax )
{
	
	CXTPReportRecord*           pRecordLast = NULL;
	CXTPReportRecord*           pRecord     = NULL;
	CXTPReportRecordItemText*   pItemText   = NULL;
	CXTPReportRecords*	        pRecords    = m_wndReportCtrl.GetRecords();
	int nRecordCount          = pRecords->GetCount();

	if (nRecordCount == 0)
	{		
		ar_dMin     = 0.0;
		ar_dMax     = 1000.0;
		return;
	}

	pRecordLast = pRecords->GetAt(nRecordCount-1); // last

	if (nRecordCount < 2) 
	{
		pRecord = pRecords->GetAt(nRecordCount-1); // last
	}
	else if (nRecordCount == 2)
	{
		pRecord = pRecords->GetAt(nRecordCount-2); // last -1
	}
	else
	{
		pRecord = pRecords->GetAt(nRecordCount-2); // last -1
	}

	pItemText    = (CXTPReportRecordItemText*)pRecord->GetItem(1);
	CString strCaption = pItemText->GetCaption(NULL);
	std::vector<CString> vecValue;
	CString strTok;
	int     nTokenPos = 0; 
	while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '~'))
	{
		vecValue.push_back(strTok);
	}

	if (vecValue.size() < 2)
	{
		ar_dMin = 99999999.0;
		ar_dMax = ar_dMin;
		return;
	}

	double dMinWill(0.0);
	double dMaxWill(0.0);

	if (nRecordCount < 2) 
	{
		/************************************************************************/
		/* below~2.0                                                            */
		/************************************************************************/
		dMinWill = _ttof(vecValue[1]); // ex) below~2.0 =>  below~2.0, 2.0~above
	}
	else if (nRecordCount == 2)
	{
		/************************************************************************/
		/* below~0.0                                                            */
		/* 0.0~above                                                            */
		/************************************************************************/
		double dMaxLastPrev = _ttof(vecValue[1]);

		double dMinLast(0.0);
		double dMaxLast(0.0);

		dMinLast = dMaxLastPrev;
		if (dMinLast == 0.0)
		{
			dMaxLast = 1000.0;
		}
		else
		{
			dMaxLast = dMinLast + 1000.0;
		}

		// New Item을 Add 하기전에 현재 Last Item에 Max 값 부여 ex) 2.0~above => 2.0~maxvalue
		CXTPReportRecordItemText* pItemTextLast = (CXTPReportRecordItemText*)pRecordLast->GetItem(1);

		int     nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);
		CString strMaxLast           = QBicStringFormat::FormatString(dMaxLast, nDisplayDecimalPoint);
		CString strMinLast           = QBicStringFormat::FormatString(dMinLast, nDisplayDecimalPoint);
		CString strValueLast, strCaptionLast;

		strValueLast.Format(_T("%s"), strMaxLast);
		strCaptionLast.Format(_T("%s~%s"), strMinLast, strMaxLast);

		pItemTextLast->SetValue(strValueLast);
		pItemTextLast->SetCaption(strCaptionLast);
		pItemTextLast->SetEditable(TRUE);

		dMinWill = dMaxLast;
	}
	else
	{
		/************************************************************************/
		/* below~2.0                                                            */
		/* 2.0~4.0                                                              */
		/* 4.0~above                                                            */
		/************************************************************************/

		double dMinLastPrev = _ttof(vecValue[0]);
		double dMaxLastPrev = _ttof(vecValue[1]);
		double dGap         = dMaxLastPrev - dMinLastPrev;

		double dMinLast(0.0);
		double dMaxLast(0.0);

		dMinLast = dMaxLastPrev;
		dMaxLast = dMinLast + dGap;

		// New Item을 Add 하기전에 현재 Last Item에 Max 값 부여 ex) 2.0~above => 2.0~maxvalue
		CXTPReportRecordItemText* pItemTextLast = (CXTPReportRecordItemText*)pRecordLast->GetItem(1);

		int     nDisplayDecimalPoint = GetDisplayDecimalPoint(m_nRealDigit);
		CString strMaxLast           = QBicStringFormat::FormatString(dMaxLast, nDisplayDecimalPoint);
		CString strMinLast           = QBicStringFormat::FormatString(dMinLast, nDisplayDecimalPoint);
		CString strValueLast, strCaptionLast;

		strValueLast.Format(_T("%s"), strMaxLast);
		strCaptionLast.Format(_T("%s~%s"), strMinLast, strMaxLast);

		pItemTextLast->SetValue(strValueLast);
		pItemTextLast->SetCaption(strCaptionLast);
		pItemTextLast->SetEditable(TRUE);

		dMinWill = dMaxLast;
	}

// 	double dMin = _ttof(vecValue[1]);
// 	double dMax = 2.0 * dMin - _ttof(vecValue[0]);

	ar_dMin = dMinWill;
	ar_dMax = dMaxWill;
}

void KPolygonStyleValueEdit::OnBnClickedRadioEditMethod(UINT nID)
{
	try
	{
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			StyleSelectChangeAction();
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

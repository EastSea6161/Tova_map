// GClassificationEditorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GClassificationEditorDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseConnector.h"

// KGClassificationEditorDlg 대화 상자입니다.

const int nZeroItemData = 0;

IMPLEMENT_DYNAMIC(KGClassificationEditorDlg, KResizeDialogEx)

KGClassificationEditorDlg::KGClassificationEditorDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KGClassificationEditorDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_nClassID(0)

{

}

KGClassificationEditorDlg::~KGClassificationEditorDlg()
{
}

void KGClassificationEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_COMBO_NAME, m_cboName);
	DDX_Control(pDX, IDC_COMBO_AGE, m_cboAge);
}


BEGIN_MESSAGE_MAP(KGClassificationEditorDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KGClassificationEditorDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_CHECK_SEX, &KGClassificationEditorDlg::OnBnClickedCheckSex)
	ON_BN_CLICKED(IDC_CHECK_AGE, &KGClassificationEditorDlg::OnBnClickedCheckAge)
	ON_CBN_SELCHANGE(IDC_COMBO_AGE, &KGClassificationEditorDlg::OnCbnSelchangeComboAge)
	ON_CBN_SELCHANGE(IDC_COMBO_NAME, &KGClassificationEditorDlg::OnCbnSelchangeComboName)

	ON_NOTIFY(NM_CLICK, IDC_REPORT,   &KGClassificationEditorDlg::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KGClassificationEditorDlg::OnReportItemValueChanged)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &KGClassificationEditorDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &KGClassificationEditorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KGClassificationEditorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KGClassificationEditorDlg 메시지 처리기입니다.


BOOL KGClassificationEditorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReport);
	
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	OnBnClickedRadio(IDC_RADIO1);

	InitComboName();
	InitComboAge();

	ResizeComponet();

	return TRUE;  
}

void KGClassificationEditorDlg::OnBnClickedRadio( UINT nID )
{
	try
	{
		if (nID == IDC_RADIO1)
		{
			GetDlgItem(IDC_COMBO_NAME)->ShowWindow(FALSE);
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);

			GetDlgItem(IDC_CHECK_SEX)->EnableWindow(TRUE);
			CheckDlgButton(IDC_CHECK_SEX, FALSE);
			GetDlgItem(IDC_CHECK_AGE)->EnableWindow(TRUE);
			CheckDlgButton(IDC_CHECK_AGE, FALSE);
			GetDlgItem(IDC_COMBO_AGE)->EnableWindow(TRUE);

			GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);

			m_cboAge.EnableWindow(FALSE);
			
			InitComboAge();
			ChangeReportData();
		}
		else
		{
			GetDlgItem(IDC_COMBO_NAME)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);

			GetDlgItem(IDC_CHECK_SEX)->EnableWindow(FALSE);
			CheckDlgButton(IDC_CHECK_SEX, FALSE);
			GetDlgItem(IDC_CHECK_AGE)->EnableWindow(FALSE);
			CheckDlgButton(IDC_CHECK_AGE, FALSE);
			GetDlgItem(IDC_COMBO_AGE)->EnableWindow(FALSE);
			
			InitComboAge();
			InitComboName();

			ChangeReportData();

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
}

void KGClassificationEditorDlg::InitComboAge( void )
{
	try
	{
		m_cboAge.ResetContent();

		int nZeroCur = m_cboAge.AddString(_T("1"));
		m_cboAge.SetItemData(nZeroCur, (DWORD_PTR)1);

		for (int i = 2; i < 100; i++)
		{
			CString strAge(_T(""));
			strAge.Format(_T("%d"), i);
			int nIndex = m_cboAge.AddString(strAge);
			m_cboAge.SetItemData(nIndex, (DWORD_PTR)i);
		}

		m_cboAge.SetCurSel(nZeroCur);
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

void KGClassificationEditorDlg::InitComboName( void )
{
	try
	{
		m_cboName.ResetContent();

		int nZeroCur = m_cboName.AddString(_T("---Select---"));

		m_cboName.SetItemData(nZeroCur, nZeroItemData);
		KDBaseGeneration::GetCategoryClass(m_pTarget, m_mapCategoryClassInfo);

		AutoType iter = m_mapCategoryClassInfo.begin();
		AutoType end  = m_mapCategoryClassInfo.end();
		while(iter != end)
		{
			TCategoryClassInto oInfo = iter->second;
			int nComboIndex = m_cboName.AddString(oInfo.strCategoryName);
			m_cboName.SetItemData(nComboIndex, (DWORD_PTR)oInfo.nCategoryID);
			++iter;
		}

		m_cboName.SetCurSel(nZeroCur);

		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);

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

void KGClassificationEditorDlg::InitReportHeader( void )
{
	try
	{
		m_wndReport.ResetContent();
		CXTPReportColumns*   pColumns =  m_wndReport.GetColumns();
		pColumns->Clear();

		CXTPReportColumn* pColumn = nullptr;
		if (IsDlgButtonChecked(IDC_CHECK_SEX) == BST_CHECKED)
		{
			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Age"), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pColumn->SetCaption(_T("연령"));
			}

			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Male"), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pColumn->SetCaption(_T("남"));
			}

			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Female"), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pColumn->SetCaption(_T("여"));
			}

		}
		else
		{
			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Age"), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pColumn->SetCaption(_T("연령"));
			}

			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Male/Female"), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pColumn->SetCaption(_T("남/여"));
			}
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
}

void KGClassificationEditorDlg::UpdateReportData( void )
{
	try
	{
		BOOL bCheckNew = IsDlgButtonChecked(IDC_RADIO1);

		int nCurSel		= m_cboName.GetCurSel();
		int nNameIndex	= m_cboName.GetItemData(nCurSel);

		if (TRUE == bCheckNew || nZeroItemData == nNameIndex)
		{
			UpdateReportDataNew();
		}
		else
		{
			UpdateReportDataExist();
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
}

void KGClassificationEditorDlg::UpdateReportDataNew( void )
{
	try
	{
		int nCurSelAge		= m_cboAge.GetCurSel();
		int nAgeCategory	= m_cboAge.GetItemData(nCurSelAge); 

		if (IsDlgButtonChecked(IDC_CHECK_AGE))
		{
			std::map<int, KODKey> mapMinMaxValue;
			CalculateClassGroup(nAgeCategory, mapMinMaxValue);

			CXTPReportRecord*     pRecord = NULL;
			CXTPReportRecordItem* pItem   = NULL;

			int nIndex(0);
			AutoType iter = mapMinMaxValue.begin();
			AutoType end  = mapMinMaxValue.end();
			while(iter != end)
			{
				KODKey oODKey = iter->second;
				CString strAge(_T(""));
				strAge.Format(_T("%I64d-%I64d"), oODKey.OriginID, oODKey.DestinationID);
				CString strMaxAge(_T(""));
				strMaxAge.Format(_T("%I64d"), oODKey.DestinationID);

				pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

				if (nIndex == 0)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strAge));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMaxAge));
					pItem->SetCaption(strAge);
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}

				if (IsDlgButtonChecked(IDC_CHECK_SEX))
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);

				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}

				nIndex++;
				++iter;
			}
		}
		else
		{
			CXTPReportRecord*     pRecord = nullptr;
			CXTPReportRecordItem* pItem   = nullptr;

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("전연령")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(false);

			if (IsDlgButtonChecked(IDC_CHECK_SEX))
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetEditable(TRUE);

			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
				pItem->SetAlignment(xtpColumnTextCenter);
				pItem->SetEditable(TRUE);
			}
		}

		m_wndReport.Populate();
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

void KGClassificationEditorDlg::UpdateReportDataExist( void )
{
	try
	{
		int nCurSelAge		= m_cboAge.GetCurSel();
		int nAgeCategory	= m_cboAge.GetItemData(nCurSelAge); 

		int nCurselName		= m_cboName.GetCurSel();
		int nNameIndex		= m_cboName.GetItemData(nCurselName);

		TCategoryClassInto oCategoryInfo;
		AutoType find = m_mapCategoryClassInfo.find(nNameIndex);
		AutoType end  = m_mapCategoryClassInfo.end();
		
		if (find != end)
		{
			oCategoryInfo = find->second;
		}

		std::map<KODKey,double> mapValue = oCategoryInfo.mapClassValue;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		
		if (IsDlgButtonChecked(IDC_CHECK_AGE))
		{
			int nIndex(1);
			AutoType iter = oCategoryInfo.mapAgeGroup.begin();
			AutoType end  = oCategoryInfo.mapAgeGroup.end();
			while(iter != end)
			{
				KODKey oODKey = iter->second;
				CString strAge(_T(""));
				strAge.Format(_T("%I64d-%I64d"), oODKey.OriginID, oODKey.DestinationID);
				CString strMaxAge(_T(""));
				strMaxAge.Format(_T("%I64d"), oODKey.DestinationID);

				pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

				if (nIndex == 1)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strAge));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMaxAge));
					pItem->SetCaption(strAge);
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}

				if (IsDlgButtonChecked(IDC_CHECK_SEX))
				{
					KODKey oMaleODKey(1, (Integer)nIndex);
					AutoType findMale = mapValue.find(oMaleODKey);
					AutoType end  = mapValue.end();
					if (findMale != end)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(findMale->second, _T("%.3f")));
						pItem->SetAlignment(xtpColumnTextCenter);
						pItem->SetEditable(TRUE);
					}
					else
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
						pItem->SetAlignment(xtpColumnTextCenter);
						pItem->SetEditable(TRUE);
					}

					KODKey oFeMaleODKey(2,(Integer)nIndex);
					AutoType findFeMale = mapValue.find(oFeMaleODKey);
					if (findFeMale != end)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(findMale->second, _T("%.3f")));
						pItem->SetAlignment(xtpColumnTextCenter);
						pItem->SetEditable(TRUE);
					}
					else
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
						pItem->SetAlignment(xtpColumnTextCenter);
						pItem->SetEditable(TRUE);
					}
				}
				else
				{
					KODKey oAllODKey(0, (Integer)nIndex);
					AutoType findAll = mapValue.find(oAllODKey);
					AutoType end  = mapValue.end();
					if (findAll != end)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(findAll->second, _T("%.3f")));
						pItem->SetAlignment(xtpColumnTextCenter);
						pItem->SetEditable(TRUE);
					}
					else
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
						pItem->SetAlignment(xtpColumnTextCenter);
						pItem->SetEditable(TRUE);
					}
				}

				++iter;
				nIndex++;
			}

		}
		else
		{

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("All")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(false);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("전연령"));
			}

			if (IsDlgButtonChecked(IDC_CHECK_SEX))
			{
				KODKey oMaleODKey(1, 1);
				AutoType findMale = mapValue.find(oMaleODKey);
				AutoType end  = mapValue.end();
				if (findMale != end)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(findMale->second, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}

				KODKey oFeMaleODKey(2,1);
				AutoType findFeMale = mapValue.find(oFeMaleODKey);
				if (findFeMale != end)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(findMale->second, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
			}
			else
			{
				KODKey oAllODKey(0, 1);
				AutoType findAll = mapValue.find(oAllODKey);
				AutoType end  = mapValue.end();
				if (findAll != end)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(findAll->second, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1.0, _T("%.3f")));
					pItem->SetAlignment(xtpColumnTextCenter);
					pItem->SetEditable(TRUE);
				}
			}
		}

		m_wndReport.Populate();
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


void KGClassificationEditorDlg::CalculateClassGroup(int a_AgeCategory, std::map<int, KODKey>& ar_mapAgeMinMaxValue)
{
	ar_mapAgeMinMaxValue.clear();
	try
	{
		int nMaxAge(1);
		KDBaseGeneration::GetSEDPopMaxAge(m_pTarget, nMaxAge);

		Integer nxAverageValue = (Integer) nMaxAge/a_AgeCategory;
		
		if (nxAverageValue < 1)
		{
			nxAverageValue = 1;
		}

		Integer nxMinAge(0);
		Integer nxMaxAge(0);
		for (int i = 1; i <= a_AgeCategory; i++)
		{
			if (i == 0)
			{
				nxMinAge = 1;
			}
			else
			{
				nxMinAge = nxMaxAge + 1;
			}

			if (i == a_AgeCategory && (Integer)nMaxAge  >= (nxAverageValue * i))
			{
				nxMaxAge = (Integer)nMaxAge;
			}
			else
			{
				nxMaxAge = nxAverageValue * i;
			}
			KODKey oODKey(nxMinAge, nxMaxAge);
			ar_mapAgeMinMaxValue.insert(std::make_pair(i, oODKey));
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
}

void KGClassificationEditorDlg::ChangeReportData( void )
{
	try
	{
		int nCurSel		= m_cboName.GetCurSel();
		int nNameIndex	= m_cboName.GetItemData(nCurSel);

		int nCheck = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2 );

		TCategoryClassInto oCategoryInfo;
		AutoType find = m_mapCategoryClassInfo.find(nNameIndex);
		AutoType end  = m_mapCategoryClassInfo.end();
		if (find != end && nCheck == IDC_RADIO2 )
		{
			oCategoryInfo = find->second;
			ChangeControl(oCategoryInfo);
			InitReportHeader();
			UpdateReportData();
		}
		else if(nCheck == IDC_RADIO1)
		{
			InitReportHeader();
			UpdateReportData();
		}
		else
		{
			InitReportHeader();
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
}

void KGClassificationEditorDlg::OnBnClickedCheckSex()
{
	ChangeReportData();
}

void KGClassificationEditorDlg::OnBnClickedCheckAge()
{
	try
	{
		if(IsDlgButtonChecked(IDC_CHECK_AGE) == BST_CHECKED)
		{
			m_cboAge.EnableWindow(TRUE);
		}
		else
		{
			m_cboAge.SetCurSel(nZeroItemData);
			m_cboAge.EnableWindow(FALSE);
		}

		ChangeReportData();
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

void KGClassificationEditorDlg::OnCbnSelchangeComboAge()
{
	ChangeReportData();
}

void KGClassificationEditorDlg::OnCbnSelchangeComboName()
{
	int nCurselName		= m_cboName.GetCurSel();
	int nNameIndex		= m_cboName.GetItemData(nCurselName);

	if (nNameIndex == nZeroItemData)
	{
		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);
		ChangeReportData();
		ClearControl();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(TRUE);
		ChangeReportData();
	}
	
}

void KGClassificationEditorDlg::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nRowIndex    = pItemNotify->pRow->GetIndex();
	int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

	if (0 == nColumnIndex)
	{
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
		m_strPreValue   = pItemText->GetValue();
		m_strPreCaption = pItemText->GetCaption(NULL);
	}
}

void KGClassificationEditorDlg::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

		if (!pItemNotify->pRow || !pItemNotify->pColumn)
			return;

		int nRowIndex    = pItemNotify->pRow->GetIndex();
		int nColumnIndex = pItemNotify->pColumn->GetItemIndex();

		int nRecordCount = m_wndReport.GetRecords()->GetCount();

		CXTPReportRecordItemText* pItemText = nullptr;

		if (nColumnIndex == 0)
		{
			pItemText = (CXTPReportRecordItemText*)pItemNotify->pItem;
			CString strValue   = pItemText->GetValue();
			CString strCaption = pItemText->GetCaption(NULL);

			strValue.Replace(_T(" "),	_T(""));
			strCaption.Replace(_T(" "), _T(""));

			std::vector<CString> vecValue;
			CString strTok;
			int		nTokenPos = 0;

			if (nRowIndex == 0)
			{
				while (AfxExtractSubString(strTok, strValue, nTokenPos++, '-'))
				{
					vecValue.push_back(strTok);
				}
			}
			else
			{
				while (AfxExtractSubString(strTok, strCaption, nTokenPos++, '-'))
				{
					vecValue.push_back(strTok);
				}
			}

			if (vecValue.size() != 2)
			{
				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_wndReport.RedrawControl();
				return;			
			}

			if (nRowIndex == 0)
			{
				if (QBicStringChecker::IsNumeric(vecValue[0]) == false)
				{
					AfxMessageBox(_T("숫자를 입력해 주세요."));
					pItemText->SetValue(m_strPreValue);
					pItemText->SetCaption(m_strPreCaption);
					m_wndReport.RedrawControl();
					return;
				}

				if (QBicStringChecker::IsNumeric(vecValue[1]) == false)
				{
					AfxMessageBox(_T("숫자를 입력해 주세요."));
					pItemText->SetValue(m_strPreValue);
					pItemText->SetCaption(m_strPreCaption);
					m_wndReport.RedrawControl();
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
					m_wndReport.RedrawControl();
					return;
				}
			}

			int nMin = _ttoi(vecValue[0]);
			int nMax = 0;

			if (nRowIndex == 0)
			{
				nMax = _ttoi(vecValue[1]);
			}
			else
			{
				nMax = _ttoi(strValue);
			}

			if (nMin > nMax)
			{
				CString strOut;
				strOut.Format(_T("%.1f보다 큰 숫자를 입력해 주세요."), nMin);
				AfxMessageBox(strOut);

				pItemText->SetValue(m_strPreValue);
				pItemText->SetCaption(m_strPreCaption);
				m_wndReport.RedrawControl();
				return;
			}
			else
			{
				strCaption.Format(_T("%d-%d"), nMin, nMax);
				pItemText->SetCaption(strCaption);
			}

			CalculateReportAge();
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
}



void KGClassificationEditorDlg::CalculateReportAge( void )
{
	try
	{
		CXTPReportRecords*			pRecords	= nullptr;
		CXTPReportRecord*			pRecord		= nullptr;
		CXTPReportRows*				pRows		= nullptr;
		CXTPReportRecordItemText*	pItemText   = nullptr;

		pRecords = m_wndReport.GetRecords();
		int nCount = pRecords->GetCount();

		int nPreValue = -9999;

		for (int i = 0; i < nCount; i ++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0); 

			CString strCaption = pItemText->GetCaption(NULL);
			std::vector<CString> vecValue;
			CString strTok;
			int	nTokenPos = 0;
			while(AfxExtractSubString(strTok, strCaption, nTokenPos++, '-'))
			{
				vecValue.push_back(strTok);
			}

			int nMinValue = _ttoi(vecValue[0]);
			int nMaxValue = _ttoi(vecValue[1]);

			if (i == 0)
			{
				nPreValue = nMaxValue;
			}
			else
			{
				if (nPreValue != nMinValue)
				{
					nMinValue = nPreValue;
				}
			}

			if (nPreValue > nMaxValue)
			{
				nMaxValue = nPreValue;
			}

			CString strValue;
			strValue.Format(_T("%d"), nMaxValue);		
			strCaption.Format(_T("%d-%d"), nMinValue, nMaxValue);

			if (i == 0)
			{
				pItemText->SetValue(strCaption);
			}
			else
			{
				pItemText->SetValue(strValue);
			}

			pItemText->SetCaption(strCaption);		
			nPreValue = nMaxValue + 1;

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
}

void KGClassificationEditorDlg::OnBnClickedButtonDel()
{
	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			if (IDNO == AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO))
			{
				return;
			}
		}
		else {
			if (IDNO == AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO))
			{
				return;
			}
		}
		

		int nCurselName		= m_cboName.GetCurSel();
		int nNameIndex		= m_cboName.GetItemData(nCurselName);

		KDBaseGeneration::DeleteCategoryClass(m_pTarget, nNameIndex);

		InitComboName();
		ChangeReportData();

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

void KGClassificationEditorDlg::OnBnClickedOk()
{
	try
	{
		CButton* pRadioNew;
		pRadioNew = (CButton*)GetDlgItem(IDC_RADIO1);
		BOOL bCheck = pRadioNew->GetCheck();

		if (bCheck == TRUE)
		{
			SaveNewCategory();
		}
		else
		{
			SaveExistCategory();
		}

		KResizeDialogEx::OnOK();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}
}

void KGClassificationEditorDlg::SaveExistCategory()
{
	try
	{
		int nCurselName		= m_cboName.GetCurSel();
		int nNameIndex		= m_cboName.GetItemData(nCurselName);

		KDBaseGeneration::DeleteCategoryValue(m_pTarget, nNameIndex);
		SaveCategoryClassValue(nNameIndex);
		m_nClassID = nNameIndex;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KGClassificationEditorDlg::SaveNewCategory()
{
	try
	{
		int nMaxID(0);
		KDBaseGeneration::GetMaxCategoryID(m_pTarget, nMaxID);
		SaveCategoryClass(nMaxID);

		BOOL bChkAge		= IsDlgButtonChecked(IDC_CHECK_AGE);
		if (bChkAge == TRUE)
		{
			SaveCategoryAgeGroup(nMaxID);
		}

		SaveCategoryClassValue(nMaxID);
		m_nClassID = nMaxID;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


void KGClassificationEditorDlg::SaveCategoryClass( int a_nMaxID )
{
	CString strName(_T(""));
	int nAgeCheck(0);
	int nSexCheck(0);
	CString strSQL(_T(""));
	try
	{
		GetDlgItemText(IDC_EDIT_NAME,strName);
		if (_T("") == strName)
		{
			ThrowException(_T("Name의 값이 없습니다."));
		}

		BOOL bChkAge		= IsDlgButtonChecked(IDC_CHECK_AGE);
		if (bChkAge == TRUE)
			nAgeCheck = 1;

		BOOL bChkSex		= IsDlgButtonChecked(IDC_CHECK_SEX);
		if (bChkSex == TRUE)
			nSexCheck = 1;

		strSQL.Format(_T("Insert Into %s(category_id, name, sex_use, age_use) "), TABLE_CATEGORY_CLASS);
		strSQL.AppendFormat(_T("Values (%d, '%s', %d, %d)"), a_nMaxID, strName, nSexCheck, nAgeCheck);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KGClassificationEditorDlg::SaveCategoryAgeGroup( int a_nMaxID )
{
	std::map<int, KODKey> mapCategroyAgeGroup;

	try
	{
		CXTPReportRecords*			pRecords	= nullptr;
		CXTPReportRecord*			pRecord		= nullptr;
		CXTPReportRows*				pRows		= nullptr;
		CXTPReportRecordItemText*	pItemText   = nullptr;

		pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			CString strCaption = pItemText->GetCaption(NULL);
			std::vector<CString> vecValue;
			CString strTok;
			int nTokenPos(0);
			while(AfxExtractSubString(strTok, strCaption, nTokenPos++, '-'))
			{
				vecValue.push_back(strTok);
			}

			Integer nMinValue = _ttoi64(vecValue[0]);
			Integer nMaxValue = _ttoi64(vecValue[1]);
			KODKey oODKey(nMinValue, nMaxValue);

			mapCategroyAgeGroup.insert(std::make_pair(i, oODKey));
		}

		KDBaseGeneration::SaveCategroyAgeGroup(m_pTarget, a_nMaxID, mapCategroyAgeGroup);
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

void KGClassificationEditorDlg::SaveCategoryClassValue( int a_nMaxID )
{
	std::map<KODKey, double> mapCategroyAgeValue;
	try
	{
		BOOL bChkSex		= IsDlgButtonChecked(IDC_CHECK_SEX);
		if (bChkSex == TRUE)
		{
			SaveCategoryClassValueUseSex(mapCategroyAgeValue);
		}
		else
		{
			SaveCategoryClassValueNotUseSex(mapCategroyAgeValue);
		}

		KDBaseGeneration::SaveCategoryClassValue(m_pTarget, a_nMaxID, mapCategroyAgeValue);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KGClassificationEditorDlg::SaveCategoryClassValueUseSex( std::map<KODKey, double>& ar_mapCategroyValue )
{
	try
	{
		CXTPReportRecords*			pRecords	= NULL;
		CXTPReportRecord*			pRecord		= NULL;
		CXTPReportRows*				pRows		= NULL;
		CXTPReportRecordItemNumber*	pItemNumber   = NULL;

		pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			double dValue = pItemNumber->GetValue();
			KODKey oMaleODKey(1, i);
			ar_mapCategroyValue.insert(std::make_pair(oMaleODKey, dValue));

			pRecord = pRecords->GetAt(i);
			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			dValue = pItemNumber->GetValue();
			KODKey oFemaleODKey(2, i);
			ar_mapCategroyValue.insert(std::make_pair(oFemaleODKey, dValue));
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
		throw -1;
	}
}

void KGClassificationEditorDlg::SaveCategoryClassValueNotUseSex( std::map<KODKey, double>& ar_mapCategroyValue )
{
	try
	{
		CXTPReportRecords*			pRecords	= NULL;
		CXTPReportRecord*			pRecord		= NULL;
		CXTPReportRows*				pRows		= NULL;
		CXTPReportRecordItemNumber*	pItemNumber   = NULL;

		pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			double dValue = pItemNumber->GetValue();
			KODKey oAllODKey(0, i);
			ar_mapCategroyValue.insert(std::make_pair(oAllODKey, dValue));
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
		throw -1;
	}
}


void KGClassificationEditorDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KGClassificationEditorDlg::ChangeControl( TCategoryClassInto a_oCategoryInfo )
{
	try
	{
		if (a_oCategoryInfo.nAgeUse == 0)
		{
			CheckDlgButton(IDC_CHECK_AGE, FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_AGE, TRUE);
		}

		if (a_oCategoryInfo.nSexUse == 0)
		{
			CheckDlgButton(IDC_CHECK_SEX, FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_SEX, TRUE);
		}

		int nCountValue = a_oCategoryInfo.mapAgeGroup.size();
		int nCursel(0);
		int nCount = m_cboAge.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			int nItemData = m_cboAge.GetItemData(i);
			if (nCountValue == 0)
			{
				if (nItemData == 1)
				{
					nCursel = i;
				}
			}
			else if (nItemData == nCountValue)
			{
				nCursel = i;
			}

		}
		m_cboAge.SetCurSel(nCursel);

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

void KGClassificationEditorDlg::ClearControl( void )
{
	try
	{
		GetDlgItem(IDC_CHECK_SEX)->EnableWindow(FALSE);
		CheckDlgButton(IDC_CHECK_SEX, FALSE);
		GetDlgItem(IDC_CHECK_AGE)->EnableWindow(FALSE);
		CheckDlgButton(IDC_CHECK_AGE, FALSE);

		m_cboAge.SetCurSel(nZeroItemData);
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

void KGClassificationEditorDlg::ResizeComponet( void )
{
	SetResize(IDC_RADIO1,			SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_RADIO2,			SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_STATIC_NAME,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_COMBO_NAME,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_EDIT_NAME,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_DEL,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_CHECK_SEX,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_CHECK_AGE,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_CHECK_AGE,		SZ_TOP_LEFT,      SZ_TOP_LEFT);
	SetResize(IDC_REPORT,			SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR2,		SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,					SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}

int KGClassificationEditorDlg::GetClassID()
{
	return m_nClassID;
}

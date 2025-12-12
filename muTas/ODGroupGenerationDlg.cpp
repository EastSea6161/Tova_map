// ODGroupGenerationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ODGroupGenerationDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "ImChampFrameWindow.h"
#include "TableEditFieldInfo.h"
#include "KBulkDBase.h"
#include "KxBulkDbase.h"

enum
{
	_0_COLUMN_ID,
	_1_COLUMN_NAME,
	_2_COLUMN_ORIGIN,
	_3_COLUMN_DESTINATION
};

// KODGroupGenerationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KODGroupGenerationDlg, KResizeDialogEx)

KODGroupGenerationDlg::KODGroupGenerationDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KODGroupGenerationDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KODGroupGenerationDlg::~KODGroupGenerationDlg()
{
}

void KODGroupGenerationDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBO_GROUP_NAME, m_cboGroupField);
	DDX_Control(pDX, IDC_REPORT2,        m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO1,         m_cboOutColumnName);
}


BEGIN_MESSAGE_MAP(KODGroupGenerationDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2,   &KODGroupGenerationDlg::OnBnClickedRadio)
	ON_CBN_SELCHANGE(IDC_CBO_GROUP_NAME,                   &KODGroupGenerationDlg::OnCbnSelchangeCboGroupField)
	ON_BN_CLICKED(IDCANCEL, &KODGroupGenerationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,     &KODGroupGenerationDlg::OnBnClickedOk)

END_MESSAGE_MAP()


// KODGroupGenerationDlg 메시지 처리기입니다.


void KODGroupGenerationDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC_HBar2,      SZ_TOP_LEFT,      SZ_TOP_RIGHT);
		SetResize(IDC_REPORT2,           SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC4,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO1,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO2,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_COLUMN_NAME,  SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO1,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);

		SetResize(IDC_STATIC_HBar1,      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);

		SetResize(IDOK,                  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,              SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KODGroupGenerationDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, FALSE);
	InitReportHeader();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	ControlRadio();

	LoadZoneGroupField();
	LoadReportData();
	m_ctrlReport.Populate();
	LoadOutColumnName();

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KODGroupGenerationDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_0_COLUMN_ID,       _T("Code"),     50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("코드번호"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME,     _T("Name"),     40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("명칭"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_2_COLUMN_ORIGIN,   _T("Origin"),   30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("기점"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_3_COLUMN_DESTINATION, _T("Destination"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("종점"));
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


void KODGroupGenerationDlg::LoadZoneGroupGenInfo(std::map<CString, int> &a_mapZoneColumnGroupKey)
{
	a_mapZoneColumnGroupKey.clear();

// 	try
// 	{
// 		CString strSQL(_T(" SELECT KCode_Group_Key, Output_Column FROM Group_Generation_Zone ORDER BY seq DESC "));
// 
// 		KDBaseConPtr   spDBaseConnection = m_pTarget->GetDBaseConnection();
// 		KResultSetPtr         spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
// 		
// 		while (spResultSet->Next())
// 		{
// 			int     nGroupKey           = spResultSet->GetValueInt(0);
// 			CString strOutputColumnName = spResultSet->GetValueString(1);
// 
// 			a_mapZoneColumnGroupKey.insert(std::make_pair(strOutputColumnName, nGroupKey));
// 		}
// 	}
// 	catch (...)
// 	{
// 		TxLogDebugException();	
// 		a_mapZoneColumnGroupKey.clear();
// 	}
}

void KODGroupGenerationDlg::LoadZoneGroupField()
{
	try
	{
		CString strPreSelName(_T(""));

		int nPreSel = m_cboGroupField.GetCurSel();
		if (nPreSel >=0)
		{
			TCodeGoupColumn* pTCodeGoupColumn = (TCodeGoupColumn*)m_cboGroupField.GetItemData(nPreSel);
			try // delete Coloumn이후 m_cboOutColumnName에 반영이 안되어
			{
				strPreSelName = pTCodeGoupColumn->pIOColumn->Name();
			}
			catch (...)
			{
				TxLogDebugException();	
				strPreSelName = _T("");
			}
		}

		m_cboGroupField.ResetContent();
		m_mapCodeGoupColumn.clear();

// 		std::map<CString, int> mapZoneColumnGroupKey;
// 		LoadZoneGroupGenInfo(mapZoneColumnGroupKey);

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
		const 
			KIOColumns* pIOColumns = pTable->Columns();
		int nCount = pIOColumns->ColumnCount();

		int nSelectIndex(0);
		int nCodeColumnCount(0);
		for (int i=0; i <nCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);

// 			AutoType itFind = mapZoneColumnGroupKey.find(pIOColumn->Name());
// 			if (itFind == mapZoneColumnGroupKey.end())
// 				continue;

// 			if (pIOColumn->CodeType() != KEMIOCodeTypeIsSingleCode || pIOColumn->ColumnType() != KEMIOColumnTypeUserDefine)
// 				continue;
			if (pIOColumn->CodeType() != KEMIOCodeTypeIsSingleCode)
				continue;
			
// 			int nGroupKey = itFind->second;
// 			KCodeGroup* pCodeGrup = m_pTarget->CodeManager()->FindCodeGroup(nGroupKey);
			KCodeGroup* pCodeGrup = m_pTarget->CodeManager()->FindCodeGroup(pIOColumn->CodeGroup());
			if (nullptr == pCodeGrup)
			{
				continue;
			}

			TCodeGoupColumn oTCodeGoupColumn;
			oTCodeGoupColumn.pIOColumn = pIOColumn;
			oTCodeGoupColumn.pCodeGroup = pCodeGrup;

			int nIndex = m_cboGroupField.AddString(pIOColumn->Caption());
			m_mapCodeGoupColumn.insert(std::make_pair(nIndex, oTCodeGoupColumn));

			if (strPreSelName.CompareNoCase(pIOColumn->Name()) == 0)
			{
				nSelectIndex = nIndex;
			}

			nCodeColumnCount++;
		}

		if (nCodeColumnCount > 0)
		{
			m_cboGroupField.SetCurSel(nSelectIndex);
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


void KODGroupGenerationDlg::LoadReportData()
{
	try
	{
		m_ctrlReport.ResetContent();

		int nCurSel = m_cboGroupField.GetCurSel();  
		if (nCurSel < 0)
			return;

		std::map<int, CString> mapCode;

		AutoType itFind = m_mapCodeGoupColumn.find(nCurSel);
		if (itFind != m_mapCodeGoupColumn.end())
		{
			TCodeGoupColumn &oTCodeGoupColumn = itFind->second;
			KCodeGroup*      pCodeGroup       = oTCodeGoupColumn.pCodeGroup;
			
			pCodeGroup->GetCodes(mapCode);
		}
		else
		{
			return;
		}

		std::map<int, CString>::iterator iter = mapCode.begin();
		std::map<int, CString>::iterator end  = mapCode.end();

		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		struct TCodeCaption
		{
			int     nCode;
			CString strCodeName;
		};

		std::vector<TCodeCaption> vecCodeCaption;

		int nIndex(0);
		while (iter != end)
		{
			TCodeCaption oCodeCaption;
			oCodeCaption.nCode       = iter->first;
			oCodeCaption.strCodeName = iter->second;
			vecCodeCaption.push_back(oCodeCaption);

			nIndex++;
			++iter;
		}

		int nCodeValue(1);
		size_t nxCount = vecCodeCaption.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TCodeCaption& oOriginCodeCaption = vecCodeCaption[i];

			for (size_t m=0; m<nxCount; m++)
			{
				pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

				TCodeCaption& oDestCodeCaption = vecCodeCaption[m];

				CString strTemp;
				strTemp.Format(_T("%d"), nCodeValue);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
				pItem->SetCaption(strTemp);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable (FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetItemData((DWORD_PTR)nCodeValue);

				strTemp.Format(_T("%s:%s"), oOriginCodeCaption.strCodeName, oDestCodeCaption.strCodeName);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
				//pItem->SetCaption(strTemp);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable (TRUE);

				strTemp.Format(_T("%d:%s"), oOriginCodeCaption.nCode, oOriginCodeCaption.strCodeName);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
				pItem->SetItemData((DWORD_PTR)oOriginCodeCaption.nCode);
				pItem->SetCaption(strTemp);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable (FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

				strTemp.Format(_T("%d:%s"), oDestCodeCaption.nCode, oDestCodeCaption.strCodeName);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
				pItem->SetItemData((DWORD_PTR)oDestCodeCaption.nCode);
				pItem->SetCaption(strTemp);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable (FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

				nCodeValue++;
			}
		}

		m_ctrlReport.Populate();
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


void KODGroupGenerationDlg::LoadOutColumnName()
{
	try
	{
		CString strPreSelName(_T(""));

		int nPreSel = m_cboOutColumnName.GetCurSel();
		if (nPreSel >=0)
		{
			KIOColumn* pIOColumn = (KIOColumn*)m_cboOutColumnName.GetItemData(nPreSel);
			try // delete Coloumn이후 m_cboOutColumnName에 반영이 안되어
			{
				strPreSelName = pIOColumn->Name();
			}
			catch (...)
			{
				TxLogDebugException();	
				strPreSelName = _T("");
			}
		}

		m_cboOutColumnName.ResetContent();

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
		const 
			KIOColumns* pIOColumns = pTable->Columns();
		int nCount = pIOColumns->ColumnCount();

		int nSelectIndex(0);
		int nCodeColumnCount(0);
		for (int i=0; i <nCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);

			if (pIOColumn->CodeType() != KEMIOCodeTypeIsSingleCode || pIOColumn->ColumnType() != KEMIOColumnTypeUserDefine)
				continue;

			int nIndex = m_cboOutColumnName.AddString(pIOColumn->Caption());
			m_cboOutColumnName.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			if (strPreSelName.CompareNoCase(pIOColumn->Name()) == 0)
			{
				nSelectIndex = nIndex;
			}
			nCodeColumnCount++;
		}

		if (nCodeColumnCount > 0)
		{
			m_cboOutColumnName.SetCurSel(nSelectIndex);
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


void KODGroupGenerationDlg::OnBnClickedRadio( UINT nID )
{
	ControlRadio();
}


void KODGroupGenerationDlg::ControlRadio()
{
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		GetDlgItem(IDC_EDIT_COLUMN_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_COLUMN_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	}
}


void KODGroupGenerationDlg::OnCbnSelchangeCboGroupField()
{
	LoadReportData();
}


void KODGroupGenerationDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


unsigned __stdcall KODGroupGenerationDlg::ThreadCaller( void* p )
{
	try
	{
		QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
		QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

		KODGroupGenerationDlg* pGroupGeneration = (KODGroupGenerationDlg*)pParameter->GetParameter();
		pGroupGeneration->Run();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return 0;
}


void KODGroupGenerationDlg::Run()
{
	try
	{
		m_mapResult.clear();

		int      nCurSel = m_cboGroupField.GetCurSel();  
		AutoType itFind  = m_mapCodeGoupColumn.find(nCurSel);

		TCodeGoupColumn oTCodeGoupColumn;
		if (itFind != m_mapCodeGoupColumn.end())
		{
			oTCodeGoupColumn = itFind->second;
			KIOColumn*      pIOColumn         = oTCodeGoupColumn.pIOColumn;
			KIOTable*       pTable            = m_pTarget->Tables()->FindTable(TABLE_ZONE);

			KBulkDBase::BulkZoneSelect( pTable, pIOColumn->Name(), m_mapResult);
		}

		KeyCodeName();

		KIOColumn*  pOutColumn    = nullptr;
		CString     strOutColumnName(_T(""));
		KIOTable*   pTable        = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
		bool        bNewColumn(true);

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			GetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);
			strOutColumnName.Trim();
			strOutColumnName.Replace(_T(" "), _T(""));

			const KIOColumns* pColumCollection = pTable->Columns();
			int               nColCnt          = pColumCollection->ColumnCount();
			for (int i= 0; i< nColCnt; i++)
			{
				KIOColumn*    pIOColumn        = pColumCollection->GetColumn(i);
				CString       strCaption       = pIOColumn->Caption();

				if (strOutColumnName.CompareNoCase(strCaption) == 0)
				{
					bNewColumn = false;
					pOutColumn = pIOColumn;
					break;
				}
			}
		}
		else
		{
			bNewColumn = false;

			int nCur   = m_cboOutColumnName.GetCurSel();
			pOutColumn = (KIOColumn*)m_cboOutColumnName.GetItemData(nCur);
		}

		QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
		if (spProgressWindow)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				spProgressWindow->SetStatus(_T("O/D 그룹 생성 모듈 실행 중..."));
			}
			else {
				spProgressWindow->SetStatus(_T("Run O/D Group Generation Module..."));
			}
		}

		int nCodeGroupKey = AddCodeGroup();
		if (nCodeGroupKey< 0)
		{
			return;
		}

		if ( bNewColumn )
		{
			pOutColumn = AddParaODCodeColumn(nCodeGroupKey, strOutColumnName);                 
		}
		else
		{
			KIOColumn oIOColumnUpdate = *pOutColumn;

			if (pOutColumn->CodeGroup() != nCodeGroupKey) // 변경이 일어 날시
			{
				try
				{
					//pTable->BeginTransaction();

					oIOColumnUpdate.CodeGroup(nCodeGroupKey);

					pTable->UpdateColumn(oIOColumnUpdate);
					//pTable->Commit();
				}
				catch (...)
				{
					TxLogDebugException();	
					//pTable->Rollback();
				}
			}
		}

		KTableEditFieldInfo::DefaultCodeFiledData(pTable, *pOutColumn);
		UpdateParaODTable(pOutColumn, m_mapResult);   
		int nZoneGoupKey = oTCodeGoupColumn.pCodeGroup->CodeGroupKey();
		UpdateResultTable(pOutColumn, nCodeGroupKey);

		m_nRunResult = 1;
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


void KODGroupGenerationDlg::OnBnClickedOk()
{
	
	try
	{
		if (InputCheck() == false)
			return;

		m_nRunResult = 0;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, false, nLang); 

		if (m_nRunResult != 1)
		{
			AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
			return;
		}
		try
		{
			CWaitCursor cw;

			KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
			pTable->Notify();

			ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_PARAMETER_OD);            
		}
		catch (...)
		{
			TxLogDebugException();
		}

		KResizeDialogEx::OnOK();
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


bool KODGroupGenerationDlg::InputCheck()
{
	try
	{

		if ( m_cboGroupField.GetCurSel() < 0)
		{
			AfxMessageBox(_T("존 그룹 컬럼을 설정해 주세요."));
			GetDlgItem(IDC_CBO_GROUP_NAME)->SetFocus();
			return false;
		}

		int nCurSel = m_cboGroupField.GetCurSel();  
		AutoType itFind = m_mapCodeGoupColumn.find(nCurSel);
		if (itFind == m_mapCodeGoupColumn.end())
		{
			AfxMessageBox(_T("존 그룹 컬럼을 설정해 주세요."));
			GetDlgItem(IDC_CBO_GROUP_NAME)->SetFocus();
			return false;
		}

		CString strODGroupName(_T(""));
		GetDlgItemText(IDC_EDIT_ODGROUP_NAME, strODGroupName);
		strODGroupName.Trim();
		SetDlgItemText(IDC_EDIT_ODGROUP_NAME, strODGroupName);

		if (strODGroupName.GetLength() == 0)
		{
			AfxMessageBox(_T("O/D 코드 그룹 명칭을 입력해 주세요."));
			GetDlgItem(IDC_EDIT_ODGROUP_NAME)->SetFocus();
			return false;
		}

		std::vector<KCodeGroup*> vecCodeGroup;
		KCodeManager* pCodeManager = m_pTarget->CodeManager();
		pCodeManager->GetCodeGroups(vecCodeGroup);

		size_t nxCount = vecCodeGroup.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KCodeGroup* pCodeGroup       = vecCodeGroup[i];
			CString     strCodeGroupName = pCodeGroup->CodeGroupName();

			if (strCodeGroupName.CompareNoCase(strODGroupName) == 0)
			{
				AfxMessageBox(_T("입력한 명칭의 그룹이 이미 존재합니다.\n다시 입력해 주세요."));
				GetDlgItem(IDC_EDIT_ODGROUP_NAME)->SetFocus();
				return false;
			}
		}

		CString strOutColumnName(_T(""));
		if ( GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1 )
		{
			GetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);
			strOutColumnName.Trim();
			SetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);

			if (strOutColumnName.GetLength() == 0)
			{
				AfxMessageBox(_T("컬럼명을 입력해 주세요."));
				GetDlgItem(IDC_EDIT_COLUMN_NAME)->SetFocus();
				return false;
			}

			KIOTable*         pTable           = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
			const KIOColumns* pColumCollection = pTable->Columns();
			int               nColCnt          = pColumCollection->ColumnCount();
			for (int i= 0; i< nColCnt; i++)
			{
				KIOColumn*    pIOColumn        = pColumCollection->GetColumn(i);
				CString       strCaption       = pIOColumn->Caption();

				if (strOutColumnName.CompareNoCase(strCaption) == 0)
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s' 컬럼이 이미 존재합니다.\r\n덮어쓰시겠습니까?"), strCaption);
					if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
					{
						GetDlgItem(IDC_EDIT_COLUMN_NAME)->SetFocus();
						return false; 
					}
				}
			}
		}
		else
		{
			int nIndex = m_cboOutColumnName.GetCurSel();
			if (nIndex < 0)
			{
				AfxMessageBox(_T("컬럼을 선택해 주세요."));
				m_cboOutColumnName.SetFocus();
				return false;
			}
		}

		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
	return false;
}


void KODGroupGenerationDlg::KeyCodeName()
{
	try
	{
		m_mapKeyCodeName.clear();

		CXTPReportRecords*  pRecords = m_ctrlReport.GetRecords();
		int nRecordsCount = pRecords->GetCount();

		CXTPReportRecord*         pRecord   = NULL;
		CXTPReportRecordItemText* pItemText = NULL;
		CString strValue;
		for (int i=0; i<nRecordsCount; i++)
		{
			pRecord   = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			strValue  = pItemText->GetValue();
			int nCode = _ttoi(strValue);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			strValue  = pItemText->GetValue();
			CString strName = strValue;

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			strValue  = pItemText->GetValue();
			int nOriginCode = _ttoi(strValue);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(3);
			strValue  = pItemText->GetValue();
			int nDestCode = _ttoi(strValue);

			KIntKey   oKey(nOriginCode, nDestCode);
			TCodeName oCodeName;
			oCodeName.m_nCode   = nCode;
			oCodeName.m_strName = strName;

			m_mapKeyCodeName.insert(std::make_pair(oKey, oCodeName));
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


int KODGroupGenerationDlg::AddCodeGroup()
{
	int nCodeGroup(0);
	try
	{
		CString strGroupCodeName(_T(""));
		GetDlgItemText(IDC_EDIT_ODGROUP_NAME, strGroupCodeName);
		strGroupCodeName.Trim();
		strGroupCodeName.Replace(_T(" "), _T(""));

		KCodeManager* pCodeManager = m_pTarget->CodeManager();
		int nNewCodeGroupKey = pCodeManager->NextGroupKey();

		if ( pCodeManager->AddCodeGroup(nNewCodeGroupKey, strGroupCodeName) == false)
		{
			QBicMessageBox::MessageBox(_T("코드 생성에 실패하였습니다."), _T("Information"));
			throw;
		}

		KCodeGroup* pCodeGrup = m_pTarget->CodeManager()->FindCodeGroup(nNewCodeGroupKey);
		nCodeGroup = pCodeGrup->CodeGroupKey();

		std::map<KIntKey, TCodeName>::iterator iter = m_mapKeyCodeName.begin();
		std::map<KIntKey, TCodeName>::iterator end  = m_mapKeyCodeName.end();

		while (iter != end)
		{
			TCodeName& oCodeName = iter->second;
			pCodeGrup->AddCode(oCodeName.m_nCode, oCodeName.m_strName);
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return -1;
	}
	catch (...)
	{
		TxLogDebugException();
		return -1;
	}

	return nCodeGroup;
}


KIOColumn* KODGroupGenerationDlg::AddParaODCodeColumn( int a_nCodeGroup, CString a_strColumnName )
{
	KIOColumn* pColumn = nullptr;
	try
	{
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);

		const KIOColumns* pColumCollection = pTable->Columns();

		int nMaxDisplayCount(1);
		int nCount = pColumCollection->ColumnCount();

		if (0 == nCount)
		{
			nMaxDisplayCount = 1;
		}
		else
		{
			for (int i = 0; i < nCount; i++)
			{
				KIOColumn* pColumn = pColumCollection->GetColumn(i);
				int nDisplayCount = pColumn->DisplayOrder();
				if (nMaxDisplayCount < nDisplayCount)
				{
					nMaxDisplayCount = nDisplayCount;
				}
			}
		}

		KIOColumn oColumn;
		oColumn.Name(pTable->GetUsableColumnName());
		oColumn.Caption(a_strColumnName);
		oColumn.CodeGroup(a_nCodeGroup);
		oColumn.CodeType(KEMIOCodeTypeIsSingleCode);
		oColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oColumn.DataType(KEMIODataTypeInteger);
		oColumn.DisplayOrder(nMaxDisplayCount+1);
		oColumn.ColumnInfo(a_strColumnName);
		oColumn.PrimaryKey(false);
		oColumn.Visible(true);

		pColumn = pTable->AddColumn(oColumn, true);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw;
	}

	return pColumn;
}


void KODGroupGenerationDlg::UpdateParaODTable( KIOColumn* a_pColumn, std::map<Integer, int>& ar_mapValue )
{
	try
	{
		struct ZoneCode
		{
			Integer ZoneID;
			int     Code;
		};

		std::vector<ZoneCode> vecZoneCode;

		std::map<Integer, int>::iterator iter = ar_mapValue.begin();
		std::map<Integer, int>::iterator end  = ar_mapValue.end();

		while (iter != end)
		{
			ZoneCode oZoneCode;
			oZoneCode.ZoneID = iter->first;
			oZoneCode.Code   = iter->second;

			vecZoneCode.push_back(oZoneCode);

			++iter;
		}

		std::map<KIntKey, TCodeName>::iterator iterFind;
		std::map<KODKey, int> mapParaResult;
		size_t nxCount = vecZoneCode.size();
		for (size_t i=0; i<nxCount; i++)
		{
			ZoneCode& oOriginZoneCode = vecZoneCode[i];
			for (size_t j=0; j<nxCount; j++)
			{
				ZoneCode& oDestZoneCode = vecZoneCode[j];

				KODKey oKey(oOriginZoneCode.ZoneID, oDestZoneCode.ZoneID);
				iterFind = m_mapKeyCodeName.find(KIntKey(oOriginZoneCode.Code, oDestZoneCode.Code));
				if (iterFind != m_mapKeyCodeName.end())
				{
					TCodeName& oCodeName = iterFind->second;
					mapParaResult.insert(std::make_pair(oKey, oCodeName.m_nCode));
				}
				else
				{
					mapParaResult.insert(std::make_pair(oKey, 0));
				}
			}
		}

		nxCount = mapParaResult.size();
		if (nxCount == 0)
			return;

		KIOTable*   pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
		KIOColumns oColumns;
		oColumns.AddNewColumn(a_pColumn);

		QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
		if (spProgressWindow)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				spProgressWindow->SetStatus(_T("데이터 저장 중..."));
			}
			else {
				spProgressWindow->SetStatus(_T("Database Insert..."));
			}
		}

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		try
		{
			spDBaseConnection->BeginTransaction();
			KxBulkDbase oBulkDBase(spDBaseConnection, pTable, &oColumns);

			std::map<KODKey, int>::iterator iter = mapParaResult.begin();
			std::map<KODKey, int>::iterator end  = mapParaResult.end();

			while (iter != end)
			{
				oBulkDBase.ImportData(iter->first, iter->second);
				++iter;
			}

			oBulkDBase.ExecuteFullOuterJoin();
			spDBaseConnection->Commit();
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
			spDBaseConnection->RollBack();
		}
		catch (...)
		{
			TxLogDebugException();
			spDBaseConnection->RollBack();
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


void KODGroupGenerationDlg::UpdateResultTable(KIOColumn* a_pColumn, int a_nODGroupCode)
{
	CString strDelSQL(_T(""));
	strDelSQL.AppendFormat(_T(" DELETE FROM Group_Generation_OD WHERE Output_Column = '%s' "), a_pColumn->Name());
	CString strSelectSQL(_T(" SELECT Max(seq) FROM Group_Generation_OD "));

	KDBaseConPtr   spDBaseConnection  = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		int nMaxSeq(0);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSelectSQL);
		if (spResultSet->Next())
			nMaxSeq = spResultSet->GetValueInt(0) + 1;

		spDBaseConnection->ExecuteUpdate(strDelSQL);

		CString strInsertSQL(_T(""));
		strInsertSQL.Append(_T(" Insert Into Group_Generation_OD (seq, OD_Group_Key, Output_Column) "));
		strInsertSQL.AppendFormat(_T(" Values (%d, %d, '%s') "), nMaxSeq, a_nODGroupCode, a_pColumn->Name());
		spDBaseConnection->ExecuteUpdate(strInsertSQL);

		spDBaseConnection->Commit();
	}
	catch (...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}
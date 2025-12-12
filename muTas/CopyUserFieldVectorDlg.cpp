// CopyUserFieldVectorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CopyUserFieldVectorDlg.h"
#include "afxdialogex.h"
#include "KExRecordItem.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "ExistNameChecker.h"
#include "CustomReportRecordItem.h"

enum
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_USER_FIELD,
	_2_COLUMN_CHECK_NEWFIELD,
	_3_COLUMN_COPY_FIELD_NAME
};

// KCopyUserFieldVectorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCopyUserFieldVectorDlg, KResizeDialogEx)

KCopyUserFieldVectorDlg::KCopyUserFieldVectorDlg(KProject* a_pProject, CString a_strTableName, std::vector<TUserColumnInfo> &a_vecUserColumnInfo, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCopyUserFieldVectorDlg::IDD, pParent)
	, m_pProject(a_pProject)
	, m_strTableName(a_strTableName)
	, m_vecUserColumnInfo(a_vecUserColumnInfo)
	, m_pSelTarget(nullptr)
{

}

KCopyUserFieldVectorDlg::~KCopyUserFieldVectorDlg()
{
}

void KCopyUserFieldVectorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO1,                 m_cboScenario);
	DDX_Control(pDX, IDC_COMBO2,                 m_cboTarget);
}


BEGIN_MESSAGE_MAP(KCopyUserFieldVectorDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1,				&KCopyUserFieldVectorDlg::OnCbnSelchangeComboScenario)
	ON_BN_CLICKED(IDOK,							&KCopyUserFieldVectorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,						&KCopyUserFieldVectorDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO2,				&KCopyUserFieldVectorDlg::OnCbnSelchangeComboTarget)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT,&KCopyUserFieldVectorDlg::OnReportCheckItem)
END_MESSAGE_MAP()


// KCopyUserFieldVectorDlg 메시지 처리기입니다.
void KCopyUserFieldVectorDlg::InitComboScenario()
{
	try
	{
		m_cboScenario.ResetContent();

		std::vector<KScenario*> vecScenario;
#pragma region 시나리오 수집
		KScenario* pScenario = m_pProject->GetBaseScenario();
		vecScenario.push_back(pScenario);

		const KTDScenarioMap& oScenarioMap  = m_pProject->GetScenarioMap();
		KTDScenarioMap::const_iterator iter = oScenarioMap.begin();
		KTDScenarioMap::const_iterator end  = oScenarioMap.end();
		while( iter != end )
		{
			KScenario* pScenario = iter->second;
			vecScenario.push_back(pScenario);

			++iter;
		}
#pragma endregion 시나리오 수집

		for (size_t i= 0; i< vecScenario.size(); i++)
		{
			KScenario* pScenario = vecScenario[i];

			if (nullptr == pScenario)
				continue;

			CString strScenarioName(_T(""));

			const KTDTargetMap &mapTarget = pScenario->GetTargetMap();
			if (mapTarget.size() > 0)
			{
				strScenarioName.Format(_T("%s"), pScenario->GetName());
			}
			else
			{
				strScenarioName.Format(_T("%s(Empty)"), pScenario->GetName());
			}

			int nCur = m_cboScenario.AddString(strScenarioName);
			m_cboScenario.SetItemData(nCur, (DWORD_PTR) pScenario);
		}

		if (m_cboScenario.GetCount() > 0)
		{
			m_cboScenario.SetCurSel(0);
			ControlComboScenario();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_REPORT,		SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR1,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_RIGHT);
		SetResize(IDOK,				SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KCopyUserFieldVectorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if ( m_strTableName.CompareNoCase(TABLE_NODE) == 0 ) {
		SetDlgItemText(IDC_STATIC_NAME, _T("노드"));
	} else if ( m_strTableName.CompareNoCase(TABLE_LINK) == 0 ) {
		SetDlgItemText(IDC_STATIC_NAME, _T("링크"));
	} else if ( m_strTableName.CompareNoCase(TABLE_ZONE) == 0 ) {
		SetDlgItemText(IDC_STATIC_NAME, _T("존"));
	}
 	//SetDlgItemText(IDC_STATIC_NAME, m_strTableName);
	
	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, FALSE);
	InitReportHeader();
	UpdateReportData();
	
	InitComboScenario(); // 순서는 UpdateReportData 뒤에.. 

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KCopyUserFieldVectorDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,            _T("Select"),          40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_1_COLUMN_USER_FIELD,       _T("UserField"),       50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("사용자 컬럼"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_2_COLUMN_CHECK_NEWFIELD,   _T("New"),             40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("새 컬럼"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_3_COLUMN_COPY_FIELD_NAME,  _T("Copy Filed Name"), 70));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("컬럼 명칭"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::UpdateReportData()
{
	try
	{
		m_ctrlReport.ResetContent();

		CXTPReportRecord*       pRecord = nullptr;
		CXTPReportRecordItem*   pItem   = nullptr;
		CString strCopyFieldName(_T(""));

		size_t nxCount = m_vecUserColumnInfo.size();

		for (size_t i= 0; i< nxCount; i++)
		{
			TUserColumnInfo &oTUserColumnInfo = m_vecUserColumnInfo[i];

			CString strUserFieldName = oTUserColumnInfo.strUserFieldName;

			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord);
			
			pItem   = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			if (oTUserColumnInfo.bFunction)
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
			}
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strUserFieldName));
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem   = pRecord->AddItem(new KComboReportRecordItem);
			strCopyFieldName.Format(_T("%s_"), strUserFieldName);
			pItem->SetCaption(strCopyFieldName);
			pItem->SetAlignment(DT_CENTER);
		}

		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::OnCbnSelchangeComboScenario()
{
	ControlComboScenario();
}


void KCopyUserFieldVectorDlg::ControlComboScenario()
{
	try
	{
		InitComboTarget();	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::InitComboTarget()
{
	try
	{
		m_cboTarget.ResetContent();

		int nCurSel = m_cboScenario.GetCurSel();
		if (nCurSel< 0)
		{
			return;
		}

		KScenario* pSelScenario = (KScenario*)m_cboScenario.GetItemData(nCurSel);
		if (nullptr == pSelScenario)
			return;

		const KTDTargetMap &mapTarget = pSelScenario->GetTargetMap();
		if (mapTarget.size()< 1)
		{
			return;
		}

		KTDTargetMap::const_iterator citer  = mapTarget.begin();
		KTDTargetMap::const_iterator citEnd = mapTarget.end();

		while (citer != citEnd)
		{
			KTarget* pTarget = citer->second;
			if(nullptr == pTarget)
			{
				++citer;
				continue;
			}

			KIOTables* pIOTables =  pTarget->Tables();
			if (nullptr == pIOTables)
			{
				++citer;
				continue;
			}

			int nCur = m_cboTarget.AddString(pTarget->GetName());
			m_cboTarget.SetItemData(nCur, (DWORD_PTR) pTarget);

			++citer;
		}

		if (m_cboTarget.GetCount() > 0)
		{
			m_cboTarget.SetCurSel(0);
			ControlComboTarget();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::OnCbnSelchangeComboTarget()
{
	try
	{
		ControlComboTarget();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::ControlComboTarget()
{
	try
	{
		InitTableDataField();
		RedrawComboCopyField();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::InitTableDataField()
{
	try
	{
		m_vecUpdateColumn.clear();

		int nCurSel = m_cboTarget.GetCurSel();
		if (nCurSel < 0)
		{
			return;
		}

		KTarget*    pSelTarget = (KTarget*)m_cboTarget.GetItemData(nCurSel);
		KIOTables*  pIOTables  = pSelTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(m_strTableName);

		KIOColumns* pIOColumns = pIOTable->Columns();
		
		for (int i= 0; i< pIOColumns->ColumnCount(); i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
			pIOColumn->DataType();

			if( pIOColumn->ColumnType() != KEMIOColumnTypeUserDefine || pIOColumn->DataType() != KEMIODataTypeDouble)
			{
				continue;
			}

			m_vecUpdateColumn.push_back(pIOColumn);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		m_vecUpdateColumn.clear();
	}
}


void KCopyUserFieldVectorDlg::RedrawComboCopyField()
{
	try
	{
		int nUserDifineColumns = (int)m_vecUpdateColumn.size();
		
		CXTPReportRecordItemText* pItemText = nullptr;
		CXTPReportRecordItem*     pItem     = nullptr;
		CXTPReportRecord*         pRecord   = nullptr;
		CXTPReportRecords*        pRecords  = m_ctrlReport.GetRecords();
		int nRecordCnt          = pRecords->GetCount();
		
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem   = pRecord->GetItem(_0_COLUMN_CHECK);
			TUserColumnInfo* pUserColumnInfo = (TUserColumnInfo*)pItem->GetItemData();

			pItem   = pRecord->GetItem(_2_COLUMN_CHECK_NEWFIELD);
			if (pItem->IsChecked() == TRUE)
			{
				continue;
			}

			if (nUserDifineColumns < 1) // Table 에 Update 할 Userdefine Column 이 없음 (무조건 New 유도) 
			{
				pItem   = pRecord->GetItem(_2_COLUMN_CHECK_NEWFIELD);
				pItem->SetChecked(TRUE);

				pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_USER_FIELD);

				CString strCopyFieldName(_T(""));
				strCopyFieldName.Format(_T("%s_"), pItemText->GetValue());

				// Item - 일반 edit
				ChangeItemToNormal(pRecord, strCopyFieldName);
			}
			else
			{
				// Item - combo box 
				ChangeItemToComboBox(pRecord, m_vecUpdateColumn);
			}
		}

		m_ctrlReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::ChangeItemToNormal(CXTPReportRecord* a_pRecord, CString a_strDefaultCaption)
{
	try
	{
		KComboReportRecordItem*          pItemCombo   = (KComboReportRecordItem*)a_pRecord->GetItem(_3_COLUMN_COPY_FIELD_NAME);
		CXTPReportRecordItemEditOptions* pEditOptions = pItemCombo->GetEditOptions(nullptr); 
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		pConstraints->RemoveAll();
		pEditOptions->RemoveButtons();
		pEditOptions->m_bAllowEdit = TRUE;

		pItemCombo->SetCaption(a_strDefaultCaption);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::ChangeItemToComboBox(CXTPReportRecord* a_pRecord, std::vector<KIOColumn*> &a_vecUpdateColumn)
{
	try
	{
		KComboReportRecordItem*          pItemCombo   = (KComboReportRecordItem*)a_pRecord->GetItem(_3_COLUMN_COPY_FIELD_NAME);
		CXTPReportRecordItemEditOptions* pEditOptions = pItemCombo->GetEditOptions(nullptr); 
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		pConstraints->RemoveAll();
		pEditOptions->RemoveButtons();

		pEditOptions->AddComboButton(TRUE);
		pEditOptions->m_bAllowEdit = TRUE;
		pEditOptions->m_bExpandOnSelect = TRUE;

		for (size_t i= 0; i< a_vecUpdateColumn.size(); i++)
		{
			KIOColumn* pIOColumn            = m_vecUpdateColumn[i];
			CString    strColumnDisplayName = pIOColumn->Caption();

			pEditOptions->AddConstraint(strColumnDisplayName, (DWORD_PTR)pIOColumn);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	try
	{
		CXTPReportRecordItem* pItem   = pItemNotify->pItem;
		CXTPReportRecord*     pRecord = pItem->GetRecord();

		if (_2_COLUMN_CHECK_NEWFIELD == pItem->GetIndex())
		{
			BOOL bChecked = pItem->IsChecked();

			if (!bChecked)
			{
				if (m_vecUpdateColumn.size()< 1)
				{
					pItem->SetChecked(TRUE); // 다시 New Field 모드로
					m_ctrlReport.RedrawControl();

					AfxMessageBox(_T("갱신 가능한 컬럼이 존재하지 않습니다."));

					return;
				}
				else
				{
					ChangeItemToComboBox(pRecord, m_vecUpdateColumn);
					m_ctrlReport.RedrawControl();
				}
			}
			else
			{
				CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_USER_FIELD);
				CString strCopyFieldName(_T(""));
				strCopyFieldName.Format(_T("%s_"), pItemText->GetValue());

				ChangeItemToNormal(pRecord, strCopyFieldName);
				m_ctrlReport.RedrawControl();
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KCopyUserFieldVectorDlg::OnBnClickedOk()
{
	try
	{
		if (false == InvalidateInputInfo())
		{
			return;
		}
		
		std::vector<TUserColumnInfo> vecAddUserColumnInfo;

		CXTPReportRecords*        pRecords  = m_ctrlReport.GetRecords();
		CXTPReportRecord*         pRecord   = nullptr;
		CXTPReportRecordItem*     pItem     = nullptr;
		CXTPReportRecordItemText* pItemText = nullptr;
		int nRecordCnt          = pRecords->GetCount();

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord   = pRecords->GetAt(i);
			pItem     = pRecord->GetItem(_0_COLUMN_CHECK);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			TUserColumnInfo oTAddUserColumnInfo;
			oTAddUserColumnInfo.bFunction = true;

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_USER_FIELD);
			oTAddUserColumnInfo.strUserFieldName = pItemText->GetValue();

			pItem     = pRecord->GetItem(_2_COLUMN_CHECK_NEWFIELD);
			BOOL bNew = pItem->IsChecked();
			oTAddUserColumnInfo.bNew = bNew;
										
			pItem     = pRecord->GetItem(_3_COLUMN_COPY_FIELD_NAME);
			if (bNew) // new 
			{
				CString strCopyFieldName = pItem->GetCaption(NULL);
				strCopyFieldName.Trim();
				oTAddUserColumnInfo.strCopyFieldName = strCopyFieldName;
			}
			else	  // update
			{
				oTAddUserColumnInfo.strCopyFieldName = pItem->GetCaption(NULL);
				KIOColumn* pIOColumnUpdate = (KIOColumn*)pItem->GetItemData();

				oTAddUserColumnInfo.oIOColumnUpdate = *pIOColumnUpdate;
			}
			
			vecAddUserColumnInfo.push_back(oTAddUserColumnInfo);
		}

		m_vecUserColumnInfo.clear();
		m_vecUserColumnInfo.assign(vecAddUserColumnInfo.begin(), vecAddUserColumnInfo.end());

		int        nCurSel   = m_cboTarget.GetCurSel();
		m_pSelTarget         = (KTarget*)m_cboTarget.GetItemData(nCurSel);
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	}

	KResizeDialogEx::OnOK();
}


bool KCopyUserFieldVectorDlg::InvalidateInputInfo()
{
	try
	{
		if (m_cboScenario.GetCurSel() < 0)
		{
			ThrowException(_T("Copy할 대상 Scenario를 선택해 주세요."));
		}

		if (m_cboTarget.GetCurSel() < 0)
		{
			ThrowException(_T("Copy할 대상 Target을 선택해 주세요."));
		}

		std::set<CString>    setAddColumnName;
		std::set<CString>    setUpdateColumnName;

		CXTPReportRecord*         pRecord   = nullptr;
		CXTPReportRecordItem*     pItem     = nullptr;
		CXTPReportRecordItemText* pItemText = nullptr;
		CXTPReportRecords*        pRecords  = m_ctrlReport.GetRecords();
		int nRecordCnt          = pRecords->GetCount();
		int nSelectCnt(0);

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord   = pRecords->GetAt(i);
			pItem     = pRecord->GetItem(_0_COLUMN_CHECK);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			nSelectCnt++;

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_USER_FIELD);
			CString strUserFieldName = pItemText->GetValue();

			pItem     = pRecord->GetItem(_2_COLUMN_CHECK_NEWFIELD);
			BOOL bNew = pItem->IsChecked();
			
			pItem     = pRecord->GetItem(_3_COLUMN_COPY_FIELD_NAME);
			if (bNew) // new 
			{
				CString strCopyFieldName = pItem->GetCaption(NULL);
				strCopyFieldName.Trim();

				if (strCopyFieldName.Compare(_T("")) == 0) // 컬럼 네임이 비었는지 체크
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s'의 Copy Field Name을 입력해 주세요."), strUserFieldName);

					ThrowException(strMsg);
				}

				if (strCopyFieldName.Find(_T(' ')) > -1 ) // 컬럼 네임에 공백이 있는지 체크  
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s'의 Copy Field Name을 공백 없이 입력해 주세요."), strUserFieldName);

					ThrowException(strMsg);
				}

				CString strErrCh(_T(""));
				if (QBicStringChecker::IsValidColumnName(strCopyFieldName, strErrCh) == false) // 입력 불가능한 특수문자가 있는지 체크
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s'의 Copy Field Name에 \'%s\' 문자를 사용할 수 없습니다."), strUserFieldName, strErrCh);

					ThrowException(strMsg);
				}
				
				strCopyFieldName = strCopyFieldName.MakeUpper();
				AutoType itFind = setAddColumnName.find(strCopyFieldName);
				if (itFind != setAddColumnName.end())
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s'의 Copy Field Name이 중복입력 되었습니다. \r\n 다른 Name을 입력해 주세요."), strUserFieldName);

					ThrowException(strMsg);
				}
				else
				{
					setAddColumnName.insert(strCopyFieldName);
				}
			}
			else	  // update
			{
				CString strCopyFieldName = pItem->GetCaption(NULL);

				if (strCopyFieldName.Compare(_T("")) == 0) // 컬럼 네임이 비었는지 체크
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s'의 Copy Field Name을 선택해 주세요."), strUserFieldName);

					ThrowException(strMsg);
				}

				strCopyFieldName = strCopyFieldName.MakeUpper();
				AutoType itFind = setUpdateColumnName.find(strCopyFieldName);
				if (itFind != setUpdateColumnName.end())
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s'의 Copy Field Name이 중복선택 되었습니다. \r\n 다른 Name을 선택해 주세요."), strUserFieldName);

					ThrowException(strMsg);
				}
				else
				{
					setUpdateColumnName.insert(strCopyFieldName);
				}
			}
		}

		if (nSelectCnt< 1)
		{
			ThrowException(_T("Copy 할 User Field를 선택해 주세요"));
		}

		int        nCurSel   = m_cboTarget.GetCurSel();
		KTarget*   pTarget   = (KTarget*)m_cboTarget.GetItemData(nCurSel);
		std::vector<CString> vecWrongColumnName;

		if (InvalidateAddColumn(pTarget, setAddColumnName, vecWrongColumnName) == false)
		{
			nCurSel              = m_cboScenario.GetCurSel();
			KScenario* pScenario = (KScenario*)m_cboScenario.GetItemData(nCurSel);

			CString strMsg(_T(""));
			strMsg.Format(_T("%s_%s_%s 테이블에 해당 Field가 이미 존재합니다.\r\n다른 이름을 입력해 주세요."), pScenario->GetName(), pTarget->GetName(), m_strTableName);

			for (size_t i= 0; i< vecWrongColumnName.size(); i++)
			{
				CString &strWrongColumnName = vecWrongColumnName[i];
				strMsg.AppendFormat(_T("\r\n-'%s'"), strWrongColumnName);
			}

			ThrowException(strMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return false;
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


bool KCopyUserFieldVectorDlg::InvalidateAddColumn(KTarget* a_pTarget, std::set<CString> &a_setAddColumnName, std::vector<CString> &a_vecWrongColumnName)
{
	try
	{
		a_vecWrongColumnName.clear();

		KExistNameChecker oExistNameChecker;

		KIOTables*        pIOTables  = a_pTarget->Tables();
		const KIOTable*   pIOTable   = pIOTables->FindTable(m_strTableName);
		KIOColumns*       pIOColumns = pIOTable->Columns();
		int               nColCount  = pIOColumns->ColumnCount();

		for(int i= 0; i < nColCount; ++i)
		{
			KIOColumn* pColumn    = pIOColumns->GetColumn(i);
			CString    strCaption = pColumn->Caption();

			oExistNameChecker.addName(strCaption);
		}

		AutoType iter  = a_setAddColumnName.begin();
		AutoType itEnd = a_setAddColumnName.end();

		while (iter != itEnd)
		{
			CString strAddColumnName = *iter;
			if (oExistNameChecker.isNameExistsNoCase(strAddColumnName) == true)
			{
				a_vecWrongColumnName.push_back(strAddColumnName);
			}

			++iter;
		}

		if (a_vecWrongColumnName.size() > 0)
		{
			return false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KCopyUserFieldVectorDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

// DefaultTGCrossPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultTGCrossPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationInfo.h"
#include "DBaseGeneration.h"
#include "DBaseConnector.h"
#include "GSimpleExpressionContainer.h"
#include "GClassificationEditorDlg.h"

// KDefaultTGCrossPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultTGCrossPage, CXTPPropertyPage)

KDefaultTGCrossPage::KDefaultTGCrossPage(KTarget* a_pTarget, KGenerationInfo* a_pGenerationInfo)
	: CXTPPropertyPage(KDefaultTGCrossPage::IDD)
	, m_pTarget(a_pTarget)
	, m_pGenerationInfo(a_pGenerationInfo)
{
	 m_pGCategoryExpression = new KGCategoryExpression();
}

KDefaultTGCrossPage::~KDefaultTGCrossPage()
{
	if (m_pGCategoryExpression != nullptr)
	{
		QBicDelete(m_pGCategoryExpression);
	}
}

void KDefaultTGCrossPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SEX, m_chkSex);
	DDX_Control(pDX, IDC_CHECK_AGE, m_chkAge);
	DDX_Control(pDX, IDC_COMBO_AGE_GROUP, m_cboCategoryClass);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KDefaultTGCrossPage, CXTPPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_AGE_GROUP, &KDefaultTGCrossPage::OnCbnSelchangeComboAgeGroup)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &KDefaultTGCrossPage::OnBnClickedButtonEdit)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KDefaultTGCrossPage 메시지 처리기입니다.


BOOL KDefaultTGCrossPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);

	loadCategoryClass();
	InitDefaultData();

// 	if (KDBaseGeneration::IsPopTableNotEmpty(m_pTarget) == false)
// 	{
// 		AfxMessageBox(_T("카테고리분석법을 수행하려면 성별/연령별 인구 테이블에 데이터가 필요합니다."));
// 		return FALSE;
// 	}

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefaultTGCrossPage::loadCategoryClass()
{
	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		CString strSQL = _T(" Select category_id, name, sex_use, age_use from category_class ");

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		m_vecCategoryClass.clear();
		while(spResult->Next())
		{        
			TCategoryClass record;
			record.category_id = spResult->GetValueInt   (0);
			record.name        = spResult->GetValueString(1);
			record.sex_use     = spResult->GetValueInt   (2);
			record.age_use     = spResult->GetValueInt   (3);

			m_vecCategoryClass.push_back(record);
		}

		m_cboCategoryClass.ResetContent();

		for ( size_t i=0; i<m_vecCategoryClass.size(); i++ )
		{
			TCategoryClass& record = m_vecCategoryClass[i];
			int nIndex = m_cboCategoryClass.AddString(record.name );
			m_cboCategoryClass.SetItemData(nIndex, (DWORD_PTR)&record);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDefaultTGCrossPage::loadCategoryAgeGroup( TCategoryClass* pCategory )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL;
		strSQL.Format(_T(" Select seq, min, max from category_age_group where category_id = %d "), pCategory->category_id);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		m_vecAgeRange.clear();
		while(spResult->Next())
		{        
			TAgeRange record;
			record.seq = spResult->GetValueInt(0);
			record.min = spResult->GetValueInt(1);
			record.max = spResult->GetValueInt(2);

			m_vecAgeRange.push_back(record);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDefaultTGCrossPage::loadCategoryValue()
{
	if (m_pGCategoryExpression == NULL)
	{
		return;
	}

	bool isSexUse = m_pCategoryClass->sex_use > 0 ? true : false;
	bool isAgeUse = m_pCategoryClass->age_use > 0 ? true : false;

	std::map <KCategoryKey, double>& categoryUnitValues = m_pGCategoryExpression->m_categoryUnitValues;
	int nColumnCount = m_wndReportCtrl.GetColumns()->GetCount();
	int nRowCount    = m_wndReportCtrl.GetRecords()->GetCount();

	CXTPReportRecord*           pRecord = NULL;
	CXTPReportRecordItemNumber* pItem   = NULL;

	std::map <KCategoryKey, double>::iterator iter ;
	std::map <KCategoryKey, double>::iterator end  = categoryUnitValues.end();

	for ( int iRow=0; iRow < nRowCount; iRow++ )
	{
		pRecord = m_wndReportCtrl.GetRecords()->GetAt(iRow);

		for ( int iCol=1; iCol< nColumnCount; iCol++ )
		{
			pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(iCol);

			KCategoryKey key(iRow+1, iCol);
			if (! isSexUse)
			{
				key.SexKey = -1;
			}
			if (!isAgeUse)
			{
				key.AgeSeq = -1;
			}
			iter = categoryUnitValues.find(key);
			if (iter != end )
			{
				pItem->SetValue(iter->second);
			}
			else
			{
				pItem->SetValue(0.0);
			}
		}
	}

	m_wndReportCtrl.Populate();
}

void KDefaultTGCrossPage::SelchangeComboAgeGroup()
{
	int nIndex = m_cboCategoryClass.GetCurSel();
	TCategoryClass*    pCategory = (TCategoryClass*)m_cboCategoryClass.GetItemData(nIndex);
	m_pCategoryClass = pCategory;

	bool isSexUse = pCategory->sex_use > 0 ? true : false;
	bool isAgeUse = pCategory->age_use > 0 ? true : false;

	m_chkSex.SetCheck(isSexUse);
	m_chkAge.SetCheck(isAgeUse);

	m_wndReportCtrl.ResetContent(TRUE);
	m_wndReportCtrl.GetColumns()->Clear();
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("연령"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);

	if (isSexUse)
	{
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn (1, _T("남자"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn (2, _T("여자"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}
	else
	{
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn (1, _T("남/여"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
	}


	std::map<KODKey, double> mapCategoryValues;
	KDBaseGeneration::GetCategoryClassValue(m_pTarget, m_pCategoryClass->category_id, mapCategoryValues);

	AutoType findend = mapCategoryValues.end();

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	if (isAgeUse)
	{
		loadCategoryAgeGroup(pCategory);
		for (size_t i = 0;  i < m_vecAgeRange.size(); i++)
		{
			TAgeRange& recrod = m_vecAgeRange[i];
			CString strTemp(_T(""));
			strTemp.Format(_T("%d ~ %d"), recrod.min, recrod.max);

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			Integer nxIndex = (Integer)i;

			if (isSexUse)
			{
				KODKey oMale(1, nxIndex);
				KODKey oFeMale(2,nxIndex);

				AutoType findMale	= mapCategoryValues.find(oMale);
				AutoType findFemale	= mapCategoryValues.find(oFeMale);
				if (findMale != findend)
				{
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(findMale->second, _T("%.3f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
				else
				{
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}

				if (findFemale != findend)
				{
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(findFemale->second, _T("%.3f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
				else
				{
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
			}
			else
			{
				KODKey oAll(0,nxIndex);
				AutoType findAll	= mapCategoryValues.find(oAll);

				if (findAll != findend)
				{
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(findAll->second, _T("%.3f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
				else
				{
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetEditable(FALSE);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				}
			}
		}
	}
	else
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("전연령")));
		pItem->SetAlignment(DT_CENTER);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		if (isSexUse)
		{
			KODKey oMale(1, 0);
			KODKey oFeMale(2,0);

			AutoType findMale	= mapCategoryValues.find(oMale);
			AutoType findFemale	= mapCategoryValues.find(oFeMale);
			if (findMale != findend)
			{
				pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(findMale->second, _T("%.3f")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}

			if (findFemale != findend)
			{
				pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(findFemale->second, _T("%.3f")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
		}
		else
		{
			KODKey oAll(0,0);
			AutoType findAll	= mapCategoryValues.find(oAll);

			if (findAll != findend)
			{
				pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(findAll->second, _T("%.3f")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
		}
	}
	m_wndReportCtrl.Populate();
}

int KDefaultTGCrossPage::GetCategoryIndex( int category_id )
{
	int nIndex = -1;
	for ( size_t i=0; i<m_vecCategoryClass.size(); i++ )
	{
		TCategoryClass& record = m_vecCategoryClass[i];
		if (category_id == record.category_id)
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}



void KDefaultTGCrossPage::OnCbnSelchangeComboAgeGroup()
{
	 SelchangeComboAgeGroup();
}

BOOL KDefaultTGCrossPage::OnApply()
{
	try
	{
		if (m_cboCategoryClass.GetCurSel() == -1)
		{
			throw -1;
		}

		m_pGCategoryExpression->m_category_id = m_pCategoryClass->category_id;
		m_pGCategoryExpression->m_useAge      = m_pCategoryClass->age_use;
		m_pGCategoryExpression->m_useSex      = m_pCategoryClass->sex_use;
		m_pGCategoryExpression->m_ageGroup    = m_vecAgeRange;

		bool isSexUse = m_pCategoryClass->sex_use > 0 ? true : false;
		bool isAgeUse = m_pCategoryClass->age_use > 0 ? true : false;

		std::map <KCategoryKey, double>& categoryUnitValues = m_pGCategoryExpression->m_categoryUnitValues;
		categoryUnitValues.clear();

		int nColumnCount = m_wndReportCtrl.GetColumns()->GetCount();
		int nRowCount    = m_wndReportCtrl.GetRecords()->GetCount();

		CXTPReportRecord*           pRecord = NULL;
		CXTPReportRecordItemNumber* pItem   = NULL;

		for ( int iRow=0; iRow < nRowCount; iRow++ )
		{
			pRecord = m_wndReportCtrl.GetRecords()->GetAt(iRow);

			for ( int iCol=1; iCol< nColumnCount; iCol++ )
			{
				pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(iCol);
				double dValue = pItem->GetValue();

				KCategoryKey key(iCol, iRow+1);
				if (! isSexUse)
				{
					key.SexKey = -1;
				}
				if (!isAgeUse)
				{
					key.AgeSeq = -1;
				}

				categoryUnitValues.insert(std::make_pair(key, dValue));
			}
		}

		KGSimpleExpressionContainer* pContainer = 
			(KGSimpleExpressionContainer*)m_pGenerationInfo->ExpressionContainer();
		if(nullptr == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(m_pGenerationInfo->CreateExpression());
		}

		KGCategoryExpression* pExpression = (KGCategoryExpression*)(pContainer->ExpressionObject());
		if(nullptr == pExpression)
		{
			pExpression = (KGCategoryExpression*)(pContainer->CreateExpressionObject());
		}

		*pExpression = *m_pGCategoryExpression;

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	UpdateData(TRUE);


	
	CXTPPropertyPage::OnApply();
	return TRUE;
}

void KDefaultTGCrossPage::InitDefaultData()
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = m_pGenerationInfo->ExpressionContainer();
		if(NULL == pBaseCont)
		{
			return;
		}

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if(NULL == pBaseExp)
		{
			return;
		}

		*m_pGCategoryExpression = *(KGCategoryExpression*)(pBaseExp);

		if (m_cboCategoryClass.GetCount() > 0)
		{
			int nIndex = GetCategoryIndex(m_pGCategoryExpression->m_category_id);
			if (nIndex >= 0)
			{
				m_cboCategoryClass.SetCurSel(nIndex);
			}
			else
			{
				m_cboCategoryClass.SetCurSel(0);
			}

			SelchangeComboAgeGroup();
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


void KDefaultTGCrossPage::OnBnClickedButtonEdit()
{
	KGClassificationEditorDlg oDlg(m_pTarget);
	if (oDlg.DoModal()== IDOK)
	{
		loadCategoryClass();
		if (m_cboCategoryClass.GetCount() > 0)
		{
			int nClassID = oDlg.GetClassID();
			if (nClassID != 0)
			{
				int nIndex = GetCategoryIndex(nClassID);
				if (nIndex >= 0)
				{
					m_cboCategoryClass.SetCurSel(nIndex);
				}
				else
				{
					m_cboCategoryClass.SetCurSel(0);
				}

				SelchangeComboAgeGroup();
			}
		}
		else
		{
			AfxMessageBox(_T("카테고리분석법을 수행하기 위해 먼저 카테고리 분류를 해야 합니다."));
			return;
		}
	}
}


void KDefaultTGCrossPage::OnSize(UINT nType, int cx, int cy)
{
	CXTPPropertyPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KDefaultTGCrossPage::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC,			SZ_TOP_LEFT,				SZ_TOP_LEFT);
		SetResize(IDC_COMBO_AGE_GROUP,	SZ_TOP_LEFT,				SZ_TOP_LEFT);
		SetResize(IDC_BUTTON_EDIT,		SZ_TOP_LEFT,				SZ_TOP_LEFT);
		SetResize(IDC_CHECK_SEX,		SZ_TOP_LEFT,				SZ_TOP_LEFT);
		SetResize(IDC_CHECK_AGE,		SZ_TOP_LEFT,				SZ_TOP_LEFT);

		SetResize(IDC_REPORT,			SZ_TOP_LEFT,				SZ_BOTTOM_RIGHT);
		
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

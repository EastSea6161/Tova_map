// StatisticsBaseDefineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StatisticsBaseDefineDlg.h"
#include "KStatisticsDlg.h"
#include "StatisticsFunctionDefineDlg.h"
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"
#include "GRegressionExpression.h"
#include "GenerationInfo.h"
#include "KDBaseStatistics.h"
//^#include "DBaseConnector.h"
//^^ #include "TableOwner.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
//^^ #include "IOItem.h"
#include "KExRecordItem.h"
//^#include "DevideDataInterval.h"
#include "ParseExpression.h"
enum
{
	_0_COLUMN_CHECK = 0,    
	_1_COLUMN_NAME
};

// KStatisticsBaseDefineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KStatisticsBaseDefineDlg, CXTPResizeDialog)

KStatisticsBaseDefineDlg::KStatisticsBaseDefineDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KStatisticsBaseDefineDlg::IDD, pParent)
	, m_strFunction(_T(""))
	, m_bChkZero(TRUE)
	, m_bChkGroup(FALSE)
{
	m_pSelectedColumns = new KIOColumns();
	m_pExpression      = new KGRegressionExpression();
	m_strFilter = _T("");
}

KStatisticsBaseDefineDlg::~KStatisticsBaseDefineDlg()
{
	delete m_pSelectedColumns;
	delete m_pExpression;
}

void KStatisticsBaseDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Text(pDX, IDC_EDIT_FUNCTION, m_strFunction);
	DDX_Check(pDX, IDC_CHECK1, m_bChkZero);
	DDX_Check(pDX, IDC_CHECK2, m_bChkGroup);
	DDX_Control(pDX, IDC_COMBO1, m_cboGroup);
}


BEGIN_MESSAGE_MAP(KStatisticsBaseDefineDlg, CXTPResizeDialog)
	ON_BN_CLICKED(IDOK, &KStatisticsBaseDefineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_FUNCTION, &KStatisticsBaseDefineDlg::OnBnClickedBtnFunction)
	ON_COMMAND(ID_SELECT_CANCEL, &KStatisticsBaseDefineDlg::OnSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KStatisticsBaseDefineDlg::OnSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT, OnReportColumnRButtonClick)
	ON_BN_CLICKED(IDC_CHECK2, &KStatisticsBaseDefineDlg::OnBnClickedCheckGroupFiled)
END_MESSAGE_MAP()


// KStatisticsBaseDefineDlg 메시지 처리기입니다.

BOOL KStatisticsBaseDefineDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();    

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if(m_pTable != NULL)
	{
		SetDlgItemText(IDC_STATIC_NAME, m_pTable->Caption());

		CString strColumnGroup = _T("");
		strColumnGroup.Format(_T(" %s "), m_pTable->Caption());
		SetDlgItemText(IDC_STATIC_COLUMN, strColumnGroup);
	}

	if(m_pTarget != NULL)
	{
		CString strTargetYear = _T("");
		strTargetYear.Format(_T("[ %s ]"), m_pTarget->Caption());
		SetDlgItemText(IDC_STATIC_TARGETYEAR, strTargetYear);
	}

	InitReportHeader();
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);    
    KReportCtrlSetting::Default(m_wndReportCtrl, FALSE, FALSE, FALSE);
	InitReportData();
	m_wndReportCtrl.Populate();

	InitGroupFiled();
	m_cboGroup.SetCurSel(0);

	DisableGroupFiled(m_bChkGroup);

	SetResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KStatisticsBaseDefineDlg::SetResizeComponent()
{
	SetResize(IDC_STATIC_COLUMN,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT,           SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_FUNCTION,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT_FUNCTION,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BTN_FUNCTION,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK1,           SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK2,           SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO1,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,             SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KStatisticsBaseDefineDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	//m_wndReportCtrl.FocusSubItems(TRUE);
	m_wndReportCtrl.SelectionEnable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME,       _T("Column"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼명"));
    }

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(TRUE);
}


void KStatisticsBaseDefineDlg::InitReportData()
{
	if(m_pTable == NULL)
	{
		return;
	}

	const KIOColumns* pColumns = m_pTable->Columns();

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	m_wndReportCtrl.ResetContent(TRUE);
	for(int i= 0; i< pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

        if (pColumn->PrimaryKey())
            continue;

        if(/*pColumn->CodeType() == KEMIOCodeTypeIsSingleCode ||*/ pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
        {
            continue;
        }

        if( KEMIODataTypeString == pColumn->DataType())
        {
            continue;
        }

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
	
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(pColumn->Caption()));
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(xtpColumnTextLeft);
		pItem->SetItemData((DWORD_PTR) pColumn);
	}
}


void KStatisticsBaseDefineDlg::InitGroupFiled()
{
	const KIOColumns* pColumns = m_pTable->Columns();

	for(int i= 0; i< pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		if( pColumn->PrimaryKey() == true )
		{
			continue;
		}

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		/*if( pColumn->DataType() == KEMIODataTypeInteger && pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)*/
		{
			int nCur = m_cboGroup.AddString(pColumn->Caption());
			m_cboGroup.SetItemData(nCur, DWORD_PTR(pColumn));
		}
	}

	if(m_cboGroup.GetCount() < 1)
	{
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		m_bChkGroup = FALSE;
		UpdateData(FALSE);
	}
}


void KStatisticsBaseDefineDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_pSelectedColumns->Clear();

	CXTPReportRow*        pRow    = NULL;
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL; 

	int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; ++i)
	{
		pRow = m_wndReportCtrl.GetRows()->GetAt(i);
		pRecord = pRow->GetRecord();
		
		pItem   = pRecord->GetItem(_0_COLUMN_CHECK);
		if( FALSE == pItem->IsChecked())
		{
			continue;
		}

		pItem   = pRecord->GetItem(_1_COLUMN_NAME);
		KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();

		m_pSelectedColumns->AddNewColumn(pColumn);
	}

	if(m_pSelectedColumns->ColumnCount() < 1 && m_strFunction.Compare(_T("")) == 0)
	{
		AfxMessageBox(_T("컬럼을 선택하거나 수식을 설정해 주세요."));
		return;
	}

	KStatisticsDlg dlg;
	dlg.SetInitValue  (m_pTable, m_pSelectedColumns, m_strFilter);
	
	m_pGroupColumn = GetCurGroupField();
	dlg.SetGroupColumn(m_pGroupColumn);


	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}
	QBicSimpleProgressThread::ExecuteThread(StatisticThreadCaller, this, false, nLang);

	dlg.SetNormalInfo(m_vecNormalInfo);
	dlg.SetGroupInfo(m_vecGroupInfo);
	dlg.SetNormalExpressionInfo(m_vecExpressionInfo);
	dlg.SetGroupExpressionInfo(m_vecGroupExpressionInfo);

	dlg.DoModal();
	
	/*CXTPResizeDialog::OnOK();*/
}


unsigned __stdcall KStatisticsBaseDefineDlg::StatisticThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 통계량 연산 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Calculating for Statistics..."));
	}

	KStatisticsBaseDefineDlg* pDlg = (KStatisticsBaseDefineDlg*)pParameter->GetParameter();

	pDlg->GetSourceData();

	return 0;
}


void KStatisticsBaseDefineDlg::GetSourceData()
{
	if(NULL == m_pGroupColumn)
	{
		GetNormalSourceData();
	}
	else
	{
		GetGroupSourceData();
	}
}


void KStatisticsBaseDefineDlg::GetNormalSourceData()
{
	if(NULL == m_pSelectedColumns)
	{
		return;
	}

	// 일반 데이터
	m_vecNormalInfo.clear();
	for (int i=0; i<m_pSelectedColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = m_pSelectedColumns->GetColumn(i);
		std::vector<double> vecData;
		// 1. Get DB value 
		if( false == GetNormalData(pColumn, vecData) )
		{
			continue;
		}

		if(vecData.size() < 1)
		{
			continue;
		}

		TSingleNormalInfo oTSingleNormalInfo;
		oTSingleNormalInfo.strCaption = pColumn->Caption();

		// 2. caculate statistic
		KDevideDataInterval &oDevideDataInterval = oTSingleNormalInfo.oDevideDataInterval;
		oDevideDataInterval.SetDataCalculate(vecData);
		
		m_vecNormalInfo.push_back(oTSingleNormalInfo);
	}    

	// 수식 데이터
	m_vecExpressionInfo.clear();

	std::vector<double> vecExpressionData;
	if(true == GetExpressionNormalData(vecExpressionData))
	{
		if(vecExpressionData.size() > 0)
		{
			TSingleExpressionInfo oTSingleExpressionInfo;
			oTSingleExpressionInfo.strCaption              = m_pExpression->GetCaptionExpression();

			KDevideDataInterval &oDevideDataInterval = oTSingleExpressionInfo.oDevideDataInterval;
			oDevideDataInterval.SetDataCalculate(vecExpressionData);
			
			m_vecExpressionInfo.push_back(oTSingleExpressionInfo);
		}
	}
	else
	{
		CString strCaption = m_pExpression->GetCaptionExpression();
		if(strCaption.Compare(_T("")) != 0)
		{
			TSingleExpressionInfo oTSingleExpressionInfo;
			oTSingleExpressionInfo.strCaption              = m_pExpression->GetCaptionExpression();

			m_vecExpressionInfo.push_back(oTSingleExpressionInfo);
		}
	}
}


bool KStatisticsBaseDefineDlg::GetNormalData(KIOColumn* pColumn, std::vector<double> &a_vecData)
{
	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}
	if(m_bChkZero == FALSE)
	{
		strWhere += " AND ";
		strWhere += pColumn->Name();
		strWhere += " != 0 ";
	}

	strSQL.Format (_T(" SELECT %s FROM %s %s "), pColumn->Name(), m_pTable->Name(), strWhere );

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	while( pResult->Next() )
	{
		double dValue = pResult->GetValueDouble(0);
		a_vecData.push_back(dValue);
	}
	return true;
}


bool KStatisticsBaseDefineDlg::GetExpressionNormalData( std::vector<double> &a_vecData )
{
	KIOColumns oReqColumns;
	KIOColumn* pColumn = NULL;

    KParseExpression oParseExpression;
    oParseExpression.SetTable(m_pTable);
    oParseExpression.SetCaptionExpression(m_pExpression->GetCaptionExpression());
    std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();

    CString strFunction = oParseExpression.GetRealExpression();
    size_t nxCount = vecUsedColumnName.size();
    for (size_t i=0; i<nxCount; i++)
    {
        pColumn = oReqColumns.AddNewColumn();
        pColumn->Name(vecUsedColumnName[i]);
    }

    /*
	const std::list<KExpressionElement*>& elementList = m_pExpression->GetExpressionList();
	std::list<KExpressionElement*>::const_iterator itElement, itEnd = elementList.end();
	for(itElement = elementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		if(pElement->ElementType() == KEMExpressionElementField)
		{
			KExpressionElementField* pField = (KExpressionElementField*)pElement;
			pColumn = oReqColumns.AddNewColumn();
			pColumn->Name(pField->FieldName());
			pColumn->Caption(pField->FieldCaption());
		}
	}
    */
	int nReqColumnCount = oReqColumns.ColumnCount();

	KParseVariables       variables;
	KParseVariableInfo*   pVarInfo = NULL;
	CString               strColumnName =_T("");

	for(int j = 0; j < nReqColumnCount; ++j)
	{
		pVarInfo = variables.AddNewVariable();
		pColumn = oReqColumns.GetColumn(j);
		pVarInfo->SetVariable(pColumn->Name(), j);

		strColumnName += ( pColumn->Name() );

		if(j < nReqColumnCount -1)
		{
			strColumnName += ( _T(", ") );
		}
	}		

	KSDoubleRecordArray oRecordArray(nReqColumnCount);
	KSDoubleRecord*     pRecord = NULL;
	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

    if (nReqColumnCount > 0)
    {
        CString strSQL(_T(""));
        CString strWhere(_T(" Where 1 = 1 "));

        if (m_strFilter.GetLength() > 0)
        {
            strWhere += " AND ";
            strWhere += m_strFilter;
        }

        strSQL.Format (_T(" SELECT %s FROM %s %s "), strColumnName, m_pTable->Name(), strWhere);

        KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
        if(pResult == NULL) return false;

        while( pResult->Next() )
        {
            pRecord = oRecordArray.AddNewRecord();
            for(int j = 0; j < nReqColumnCount; ++j)
            {
                pRecord->SetAt( j, pResult->GetValueDouble(j) );
            }
        }
    }
	//CString     strFunction = m_pExpression->GetExpression();
	KGeneration gen;
	int nRet = gen.MuParseExecute(strFunction, variables, oRecordArray, a_vecData, NULL);
	if(1 != nRet)
	{
		return false;
	}

	return true;
}


void KStatisticsBaseDefineDlg::GetGroupSourceData()
{
	if(NULL == m_pSelectedColumns)
	{
		return;
	}

	KCodeGroup*       pCodeData       = NULL;
	KCodeManager*     pCodeManager    = NULL;
	pCodeManager    = m_pTable->GetOwner()->GetCodeManager();
	pCodeData   =     pCodeManager->FindCodeGroup(m_pGroupColumn->CodeGroup());

	// 일반 데이터
	m_vecGroupInfo.clear();
	for (int i=0; i<m_pSelectedColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = m_pSelectedColumns->GetColumn(i);
		std::map<Integer, std::vector<double>> mapSourceData;
		// 1. Get DB value 
		if(false == GetGroupData(pColumn, mapSourceData))
		{
			continue;
		}

		if(mapSourceData.size() < 1)
		{
			continue;
		}

		TSingleGroupInfo oTSingleGroupInfo;
		oTSingleGroupInfo.strCaption = pColumn->Caption();
		std::vector<TSingleNormalInfo> &vecNormalData = oTSingleGroupInfo.vecNormalData;

		std::map<Integer, std::vector<double>>::iterator iterBegin = mapSourceData.begin();
		std::map<Integer, std::vector<double>>::iterator iterEnd   = mapSourceData.end();
		while(iterBegin != iterEnd)
		{
			TSingleNormalInfo oTSingleNormalInfo;

			std::vector<double> vecData = iterBegin->second;

			// 2. caculate statistic
			KDevideDataInterval &oDevideDataInterval = oTSingleNormalInfo.oDevideDataInterval;
			oDevideDataInterval.SetDataCalculate(vecData);

			Integer nxType                = iterBegin->first;
			oTSingleNormalInfo.strCaption =  pCodeData->SingleCodeValue((int)nxType);

			vecNormalData.push_back(oTSingleNormalInfo);

			++iterBegin;
		}
		m_vecGroupInfo.push_back(oTSingleGroupInfo);
	}    

	// 수식 데이터
	m_vecGroupExpressionInfo.clear();

	std::map<Integer, std::vector<double>> mapExpressionData;

	if(true == GetExpressionGroupData(mapExpressionData))
	{
		if(mapExpressionData.size() > 0)
		{
			TSingleGroupExpressionInfo oTSingleGroupExpressionInfo;
			oTSingleGroupExpressionInfo.strCaption = m_pExpression->GetCaptionExpression();
			std::vector<TSingleExpressionInfo> &vecExpressionInfo = oTSingleGroupExpressionInfo.vecExpressionData;

			std::map<Integer, std::vector<double>>::iterator iterBegin = mapExpressionData.begin();
			std::map<Integer, std::vector<double>>::iterator iterEnd   = mapExpressionData.end();
			while(iterBegin != iterEnd)
			{
				TSingleExpressionInfo oTSingleExpressionInfo;

				std::vector<double> vecExpressionData = iterBegin->second;

				KDevideDataInterval &oDevideDataInterval = oTSingleExpressionInfo.oDevideDataInterval;
				oDevideDataInterval.SetDataCalculate(vecExpressionData);

				Integer nxType  = iterBegin->first;
				oTSingleExpressionInfo.strCaption = pCodeData->SingleCodeValue((int)nxType);

				vecExpressionInfo.push_back(oTSingleExpressionInfo);

				++iterBegin;
			}
			m_vecGroupExpressionInfo.push_back(oTSingleGroupExpressionInfo);
		}
	}
	else
	{
		CString strCaption = m_pExpression->GetCaptionExpression();
		if(strCaption.Compare(_T("")) != 0)
		{
			TSingleGroupExpressionInfo oTSingleGroupExpressionInfo;
			oTSingleGroupExpressionInfo.strCaption = m_pExpression->GetCaptionExpression();

			m_vecGroupExpressionInfo.push_back(oTSingleGroupExpressionInfo);
		}
	}
}


bool KStatisticsBaseDefineDlg::GetGroupData( KIOColumn* a_pColumn, std::map<Integer, std::vector<double>> &a_mapSourceData)
{
	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}
	if(m_bChkZero == FALSE)
	{
		strWhere += " AND ";
		strWhere += a_pColumn->Name();
		strWhere += " != 0 ";
	}

	strSQL.Format (_T(" SELECT %s, %s FROM %s %s "), a_pColumn->Name(), m_pGroupColumn->Name(), m_pTable->Name(), strWhere );

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	while( pResult->Next() )
	{
		double  dValue  = pResult->GetValueDouble(0);
		Integer nxGroup = pResult->GetValueInt64 (1);

		std::map<Integer, std::vector<double>>::iterator iterKey = a_mapSourceData.find(nxGroup);
		if(iterKey != a_mapSourceData.end())
		{
			std::vector<double> &vecData = iterKey->second;
			vecData.push_back(dValue);
		}
		else
		{
			std::vector<double> vecData;
			vecData.push_back(dValue);
			a_mapSourceData.insert( std::make_pair(nxGroup, vecData) );
		}
	}

	return true;
}


bool KStatisticsBaseDefineDlg::GetExpressionGroupData( std::map<Integer, std::vector<double>> &a_mapExpressionData )
{
	KIOColumns oReqColumns;
	KIOColumn* pColumn = NULL;

    KParseExpression oParseExpression;
    oParseExpression.SetTable(m_pTable);
    oParseExpression.SetCaptionExpression(m_pExpression->GetCaptionExpression());
    std::vector<CString> vecUsedColumnName = oParseExpression.GetUsedColumnName();

    CString strFunction = oParseExpression.GetRealExpression();
    size_t nxCount = vecUsedColumnName.size();
    for (size_t i=0; i<nxCount; i++)
    {
        pColumn = oReqColumns.AddNewColumn();
        pColumn->Name(vecUsedColumnName[i]);
    }

    /*
	const std::list<KExpressionElement*>& elementList = m_pExpression->GetExpressionList();
	std::list<KExpressionElement*>::const_iterator itElement, itEnd = elementList.end();
	for(itElement = elementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		if(pElement->ElementType() == KEMExpressionElementField)
		{
			KExpressionElementField* pField = (KExpressionElementField*)pElement;
			pColumn = oReqColumns.AddNewColumn();
			pColumn->Name(pField->FieldName());
			pColumn->Caption(pField->FieldCaption());
		}
	}
	*/

	int nReqColumnCount = oReqColumns.ColumnCount();
	if( nReqColumnCount < 1 )
	{
		return false;
	}

	KParseVariables       variables;
	KParseVariableInfo*   pVarInfo = NULL;
	CString               strColumnName =_T("");

	for(int j = 0; j < nReqColumnCount; ++j)
	{
		pVarInfo = variables.AddNewVariable();
		pColumn = oReqColumns.GetColumn(j);
		pVarInfo->SetVariable(pColumn->Name(), j);

		strColumnName += ( pColumn->Name() );

		if(j < nReqColumnCount -1)
		{
			strColumnName += ( _T(", ") );
		}
	}		

	KSDoubleRecord*     pRecord = NULL;
	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}

	strSQL.Format (_T(" SELECT %s, %s FROM %s %s "), m_pGroupColumn->Name(), strColumnName, m_pTable->Name(), strWhere);

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	std::map<Integer, KSDoubleRecordArray*> mapDoubleRecordArray;
	while( pResult->Next() )
	{
		Integer nxGroup = pResult->GetValueInt64(0);

		std::map<Integer, KSDoubleRecordArray*>::iterator iterKey = mapDoubleRecordArray.find(nxGroup);
		if(iterKey != mapDoubleRecordArray.end())
		{
			KSDoubleRecordArray*    pRecordArray = iterKey->second;
			pRecord = pRecordArray->AddNewRecord();
		}
		else
		{
			KSDoubleRecordArray*   pRecordArray = new KSDoubleRecordArray(nReqColumnCount);

			pRecord = pRecordArray->AddNewRecord();
			mapDoubleRecordArray.insert( std::make_pair(nxGroup, pRecordArray));

			std::vector<double> vecData;
			a_mapExpressionData.insert(  std::make_pair(nxGroup, vecData));
		}

		// ZERO 값을 포함 하지 않을 경우 처리
		if(FALSE == m_bChkZero)
		{
			int nZeroValCount = 0;
			for(int j = 0; j < nReqColumnCount; ++j)
			{
				if(pResult->GetValueDouble(1+j) == 0.0)
				{
					nZeroValCount++;
				}
			}

			if(nReqColumnCount == nZeroValCount)
			{
				continue;
			}
		}

		for(int j = 0; j < nReqColumnCount; ++j)
		{
			pRecord->SetAt( j, pResult->GetValueDouble(1+j) );
		}
	}

	//CString     strFunction = m_pExpression->GetExpression();
	KGeneration gen;

	std::map<Integer, KSDoubleRecordArray*>::iterator iterBegin = mapDoubleRecordArray.begin();
	std::map<Integer, KSDoubleRecordArray*>::iterator iterEnd   = mapDoubleRecordArray.end();
	std::map<Integer, std::vector<double>>::iterator iterExData  = a_mapExpressionData.begin();

	bool bResult = true;
	while( iterBegin != iterEnd )
	{
		KSDoubleRecordArray *pRecordArray = iterBegin->second;
		std::vector<double> &vecData      = iterExData->second;

		int nRet = gen.MuParseExecute(strFunction, variables, *pRecordArray, vecData, NULL);
		if(1 != nRet)
		{
			bResult = false;
		}

		delete pRecordArray;

		++iterExData;
		++iterBegin;
	}

	return bResult;
}


void KStatisticsBaseDefineDlg::OnBnClickedBtnFunction()
{

	KStatisticsFunctionDefineDlg dlg;
	dlg.SetTable(m_pTable);
	dlg.SetExpression(m_pExpression);

	if(IDOK == dlg.DoModal())
	{
		m_strFunction =  m_pExpression->GetCaptionExpression();
		UpdateData(FALSE);
	}
}


void KStatisticsBaseDefineDlg::OnSelectCancel()
{
	CheckboxControl( FALSE );
}


void KStatisticsBaseDefineDlg::OnSelectAll()
{
	CheckboxControl( TRUE );
}


void KStatisticsBaseDefineDlg::CheckboxControl(BOOL a_bCheck)
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		pItemCheck->SetChecked(a_bCheck);
	}

	m_wndReportCtrl.RedrawControl();
}


void KStatisticsBaseDefineDlg::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}

void KStatisticsBaseDefineDlg::OnBnClickedCheckGroupFiled()
{
	UpdateData(TRUE);
	DisableGroupFiled(m_bChkGroup);
}


void KStatisticsBaseDefineDlg::DisableGroupFiled(BOOL a_bDisable)
{
	m_cboGroup.EnableWindow(a_bDisable);
}


KIOColumn* KStatisticsBaseDefineDlg::GetCurGroupField()
{
	UpdateData(TRUE);
	KIOColumn* pColumn = NULL;

	if( TRUE == m_bChkGroup )
	{
		int nCur = m_cboGroup.GetCurSel();
		
		if(nCur < 0)
		{
			return NULL;
			
		}

		pColumn = (KIOColumn* )m_cboGroup.GetItemData(nCur);
	}
	else
	{
		pColumn = NULL;
	}

	return pColumn;
}
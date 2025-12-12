// DefineTransitCodeCostDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefineTransitCodeCostDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
//^#include "DBaseConnector.h"
#include "CustomReportRecordItem.h"

#include "KExRecordItem.h"
//#include "StringChecker.h"
#include "Scenario.h"
#include "Project.h"

enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_MODE,
	_2_COLUMN_TYPE,
	_3_COLUMN_NAME,
	_4_COLUMN_BASIC_RATE,
	_5_COLUMN_BASIC_DISTANCE,
	_6_COLUMN_ADD_RATE,
	_7_COLUMN_ADD_DISTANCE,
	_8_COLUMN_MERGE_RATE
};

const double dBASIC_RATE     = 1050;
const double dBASIC_DISTANSE = 10;
const double dADD_RATE		 = 100;
const double dADD_DISTANSE	 = 5;

// KDefineTransitCodeCostDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefineTransitCodeCostDlg, KResizeDialogEx)

KDefineTransitCodeCostDlg::KDefineTransitCodeCostDlg(KTarget* a_pTarget, bool a_bUseImport, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefineTransitCodeCostDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_bUseImport(a_bUseImport),
	m_cSeparator(_T(','))
{

}

KDefineTransitCodeCostDlg::~KDefineTransitCodeCostDlg()
{
}

void KDefineTransitCodeCostDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wmReportControl);
}


BEGIN_MESSAGE_MAP(KDefineTransitCodeCostDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINK, &KDefineTransitCodeCostDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL_LINK, &KDefineTransitCodeCostDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &KDefineTransitCodeCostDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefineTransitCodeCostDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KDefineTransitCodeCostDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &KDefineTransitCodeCostDlg::OnBnClickedButtonImport)
END_MESSAGE_MAP()


// KDefineTransitCodeCostDlg 메시지 처리기입니다.


BOOL KDefineTransitCodeCostDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		ResizeComponent();

		KReportCtrlSetting::Default(m_wmReportControl, TRUE);
		KDBaseDefineTransitCost::GetTransitCostValue2DB(m_pTarget, m_mapTransitTypeCost);
		KDBaseDefineTransitCost::GetTransitType2DB(m_pTarget, m_vecUsingTransitType);
		GetTransitTypeCode();
		GetUsingTrasitTypeCode();

		InitReportHeader();
		UpdateReportControl();

		CButton* pOKButton = (CButton*)GetDlgItem(IDOK);
		CButton* pCANCLEButton = (CButton*)GetDlgItem(IDCANCEL);
		if (true == m_bUseImport)
		{
			pCANCLEButton->EnableWindow(FALSE);
		}

		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefineTransitCodeCostDlg::GetTransitTypeCode()
{
	KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
	KCodeGroup* pTTypeCodeGroup		= pCodeMgr->FindCodeGroup(TRANSIT_TYPE_CODE_KEY);
	pTTypeCodeGroup->GetCodes(m_vecTransitTypeCode);
	
	KCodeGroup* pModeCodeGroup		= pCodeMgr->FindCodeGroup(TRANSIT_MODE_CODE_KEY);
	pModeCodeGroup->GetCodes(m_vecTransitModeCode);
}

void KDefineTransitCodeCostDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_MODE, _T("Mode"), 70));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("타입 설정"));
	}
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	int nCount = m_vecTransitModeCode.size();

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_MODE_CODE_KEY);

	for (int i = 0; i < nCount; i++)
	{
		int nCode = m_vecTransitModeCode[i];
		CString strCaption = pCodeGroup->SingleCodeValue(nCode);
		pEditOptions->AddConstraint(strCaption, nCode);
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_TYPE, _T("Type"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("타입"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_NAME, _T("Name"), 65));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("명칭"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_4_COLUMN_BASIC_RATE, _T("Base Fare"), 50));
	pColumn->SetTooltip(_T("Base Fare"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("기본운임"));
		pColumn->SetTooltip(_T("기본운임"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_5_COLUMN_BASIC_DISTANCE, _T("Base Dist"), 50));
	pColumn->SetTooltip(_T("Base Distance"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("~Km까지"));
		pColumn->SetTooltip(_T("~Km까지"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_6_COLUMN_ADD_RATE, _T("Dist-Base Fare"), 50));
	pColumn->SetTooltip(_T("Dist-Base Fare"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("추가운임"));
		pColumn->SetTooltip(_T("추가운임"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_7_COLUMN_ADD_DISTANCE, _T("Uint Dist"), 55));
	pColumn->SetTooltip(_T("Uint Distance"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("매 ~Km마다"));
		pColumn->SetTooltip(_T("매 ~Km마다"));
	}

	pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_8_COLUMN_MERGE_RATE, _T("D.F.P"), 60));
	pColumn->SetTooltip(_T("Discount Fare Policy"));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("통합환승할인"));
		pColumn->SetTooltip(_T("통합환승할인제 적용"));
	}
}

void KDefineTransitCodeCostDlg::UpdateReportControl()
{
	std::map<int, bool>::iterator iter = m_mapUsingTransitCode.begin();
	std::map<int, bool>::iterator end  = m_mapUsingTransitCode.end();

	KCodeManager* pCodeMgr		= m_pTarget->CodeManager();
	KCodeGroup*	  pCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_TYPE_CODE_KEY);

	while(iter != end)
	{
		int		nCode	= iter->first;
		bool	bUsing	= iter->second;

		CString strCode(_T(""));
		strCode.Format(_T("%d"), nCode);
		CString strCodeName = pCodeGroup->SingleCodeValue(nCode);

		CXTPReportRecord*		pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem	=  nullptr;

		if (bUsing == true)
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("씀"));
			}

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			AutoType iterMode = m_mapTransitTypeCost.find(nCode);
			AutoType endMode  = m_mapTransitTypeCost.end();

			if (iterMode != endMode)
			{
				TTransitCost oTransitCost = iterMode->second;
				pItem->SetItemData(oTransitCost.nMode);
			}
			else
			{
				pItem->SetItemData(TRANSIT_MODE_BUS);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
		}
		else
		{
			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			AutoType iterMode = m_mapTransitTypeCost.find(nCode);
			AutoType endMode  = m_mapTransitTypeCost.end();

			if (iterMode != endMode)
			{
				TTransitCost oTransitCost = iterMode->second;
				pItem->SetItemData(oTransitCost.nMode);
			}
			else
			{
				pItem->SetItemData(TRANSIT_MODE_BUS);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCode));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCodeName));
		pItem->SetEditable(TRUE);

		AutoType iterFind = m_mapTransitTypeCost.find(nCode);
		AutoType iterend  = m_mapTransitTypeCost.end();

		if (iterFind != iterend)
		{
			TTransitCost oTransitCost = iterFind->second;
			
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitCost.dTbasicRate, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitCost.dTbasicDistance, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitCost.dTAddRate, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitCost.dTAddDistance, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			if (1 == oTransitCost.nTMergeRate)
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
			}
		}
		else
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBASIC_RATE, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBASIC_DISTANSE, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dADD_RATE, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dADD_DISTANSE, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(FALSE);
			
		}

		++iter;
	}

	m_wmReportControl.Populate();
}

void KDefineTransitCodeCostDlg::GetUsingTrasitTypeCode()
{
	m_mapUsingTransitCode.clear();

	int nCodeCount = m_vecTransitTypeCode.size();
	for (int i = 0; i < nCodeCount ; i++)
	{
		int nCode = m_vecTransitTypeCode[i];
		m_mapUsingTransitCode.insert(std::make_pair(nCode, false));
	}

	int nUseingCount = m_vecUsingTransitType.size();
	for (int k = 0; k <nUseingCount; k++)
	{
		int nCode = m_vecUsingTransitType[k];
		AutoType iter = m_mapUsingTransitCode.find(nCode);
		AutoType end  = m_mapUsingTransitCode.end();
		if (iter != end) {
			iter->second = true;
		}
		else
		{
			m_mapUsingTransitCode.insert(std::make_pair(nCode, true));
		}
	}
}


void KDefineTransitCodeCostDlg::OnBnClickedButtonAdd()
{
	try
	{
		int nTotalRowCount = m_wmReportControl.GetRows()->GetCount();

		CXTPReportRecord*     pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem   = NULL;

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(TRANSIT_MODE_BUS);

		int nMaxCode(0);
		GetMaxCodeValue(nMaxCode);

		nMaxCode = nMaxCode + 1;
		CString strCode(_T(""));
		strCode.Format(_T("%d"), nMaxCode);

		pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem	= pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBASIC_RATE, _T("%.0f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBASIC_DISTANSE, _T("%.0f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dADD_RATE, _T("%.0f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dADD_DISTANSE, _T("%.0f")));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetChecked(FALSE);

		m_wmReportControl.Populate();

		nTotalRowCount = m_wmReportControl.GetRows()->GetCount();
		CXTPReportRow* pRow = m_wmReportControl.GetRows()->GetAt(nTotalRowCount - 1);

		XTP_REPORTRECORDITEM_ARGS args(&m_wmReportControl, pRow, m_wmReportControl.GetColumns()->GetAt(_3_COLUMN_NAME));
		m_wmReportControl.EditItem(&args);

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


void KDefineTransitCodeCostDlg::OnBnClickedButtonDel()
{
	std::vector<CXTPReportRecord*> vecRecrod;
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;

	try
	{
		pRecords			= m_wmReportControl.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_SELECT);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_wmReportControl.GetRecords()->RemoveRecord(pRecord);
			m_wmReportControl.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
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

void KDefineTransitCodeCostDlg::GetMaxCodeValue(int& a_nMaxCode)
{
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItemText*    pItemText	   = nullptr;

	try
	{
		pRecords      = m_wmReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText   = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_TYPE);
			CString strValue = pItemText->GetValue();
			int nUserCode = _ttoi(strValue);

			if (nUserCode > a_nMaxCode)
			{
				a_nMaxCode = nUserCode;
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


void KDefineTransitCodeCostDlg::OnBnClickedOk()
{
	try
	{
		UpdateResultCode();

		KIOTable* pTransitTable = m_pTarget->Tables()->FindTable(TABLE_TRANSIT);
		pTransitTable->Notify();

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
		AfxMessageBox(_T("오류가 발생하였습니다."));
		return;
	}

	AfxMessageBox(_T("적용되었습니다."));
	KResizeDialogEx::OnOK();
}


void KDefineTransitCodeCostDlg::OnBnClickedCancel()
{
	if (true == m_bUseImport)
	{
		AfxMessageBox(_T("대중교통 데이터 불러오기를 수행 한 후 대중교통 비용 설정을 해야 합니다."));
		return;
	}
	KResizeDialogEx::OnCancel();
}

void KDefineTransitCodeCostDlg::UpdateResultCode()
{
	try
	{
		CXTPReportRecords*           pRecords      = m_wmReportControl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;
		CXTPReportRecordItemNumber*	 pItemNum	   = nullptr;

		std::vector<TTransitCost> vecTransitCost;
		std::map<int, CString> mapResultTransitCode;
		
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TTransitCost oTransitCost;

			pRecord = pRecords->GetAt(i);
			
			pItem = pRecord->GetItem(_1_COLUMN_MODE);
			int nSystemCode = pItem->GetItemData();
			oTransitCost.nMode = nSystemCode;

			pItemText		= (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_TYPE);
			CString strCode = pItemText->GetValue();
			int nCode		= _ttoi(strCode);
			if (false == QBicStringChecker::IsUnSignedInteger(strCode))
			{
				CString strMsg;
				strMsg.Format(_T("Type : '%s' 의 값이 잘못되었습니다. \n0보다 큰 값을 입력해 주세요"), strCode);
				ThrowException(strMsg);
			}
			oTransitCost.nTType	= nCode;

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_3_COLUMN_NAME);
			CString strCodeName = pItemText->GetValue();

			if (strCodeName == _T("") || strCodeName == _T("'"))
			{
				CString strMsg;
				strMsg.Format(_T("Type : '%s' 의 이름이 없습니다."), strCode);
				ThrowException(strMsg);
			}
			else if (strCodeName.Find(_T(" ")) != -1)
			{
				CString strMsg;
				strMsg.Format(_T("Type : '%s' 의 이름에 공백이 포함되어 있습니다."), strCode);
				ThrowException(strMsg);
			}

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_BASIC_RATE);
			oTransitCost.dTbasicRate = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(_5_COLUMN_BASIC_DISTANCE);
			oTransitCost.dTbasicDistance = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(_6_COLUMN_ADD_RATE);
			oTransitCost.dTAddRate = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(_7_COLUMN_ADD_DISTANCE);
			oTransitCost.dTAddDistance = pItemNum->GetValue();


			pItem    = pRecord->GetItem(_8_COLUMN_MERGE_RATE);
			if( TRUE == pItem->IsChecked() )
			{
				oTransitCost.nTMergeRate = 1;
			}
			else
			{
				oTransitCost.nTMergeRate = 0;
			}

			vecTransitCost.push_back(oTransitCost);

			AutoType iter = mapResultTransitCode.find(nCode);
			AutoType end  = mapResultTransitCode.end();
			if (iter != end)
			{
				CString strMsg;
				strMsg.Format(_T("Type : '%d' 값이 중복되어 사용되고 있습니다."), nCode);
				ThrowException(strMsg);
			}
			else
			{
				mapResultTransitCode.insert(std::make_pair(nCode, strCodeName));
			}
		}

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup* pTTypeCodeGroup		= pCodeMgr->FindCodeGroup(TRANSIT_TYPE_CODE_KEY);
		pTTypeCodeGroup->RemoveAllCode();

		AutoType iterCode = mapResultTransitCode.begin();
		AutoType endCode  = mapResultTransitCode.end();
		while(iterCode != endCode)
		{
			int	nCode		= iterCode->first;
			CString strCode = iterCode->second;
			pTTypeCodeGroup->AddCode(nCode, strCode);
			iterCode++;
		}

		KDBaseDefineTransitCost::UpdateTransitCost(m_pTarget, vecTransitCost);		
		KDBaseDefineTransitCost::UpdateTransitInfoMode(m_pTarget, vecTransitCost);

		KIOTable* pTransit = m_pTarget->Tables()->FindTable(TABLE_TRANSIT);
		pTransit->Notify();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDefineTransitCodeCostDlg::ResizeComponent()
{
	SetResize(IDC_REPORT			, SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ADD_LINK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL_LINK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC4			, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK					, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL				, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_EXPORT     , SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_IMPORT     , SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KDefineTransitCodeCostDlg::OnBnClickedButtonExport()
{
	m_strExportFile = _T("");

	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Data");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strExportFile.Format(_T("%s.txt"), strExportFile);


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ExportThreadCaller, this, false, nLang);

		if (m_bThreadOK)
			AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineTransitCodeCostDlg::ExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefineTransitCodeCostDlg* pDefineTransitCodeCostDlg = (KDefineTransitCodeCostDlg*)pParameter->GetParameter();
	pDefineTransitCodeCostDlg->Export();

	return 0;
}

void KDefineTransitCodeCostDlg::Export( void )
{
	m_bThreadOK = false;

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strExportFile, nOpenMode);

	try
	{
		CString strLine;
		
		//WriteHeader
		strLine = _T("###Mode, Type, Name, Base Fare, Base Dist, Dist-Base Fare, Unit Dist, D.F.P\r\n");
		outFile.WriteString(strLine);

		//WriteData
		CXTPReportRecords* pRecords = m_wmReportControl.GetRecords();
		int                nRecord  = pRecords->GetCount();

		int     nMode;
		CString strCode, strCodeName, strChecked;
		double  dBasicRate, dBasicDist, dAddRate, dAddDist;

		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord*           pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem*       pItem   = nullptr;
			CXTPReportRecordItemNumber* pNumber = nullptr;

			pItem = pRecord->GetItem(1);
			nMode = (int)pItem->GetItemData();

			pItem = pRecord->GetItem(2);
			strCode = pItem->GetCaption();

			pItem = pRecord->GetItem(3);
			strCodeName = pItem->GetCaption();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			dBasicRate = pNumber->GetValue();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			dBasicDist = pNumber->GetValue();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
			dAddRate = pNumber->GetValue();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(7);
			dAddDist = pNumber->GetValue();

			pItem = pRecord->GetItem(8);

			if (pItem->IsChecked())
			{
				strChecked = _T("1");
			}
			else
			{
				strChecked = _T("0");
			}

			strLine.Format(_T("%d, %s, '%s', %f, %f, %f, %f, %s\r\n"), 
				nMode, strCode, strCodeName, dBasicRate, dBasicDist
				, dAddRate, dAddDist, strChecked);

			outFile.WriteString(strLine);
		}		

		outFile.Close();
		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KDefineTransitCodeCostDlg::OnBnClickedButtonImport()
{
	m_setDetailID.clear();

	try
	{
		TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CFileDialog filedlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );
		
		KScenario* pScenario   = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject    = (KProject*)pScenario->GetParentObject();
		CString    strLocation = pProject->GetLocation();

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

		if( filedlg.DoModal() != IDOK )
			return;

		m_strImportFile = filedlg.GetPathName();


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(ImportThreadCaller, this, false, nLang);

		if (m_bThreadOK)
			UpdateImportData();

		if (m_bThreadOK)
			AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
		else
			AfxMessageBox(m_strErr);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KDefineTransitCodeCostDlg::ImportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefineTransitCodeCostDlg* pDefineTransitCodeCostDlg = (KDefineTransitCodeCostDlg*)pParameter->GetParameter();
	pDefineTransitCodeCostDlg->Import();

	return 0;
}

void KDefineTransitCodeCostDlg::Import( void )
{
	m_mapImportData.clear();
	m_bThreadOK = false;

	UINT         nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT         nCodePage = 949;
	CStdioFileEx csvFile;

	try
	{
		CString strLine, strTemp, strErr;

		csvFile.SetCodePage(nCodePage);

		if (csvFile.Open(m_strImportFile, nOpenMode) == FALSE)
			return;

		int nLine(0);

		while (csvFile.ReadString(strLine))
		{
			++nLine;

			strLine.Trim();

			if (strLine.IsEmpty())
				continue;

			strTemp = strLine.Left(1);

			if (strTemp.CompareNoCase(_T("#")) == 0)
				continue;

			strLine.Replace(_T(" "),  _T(""));
			strLine.Replace(_T("\""), _T(""));
			strLine.Replace(_T("'"),  _T(""));

			CSVRow row;
			ParseCSVLineString(strLine, row);

			if (row.size() != 8)
			{
				strErr.Format(_T("Line : %d 데이터 개수 불일치"), nLine);
				ThrowException(strErr);
			}

			TDefineData oDefineData = {};

			oDefineData.nMode         = _ttoi(row[0]);
			oDefineData.nDetailCode   = _ttoi(row[1]);
			oDefineData.strDetailName = row[2];
			oDefineData.dBaseFare     = _ttof(row[3]);
			oDefineData.dBaseDist     = _ttof(row[4]);
			oDefineData.dAddRate      = _ttof(row[5]);
			oDefineData.dAddDist      = _ttof(row[6]);
			oDefineData.nDFP          = _ttoi(row[7]);

			if (!VerifyImportData(oDefineData, strTemp))
			{
				strErr.Format(_T("Line : %d %s"), nLine, strTemp);
				ThrowException(strErr);
			}

			if (m_mapImportData.find(oDefineData.nDetailCode) != m_mapImportData.end())
			{
				strErr.Format(_T("Line : %d 중복된 상세 Type(Detail_Mode_ID)이 있습니다."), nLine);
				ThrowException(strErr);
			}
			
			m_mapImportData.insert(std::make_pair(oDefineData.nDetailCode, oDefineData));
			m_setDetailID.insert(oDefineData.nDetailCode);
		}
		
		csvFile.Close();
		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		csvFile.Close();
		TxExceptionPrint(ex);
		m_strErr = ex->GetErrorMessage();
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();
		m_strErr = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefineTransitCodeCostDlg::ParseCSVLineString( CString strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;
	
	while( AfxExtractSubString( strRow, strLine, nSub++, m_cSeparator ) == TRUE )
	{
		if( true == bQuote )
		{
			strQuote += strRow;
			if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') )
			{
				strQuote.Remove( _T('"') );
				strRow = strQuote;
				bQuote = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		}
		else
		{
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE )
			{
				row.push_back( strRow );
			}
			else if( strRow.GetLength() == 1 )
			{
				chFirst = strRow.GetAt( 0 );
				if( chFirst == _T('"') )
				{
					bQuote = true;
				}
				else
				{
					row.push_back(strRow);
				}
			}
			else
			{
				chFirst = strRow.GetAt( 0 );
				chLast = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) )
				{
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				}
				else if( chFirst == _T('"') )
				{
					bQuote = true;
					strQuote = strRow;
				}
				else
				{
					row.push_back( strRow );
				}
			}
		}
	}
}

bool KDefineTransitCodeCostDlg::VerifyImportData( TDefineData a_oDefineData, CString& a_strErr )
{
	a_strErr = _T("");
	bool bResult = false;

	try
	{
		//Verify Mode
		bool bCheckModeID(false);

		for (size_t i = 0; i < m_vecTransitModeCode.size(); i++)
		{
			int nModeID = m_vecTransitModeCode[i];
			if (a_oDefineData.nMode == nModeID)
			{
				bCheckModeID = true;
			}
		}
		
		if (bCheckModeID == false)
		{
			ThrowException(_T("정의되지 않은 ModeID입니다."));
		}

		//Check DFP
 		if (!(a_oDefineData.nDFP == 0 || a_oDefineData.nDFP == 1))
		{
			ThrowException(_T("D.F.P의 값은 0 또는 1이어야 합니다."));
		}

		bResult = true;
	}
	catch (KExceptionPtr ex)
	{
		a_strErr = ex->GetErrorMessage();
	}
	catch (...)
	{
		a_strErr = _T("데이터에 오류가 존재합니다.");
	}

	return bResult;	
}

void KDefineTransitCodeCostDlg::UpdateImportData( void )
{
	//ReportCtrl 값을 m_mapReportData에 담음
	m_mapReportData.clear();
	
	try
	{
		int nRecord = m_wmReportControl.GetRecords()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord*           pRecord = m_wmReportControl.GetRecords()->GetAt(i);
			CXTPReportRecordItem*       pItem   = nullptr;
			CXTPReportRecordItemNumber* pNumber = nullptr;

			TDefineData oDefineData = {};



			pItem = pRecord->GetItem(1);
			oDefineData.nMode = (int)pItem->GetItemData();

			pItem = pRecord->GetItem(2);
			oDefineData.nDetailCode = _ttoi(pItem->GetCaption());

			pItem = pRecord->GetItem(3);
			oDefineData.strDetailName = pItem->GetCaption();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oDefineData.dBaseFare = pNumber->GetValue();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			oDefineData.dBaseDist = pNumber->GetValue();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
			oDefineData.dAddRate = pNumber->GetValue();

			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(7);
			oDefineData.dAddDist = pNumber->GetValue();

			pItem = pRecord->GetItem(8);

			if (pItem->IsChecked())
			{
				oDefineData.nDFP = 1;
			}
			else
			{
				oDefineData.nDFP = 0;
			}

			m_mapReportData.insert(std::make_pair(oDefineData.nDetailCode, oDefineData));
			m_setDetailID.insert(oDefineData.nDetailCode);

			pItem = pRecord->GetItem(0);

			if (!pItem->GetHasCheckbox())
				m_setUsedDetailID.insert(oDefineData.nDetailCode);
		}

		//ReportCtrl Redraw
		m_wmReportControl.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem	  = nullptr;

		std::set<int>::iterator bSetIter, eSetIter = m_setDetailID.end();

		std::map<int, TDefineData>::iterator fImportIter, fReportIter;

		for (bSetIter = m_setDetailID.begin(); bSetIter != eSetIter; ++bSetIter)
		{
			int nDetailModeID = *bSetIter;

			fImportIter = m_mapImportData.find(nDetailModeID);
			fReportIter = m_mapReportData.find(nDetailModeID);

			pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);

			if (fImportIter != m_mapImportData.end())
			{
				AddReportData(pRecord, fImportIter->second);
			}
			else if (fReportIter != m_mapReportData.end())
			{
				fReportIter = m_mapReportData.find(nDetailModeID);
				AddReportData(pRecord, fReportIter->second);			
			}
			else
			{
				ThrowException(_T("unexepcted error..."));
			}
		}

		m_wmReportControl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		m_bThreadOK = false;
		m_strErr = ex->GetErrorMessage();
	}
	catch (...)
	{
		m_bThreadOK = false;
		m_strErr = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}	
}

void KDefineTransitCodeCostDlg::AddReportData( CXTPReportRecord* a_pRecord, TDefineData a_oDefineData )
{
	std::set<int>::iterator fSetIter = m_setUsedDetailID.find(a_oDefineData.nDetailCode);

	CXTPReportRecordItem* pItem = nullptr;
	CString strValue;

	if (fSetIter != m_setUsedDetailID.end())//Used일 경우
	{
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(_T("Used")));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pItem->SetCaption(_T("씀"));
		}

		pItem = a_pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(a_oDefineData.nMode);
		
		strValue.Format(_T("%d"), a_oDefineData.nDetailCode);
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(strValue));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);
	}
	else
	{
		pItem = a_pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);

		pItem = a_pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(a_oDefineData.nMode);

		strValue.Format(_T("%d"), a_oDefineData.nDetailCode);
		pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(strValue));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);
	}

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemText(a_oDefineData.strDetailName));
	pItem->SetEditable(TRUE);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemNumber(a_oDefineData.dBaseFare, _T("%.2f")));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemNumber(a_oDefineData.dBaseDist, _T("%.2f")));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemNumber(a_oDefineData.dAddRate, _T("%.2f")));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = a_pRecord->AddItem(new CXTPReportRecordItemNumber(a_oDefineData.dAddDist, _T("%.2f")));
	pItem->SetEditable(TRUE);
	pItem->SetAlignment(DT_CENTER);

	pItem = a_pRecord->AddItem(new KExRecordItemCheck());
	pItem->HasCheckbox(TRUE);
	pItem->SetAlignment(xtpColumnIconCenter);

	if (1 == a_oDefineData.nDFP)
	{
		pItem->SetChecked(TRUE);
	}
	else
	{
		pItem->SetChecked(FALSE);
	}
}

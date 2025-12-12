// MultiAssignModelSOBADlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiAssignModelSOBADlg.h"
#include "afxdialogex.h"

#include "ImChampFrameWindow.h"
#include "Scenario.h"
#include "MultiAssignInfo.h"
#include "AInputInfo.h"

#include "KExRecordItem.h"
#include "CustomReportRecordItem.h"

#include "DBaseAssignment.h"
#include "Launcher.h"
#include "ImChampDir.h"
#include "WriteCodeAgrument.h"
#include "..\QBicCommon\QBicWindowSystem.h"
#include "AssignmentModelDlg.h"
#include "InspectionNetwork.h"
#include "KBulkDBase2File.h"
#include "KxBulkDbase.h"
#include "DBaseMultiAssignSOBA.h"
//#include "DBaseCheckData.h"
#include "AWarmStartDlg.h"
#include "SOBASelectScenarioDlg.h"


// MultiAssignModelSOBADlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(MultiAssignModelSOBADlg, KDialogEx)

MultiAssignModelSOBADlg::MultiAssignModelSOBADlg(KTarget* a_pTarget, KEMAssignmentModel a_emModel, int a_nModeInVDF, CWnd* pParent /*=NULL*/)
	: KDialogEx(MultiAssignModelSOBADlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_emAModel(a_emModel)
	, m_pInfo(nullptr)
	, m_nModeInVDF(a_nModeInVDF)
{
	m_pParent = (KAssignmentModelDlg*)pParent;
	m_bTreeChecked = false;
	m_bRunFailed = false;

	if (!KDBaseMultiAssignSOBA::CheckMultiAssignSOBATableExists())
	{
		KDBaseMultiAssignSOBA::CreateMultiAssignSOBATable();
	}
}

MultiAssignModelSOBADlg::~MultiAssignModelSOBADlg()
{
	ClearMultiAssignInfos();
}


void MultiAssignModelSOBADlg::ClearMultiAssignInfos( void )
{
	m_arrSelectedInfo.clear();
	int nSize = m_vecInfo.size();

	for (int i = 0; i < nSize; ++i)
	{
		KMultiAssignInfoSOBA* pInfo = m_vecInfo[i];
		QBicDelete(pInfo);
	}

	m_vecInfo.clear();
	
	m_pInfo = nullptr;
}


void MultiAssignModelSOBADlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,     m_wndTree);
	DDX_Control(pDX, IDC_REPORT1,    m_wndReport);
	DDX_Control(pDX, IDC_COMBO1,     m_cboOverwriteInfo);
	DDX_Control(pDX, IDC_COMBO_NAME, m_cboName);
}


BEGIN_MESSAGE_MAP(MultiAssignModelSOBADlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &MultiAssignModelSOBADlg::OnBnClickedRun)
	ON_BN_CLICKED(IDCANCEL, &MultiAssignModelSOBADlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &MultiAssignModelSOBADlg::OnReportItemClick)
	//ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &MultiAssignModelSOBADlg::OnReportCheckItemTarget)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &MultiAssignModelSOBADlg::OnItemButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT1, &MultiAssignModelSOBADlg::OnReportCheckItemMode)
	ON_BN_CLICKED(IDC_RADIO_NEW,       &MultiAssignModelSOBADlg::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, &MultiAssignModelSOBADlg::OnBnClickedRadioOverwrite)
	ON_CBN_SELCHANGE(IDC_COMBO_NAME,   &MultiAssignModelSOBADlg::OnCbnSelChangeComboName)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON1, &MultiAssignModelSOBADlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BTN_ADD_SCENARIO, &MultiAssignModelSOBADlg::OnBnClickedBtnAddScenario)
END_MESSAGE_MAP()

BOOL MultiAssignModelSOBADlg::OnCopyData(CWnd* a_pWnd, COPYDATASTRUCT* a_pCopyDataStruct)
{
	try
	{
		int     nMsgCode = a_pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)a_pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);

		if (m_nResultCode == KLAUNCHER_USER_FORCE_STOP)
			theApp.KillKlauncher();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}   

	return KDialogEx::OnCopyData(a_pWnd, a_pCopyDataStruct);
}


void MultiAssignModelSOBADlg::OnReportItemClick( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) a_pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	// 2019.04.22 수정 : 추가된 시나리오만 트리에 나오도록 수정
	//if (!m_bTreeChecked)
	//{
	//	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	//	CXTPReportRecordItem* pItemTarget = pRecord->GetItem(0);

	//	//KMultiAssignInfo* pDlg = (KMultiAssignInfo*)pItemNotify->pItem->GetItemData();
	//	KMultiAssignInfoSOBA* pDlg = (KMultiAssignInfoSOBA*)pItemTarget->GetItemData();

	//	if (pDlg == nullptr)
	//		return;

	//	if (m_pInfo != nullptr)
	//	{
	//		SaveModeInfo();
	//		SaveOutputInfo();
	//	}

	//	UpdateModeData(pDlg);
	//	UpdateOutputData(pDlg);

	//	m_pInfo = pDlg;
	//}

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItemTarget = pRecord->GetItem(0);

	//KMultiAssignInfo* pDlg = (KMultiAssignInfo*)pItemNotify->pItem->GetItemData();
	KMultiAssignInfoSOBA* pDlg = (KMultiAssignInfoSOBA*)pItemTarget->GetItemData();

	if (pDlg == nullptr)
		return;

	if (m_pInfo != nullptr)
	{
		SaveModeInfo();
		SaveOutputInfo();
	}

	UpdateModeData(pDlg);
	UpdateOutputData(pDlg);

	m_pInfo = pDlg;
}


//void MultiAssignModelSOBADlg::OnReportCheckItemTarget(NMHDR* a_pNotifyStruct, LRESULT* a_pResult)
//{
//	m_bTreeChecked = true;
//	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
//	ASSERT(NULL != pItemNotify);
//
//	KMultiAssignInfo* pDlg = (KMultiAssignInfo*)pItemNotify->pItem->GetItemData();
//
//	if (pDlg == nullptr)
//	{
//		return;
//	}
//
//	if (m_pInfo != nullptr)
//	{
//		SaveModeInfo();
//		SaveOutputInfo();
//	}
//
//
//	if (pItemNotify->pItem->IsChecked())
//	{
//		KTarget* pTarget = pDlg->Target();
//		std::map<KTarget*, KARunningOptionInfo>::iterator itRunningOption = m_mapRunningOption.find(pTarget);
//
//		if (m_mapRunningOption.end() != itRunningOption)
//		{
//			KARunningOptionInfo& ro = itRunningOption->second;
//
//			// 2019.04.19 수정 : 웜스타트 초기치 선택시 현재 체크되어 있는 시나리오를 선택 할 수 있도록 수정
//			KAWarmStartDlg oDlg(pTarget, pDlg->GetInputInfo(), ro.Target(), ro.Scnario());
//			std::set<KTarget*> setCheckedTarget = GetCheckedTarget();
//			oDlg.SetCheckedTarget(setCheckedTarget);
//
//			if (oDlg.DoModal() == IDOK)
//			{
//				pDlg->SetSelected(true);
//				KTarget* pSelTarget = oDlg.GetWarmTarget();
//				KScenario* pSelScenario = oDlg.GetWarmScnario();
//
//				if (NULL != pSelScenario && NULL != pSelTarget)
//				{
//					CString strCaption(_T(""));
//					strCaption.Format(_T("%s_%s"), pSelScenario->GetName(), pSelTarget->GetName());
//
//					CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
//					CXTPReportRecordItem* pItemStatus = pRecord->GetItem(1);
//					pItemStatus->SetCaption(_T("선택됨"));
//					CXTPReportRecordItem* pItemWarmstart = pRecord->GetItem(2);
//					pItemWarmstart->SetCaption(strCaption);
//
//					ro.IncludeWarmStart(true);
//					ro.IncludeTurnRestriction(oDlg.isTurnRestriction());
//					ro.IncludeTurnVolume(oDlg.isIncludeTurnVolume());
//					ro.IncludeBusInitialVolume(oDlg.isBusInitialVolume());
//					ro.IncludePathAnalysis(oDlg.isPathAnalysis());
//					ro.IncludeIntersectionDelay(oDlg.isIntersectionDelay());
//
//					ro.Target(pSelTarget);
//					ro.Scnario(pSelScenario);
//				}
//			}
//			else
//			{
//				pDlg->SetSelected(false);
//				pItemNotify->pItem->SetChecked(FALSE);
//				m_wndTree.RedrawControl();
//			}
//		}
//	}
//	else
//	{
//		pDlg->SetSelected(false);
//		CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
//		CXTPReportRecordItem* pItemStatus = pRecord->GetItem(1);
//		pItemStatus->SetCaption(_T(""));
//		CXTPReportRecordItem* pItemWarmstart = pRecord->GetItem(2);
//		pItemWarmstart->SetCaption(_T(""));
//	}
//
//	UpdateModeData(pDlg);
//	UpdateOutputData(pDlg);
//
//	m_pInfo = pDlg;
//
//}


void MultiAssignModelSOBADlg::OnReportCheckItemMode( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (nIndex == 0)
	{
		if (pItemNotify->pRow->GetRecord()->GetItem(nIndex)->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetEditable(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetBackgroundColor(RGB(255, 255, 255));
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(2)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
	}

	m_wndReport.RedrawControl();
}

// MultiAssignModelSOBADlg 메시지 처리기입니다.


BOOL MultiAssignModelSOBADlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReport);

	InitModeReport();

	InitDefaultData();

	KReportCtrlSetting::DefaultC(m_wndTree);
	//m_wndTree.GetReportHeader()->SetAutoColumnSizing(FALSE);
	//KReportCtrlSetting::VerticalNoLine(m_wndTree);
	//KReportCtrlSetting::HorizontalNoLine(m_wndTree);

	InitTreeReport();
	UpdateTreeData();

	
	//Output
	SetDefualtDescription();

	NewMultiScenarioName();
	InitMultiScenarioCombo();

	//Init 이후 Select Matrix 정보 바로 표출되게 하기

	int nSize = m_vecInfo.size();

	if (nSize != 0)
	{
		KMultiAssignInfoSOBA* pInfo = m_vecInfo[0];	//0 번은 Highway Assignment 고정

		if (m_pInfo != nullptr)
		{
			SaveModeInfo();
			SaveOutputInfo();
		}

		UpdateModeData(pInfo);
		UpdateOutputData(pInfo);

		m_pInfo = pInfo;		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void MultiAssignModelSOBADlg::UpdateInputList(KMultiAssignInfoSOBA* a_pInfo )
{
	std::list<KAInputInfo*> lstParent  = m_pParent->InputList();
	std::list<KAInputInfo*> lstCurrent = a_pInfo->GetInputInfo();

	std::list<KAInputInfo*>::iterator bIter = lstCurrent.begin();
	std::list<KAInputInfo*>::iterator bIterParent;


	while (bIter != lstCurrent.end())
	{
		KAInputInfo* pInfoCurrent = *bIter;

		bIterParent = lstParent.begin();

		while (bIterParent != lstParent.end())
		{
			KAInputInfo* pInfoParent = *bIterParent;

			if (pInfoCurrent->Mode()->ModeID() == pInfoParent->Mode()->ModeID())
			{
				pInfoCurrent->Occupancy(pInfoParent->Occupancy());
				pInfoCurrent->PCU(pInfoParent->PCU());

				if (pInfoParent->Selected())
				{
					KIOColumn* pColumnParent = pInfoParent->InputColumn();
					
					KIOTable*         pTable   = a_pInfo->Target()->Tables()->FindTable(TABLE_MODE_OD);
					const KIOColumns* pColumns =  pTable->Columns();

					int nCount = pColumns->ColumnCount();

					for (int i = 0; i < nCount; ++i)
					{
						KIOColumn* pColumn    = pColumns->GetColumn(i);
						CString    strCaption = pColumn->Caption();

						if (strCaption.CompareNoCase(pColumnParent->Caption()) == 0)
						{
							pInfoCurrent->Selected(true);
							pInfoCurrent->InputColumn(pColumn);

							break;
						}
					}

					pInfoCurrent->Selected(true);
				}
			}			

			++bIterParent;
		}

		++bIter;
	}
}


void MultiAssignModelSOBADlg::InitDefaultData( void )
{
	ClearMultiAssignInfos();

	KMultiAssignInfoSOBA* pDlg     = nullptr;
	KProject*         pProject = ImChampFrameWindow::GetProject();

	//Parent Target;
	pDlg = new KMultiAssignInfoSOBA(m_pTarget, true);
	pDlg->InputList(m_pParent->InputList());
	pDlg->SetSelected(true);
	pDlg->SetRunningOption(m_RunningOption);
	m_vecInfo.push_back(pDlg);
	m_arrSelectedInfo.push_back(pDlg);

	

	//Base Scenario
	KScenario* pBaseScenario = pProject->GetBaseScenario();
	const KTDTargetMap& targetMap = pBaseScenario->GetTargetMap();

    if (targetMap.size() > 0)
    {
        if (targetMap.begin()->second != m_pTarget && !targetMap.begin()->second->IsEmptyData())
        {
            pDlg = new KMultiAssignInfoSOBA(targetMap.begin()->second);

            UpdateInputList(pDlg);

            m_vecInfo.push_back(pDlg);

			// 2019.04.19 수정 : 웜스타트 초기치 선택시 처음에는 아무것도 선택되지 않은 상태로 표출(현재 활성화된 시나리오는 제외)
			KARunningOptionInfo ro = m_RunningOption;
			ro.Scnario(NULL);
			ro.Target(NULL);
			pDlg->SetRunningOption(ro);
        }	
    }
	//Base를 제외한 시나리오
	const KTDScenarioMap& oScenarioMap  = pProject->GetScenarioMap();

	KTDScenarioMap::const_iterator bIterScenario = oScenarioMap.begin();

	while (bIterScenario != oScenarioMap.end())
	{
		int nLoop(0);

		KScenario*          pScenario  = bIterScenario->second;
		const KTDTargetMap& oTargetMap = pScenario->GetTargetMap();

		KTDTargetMap::const_iterator bIterTarget = oTargetMap.begin();

		while (bIterTarget != oTargetMap.end())
		{
			if (bIterTarget->second != m_pTarget && !bIterTarget->second->IsEmptyData())
			{
				pDlg = new KMultiAssignInfoSOBA(bIterTarget->second);

				UpdateInputList(pDlg);

				m_vecInfo.push_back(pDlg);
				
				// 2019.04.19 수정 : 웜스타트 초기치 선택시 처음에는 아무것도 선택되지 않은 상태로 표출(현재 활성화된 시나리오는 제외)
				KARunningOptionInfo ro = m_RunningOption;
				ro.Scnario(NULL);
				ro.Target(NULL);
				pDlg->SetRunningOption(ro);
			}

			++bIterTarget;
		}

		++bIterScenario;
	}
}

void MultiAssignModelSOBADlg::InitTreeReport( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nItemIndex(0);
	
	pColumn = m_wndTree.AddColumn(new CXTPReportColumn(nItemIndex++, _T("::: Select Target"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);
//	pColumn->SetTreeColumn(TRUE);
	
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("분석할 시나리오"));
    }

	pColumn = m_wndTree.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Initial Warm Start Value"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("웜 스타트 초기치"));
	}

	pColumn = m_wndTree.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Remove from list"), 70, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("목록에서 삭제"));
	}


	m_wndTree.Populate();
}

void MultiAssignModelSOBADlg::UpdateTreeData( void )
{
	//UpdateData
	m_wndTree.ResetContent();
	CString strValue;

	CXTPReportRecord*     pScenarioRecord = nullptr;
	CXTPReportRecord*     pTargetRecord   = nullptr;
	CXTPReportRecords*    pTargetRecords  = nullptr;
	CXTPReportRecordItem* pItem           = nullptr;

	int nRecord = m_vecInfo.size();

	// 2019.04.22 수정 : 추가된 시나리오만 트리에 나오도록 수정
	//for (int i = 0; i < nRecord; ++i)
	//{
	//	KMultiAssignInfo* pDlg    = m_vecInfo[i];
	//	KTarget*          pTarget = pDlg->Target();		

	//	strValue = pTarget->CaptionScenario();

	//	pScenarioRecord = m_wndTree.AddRecord(new CXTPReportRecord);
	//	pItem           = pScenarioRecord->AddItem(new CXTPReportRecordItemText(strValue));
	//	pItem = pScenarioRecord->AddItem(new CXTPReportRecordItem());
	//	pItem->SetEditable(FALSE);
	//	pItem = pScenarioRecord->AddItem(new CXTPReportRecordItem());
	//	pItem->SetEditable(FALSE);

	//	strValue = pTarget->CaptionTargetYear();

	//	pTargetRecords  = pScenarioRecord->GetChilds();
	//	pTargetRecord   = pTargetRecords->Add(new CXTPReportRecord);
	//	pItem           = pTargetRecord->AddItem(new CXTPReportRecordItemText(strValue));
	//	pItem->SetItemData((DWORD_PTR)pDlg);
	//	pItem->HasCheckbox(TRUE);

	//	if (i == 0)
	//	{
	//		pItem->SetChecked(TRUE);
	//		pItem->SetEditable(FALSE);

	//		CString strWormstart;
	//		std::map<KTarget*, KARunningOptionInfo>::iterator itRunningOption = m_mapRunningOption.find(pTarget);

	//		if (m_mapRunningOption.end() != itRunningOption)
	//		{
	//			KARunningOptionInfo& ro = itRunningOption->second;
	//			strWormstart  = ro.Target()->CaptionScenario() + _T("_") + ro.Target()->CaptionTargetYear();
	//		}
	//		pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(_T("선택됨")));
	//		pItem->SetEditable(FALSE);
	//		pItem->SetAlignment(DT_CENTER);
	//		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	//		pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(strWormstart));
	//		pItem->SetEditable(FALSE);
	//		pItem->SetAlignment(DT_CENTER);
	//		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	//	}
	//	else
	//	{
	//		CString strStatus;
	//		if (!CheckTargetRunnable(pTarget, strStatus))
	//		{
	//			pItem->SetEditable(FALSE);
	//			pDlg->SetError(true);
	//			pDlg->SetStatusString(strStatus);
	//		}

	//		pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(strStatus));
	//		pItem->SetEditable(FALSE);
	//		pItem->SetAlignment(DT_CENTER);
	//		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	//		pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText());
	//		pItem->SetEditable(FALSE);
	//		pItem->SetAlignment(DT_CENTER);
	//		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	//	}
	//}
	for (KMultiAssignInfoSOBA* pInfo : m_arrSelectedInfo)
	{
		KTarget*          pTarget = pInfo->Target();
		CString strName;
		strName.Format(_T("%s_%s"), pTarget->CaptionScenario(), pTarget->GetName());

		CXTPReportRecord* pRecord = m_wndTree.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
		pItem->SetItemData((DWORD_PTR)pInfo);
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);

		CString strWormstart;
		KARunningOptionInfo& ro = pInfo->GetRunningOption();
		if(ro.Target() != NULL)
		{
			strWormstart.Format(_T("%s_%s"), ro.Target()->CaptionScenario(), ro.Target()->GetName());
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strWormstart));
		pItem->SetEditable(FALSE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		if (!pInfo->IsBase())
		{
			CXTPReportRecordItemControl* pItemButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			pItemButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemButton->SetCaption(_T("삭제"));
			}
			else
			{
				pItemButton->SetCaption(_T("Remove"));
			}
			pItemButton->SetSize(CSize(70, 0));
		}
		else
		{
			pItem->SetCaption(_T("(기본 설정)"));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);
		}
	}
	
	m_wndTree.Populate();
	//m_wndTree.ExpandAll(TRUE);
}

void MultiAssignModelSOBADlg::InitModeReport( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nItemIndex(0);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Mode"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);			

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Matrix"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	m_wndReport.Populate();
}

void MultiAssignModelSOBADlg::UpdateModeData(KMultiAssignInfoSOBA* a_pInfo )
{
	std::map<int, std::vector<CString>> mapModeChocieColumns;
	KTarget*                pTarget      = a_pInfo->Target();
	std::list<KAInputInfo*> lstInputInfo = a_pInfo->GetInputInfo();

	m_wndReport.ResetContent();

	CXTPReportRecords*			 pRecords	  = m_wndReport.GetRecords();
	CXTPReportRecord*			 pRecord	  = nullptr;
	CXTPReportRecordItem*        pItem   	  = nullptr;
	CXTPReportRecordItemComboEx* pItemCombo   = nullptr;
	KMode*						 pMode	      = nullptr;
	KIOColumn*					 pInputColumn = nullptr;
	CString						 strModeName  (_T(""));
	double						 dOccupancy   (1.0);
	double						 dPCE		  (1.0);
	bool						 bPreLoad	  = false;
	bool						 bSelected	  = false;

	KDBaseAssignment::GetModeChoiceODAtGroupingMode(pTarget, mapModeChocieColumns);
	AutoType iterModeColumns = mapModeChocieColumns.begin();
	AutoType  endModeColumns = mapModeChocieColumns.end();

	std::list<KAInputInfo*>::iterator iterInput; 

	AutoType endInput  = lstInputInfo.end();

	for(iterInput = lstInputInfo.begin(); iterInput != endInput; ++iterInput)
	{
		KAInputInfo* pInput	= *iterInput;
		bSelected			= pInput->Selected();
		pMode				= pInput->Mode();
		strModeName		    = pMode->ModeName();
		pInputColumn		= pInput->InputColumn();
		dOccupancy			= pInput->Occupancy();
		dPCE				= pInput->PCU();
		bPreLoad			= pInput->PreLoad();

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		std::vector<CString> vecModeColumns;
		AutoType findColumn = mapModeChocieColumns.find(pMode->ModeID());
		AutoType endColumne = mapModeChocieColumns.end();
		if (findColumn != endModeColumns)
		{
			vecModeColumns = findColumn->second;
		}

		size_t nColumnCount = vecModeColumns.size();

		if(bSelected == true)
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->SetItemData((DWORD_PTR)pInput);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			// 2019.04.22 수정 : 분석대상으로 추가된 시나리오는 수단을 변경하지 못한다.
			if (nColumnCount < 1)
			{
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				//pItem->SetEditable(TRUE);
				pItem->SetEditable(FALSE);
				pItem->SetChecked(TRUE);
			}
			

			if (pTarget == m_pTarget || a_pInfo->GetError() || a_pInfo->GetSelected())
			{
				pItem->SetEditable(FALSE);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
			pItem->SetItemData((DWORD_PTR)pMode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			KIOTable* pTable = pTarget->Tables()->FindTable(TABLE_MODE_OD);
			const KIOColumns* pColumns	= pTable->Columns();

			int nIndex       = 0;
			int nCurrentIndex   = 0;
			for (size_t i = 0; i < nColumnCount; i++)
			{
				CString strColunmName = vecModeColumns[i];
				KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
				nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);

				CString strCompareColumnName = pColumn->Caption();

                CString strCompareInputName(_T(""));
                if (pInput->InputColumn() != nullptr) {
                    strCompareInputName = pInput->InputColumn()->Caption();
                }

				if(strCompareColumnName == strCompareInputName)
				{
					nCurrentIndex = nIndex;
				}
			}

			pItemCombo->SetCurSel(nCurrentIndex);
			pItemCombo->SetAlignment(xtpColumnTextCenter);
			pItemCombo->SetBackgroundColor(RGB(255,255,255));
			pItemCombo->SetEditable(TRUE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(255,255,255));
			pItem->SetEditable(TRUE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(255,255,255));
			pItem->SetEditable(TRUE);

			pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
			pItem->SetItemData(pInput->NetworkType());

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetEditable(TRUE);
			if(bPreLoad == true)
			{
				pItem->SetChecked(TRUE);
			}	
		}
		else
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetItemData((DWORD_PTR)pInput);
			pItem->SetAlignment(xtpColumnIconCenter);
			if (nColumnCount < 1)
			{
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				//pItem->SetEditable(TRUE);
				pItem->SetEditable(FALSE);
				pItem->SetChecked(FALSE);
			}

			if (pTarget == m_pTarget || a_pInfo->GetError() || a_pInfo->GetSelected())
			{
				pItem->SetEditable(FALSE);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
			pItem->SetItemData((DWORD_PTR)pMode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			KIOTable* pTable = pTarget->Tables()->FindTable(TABLE_MODE_OD);
			const KIOColumns* pColumns	= pTable->Columns();

			int nSelIndex       = 0;
			for (size_t i = 0; i < nColumnCount; i++)
			{
				CString strColunmName = vecModeColumns[i];
				KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
				int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
			}
			pItemCombo->SetCurSel(nSelIndex);
			pItemCombo->SetAlignment(xtpColumnTextCenter);
			pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemCombo->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CCustomReportRecordItem/*CXTPReportRecordItem*/);
			pItem->SetItemData(pInput->NetworkType());

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);
		}

	}
	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
}

void MultiAssignModelSOBADlg::UpdateOutputData(KMultiAssignInfoSOBA* a_pInfo )
{
	KTarget* pTarget = a_pInfo->Target();

	m_nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(pTarget);

	InitCboOverWrite(pTarget);
	InitOutputInfo(a_pInfo);
}

void MultiAssignModelSOBADlg::SaveModeInfo()
{
	//설정정보를 Current KMultiAssignInfo* m_pInfo;에 저장함

	CXTPReportRecords* pRecords	= m_wndReport.GetRecords();
	int                nRecord  = pRecords->GetCount();

	for(int i = 0; i < nRecord; i++)
	{
		CXTPReportRecord*     pRecord  = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem	   = pRecord->GetItem(0);
		KAInputInfo*          pInfo	   = (KAInputInfo*)pItem->GetItemData();

		if(pRecord->GetItem(0)->IsChecked() == FALSE)
		{
			pInfo->Selected(false);
			CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)(pRecord->GetItem(1));
			KMode*					  pMode	    = (KMode*)pTextItem->GetItemData();
			pInfo->Mode(pMode);

			CXTPReportRecordItemComboEx* pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(2));
			KIOColumn*					 pColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());
			pInfo->InputColumn(pColumn);
		}
		else
		{
			pInfo->Selected(true);
			CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)(pRecord->GetItem(1));
			KMode*					  pMode	    = (KMode*)pTextItem->GetItemData();
			pInfo->Mode(pMode);

			CXTPReportRecordItemComboEx* pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(2));
			KIOColumn*					 pColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());
			pInfo->InputColumn(pColumn);	
		}
	}
}

void MultiAssignModelSOBADlg::SetDefualtDescription( void )
{
	switch(m_emAModel)
	{
	case KEMAssignmentModelAllorNothing:
		m_strDescription = _T("All-or-Nothing");
		break;
	case KEMAssignmentModelIncremental:
		m_strDescription = _T("Incremental");
		break;
	case KEMAssignmentModelUserEquilibriumFW:
		m_strDescription = _T("UserEquilibrium_FW");
		break;
	case KEMAssignmentModelUserEquilibriumOB:
		m_strDescription = _T("UserEquilibrium_OB");
		break;
	case KEMAssignmentModelMultiModal:
		m_strDescription = _T("Multi-Modal");
		break;
	default:
		m_strDescription = _T("");
		break;
	}
}

void MultiAssignModelSOBADlg::InitCboOverWrite( KTarget* a_pTarget )
{
	KDBaseAssignment::GetRunHighwayAssignInfo(a_pTarget, m_vecAssignResultInfo);

	int     nItemIndex;
	int     nSize = m_vecAssignResultInfo.size();
	CString strMsg;

	m_cboOverwriteInfo.ResetContent();
	for (int i = 0; i < nSize; ++i)
	{
		TAssignResultInfo& oAssignResultInfo = m_vecAssignResultInfo[i];
		strMsg.Format(_T("%s(%s~%s)"), oAssignResultInfo.TstrDescription, oAssignResultInfo.TstrStartDate, oAssignResultInfo.TstrEndDate);
		nItemIndex = m_cboOverwriteInfo.AddString(strMsg);
		m_cboOverwriteInfo.SetItemData(nItemIndex, (DWORD_PTR)oAssignResultInfo.TnRunSeq);
	}
}

void MultiAssignModelSOBADlg::InitOutputInfo(KMultiAssignInfoSOBA* a_pInfo )
{
	TAssignResultInfo& oAssign = a_pInfo->m_TOverWirteAResultInfo;
	

	CButton* pNewCheck = (CButton*)GetDlgItem(IDC_RADIO_NEW);
	pNewCheck->SetCheck(0);
	CButton* pOverCheck = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	pOverCheck->SetCheck(0);



	if ( oAssign.TbUseData == false)
	{
		if (m_vecAssignResultInfo.size() > 0)
		{
			pOverCheck->EnableWindow(TRUE);
			pOverCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(0);
			m_cboOverwriteInfo.EnableWindow(TRUE);
			ChangeComboBox();
		}
		else
		{
			pNewCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
			pEditBox->SetWindowTextW(strDesciption);
			pOverCheck->EnableWindow(FALSE);
		}
	}
	else
	{
		if (a_pInfo->m_bUseOverWrite == false)
		{
			pNewCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(-1);
			m_cboOverwriteInfo.EnableWindow(FALSE);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
			pEditBox->SetWindowTextW(strDesciption);
			//pEditBox->SetWindowTextW(m_oAssign.TDescription);
		}
		else
		{
			pOverCheck->SetCheck(1);
			m_cboOverwriteInfo.SetCurSel(oAssign.TnRunSeq - 1);
			CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
			pEditBox->SetWindowTextW(oAssign.TstrDescription);
		}

	}
}

void MultiAssignModelSOBADlg::ChangeComboBox( void )
{
	int nSeq = 0;

	int nCurSel = m_cboOverwriteInfo.GetCurSel();

	if (nCurSel < 0)
	{
		nCurSel = 0;
	}

	m_cboOverwriteInfo.SetCurSel(nCurSel);

	nSeq = m_cboOverwriteInfo.GetItemData(nCurSel);

	for (int i=0; i<(int)m_vecAssignResultInfo.size(); i++)
	{
		TAssignResultInfo& oAssignResultInfo = m_vecAssignResultInfo[i];
		if (nSeq == oAssignResultInfo.TnRunSeq)
		{
			CString strDesciption = _T("");
			strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
			SetDlgItemText(IDC_EDIT_TEXT, strDesciption);
		}
	}
}

void MultiAssignModelSOBADlg::OnBnClickedRadioNew( void )
{
	CButton* pOverCheck = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);
	pOverCheck->SetCheck(0);
	m_cboOverwriteInfo.SetCurSel(-1);
	m_cboOverwriteInfo.EnableWindow(FALSE);
	CEdit *pEditBox=(CEdit *)GetDlgItem(IDC_EDIT_TEXT);
	CString strDesciption = _T("");
	strDesciption.Format(_T("%s(%d)"),m_strDescription, m_nRunSeq);
	pEditBox->SetWindowTextW(strDesciption);
}

void MultiAssignModelSOBADlg::OnBnClickedRadioOverwrite( void )
{
	CButton* pNewCheck = (CButton*)GetDlgItem(IDC_RADIO_NEW);
	pNewCheck->SetCheck(0);

	m_cboOverwriteInfo.EnableWindow(TRUE);
	ChangeComboBox();
}

void MultiAssignModelSOBADlg::SaveOutputInfo( void )
{
	TAssignResultInfo& oAssign = m_pInfo->m_TOverWirteAResultInfo;
	oAssign.TbUseData = true;

	CString strEditBoxName = _T("");

	GetDlgItemText(IDC_EDIT_TEXT, strEditBoxName);

	bool bCheckRadio = true;

	CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_NEW);

	if (pCheck->GetCheck() == 1)
	{
		bCheckRadio = false;
	}

	int nCurSel = m_cboOverwriteInfo.GetCurSel();
	
	if (nCurSel < 0 || bCheckRadio == false )
	{
		oAssign.TstrDescription	 = strEditBoxName;
		m_pInfo->m_bUseOverWrite = false;
		return;
	}

	int nSeq = m_cboOverwriteInfo.GetItemData(nCurSel);

	for (int i=0 ;  i<(int)m_vecAssignResultInfo.size() ; i++ )
	{
		TAssignResultInfo& oAssignResultInfo = m_vecAssignResultInfo[i];
		if (nSeq == oAssignResultInfo.TnRunSeq)
		{
			oAssignResultInfo.TstrDescription	= strEditBoxName;
			//oAssign                             = oAssignResultInfo;
			//info
			oAssign.TbUseData = oAssignResultInfo.TbUseData;
			oAssign.TbUseOverWrite = oAssignResultInfo.TbUseOverWrite;
			oAssign.TnRunSeq= oAssignResultInfo.TnRunSeq;
			oAssign.TstrStartDate = oAssignResultInfo.TstrStartDate;
			oAssign.TstrEndDate = oAssignResultInfo.TstrEndDate;
			oAssign.TnType = oAssignResultInfo.TnType;
			oAssign.TstrDescription = oAssignResultInfo.TstrDescription;

			//link
			oAssign.TstrAverageTime = oAssignResultInfo.TstrAverageTime;
			oAssign.TstrVC = oAssignResultInfo.TstrVC;
			oAssign.TstrSumVolumn = oAssignResultInfo.TstrSumVolumn;
			oAssign.TstrTrip = oAssignResultInfo.TstrTrip;
			oAssign.TstrModeVolume = oAssignResultInfo.TstrModeVolume;//
			oAssign.TstrVDFTime = oAssignResultInfo.TstrVDFTime;//
			oAssign.TstrAVETimeToll = oAssignResultInfo.TstrAVETimeToll;
			oAssign.TstrVDFTimeToll = oAssignResultInfo.TstrVDFTimeToll;//
			oAssign.TstrBusInitVolume = oAssignResultInfo.TstrBusInitVolume;

			//od paramter
			oAssign.TstrODAveTime = oAssignResultInfo.TstrODAveTime;
			oAssign.TstrODVDFTime = oAssignResultInfo.TstrODVDFTime;//
			oAssign.TstrODAveBus = oAssignResultInfo.TstrODAveBus;
			oAssign.TstrODAveSubway = oAssignResultInfo.TstrODAveSubway;
			oAssign.TstrODAveBusSubway = oAssignResultInfo.TstrODAveBusSubway;
			oAssign.TstrODBusDirect = oAssignResultInfo.TstrODBusDirect;
			oAssign.TstrODBusOneTrans = oAssignResultInfo.TstrODBusOneTrans;
			oAssign.TstrODBusTwoTrans = oAssignResultInfo.TstrODBusTwoTrans;

			//od
			oAssign.TstrODMBus = oAssignResultInfo.TstrODMBus;
			oAssign.TstrODMRail = oAssignResultInfo.TstrODMRail;
			oAssign.TstrODMBusRail = oAssignResultInfo.TstrODMBusRail;
			oAssign.TstrODMInterMode = oAssignResultInfo.TstrODMInterMode;

			//lineInfo
			oAssign.TstrLInfoPassengernum = oAssignResultInfo.TstrLInfoPassengernum;
			oAssign.TstrLinfoTotalLength = oAssignResultInfo.TstrLinfoTotalLength;
			oAssign.TstrLinfoAvgLength = oAssignResultInfo.TstrLinfoAvgLength;
			oAssign.TstrLinfoMaxPassenger = oAssignResultInfo.TstrLinfoMaxPassenger;
			oAssign.LinfoAvePassenger = oAssignResultInfo.LinfoAvePassenger;
			oAssign.TstrLinfoMaxSecond = oAssignResultInfo.TstrLinfoMaxSecond;
			oAssign.TstrLinfoAvgSecond = oAssignResultInfo.TstrLinfoAvgSecond;
			oAssign.TstrLinfoMaxStanding = oAssignResultInfo.TstrLinfoMaxStanding;
			oAssign.TstrLinfoAvgStanding = oAssignResultInfo.TstrLinfoAvgStanding;
			oAssign.TstrLinfoMaxCongestion = oAssignResultInfo.TstrLinfoMaxCongestion;
			oAssign.TstrLinfoAvgCongestion = oAssignResultInfo.TstrLinfoAvgCongestion;
			oAssign.TstrLinfoKMPassenger = oAssignResultInfo.TstrLinfoKMPassenger;
			oAssign.TstrLinfoOncePassenger = oAssignResultInfo.TstrLinfoOncePassenger;
			oAssign.TstrLinfoKMOncePassenger = oAssignResultInfo.TstrLinfoKMOncePassenger;

			//line
			oAssign.TstrLineBoradNet = oAssignResultInfo.TstrLineBoradNet;
			oAssign.TstrLineBoradTransfer = oAssignResultInfo.TstrLineBoradTransfer;
			oAssign.TstrLineBoradTotal = oAssignResultInfo.TstrLineBoradTotal;
			oAssign.TstrLineAlightNet = oAssignResultInfo.TstrLineAlightNet;
			oAssign.TstrLineAlightTransfer = oAssignResultInfo.TstrLineAlightTransfer;
			oAssign.TstrLineAlightTotal = oAssignResultInfo.TstrLineAlightTotal;
			oAssign.TstrLinePassenger = oAssignResultInfo.TstrLinePassenger;

			//Node
			oAssign.TstrNodeBusNetBorad = oAssignResultInfo.TstrNodeBusNetBorad;
			oAssign.TstrNodeBusTransferBoard = oAssignResultInfo.TstrNodeBusTransferBoard;
			oAssign.TstrNodeBusSumBoard = oAssignResultInfo.TstrNodeBusSumBoard;
			oAssign.TstrNodeBusNetAlight = oAssignResultInfo.TstrNodeBusNetAlight;
			oAssign.TstrNodeBusTransferAlight = oAssignResultInfo.TstrNodeBusTransferAlight;
			oAssign.TstrNodeBusSumAlight = oAssignResultInfo.TstrNodeBusSumAlight;

			oAssign.TstrNodePRAuto = oAssignResultInfo.TstrNodePRAuto;
			oAssign.TstrNodePRBus = oAssignResultInfo.TstrNodePRBus;
			oAssign.TstrNodePRRail = oAssignResultInfo.TstrNodePRRail;
			oAssign.TstrNodePRTransit = oAssignResultInfo.TstrNodePRTransit;

			//Terminal
			oAssign.TstrTerminalTrip = oAssignResultInfo.TstrTerminalTrip;

			//Terminal Result
			oAssign.TstrTR_AC_Auto = oAssignResultInfo.TstrTR_AC_Auto;
			oAssign.TstrTR_AC_Taxi = oAssignResultInfo.TstrTR_AC_Taxi;
			oAssign.TstrTR_AC_Bus = oAssignResultInfo.TstrTR_AC_Bus;
			oAssign.TstrTR_AC_Subway = oAssignResultInfo.TstrTR_AC_Subway;
			oAssign.TstrTR_AC_Transit = oAssignResultInfo.TstrTR_AC_Transit;
			oAssign.TstrTR_ER_Auto = oAssignResultInfo.TstrTR_ER_Auto;
			oAssign.TstrTR_ER_Taxi = oAssignResultInfo.TstrTR_ER_Taxi;
			oAssign.TstrTR_ER_Bus = oAssignResultInfo.TstrTR_ER_Bus;
			oAssign.TstrTR_ER_Subway = oAssignResultInfo.TstrTR_ER_Subway;
			oAssign.TstrTR_ER_Transit = oAssignResultInfo.TstrTR_ER_Transit;
			oAssign.TstrTR_Total = oAssignResultInfo.TstrTR_Total;

			m_pInfo->m_bUseOverWrite = true;
		}
	}
}

void MultiAssignModelSOBADlg::OnBnClickedRun()
{
	try
	{
		if (m_pInfo != nullptr)
		{
			SaveModeInfo();
			SaveOutputInfo();
		}

		ValidateInput();

		m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
		bool bEnableStop = true;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicListSimpleProgressThread::ExecuteThread(MultiAssignModelThreadProc, this, bEnableStop, nLang);

		if (m_bIsUserStop || m_bRunFailed)
			return;

		if (m_cboName.GetCurSel() == 0)
		{
			SaveMultiScenarioInfo();
		}
		else
		{
			EditMultiScenarioInfo();
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	KDialogEx::OnOK();
}


void MultiAssignModelSOBADlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}

void MultiAssignModelSOBADlg::AddStatusMessage( CString strMsg )
{
	QBicListSimpleProgressPtr spProgressWindow = QBicListSimpleProgressThread::SimpleProgress();
	
	if (spProgressWindow)
		spProgressWindow->SetStatus(CString(strMsg));
}

unsigned __stdcall MultiAssignModelSOBADlg::MultiAssignModelThreadProc( void* pParam )
{
	QBicListSimpleProgressParameter* pParameter = (QBicListSimpleProgressParameter*)pParam;
	MultiAssignModelSOBADlg*            pDlg       = (MultiAssignModelSOBADlg*)pParameter->GetParameter();;
	pDlg->RunMultiAssignModel();
	
	return 0;
}

void MultiAssignModelSOBADlg::RunMultiAssignModel( void )
{
	m_bIsUserStop = false;

	// 2019.04.22 수정 : 분석할 시나리오를 트리에 추가하는 방식으로 변경
	//CXTPReportRecord*     pScenarioRecord = nullptr;
	//CXTPReportRecords*    pTargetRecords  = nullptr;
	//CXTPReportRecord*     pTargetRecord   = nullptr;
	//CXTPReportRecordItem* pItem           = nullptr;
	//CString               strLog;

	//int nRecord = m_wndTree.GetRecords()->GetCount();

	//for (int i = 0; i < nRecord; ++i)
	//{
	//	pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
	//	pTargetRecords  = pScenarioRecord->GetChilds();

	//	int nTargetRecord = pTargetRecords->GetCount();

	//	for (int j = 0; j < nTargetRecord; ++j)
	//	{
	//		if (m_bIsUserStop)
	//		{
	//			TxLogDebug(_T("UserStop"));
	//			return;
	//		}


	//		pTargetRecord = pTargetRecords->GetAt(j);
	//		pItem         = pTargetRecord->GetItem(0);

	//		if (!pItem->IsChecked())
	//			continue;

	//		KMultiAssignInfo* pDlg = (KMultiAssignInfo*)pItem->GetItemData();

	//		KTarget* pTarget = pDlg->Target();
	//		
	//		//AddLog
	//		strLog.Format(_T(">> Multi Scenario Assignment Start - %s(%d) <<"), pTarget->CaptionScenario(), pTarget->GetTargetYear());
	//		AddStatusMessage(strLog);

	//		RunMultiAssignModelSelected(pDlg);
	//	}
	//}
	m_bRunFailed = false;
	CString               strLog;
	int nRecord = m_wndTree.GetRecords()->GetCount();
	KTarget* pLastRunTarget = NULL;
	for (int i = 0; i < nRecord; ++i)
	{
		// 2019.04.24 수정 : 여러 시나리오 구동 중 오류가 발생하면 나머지 시나리오의 수행을 중단
		if (m_bRunFailed)
		{
			if (NULL != pLastRunTarget)
			{
				strLog.Format(_T(">> Multi Scenario Assignment Failed - %s(%d) <<"), pLastRunTarget->CaptionScenario(), pLastRunTarget->GetTargetYear());
			}
			else
			{
				strLog = _T(">> Multi Scenario Assignment Failed <<");
			}
			AddStatusMessage(strLog);
			TxLogDebug(strLog);
			break;
		}

		CXTPReportRecord* pRecord = m_wndTree.GetRecords()->GetAt(i);

		if (m_bIsUserStop)
		{
			TxLogDebug(_T("UserStop"));
			return;
		}

		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		KMultiAssignInfoSOBA* pInfo = (KMultiAssignInfoSOBA*)pItem->GetItemData();

		KTarget* pTarget = pInfo->Target();
		pLastRunTarget = pTarget;

		//AddLog
		strLog.Format(_T(">> Multi Scenario Assignment Start - %s(%d) <<"), pTarget->CaptionScenario(), pTarget->GetTargetYear());
		AddStatusMessage(strLog);

		RunMultiAssignModelSelected(pInfo);
	}
}

void MultiAssignModelSOBADlg::DeleteBushFile( void )
{
	try
	{
		CString strTargetLocation = m_pRunTarget->GetLocation();
		CString strTempLocation = strTargetLocation + _T("\\") + BUSH_PATHVOLUMN_FOLDER_NAME;

		DeletePath(strTempLocation);

		KDBaseAssignment::DeleteAssignPathInfo(m_pRunTarget->GetDBaseConnection());
		KDBaseAssignment::DeleteAssignTurnVolumnInfo(m_pRunTarget->GetDBaseConnection());
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

void MultiAssignModelSOBADlg::DeletePath( CString a_strPath )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(a_strPath.Right(1) != _T("\\"))
		a_strPath += _T("\\");

	a_strPath += _T("*.*");
	bContinue = finder.FindFile(a_strPath);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeletePath(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}

	finder.Close();

	a_strPath = a_strPath.Left(a_strPath.ReverseFind('\\'));  
	::RemoveDirectory((LPCTSTR)a_strPath);
}

void MultiAssignModelSOBADlg::RunMultiAssignModelSelected(KMultiAssignInfoSOBA* a_pInfo )
{
	m_pRunTarget = a_pInfo->Target();
	m_InputList.clear();
	m_InputList = a_pInfo->GetInputInfo();

	
	KARunningOptionInfo& runningOption = a_pInfo->GetRunningOption();


	DeleteBushFile();

	CString strModelFolder;
	strModelFolder.Format(_T("%s%s"), m_pRunTarget->GetLocation(), DLL_READ_WRITE_PATH);

	CreateDirectory(strModelFolder, NULL);
	CreateDirectory(ConvertSavePath(_T("")), NULL);

	TxLogDebugStartMsg();
	// Step 1: Argument 생성
	AddStatusMessage(_T("> Creating Argument File..."));
	KIOColumns  modeColumnCollection;

	CString strNetworkCodeFolder(_T(""));
	strNetworkCodeFolder.Format(_T("%s\\%s"), DLL_MODEL_FOLDER_NAME, DLL_HIGHWAYASSIGN_FOLDER_NAME);

	try 
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pRunTarget, strNetworkCodeFolder);
	} catch (...) {
		m_bRunFailed = true;
		TxLogDebugWarning();
		return;
	}

	if (m_emAModel == KEMAssignmentModelUserEquilibriumOB)
	{
		if (QBicWindowSystem::ProcessOnWindowsStatus() < 1)
		{
			AddStatusMessage(_T("> 32bit Process"));
		}
		else
		{
			AddStatusMessage(_T("> 64bit Process"));
		}

		if ( ! CreateOBArgument(modeColumnCollection, a_pInfo->m_TOverWirteAResultInfo, runningOption) )
		{
			m_bRunFailed = true;
			TxLogDebugWarning();
			return;
		}
	}
	else
	{
		if ( ! CreateArgument(modeColumnCollection, a_pInfo->m_TOverWirteAResultInfo, runningOption) )
		{
			m_bRunFailed = true;
			TxLogDebugWarning();
			return;
		}
	}

	AddStatusMessage(_T("> Create Argument File End."));

	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		m_bIsUserStop = true;
		return;
	}

	// Step 2: Dll Load
	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	AddStatusMessage(_T("> Assignment Start ..."));

	CTime timeStart = CTime::GetCurrentTime();

	CTime timeDoneDll = CTime::GetCurrentTime();
	CString strDllStartTime = _T("");
	strDllStartTime.Format(_T("[Start][%s]"), timeDoneDll.Format(_T("%Y-%m-%d %H:%M:%S")));
	//iteDllLog(strDllStartTime);

	// 라이브러리 로드
	if(KEMAssignmentModelUserEquilibriumOB == m_emAModel)
	{
		//OBUE Single 일때
		if (m_nModeInVDF < 2)
		{
			if (QBicWindowSystem::ProcessOnWindowsStatus() < 1)
			{
				//32bit
				//if (m_RunningOption.IncludeWarmStart() == true)
				if (runningOption.IncludeWarmStart() == true)
				{
					//Warm Start 일때
					KLauncher::Run(strWindowName, 31);
				}
				else
				{
					//Not Warm Start
					KLauncher::Run(strWindowName, 34);
				}

				if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
				{
					//AddStatusMessage(_T("> OBUE Assignment Complete"));
				}
				else if (DLL_LOAD_ERROR == m_nResultCode)
				{
					AddStatusMessage(_T("> Loading DLL File Error."));
					m_bRunFailed = true;
					return;
				}
				else if (DLL_USER_STOPPING == m_nResultCode)
				{
					AddStatusMessage(_T("> Assignment Stopped..."));
					return;
				}
				else
				{
					if (QBicListSimpleProgressThread::IsUserStopped())
					{
						AddStatusMessage(_T("> User Stopped..."));
						m_bIsUserStop = true;
						TxLogDebug(_T("User Stop.."));
					}
					else
					{
						AddStatusMessage(_T("> Assignment Module End with Error..."));
						m_bRunFailed = true;
						TxLogDebug(_T("Assignment Run Error.."));
					}
					return;
				}

				//Balancing 실행여부 Balancing 2개의 Dll 사용
				//if (m_RunningOption.m_nIteration != 0)
				if (runningOption.m_nIteration != 0)
				{
					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run(strWindowName, 35);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P1 Complete..."));
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						m_bRunFailed = true;
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							m_bIsUserStop = true;
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							m_bRunFailed = true;
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}

					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run(strWindowName, 36);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P2 Complete..."));
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						m_bRunFailed = true;
						return;
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							m_bIsUserStop = true;
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							m_bRunFailed = true;
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}
				}
			}
			else
			{
				//64bit
				//if (m_RunningOption.IncludeWarmStart() == true)
				if (runningOption.IncludeWarmStart() == true)
				{
					// Warm Start 
					KLauncher::Run64(strWindowName, 1004);
				}
				else
				{
					//Not Warm Start
					KLauncher::Run64(strWindowName, 1001);
				}

				if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
				{
					//AddStatusMessage(_T("> OBUE Assignment Complete"));
				}
				else if (DLL_LOAD_ERROR == m_nResultCode)
				{
					AddStatusMessage(_T("> Loading DLL File Error."));
					m_bRunFailed = true;
					return;
				}
				else if (DLL_USER_STOPPING == m_nResultCode)
				{
					AddStatusMessage(_T("> Assignment Stopped..."));
					return;
				}
				else
				{
					if (QBicListSimpleProgressThread::IsUserStopped())
					{
						AddStatusMessage(_T("> User Stopped..."));
						m_bIsUserStop = true;
						TxLogDebug(_T("User Stop.."));
					}
					else
					{
						AddStatusMessage(_T("> Assignment Module End with Error..."));
						m_bRunFailed = true;
						TxLogDebug(_T("Assignment Run Error.."));
					}
					return;
				}

				//Balancing 실행여부 Balancing 2개의 Dll 사용 
				//if (m_RunningOption.m_nIteration != 0)
				if (runningOption.m_nIteration != 0)
				{
					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run64(strWindowName, 1002);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P1 Complete..."));
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						m_bRunFailed = true;
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							m_bIsUserStop = true;
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							m_bRunFailed = true;
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}

					m_nResultCode = DLL_UNEXPECTED_ERROR;
					KLauncher::Run64(strWindowName, 1003);
					if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
					{
						//AddStatusMessage(_T(">Balancing P2 Complete..."));
					}
					else if (DLL_LOAD_ERROR == m_nResultCode)
					{
						AddStatusMessage(_T("> Loading DLL File Error."));
						m_bRunFailed = true;
						return;
					}
					else if (DLL_USER_STOPPING == m_nResultCode)
					{
						AddStatusMessage(_T("> Assignment Stopped..."));
						return;
					}
					else 			
					{
						if (QBicListSimpleProgressThread::IsUserStopped())
						{
							AddStatusMessage(_T("> User Stopped..."));
							m_bIsUserStop = true;
							TxLogDebug(_T("User Stop.."));
						}
						else
						{
							AddStatusMessage(_T("> Assignment Module End with Error..."));
							m_bRunFailed = true;
							TxLogDebug(_T("Assignment Run Error.."));
						}
						return;
					}
				}
			}
		}
		else
		{
			//OBUE Muliti 일때

            // 32bit
            if (QBicWindowSystem::ProcessOnWindowsStatus() < 1) {

                KLauncher::Run(strWindowName, 4);
            }
            else {

                KLauncher::Run64(strWindowName, 1007);
            }
		}
	} 
	else
	{
		//일반 Assign
		KLauncher::Run(strWindowName, 2);
	}


	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		m_bIsUserStop = true;
		return;
	}

	timeDoneDll = CTime::GetCurrentTime();
	CString strDllEndTime = _T("");
	strDllEndTime.Format(_T("[ End ][%s]"), timeDoneDll.Format(_T("%Y-%m-%d %H:%M:%S")));
	//WriteDllLog(strDllEndTime);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		AddStatusMessage(_T("> Assignment End."));
		TxLogDebug(_T("Model Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		TxLogDebug(_T("Model Stop.."));
		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Assignment DLL Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));
		m_bRunFailed = true;
		return;
	}
	else
	{
		if (QBicListSimpleProgressThread::IsUserStopped())
		{
			AddStatusMessage(_T("> User Stopped..."));
			m_bIsUserStop = true;
			TxLogDebug(_T("User Stop.."));
		}
		else
		{
			AddStatusMessage(_T("> Assignment Module End with Error..."));
			m_bRunFailed = true;
			TxLogDebug(_T("Model Run Error.."));
		}
		return;
	}

	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> Assignment Stopped..."));
		m_bIsUserStop = true;
		return;
	}

	/*Step 5. 결과 정보 저장*/
	AddStatusMessage(_T("> Saving Output Data..."));

	/*Step 5.1 Column 추가*/
	//AddStatusMessage(_T("> Column 추가 ..."));
	KIOColumns         rLinkColumnCollection, rParaODColumnCollection;
	TAssignResultColumnName oresultColumnName;
	InitTAssingReulstColumn(oresultColumnName);

	int nRunSeq(0);

	CString strDescription = _T("");
	strDescription = a_pInfo->m_TOverWirteAResultInfo.TstrDescription;	

	try
	{
		nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pRunTarget);

		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB )
		{
			if (m_nModeInVDF < 2)
			{
				AddAssignOBSingeResultColumn(nRunSeq, rLinkColumnCollection, rParaODColumnCollection, 
					oresultColumnName, strDescription, runningOption);
			}
			else
			{
				AddAssignOBMultiResultColumn(nRunSeq, rLinkColumnCollection, rParaODColumnCollection, 
					oresultColumnName, strDescription, runningOption);
			}
		}
		else
		{
			AddAssignResultColumn(nRunSeq, rLinkColumnCollection, rParaODColumnCollection, oresultColumnName, strDescription);
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AfxMessageBox(_T("데이터 컬럼 추가 시 예상치 못한 오류가 발생하였습니다."));
		m_bRunFailed = true;
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AfxMessageBox(_T("데이터 컬럼 추가 시 예상치 못한 오류가 발생하였습니다."));
		m_bRunFailed = true;
		return;
	}

	/*Step 5.3 UpdateDataBase*/
	KDBaseConPtr spDBaseConnection = m_pRunTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if (QBicListSimpleProgressThread::IsUserStopped() == true)
		{
			AddStatusMessage(_T("> Assignment Stopped..."));
			m_bIsUserStop = true;
			ThrowException(_T("Assignment Stopped..."));
		}

		/*Step 5.3.1 링크 정보 입력 ...*/		
		//추가 
		AddStatusMessage(_T("> Saving Output Data(Link Table)..."));
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB )
		{
			if (m_nModeInVDF < 2)
			{
				UpdateOBSingleLinkFile2DB(spDBaseConnection, rLinkColumnCollection, runningOption);
			}
			else
			{
				UpdateOBMultiLinkFile2DB(spDBaseConnection, rLinkColumnCollection, runningOption);
			}
		}
		else
		{
			UpdateLinkFile2DB(spDBaseConnection, rLinkColumnCollection);
		}

		if (QBicListSimpleProgressThread::IsUserStopped() == true)
		{
			AddStatusMessage(_T("> Assignment Stopped..."));
			m_bIsUserStop = true;
			ThrowException(_T("Assignment Stopped..."));
		}

		/*Step 5.3.2 수단 OD 정보 입력 ...*/
		AddStatusMessage(_T("> Saving Output Data(Attribute Table)..."));
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB )
		{
			if (m_nModeInVDF < 2)
			{
				UpdateOBSingleModeODFile2DB(spDBaseConnection, rParaODColumnCollection);
			}
			else
			{
				UpdateOBMultiModeODFile2DB(spDBaseConnection, rParaODColumnCollection);
			}
		}
		else
		{
			UpdateModeODFile2DB(spDBaseConnection, rParaODColumnCollection);
		}

		if (QBicListSimpleProgressThread::IsUserStopped() == true)
		{
			AddStatusMessage(_T("> Assignment Stopped..."));
			m_bIsUserStop = true;
			ThrowException(_T("Assignment Stopped..."));
		}

		/*Step 5.3.4. 배정모형 종료정보 업데이트 ...*/
		CTime timeEnd = CTime::GetCurrentTime();

		nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pRunTarget);

		if (a_pInfo->m_bUseOverWrite == true)
		{
			//OverWrite일경우 기존 데이터 삭제
			DeleteOverWriteAssignModel(spDBaseConnection, a_pInfo);
		}

		int nType = 1;
		if (m_emAModel == KEMAssignmentModelIncremental)
		{
			nType = 2;
		}
		else if (m_emAModel == KEMAssignmentModelUserEquilibriumFW)
		{
			nType = 3;
		}
		else if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelUserEquilibriumOBMulti)
		{
			nType = 4;
		}

		//AssignModel 정보 입력
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB )
		{
			if (m_nModeInVDF < 2)
			{
				KDBaseAssignment::UpdateOBSingleARunSeq(spDBaseConnection, nRunSeq, oresultColumnName, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")),
					strDescription, nType);
			}
			else
			{
				KDBaseAssignment::UpdateOBMultiARunSeq(spDBaseConnection, nRunSeq, oresultColumnName, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")),
					strDescription, nType);
			}
		}
		else
		{
			KDBaseAssignment::UpdateAssignRunSeq(spDBaseConnection, nRunSeq, oresultColumnName, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")), 
				strDescription, nType);
		}

		int nIncludeTurn = 0;
		//if (m_RunningOption.IncludeTurnRestriction() == true)
		if (runningOption.IncludeTurnRestriction() == true)
		{
			nIncludeTurn = 1;
		}

		//Include Path Analysis 일 경우 추가 정보 DB에 Write
		//if (m_RunningOption.IncludePathAnalysis())
		if (runningOption.IncludePathAnalysis())
		{
			KDBaseAssignment::UpdateAssignPathmodel(spDBaseConnection, nRunSeq, nIncludeTurn, m_nUEOBaseZoneCentroidThrought ,oresultColumnName.AverageTime, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
			KDBaseAssignment::UpdateAssignpathInfo(spDBaseConnection, m_InputList, oresultColumnName);
		}

		//Turn Volume 정보 저장을 위하여 DB에 Write
		//KDBaseAssignment::UpdateAssignTurnVolumeInfo(spDBaseConnection, m_InputList, m_RunningOption);
		KDBaseAssignment::UpdateAssignTurnVolumeInfo(spDBaseConnection, m_InputList, runningOption);

		rLinkColumnCollection.Clear();
		rParaODColumnCollection.Clear();
		spDBaseConnection->Commit();

	}
	catch(KExceptionPtr ex)
	{
		m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AddStatusMessage(ex->GetErrorMessage());
		m_bRunFailed = true;
		return;
	}
	catch(...)
	{
		m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
		spDBaseConnection->RollBack();
		DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection);
		AddStatusMessage(_T("Assignment Running Failed."));
		TxLogDebugException();
		m_bRunFailed = true;
		return;
	}

	try
	{
		/*Column 삭제*/
		if (true == DeleteCreateColumn(rLinkColumnCollection, rParaODColumnCollection))
		{
			if (a_pInfo->m_bUseOverWrite == true)
			{
				KDBaseAssignment::DeleteOverWriteColumn(m_pRunTarget, a_pInfo->m_TOverWirteAResultInfo);
			}
		}

		KIOTable* pLinkTable = m_pRunTarget->Tables()->FindTable(TABLE_LINK);
		pLinkTable->Notify();
		KIOTable* pModeODTable = m_pRunTarget->Tables()->FindTable(TABLE_PARAMETER_OD);
		pModeODTable->Notify();

		AddStatusMessage(_T("> Assignment Complete."));
		TxLogDebugEndMsg();
	}
	catch(...)
	{
		TxLogDebugException();
		m_bRunFailed = true;
	}	


	if (DLL_PLANNED_TERMINATION == m_nResultCode)
	{
		if (m_emAModel == KEMAssignmentModelUserEquilibriumOB || m_emAModel == KEMAssignmentModelSegment)
		{//32Bit 또는 64Bit / VDF(멀티)인지 확인 필요-멀티는 무조건 32Bit(18.07.13 일자로 FOriginBasedUE_MC_64.dll 추가됨에 따라 멀티도 64bit 가능)
			if (QBicWindowSystem::ProcessOnWindowsStatus() < 1)
			{
				CopyFile(_T("assignment.arg"), ConvertSavePath(_T("assignment.arg")), FALSE);
			}
			else
			{
				CString strFileName = ImChampDir::GetAppDirectory() + _T("\\x64\\assignment.arg");
				CopyFile(strFileName, ConvertSavePath(_T("assignment.arg")), FALSE);
			}
		}
		else
		{
			CopyFile(_T("assignment.arg"), ConvertSavePath(_T("assignment.arg")), FALSE);
		}
	}
}

CString MultiAssignModelSOBADlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s\\%s"), m_pRunTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_HIGHWAYASSIGN_FOLDER_NAME,a_strFileName);
	return strSavePath;
}

bool MultiAssignModelSOBADlg::CreateArgument( KIOColumns& modeColumnCollection, TAssignResultInfo& a_AssignResultInfo, KARunningOptionInfo& runningOption )
{
	if (!ClearFiles())
		return false;

	ImChampDir::SetCurrentDirectoryToApp();

	CString strFileName = ImChampDir::GetAppDirectory() + _T("\\assignment.arg");
	UINT    nOpenMode   = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	
	CStdioFileEx of(strFileName, nOpenMode);

	bool bResult = false;

	// Step 0. Project Info
	bResult = WriteInfoArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteInfoArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step 1. Model
	bResult = WriteModelArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteModelArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step 2. Input
	bResult = WriteInputArgument(of, modeColumnCollection);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step3. Run Options
	bResult = WriteRunOptionsArgument(of, runningOption);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteRunOptionsArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	
	// Step4. VDF
	bResult = WriteVDFArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteVDFArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step5. Transit Assign Option
	bResult = WriteTransitAssignOptionArgument(of);
	if (!bResult)
	{

		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step6. Set OD Demand Share
	bResult = WriteODShareArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteODShareArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// Step7. Stopping Criteria
	bResult = WriteStoppingCriteria(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteStoppingCriteria) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = WriteFolderArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteFolder) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// 출력 제목
	bResult = WriteModelTitle(of, a_AssignResultInfo.TstrDescription);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(출력 제목) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	//include 데이터 생성

	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;


	KDBaseAssignment::GetHighwayAssignmentTypeData(m_pRunTarget, setIncludeNodeCode, setIncludeLinkCode);

	KInspectionNetwork::Inspection(m_pRunTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);


	// 노드파일 생성
	//bResult = KBulkDBase2File::ExportDB2NodeFileExcludeNodeOff(m_pRunTarget->GetDBaseConnection());

	CString strNodeLocation = ConvertSavePath(_T("node.dat"));

	//bResult = KBulkDBase2File::ExportDB2NodeFileExcludeType(m_pRunTarget->GetDBaseConnection(), setUsableNodeData, strNodeLocation);

	bResult = KBulkDBase2File::ExportDB2NodeFile(m_pRunTarget->GetDBaseConnection(), strNodeLocation);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	

	// 링크파일 생성
	
	//CString strAddQuery     = m_pParent->oKABPRFnDlg.GetAddQuery();
    CString strAddQuery;
    try
    {
        strAddQuery = m_pParent->oKABPRFnDlg.MultiScenarioLinkQuery(m_pRunTarget);
    }
    catch (int& ex) {
    	
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
        return false;
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...) {
        TxLogDebugException();
        return false;
    }
	
	CString strLinkLocation = ConvertSavePath(_T("link.dat"));
	bResult	= m_pParent->oKABPRFnDlg.ExportDB2LinkExcludeType(m_pRunTarget->GetDBaseConnection(), strAddQuery, setUsableLinkData, strLinkLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 수단파일 생성
	CString strODLocation = ConvertSavePath(_T("mode_od.dat"));
	if (KmzSystem::IsPassengerSystem())
	{
		bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunTarget->GetDBaseConnection(), _T("mode_od"), modeColumnCollection, strODLocation);
	}
	else
	{
		bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunTarget->GetDBaseConnection(), TABLE_VEHICLE_MODE_OD, modeColumnCollection, strODLocation);
	}    

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 회전정보파일생성 등등
	//if ( m_RunningOption.IncludeTurnRestriction() )
	if (runningOption.IncludeTurnRestriction())
	{
		//bResult = KBulkDBase2File::ExportDB2TurnFile(m_pRunTarget->GetDBaseConnection());
		CString strTurnLocation = ConvertSavePath(_T("turn.dat"));
		bResult = KBulkDBase2File::ExportDB2TurnFile(m_pRunTarget->GetDBaseConnection(), strTurnLocation);
	}
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(회전정보) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Transit 파일 생성

	CString strTransitLineLocation = ConvertSavePath(_T("line.dat"));
	CString strTransitInfoLocation = ConvertSavePath(_T("line_info.dat"));
	bResult = KBulkDBase2File::ExportDB2LineFile(m_pRunTarget->GetDBaseConnection(),strTransitInfoLocation,strTransitLineLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(Transit) 중 오류가 발생하였습니다. !!!"));
		return false;
	}       

	//if (KmzSystem::IsPassengerSystem() == true && KEMAssignmentModelUserEquilibriumFW == m_emAModel && true == m_RunningOption.IncludeIntersectionDelay())
	if (KmzSystem::IsPassengerSystem() == true && KEMAssignmentModelUserEquilibriumFW == m_emAModel && true == runningOption.IncludeIntersectionDelay())
	{
		CString strInterType = ConvertSavePath(_T("InterType.dat"));
		KDBaseAssignment::InterSectionTypeDB2File(m_pRunTarget->GetDBaseConnection(), strInterType);

		CString strMovemnet = ConvertSavePath(_T("Movement.dat"));
		KDBaseAssignment::InterSectionMovementDB2File(m_pRunTarget->GetDBaseConnection(), strMovemnet);

		CString strBoundType = ConvertSavePath(_T("BoundTurnType.dat"));
		KDBaseAssignment::InterSectionBoundTurnTypeDB2File(m_pRunTarget->GetDBaseConnection(), strBoundType);

		CString strPhaseOper = ConvertSavePath(_T("PhaseOper.dat"));
		KDBaseAssignment::InterSectionPhaseOperDB2File(m_pRunTarget->GetDBaseConnection(), strPhaseOper);
	}

	return true;


}

bool MultiAssignModelSOBADlg::CreateOBArgument( KIOColumns& modeColumnCollection, TAssignResultInfo& a_AssignResultInfo, KARunningOptionInfo& runningOption )
{
	if (! ClearFiles())
	{
		return false;
	}

	CString strFileName(_T(""));

	if (QBicWindowSystem::ProcessOnWindowsStatus() < 1)
	{
		strFileName = ImChampDir::GetAppDirectory() + _T("\\assignment.arg");
	}
	else
	{
		strFileName = ImChampDir::GetAppDirectory() + _T("\\x64\\assignment.arg");
	}

	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(strFileName, nOpenMode);

	bool bResult = false;
	// Step 0. Project Info
	bResult = WriteInfoArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteInfoArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step 1. Model
	bResult = WriteModelArgument(of);
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(WriteModelArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step 2. Input
	bResult = WriteInputOBArgument(of, modeColumnCollection);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step3. Run Options
	bResult = WriteRunOptionsArgument(of, runningOption);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteRunOptionsArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	//Setp3-1 Bush 경로
	bResult = WriteBushTempLocation(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteBushTemp) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step4. VDF
	bResult = WriteOBVDfArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteVDFArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step5. Transit Assign Option
	bResult = WriteTransitAssignOptionArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step6. Set OD Demand Share
	bResult = WriteODShareArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteODShareArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// Step7. Stopping Criteria
	bResult = WriteStoppingCriteria(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteStoppingCriteria) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = WriteFolderArgument(of);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteFolder) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// Step8 Warm Start
	//if (true == m_RunningOption.IncludeWarmStart())
	if (true == runningOption.IncludeWarmStart())
	{
		bResult = WriteFolderWarmStart(of, runningOption);
		if (!bResult)
		{
			of.Close();
			AfxMessageBox(_T("Argument 생성(WriteFolder) 중 오류가 발생하였습니다. !!!"));
			return false;
		}
	}
	// Step9. Balancing
	bResult = WriteBalancing(of, runningOption);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(Balancing) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	// Step10. 출력제목
	bResult = WriteModelTitle(of, a_AssignResultInfo.TstrDescription);
	if (!bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(출력 제목) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	//include 데이터 생성
	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;

	KDBaseAssignment::GetAssignmentTypeData(m_pRunTarget, setIncludeNodeCode, setIncludeLinkCode);
	KInspectionNetwork::Inspection(m_pRunTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);

	// 노드파일 생성
	//bResult = KBulkDBase2File::ExportDB2NodeFile(m_pRunTarget->GetDBaseConnection());
	CString strNodeLocation = ConvertSavePath(_T("node.dat"));
	bResult	= KBulkDBase2File::ExportDB2NodeFile(m_pRunTarget->GetDBaseConnection(), strNodeLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 링크파일 생성
	CString strLinkLocation = ConvertSavePath(_T("link.dat"));
    try
    {
        bResult = m_pParent->m_OBVDFDlg.MultiScenarioExportDB2LinkFileExcludeType(m_pRunTarget->GetDBaseConnection(), setUsableLinkData, strLinkLocation, m_pRunTarget);
    }
    catch (int& ex) {
    	
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);

        return false;
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);

        return false;
    }
    catch (...) {
        TxLogDebugException();

        return false;
    }

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 수단파일 생성
	CString strODLocation = ConvertSavePath(_T("mode_od.dat"));
	if (KmzSystem::IsPassengerSystem())
	{
		bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunTarget->GetDBaseConnection(), _T("mode_od"), modeColumnCollection, strODLocation);
	}
	else
	{
		bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunTarget->GetDBaseConnection(), TABLE_VEHICLE_MODE_OD, modeColumnCollection, strODLocation);
	}    

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 회전정보파일생성 등등
	//if ( m_RunningOption.IncludeTurnRestriction() )
	if (runningOption.IncludeTurnRestriction())
	{
		//bResult = KBulkDBase2File::ExportDB2TurnFile(m_pRunTarget->GetDBaseConnection());
		CString strTurnLocation = ConvertSavePath(_T("turn.dat"));
		bResult = KBulkDBase2File::ExportDB2TurnFile(m_pRunTarget->GetDBaseConnection(),  strTurnLocation);
	}
	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(회전정보) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// 라인정보 생성
	CString strTransitLineLocation = ConvertSavePath(_T("line.dat"));
	CString strTransitInfoLocation = ConvertSavePath(_T("line_info.dat"));
	bResult = KBulkDBase2File::ExportDB2LineFile(m_pRunTarget->GetDBaseConnection(),strTransitInfoLocation,strTransitLineLocation);

	if (!bResult)
	{
		AfxMessageBox(_T("Argument 생성(Transit) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::ClearFiles()
{
	TxLogDebugStartMsg();
	// 생성 파일 삭제
	int nResult = -1;

	try
	{
		nResult = remove("assignment.arg");
		nResult = remove("x64\\assignment.arg");
		nResult = remove((CStringA)ConvertSavePath(_T("node.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("link.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("para_od.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("turn.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("line_info.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("line.dat")));

		// 결과 파일 삭제
		nResult = remove((CStringA)ConvertSavePath(_T("out_link.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("out_od.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("line_info_out.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("line_out.dat")));
		nResult = remove((CStringA)ConvertSavePath(_T("turnvolume.dat")));
		//nResult = remove((CStringA)ConvertSavePath(_T("Assignment_Log.txt")));
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}

bool MultiAssignModelSOBADlg::WriteInfoArgument( CStdioFileEx& of )
{
	CString strOut;

	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();

	try
	{
		strOut.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), m_pRunTarget->CaptionScenario(), m_pRunTarget->CaptionTargetYear());
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool MultiAssignModelSOBADlg::WriteModelArgument( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		strOut.Format(_T("model\t1\r\n"));
		of.WriteString(strOut);
		strOut.Format(_T("%d\r\n"), m_emAModel);
		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteInputArgument( CStdioFileEx& of, KIOColumns& selectedModes )
{
	CStringArray strOutArray;
	CString      strOut;

	selectedModes.Clear();

	try
	{
		// Step 2.1 Select OD Type
		strOut.Format(_T("%d\t%f\r\n"), m_emODType, m_dAnalysisTime);
		strOutArray.Add(strOut);
		// Step 2.2 Select Demand
		std::list<KAInputInfo*>::iterator iter = m_InputList.begin();
		std::list<KAInputInfo*>::iterator end  = m_InputList.end();

		int nSelectedCount = 0;
		while ( iter != end )
		{
			KAInputInfo* pInputInfo = *iter;
			if ( pInputInfo->Selected() )
			{
				KIOColumn* pColumn = pInputInfo->InputColumn();
				if (pColumn)
				{
					int nColumnIndex = selectedModes.AddNewColumn(pColumn);
					strOut = pInputInfo->GetArgumentInfo(nColumnIndex + 1); //OZone(0) / DZone(0) 1를 시작...
					strOutArray.Add(strOut);  
					nSelectedCount++;
				}                
			}
			++iter;
		}

		if (nSelectedCount == 0)
		{
			strOut.Format(_T("input\t%d\r\n"), 0);
			of.WriteString(strOut);
			return false;
		}

		int nCount = strOutArray.GetCount();
		strOut.Format(_T("input\t%d\r\n"), nCount);
		of.WriteString(strOut);

		for (int i=0; i<nCount; i++)
		{
            if (i > 0) { // 수단 OD 데이터 컬럼명
                KIOColumn* pColumn = selectedModes.GetColumn(i - 1);
                strOut.Format(_T("%s\r\n"), pColumn->Caption());
                of.WriteString(strOut);
            }
            CString strTemp = strOutArray[i];

			of.WriteString(strOutArray[i]);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteRunOptionsArgument( CStdioFileEx& of, KARunningOptionInfo& runningOption )
{
	CString strOut;
	try
	{
		strOut.Format(_T("run_options\t%d\r\n"), 1);
		of.WriteString(strOut);

		//of.WriteString(m_RunningOption.GetArgumentInfo());
		of.WriteString(runningOption.GetArgumentInfo());
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteVDFArgument( CStdioFileEx& of )
{
	bool bResult = false;

	try
	{
		bResult = m_pParent->oKABPRFnDlg.WriteInputArgument(of);
	}
	catch(...)
	{
		return false;
	}

	return bResult;
}

bool MultiAssignModelSOBADlg::WriteTransitAssignOptionArgument( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		if (KEMAssignmentModelMultiModal == m_emAModel || KEMAssignmentModelUserEquilibriumOBMulti == m_emAModel)
		{
			m_pParent->oKASetTransitAssignOptionsDlg.WriteInputArgument(of);
		}
		else
		{
			strOut.Format(_T("transit_assign_options\t%d\r\n"), 0);
			of.WriteString(strOut);
		}        
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	return true;
}

bool MultiAssignModelSOBADlg::WriteODShareArgument( CStdioFileEx& of )
{
	CString strOut;

	try
	{
		int  nCount = m_TripRateList.size();

		if (nCount <= 0)
		{
			strOut.Format(_T("od_share\t%d\r\n"), 0);
			of.WriteString(strOut);
			return true;
		}

		strOut.Format(_T("od_share\t%d\r\n"), nCount + 1);
		of.WriteString(strOut);

		strOut.Format(_T("%d\r\n"), nCount);
		of.WriteString(strOut);

		std::list<double>::iterator iter = m_TripRateList.begin();
		std::list<double>::iterator end  = m_TripRateList.end();

		int nSeq = 0;
		while (iter != end)
		{
			strOut.Format(_T("%d\t%f\r\n"), ++nSeq, *iter);
			of.WriteString(strOut);
			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	return true;
}

bool MultiAssignModelSOBADlg::WriteStoppingCriteria( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		strOut.Format(_T("stopping_criteria\t%d\r\n"), 1);
		of.WriteString(strOut);
		if ( KEMAssignmentModelUserEquilibriumOB == m_emAModel 
			|| KEMAssignmentModelUserEquilibriumOBMulti == m_emAModel)
		{
			strOut.Format(_T("%d\t%g\t%d\t%f\r\n"), 
				m_nHighwayIterationNo,  m_dHighwayRelativeGap, 
				m_nUEOBaseZoneCentroidThrought, m_dUEOBaseScaleFactor);
		}
		else
		{
			strOut.Format(_T("%d\t%g\t%d\t%f\r\n"), 
				m_nHighwayIterationNo,  m_dHighwayRelativeGap, 
				m_nTransitIterationNo,  m_dTransitNormalizeGap);
		}

		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}  
	return true;
}

bool MultiAssignModelSOBADlg::WriteFolderArgument( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		strFolder.Format(_T("%s"), m_pRunTarget->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + DLL_HIGHWAYASSIGN_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteModelTitle(CStdioFileEx& of, CString a_strDescription)
{
	CString strOut(_T(""));
	try
	{
		strOut.Format(_T("%s\r\n"), a_strDescription);
		of.WriteString(strOut);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void MultiAssignModelSOBADlg::InitTAssingReulstColumn( TAssignResultColumnName& a_oRuslutColumn )
{
	a_oRuslutColumn.AverageTime			= _T("");
	a_oRuslutColumn.VC					= _T("");
	a_oRuslutColumn.SumVolume			= _T("");
	a_oRuslutColumn.Trip				= _T("");
	a_oRuslutColumn.AvgTimeToll			= _T("");
	a_oRuslutColumn.BusInitialVolumn	= _T("");
	a_oRuslutColumn.ModeVolumn.clear();
	a_oRuslutColumn.VDFTime.clear();
	a_oRuslutColumn.VDFTollTime.clear();

	a_oRuslutColumn.ODAvgTime			= _T("");
	a_oRuslutColumn.ODAvgBusTime		= _T("");
	a_oRuslutColumn.ODAvgSubwayTime		= _T("");
	a_oRuslutColumn.ODBusDirect			= _T("");
	a_oRuslutColumn.ODBusOneTransfer	= _T("");
	a_oRuslutColumn.ODBustwoTransfer    = _T("");
	a_oRuslutColumn.ODAVgBusSubwayTime	= _T("");
	a_oRuslutColumn.ODVDFTime.clear();

	a_oRuslutColumn.ODMBus				= _T("");
	a_oRuslutColumn.ODMRail				= _T("");
	a_oRuslutColumn.ODMBusRail			= _T("");

	a_oRuslutColumn.LinfoPassenger		= _T("");

	a_oRuslutColumn.LineBoardNet		= _T("");
	a_oRuslutColumn.LineBoardTransfer	= _T("");
	a_oRuslutColumn.LineBoardTotal		= _T("");
	a_oRuslutColumn.LineAlightNet		= _T("");
	a_oRuslutColumn.LineAlightTransfer	= _T("");
	a_oRuslutColumn.LineAlightTotal		= _T("");
	a_oRuslutColumn.LinePassenger		= _T("");

	a_oRuslutColumn.NodeBusNetBorad			= _T("");
	a_oRuslutColumn.NodeBusTransferBoard	= _T("");
	a_oRuslutColumn.NodeBusSumBoard			= _T("");
	a_oRuslutColumn.NodeBusNetAlight		= _T("");
	a_oRuslutColumn.NodeBusTransferAlight	= _T("");
	a_oRuslutColumn.NodeBusSumAlight		= _T("");

	a_oRuslutColumn.TerminalTrip			= _T("");
}

void MultiAssignModelSOBADlg::AddAssignOBMultiResultColumn( int runSeq, KIOColumns& rLinkColumnCollection, 
	KIOColumns& rParaODColumnCollection, TAssignResultColumnName& resultColumnName , CString a_strColumnInfo, 
	KARunningOptionInfo& runningOption)
{
	TxLogDebugStartMsg();
	CString strErrMsg = _T("컬럼 추가에 실패하였습니다.");

	rLinkColumnCollection.Clear();
	rParaODColumnCollection.Clear();

	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);
	KIOTable*        pParaODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);

	CString strColumnName, strRunSeq, strDescription;
	strRunSeq.Format(_T("(%d)"), runSeq);

    KIOColumn* pColumn = nullptr;

	//★★ Link Table ★★    
    //★ Average Time
	strColumnName.Format(_T("AvgTime%s"), strRunSeq);
	strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
	if (pColumn == nullptr)	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.AverageTime = pColumn->Name();

    //★ Average Time (w Toll)
    strColumnName.Format(_T("AvgTime+%s"), strRunSeq);
    strDescription.Format(_T("Average_Time_%s"),a_strColumnInfo);
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.AvgTimeToll = pColumn->Name();

    //★ VDF Time
    std::vector<CString> vecVDFTime;
    for (int i = 0; i < m_nModeInVDF; i ++)
    {
        CString strColumnCaption = _T("");
        int nIndex = i + 1; 
        strColumnCaption.Format(_T("%s_%d%s%s"), _T("VDF"), nIndex,_T("Time"), strRunSeq);
        strDescription.Format(_T("%s_%d%s_%s"),_T("VDF"), nIndex,_T("Time") ,a_strColumnInfo);
        pColumn = CreateResultColumn(strColumnCaption, pLinkTable, strDescription);
        if (pColumn == nullptr)
        {
            ThrowException(strErrMsg);
        }
        rLinkColumnCollection.AddNewColumn(pColumn);
        vecVDFTime.push_back(pColumn->Name());
    }
    resultColumnName.VDFTime = vecVDFTime;    

    //★ VDF Toll Time
    std::vector<CString> vecVDFTollTime;
    for (int i = 0; i < m_nModeInVDF; i ++)
    {
        CString strColumnCaption = _T("");
        int nIndex = i + 1; 
        strColumnCaption.Format(_T("VDF%dTime+%s"), nIndex, strRunSeq);
        strDescription.Format(_T("VDF%d_%s"),nIndex,a_strColumnInfo);
        pColumn = CreateResultColumn(strColumnCaption, pLinkTable, strDescription);
        if (pColumn == nullptr)
        {
            ThrowException(strErrMsg);
        }
        rLinkColumnCollection.AddNewColumn(pColumn);
        vecVDFTollTime.push_back(pColumn->Name());
    }
    resultColumnName.VDFTollTime = vecVDFTollTime;

    //★ V/C
	strColumnName.Format(_T("V/C%s"), strRunSeq);
	strDescription.Format(_T("V/C_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
	if (pColumn == nullptr) {
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.VC = pColumn->Name();

    //★ Total Volume
    strColumnName.Format(_T("TotVol%s"), strRunSeq);
    strDescription.Format(_T("Total_Volume_%s"), a_strColumnInfo);
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.SumVolume = pColumn->Name();

    //★ 수단별 Volume
	std::vector<CString> vecModeName;
	GetUseModeName(vecModeName);
	std::vector<CString> vecModeVolumne;
	for (size_t i=0; i<vecModeName.size(); i++)
	{
		CString strColumnCaption =  vecModeName[i];
		strColumnName.Format(_T("%s%s"),strColumnCaption, strRunSeq );
		strDescription.Format(_T("%s_%s"),strColumnCaption ,a_strColumnInfo);

		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription); 
		if (pColumn == nullptr) {
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		vecModeVolumne.push_back(pColumn->Name());
	}
	resultColumnName.ModeVolumn = vecModeVolumne;
    
    //★ Bus Initial Volume
    //if (true == m_RunningOption.IncludeBusInitialVolume())
	if (true == runningOption.IncludeBusInitialVolume())
    {
        strColumnName.Format( _T("BusInitVol%s"),  strRunSeq);
        strDescription.Format(_T("Bus_InitVolumn_%s"), a_strColumnInfo);
        pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
        if (pColumn == nullptr)
        {
            ThrowException(strErrMsg);
        }
        rLinkColumnCollection.AddNewColumn(pColumn);
        resultColumnName.BusInitialVolumn = pColumn->Name();
    }

	//OD Table
	strColumnName.Format (_T("AvgTime%s"),strRunSeq);
	strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pParaODTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rParaODColumnCollection.AddNewColumn(pColumn);
	resultColumnName.ODAvgTime = pColumn->Name();


	std::vector<CString> vecODVDFTime;
	for (int i = 0; i < m_nModeInVDF; i ++)
	{
		CString strColumnCaption = _T("");
		int nIndex = i + 1; 
		strColumnCaption.Format(_T("VDF%dTime%s"),  nIndex, strRunSeq);
		strDescription.Format(  _T("VDF%dTime_%s"), nIndex, a_strColumnInfo);
		pColumn =  CreateResultColumn(strColumnCaption, pParaODTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rParaODColumnCollection.AddNewColumn(pColumn);
		vecODVDFTime.push_back(pColumn->Name());
	}

	resultColumnName.ODVDFTime = vecODVDFTime;
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::AddAssignOBSingeResultColumn( int runSeq, KIOColumns& rLinkColumnCollection, 
	KIOColumns& rParaODColumnCollection, TAssignResultColumnName& resultColumnName , CString a_strColumnInfo,
	KARunningOptionInfo& runningOption)
{
	TxLogDebugStartMsg();
	CString strErrMsg = _T("> Column Add Failed ...");

	rLinkColumnCollection.Clear();
	rParaODColumnCollection.Clear();

	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);
	KIOTable*        pParaODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);

	CString strColumnName, strRunSeq, strDescription;
	strRunSeq.Format(_T("(%d)"), runSeq);

    KIOColumn* pColumn = nullptr;

	//★★ Link Table ★★
    //★ Average Time
    strColumnName.Format(_T("AvgTime%s"), strRunSeq);
    strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.AverageTime = pColumn->Name();

    //★ Average Time (w Toll)
    strColumnName.Format (_T("AvgTime+%s"),  strRunSeq);
    strDescription.Format(_T("Average_Time+_%s"), a_strColumnInfo);
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr) {
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.AvgTimeToll = pColumn->Name();

    //★ V/C
	strColumnName.Format( _T("V/C%s"),  strRunSeq);
	strDescription.Format(_T("V/C_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
	if (pColumn == nullptr) {
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.VC = pColumn->Name();

    //★ Total Volume
    strColumnName.Format( _T("TotVol%s"), strRunSeq);
    strDescription.Format(_T("Total_Volume_%s"), a_strColumnInfo);
    pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
    if (pColumn == nullptr)	{
        ThrowException(strErrMsg);
    }
    rLinkColumnCollection.AddNewColumn(pColumn);
    resultColumnName.SumVolume = pColumn->Name();

    //★ 수단별 Volume
	std::vector<CString> vecModeName;
	GetUseModeName(vecModeName);
	std::vector<CString> vecModeVolumne;
	for (size_t i=0; i<vecModeName.size(); i++)
	{
		CString strColumnCaption = vecModeName[i];
		strColumnName.Format( _T("%s%s"),strColumnCaption,  strRunSeq );
		strDescription.Format(_T("%s_%s"),strColumnCaption, a_strColumnInfo);
		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
		if (pColumn == nullptr) {
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		vecModeVolumne.push_back(pColumn->Name());
	}
	resultColumnName.ModeVolumn = vecModeVolumne;
    
    //★ Bus Initial Volume
	//if (true == m_RunningOption.IncludeBusInitialVolume())
	if (true == runningOption.IncludeBusInitialVolume())
	{
		strColumnName.Format( _T("BusInitVol%s"),  strRunSeq);
		strDescription.Format(_T("Bus_InitVolumn_%s"), a_strColumnInfo);
		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		resultColumnName.BusInitialVolumn = pColumn->Name();
	}

	//OD Table
	strColumnName.Format (_T("AvgTime%s"),  strRunSeq);
	strDescription.Format(_T("Average_Time_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pParaODTable, strDescription);

	if (pColumn == nullptr) {
		ThrowException(strErrMsg);
	}
	rParaODColumnCollection.AddNewColumn(pColumn);
	resultColumnName.ODAvgTime = pColumn->Name();
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::AddAssignResultColumn(int runSeq, KIOColumns& rLinkColumnCollection, KIOColumns& rParaODColumnCollection
	, TAssignResultColumnName& resultColumnName, CString a_strColumnInfo)
{
	TxLogDebugStartMsg();
	CString strErrMsg = _T("> Column Add Failed ...");

	rLinkColumnCollection.Clear();
	rParaODColumnCollection.Clear();

	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);
	KIOTable*        pParaODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);

	CString strColumnName, strRunSeq, strDescription;
	strRunSeq.Format(_T("(%d)"), runSeq);

	//link Talbe 
	//Average_time
	strColumnName.Format (_T("AvgTime%s"),  strRunSeq);
	strDescription.Format(_T("AvgTime_%s"), a_strColumnInfo);
	KIOColumn* pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.AverageTime = pColumn->Name();

	//VC
	strColumnName.Format (_T("V/C%s"),strRunSeq);
	strDescription.Format(_T("V/C_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.VC = pColumn->Name();

	//SumVolume
	strColumnName.Format (_T("TotVol%s"),  strRunSeq);
	strDescription.Format(_T("TotVol_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.SumVolume = pColumn->Name();

	//Trip
	strColumnName.Format (_T("Trip%s"),  strRunSeq);
	strDescription.Format(_T("Trip_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rLinkColumnCollection.AddNewColumn(pColumn);
	resultColumnName.Trip = pColumn->Name();

	std::vector<CString> vecModeName;
	GetUseModeName(vecModeName);

	//Modevolume
	std::vector<CString> vecModeVolumne;
	for (size_t i=0; i<vecModeName.size(); i++)
	{
		CString strColumnCaption = vecModeName[i];
		strColumnName.Format (_T("%s%s"),strColumnCaption, strRunSeq );
		strDescription.Format(_T("%s_%s"),strColumnCaption, a_strColumnInfo);

		pColumn = CreateResultColumn(strColumnName, pLinkTable, strDescription);
		if (pColumn == nullptr)
		{
			ThrowException(strErrMsg);
		}
		rLinkColumnCollection.AddNewColumn(pColumn);
		vecModeVolumne.push_back(pColumn->Name());
	}
	resultColumnName.ModeVolumn = vecModeVolumne;

	//OD Table
	//ODAverage
	strColumnName.Format (_T("AvgTime%s"),strRunSeq);
	strDescription.Format(_T("AvgTime_%s"), a_strColumnInfo);
	pColumn = CreateResultColumn(strColumnName, pParaODTable, strDescription);

	if (pColumn == nullptr)
	{
		ThrowException(strErrMsg);
	}
	rParaODColumnCollection.AddNewColumn(pColumn);
	resultColumnName.ODAvgTime = pColumn->Name();

	TxLogDebugEndMsg();
}

KIOColumn* MultiAssignModelSOBADlg::CreateResultColumn( CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription )
{
	const KIOColumns* pColumCollection = a_pTable->Columns();

	int nMaxDisplayCount(0);
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
	oColumn.Name(a_pTable->GetUsableColumnName());
	oColumn.Caption(a_strColumnName);
	oColumn.CodeType(KEMIOCodeTypeIsNothing);
	oColumn.ColumnType(KEMIOColumnTypeUserDefine);
	oColumn.DataType(KEMIODataTypeDouble);
	oColumn.DisplayOrder(nMaxDisplayCount+1);
	oColumn.ColumnInfo(a_strDescription);
	oColumn.PrimaryKey(false);
	oColumn.Visible(true);

	KIOColumn* pColumn = a_pTable->AddColumn(oColumn);
	return pColumn;
}

bool MultiAssignModelSOBADlg::DeleteCreateColumn( KIOColumns& rLinkColumnCollecion, KIOColumns& rParaODColumnCollecion )
{
	int nLinkAddColumnCollections	= rLinkColumnCollecion.ColumnCount();
	int nModeODAddColumnCollections = rParaODColumnCollecion.ColumnCount();

	if( 0 != nLinkAddColumnCollections ||  0 != nModeODAddColumnCollections )
	{
		TxLogDebug(_T("Delete Added Column"));

		KIOTable* pLinkTable = m_pRunTarget->Tables()->FindTable(TABLE_LINK);
		for (int i = 0; i < nLinkAddColumnCollections; ++i)
		{
			pLinkTable->RemoveColumn(rLinkColumnCollecion.GetColumn(i)->Name());
		}

		KIOTable* pModeODTable = m_pRunTarget->Tables()->FindTable(TABLE_MODE_OD);
		for (int i = 0; i < nModeODAddColumnCollections; ++i)
		{
			pModeODTable->RemoveColumn(rParaODColumnCollecion.GetColumn(i)->Name());
		}

		return false;
	}

	return true;
}

void MultiAssignModelSOBADlg::UpdateLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection )
{
	TxLogDebugStartMsg();

	KIOTables* pIOTables  = m_pRunTarget->Tables();
	KIOTable*  pLinkTable = pIOTables->FindTable(TABLE_LINK);

	int nColumnCount = rLinkColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("None ColumnCount"));

	CString strLocation = ConvertSavePath(_T("out_link.dat"));
	std::ifstream ifs(strLocation, std::ios::binary );    
	try
	{
		if (!ifs)
		{
			ThrowException(_T("File Not Found"));
		}

		KIDKeyDoubleRecords records(nColumnCount);

		if (!ifs.is_open())
		{
			throw -1;
		}

		if ( ifs )
		{
			records.ReadBytes(ifs);
		}
		else
		{
			ThrowException(_T("error file read"));
		}
		ifs.close();

		KBulkDBase::BulkLinkUpdate(spDBaseConnection, pLinkTable, rLinkColumnCollection, records);  

	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::UpdateOBSingleLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection, KARunningOptionInfo& runningOption )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);

	int nColumnCount = rLinkColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("None ColumnCount"));

	CString strLocation = ConvertSavePath(_T("out_link.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );   
	try
	{
		KIDKeyDoubleRecords records(nColumnCount);

		if (!ifs) {
			ThrowException(_T("File Not Found"));
		}

		__int64 nxLinkID;
		double  dValue;
		__int64	nxFTNodeID;

		while (!ifs.eof())
		{
			int nIndex = 0;
			ifs.read( reinterpret_cast<char*>(&nxLinkID), sizeof(Integer) );
			if(ifs.eof()) 
				break;

			KSDoubleRecord* pRecord = records.AddRecord(nxLinkID);

			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));
			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // AVerageT
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // avg Toll
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //vdf Time
            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //vdf Toll Time

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VC
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //SumVoulm
			pRecord->SetAt(nIndex,dValue);
			nIndex++;
            
			std::vector<CString> vecModeName;
			GetUseModeName(vecModeName);
			for (size_t i = 0; i < vecModeName.size(); i++)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // 수단별 volume
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
            
			//if (true == m_RunningOption.IncludeBusInitialVolume())
			if (true == runningOption.IncludeBusInitialVolume())
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // BusInitVolume
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double));
			}
		}
		ifs.close();

		KBulkDBase::BulkLinkUpdate(spDBaseConnection, pLinkTable, rLinkColumnCollection, records);  
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::UpdateOBMultiLinkFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rLinkColumnCollection, KARunningOptionInfo& runningOption)
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pLinkTable    = pIOTables->FindTable(TABLE_LINK);

	int nColumnCount = rLinkColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("None ColumnCount"));

	CString strLocation = ConvertSavePath(_T("out_link.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );    

	try
	{
		KIDKeyDoubleRecords records(nColumnCount);

		if (!ifs) {
			ThrowException(_T("File Not Found"));
		}

		__int64 nxLinkID;
		double  dValue;
		__int64 nxFTNodeID;

		while (!ifs.eof())
		{
			int nIndex = 0;

			ifs.read( reinterpret_cast<char*>(&nxLinkID), sizeof(Integer) );
			if(ifs.eof())
				break;

			KSDoubleRecord* pRecord = records.AddRecord(nxLinkID);

			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));
			ifs.read( reinterpret_cast<char*>(&nxFTNodeID), sizeof(Integer));

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // AVerageT
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // avg Toll
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            for (int i = 0; i <m_nModeInVDF; i++)
            {
                ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VDF 별 Time
                pRecord->SetAt(nIndex,dValue);
                nIndex++;
            }

            for (int i = 0; i <m_nModeInVDF; i++)
            {
                ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VDF별 Toll Time
                pRecord->SetAt(nIndex,dValue);
                nIndex++;
            }

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // VC
            pRecord->SetAt(nIndex,dValue);
            nIndex++;

            ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //SumVoulm
			pRecord->SetAt(nIndex,dValue);
			nIndex++;

			std::vector<CString> vecModeName;
			GetUseModeName(vecModeName);
			for (size_t i = 0; i < vecModeName.size(); i++)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // 수단별 volume
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}

			//if (true == m_RunningOption.IncludeBusInitialVolume())
			if (true == runningOption.IncludeBusInitialVolume())
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); // avg Toll
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double));
			}
		}
		ifs.close();

		KBulkDBase::BulkLinkUpdate(spDBaseConnection, pLinkTable, rLinkColumnCollection, records);  
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::UpdateModeODFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pParaODTable  = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));

	CString strLocation = ConvertSavePath(_T("out_od.dat"));
	std::ifstream ifs(strLocation, std::ios::binary );

	try
	{
		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;

		KxBulkDbase oBulkDBase(spDBaseConnection, pParaODTable, &rParaODColumnCollection);

		//TxLogDebug(_T("import loop start!"));
		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);

			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		//TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();
		ifs.close();

		//delete pRecord;
		QBicDelete(pRecord);
	}
	catch(KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}    
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::UpdateOBSingleModeODFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));

	CString strLocation = ConvertSavePath(_T("out_od.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );
	try
	{
		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;
		double dValue;

		KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rParaODColumnCollection);
		TxLogDebug(_T("import loop start!"));
		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);
			ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double));
			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();
		ifs.close();
		QBicDelete(pRecord);
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebugException();
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}    
	TxLogDebugEndMsg();
}

void MultiAssignModelSOBADlg::UpdateOBMultiModeODFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection )
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pRunTarget->Tables();
	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();

	if (nColumnCount == 0 )
		ThrowException(_T("적용 컬럼 정보가 없습니다."));

	CString strLocation = ConvertSavePath(_T("out_od.dat"));
	std::ifstream ifs( strLocation, std::ios::binary );
	try
	{


		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;
		//double dValue;

		KxBulkDbase oBulkDBase(spDBaseConnection, pODTable, &rParaODColumnCollection);
		TxLogDebug(_T("import loop start!"));
		while (!ifs.eof())
		{
			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );
			pRecord->ReadBytes(ifs);
			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();
		ifs.close();
		QBicDelete(pRecord);
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebugException();
		ifs.close();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		throw 1;
	}    
	TxLogDebugEndMsg();
}

bool MultiAssignModelSOBADlg::DeleteOverWriteAssignModel( KDBaseConPtr spDBaseConnection, KMultiAssignInfoSOBA* a_pInfo )
{
	int nRunSeq = a_pInfo->m_TOverWirteAResultInfo.TnRunSeq;

	CString strQuery = _T("");
	strQuery.Format(_T("Delete from assign_model where run_seq = %d "), nRunSeq);

	spDBaseConnection->ExecuteUpdate(strQuery);

	return true;
}

void MultiAssignModelSOBADlg::GetUseModeName( std::vector<CString>& a_vecModeName )
{
	try
	{
		AutoType iter = m_InputList.begin();
		AutoType end  = m_InputList.end();
		while(iter != end)
		{
			KAInputInfo* oInfo = *iter;
			if (oInfo->Selected() == true)
			{
				a_vecModeName.push_back(oInfo->Mode()->ModeName());
			}
			++iter;
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

bool MultiAssignModelSOBADlg::WriteInputOBArgument( CStdioFileEx& of, KIOColumns& selectedModes )
{
	//KIOColumn* pColumn = NULL;    
	CStringArray strOutArray;
	CString      strOut;

	selectedModes.Clear();

	try
	{
		// Step 2.1 Select OD Type
		strOut.Format(_T("%d\t%f\r\n"), m_emODType, m_dAnalysisTime);
		strOutArray.Add(strOut);
		// Step 2.2 Select Demand
		std::list<KAInputInfo*>::iterator iter = m_InputList.begin();
		std::list<KAInputInfo*>::iterator end  = m_InputList.end();

		int nSelectedCount = 0;
		while ( iter != end )
		{
			KAInputInfo* pInputInfo = *iter;
			if ( pInputInfo->Selected() )
			{
				KIOColumn* pColumn = pInputInfo->InputColumn();
				if (pColumn)
				{
					int nColumnIndex = selectedModes.AddNewColumn(pColumn);
					int nModeID = pInputInfo->Mode()->ModeID();
					strOut = pInputInfo->GetOBArgumentInfo(nColumnIndex + 1, m_pParent->m_oTOBMultiVDF.TmapVDFModeMating, m_nModeInVDF, nModeID); //OZone(0) / DZone(0) 1를 시작...
					strOutArray.Add(strOut);  
					nSelectedCount++;
				}                
			}
			++iter;
		}

		if (nSelectedCount == 0)
		{
			strOut.Format(_T("input\t%d\r\n"), 0);
			of.WriteString(strOut);
			return false;
		}

		int nCount = strOutArray.GetCount();
		strOut.Format(_T("input\t%d\r\n"), nCount);
		of.WriteString(strOut);

		for (int i=0; i<nCount; i++)
		{
            if (i > 0) { // 수단 OD 데이터 컬럼명
                KIOColumn* pColumn = selectedModes.GetColumn(i - 1);
                strOut.Format(_T("%s\r\n"), pColumn->Caption());
                of.WriteString(strOut);
            }
            CString strTemp = strOutArray[i];
			of.WriteString(strOutArray[i]);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteBushTempLocation( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		CString strTargetLocation = m_pRunTarget->GetLocation();
		CString strTempLocation = strTargetLocation + _T("\\") + BUSH_PATHVOLUMN_FOLDER_NAME;

		//DeletePath(strTempLocation);

		CreateDirectory(strTempLocation, NULL);

		strOut.Format(_T("%s\r\n"), strTempLocation);

		of.WriteString(strOut);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteOBVDfArgument( CStdioFileEx& of )
{
	bool bResult = false;

	try
	{
		bResult = m_pParent->m_OBVDFDlg.WriteInputArgument(of);
	}
	catch(...)
	{
		return false;
	}

	return bResult;
}

bool MultiAssignModelSOBADlg::WriteFolderWarmStart( CStdioFileEx& of, KARunningOptionInfo& runningOption )
{
	CString strFolder;
	try
	{
		// 2019.04.29 추가 : Base 네트워크 명칭 라인 추가, [프로젝트명]_[시나리오명]_[분석년도명] 형식
		CString strBaseNetwork;	// Base 네트워크 명칭

		KTarget* pTarget = runningOption.Target();
		KScenario* pScenario = (KScenario*)pTarget->GetParentObject();
		KProject* pProject = (KProject*)pScenario->GetParentObject();		

		strBaseNetwork.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), pScenario->GetName(), pTarget->GetName());
		of.WriteString(strBaseNetwork);


		//strFolder.Format(_T("%s"),m_RunningOption.Target()->GetLocation());
		strFolder.Format(_T("%s"), runningOption.Target()->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + DLL_HIGHWAYASSIGN_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);

		//strFolder.Format(_T("%s"),m_RunningOption.Target()->GetLocation());
		strFolder.Format(_T("%s"), runningOption.Target()->GetLocation());
		strFolder = strFolder + _T("\\") +BUSH_PATHVOLUMN_FOLDER_NAME;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool MultiAssignModelSOBADlg::WriteBalancing( CStdioFileEx& of, KARunningOptionInfo& runningOption )
{
	CString strOut(_T(""));
	try
	{
		//strOut.Format(_T("%d\t%E\r\n"), m_RunningOption.m_nIteration, m_RunningOption.m_dMaximumPathRate);
		strOut.Format(_T("%d\t%E\r\n"), runningOption.m_nIteration, runningOption.m_dMaximumPathRate);
		of.WriteString(strOut);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void MultiAssignModelSOBADlg::SaveMultiScenarioInfo( void )
{
	try
	{

		CString strName, strModes;

		//Get Name
		GetDlgItemText(IDC_EDIT_NAME, strName);

		strName.Trim();

		if (strName.IsEmpty())
		{
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			ThrowException(_T("Multi Scenario 명칭을 입력하세요."));
		}

		if (KDBaseMultiAssignSOBA::IsExistName(strName))
		{
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			ThrowException(_T("중복된 명칭입니다."));
		}

		//Get Modes
		strModes = ProjectModes();		

		int nID = KDBaseMultiAssignSOBA::InsertMutliInfo(strName, strModes);

		//Get Data;
		// 2019.04.22 수정 : 분석할 시나리오를 트리에 추가하는 방식으로 변경
		//CXTPReportRecord*     pScenarioRecord = nullptr;
		//CXTPReportRecords*    pTargetRecords  = nullptr;
		//CXTPReportRecord*     pTargetRecord   = nullptr;
		//CXTPReportRecordItem* pItem           = nullptr;

		//int nRecord = m_wndTree.GetRecords()->GetCount();

		//for (int i = 0; i < nRecord; ++i)
		//{
		//	pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
		//	pTargetRecords  = pScenarioRecord->GetChilds();

		//	int nTargetRecord = pTargetRecords->GetCount();

		//	for (int j = 0; j < nTargetRecord; ++j)
		//	{
		//		pTargetRecord = pTargetRecords->GetAt(j);
		//		pItem         = pTargetRecord->GetItem(0);

		//		if (!pItem->IsChecked())
		//			continue;

		//		KMultiAssignInfo*       pDlg     = (KMultiAssignInfo*)pItem->GetItemData();
		//		std::list<KAInputInfo*> lstInput = pDlg->GetInputInfo();
		//		KTarget*                pTarget  = pDlg->Target();
		//		CString                 strData(_T(""));

		//		std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

		//		while (bIter != lstInput.end())
		//		{
		//			KAInputInfo* pInfo = *bIter;

		//			if (pInfo->Selected())
		//			{
		//				KIOColumn* pColumn = pInfo->InputColumn();
		//				KMode*     pMode   = pInfo->Mode();

		//				if (!strData.IsEmpty())
		//					strData.Append(_T(";"));

		//				strData.AppendFormat(_T("%d:%s"), pMode->ModeID(), pColumn->Caption());
		//			}

		//			++bIter;
		//		}

		//		KDBaseMultiAssign::InsertMultiData(nID, pTarget->CaptionScenario(), pTarget->CaptionTargetYear(), strData);
		//	}
		//}
		int nRecord = m_wndTree.GetRecords()->GetCount();
		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord* pRecord = m_wndTree.GetRecords()->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);

			KMultiAssignInfoSOBA*       pInfo     = (KMultiAssignInfoSOBA*)pItem->GetItemData();
			KARunningOptionInfo& ro = pInfo->GetRunningOption();
			std::list<KAInputInfo*> lstInput = pInfo->GetInputInfo();
			KTarget*                pTarget  = pInfo->Target();
			CString                 strData(_T(""));

			std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

			while (bIter != lstInput.end())
			{
				KAInputInfo* pInputInfo = *bIter;

				if (pInputInfo->Selected())
				{
					KIOColumn* pColumn = pInputInfo->InputColumn();
					KMode*     pMode   = pInputInfo->Mode();

					if (!strData.IsEmpty())
						strData.Append(_T(";"));

					strData.AppendFormat(_T("%d:%s"), pMode->ModeID(), pColumn->Caption());
				}

				++bIter;
			}

			CString strWSScenario;
			CString strWSTarget;
			ASSERT(NULL != ro.Scnario() && NULL != ro.Target());

			if (NULL != ro.Scnario())
			{
				strWSScenario = ro.Scnario()->GetName();
			}
			if (NULL != ro.Target())
			{
				strWSTarget = ro.Target()->GetName();
			}

			KDBaseMultiAssignSOBA::InsertMultiData(nID, i+1, pTarget->CaptionScenario(), pTarget->CaptionTargetYear(), strData, 
				strWSScenario, strWSTarget);

		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		throw 1;
	}
	catch (...)
	{
		AfxMessageBox(_T("여러 시나리오 반복 실행 설정 정보 저장에 실패하였습니다."));
		throw 1;
	}
}

void MultiAssignModelSOBADlg::EditMultiScenarioInfo( void )
{
	try
	{
		CString strName, strModes;

		//Get Name
		GetDlgItemText(IDC_EDIT_NAME, strName);

		strName.Trim();

		if (strName.IsEmpty())
		{
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			ThrowException(_T("Multi Scenario 명칭을 입력하세요."));
		}

		int nID = m_cboName.GetItemData(m_cboName.GetCurSel());

		if (KDBaseMultiAssignSOBA::IsExistNameExcludeCurrent(nID, strName))
		{
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			ThrowException(_T("중복된 명칭입니다."));
		}

		//Get Modes
		strModes = ProjectModes();		

		KDBaseMultiAssignSOBA::UpdateMultiInfo(nID, strName, strModes);

		//Get Data;
		// 2019.04.22 수정 : 분석할 시나리오를 트리에 추가하는 방식으로 변경
		//CXTPReportRecord*     pScenarioRecord = nullptr;
		//CXTPReportRecords*    pTargetRecords  = nullptr;
		//CXTPReportRecord*     pTargetRecord   = nullptr;
		//CXTPReportRecordItem* pItem           = nullptr;

		//int nRecord = m_wndTree.GetRecords()->GetCount();

		//for (int i = 0; i < nRecord; ++i)
		//{
		//	pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
		//	pTargetRecords  = pScenarioRecord->GetChilds();

		//	int nTargetRecord = pTargetRecords->GetCount();

		//	for (int j = 0; j < nTargetRecord; ++j)
		//	{
		//		pTargetRecord = pTargetRecords->GetAt(j);
		//		pItem         = pTargetRecord->GetItem(0);

		//		if (!pItem->IsChecked())
		//			continue;

		//		KMultiAssignInfo*       pDlg     = (KMultiAssignInfo*)pItem->GetItemData();
		//		std::list<KAInputInfo*> lstInput = pDlg->GetInputInfo();
		//		KTarget*                pTarget  = pDlg->Target();
		//		CString                 strData(_T(""));

		//		std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

		//		while (bIter != lstInput.end())
		//		{
		//			KAInputInfo* pInfo = *bIter;

		//			if (pInfo->Selected())
		//			{
		//				KIOColumn* pColumn = pInfo->InputColumn();
		//				KMode*     pMode   = pInfo->Mode();

		//				if (!strData.IsEmpty())
		//					strData.Append(_T(";"));

		//				strData.AppendFormat(_T("%d:%s"), pMode->ModeID(), pColumn->Caption());
		//			}

		//			++bIter;
		//		}

		//		KDBaseMultiAssign::InsertMultiData(nID, pTarget->CaptionScenario(), pTarget->CaptionTargetYear(), strData);
		//	}
		//}
		int nRecord = m_wndTree.GetRecords()->GetCount();
		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord* pRecord = m_wndTree.GetRecords()->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);

			KMultiAssignInfoSOBA*       pInfo     = (KMultiAssignInfoSOBA*)pItem->GetItemData();
			KARunningOptionInfo& ro = pInfo->GetRunningOption();
			std::list<KAInputInfo*> lstInput = pInfo->GetInputInfo();
			KTarget*                pTarget  = pInfo->Target();
			CString                 strData(_T(""));

			std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

			while (bIter != lstInput.end())
			{
				KAInputInfo* pInputInfo = *bIter;

				if (pInputInfo->Selected())
				{
					KIOColumn* pColumn = pInputInfo->InputColumn();
					KMode*     pMode   = pInputInfo->Mode();

					if (!strData.IsEmpty())
						strData.Append(_T(";"));

					strData.AppendFormat(_T("%d:%s"), pMode->ModeID(), pColumn->Caption());
				}

				++bIter;
			}

			CString strWSScenario;
			CString strWSTarget;
			ASSERT(NULL != ro.Scnario() && NULL != ro.Target());

			if (NULL != ro.Scnario())
			{
				strWSScenario = ro.Scnario()->GetName();
			}
			if (NULL != ro.Target())
			{
				strWSTarget = ro.Target()->GetName();
			}

			KDBaseMultiAssignSOBA::InsertMultiData(nID, i+1, pTarget->CaptionScenario(), pTarget->CaptionTargetYear(), strData, 
				strWSScenario, strWSTarget);
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		throw 1;
	}
	catch (...)
	{
		AfxMessageBox(_T("여러 시나리오 반복 실행 설정 정보 저장에 실패하였습니다."));
		throw 1;
	}
}

CString MultiAssignModelSOBADlg::ProjectModes()
{
	CString strModes;

	KProject*           pProject = ImChampFrameWindow::GetProject();
	std::vector<KMode*> vecMode  = pProject->GetMode();

	int nSize = vecMode.size();

	for (int i = 0; i < nSize; ++i)
	{
		KMode* pMode = vecMode[i];

		if (i == 0)
		{
			strModes.Format(_T("%d"), pMode->ModeID());
		}
		else
		{
			strModes.AppendFormat(_T(";%d"), pMode->ModeID());
		}
	}

	return strModes;
}

void MultiAssignModelSOBADlg::NewMultiScenarioName( void )
{
	CString strValue;
	int     nID = KDBaseMultiAssignSOBA::NextMultiInfoID();

	strValue.Format(_T("Multi_Scenario_Assign(%d)"), nID);

	while (KDBaseMultiAssignSOBA::IsExistName(strValue))
	{
		strValue.Format(_T("Multi_Scenario_Assign(%d)"), ++nID);
	}
	
	SetDlgItemText(IDC_EDIT_NAME, strValue);
}

void MultiAssignModelSOBADlg::InitMultiScenarioCombo( void )
{
	//Get Data
	std::map<int, CString> mapInfo;
	CString strModes = ProjectModes();

	KDBaseMultiAssignSOBA::SelectMultiInfos(strModes, mapInfo);

	//Init Control
	m_cboName.ResetContent();

	int nIndex;
	
	nIndex = m_cboName.InsertString(-1, _T("---- Insert New Data ----"));
	m_cboName.SetItemData(nIndex, -1);

	std::map<int, CString>::iterator bIter = mapInfo.begin();

	while (bIter != mapInfo.end())
	{
		nIndex = m_cboName.InsertString(-1, bIter->second);
		m_cboName.SetItemData(nIndex, bIter->first);

		++bIter;
	}

	m_cboName.SetCurSel(0);
}

void MultiAssignModelSOBADlg::OnCbnSelChangeComboName( void )
{
	CString strValue;
	int     nCurSel = m_cboName.GetCurSel();

	if (nCurSel == 0)
	{
		NewMultiScenarioName();

		InitDefaultData();
		UpdateTreeData();

		m_wndReport.ResetContent();
	}
	else
	{
		m_cboName.GetLBText(nCurSel, strValue);
		SetDlgItemText(IDC_EDIT_NAME, strValue);

		InitDefaultData();
		UpdateSavedData();
		UpdateTreeData();
	}

	int nSize = m_vecInfo.size();

	if (nSize != 0)
	{
		KMultiAssignInfoSOBA* pInfo = m_vecInfo[0];	//0 번은 Highway Assignment 고정

		if (m_pInfo != nullptr)
		{
			SaveModeInfo();
			SaveOutputInfo();
		}

		UpdateModeData(pInfo);
		UpdateOutputData(pInfo);

		m_pInfo = pInfo;
	}
}

void MultiAssignModelSOBADlg::UpdateSavedData( void )
{
	//RedrawTreeCtrl();

	// 기존 선택된 배열에서 활성 시나리오만 남기고 제거
	auto it = m_arrSelectedInfo.begin();
	while (it != m_arrSelectedInfo.end())
	{
		KMultiAssignInfo* pSelectedInfo = *it;
		if (!pSelectedInfo->IsBase())
		{
			it = m_arrSelectedInfo.erase(it);
		}
		else
		{
			++it;
		}
	}

	// 저장된 실행목록을 불러와서 선택 목록에 추가
	int nID = m_cboName.GetItemData(m_cboName.GetCurSel());

	std::vector<TMultiAssignDataSOBA> vecData;
	KDBaseMultiAssignSOBA::SelectMultiDatas(nID, vecData);

	for (TMultiAssignDataSOBA& mad : vecData)
	{
		for (KMultiAssignInfoSOBA* pInfo : m_vecInfo)
		{
			if (pInfo->IsBase())
			{
				continue;
			}

			KTarget* pTarget = pInfo->Target();

			CString strScenario = pTarget->CaptionScenario();
			CString strTarget = pTarget->CaptionTargetYear();

			if (mad.strScenario.CompareNoCase(strScenario) == 0 && mad.strTarget.CompareNoCase(strTarget) == 0)
			{
				UpdateMatrixData(pInfo, mad.strData);
				// 웜스타트 초기치 대상 확인

				if (CheckWormStartRunnable(mad.strWSScenario, mad.strWSTarget))
				{
					KTarget* pWSTarget = GetWormStartTarget(mad.strWSScenario, mad.strWSTarget);
					ASSERT(NULL != pWSTarget);
					if (CheckMode(pInfo, pWSTarget))
					{
						KARunningOptionInfo& ro = pInfo->GetRunningOption();
						ro.Target(pWSTarget);
						ro.Scnario((KScenario*)pWSTarget->GetParentObject());
						pInfo->SetSelected(true);
						m_arrSelectedInfo.push_back(pInfo);
					}
				}
				break;
			}
		}
	}

	//for (KMultiAssignInfoSOBA* pInfo : m_vecInfo)
	//{
	//	if (pInfo->IsBase())
	//	{
	//		continue;
	//	}

	//	KTarget* pTarget = pInfo->Target();

	//	CString strScenario = pTarget->CaptionScenario();
	//	CString strTarget = pTarget->CaptionTargetYear();

	//	for (TMultiAssignDataSOBA& mad : vecData)
	//	{
	//		if (mad.strScenario.CompareNoCase(strScenario) == 0 && mad.strTarget.CompareNoCase(strTarget) == 0)
	//		{
	//			UpdateMatrixData(pInfo, mad.strData);
	//			// 웜스타트 초기치 대상 확인
	//			if (CheckWormStartRunnable(mad.strWSScenario, mad.strWSTarget))
	//			{
	//				KTarget* pWSTarget = GetWormStartTarget(mad.strWSScenario, mad.strWSTarget);
	//				ASSERT(NULL != pWSTarget);
	//				KARunningOptionInfo& ro = pInfo->GetRunningOption();
	//				ro.Target(pWSTarget);
	//				ro.Scnario((KScenario*)pWSTarget->GetParentObject());
	//				pInfo->SetSelected(true);
	//				m_arrSelectedInfo.push_back(pInfo);
	//				
	//			}
	//			break;
	//		}
	//	}
	//}

}

//void MultiAssignModelSOBADlg::RedrawTreeCtrl( void )
//{
//	std::vector<TMultiAssignDataSOBA> vecData;
//
//	int nID = m_cboName.GetItemData(m_cboName.GetCurSel());
//	
//	KDBaseMultiAssignSOBA::SelectMultiDatas(nID, vecData);
//
//	CXTPReportRecord*     pScenarioRecord = nullptr;
//	CXTPReportRecords*    pTargetRecords  = nullptr;
//	CXTPReportRecord*     pTargetRecord   = nullptr;
//	CXTPReportRecordItem* pItem           = nullptr;
//
//	int nRecord = m_wndTree.GetRecords()->GetCount();
//
//	for (int i = 0; i < nRecord; ++i)
//	{
//		pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
//		pTargetRecords  = pScenarioRecord->GetChilds();
//
//		int nTargetRecord = pTargetRecords->GetCount();
//
//		for (int j = 0; j < nTargetRecord; ++j)
//		{
//			pTargetRecord = pTargetRecords->GetAt(j);
//			pItem         = pTargetRecord->GetItem(0);
//			
//			KMultiAssignInfoSOBA* pDlg = (KMultiAssignInfoSOBA*)pItem->GetItemData();
//
//			KTarget* pTarget = pDlg->Target();
//
//			CString strScenario = pTarget->CaptionScenario();
//			CString strTarget   = pTarget->CaptionTargetYear();
//
//			int nSize = vecData.size();
//
//			for (int k = 0; k < nSize; ++k)
//			{
//				TMultiAssignDataSOBA oData = vecData[k];
//
//				if (oData.strScenario.CompareNoCase(strScenario) == 0 && oData.strTarget.CompareNoCase(strTarget) == 0)
//				{
//					pItem->SetChecked(TRUE);
//					UpdateMatrixData(pDlg, oData.strData);
//
//					break;
//				}
//			}
//		}
//	}
//
//	m_wndTree.RedrawControl();
//}

void MultiAssignModelSOBADlg::UpdateMatrixData(KMultiAssignInfoSOBA* a_pDlg, CString a_strData )
{
	std::map<int, CString> mapData;
	SeperateBySemicolon(a_strData, mapData);

	std::map<int, CString>::iterator fIter;

	std::list<KAInputInfo*> lstInput = a_pDlg->GetInputInfo();

	std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

	while (bIter != lstInput.end())
	{
		KAInputInfo* pInfo = *bIter;
		
		KMode* pMode = pInfo->Mode();
		fIter = mapData.find(pMode->ModeID());

		if (fIter != mapData.end())
		{
			
			KIOTable* pTable = a_pDlg->Target()->Tables()->FindTable(TABLE_MODE_OD);
			const KIOColumns* pColumns	= pTable->Columns();

			int nColumn = pColumns->ColumnCount();

			for (int i = 0; i < nColumn; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);
				CString    strCaption = pColumn->Caption();

				if (strCaption.CompareNoCase(fIter->second) == 0)
				{
					pInfo->Selected(true);
					pInfo->InputColumn(pColumn);
				}				
			}
		}

		++bIter;
	}
}

void MultiAssignModelSOBADlg::SeperateBySemicolon( CString a_strValue, std::map<int, CString>& a_mapData )
{
	a_mapData.clear();
	std::vector<CString> vecData;

	try
	{
		CString strTok;
		int     nTokenPos = 0;

		while (AfxExtractSubString(strTok, a_strValue, nTokenPos++, _T(';')))
		{
			vecData.push_back(strTok);
		}
		
		int nSize = vecData.size();

		for (int i = 0; i < nSize; ++i)
		{
			CString strID;
			CString strColumn;

			int nIndex = 0;
			nTokenPos  = 0;

			while (AfxExtractSubString(strTok, vecData[i], nTokenPos++, _T(':')))
			{
				if (nIndex++ == 0)
				{
					strID = strTok;
				}
				else
				{
					strColumn = strTok;
				}
			}

			int nID = _ttoi(strID);

			a_mapData.insert(std::make_pair(nID, strColumn));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void MultiAssignModelSOBADlg::ValidateInput( void )
{
	try
	{
		// 2019.04.22 수정 : 분석할 시나리오를 트리에 추가하는 방식으로 변경
		//CXTPReportRecord*     pScenarioRecord = nullptr;
		//CXTPReportRecords*    pTargetRecords  = nullptr;
		//CXTPReportRecord*     pTargetRecord   = nullptr;
		//CXTPReportRecordItem* pItem           = nullptr;

		//int nRecord = m_wndTree.GetRecords()->GetCount();

		//for (int i = 0; i < nRecord; ++i)
		//{
		//	pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
		//	pTargetRecords  = pScenarioRecord->GetChilds();

		//	int nTargetRecord = pTargetRecords->GetCount();

		//	for (int j = 0; j < nTargetRecord; ++j)
		//	{
		//		pTargetRecord = pTargetRecords->GetAt(j);
		//		pItem         = pTargetRecord->GetItem(0);

		//		if (!pItem->IsChecked())
		//			continue;

		//		bool bIsChecked = false;

		//		KMultiAssignInfo*       pDlg     = (KMultiAssignInfo*)pItem->GetItemData();
		//		std::list<KAInputInfo*> lstInput = pDlg->GetInputInfo();

		//		std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

		//		while (bIter != lstInput.end())
		//		{
		//			KAInputInfo* pInfo = *bIter;

		//			if (pInfo->Selected())
		//			{
		//				bIsChecked = true;
		//				break;
		//			}

		//			++bIter;
		//		}

		//		if (!bIsChecked)
		//		{
		//			ThrowException(_T("수단정보를 선택하지 않았습니다."));
		//		}

		//	}
		//}
		int nRecord = m_wndTree.GetRecords()->GetCount();
		for (int i = 0; i < nRecord; ++i)
		{
			CXTPReportRecord* pRecord = m_wndTree.GetRecords()->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);

			bool bIsChecked = false;

			KMultiAssignInfoSOBA*       pDlg     = (KMultiAssignInfoSOBA*)pItem->GetItemData();
			std::list<KAInputInfo*> lstInput = pDlg->GetInputInfo();

			std::list<KAInputInfo*>::iterator bIter = lstInput.begin();

			while (bIter != lstInput.end())
			{
				KAInputInfo* pInfo = *bIter;

				if (pInfo->Selected())
				{
					bIsChecked = true;
					break;
				}

				++bIter;
			}

			if (!bIsChecked)
			{
				ThrowException(_T("수단정보를 선택하지 않았습니다."));
			}

		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}	
}


void MultiAssignModelSOBADlg::OnBnClickedDelete()
{
	int nCurSel = m_cboName.GetCurSel();

	if (nCurSel == 0)
	{
		AfxMessageBox(_T("삭제할 항목을 선택해 주세요."));
		return;
	}

	int nID = m_cboName.GetItemData(nCurSel);

	KDBaseMultiAssignSOBA::DeleteMultiAssign(nID);
	
	InitMultiScenarioCombo();
	OnCbnSelChangeComboName();
}


void MultiAssignModelSOBADlg::OnBnClickedBtnAddScenario()
{
	KSOBASelectScenarioDlg dlg(m_pTarget);
	dlg.SetMultiAssignInfoArray(m_vecInfo);

	if (dlg.DoModal() == IDOK)
	{
		KMultiAssignInfoSOBA* pInfo = dlg.GetSelectedAssignInfo();
		if (NULL != pInfo)
		{
			pInfo->SetSelected(true);
			m_arrSelectedInfo.push_back(pInfo);
			UpdateTreeData();
			UpdateModeData(pInfo);
			UpdateOutputData(pInfo);
			
			m_pInfo = pInfo;
		}
	}
}


void MultiAssignModelSOBADlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*)pNotifyStruct;
	if (!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);

	KMultiAssignInfoSOBA* pDeletedInfo = (KMultiAssignInfoSOBA*)pItem->GetItemData();
	if (NULL == pDeletedInfo)
	{
		return;
	}

	for (std::vector<KMultiAssignInfoSOBA*>::iterator it = m_arrSelectedInfo.begin(); it != m_arrSelectedInfo.end(); ++it)
	{
		if (pDeletedInfo == *it)
		{
			pDeletedInfo->SetSelected(false);
			m_arrSelectedInfo.erase(it);
			break;
		}
	}


	// 해당 분석 대상이 다른 시나리오에서 참조될 경우 이전 분석 대상이 있으면 그대로 두고, 없으면 분석 대상에서 제거
	std::queue<KMultiAssignInfoSOBA*> queTemp;
	queTemp.push(pDeletedInfo);
	
	while (!queTemp.empty())
	{
		KMultiAssignInfoSOBA* pDeleted = queTemp.front();
		queTemp.pop();

		KTarget* pTargetDeleted = pDeleted->Target();
		ASSERT(NULL != pTargetDeleted);

		for (std::vector<KMultiAssignInfoSOBA*>::iterator it = m_arrSelectedInfo.begin(); it != m_arrSelectedInfo.end(); )
		{
			KMultiAssignInfoSOBA* pSelected = *it;
			if (pSelected->IsBase())
			{
				++it;
				continue;
			}

			KARunningOptionInfo& roSelected = pSelected->GetRunningOption();

			if (pTargetDeleted == roSelected.Target())
			{
				if (CheckTargetBushFile(pTargetDeleted) == false || CheckMode(pSelected, pTargetDeleted) == false)
				{
					pSelected->SetSelected(false);
					it = m_arrSelectedInfo.erase(it);
					queTemp.push(pSelected);
					//if (CheckTargetBushFile(pSelected->Target()) == false)
					//{
					//	queTemp.push(pSelected);
					//}
				}
				else
				{
					++it;
				}
			}
			else
			{
				++it;
			}
		}

		KARunningOptionInfo& roDeleted = pDeleted->GetRunningOption();
		roDeleted.Target(NULL);
		roDeleted.Scnario(NULL);
	}

	UpdateTreeData();
}


bool MultiAssignModelSOBADlg::CheckTargetBushFile(KTarget* pTarget)
{
	try
	{
		CString strBushPath(_T(""));
		strBushPath.Format(_T("%s\\%s\\%s"), pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, _T("*.bsh"));

		CFileFind oCFileFinder;
		if (FALSE == oCFileFinder.FindFile(strBushPath))
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}



bool MultiAssignModelSOBADlg::CheckWormStartRunnable(CString strWSScenario, CString strWSTarget)
{
	// 웜스타트 대상 정보를 검색
	KMultiAssignInfoSOBA* pWSInfo = NULL;

	for (KMultiAssignInfoSOBA* pInfo : m_vecInfo)
	{
		KTarget* pTarget = pInfo->Target();

		CString strScenario = pTarget->CaptionScenario();
		CString strTarget = pTarget->CaptionTargetYear();

		if (strWSScenario.CompareNoCase(strScenario) == 0 && strWSTarget.CompareNoCase(strTarget) == 0)
		{
			pWSInfo = pInfo;
			break;
		}
	}

	if (NULL == pWSInfo || pWSInfo->Target() == NULL)
	{
		return false;
	}

	KTarget* pWSTarget = pWSInfo->Target();

	if (pWSTarget->IsEmptyData())
	{
		return false;
	}

	// 선행 분석 대상으로 존재 하는지 확인
	bool bRunnable = false;
	for (KMultiAssignInfoSOBA* pSelected : m_arrSelectedInfo)
	{
		if (pWSInfo->Target() == pSelected->Target())
		{
			bRunnable = true;
			break;
		}
	}

	if (!bRunnable)
	{
		if (CheckTargetBushFile(pWSTarget))
		{
			bRunnable = true;
		}
	}

	return bRunnable;
}


KTarget* MultiAssignModelSOBADlg::GetWormStartTarget(CString strWSScenario, CString strWSTarget)
{
	KTarget* pWSTarget = NULL;
	for (KMultiAssignInfoSOBA* pInfo : m_vecInfo)
	{
		KTarget* pTarget = pInfo->Target();

		CString strScenario = pTarget->CaptionScenario();
		CString strTarget = pTarget->CaptionTargetYear();

		if (strWSScenario.CompareNoCase(strScenario) == 0 && strWSTarget.CompareNoCase(strTarget) == 0)
		{
			pWSTarget = pInfo->Target();
			break;
		}
	}

	return pWSTarget;
}


void MultiAssignModelSOBADlg::GetExistModeID(KTarget* pTarget, std::set<int>& setModeInfo)
{
	// 대상 Target이 이미 선택한 시나리오로인 경우 해당 MultiAssignInfoSOBA의 Input리스트를 검색
	// 그렇지 않으면 DB에서 검색

	setModeInfo.clear();

	KMultiAssignInfoSOBA* pSelectedInfo = NULL;
	for (KMultiAssignInfoSOBA* pInfo : m_arrSelectedInfo)
	{
		if (pInfo->Target() == pTarget)
		{
			pSelectedInfo = pInfo; 
			break;
		}
	}


	if (NULL == pSelectedInfo)
	{
		CString strSQL(_T(""));
		try
		{
			strSQL.Format(_T("Select mode_id From %s Order by mode_id"), TABLE_ASSIGN_PATH_INPUT);

			KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
			KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

			while (spResultset->Next())
			{
				int nModeID = spResultset->GetValueInt(0);
				setModeInfo.insert(nModeID);
			}
		}
		catch (KExceptionPtr ex)
		{
			TxLogDebug(ex->GetErrorMessage());
		}
		catch (...)
		{
			TxLogDebugException();
		}
	}
	else
	{
		std::list<KAInputInfo*>& lstInput = pSelectedInfo->GetInputInfo();
		for (KAInputInfo* pInput : lstInput)
		{
			if (pInput->Selected())
			{
				setModeInfo.insert(pInput->Mode()->ModeID());
			}
		}
	}
}


bool MultiAssignModelSOBADlg::CheckMode(KMultiAssignInfoSOBA* pInfo, KTarget* pWSTarget)
{
	if (pInfo == NULL || pWSTarget == NULL)
	{
		return false;
	}

	try
	{
		std::set<int> setSelectMode;
		std::set<int> setExistMode;

		std::list<KAInputInfo*>& lstRunInput = pInfo->GetInputInfo();
		for (KAInputInfo* pInput : lstRunInput)
		{
			if (pInput->Selected())
			{
				setSelectMode.insert(pInput->Mode()->ModeID());
			}
		}

		GetExistModeID(pWSTarget, setExistMode);

		if (setSelectMode.size() != setExistMode.size())
		{
			return false;
		}

		for (int nModeId : setSelectMode)
		{
			if (setExistMode.find(nModeId) == setExistMode.end())
			{
				return false;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return true;
}
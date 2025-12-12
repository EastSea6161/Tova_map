// MultiScenarioSelect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ZoningMultiScenarioSelectDlg.h"
#include "afxdialogex.h"
#include "ImChampFrameWindow.h"
#include "Project.h"
#include "Scenario.h"



const CString OPERATION_STRING_DIVISION = _T("현재 세분화 할 존 노드 번호 : ");
const CString OPERATION_STRING_INTEGRATION = _T("현재 집합화 할 존 노드 번호 : ");
const CString OPERATION_STRING_DIVISION_EN = _T("Zone ID(to be divided) : ");
const CString OPERATION_STRING_INTEGRATION_EN = _T("Zone ID(to be integrated) : ");


// KZoningMultiScenarioSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KZoningMultiScenarioSelectDlg, KDialogEx)

KZoningMultiScenarioSelectDlg::KZoningMultiScenarioSelectDlg(KEMZoningOperation emOperation, CWnd* pParent /*=NULL*/)
	: KDialogEx(IDD_4410_MultiScenarioSelect, pParent), m_emOperation(emOperation)
	, m_strOperation(_T(""))
{
	m_pActiveTarget = nullptr;
	if (KEMZoningOperationDivision == m_emOperation)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_strOperation = OPERATION_STRING_DIVISION;
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			m_strOperation = OPERATION_STRING_DIVISION_EN;
		}
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_strOperation = OPERATION_STRING_INTEGRATION;
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			m_strOperation = OPERATION_STRING_INTEGRATION_EN;
		}
	}
}


KZoningMultiScenarioSelectDlg::~KZoningMultiScenarioSelectDlg()
{
}


void KZoningMultiScenarioSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtNodeList);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_strOperation);
}


BEGIN_MESSAGE_MAP(KZoningMultiScenarioSelectDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KZoningMultiScenarioSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KZoningMultiScenarioSelectDlg 메시지 처리기입니다.


BOOL KZoningMultiScenarioSelectDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, TRUE);


	UpdateData(FALSE);

	m_strSelectedNode = _T("");
	for (__int64 nodeId : m_setSelectedNode)
	{
		CString strTemp;
		strTemp.Format(_T("%I64d, "), nodeId);
		m_strSelectedNode += strTemp;
	}

	if (m_strSelectedNode.GetLength() > 2)
	{
		m_strSelectedNode.Delete(m_strSelectedNode.GetLength() - 2);
	}

	m_edtNodeList.SetWindowText(m_strSelectedNode);
	

	initTreeControlHeader();
	initTreeControlRecord();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KZoningMultiScenarioSelectDlg::SetTarget(KTarget* pTarget)
{
	m_pActiveTarget = pTarget;
}



void KZoningMultiScenarioSelectDlg::SetSelectedZoneNodeSet(std::set<__int64>& setSelectedNode)
{
	m_setSelectedNode = setSelectedNode;
}


std::vector<KTarget*> KZoningMultiScenarioSelectDlg::GetSelectedTarget()
{
	return m_arrSelectedTarget;
}




void KZoningMultiScenarioSelectDlg::initTreeControlHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	int nItemIndex(0);

	KEMLanguageType emLanguage = KmzSystem::GetLanguage();

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Select Target"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
	if (KEMKorea == emLanguage) {
		pColumn->SetCaption(_T("시나리오 선택"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(nItemIndex, _T("Status"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KEMKorea == emLanguage)
	{
		pColumn->SetCaption(_T("상태설명"));
	}
}


void KZoningMultiScenarioSelectDlg::initTreeControlRecord()
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecord*		pRecord = nullptr;
		CXTPReportRecordItem*	pItem = nullptr;

		KProject* pProject = ImChampFrameWindow::GetProject();

		//Base Scenario
		KScenario* pBaseScenario = pProject->GetBaseScenario();
		if (nullptr != pBaseScenario && pBaseScenario->GetTargetMap().size() > 0)
		{
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pBaseScenario->GetName()));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR)pBaseScenario);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetEditable(FALSE);

			addTreeRecord(pRecord, pBaseScenario);
		}

		const KTDScenarioMap& mapScenario = pProject->GetScenarioMap();
		for (KTDScenarioMap::const_iterator citScenario = mapScenario.begin();
			citScenario != mapScenario.end(); ++citScenario)
		{
			KScenario* pScenario = citScenario->second;
			if (nullptr == pScenario || pScenario->GetTargetMap().empty())
			{
				continue;
			}

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pScenario->GetName()));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR)pScenario);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetEditable(FALSE);


			addTreeRecord(pRecord, pScenario);
		}

		m_wndReportCtrl.Populate();
		m_wndReportCtrl.ExpandAll();
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


void KZoningMultiScenarioSelectDlg::addTreeRecord(CXTPReportRecord* pRecord, KScenario* pScenario)
{
	if (pRecord == nullptr || pScenario == nullptr)
	{
		return;
	}

	CXTPReportRecord*     pChildRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	const KTDTargetMap& mapTarget = pScenario->GetTargetMap();
	for (KTDTargetMap::const_iterator citTarget = mapTarget.begin(); citTarget != mapTarget.end(); ++citTarget)
	{
		KTarget* pTarget = citTarget->second;
		if (nullptr == pTarget)
		{
			continue;
		}


		pChildRecord = pRecord->GetChilds()->Add(new CXTPReportRecord());
		pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(pTarget->GetName()));
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetItemData((DWORD_PTR)pTarget);

		if (pTarget == m_pActiveTarget)
		{
			pItem->SetChecked(TRUE);
			pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(_T("Selectable")));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItem->SetCaption(_T("선택 가능"));
			}
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		else
		{
			if (checkSelectable(pTarget))
			{
				pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(_T("Selectable")));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pItem->SetCaption(_T("선택 가능"));
				}
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				pItem->SetEditable(FALSE);
				pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(_T("Zone node does not exist")));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pItem->SetCaption(_T("존 노드가 존재하지 않음"));
				}
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
		}
	}
}


bool KZoningMultiScenarioSelectDlg::checkSelectable(KTarget* pTarget)
{
	CString strSQL;
	strSQL.Format(_T("Select zone_id From %s Where zone_id in (%s)"), TABLE_ZONE, m_strSelectedNode);

	std::set<__int64> setZone;
	KDBaseConPtr spDBConnection = pTarget->GetDBaseConnection();
	KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
	int count = 0;
	while (spResultset->Next())
	{
		__int64 zoneId = spResultset->GetValueInt64(0);
		setZone.insert(zoneId);
	}

	if (setZone != m_setSelectedNode)
	{
		return false;
	}

	return true;
}

void KZoningMultiScenarioSelectDlg::OnBnClickedOk()
{
	m_arrSelectedTarget.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*  pRecord = nullptr;
		int                nRecord = pRecords->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = pRecords->GetAt(i);
			if (pRecord->HasChildren())
			{
				CXTPReportRecords* pChildRecords = pRecord->GetChilds();
				int                nChildRecord = pChildRecords->GetCount();

				for (int j = 0; j < nChildRecord; ++j)
				{
					CXTPReportRecord*  pRecord = pChildRecords->GetAt(j);
					CXTPReportRecordItem* pItem = pRecord->GetItem(0);
					if (nullptr == pItem)
					{
						continue;
					}

					if (pItem->IsChecked())
					{
						KTarget* pTarget = (KTarget*)(pItem->GetItemData());
						m_arrSelectedTarget.push_back(pTarget);
					}
				}
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return;
	}
	catch (...)
	{
		TxLogDebug(_T("unexpected error - KCopyProject::OnBnClickedCheck1"));
		return;
	}

	std::vector<KTarget*> m_arrSelectedTarget;

	CDialogEx::OnOK();
}

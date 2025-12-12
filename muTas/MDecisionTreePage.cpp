/**
 * @file MDecisionTreePage.cpp
 * @brief KMDecisionTreePage 구현 파일
 * @author 
 * @date 2011.06.07
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "MDecisionTreePage.h"
#include "afxdialogex.h"
#include <queue>
#include "Mode.h"
#include "ModeManager.h"
#include "ModeTree.h"
#include "ModeTreeNodeMode.h"
#include "Target.h"


// KMDecisionTreePage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMDecisionTreePage , CXTPPropertyPage)

KMDecisionTreePage ::KMDecisionTreePage ()
	: CXTPPropertyPage(KMDecisionTreePage ::IDD)
{
	m_pModeTree = new KModeTree;
}

KMDecisionTreePage ::~KMDecisionTreePage ()
{
	if(NULL != m_pModeTree)
	{
		delete m_pModeTree;
	}
}

void KMDecisionTreePage ::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_COMBO1, m_cmbMode);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON2, m_btnRemove);
}


BEGIN_MESSAGE_MAP(KMDecisionTreePage , CXTPPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &KMDecisionTreePage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KMDecisionTreePage::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON3, &KMDecisionTreePage::OnBnClickedButton3)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_REPORT, OnReportSelChanged)
END_MESSAGE_MAP()



void KMDecisionTreePage::Target(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KMDecisionTreePage::ModeChoiceModel(KEMModeChoiceModel emMModel)
{
	m_emMModel = emMModel;
}


KModeTree* KMDecisionTreePage::ModeTree(void) const
{
	return m_pModeTree;
}


void KMDecisionTreePage::ModeTree(const KModeTree* pModeTree)
{
	if(NULL != pModeTree)
	{
		*m_pModeTree = *pModeTree;
	}
}


void KMDecisionTreePage::initReportControl(void)
{
	KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE);

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Mode"), 150));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);
}


void KMDecisionTreePage::updateReportRecords(void)
{
	m_wndReport.GetRecords()->RemoveAll();

	std::queue<KModeTreeNodeMode*> modeQ;
	std::queue<CXTPReportRecord*> recordQ;

	CXTPReportRecord* pRootRecord = m_wndReport.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pRootItem = pRootRecord->AddItem(new CXTPReportRecordItemText);
	pRootItem->SetCaption(_T("Decision Tree"));

	int nModeCount = m_pModeTree->ModeNodeCount();
	for(int i = 0; i < nModeCount; ++i)
	{
		KModeTreeNodeMode* pModeNode = m_pModeTree->ModeNode(i);
		modeQ.push(pModeNode);
		recordQ.push(pRootRecord);
	}

	while(modeQ.empty() == false)
	{
		ASSERT(modeQ.size() == recordQ.size());
		KModeTreeNodeMode* pModeNode = modeQ.front();
		modeQ.pop();
		CXTPReportRecord* pParentRecord = recordQ.front();
		recordQ.pop();

		CXTPReportRecord* pRecord;
		CXTPReportRecordItem* pItem;
		pRecord = pParentRecord->GetChilds()->Add(new CXTPReportRecord);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pModeNode->ModeName());
		pItem->SetItemData((DWORD_PTR)pModeNode);

		// remain mode list에서 report에 추가된 mode를 삭제
		removeModeFromList(pModeNode->Mode());

		int nSubNodeCount = pModeNode->NodeCount();
		for(int i = 0; i < nSubNodeCount; ++i)
		{
			KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pModeNode->Node(i));
			modeQ.push(pSubNode);
			recordQ.push(pRecord);
		}
	}

	m_wndReport.Populate();
	m_wndReport.ExpandAll(TRUE);
	m_wndReport.SetFocusedRow(m_wndReport.GetRows()->GetAt(0));
}


void KMDecisionTreePage::initModeList(void)
{
	// todo : mode List를 초기화 하는 code 구현
	KModeManager* pModeManager = m_pTarget->ModeManager();
	ASSERT(NULL != pModeManager);
	int nModeCount = pModeManager->GetModeCount();
	for(int i = 0; i < nModeCount; ++i)
	{
		KMode* pMode = pModeManager->GetMode(i);
		m_ModeList.push_back(pMode);
	}
	//KMode* pMode1 = new KMode(KEMModeGroupODBase, 1, _T("Auto"));
	//m_ModeList.push_back(pMode1);
	//KMode* pMode2 = new KMode(KEMModeGroupODBase, 2, _T("Transit"));
	//m_ModeList.push_back(pMode2);
	//KMode* pMode3 = new KMode(KEMModeGroupODBase, 3, _T("Bus"));
	//m_ModeList.push_back(pMode3);
	//KMode* pMode4 = new KMode(KEMModeGroupODBase, 4, _T("Rail"));
	//m_ModeList.push_back(pMode4);
	//KMode* pMode5 = new KMode(KEMModeGroupODBase, 5, _T("GTX"));
	//m_ModeList.push_back(pMode5);


	m_RemainModeList = m_ModeList;
}


void KMDecisionTreePage::updateModeCombobox(void)
{
	if(m_cmbMode.GetSafeHwnd() == NULL)
	{
		return;
	}

	m_cmbMode.ResetContent();

	std::list<KMode*>::iterator itMode, itEnd = m_RemainModeList.end();
	for(itMode = m_RemainModeList.begin(); itMode != itEnd; ++itMode)
	{
		KMode* pMode = *itMode;
		int nIndex = m_cmbMode.AddString(pMode->ModeName());
		m_cmbMode.SetItemData(nIndex, (DWORD_PTR)pMode);
	}
	m_cmbMode.SetCurSel(0);
}


void KMDecisionTreePage::removeModeFromList(KMode* pMode)
{
	std::list<KMode*>::iterator itMode, itEnd = m_RemainModeList.end();
	for(itMode = m_RemainModeList.begin(); itMode != itEnd; ++itMode)
	{
		KMode* pRemainMode = *itMode;
		if(pMode->ModeID() == pRemainMode->ModeID())
		{
			m_RemainModeList.erase(itMode);
			break;
		}
	}
}


void KMDecisionTreePage::addNotNestedLogit(void)
{
	int nSel = m_cmbMode.GetCurSel();
	KMode* pMode = (KMode*)(m_cmbMode.GetItemData(nSel));
	KModeTreeNodeMode* pNode = m_pModeTree->AddModeNode();
	pNode->Mode(pMode);
}


void KMDecisionTreePage::addNestedLogit(void)
{
	int nSel = m_cmbMode.GetCurSel();
	KMode* pMode = (KMode*)(m_cmbMode.GetItemData(nSel));

	CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	if(NULL == pRow)
	{
		return;
	}

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KModeTreeNodeMode* pParentNode = (KModeTreeNodeMode*)(pItem->GetItemData());
	KModeTreeNodeMode* pNode = NULL;
	if(NULL == pParentNode)
	{
		pNode = m_pModeTree->AddModeNode();
	}
	else
	{
		pNode = pParentNode->AddModeNode();
	}
	pNode->Mode(pMode);

}


void KMDecisionTreePage::removeNotNestedLogit(void)
{
	CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	if(NULL == pRow)
	{
		return;
	}

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KModeTreeNodeMode* pRemoveNode = (KModeTreeNodeMode*)(pItem->GetItemData());
	if(NULL == pRemoveNode)
	{
		return;
	}
	KMode* pMode = pRemoveNode->Mode();
	m_RemainModeList.push_back(pMode);
	m_pModeTree->RemoveModeNode(pRemoveNode);
}


void KMDecisionTreePage::removeNestedLogit(void)
{
	CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	if(NULL == pRow)
	{
		return;
	}

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecordItem* pItem = pRecord->GetItem(0);
	KModeTreeNodeMode* pRemoveNode = (KModeTreeNodeMode*)(pItem->GetItemData());
	if(NULL == pRemoveNode)
	{
		return;
	}

	std::queue<KModeTreeNodeMode*> nodeQ;
	nodeQ.push(pRemoveNode);
	while(nodeQ.empty() == false)
	{
		KModeTreeNodeMode* pNode = nodeQ.front();
		nodeQ.pop();
		KMode* pMode = pNode->Mode();
		m_RemainModeList.push_back(pMode);
		int nNodeCount = pNode->NodeCount();
		for(int i = 0; i < nNodeCount; ++i)
		{
			KModeTreeNodeMode* pSubNode = (KModeTreeNodeMode*)(pNode->Node(i));
			nodeQ.push(pSubNode);
		}
	}

	CXTPReportRecord* pParentRecord = pRecord->GetParentRecord();
	CXTPReportRecordItem* pParentItem = pParentRecord->GetItem(0);
	KModeTreeNodeMode* pParentNode = (KModeTreeNodeMode*)(pParentItem->GetItemData());
	if(NULL == pParentNode)
	{
		m_pModeTree->RemoveModeNode(pRemoveNode);
	}
	else
	{
		pParentNode->RemoveNode(pRemoveNode);
	}
}


void KMDecisionTreePage::updateButtonState(void)
{
	CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	if(NULL == pRow)
	{
		m_btnAdd.EnableWindow(FALSE);
		m_btnRemove.EnableWindow(FALSE);
	}
	else
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);

		CXTPReportRecord* pParentRecord = pRecord->GetParentRecord();
		if(NULL == pParentRecord)
		{
			m_btnRemove.EnableWindow(FALSE);
			m_btnAdd.EnableWindow(TRUE);
		}
		else
		{
			m_btnRemove.EnableWindow(TRUE);
			CXTPReportRecordItem* pParentItem = pParentRecord->GetItem(0);
			if(pParentItem->GetItemData() == NULL)
			{
				m_btnAdd.EnableWindow(TRUE);
			}
			else
			{
				m_btnAdd.EnableWindow(FALSE);
			}
		}
	}
}


// KMDecisionTreePage  메시지 처리기입니다.
BOOL KMDecisionTreePage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initReportControl();
	initModeList();
	updateReportRecords();
	updateModeCombobox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL KMDecisionTreePage::OnApply()
{
	if( FALSE == InvalidateInputInfo() )
	{
		return FALSE;
	}

	CXTPPropertyPage::OnApply();
	return TRUE;
}


BOOL KMDecisionTreePage::InvalidateInputInfo()
{
	int nModeNodeCount = m_pModeTree->ModeNodeCount();

	if (nModeNodeCount < 1)
	{
		if(m_pParentSheet != NULL)
		{
			m_pParentSheet->SetActivePage(this);
		}
		CString strInputDataCheckMsg = _T("");
		strInputDataCheckMsg.Format( _T("%s에 대한 Decision Tree 관련 정보가 존재하지 않습니다."), this->GetCaption() );
		
		AfxMessageBox(strInputDataCheckMsg); 

		return FALSE;
	}

	return TRUE;
}


void KMDecisionTreePage::OnBnClickedButton1()
{
	int nSel = m_cmbMode.GetCurSel();
	if(CB_ERR == nSel)
	{
		return;
	}

	if(KEMModeChoiceNotNestedLogit == m_emMModel)
	{
		addNotNestedLogit();
	}
	else
	{
		addNestedLogit();
	}
	updateReportRecords();
	updateModeCombobox();
}


void KMDecisionTreePage::OnBnClickedButton2()
{
	if(KEMModeChoiceNotNestedLogit == m_emMModel)
	{
		removeNotNestedLogit();
	}
	else
	{
		removeNestedLogit();
	}
	updateReportRecords();
	updateModeCombobox();
}


//void KMDecisionTreePage::OnBnClickedButton3()
//{
//	// todo : 수단 추가 기능 구현
//	AfxMessageBox(_T("수단 추가 기능은 추후 구현 예정입니다."));
//}


void KMDecisionTreePage::OnReportSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	updateButtonState();
}
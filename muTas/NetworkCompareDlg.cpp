// NetworkCompareDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NetworkCompareDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Target.h"
#include "Scenario.h"
#include "ImChampFrameWindow.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseTurn.h"
#include "NetworkShapeInsert.h"
#include "KExRecordItem.h"
// KNetworkCompareDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KNetworkCompareDlg, KResizeDialogEx)

KNetworkCompareDlg::KNetworkCompareDlg( CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KNetworkCompareDlg::IDD, pParent), m_pTableA(nullptr), m_pTableB(nullptr), m_bCompared(false)
{
}

KNetworkCompareDlg::~KNetworkCompareDlg()
{
    try
    {
    	if (m_pTableA)
        {
            m_pTableA->Unregister(this);
        }
        if (m_pTableB)
        {
            m_pTableB->Unregister(this);
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

void KNetworkCompareDlg::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReport);
    DDX_Control(pDX, IDC_COMBO_SCENARIO_A, m_cboScenarioA);
    DDX_Control(pDX, IDC_COMBO_TARGET_A, m_cboTargetA);
    DDX_Control(pDX, IDC_COMBO_SCENARIO_B, m_cboScenarioB);
    DDX_Control(pDX, IDC_COMBO_TARGET_B, m_cboTargetB);
    DDX_Control(pDX, IDC_CBO_TYPE, m_cboType);
}


BEGIN_MESSAGE_MAP(KNetworkCompareDlg, KResizeDialogEx)
    ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KNetworkCompareDlg::OnReportItemClick)
    ON_NOTIFY (XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KNetworkCompareDlg::OnItemButtonClick)
    ON_BN_CLICKED(IDCANCEL, &KNetworkCompareDlg::OnBnClickedCancel)
    ON_CBN_SELCHANGE(IDC_COMBO_SCENARIO_A, &KNetworkCompareDlg::OnCbnSelchangeComboScenarioA)
    ON_CBN_SELCHANGE(IDC_COMBO_SCENARIO_B, &KNetworkCompareDlg::OnCbnSelchangeComboScenarioB)
    ON_BN_CLICKED(IDC_BUTTON_COMPARE, &KNetworkCompareDlg::OnBnClickedButtonCompare)
    ON_BN_CLICKED(IDC_RADIO_NODE, &KNetworkCompareDlg::OnBnClickedRadioNode)
    ON_BN_CLICKED(IDC_RADIO_LINK, &KNetworkCompareDlg::OnBnClickedRadioLink)
    ON_BN_CLICKED(IDC_RADIO_TURN, &KNetworkCompareDlg::OnBnClickedRadioTurn)
    ON_CBN_SELCHANGE(IDC_COMBO_TARGET_A, &KNetworkCompareDlg::OnCbnSelchangeComboTargetA)
    ON_CBN_SELCHANGE(IDC_COMBO_TARGET_B, &KNetworkCompareDlg::OnCbnSelchangeComboTargetB)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_A, &KNetworkCompareDlg::OnBnClickedButtonSaveA)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_B, &KNetworkCompareDlg::OnBnClickedButtonSaveB)
    ON_BN_CLICKED(IDC_CHECK1, &KNetworkCompareDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &KNetworkCompareDlg::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_CHK_FILTER, &KNetworkCompareDlg::OnBnClickedChkFilter)
    ON_CBN_SELCHANGE(IDC_CBO_TYPE, &KNetworkCompareDlg::OnCbnSelchangeCboType)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KNetworkCompareDlg::SetRadioStatus)
END_MESSAGE_MAP()


// KNetworkCompareDlg 메시지 처리기입니다.
BOOL KNetworkCompareDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
    
    m_pProject = ImChampFrameWindow::GetProject();
    InitialControl();    
    LoadScenarioCombo();
    LoadTargetComboA();
    LoadTargetComboB();

    GetDlgItem(IDC_CHK_FILTER)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);

    GetDlgItem(IDC_BUTTON_SAVE_A)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE_B)->EnableWindow(FALSE);

    GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
    GetDlgItem(IDC_CBO_TYPE)->EnableWindow(FALSE);

    InitCboType();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KNetworkCompareDlg::InitialControl()
{
    try
    {
        CheckRadioButton(IDC_RADIO_NODE, IDC_RADIO_TURN, IDC_RADIO_NODE);
        CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

        KReportCtrlSetting::Default(m_wndReport, FALSE, TRUE);
        KReportCtrlSetting::SelectionEnableFalse(m_wndReport);

        CXTPReportColumn* pColumn = NULL;
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("ID"), 70));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Topology(A-B)"), 70));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("네트워크 구조"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Attribute(A-B)"), 70));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("속성"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(3, _T("Copy(B→A)"), 70, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("복사(B→A)"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(4, _T("Copy(A→B)"), 70, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("복사(A→B)"));
		}

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(5, _T("(A)"), 30, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(6, _T("(A)"), 30, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(7, _T("(B)"), 30, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);
        
        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(8, _T("(B)"), 30, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(FALSE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(9, _T("타입(A)"), 60, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);

        pColumn = m_wndReport.AddColumn(new CXTPReportColumn(10, _T("타입(B)"), 60, FALSE));
        pColumn->SetHeaderAlignment(DT_CENTER);
        pColumn->SetSortable(TRUE);

        m_wndReport.Populate();             
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KNetworkCompareDlg::OnBnClickedCancel()
{
    try
    {
        if (m_spPropertiesDlgA)
        {
            m_spPropertiesDlgA.reset();
        }
        if (m_spPropertiesDlgB)
        {
            m_spPropertiesDlgB.reset();
        }

        Clear();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
    
    KResizeDialogEx::OnCancel();
}

void KNetworkCompareDlg::LoadScenarioCombo()
{
    try
    {
        m_cboScenarioA.ResetContent();
        m_cboScenarioB.ResetContent();

        int nIndex(-1);
        KScenario* pBaseScenario = m_pProject->GetBaseScenario();
        if (pBaseScenario != nullptr)
        {
            nIndex = m_cboScenarioA.AddString(pBaseScenario->GetName());
            m_cboScenarioA.SetItemData(nIndex, (DWORD_PTR)pBaseScenario);

            nIndex = m_cboScenarioB.AddString(pBaseScenario->GetName());
            m_cboScenarioB.SetItemData(nIndex, (DWORD_PTR)pBaseScenario);
        }
        const KTDScenarioMap& oScenarioMap  = m_pProject->GetScenarioMap();
        KTDScenarioMap::const_iterator iter = oScenarioMap.begin();
        KTDScenarioMap::const_iterator end  = oScenarioMap.end();

        while( iter != end )
        {
            KScenario* pScenario = iter->second;
            if (pScenario != nullptr)
            {
                nIndex = m_cboScenarioA.AddString(pScenario->GetName());
                m_cboScenarioA.SetItemData(nIndex, (DWORD_PTR)pScenario);

                nIndex = m_cboScenarioB.AddString(pScenario->GetName());
                m_cboScenarioB.SetItemData(nIndex, (DWORD_PTR)pScenario);
            }
            ++iter;
        }

        if (nIndex >= 0)
        {
            m_cboScenarioA.SetCurSel(0);
            m_cboScenarioB.SetCurSel(nIndex);
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

void KNetworkCompareDlg::LoadTargetComboA()
{
    try
    {
        m_cboTargetA.ResetContent();

        int nCurSelect = m_cboScenarioA.GetCurSel();
        if (nCurSelect < 0)
            return;

        KScenario* pScenario = (KScenario*)m_cboScenarioA.GetItemData(nCurSelect);
        if (pScenario == nullptr)
        {
            TxLogDebugException();
            return;
        }

        int nIndex(-1);

        const KTDTargetMap          &mapTarget    = pScenario->GetTargetMap();
        KTDTargetMap::const_iterator citTargetMap = mapTarget.begin();
        while( mapTarget.end() != citTargetMap )
        {
            KTarget* pTarget = citTargetMap->second;
            if( nullptr == pTarget )
            {
                continue;
            }            

            nIndex = m_cboTargetA.AddString(pTarget->CaptionTargetYear());
            m_cboTargetA.SetItemData(nIndex, (DWORD_PTR)pTarget);
            ++citTargetMap;
        }

        if (nIndex >= 0)
        {
            m_cboTargetA.SetCurSel(0);
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

void KNetworkCompareDlg::LoadTargetComboB()
{
    try
    {
        m_cboTargetB.ResetContent();

        int nCurSelect = m_cboScenarioB.GetCurSel();
        if (nCurSelect < 0)
            return;

        KScenario* pScenario = (KScenario*)m_cboScenarioB.GetItemData(nCurSelect);
        if (pScenario == nullptr)
        {
            TxLogDebugException();
            return;
        }

        int nIndex(-1);

        const KTDTargetMap          &mapTarget    = pScenario->GetTargetMap();
        KTDTargetMap::const_iterator citTargetMap = mapTarget.begin();
        while( mapTarget.end() != citTargetMap )
        {
            KTarget* pTarget = citTargetMap->second;
            if( nullptr == pTarget )
            {
                continue;
            }            

            if (pTarget->IsEmptyData() == false)
            {
                nIndex = m_cboTargetB.AddString(pTarget->CaptionTargetYear());
                m_cboTargetB.SetItemData(nIndex, (DWORD_PTR)pTarget);
            }
            
            ++citTargetMap;
        }

        if (nIndex >= 0)
        {
            m_cboTargetB.SetCurSel(0);
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


void KNetworkCompareDlg::OnCbnSelchangeComboScenarioA()
{
    Clear();
    LoadTargetComboA();
}

void KNetworkCompareDlg::OnCbnSelchangeComboScenarioB()
{
    Clear();
    LoadTargetComboB();
}

void KNetworkCompareDlg::OnCbnSelchangeComboTargetA()
{
    Clear();
}

void KNetworkCompareDlg::OnCbnSelchangeComboTargetB()
{
    Clear();
}

void KNetworkCompareDlg::OnBnClickedButtonCompare()
{
    try
    {
    	int nCurSelect = m_cboTargetA.GetCurSel();
        if (nCurSelect < 0)
        {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetA.SetFocus();
            return;
        }

        KTarget* pTargetA = (KTarget*)m_cboTargetA.GetItemData(nCurSelect);
        nCurSelect = m_cboTargetB.GetCurSel();
        if (nCurSelect < 0)
        {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetB.SetFocus();
            return;
        }

        KTarget* pTargetB = (KTarget*)m_cboTargetB.GetItemData(nCurSelect);
        if (pTargetA->GetObjectID() == pTargetB->GetObjectID())
        {
            m_cboScenarioB.SetFocus();
            AfxMessageBox(_T("동일한 두 네트워크는 비교할 수 없습니다."));
            return;
        }

        if (m_pTableA)
        {
            m_pTableA->Unregister(this);
        }
        if (m_pTableB)
        {
            m_pTableB->Unregister(this);
        }

        m_setTypeA.clear();
        m_setTypeB.clear();

        Compare();
		SetReportToolTip(pTargetA, pTargetB);
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


void KNetworkCompareDlg::Compare()
{
    try
    {
        CWaitCursor wc;

        int nCurSelect = m_cboTargetA.GetCurSel();
        if (nCurSelect < 0) {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetA.SetFocus();
            return;
        }

        KTarget* pTargetA = (KTarget*)m_cboTargetA.GetItemData(nCurSelect);
        nCurSelect = m_cboTargetB.GetCurSel();
        if (nCurSelect < 0) {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetB.SetFocus();
            return;
        }

        KTarget* pTargetB = (KTarget*)m_cboTargetB.GetItemData(nCurSelect);
        if (pTargetA->GetObjectID() == pTargetB->GetObjectID()) {
            m_cboScenarioB.SetFocus();
            AfxMessageBox(_T("동일한 두 네트워크는 비교할 수 없습니다."));
            return;
        }
        
        int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_NODE, IDC_RADIO_TURN);
        if (nCheckedRadioBtn == IDC_RADIO_NODE) {
            m_pTableA = pTargetA->Tables()->FindTable(TABLE_NODE);
            m_pTableA->Register(this);

            m_pTableB = pTargetB->Tables()->FindTable(TABLE_NODE);
            m_pTableB->Register(this);

            CompareNode(pTargetA, pTargetB);
        }
        else if (nCheckedRadioBtn == IDC_RADIO_LINK) {
            m_pTableA = pTargetA->Tables()->FindTable(TABLE_LINK);
            m_pTableA->Register(this);

            m_pTableB = pTargetB->Tables()->FindTable(TABLE_LINK);
            m_pTableB->Register(this);

            CompareLink(pTargetA, pTargetB);
        }

        m_wndReport.GetPaintManager()->m_strNoItems = _T("no difference between  A and B");
        m_bCompared = true;

        if (true) {
            InitCboType(); // IDC_RADIO1, IDC_RADIO2

            BOOL bEnableWindow(FALSE);
            CXTPReportRecords* pRecords = m_wndReport.GetRecords();
            if (pRecords->GetCount() < 1) { // 실행직후의 결과가 존재하는지 여부 확인
                bEnableWindow = FALSE;
            }
            else {
                bEnableWindow = TRUE;
            }

            GetDlgItem(IDC_CHK_FILTER)->EnableWindow(bEnableWindow);
            GetDlgItem(IDC_CHECK1)->EnableWindow(bEnableWindow);
            GetDlgItem(IDC_CHECK2)->EnableWindow(bEnableWindow);

            CheckDlgButton(IDC_CHK_FILTER, BST_UNCHECKED); {
                GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
                GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
                GetDlgItem(IDC_CBO_TYPE)->EnableWindow(FALSE);
            }
            
            CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
            CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
        }

        UpdateFilter();
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


void KNetworkCompareDlg::SetReportToolTip(KTarget* a_pTargetA, KTarget* a_pTargetB)
{
	try
	{
		CXTPReportColumns* pColumns = m_wndReport.GetColumns();
		CXTPReportColumn*  pColumn  = nullptr;

		KScenario* pScenarioA(nullptr);
		int nCur = m_cboScenarioA.GetCurSel();
		pScenarioA = (KScenario*)m_cboScenarioA.GetItemData(nCur);

		KScenario* pScenarioB(nullptr);
		nCur = m_cboScenarioB.GetCurSel();
		pScenarioB = (KScenario*)m_cboScenarioB.GetItemData(nCur);

		CString strToolTip(_T(""));
		if (nullptr != pScenarioA)
		{
			strToolTip.Format(_T("%s-%s"), pScenarioA->GetName(), a_pTargetA->CaptionTargetYear());
		}
		pColumn = pColumns->GetAt(5);
		pColumn->SetTooltip(strToolTip);

		pColumn = pColumns->GetAt(6);
		pColumn->SetTooltip(strToolTip);

		strToolTip = _T("");
		if (nullptr != pScenarioB)
		{
			strToolTip.Format(_T("%s-%s"), pScenarioB->GetName(), a_pTargetB->CaptionTargetYear());
		}
		pColumn = pColumns->GetAt(7);
		pColumn->SetTooltip(strToolTip);

		pColumn = pColumns->GetAt(8);
		pColumn->SetTooltip(strToolTip);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KNetworkCompareDlg::OnBnClickedRadioNode()
{
    Clear();
	CheckBoxState();
    InitCboType();
}


void KNetworkCompareDlg::OnBnClickedRadioLink()
{
    Clear();
	CheckBoxState();
    InitCboType();
}


void KNetworkCompareDlg::OnBnClickedRadioTurn()
{
    Clear();
	CheckBoxState();
}

void KNetworkCompareDlg::CompareNode(KTarget* a_pTargetA, KTarget* a_pTargetB)
{
    try
    {        
    	KBulkDBaseNode::Nodes(a_pTargetA, m_mapNodesA);
        KBulkDBaseNode::Nodes(a_pTargetB, m_mapNodesB);

        // UnionKey (A-Network, B-Network의 노드ID의 합집합)
        std::set<Integer> setKey;
        std::map<Integer, TCompareNodeField>::iterator iterA = m_mapNodesA.begin();
        std::map<Integer, TCompareNodeField>::iterator endA  = m_mapNodesA.end();

        while(iterA != endA)
        {
            setKey.insert(iterA->first);
            ++iterA;
        }
                
        std::map<Integer, TCompareNodeField>::iterator iterB = m_mapNodesB.begin();;
        std::map<Integer, TCompareNodeField>::iterator endB  = m_mapNodesB.end();;
        
        while(iterB != endB)
        {
            setKey.insert(iterB->first);
            ++iterB;
        }

        m_wndReport.ResetContent();

        CXTPReportRecord    * pRecord = NULL;
        CXTPReportRecordItem* pItem   = NULL;

        std::set<Integer>::iterator iterKey = setKey.begin(); 
        std::set<Integer>::iterator endKey  = setKey.end  (); 

        // 키 값을 기준으로 루핑하면서 비교 
        while (iterKey != endKey)
        {
            Integer nxNodeID = *iterKey;
            iterA = m_mapNodesA.find(nxNodeID);
            endA  = m_mapNodesA.end();

            bool bExistA(false);
            if (iterA != endA)
            {
                bExistA = true;
            }
            
            iterB = m_mapNodesB.find(nxNodeID);
            endB  = m_mapNodesB.end();
            
            bool bExistB(false);
            if (iterB != endB)
            {
                bExistB = true;
            }

            if (bExistA == false && bExistB == false)
            {// 이 경우는 없음. 
                ++iterKey;
                continue;
            }

            CString strKey(_T(""));
            strKey.Format(_T("%I64d"), nxNodeID);

            CXTPReportRecordItemControl* pButton = nullptr;
            if (bExistA == true && bExistB == true)
            {//둘다 존재한다. 이 경우는 Geometry 정보가 틀린경우, 속성 정보가 틀린경우
                TCompareNodeField& oNodeA = iterA->second;
                TCompareNodeField& oNodeB = iterB->second;

                bool bTopologyEqual  = oNodeA.CompareGeometry(oNodeB);
                bool bAttributeEqual = oNodeA.CompareAttribute(oNodeB);

                if (bTopologyEqual == true && bAttributeEqual == true)
                {
                    ++iterKey;
                    continue;
                }

                pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNodeID, _T("%.0f")));
                pItem->SetItemData((DWORD_PTR)nxNodeID);
                pItem->SetAlignment(DT_CENTER);
                
                if (bTopologyEqual == true)
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A == B")));
                    pItem->SetItemData(1);
                }
                else
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A != B")));
                    pItem->SetBackgroundColor(RGB(153, 204, 255));
                    pItem->SetItemData(2);
                }
                    pItem->SetAlignment(DT_CENTER);

                if (bAttributeEqual == true)
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A == B")));
                    pItem->SetItemData(1);
                }
                else
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A != B")));
                    pItem->SetItemData(2);
                    pItem->SetBackgroundColor(RGB(153, 204, 255));
                }                    
                    pItem->SetAlignment(DT_CENTER);

                if (bTopologyEqual)
                {
                    pItem   = pRecord->AddItem(new KExRecordItemCheck());
                    pItem->SetItemData((DWORD_PTR)&oNodeB);
                    pItem->HasCheckbox(TRUE);
                    pItem->SetAlignment(xtpColumnIconCenter);

                    pItem   = pRecord->AddItem(new KExRecordItemCheck());
                    pItem->SetItemData((DWORD_PTR)&oNodeA);
                    pItem->HasCheckbox(TRUE);
                    pItem->SetAlignment(xtpColumnIconCenter);   
                }
                else
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("x")));
                    pItem->SetAlignment(DT_CENTER);

                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("x")));
                    pItem->SetAlignment(DT_CENTER);
                }                   

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());  
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());    
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());      
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());    
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);
                                
                int nTypeA = oNodeA.TnNodeType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeA, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeA);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeA.insert(nTypeA);
                }
                
                int nTypeB = oNodeB.TnNodeType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeB, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeB);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeB.insert(nTypeB);
                }
            }
            else if (bExistA == true && bExistB == false)
            {
                TCompareNodeField& oNodeA = iterA->second;

                pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
                //pItem = pRecord->AddItem(new CXTPReportRecordItemText(strKey));
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNodeID, _T("%.0f")));
                pItem->SetItemData((DWORD_PTR)nxNodeID);
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(o)-B(x)")));
                pItem->SetItemData(3);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(o)-B(x)")));
                pItem->SetItemData(3);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetAlignment(DT_CENTER);

                pItem   = pRecord->AddItem(new KExRecordItemCheck());
                pItem->HasCheckbox(TRUE);
                pItem->SetAlignment(xtpColumnIconCenter);   

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());     
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());   
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pItem->SetAlignment(DT_CENTER);
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pItem->SetAlignment(DT_CENTER);

                int nTypeA = oNodeA.TnNodeType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeA, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeA);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeA.insert(nTypeA);
                }

                int nTypeB = -1; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItem());
                    pItem->SetItemData((DWORD_PTR)nTypeB);
                    pItem->SetAlignment(DT_CENTER);
                }
            }
            else /*bExistA == false && bExistB == true*/
            {
                TCompareNodeField& oNodeB = iterB->second;

                pRecord = m_wndReport.AddRecord(new CXTPReportRecord);                
                //pItem = pRecord->AddItem(new CXTPReportRecordItemText(strKey));
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNodeID, _T("%.0f")));
                pItem->SetItemData((DWORD_PTR)nxNodeID);
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(x)-B(o)")));
                pItem->SetItemData(4);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(x)-B(o)")));
                pItem->SetItemData(4);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem   = pRecord->AddItem(new KExRecordItemCheck());
                pItem->HasCheckbox(TRUE);
                pItem->SetAlignment(xtpColumnIconCenter); 

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetAlignment(DT_CENTER);
                
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pItem->SetAlignment(DT_CENTER);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());            
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());      
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);     

                int nTypeA = -1; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItem());
                    pItem->SetItemData((DWORD_PTR)nTypeA);
                    pItem->SetAlignment(DT_CENTER);
                }

                int nTypeB = oNodeB.TnNodeType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeB, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeB);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeB.insert(nTypeB);
                }
            }

            ++iterKey;
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

    m_wndReport.Populate();
}

void KNetworkCompareDlg::CompareLink(KTarget* a_pTargetA, KTarget* a_pTargetB)
{
    try
    {        
        KBulkDBaseLink::Links(a_pTargetA, m_mapLinksA);
        KBulkDBaseLink::Links(a_pTargetB, m_mapLinksB);

        //UnionKey
        std::set<Integer> setKey;
        std::map<Integer, TCompareLinkField>::iterator iterA = m_mapLinksA.begin();
        std::map<Integer, TCompareLinkField>::iterator endA  = m_mapLinksA.end();

        while(iterA != endA)
        {
            setKey.insert(iterA->first);
            ++iterA;
        }

        std::map<Integer, TCompareLinkField>::iterator iterB = m_mapLinksB.begin();;
        std::map<Integer, TCompareLinkField>::iterator endB  = m_mapLinksB.end();;

        while(iterB != endB)
        {
            setKey.insert(iterB->first);
            ++iterB;
        }

        m_wndReport.ResetContent();

        CXTPReportRecord    * pRecord = NULL;
        CXTPReportRecordItem* pItem   = NULL;

        std::set<Integer>::iterator iterKey = setKey.begin(); 
        std::set<Integer>::iterator endKey  = setKey.end  (); 

        while (iterKey != endKey)
        {
            Integer nxNodeID = *iterKey;
            iterA = m_mapLinksA.find(nxNodeID);
            endA  = m_mapLinksA.end();

            bool bExistA(false);
            if (iterA != endA)
            {
                bExistA = true;
            }

            iterB = m_mapLinksB.find(nxNodeID);
            endB  = m_mapLinksB.end();

            bool bExistB(false);
            if (iterB != endB)
            {
                bExistB = true;
            }

            if (bExistA == false && bExistB == false)
            {// 이 경우는 없음. 
                ++iterKey;
                continue;
            }

            CString strKey(_T(""));
            strKey.Format(_T("%I64d"), nxNodeID);

            CXTPReportRecordItemControl* pButton = nullptr;
            if (bExistA == true && bExistB == true)
            {
                TCompareLinkField& oLinkA = iterA->second;
                TCompareLinkField& oLinkB = iterB->second;

                bool bTopologyEqual  = oLinkA.CompareGeometry(oLinkB);
                bool bAttributeEqual = oLinkA.CompareAttribute(oLinkB);

                if (bTopologyEqual == true && bAttributeEqual == true)
                {
                    ++iterKey;
                    continue;
                }

                pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
                //pItem = pRecord->AddItem(new CXTPReportRecordItemText(strKey));
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNodeID, _T("%.0f")));
                pItem->SetItemData((DWORD_PTR)nxNodeID);
                pItem->SetAlignment(DT_CENTER);

                if (bTopologyEqual == true)
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A == B")));
                    pItem->SetItemData(1);
                }
                else
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A != B")));
                    pItem->SetBackgroundColor(RGB(153, 204, 255));
                    pItem->SetItemData(2);
                }
                    pItem->SetAlignment(DT_CENTER);

                if (bAttributeEqual == true)
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A == B")));
                    pItem->SetItemData(1);
                }
                else
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A != B")));
                    pItem->SetItemData(2);
                    pItem->SetBackgroundColor(RGB(153, 204, 255));
                }                    
                pItem->SetAlignment(DT_CENTER);

                if (bTopologyEqual)
                {
                    pItem   = pRecord->AddItem(new KExRecordItemCheck());
                    pItem->SetItemData((DWORD_PTR)&oLinkB);
                    pItem->HasCheckbox(TRUE);
                    pItem->SetAlignment(xtpColumnIconCenter);

                    pItem   = pRecord->AddItem(new KExRecordItemCheck());
                    pItem->SetItemData((DWORD_PTR)&oLinkA);
                    pItem->HasCheckbox(TRUE);
                    pItem->SetAlignment(xtpColumnIconCenter);   
                }
                else
                {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("x")));
                    pItem->SetAlignment(DT_CENTER);

                    pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("x")));
                    pItem->SetAlignment(DT_CENTER);
                }                   

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());  
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());    
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());      
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());    
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                int nTypeA = oLinkA.TnLinkType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeA, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeA);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeA.insert(nTypeA);
                }

                int nTypeB = oLinkB.TnLinkType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeB, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeB);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeB.insert(nTypeB);
                }
            }
            else if (bExistA == true && bExistB == false)
            {
                TCompareLinkField& oLinkA = iterA->second;

                pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
                //pItem = pRecord->AddItem(new CXTPReportRecordItemText(strKey));
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNodeID, _T("%.0f")));
                pItem->SetItemData((DWORD_PTR)nxNodeID);
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(o)-B(x)")));
                pItem->SetItemData(3);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(o)-B(x)")));
                pItem->SetItemData(3);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetAlignment(DT_CENTER);

                pItem   = pRecord->AddItem(new KExRecordItemCheck());
                pItem->HasCheckbox(TRUE);
                pItem->SetAlignment(xtpColumnIconCenter);   

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());     
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());   
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pItem->SetAlignment(DT_CENTER);
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pItem->SetAlignment(DT_CENTER);

                int nTypeA = oLinkA.TnLinkType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeA, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeA);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeA.insert(nTypeA);
                }

                int nTypeB = -1; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItem());
                    pItem->SetItemData((DWORD_PTR)nTypeB);
                    pItem->SetAlignment(DT_CENTER);
                }
            }
            else /*bExistA == false && bExistB == true*/
            {
                TCompareLinkField& oLinkB = iterB->second;

                pRecord = m_wndReport.AddRecord(new CXTPReportRecord);                
                //pItem = pRecord->AddItem(new CXTPReportRecordItemText(strKey));
                pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNodeID, _T("%.0f")));
                pItem->SetItemData((DWORD_PTR)nxNodeID);
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(x)-B(o)")));
                pItem->SetItemData(4);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("A(x)-B(o)")));
                pItem->SetItemData(4);
                pItem->SetAlignment(DT_CENTER);
                pItem->SetBackgroundColor(RGB(153, 204, 255));

                pItem   = pRecord->AddItem(new KExRecordItemCheck());
                pItem->HasCheckbox(TRUE);
                pItem->SetAlignment(xtpColumnIconCenter); 

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pItem->SetAlignment(DT_CENTER);

                pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
                pItem->SetItemData((DWORD_PTR)a_pTargetA);
                pItem->SetAlignment(DT_CENTER);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());            
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("map"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("지도"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);

                pItem   = pRecord->AddItem(new CXTPReportRecordItem());      
                pItem->SetItemData((DWORD_PTR)a_pTargetB);
                pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Att"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("속성"));
				}
                pButton->SetSize(CSize(30, 0));
                pButton->SetThemed(TRUE);           

                int nTypeA = -1; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItem());
                    pItem->SetItemData((DWORD_PTR)nTypeA);
                    pItem->SetAlignment(DT_CENTER);
                }

                int nTypeB = oLinkB.TnLinkType; {
                    pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nTypeB, _T("%.0f")));
                    pItem->SetItemData((DWORD_PTR)nTypeB);
                    pItem->SetAlignment(DT_CENTER);

                    m_setTypeB.insert(nTypeB);
                }
            }

            ++iterKey;
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

    m_wndReport.Populate();
}

void KNetworkCompareDlg::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    try
    {
        XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

        if (!pClickedItem->pRow || !pClickedItem->pColumn)
            return;

        int nRowIndex    = pClickedItem->pRow->GetIndex();
        int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

        CXTPReportRecord*     pRecord = pClickedItem->pRow->GetRecord();

        if( 3 == nColumnIndex )
        {
            CXTPReportRecordItem* pItemA = pRecord->GetItem(3);
            CXTPReportRecordItem* pItemB = pRecord->GetItem(4);
            if (pItemA->GetHasCheckbox() == TRUE)
            {
                if (pItemB->GetHasCheckbox() == TRUE)
                {
                    pItemB->SetChecked(FALSE);
                }
            }
        }

        if( 4 == nColumnIndex )
        {
            CXTPReportRecordItem* pItemA = pRecord->GetItem(3);
            CXTPReportRecordItem* pItemB = pRecord->GetItem(4);
            if (pItemB->GetHasCheckbox() == TRUE)
            {
                if (pItemA->GetHasCheckbox() == TRUE)
                {
                    pItemA->SetChecked(FALSE);
                }
            }
        }
    }
    catch(...)
    {
        TxLogDebugException();
    } 

	CheckBoxState();
}

void KNetworkCompareDlg::OnItemButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    try
    {
        XTP_NM_REPORTITEMCONTROL* pClickedItem = (XTP_NM_REPORTITEMCONTROL*) pNMHDR;
        if(!(pClickedItem->pRow && pClickedItem->pItem && pClickedItem->pItemControl))
            return;

        int nRowIndex    = pClickedItem->pRow->GetIndex();
        int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

        CXTPReportRecord*     pRecord = pClickedItem->pRow->GetRecord();
        CXTPReportRecordItem* pItemID = pRecord->GetItem(0);
                
        int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_NODE, IDC_RADIO_TURN);
        Integer nxID = (Integer)pItemID->GetItemData();

        /*A-Map*/
        if( 5 == nColumnIndex )
        {
            CXTPReportRecordItem* pItem = pRecord->GetItem(nColumnIndex);
            KTarget*  pTarget   = (KTarget*)pItem->GetItemData();
            KMapView* pMapView  = ImChampFrameWindow::GetShowMapView(pTarget);
            if (nCheckedRadioBtn == IDC_RADIO_NODE) {
                pMapView->PanToFeature(KLayerID::Node(), nxID);
            }
            else if (nCheckedRadioBtn == IDC_RADIO_LINK) {
                pMapView->PanToFeature(KLayerID::Link(), nxID);
            }
            else
            {
                __int64 nxNodeID = KBulkDBaseTurn::FindIntersectionNode(pTarget, nxID);
                pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
            }
        }
        /*A-Attribute*/
        if( 6 == nColumnIndex )
        {
            CXTPReportRecordItem* pItem = pRecord->GetItem(nColumnIndex);
            KTarget*  pTarget   = (KTarget*)pItem->GetItemData();
            if (m_spPropertiesDlgA != NULL)
            {			
                m_spPropertiesDlgA->ShowWindow(SW_SHOW);		
            }
            else
            {
                m_spPropertiesDlgA = KPropertiesDlgPtr(new KPropertiesDlg(this));
                m_spPropertiesDlgA->Create(IDD_2200_PROPERTIES);
                m_spPropertiesDlgA->ShowWindow(SW_SHOW);
            }		
            CString strWhere(_T(""));
            if (nCheckedRadioBtn == IDC_RADIO_NODE)
            {
                strWhere.Format(_T(" %s = '%I64d' "), COLUMN_NODE_ID, nxID);
                m_spPropertiesDlgA->ShowInformation(pTarget, TABLE_NODE, strWhere, TRUE);
            }
            else if (nCheckedRadioBtn == IDC_RADIO_LINK)
            {
                strWhere.Format(_T(" %s = '%I64d' "), COLUMN_LINK_ID, nxID);
                m_spPropertiesDlgA->ShowInformation(pTarget, TABLE_LINK, strWhere, TRUE);
            }
            else
            {
                strWhere.Format(_T(" %s = '%I64d' "), COLUMN_TURN_ID, nxID);
                m_spPropertiesDlgA->ShowInformation(pTarget, TABLE_TURN, strWhere, TRUE);
            }            
        }
        /*B-Map*/
        if( 7 == nColumnIndex )
        {
            CXTPReportRecordItem* pItem = pRecord->GetItem(nColumnIndex);
            KTarget*  pTarget   = (KTarget*)pItem->GetItemData();
            KMapView* pMapView  = ImChampFrameWindow::GetShowMapView(pTarget);
            if (nCheckedRadioBtn == IDC_RADIO_NODE) {
                pMapView->PanToFeature(KLayerID::Node(), nxID);
            }
            else if (nCheckedRadioBtn == IDC_RADIO_LINK) {
                pMapView->PanToFeature(KLayerID::Link(), nxID);
            }
            else {
                __int64 nxNodeID = KBulkDBaseTurn::FindIntersectionNode(pTarget, nxID);
                pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
            }
        }
        /*B-Attribute*/
        if( 8 == nColumnIndex )
        {
            CXTPReportRecordItem* pItem = pRecord->GetItem(nColumnIndex);
            KTarget*  pTarget   = (KTarget*)pItem->GetItemData();
            if (m_spPropertiesDlgB != NULL)
            {			
                m_spPropertiesDlgB->ShowWindow(SW_SHOW);		
            }
            else
            {
                m_spPropertiesDlgB = KPropertiesDlgPtr(new KPropertiesDlg(this));
                m_spPropertiesDlgB->Create(IDD_2200_PROPERTIES);
                m_spPropertiesDlgB->ShowWindow(SW_SHOW);
            }		
            CString strWhere(_T(""));
            if (nCheckedRadioBtn == IDC_RADIO_NODE)
            {
                strWhere.Format(_T(" %s = '%I64d' "), COLUMN_NODE_ID, nxID);
                m_spPropertiesDlgB->ShowInformation(pTarget, TABLE_NODE, strWhere, TRUE);
            }
            else if (nCheckedRadioBtn == IDC_RADIO_LINK)
            {
                strWhere.Format(_T(" %s = '%I64d' "), COLUMN_LINK_ID, nxID);
                m_spPropertiesDlgB->ShowInformation(pTarget, TABLE_LINK, strWhere, TRUE);
            }
            else
            {
                strWhere.Format(_T(" %s = '%I64d' "), COLUMN_TURN_ID, nxID);
                m_spPropertiesDlgB->ShowInformation(pTarget, TABLE_TURN, strWhere, TRUE);
            }   
        }
    }
    catch (...) {
        TxLogDebugException();		
    }
}

void KNetworkCompareDlg::NotifyProcess( LPCTSTR a_strSubjectName/*=_T("")*/, Integer a_nxObjectID/*=0*/ )
{
    if (m_bCompared)
        Compare();
}

void KNetworkCompareDlg::Clear()
{
    try
    {
        m_bCompared = false;
        m_wndReport.GetPaintManager()->m_strNoItems = _T("");
        m_wndReport.ResetContent();
        m_wndReport.Populate(); 

        m_mapNodesA.clear();
        m_mapNodesB.clear();

        m_mapLinksA.clear();
        m_mapLinksB.clear();

		m_mapFNodesA.clear();
		m_mapFNodesB.clear();

		m_mapFLinksA.clear();
		m_mapFLinksB.clear();

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

void KNetworkCompareDlg::OnBnClickedButtonSaveA()
{
    try
    {
        CWaitCursor wc;

        int nCurSelect = m_cboTargetA.GetCurSel();
        if (nCurSelect < 0)
        {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetA.SetFocus();
            return;
        }

        KTarget* pTargetA = (KTarget*)m_cboTargetA.GetItemData(nCurSelect);
        nCurSelect = m_cboTargetB.GetCurSel();
        if (nCurSelect < 0)
        {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetB.SetFocus();
            return;
        }

        KTarget* pTargetB = (KTarget*)m_cboTargetB.GetItemData(nCurSelect);
        if (pTargetA->GetObjectID() == pTargetB->GetObjectID())
        {
            m_cboScenarioB.SetFocus();
            AfxMessageBox(_T("동일한 두 네트워크는 비교할 수 없습니다."));
            return;
        }

        int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_NODE, IDC_RADIO_TURN);
        if (nCheckedRadioBtn == IDC_RADIO_NODE)
        {
			SavaNodeB2A();
        }
        else if (nCheckedRadioBtn == IDC_RADIO_LINK)
        {
            SavaLinkA();
        }

		CheckBoxState();
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

void KNetworkCompareDlg::SavaNodeA2B()
{
    try
    {
        KMapView* pMapView = nullptr;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
						   
		QBicDonkeyProgress oDonkeyProgress(this, QBicDonkeyProgress::DEFAULT_TITLE, TRUE, FALSE, FALSE, nLang);

        CXTPReportRecords* pRecords = m_wndReport.GetRecords();
        int nChanedCount(0);
        int nZoneCnt(0);

        int nCount = pRecords->GetCount();
        for (int i=0; i<nCount; i++)
        {
            CXTPReportRecord*     pRecord = pRecords->GetAt(i);              
            CXTPReportRecordItem* pItem   = pRecord->GetItem(4);

            //★ 보이고 있는 것만
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

            if (pItem->GetHasCheckbox() == FALSE) {
                continue;                  
            }

            if (pItem->IsChecked() == FALSE) {
                continue;
            }

            //A의 것을 B에 복사 행위를 수행
            CXTPReportRecordItem* pItemID      = pRecord->GetItem(0);
            CXTPReportRecordItem* pItemTopo    = pRecord->GetItem(1);
            CXTPReportRecordItem* pItemAtt     = pRecord->GetItem(2);
            CXTPReportRecordItem* pItemTargetA = pRecord->GetItem(5);
            CXTPReportRecordItem* pItemTargetB = pRecord->GetItem(7);            

            Integer nxNodeID  = (Integer)pItemID->GetItemData();
            int     nTopoOper = (int)pItemTopo->GetItemData();
            int     nAttOper  = (int)pItemAtt->GetItemData();

            KTarget*  pTargetA  = (KTarget*)pItemTargetA->GetItemData();
            KMapView* pMapViewA = ImChampFrameWindow::GetShowMapView(pTargetA);
            KTarget*  pTargetB  = (KTarget*)pItemTargetB->GetItemData();
            KMapView* pMapViewB = ImChampFrameWindow::GetShowMapView(pTargetB); {
                pMapView = pMapViewB;
            }            

            std::map<Integer, TCompareNodeField>::iterator iterA = m_mapNodesA.find(nxNodeID);
            std::map<Integer, TCompareNodeField>::iterator endA  = m_mapNodesA.end();

            std::map<Integer, TCompareNodeField>::iterator iterB = m_mapNodesB.find(nxNodeID);
            std::map<Integer, TCompareNodeField>::iterator endB  = m_mapNodesB.end();

            KDBaseConPtr spDBaseConnection = pTargetB->GetDBaseConnection();

            if (nTopoOper == 1 && nAttOper == 2)
            {
                //A의 속성 정보를 B에 Update
                if (iterA == endA)
                    continue;
                if (iterB == endB)
                    continue;

                TCompareNodeField& oNodeA = iterA->second;
                TCompareNodeField& oNodeB = iterB->second;

                CString strSQL = oNodeA.GetUpdateSQL();

                if (strSQL.GetLength() > 0) {
                    CString strMsg;
                    strMsg.Format(_T("Node: %I64d Update"), nxNodeID);
                    oDonkeyProgress.SendTextEvent(strMsg);

                    //★ 속성 정보 업데이트
                    spDBaseConnection->ExecuteUpdate(strSQL);

                    if (oNodeA.TnNodeType != oNodeB.TnNodeType)
                    {
                        // 동일 타입은 올수 없다. 즉, 둘다 true일 수 없다.
                        bool isAnZone(false);  // A는 존 노드  -> B는 일반노드인경우
                        bool isBnZone(false);  // A는 일반노드 -> B는 존인경우

                        ITxGeometryPtr spZoneGeometry;
                        if (oNodeA.TnNodeType == 0) {
                            isAnZone = true;
                            spZoneGeometry = FindZoneGeometry(pMapViewA, nxNodeID);
                        }

                        if (oNodeB.TnNodeType == 0)
                            isBnZone = true;

                        if (isAnZone && isBnZone == false) {
                            TTempZoneInfo oTempZoneInfo;
                            FindZoneInfo  (pTargetA, nxNodeID, oTempZoneInfo);
                            InsertZone(pMapViewB, spZoneGeometry, oTempZoneInfo);
                        }

                        if (isAnZone == false && isBnZone == true) {
                            DeleteZone(pMapViewB, oNodeB.TnxNodeID);
                        }
                    }

                    nChanedCount++;
                }
            }
            else if (nTopoOper == 3)
            {
                //A의 Geometry, 속성 추가
                if (iterA == endA)
                    continue;

                TCompareNodeField& oNodeA = iterA->second;

                //★ 
                TxRecord oRecord = oNodeA.GetInsertRecord();
                
                ITxGeometryPtr spZoneGeometry;
                bool isZone(false);
                if (oNodeA.TnNodeType == 0) {
                    isZone = true;
                    spZoneGeometry = FindZoneGeometry(pMapViewA, nxNodeID);    
                }
                
                if (true /*Node*/) {
                    TxPointPtr spPoint = TxPointPtr(new TxPoint(oNodeA.TdX, oNodeA.TdY));
                    InsertNode(pMapViewB, nxNodeID, spPoint, oRecord);    
                }
                
                if (isZone) {
                    TTempZoneInfo oTempZoneInfo;
                    FindZoneInfo (pTargetA, nxNodeID, oTempZoneInfo);
                    InsertZone(pMapViewB, spZoneGeometry, oTempZoneInfo);

                    nZoneCnt++;
                }

                nChanedCount++;
            }
        }

        if (nChanedCount > 0)
        {
            try
            {  
                if (pMapView != nullptr) {
                    pMapView->ReloadRender(false, 1);

                    if (nZoneCnt > 0) {
                        pMapView->ReloadRender(false, 3);
                    }

                    pMapView->MapRefresh();
                }

                if (m_pTableB)
                    m_pTableB->Notify();
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

void KNetworkCompareDlg::SavaNodeB2A()
{
    try
    {
        KMapView* pMapView = nullptr;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
						   
		QBicDonkeyProgress oDonkeyProgress(this, QBicDonkeyProgress::DEFAULT_TITLE, TRUE, FALSE, FALSE, nLang);

    	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
        int nChanedCount(0);
        int nZoneCnt(0);

        int nCount = pRecords->GetCount();
        for (int i=0; i<nCount; i++)
        {
            CXTPReportRecord*     pRecord = pRecords->GetAt(i);              
            CXTPReportRecordItem* pItem   = pRecord->GetItem(3);

            //★ 보이고 있는 것만
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

            if (pItem->GetHasCheckbox() == FALSE) {
                continue;                  
            }

            if (pItem->IsChecked() == FALSE) {
                continue;
            }
            
            //B의 것을 A에 복사 행위를 수행
            CXTPReportRecordItem* pItemID      = pRecord->GetItem(0);
            CXTPReportRecordItem* pItemTopo    = pRecord->GetItem(1);
            CXTPReportRecordItem* pItemAtt     = pRecord->GetItem(2);
            CXTPReportRecordItem* pItemTargetA = pRecord->GetItem(5);
            CXTPReportRecordItem* pItemTargetB = pRecord->GetItem(7);            

            Integer nxNodeID  = (Integer)pItemID->GetItemData();
            int     nTopoOper = (int)pItemTopo->GetItemData();
            int     nAttOper  = (int)pItemAtt->GetItemData();

            KTarget*  pTargetA  = (KTarget*)pItemTargetA->GetItemData();
            KMapView* pMapViewA = ImChampFrameWindow::GetShowMapView(pTargetA); {
                pMapView = pMapViewA;
            }

            KTarget*  pTargetB  = (KTarget*)pItemTargetB->GetItemData();
            KMapView* pMapViewB = ImChampFrameWindow::GetShowMapView(pTargetB);

            std::map<Integer, TCompareNodeField>::iterator iterA = m_mapNodesA.find(nxNodeID);
            std::map<Integer, TCompareNodeField>::iterator endA  = m_mapNodesA.end();

            std::map<Integer, TCompareNodeField>::iterator iterB = m_mapNodesB.find(nxNodeID);
            std::map<Integer, TCompareNodeField>::iterator endB  = m_mapNodesB.end();

			KDBaseConPtr spDBaseConnection = pTargetA->GetDBaseConnection();
            #pragma region comment
            /*
                                         Geometry     Attribute
                동일한 ID 데이터가 존재   (1) A == B    (2) A != B
                A에만 존재하는 경우       (3) A(o)-B(x) (3) A(o)-B(x)  : 존노드 처리 필요
                B에만 존재하는 경우       (4) A(x)-B(o) (4) A(x)-B(o)  : 존노드 처리 필요
            */
            #pragma endregion comment
            if (nTopoOper == 1 && nAttOper == 2)
            {
                //B의 속성 정보를 A에 Update
                if (iterA == endA)
                    continue;
                if (iterB == endB)
                    continue;

                TCompareNodeField& oNodeA = iterA->second;
                TCompareNodeField& oNodeB = iterB->second;
                
                CString strSQL = oNodeB.GetUpdateSQL();
                if (strSQL.GetLength() > 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("Node: %I64d Update"), nxNodeID);
                    oDonkeyProgress.SendTextEvent(strMsg);

                    spDBaseConnection->ExecuteUpdate(strSQL);
                    if (oNodeA.TnNodeType != oNodeB.TnNodeType)
                    {
                        // 동일 타입은 올수 없다. 즉, 둘다 true일 수 없다.
                        bool isAnZone(false);  // A는 존 노드  <- B는 일반노드인경우
                        bool isBnZone(false);  // A는 일반노드 <- B는 존인경우

                        ITxGeometryPtr spZoneGeometry;
                        if (oNodeA.TnNodeType == 0) {
                            isAnZone = true;                            
                        }

                        if (oNodeB.TnNodeType == 0) {
                            isBnZone = true;
                            spZoneGeometry = FindZoneGeometry(pMapViewB, nxNodeID);
                        }
                                                
                        if (isAnZone && isBnZone == false) {
                            DeleteZone(pMapViewA, nxNodeID);
                        }

                        if (isAnZone == false && isBnZone == true) {
                            TTempZoneInfo oTempZoneInfo;
                            FindZoneInfo  (pTargetB, nxNodeID, oTempZoneInfo);
                            InsertZone(pMapViewA, spZoneGeometry, oTempZoneInfo);
                        }
                    }

                    nChanedCount++;
                }
            }
            else if (nTopoOper == 4)
            {
                //B의 Geometry, 속성 추가
                if (iterB == endB)
                    continue;

                TCompareNodeField& oNodeB = iterB->second;
                //★ 
                TxRecord oRecord = oNodeB.GetInsertRecord();

                ITxGeometryPtr spZoneGeometry;
                bool isZone(false);
                if (oNodeB.TnNodeType == 0) {
                    isZone = true;
                    spZoneGeometry = FindZoneGeometry(pMapViewB, nxNodeID);    
                }

                if (true /*Node*/) {
                    TxPointPtr spPoint = TxPointPtr(new TxPoint(oNodeB.TdX, oNodeB.TdY));
                    InsertNode(pMapViewA, nxNodeID, spPoint, oRecord);    
                }

                if (isZone) {
                    TTempZoneInfo oTempZoneInfo;
                    FindZoneInfo  (pTargetB, nxNodeID, oTempZoneInfo);
                    InsertZone(pMapViewA, spZoneGeometry, oTempZoneInfo);

                    nZoneCnt++;
                }

                nChanedCount++;                
            }
        }

        if (nChanedCount > 0)
        {
            try
            {
                if (pMapView != nullptr) {
                    pMapView->ReloadRender(false, 1);

                    if (nZoneCnt > 0) {
                        pMapView->ReloadRender(false, 3);
                    }

                    pMapView->MapRefresh();
                }

                if (m_pTableA)
                    m_pTableA->Notify();
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

void KNetworkCompareDlg::SavaLinkA()
{
    try
    {
        KMapView* pMapView = nullptr;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkeyProgress(this, QBicDonkeyProgress::DEFAULT_TITLE, TRUE, FALSE, FALSE, nLang);

        CString strNotExistNode(_T(""));

        CXTPReportRecords* pRecords = m_wndReport.GetRecords();
        int nChanedCount(0);
        int nCount = pRecords->GetCount();
        for (int i=0; i<nCount; i++)
        {
            CXTPReportRecord* pRecord = pRecords->GetAt(i);              
            CXTPReportRecordItem* pItem   = pRecord->GetItem(3);

            //★ 보이고 있는 것만
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

            if (pItem->GetHasCheckbox() == FALSE) {
                continue;                  
            }

            if (pItem->IsChecked() == FALSE) {
                continue;
            }

            //B의 것을 A에 복사 행위를 수행
            CXTPReportRecordItem* pItemID   = pRecord->GetItem(0);
            CXTPReportRecordItem* pItemTopo = pRecord->GetItem(1);
            CXTPReportRecordItem* pItemAtt  = pRecord->GetItem(2);
            CXTPReportRecordItem* pItemMap  = pRecord->GetItem(5);

            Integer nxLinkID  = (Integer)pItemID->GetItemData();
            int     nTopoOper = (int)pItemTopo->GetItemData();
            int     nAttOper  = (int)pItemAtt->GetItemData();

            KTarget*  pTargetA  = (KTarget*)pItemMap->GetItemData();
            KMapView* pMapViewA = ImChampFrameWindow::GetShowMapView(pTargetA); {
                pMapView = pMapViewA;
            }

            CXTPReportRecordItem* pItemMapB  = pRecord->GetItem(7);
            KTarget*  pTargetB  = (KTarget*)pItemMapB->GetItemData();
            KMapView* pMapViewB = ImChampFrameWindow::GetShowMapView(pTargetB);

            std::map<Integer, TCompareLinkField>::iterator iterA = m_mapLinksA.find(nxLinkID);
            std::map<Integer, TCompareLinkField>::iterator endA  = m_mapLinksA.end();

            std::map<Integer, TCompareLinkField>::iterator iterB = m_mapLinksB.find(nxLinkID);
            std::map<Integer, TCompareLinkField>::iterator endB  = m_mapLinksB.end();

			KDBaseConPtr spDBaseConnection = pTargetA->GetDBaseConnection();

            if (nTopoOper == 1 && nAttOper == 2)
            {
                //B의 속성 정보를 A에 Update
                if (iterA == endA)
                    continue;
                if (iterB == endB)
                    continue;

                TCompareLinkField& oLinkA = iterA->second;
                TCompareLinkField& oLinkB = iterB->second;

                CString strSQL = oLinkB.GetUpdateSQL();
                if (strSQL.GetLength() > 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("Link : %I64d Update"), nxLinkID);
                    oDonkeyProgress.SendTextEvent(strMsg);
                                        
                    if (oLinkA.TnLinkType != oLinkB.TnLinkType)
                    {
                        //★ 향후 처리 필요-주제도 업데이트
                    }
                    spDBaseConnection->ExecuteUpdate(strSQL);
                    nChanedCount++;
                }
            }
            else if (nTopoOper == 4)
            {
                //B의 Geometry, 속성 추가
                if (iterB == endB)
                    continue;

                TCompareLinkField& oLinkB = iterB->second;
                TxRecord oRecord = oLinkB.GetInsertRecord();

                if (KBulkDBaseNode::ExistNode(pTargetA, oLinkB.TnxFNodeID) == false) {
                    strNotExistNode.AppendFormat(_T("\n 링크 - 시작노드 : '%I64d'이 존재하지 않습니다."), oLinkB.TnxFNodeID);
                    continue;
                }

                if (KBulkDBaseNode::ExistNode(pTargetA, oLinkB.TnxTNodeID) == false)
                {
                    strNotExistNode.AppendFormat(_T("\n 링크 - 끝노드   : '%I64d'이 존재하지 않습니다."), oLinkB.TnxTNodeID);
                    continue;
                }

                ITxGeometryPtr spLinkGeo = FineLinkGeometry(pMapViewB, nxLinkID);
                if (spLinkGeo != nullptr) {
                    InsertLink(pMapViewA, nxLinkID, spLinkGeo, oRecord);
                    nChanedCount++;
                }
            }
        }

        if (nChanedCount > 0)
        {
            try
            {
                if (pMapView != nullptr) {
                    pMapView->ReloadRender(false, 2);
                    pMapView->MapRefresh();
                }

                if (m_pTableA)
                    m_pTableA->Notify();
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

        oDonkeyProgress.Close();
        if(strNotExistNode.GetLength() > 0)
            AfxMessageBox(strNotExistNode);
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

void KNetworkCompareDlg::OnBnClickedButtonSaveB()
{
    try
    {
        CWaitCursor wc;

        int nCurSelect = m_cboTargetA.GetCurSel();
        if (nCurSelect < 0)
        {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetA.SetFocus();
            return;
        }

        KTarget* pTargetA = (KTarget*)m_cboTargetA.GetItemData(nCurSelect);
        nCurSelect = m_cboTargetB.GetCurSel();
        if (nCurSelect < 0)
        {
            AfxMessageBox(_T("네트워크를 선택해 주세요."));
            m_cboTargetB.SetFocus();
            return;
        }

        KTarget* pTargetB = (KTarget*)m_cboTargetB.GetItemData(nCurSelect);
        if (pTargetA->GetObjectID() == pTargetB->GetObjectID())
        {
            m_cboScenarioB.SetFocus();
            AfxMessageBox(_T("동일한 두 네트워크는 비교할 수 없습니다."));
            return;
        }

        int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO_NODE, IDC_RADIO_TURN);
        if (nCheckedRadioBtn == IDC_RADIO_NODE)
        {
            SavaNodeA2B();
        }
        else if (nCheckedRadioBtn == IDC_RADIO_LINK)
        {
            SavaLinkB();
        }

		CheckBoxState();
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


void KNetworkCompareDlg::SavaLinkB()
{
    try
    {
        KMapView* pMapView = nullptr;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkeyProgress(this, QBicDonkeyProgress::DEFAULT_TITLE, TRUE, FALSE, FALSE, nLang);
        CString strNotExistNode(_T(""));

        CXTPReportRecords* pRecords = m_wndReport.GetRecords();
        int nChanedCount(0);
        int nCount = pRecords->GetCount();
        for (int i=0; i<nCount; i++)
        {
            CXTPReportRecord* pRecord = pRecords->GetAt(i);              
            CXTPReportRecordItem* pItem  = pRecord->GetItem(4);

            //★ 보이고 있는 것만
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

            if (pItem->GetHasCheckbox() == FALSE) {
                continue;                  
            }

            if (pItem->IsChecked() == FALSE) {
                continue;
            }

            //A의 것을 B에 복사 행위를 수행
            CXTPReportRecordItem* pItemID   = pRecord->GetItem(0);
            CXTPReportRecordItem* pItemTopo = pRecord->GetItem(1);
            CXTPReportRecordItem* pItemAtt  = pRecord->GetItem(2);
            CXTPReportRecordItem* pItemMapA = pRecord->GetItem(5);
            CXTPReportRecordItem* pItemMap  = pRecord->GetItem(7);

            Integer nxLinkID  = (Integer)pItemID->GetItemData();
            int     nTopoOper = (int)pItemTopo->GetItemData();
            int     nAttOper  = (int)pItemAtt->GetItemData();

            KTarget*  pTargetA  = (KTarget*)pItemMapA->GetItemData();
            KMapView* pMapViewA = ImChampFrameWindow::GetShowMapView(pTargetA);

            KTarget*  pTargetB  = (KTarget*)pItemMap->GetItemData();
            KMapView* pMapViewB = ImChampFrameWindow::GetShowMapView(pTargetB); {
                pMapView = pMapViewB;
            }

            std::map<Integer, TCompareLinkField>::iterator iterA = m_mapLinksA.find(nxLinkID);
            std::map<Integer, TCompareLinkField>::iterator endA  = m_mapLinksA.end();

            std::map<Integer, TCompareLinkField>::iterator iterB = m_mapLinksB.find(nxLinkID);
            std::map<Integer, TCompareLinkField>::iterator endB  = m_mapLinksB.end();

			KDBaseConPtr spDBaseConnection = pTargetB->GetDBaseConnection();

            if (nTopoOper == 1 && nAttOper == 2)
            {
                //A의 속성 정보를 B에 Update
                if (iterA == endA)
                    continue;
                if (iterB == endB)
                    continue;

                TCompareLinkField& oLinkA = iterA->second;
                TCompareLinkField& oLinkB = iterB->second;

                CString strSQL = oLinkA.GetUpdateSQL();
                if (strSQL.GetLength() > 0)
                {
                    CString strMsg;
                    strMsg.Format(_T("Link : %I64d Update"), nxLinkID);
                    oDonkeyProgress.SendTextEvent(strMsg);
                                        
                    if (oLinkA.TnLinkType != oLinkB.TnLinkType)
                    {   
                        //★ 
                    }
                    spDBaseConnection->ExecuteUpdate(strSQL);
                    nChanedCount++;
                }
            }
            else if (nTopoOper == 3)
            {
                //A의 Geometry, 속성 추가
                if (iterA == endA)
                    continue;

                TCompareLinkField& oLinkA = iterA->second;
                TxRecord oRecord = oLinkA.GetInsertRecord();

                if (KBulkDBaseNode::ExistNode(pTargetB, oLinkA.TnxFNodeID) == false) {
                    strNotExistNode.AppendFormat(_T("\n 링크 - 시작노드 : '%I64d'이 존재하지 않습니다."), oLinkA.TnxFNodeID);
                    continue;
                }

                if (KBulkDBaseNode::ExistNode(pTargetB, oLinkA.TnxTNodeID) == false)
                {
                    strNotExistNode.AppendFormat(_T("\n 링크 - 끝노드   : '%I64d'이 존재하지 않습니다."), oLinkA.TnxTNodeID);
                    continue;
                }


                ITxGeometryPtr spLinkGeo = FineLinkGeometry(pMapViewA, nxLinkID);
                if (spLinkGeo != nullptr) {
                    InsertLink(pMapViewB, nxLinkID, spLinkGeo, oRecord);
                    nChanedCount++;
                }
            }
        }

        if (nChanedCount > 0)
        {
            try
            {
                if (pMapView != nullptr) {
                    pMapView->ReloadRender(false, 2);
                    pMapView->MapRefresh();
                }

                if (m_pTableB)
                    m_pTableB->Notify();
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

        oDonkeyProgress.Close();
        if(strNotExistNode.GetLength() > 0)
            AfxMessageBox(strNotExistNode);
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

void KNetworkCompareDlg::OnBnClickedCheck1()
{
    try
    {
        BOOL bChecked = IsDlgButtonChecked(IDC_CHECK1); 
		if (TRUE == bChecked)
		{
			CheckDlgButton(IDC_CHECK2, FALSE);
		}
		
        CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	    
        for (int i=0; i<pRecords->GetCount(); i++)
        {
            CXTPReportRecord*     pRecord = pRecords->GetAt(i);
            CXTPReportRecordItem* pItem   = pRecord->GetItem(3);
			CXTPReportRecordItem* pItemB = pRecord->GetItem(4);

            //★ 
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

            if (pItem->GetHasCheckbox() == TRUE) {
                pItem->SetChecked(bChecked);
            }

			if (TRUE == bChecked)
			{
				if (pItemB->GetHasCheckbox() == TRUE)
				{
					pItemB->SetChecked(FALSE);
				}
			}
        }

        m_wndReport.RedrawControl();

		CheckBoxState();
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


void KNetworkCompareDlg::OnBnClickedCheck2()
{
    try
    {
        BOOL bChecked = IsDlgButtonChecked(IDC_CHECK2); 
		if (TRUE == bChecked)
		{
			CheckDlgButton(IDC_CHECK1, FALSE);
		}

        CXTPReportRecords* pRecords = m_wndReport.GetRecords();

        for (int i=0; i<pRecords->GetCount(); i++)
        {
            CXTPReportRecord*     pRecord = pRecords->GetAt(i);
            CXTPReportRecordItem* pItem   = pRecord->GetItem(4);
			CXTPReportRecordItem* pItemA = pRecord->GetItem(3);

            //★ 
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

            if (pItem->GetHasCheckbox() == TRUE) {
                pItem->SetChecked(bChecked);
            }

			if (TRUE == bChecked)
			{
				if (pItemA->GetHasCheckbox() == TRUE)
				{
					pItemA->SetChecked(FALSE);
				}
			}
        }

        m_wndReport.RedrawControl();

		CheckBoxState();
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

void KNetworkCompareDlg::CheckBoxState()
{
	try
	{
		int nCountBoxA(0);
		int nCountBoxB(0);

		int nCountCheckStateA(0);
		int nCountCheckStateB(0);

		CXTPReportRecords* pRecords = m_wndReport.GetRecords();
		for (int i=0; i < pRecords->GetCount(); i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(3);

            //★ 
            if (pRecord->IsVisible() == FALSE) {
                continue;
            }

			if (pItem->GetHasCheckbox() == TRUE) {
				nCountBoxA++;

				if (pItem->IsChecked()== TRUE) {
					nCountCheckStateA++;
				}
			}

			pItem   = pRecord->GetItem(4);

			if (pItem->GetHasCheckbox() == TRUE) {
				nCountBoxB++;
				if (pItem->IsChecked()== TRUE) {
					nCountCheckStateB++;
				}
			}
		}

		CheckDlgButton(IDC_CHECK1, FALSE);
		if (nCountBoxA != 0) {
			if ((nCountBoxA == nCountCheckStateA) && (0 == nCountCheckStateB)) {
				CheckDlgButton(IDC_CHECK1, TRUE);
			}
		}

		CheckDlgButton(IDC_CHECK2, FALSE);
		if (nCountBoxB != 0) {
			if ((nCountBoxB == nCountCheckStateB) && (0 == nCountCheckStateA)) {
				CheckDlgButton(IDC_CHECK2, TRUE);
			}
		}

		if (nCountCheckStateA == 0) {
			GetDlgItem(IDC_BUTTON_SAVE_A)->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(IDC_BUTTON_SAVE_A)->EnableWindow(TRUE);
		}

		if (nCountCheckStateB == 0) {
			GetDlgItem(IDC_BUTTON_SAVE_B)->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(IDC_BUTTON_SAVE_B)->EnableWindow(TRUE);
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

void KNetworkCompareDlg::FindZoneInfo( KTarget* pTarget, Integer nxID, TTempZoneInfo& oZoneInfo )
{
    CString strSQL(_T(""));
    strSQL.AppendFormat(_T("Select zone_id, name, full_name, main_category, sub_category From Zone Where zone_id = %I64d"), nxID);
    
    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
    KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
    if (spResultSet->Next()) {
        oZoneInfo.m_nxZoneID      = spResultSet->GetValueInt64 (0);
        oZoneInfo.m_strName       = spResultSet->GetValueString(1);
        oZoneInfo.m_strFullName   = spResultSet->GetValueString(2);
        oZoneInfo.m_nMainCategory = spResultSet->GetValueInt   (3);
        oZoneInfo.m_nSubCategory  = spResultSet->GetValueInt   (4);
    }
    else {
        oZoneInfo.m_nxZoneID = nxID;
    }
}

ITxGeometryPtr KNetworkCompareDlg::FindZoneGeometry( KMapView* pMapView, Integer nxID )
{   
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return nullptr;

    TxFeaturePtr spFeature = spLayer->GetFeature(nxID);
    if (spFeature != nullptr)
        return spFeature->Geometry();

    return nullptr;
}

void KNetworkCompareDlg::InsertZone( KMapView* pMapView, ITxGeometryPtr spGeometry, TTempZoneInfo& oZoneInfo )
{
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    TxRecord oRecord; {
        oRecord.AddItem(_T("zone_id"),       oZoneInfo.m_nxZoneID);
        oRecord.AddItem(_T("name"),          oZoneInfo.m_strName);
        oRecord.AddItem(_T("full_name"),     oZoneInfo.m_strFullName);
        oRecord.AddItem(_T("main_category"), oZoneInfo.m_nMainCategory);
        oRecord.AddItem(_T("sub_category"),  oZoneInfo.m_nSubCategory);
    }

    spFLayer->Insert(oZoneInfo.m_nxZoneID, spGeometry, oRecord);
}

void KNetworkCompareDlg::DeleteZone( KMapView* pMapView, Integer nxID )
{
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    spFLayer->Delete(nxID);
}

void KNetworkCompareDlg::InsertNode( KMapView* pMapView, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord )
{
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    spFLayer->Insert(nxID, spGeometry, oRecord);
}

void KNetworkCompareDlg::InsertLink( KMapView* pMapView, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord)
{
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return;

    TxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
    if (spFLayer == nullptr)
        return;

    spFLayer->Insert(nxID, spGeometry, oRecord);
}

ITxGeometryPtr KNetworkCompareDlg::FineLinkGeometry( KMapView* pMapView, Integer nxID )
{
    ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return nullptr;

    TxFeaturePtr spFeature = spLayer->GetFeature(nxID);
    if (spFeature != nullptr)
        return spFeature->Geometry();

    return nullptr;
}


void KNetworkCompareDlg::OnBnClickedChkFilter()
{
    if (IsDlgButtonChecked(IDC_CHK_FILTER) == BST_CHECKED) { //★ 필터 조건 적용
        GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
        GetDlgItem(IDC_CBO_TYPE)->EnableWindow(TRUE);
    }
    else {
        GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
        GetDlgItem(IDC_CBO_TYPE)->EnableWindow(FALSE);
    }

    UpdateFilter();
    CheckBoxState();
}

void KNetworkCompareDlg::UpdateFilter()
{
    CXTPReportRecords* pRecords = m_wndReport.GetRecords();

    if (IsDlgButtonChecked(IDC_CHK_FILTER) != BST_CHECKED || m_cboType.GetCount() == 0) {
        int nCopyBA(0);
        int nCopyAB(0);
        
        for (int i = 0; i < pRecords->GetCount(); i++) {
            CXTPReportRecord* pRecord = pRecords->GetAt(i);
            pRecord->SetVisible(TRUE);

            CXTPReportRecordItem* pItem = pRecord->GetItem(3);
            if (pItem->GetHasCheckbox() == TRUE) {
                nCopyBA++;
            }

            pItem = pRecord->GetItem(4);
            if (pItem->GetHasCheckbox() == TRUE) {
                nCopyAB++;
            }
        }

        if (nCopyBA > 0) {
            GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
        }
        else {
            GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
            CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
        }

        if (nCopyAB > 0) {
            GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
        }
        else {
            GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
            CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
        }

        m_wndReport.Populate();
        return;
    }

    int nCheck = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);    
    int nType = -1; {
        int nSel = m_cboType.GetCurSel();
        nType = (int)m_cboType.GetItemData(nSel);
    }
        
    int nColIdx = 10;
    if (IDC_RADIO1 == nCheck) {
        nColIdx = 9;
    }

    int nCopyBA(0);
    int nCopyAB(0);
    for (int i = 0; i < pRecords->GetCount(); i++) {
        CXTPReportRecord* pRecord = pRecords->GetAt(i);
        CXTPReportRecordItem* pItem = pRecord->GetItem(nColIdx);
        int nItemType = (int)pItem->GetItemData();
        if (nItemType == nType) {
            pRecord->SetVisible(TRUE);

            CXTPReportRecordItem* pItemCopy = pRecord->GetItem(3);
            if (pItemCopy->GetHasCheckbox() == TRUE) {
                nCopyBA++;
            }

            pItemCopy = pRecord->GetItem(4);
            if (pItemCopy->GetHasCheckbox() == TRUE) {
                nCopyAB++;
            }

        }
        else {
            pRecord->SetVisible(FALSE);
        }
    }

    if (nCopyBA > 0) {
        GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
    }
    else {
        GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
        CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
    }

    if (nCopyAB > 0) {
        GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
    }
    else {
        GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
        CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
    }

    m_wndReport.Populate();
}


void KNetworkCompareDlg::OnCbnSelchangeCboType()
{
    int nCheck = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
    int nAB = 1;
    if (nCheck == IDC_RADIO1) {
        nAB = 0;
    }
    int nNetType(1); //★ 링크
    nCheck = GetCheckedRadioButton(IDC_RADIO_NODE, IDC_RADIO_LINK);
    if (nCheck == IDC_RADIO_NODE) {
        nNetType = 0;
    }

    auto& mapType = m_mapCboType[nAB];
    int&  nPreType = mapType[nNetType];

    nPreType = m_cboType.GetCurSel();

    UpdateFilter();
}

void KNetworkCompareDlg::InitCboType()
{
    m_cboType.ResetContent();
    KTarget* pTarget = nullptr;

    int nCheck = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
    int nAB = 0;
    if (nCheck == IDC_RADIO1) {
        int nCurSelect = m_cboTargetA.GetCurSel();
        if (nCurSelect >= 0) {
            pTarget = (KTarget*)m_cboTargetA.GetItemData(nCurSelect);
        }
    }
    else {
        int nCurSelect = m_cboTargetB.GetCurSel();
        if (nCurSelect >= 0) {
            pTarget = (KTarget*)m_cboTargetB.GetItemData(nCurSelect);
        }

        nAB = 1;
    }
        
    if (pTarget == nullptr) {
        return;
    }

    try
    {
        int nNetType(0); //★ 노드
        std::map<int, CString> mapCode; {
            nCheck = GetCheckedRadioButton(IDC_RADIO_NODE, IDC_RADIO_LINK);
            KCodeManager* pCodeMgr = pTarget->CodeManager();
            KCodeGroup*   pCodeUserGroup = nullptr;
            if (nCheck == IDC_RADIO_NODE) {
                pCodeUserGroup = pCodeMgr->FindCodeGroup(USER_NODECODE_GROUP_KEY);
            }
            else {
                pCodeUserGroup = pCodeMgr->FindCodeGroup(USER_LINKCODE_GROUP_KEY);
                nNetType = 1;
            }
            pCodeUserGroup->GetCodes(mapCode);
        }

        for (auto iter = mapCode.begin(); iter != mapCode.end(); ++iter) {
            int nType = iter->first;

            if (nCheck == IDC_RADIO1) {
                if (m_setTypeA.size() > 0) {

                    if (m_setTypeA.find(nType) == m_setTypeA.end()) {
                        continue;
                    }
                }
            }
            else {
                if (m_setTypeB.size() > 0) {
                    if (m_setTypeB.find(nType) == m_setTypeB.end()) {
                        continue;
                    }
                }
            }

            CString strName = iter->second; {
                strName.Trim();
            }

            CString strItem(_T(""));
            if (strName.GetLength() == 0) {
                strItem.Format(_T("%d:(x)"), nType);
            }
            else {
                strItem.Format(_T("%d:%s"), nType, strName);
            }

            int nID = m_cboType.AddString(strItem); {
                m_cboType.SetItemData(nID, nType);
            }
        }

        auto& mapType = m_mapCboType[nAB];
        int&  nPreType = mapType[nNetType];

        int nCnt = m_cboType.GetCount();
        if (nCnt > 0 && nCnt >= nPreType) {
            m_cboType.SetCurSel(nPreType);
        }
        else {
            m_cboType.SetCurSel(0);
        }
    }
    catch (int& ex) {
    	
        CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    }
    catch (...) {
        TxLogDebugException();
    }
}

void KNetworkCompareDlg::SetRadioStatus(UINT nID)
{
    InitCboType(); // IDC_RADIO1, IDC_RADIO2
    UpdateFilter();
    CheckBoxState();
}


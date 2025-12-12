// KPASumDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KPASumDlg.h"
#include "afxdialogex.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "PurposeManager.h"
#include "PAPurpose.h"
#include "KDBaseZonePASum.h"
// KPASumDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPASumDlg, CXTPResizeDialog)

KPASumDlg::KPASumDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KPASumDlg::IDD, pParent)
{

}

KPASumDlg::~KPASumDlg()
{
}

void KPASumDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT1, m_wndReportTree);
    DDX_Control(pDX, IDC_REPORT2, m_wndReportData);
}

void KPASumDlg::initTreeReportControl(void)
{
    // Report Header 설정
    m_wndReportTree.ShowHeader(FALSE);

    // Report tree 설정
    m_wndReportTree.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;

    // Add Columns
    CXTPReportColumn* pColumn = m_wndReportTree.AddColumn(new CXTPReportColumn(0, _T("Target"), 150));
    pColumn->SetTreeColumn(TRUE);

    // Report control 설정
    m_wndReportTree.AllowEdit(FALSE);
    m_wndReportTree.EditOnClick(FALSE);
    m_wndReportTree.SetMultipleSelection(FALSE);
    m_wndReportTree.SetTreeIndent(10);
}

void KPASumDlg::initDataReportHeader()
{
    CXTPReportColumn* pColumn = NULL;
    pColumn = m_wndReportData.AddColumn(new CXTPReportColumn(0, _T("시나리오"), 50));
    m_wndReportData.GetColumns()->GetGroupsOrder()->Add(pColumn);
    pColumn->SetVisible(FALSE);

    pColumn = m_wndReportData.AddColumn(new CXTPReportColumn(1, _T("Target"),  50));
    std::map<KPurposeKey, CString>::iterator iter = m_AllPurposeMap.begin();
    std::map<KPurposeKey, CString>::iterator end  = m_AllPurposeMap.end();

    int nColumnIndex = 2;
    CString strColumnName;
    while ( iter != end )
    {        
        strColumnName = iter->second; // +  _T("(P)");
        pColumn = m_wndReportData.AddColumn(new CXTPReportColumn(nColumnIndex, strColumnName,  40));
        ++nColumnIndex;
        /*strColumnName = iter->second +  _T("(A)");
        pColumn = m_wndReportData.AddColumn(new CXTPReportColumn(nColumnIndex, strColumnName,  50));
        ++nColumnIndex;*/
        ++iter;
    }
}

void KPASumDlg::updateTargetTree(void)
{
    CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
    KProject* pProject = pApp->GetProject();
    if(NULL == pProject)
    {
        return;
    }

    CXTPReportRecord* pBaseRecord = NULL;
    CXTPReportRecordItem* pBaseItem = NULL;

    KScenario* pBase = pProject->GetBaseScenario();
    if((NULL != pBase) && (pBase->IsOpened() == true))
    {
        pBaseRecord = m_wndReportTree.AddRecord(new CXTPReportRecord);
        pBaseItem = pBaseRecord->AddItem(new CXTPReportRecordItemText);
        pBaseItem->SetCaption(pBase->GetName());
        pBaseItem->SetItemData((DWORD_PTR)pBase);

        addTargetRecord(pBaseRecord, pBase);
    }

    const KTDScenarioMap& scenarioMap = pProject->GetScenarioMap();
    KTDScenarioMap::const_iterator itScenario, itEnd = scenarioMap.end();
    for(itScenario = scenarioMap.begin(); itScenario != itEnd; ++itScenario)
    {
        KScenario* pScenario = itScenario->second;
        if(pScenario->IsOpened() == false)
        {
            continue;
        }
        CXTPReportRecord* pRecord;
        CXTPReportRecordItem* pItem;
        pRecord = m_wndReportTree.AddRecord(new CXTPReportRecord);
        pItem = pRecord->AddItem(new CXTPReportRecordItemText);
        pItem->SetCaption(pScenario->GetName());
        pItem->SetItemData((DWORD_PTR)pScenario);

        addTargetRecord(pRecord, pScenario);
    }

    if(NULL != pBaseRecord)
    {
        CXTPReportRecord* pCheckRecord = pBaseRecord->GetChilds()->GetAt(0);
        if(NULL != pCheckRecord)
        {
            CXTPReportRecordItem* pCheckItem = pCheckRecord->GetItem(0);
            if(NULL != pCheckItem)
            {
                pCheckItem->SetChecked(TRUE);
                KTarget* pTarget = (KTarget*)(pCheckItem->GetItemData());
                //updateFieldConstraint(pTarget);
            }
        }
    }

    m_wndReportTree.Populate();
    m_wndReportTree.ExpandAll(TRUE);
}

void KPASumDlg::addTargetRecord(CXTPReportRecord* pParnet, KScenario* pScenario)
{
    CXTPReportRecord* pRecord = NULL;
    CXTPReportRecordItem* pItem = NULL;

    const KTDTargetMap& targetMap = pScenario->GetTargetMap();
    KTDTargetMap::const_iterator itTarget, itEnd = targetMap.end();
    for(itTarget = targetMap.begin(); itTarget != itEnd; ++itTarget)
    {
        KTarget* pTarget = itTarget->second;
        if(pTarget->IsEmptyData() == true)
        {
            continue;
        }

        pRecord = pParnet->GetChilds()->Add(new CXTPReportRecord);
        pItem = pRecord->AddItem(new CXTPReportRecordItemText);
        pItem->SetCaption(pTarget->GetName());
        pItem->SetItemData((DWORD_PTR)pTarget);
        pItem->HasCheckbox(TRUE);        

        KPurposeManager* pPurposeMgr = pTarget->PurposeManager();
        if(NULL == pPurposeMgr)
        {
            continue;
        }

        CString strTemp;
        int nPurposeCount = pPurposeMgr->GetPurposeCount();
        for(int i = 0; i < nPurposeCount; ++i)
        {
            KPAPurpose* pPurpose = pPurposeMgr->GetPurpose(i);

            strTemp.Format(_T("%s(P)"), pPurpose->PurposeName());
            KPurposeKey keyP(pPurpose->PurposeGroup(), pPurpose->PurposeID(), 1);
            m_AllPurposeMap.insert(std::make_pair(keyP, strTemp));

            strTemp.Format(_T("%s(A)"), pPurpose->PurposeName());
            KPurposeKey keyA(pPurpose->PurposeGroup(), pPurpose->PurposeID(), 2);
            m_AllPurposeMap.insert(std::make_pair(keyA, strTemp));
        }
    }
}

void KPASumDlg::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result)
{
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
    if(!pItemNotify->pRow || !pItemNotify->pColumn)
    {
        return;
    }

    size_t nCount = pItemNotify->pColumn->GetItemIndex();//m_AllPurposeMap.size();
    CString str;
    str.Format(_T("%d"), nCount);
    
    if(pItemNotify->pColumn->GetItemIndex() == 0)
    {
        CXTPReportRecord* pCurrent = pItemNotify->pItem->GetRecord();
        if(pCurrent->HasChildren() == FALSE)
        {
            /*CXTPReportRecords* pParentRecords = m_wndReportTree.GetRecords();
            int nParentCount = pParentRecords->GetCount();
            for(int i = 0; i < nParentCount; ++i)
            {
                CXTPReportRecord* pParentRecord = pParentRecords->GetAt(i);
                CXTPReportRecords* pRecords = pParentRecord->GetChilds();
                int nRecordCount = pRecords->GetCount();
                for(int j = 0; j < nRecordCount; ++j)
                {
                    CXTPReportRecord* pRecord = pRecords->GetAt(j);
                    CXTPReportRecordItem* pItem = pRecord->GetItem(0);
                    pItem->SetChecked(FALSE);
                }
            }

            pItemNotify->pItem->SetChecked(TRUE);*/
            KTarget* pTarget = (KTarget*)(pItemNotify->pItem->GetItemData());
            AfxMessageBox(pTarget->GetName());
        }
    }

}

void KPASumDlg::GetPurposeInfo()
{
    CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
    KProject* pProject = pApp->GetProject();
    if(NULL == pProject)
    {
        return;
    }

    KScenario* pBase = pProject->GetBaseScenario();
    if((NULL != pBase) && (pBase->IsOpened() == true))
    {
       
    }
}

BEGIN_MESSAGE_MAP(KPASumDlg, CXTPResizeDialog)
    ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KPASumDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void KPASumDlg::initDataReport()
{
    //std::map<KPurposeKey, CString> m_AllPurposeMap;
    
    CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
    KProject* pProject = pApp->GetProject();
    if(NULL == pProject)
    {
        return;
    }

    KScenario* pBase = pProject->GetBaseScenario();
    if((NULL != pBase) && (pBase->IsOpened() == true))
    {
        initDataReportTarget(pBase);
    }    

    const KTDScenarioMap& scenarioMap = pProject->GetScenarioMap();
    KTDScenarioMap::const_iterator itScenario, itEnd = scenarioMap.end();
    for(itScenario = scenarioMap.begin(); itScenario != itEnd; ++itScenario)
    {
        KScenario* pScenario = itScenario->second;
        if(pScenario->IsOpened() == false)
        {
            continue;
        }
        initDataReportTarget(pScenario);
    }
}

void KPASumDlg::initDataReportTarget(KScenario* pScenario )
{
    std::map<KPurposeKey, CString> records = m_AllPurposeMap;
    if((NULL != pScenario) && (pScenario->IsOpened() == true))
    {
        const KTDTargetMap& targetMap = pScenario->GetTargetMap();
        KTDTargetMap::const_iterator itTarget, itEnd = targetMap.end();
        for(itTarget = targetMap.begin(); itTarget != itEnd; ++itTarget)
        {
            KTarget* pTarget = itTarget->second;
            KDBaseZonePASum::PASum(pTarget, records);

            CXTPReportRecord* pRecord;
            CXTPReportRecordItem* pItem;
            pRecord = m_wndReportData.AddRecord(new CXTPReportRecord);
            pItem = pRecord->AddItem(new CXTPReportRecordItemText(pScenario->GetName()));
            pItem = pRecord->AddItem(new CXTPReportRecordItemText(pTarget->GetName()));

            std::map<KPurposeKey, CString>::iterator iter = records.begin();
            std::map<KPurposeKey, CString>::iterator end  = records.end();

            while (iter != end)
            {
                pItem = pRecord->AddItem(new CXTPReportRecordItemText(iter->second));
                m_wndReportData.Populate();
                ++iter;
            }
            m_wndReportData.Populate();
        }
    }  
}

// KPASumDlg 메시지 처리기입니다.
BOOL KPASumDlg::OnInitDialog()
{
    CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    initTreeReportControl();
    updateTargetTree();

	KReportCtrlSetting::Default(m_wndReportData);
    initDataReportHeader();
    initDataReport();
    m_wndReportData.Populate();
    //updatePurposeRecord();

    SetResize(IDC_REPORT1,   SZ_TOP_LEFT,     SZ_BOTTOM_LEFT);
    SetResize(IDC_REPORT2,   SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR2, SZ_TOP_LEFT,     SZ_BOTTOM_LEFT);
    SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPASumDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CXTPResizeDialog::OnOK();
}

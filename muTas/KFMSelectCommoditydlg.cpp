// KFMSelectCommodity.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KFMSelectCommodityDlg.h"
#include "afxdialogex.h"

//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Purpose.h"
#include "Target.h"



/* define column index */
#define MDEMAND_PURPOSE		0
#define MDEMAND_SELECTION	1

#define MINPUT_NAME			0
#define MINPUT_STARTDATE	1
#define MINPUT_ENDDATE		2


// KFMSelectCommodityDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(KFMSelectCommodityDlg, KDialogEx)

    KFMSelectCommodityDlg::KFMSelectCommodityDlg(CWnd* pParent /*=NULL*/)
    :	KDialogEx(KFMSelectCommodityDlg::IDD, pParent),
    m_pTarget(NULL)
{

}

KFMSelectCommodityDlg::~KFMSelectCommodityDlg()
{
}

void KFMSelectCommodityDlg::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReport);
    DDX_Control(pDX, IDC_REPORT3, m_wndInput);
}


BEGIN_MESSAGE_MAP(KFMSelectCommodityDlg, KDialogEx)
    ON_BN_CLICKED(IDOK, &KFMSelectCommodityDlg::OnBnClickedOk)
    ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT,  OnReportCheckItem)
    ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT3, OnInputCheckItem)
	ON_BN_CLICKED(IDC_CHECK1, &KFMSelectCommodityDlg::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()


void KFMSelectCommodityDlg::initReportControl(void)
{
    //// Report Header 설정
    //m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
    //m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
    //m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
    //m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

    // Report tree 설정
    //m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
    ////m_wndReport.GetPaintManager()->SetGridStyle(FALSE, xtpReportGridSolid);
    //m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

    // Add Columns
    CXTPReportColumn* pColumn = NULL;
    pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MDEMAND_PURPOSE, _T("품목"), 150));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReport.AddColumn(new CXTPReportColumn(MDEMAND_SELECTION, _T("선택"), 100));
    pColumn->SetHeaderAlignment(DT_CENTER);
    pColumn->SetAlignment(xtpColumnIconCenter);

    //// Report control 설정
    //m_wndReport.AllowEdit(FALSE);
    //m_wndReport.EditOnClick(FALSE);
    //m_wndReport.SetMultipleSelection(FALSE);
}


void KFMSelectCommodityDlg::updatePurposeRecord(void)
{
    size_t nCount = m_vecFModeChoicInfo.size();
    for (size_t i=0; i<nCount; i++)
    {
        KFModeChoiceInfo*     pKFModeChoiceInfo = m_vecFModeChoicInfo[i];
        KPurpose*             pPurpose          = pKFModeChoiceInfo->Purpose();

        CXTPReportRecord*     pRecord           = m_wndReport.AddRecord(new CXTPReportRecord);
        CXTPReportRecordItem* pItem             = pRecord->AddItem(new CXTPReportRecordItemText(pPurpose->PurposeName()));
        
        pItem->SetItemData((DWORD_PTR)pKFModeChoiceInfo);
        pItem = pRecord->AddItem(new CXTPReportRecordItem);
        pItem->HasCheckbox(TRUE);
       
        if ( pKFModeChoiceInfo->Selected() )
        {
            pItem->SetChecked(TRUE);
        }
        else
        {
            pItem->SetChecked(FALSE);
        }
    }

    m_wndReport.Populate();
}


void KFMSelectCommodityDlg::initInputReportControl(void)
{
    //// Report Header 설정
    //m_wndInput.GetReportHeader()->AllowColumnRemove(FALSE);
    //m_wndInput.GetReportHeader()->AllowColumnSort(FALSE);
    //m_wndInput.GetReportHeader()->AllowColumnReorder(FALSE);
    //m_wndInput.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

    // Report tree 설정
    m_wndInput.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
    m_wndInput.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

    // Add Columns
    CXTPReportColumn* pColumn = NULL;
    pColumn = m_wndInput.AddColumn(new CXTPReportColumn(MINPUT_NAME, _T("품목"), 150));
    pColumn->SetTreeColumn(TRUE);
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndInput.AddColumn(new CXTPReportColumn(MINPUT_STARTDATE, _T("Created Date"), 100));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndInput.AddColumn(new CXTPReportColumn(MINPUT_ENDDATE, _T("Modified Date"), 100));
    pColumn->SetHeaderAlignment(DT_CENTER);

    //// Report control 설정
    //m_wndInput.AllowEdit(FALSE);
    //m_wndInput.EditOnClick(FALSE);
    //m_wndInput.SetMultipleSelection(FALSE);
    //m_wndInput.SetTreeIndent(10);
}


void KFMSelectCommodityDlg::updateInputRecord(void)
{
    m_wndInput.GetRecords()->RemoveAll();

    CXTPReportRecords* pRecords = m_wndReport.GetRecords();
    int nRecordCount = pRecords->GetCount();
    for(int i = 0; i < nRecordCount; ++i)
    {
        CXTPReportRecord*     pRecord        = pRecords->GetAt(i);
        CXTPReportRecordItem* pItemSelection = pRecord->GetItem(MDEMAND_SELECTION);
        CXTPReportRecordItem* pItemPurpose   = pRecord->GetItem(MDEMAND_PURPOSE);

        KFModeChoiceInfo* pKFModeChoiceInfo = (KFModeChoiceInfo*)(pItemPurpose->GetItemData());

        if(pItemSelection->IsChecked() == TRUE)
        {
            CXTPReportRecord*     pInputRecord = m_wndInput.AddRecord(new CXTPReportRecord);
            CXTPReportRecordItem* pInputItem   = pInputRecord->AddItem(new CXTPReportRecordItemText);
			pInputRecord->AddItem(new CXTPReportRecordItemText);
			pInputRecord->AddItem(new CXTPReportRecordItemText);

            KPurpose* pPurpose = pKFModeChoiceInfo->Purpose();
            pInputItem->SetCaption(pPurpose->PurposeName());
            pInputItem->SetItemData((DWORD_PTR)pKFModeChoiceInfo);

            updateInputPurposeRecord(pInputRecord, pKFModeChoiceInfo);   
        }
    }

    m_wndInput.Populate();
    m_wndInput.ExpandAll(TRUE);
}


void KFMSelectCommodityDlg::updateInputPurposeRecord(CXTPReportRecord* pParent, KFModeChoiceInfo* pKFModeChoiceInfo)
{
    KDBaseConnectionPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
    CString             strSQL = _T("");

    KPurpose*  pPurpose     = pKFModeChoiceInfo->Purpose();
    KIOColumn* pInputColumn = pKFModeChoiceInfo->ObjectInputColumn();

    KIOTables* pIOTables = m_pTarget->Tables();
        ASSERT(NULL != pIOTables);
    KIOTable* pPurposeODTable = pIOTables->FindTable(_T("purpose_od"));
        ASSERT(NULL != pPurposeODTable);
    const KIOColumns* pColumns = pPurposeODTable->Columns();    
    
    strSQL.Format(
        _T(" SELECT purpose_od_column_name, start_date, end_date ")
        _T(" FROM distribution_model ")
        _T(" WHERE object_group_code = %d ")
        _T(" AND detail_object_id = %d ")
        _T(" ORDER BY end_date DESC "),
        pPurpose->PurposeGroup(),
        pPurpose->PurposeID()
        );

    bool bCheck = false;

    try
    {
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CXTPReportRecords* pRecords = pParent->GetChilds();

		while(spResult->Next())
        {
            CXTPReportRecord*     pRecord = pRecords->Add(new CXTPReportRecord);
            CXTPReportRecordItem* pItem = NULL;

            CString strColumn = spResult->GetValueString(0);
            pItem = pRecord->AddItem(new CXTPReportRecordItemText);
            KIOColumn* pColumn = pColumns->GetColumn(strColumn);

            if(NULL != pColumn)
            {
                pItem->SetCaption(pColumn->Caption());
                pItem->SetItemData((DWORD_PTR)pColumn);
            }
            else
            {
                pItem->SetCaption(_T("(X)") + strColumn);
                // TODO : 존재하지 않는 컬럼
            }

            pItem->HasCheckbox(TRUE);

            if((NULL != pInputColumn) && (pInputColumn == pColumn))
            {
                pItem->SetChecked(TRUE);
                bCheck = true;
            }

            CString strStartDate = spResult->GetValueString(1);
            pItem = pRecord->AddItem(new CXTPReportRecordItemText);
            pItem->SetCaption(strStartDate);

            CString strEndDate = spResult->GetValueString(2);
            pItem = pRecord->AddItem(new CXTPReportRecordItemText);
            pItem->SetCaption(strEndDate);
        }
    }
    catch(...)
    {

    }

    if((false == bCheck) && (pParent->GetChilds()->GetCount() > 0))
    {
        CXTPReportRecord* pRecord = pParent->GetChilds()->GetAt(0);
        CXTPReportRecordItem* pItem = pRecord->GetItem(0);
        pItem->SetChecked(TRUE);
    }
}


int KFMSelectCommodityDlg::applySelectedPurpose(void)
{
    int nSelectedCount = 0;

    CXTPReportRecords* pRecords = m_wndReport.GetRecords();
    int nRecordCount = pRecords->GetCount();
    for(int i = 0; i < nRecordCount; ++i)
    {
        CXTPReportRecord*     pRecord        = pRecords->GetAt(i);
        CXTPReportRecordItem* pItemSelection = pRecord->GetItem(MDEMAND_SELECTION);
        CXTPReportRecordItem* pItemPurpose   = pRecord->GetItem(MDEMAND_PURPOSE);
        
        KFModeChoiceInfo* pKFModeChoiceInfo = (KFModeChoiceInfo*)(pItemPurpose->GetItemData());

        if(pItemSelection->IsChecked() == TRUE)
        {
            pKFModeChoiceInfo->Selected(true);     
            nSelectedCount++;
        }
        else
        {
            pKFModeChoiceInfo->Selected(false);
        }
    }

    return nSelectedCount;
}


void KFMSelectCommodityDlg::applyInputColumn(void)
{
    CXTPReportRecords*  pPurposeRecords = m_wndInput.GetRecords();
    int nPurposeCount = pPurposeRecords->GetCount();

    for(int i = 0; i < nPurposeCount; ++i)
    {
        CXTPReportRecord*     pPurposeRecord = pPurposeRecords->GetAt(i);
        CXTPReportRecordItem* pPurposeItem = pPurposeRecord->GetItem(MINPUT_NAME);

        KFModeChoiceInfo* pKFModeChoiceInfo = (KFModeChoiceInfo*)(pPurposeItem->GetItemData());

        CXTPReportRecords* pInputRecords = pPurposeRecord->GetChilds();
        int nInputCount = pInputRecords->GetCount();

        for(int j = 0; j < nInputCount; ++j)
        {
            CXTPReportRecord* pInputRecord = pInputRecords->GetAt(j);
            CXTPReportRecordItem* pInputItem = pInputRecord->GetItem(MINPUT_NAME);
            if(pInputItem->IsChecked() == TRUE)
            {
                KIOColumn* pColumn = (KIOColumn*)(pInputItem->GetItemData());
                pKFModeChoiceInfo->ObjectInputColumn(pColumn);
                break;
            }
        }
    }
}


// KFMSelectCommodityDlg 메시지 처리기입니다.
BOOL KFMSelectCommodityDlg::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	KReportCtrlSetting::Default(m_wndReport);

    initReportControl();
    updatePurposeRecord();

	KReportCtrlSetting::Default(m_wndInput);
    initInputReportControl();
    updateInputRecord();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KFMSelectCommodityDlg::OnBnClickedOk()
{
    int nSelectedCount = applySelectedPurpose();
    if (nSelectedCount < 1)
    {
        AfxMessageBox(_T("선택한 정보가 없습니다.!!! "));
        return;
    }
    applyInputColumn();

    KDialogEx::OnOK();
}


void KFMSelectCommodityDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	
	if (MDEMAND_SELECTION == nIndex)
	{
		if( FALSE == pItemNotify->pItem->IsChecked() )
		{
			CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
		}
	}

    updateInputRecord();
}


void KFMSelectCommodityDlg::OnInputCheckItem(NMHDR* pNotifyStruct, LRESULT* result)
{
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

    pItemNotify->pItem->SetChecked(TRUE);

    CXTPReportRecord* pCurrentRecord = pItemNotify->pItem->GetRecord();

    CXTPReportRecord* pPurposeRecord = pCurrentRecord->GetParentRecord();
    CXTPReportRecordItem* pPurposeItem = pPurposeRecord->GetItem(MINPUT_NAME);
    KPurpose* pPurpose = (KPurpose*)(pPurposeItem->GetItemData());

    CXTPReportRecordItem* pCurrentItem = pCurrentRecord->GetItem(MINPUT_NAME);
    KIOColumn* pCurrentInput = (KIOColumn*)(pCurrentItem->GetItemData());

    CXTPReportRecords* pInputRecords = pPurposeRecord->GetChilds();
    int nInputCount = pInputRecords->GetCount();
    for(int i = 0; i < nInputCount; ++i)
    {
        CXTPReportRecord* pInputRecord = pInputRecords->GetAt(i);
        if(pInputRecord != pCurrentRecord)
        {
            CXTPReportRecordItem* pInputItem = pInputRecord->GetItem(MINPUT_NAME);
            pInputItem->SetChecked(FALSE);
        }
    }
}


void KFMSelectCommodityDlg::OnBnClickedCheckSelectAll()
{
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
	{
		UpdateReportSelectStatus(TRUE);
	}
	else
	{

		UpdateReportSelectStatus(FALSE);
	}
}

void KFMSelectCommodityDlg::UpdateReportSelectStatus(BOOL a_bCheck)
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord        = pRecords->GetAt(i);
		CXTPReportRecordItem* pItemSelection = pRecord->GetItem(MDEMAND_SELECTION);
		pItemSelection->SetChecked(a_bCheck);
	}

	m_wndReport.RedrawControl();

	updateInputRecord();
}
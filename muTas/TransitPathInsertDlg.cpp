// TransitPathEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TransitPathInsertDlg.h"
#include "afxdialogex.h"
#include "BulkDBaseTransit.h"
#include "ImChampFrameWindow.h"
#include "MapView.h"
#include "NetworkEditLog.h"

//^#include "DBaseConnector.h"
// KTransitPathEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTransitPathInsertDlg, KResizeDialogEx)

KTransitPathInsertDlg::KTransitPathInsertDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTransitPathInsertDlg::IDD, pParent), m_pTarget(nullptr), m_nxTransitID(0),
      m_pLinkTable(nullptr), m_pOpener(nullptr)
{

}

KTransitPathInsertDlg::~KTransitPathInsertDlg()
{
    try
    {
        if (m_pLinkTable != nullptr)
        {
            m_pLinkTable->Unregister(this);
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

void KTransitPathInsertDlg::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReport);
    DDX_Control(pDX, IDC_COMBO1, m_cboDeleteSeq);
    DDX_Control(pDX, IDC_COMBO2, m_cboDeleteOption);
    DDX_Control(pDX, IDC_COMBO3, m_cboReverseSeq);    
}


BEGIN_MESSAGE_MAP(KTransitPathInsertDlg, KResizeDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KTransitPathInsertDlg::OnReportRowDblClick)
    ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KTransitPathInsertDlg::OnReportItemClick)
	//ON_NOTIFY(XTP_NM_REPORT_SELCHANGING, IDC_REPORT, &KTransitPathEditDlg::OnReportRowDblClick)
	
	//XTP_NM_REPORT_SELCHANGED
    ON_BN_CLICKED(IDOK, &KTransitPathInsertDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &KTransitPathInsertDlg::OnBnClickedButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_REVERSE, &KTransitPathInsertDlg::OnBnClickedButtonReverse)
    ON_BN_CLICKED(IDCANCEL, &KTransitPathInsertDlg::OnBnClickedCancel)
    ON_CBN_SELCHANGE(IDC_COMBO2, &KTransitPathInsertDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_RADIO1, &KTransitPathInsertDlg::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &KTransitPathInsertDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()

BOOL KTransitPathInsertDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialControl();
	return TRUE;  
}

void KTransitPathInsertDlg::InitialControl()
{
	try
	{
		KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);

		CXTPReportColumn* pColumn = NULL;
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
        pColumn->SetVisible(FALSE);

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Seq"),   50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("순서"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("LinkID"), 65));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("링크 ID"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(3, _T("To-Station"), 35));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("끝노드 정류장"));
		}

		m_wndReport.Populate();

        CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_cboDeleteOption.AddString(_T("종점까지"));
			m_cboDeleteOption.AddString(_T("시점까지"));
		}
		else
		{
			m_cboDeleteOption.AddString(_T("Forward"));
			m_cboDeleteOption.AddString(_T("Backward"));
		}
        m_cboDeleteOption.SetCurSel(1);
        OnBnClickedRadio1();

        CheckDlgButton(IDC_CHECK1, BST_CHECKED);

        SetResize(IDC_STATIC_INFO,    SZ_TOP_LEFT,     SZ_TOP_RIGHT); 
        SetResize(IDC_STATIC_HBar1,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
        SetResize(IDC_REPORT,         SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);        
        SetResize(IDC_STATIC_HBar2,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

        SetResize(IDC_RADIO1,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);        
        SetResize(IDC_COMBO1,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
        SetResize(IDC_STATIC_1,       SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
        SetResize(IDC_COMBO2,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
        SetResize(IDC_BUTTON_DELETE,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

        SetResize(IDC_RADIO2,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
        SetResize(IDC_EDIT1,          SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
        SetResize(IDC_STATIC_2,       SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
        SetResize(IDC_COMBO3,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);        
        SetResize(IDC_BUTTON_REVERSE, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

        SetResize(IDC_STATIC_HBar3,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
        SetResize(IDOK,               SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
        SetResize(IDCANCEL,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KTransitPathInsertDlg::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    KBulkDBaseLink::AllLinkFTNode(m_pTarget, m_mapLinkFTNode);
    TransitPathPopulate();
}

void KTransitPathInsertDlg::SetEditTransit( KTarget* a_pTaget, Integer a_nxTransitID )
{
    try
    {
    	if (m_pLinkTable != nullptr)
        {
            m_pLinkTable->Unregister(this);
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

	try
	{
		m_pTarget     = a_pTaget;
		m_nxTransitID = a_nxTransitID;

        m_pLinkTable  = m_pTarget->Tables()->FindTable(TABLE_LINK);	
        m_pLinkTable->Register(this);

        KBulkDBaseLink::AllLinkFTNode(m_pTarget, m_mapLinkFTNode);

		//CString strCaption(_T("Transit Insert"));
		CString strCaption(_T("Add"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strCaption = _T("노선 추가");
		}
		SetDlgItemText(IDC_STATIC_INFO, strCaption);

		TransitPathPopulate();
	}
	catch (...)
	{
		TxLogDebugException();
	}	
}

void KTransitPathInsertDlg::TransitPathPopulate(Integer a_nxDeSelectStationID)
{
    try
    {
        m_wndReport.ResetContent();
        size_t nxCount = m_vecTransitPath.size();

        CXTPReportRecord    * pRecord = NULL;
        CXTPReportRecordItem* pItem   = NULL;

        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[i];
            pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

            BOOL bCheck = FALSE;
            //선택
            pItem   = pRecord->AddItem(new CXTPReportRecordItem);
            pItem->HasCheckbox(TRUE);
            pItem->SetChecked(bCheck);
            pItem->SetAlignment(xtpColumnIconCenter);
            pItem->SetEditable(FALSE);

            // Seq
            CString strSeq;
            strSeq.Format(_T("%d"), oTransitPath.TSeq);
            pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strSeq));
            pItem->SetAlignment(xtpColumnTextCenter);

            // LinkID
            CString strLinkID;
            strLinkID.Format(_T("%I64d"), oTransitPath.TLinkID);
            pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strLinkID));

            AutoType iter = m_mapLinkFTNode.find(oTransitPath.TLinkID);
            AutoType end  = m_mapLinkFTNode.end();

            if (iter != end)
            {
                TLinkFTNodeID& oTLinkFTNodeID = iter->second;
                strLinkID.AppendFormat(_T("(%I64d->%I64d)"), oTLinkFTNodeID.TFromNodeID, oTLinkFTNodeID.TToNodeID);
            }
            pItem->SetCaption(strLinkID);
            pItem->SetAlignment(xtpColumnTextCenter);

            // 정류장여부			
            if (1 == oTransitPath.TStationYN || i == nxCount-1) 
            {
                bCheck = TRUE;
            }
            else
            {
                bCheck = FALSE;
            }

            pItem  = pRecord->AddItem(new CXTPReportRecordItem);
            pItem->HasCheckbox(TRUE);
            pItem->SetChecked(bCheck);
            pItem->SetAlignment(xtpColumnIconCenter);
        }

        m_wndReport.Populate();

        UpdatePathInfo();
    }
    catch (...)
    {
        TxLogDebugException();
    }	
}

void KTransitPathInsertDlg::OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		if (nullptr == pClickedItem)
			return;

		CXTPReportRow* pRow = pClickedItem->pRow;
		if (nullptr == pRow)
			return;

		pClickedItem->pRow->SetSelected(TRUE);		
		CXTPReportRecord*         pRecord         = pRow->GetRecord();
		CXTPReportRecordItem*     pRecordItem     = pRecord->GetItem(2);
		CXTPReportRecordItemText* pRecordItemText = dynamic_cast<CXTPReportRecordItemText*>(pRecordItem);
		if (nullptr == pRecordItemText)
			throw 1;

		KMapView* pMapView = ImChampFrameWindow::GetShowMapView(m_pTarget);
		if (nullptr == pMapView)
			throw 1;

		Integer nxLinkID = _ttoi64(pRecordItemText->GetValue());
		pMapView->PanToFeature(KLayerID::Link(), nxLinkID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KTransitPathInsertDlg::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    try
    {
        XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

        if (!pClickedItem->pRow || !pClickedItem->pColumn)
            return;

        int nRowIndex    = pClickedItem->pRow->GetIndex();
        int nColumnIndex = pClickedItem->pColumn->GetItemIndex();
        if( 0 == nColumnIndex )
        {
            CXTPReportRecords*	  pRecords    = NULL;
            CXTPReportRecord*     pRecord     = NULL;
            CXTPReportRecordItem* pItemCheck  = NULL;

            pRecords = m_wndReport.GetRecords();
            int nRecordCount = pRecords->GetCount();

            bool bBackward(true);
            int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);

            if (nCheckedRadioBtn == IDC_RADIO1)
            {
                if ( m_cboDeleteOption.GetCurSel() == 0)
                    bBackward = false;
            }

            // 현재 선택한 항목 이후 것 자동체크
            for (int i=nRowIndex+1; i<nRecordCount; i++)
            {
                pRecord    = pRecords->GetAt(i);
                pItemCheck = pRecord->GetItem(0);
                if (bBackward)
                    pItemCheck->SetChecked(TRUE);
                else
                    pItemCheck->SetChecked(FALSE);
            }

            // 현재 선택한 항목 이전 것 자동체크-False
            for (int i=0; i<nRowIndex; i++)
            {
                pRecord    = pRecords->GetAt(i);
                pItemCheck = pRecord->GetItem(0);
                if (bBackward)
                    pItemCheck->SetChecked(FALSE);
                else
                    pItemCheck->SetChecked(TRUE);
            }
        }

        if( 3 == nColumnIndex )
        {
            Integer nxLinkID = m_vecTransitPath[nRowIndex].TLinkID;
            std::map<Integer, TLinkFTNodeID>::iterator iter = m_mapLinkFTNode.begin();
            std::map<Integer, TLinkFTNodeID>::iterator end  = m_mapLinkFTNode.end();
            iter = m_mapLinkFTNode.find(nxLinkID);
            if (iter != end)
            {
                int nStationYN(0);
                TLinkFTNodeID& oLinkFTNodeID = iter->second;
                if (pClickedItem->pItem->IsChecked()== TRUE)
                    nStationYN = 1;
                else
                    nStationYN = 0;

                m_vecTransitPath[nRowIndex].TStationYN = nStationYN;

                if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
                    OppositeStationStatusChange(oLinkFTNodeID.TToNodeID, nStationYN); //자신(*주의)과 건너편 정류장 값 세팅
            }

            if (m_pOpener)
            {
                m_pOpener->TransitPathStationEditFeedback();
            }

            RedrawReportTransitPath();
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }    
}

void KTransitPathInsertDlg::OnBnClickedButtonDelete()
{   
    try
    {
        int nFirstSelectIndex = m_cboDeleteSeq.GetCurSel();
        if (nFirstSelectIndex < 0)
        {
            AfxMessageBox(_T("삭제 시작 위치를 선택해 주세요."));
            m_cboDeleteSeq.SetFocus();
            return;
        }

        int  nDeleteMethod = m_cboDeleteOption.GetCurSel(); /*0:Forward, 1:Backward*/
        int  nxCount       = (int)m_vecTransitPath.size();
        int  nSeq(1);

        if (nxCount <= nFirstSelectIndex)
        { // 이 경우 발생될 수 없음.
            AfxMessageBox(_T("시작 위치 정보가 잘못되었습니다."));
            m_cboDeleteSeq.SetFocus();
            return;
        }

        std::vector<TTransitPath> vecTransitPath;

        if (nDeleteMethod == 0)
        {
            for(int i=0; i<nFirstSelectIndex; i++) //현재 선택 이후의 것을 삭제하기 때문에 이전 것 만을 보관
            {
                TTransitPath oTransitPath = m_vecTransitPath[i];
                oTransitPath.TSeq = nSeq;
                vecTransitPath.push_back(oTransitPath);

                nSeq++;
            }
        }
        else
        {
            for(int i=nFirstSelectIndex+1; i<nxCount; i++)
            {
                TTransitPath oTransitPath = m_vecTransitPath[i];
                oTransitPath.TSeq = nSeq;
                vecTransitPath.push_back(oTransitPath);

                nSeq++;
            }
        }

        m_vecTransitPath.clear();
        m_vecTransitPath.assign(vecTransitPath.begin(), vecTransitPath.end());
        TransitPathPopulate();

        if (m_pOpener)
        {
            m_pOpener->TransitPathEditFeedback();
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


void KTransitPathInsertDlg::OnBnClickedOk()
{
    try
    {
        if (m_pOpener)
        {
            TransitSave();
            
			Integer nxTransitID = KBulkDBaseTransit::MaxTransitID(m_pTarget);
            m_pOpener->TransitSaveFeedback(nxTransitID);

			KNetworkEditLog::WriteNetworkEditLog(m_pTarget, 2, nxTransitID ,1);
            KResizeDialogEx::OnOK();
        }	
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}


void KTransitPathInsertDlg::TransitSave()
{
    Integer nxTransitID = 0;
    try
    {
    	nxTransitID = KBulkDBaseTransit::MaxTransitID(m_pTarget) + 1;
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
        AfxMessageBox(_T("대중교통 노선 번호 계산 중 오류가 발생하였습니다."));
        return;
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("대중교통 노선 번호 계산 중 오류가 발생하였습니다."));
        return;
    }

    KIOTable*   pIOTable   = nullptr;
    const 
    KIOColumns* pIOColumns = nullptr;
    KIOColumn*  pIOColumn  = nullptr;

    try
    {
        pIOTable   = m_pTarget->Tables()->FindTable(TABLE_TRANSIT);
        pIOColumns = pIOTable->Columns();	
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
        AfxMessageBox(_T("대중교통 관련 테이블 검색 중 오류가 발생하였습니다."));
        return;
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("대중교통 관련 테이블 검색 중 오류가 발생하였습니다."));
        return;
    }
        
    try
    {    	
        int nColumnCount = pIOColumns->ColumnCount();
        CString strTemp;
        std::vector<TTransitKeyValue> vecKeyValue;
        for (int i=0; i<nColumnCount; i++)
        {
            TTransitKeyValue oKeyValue;
            
            pIOColumn = pIOColumns->GetColumn(i);
            CString    strColumnName = pIOColumn->Name();

            if(_tcsicmp(strColumnName, COLUMN_TRANSIT_ID) == 0)
            {
                strTemp.Format(_T("%I64d"), nxTransitID);
                oKeyValue.TKeyName = strColumnName;
                oKeyValue.TValue   = strTemp;
            }
            else if(_tcsicmp(strColumnName, COLUMN_TRANSIT_TYPE) == 0)
            {          
                AutoType iter = m_mapTransitKeyValue.find(COLUMN_TRANSIT_DETAIL_TYPE);
                AutoType end  = m_mapTransitKeyValue.end();

                oKeyValue.TKeyName = strColumnName;
                if (iter != end)
                {
                    CString strDetailType = iter->second;
                    int nMode = KBulkDBaseTransit::GetTransitMode(m_pTarget, strDetailType);

                    CString strMode(_T(""));
                    strMode.Format(_T("%d"), nMode);
                    oKeyValue.TValue = strMode;
                }
                else
                {
                    oKeyValue.TValue   = _T("3");
                }                
            }
            else
            {
                AutoType iter = m_mapTransitKeyValue.find(strColumnName);
                AutoType end  = m_mapTransitKeyValue.end();

                oKeyValue.TKeyName = strColumnName;
                if (iter != end)
                {
                    oKeyValue.TValue = iter->second;
                }
                else
                {
                    oKeyValue.TValue = _T("");
                }                
            }
            vecKeyValue.push_back(oKeyValue);
        }
        
        KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();

        try
        {            
            CString strSQL(_T("")); 

            spDBase->BeginTransaction();
            
            //★ 노선 정보 입력
            KBulkDBaseTransit::Insert(spDBase, vecKeyValue);
            
            //★ 노선-링크 구성 테이블 저장
            if (true)
            {
                strSQL.Format(_T(" Insert Into %s (%s, %s, %s, %s) "), TABLE_TRANSITLINKS, COLUMN_TRANSIT_ID, COLUMN_TRANSIT_LINK_ID, COLUMN_TRANSIT_LINK_SEQ, COLUMN_TRANSIT_LINK_STATION_YN); //★ 필요
                strSQL.Append(_T("  Values(?, ?, ?, ?)"));

                KPreparedPtr spPrepared = spDBase->PrepareStatement(strSQL);
                size_t nxCount = m_vecTransitPath.size();
                for(size_t i=0; i<nxCount; i++) {
                    TTransitPath& oPath = m_vecTransitPath[i];
                    spPrepared->BindInt64(1, nxTransitID);
                    spPrepared->BindInt64(2, oPath.TLinkID);
                    spPrepared->BindInt64(3, i+1);

                    if (i== nxCount-1) { //★ 마지막은 무조건 정류장
                        spPrepared->BindInt(4, 1);
                    }
                    else {
                        spPrepared->BindInt(4, oPath.TStationYN);
                    }

                    spPrepared->ExecuteUpdate();
                    spPrepared->Reset();
                }
            }            
            
            //★ 노선-노드 구성 테이블 저장
            if (true) {
                strSQL.Format(_T(" Insert Into %s (%s, %s, %s, %s) "), TABLE_TRANSITNODES, COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_SEQ, COLUMN_TRANSIT_NODE_ID, COLUMN_TRANSIT_NODE_STATION_YN);
                strSQL.Append(_T("  Values(?, ?, ?, ?)"));

                KPreparedPtr spPrepared = spDBase->PrepareStatement(strSQL);
                size_t nxCount = m_vecTransitPath.size();
                __int64 nxSeq(1);
                for(size_t i=0; i<nxCount; i++) {
                    TTransitPath& oPath = m_vecTransitPath[i];
                    TLinkFTNodeID& oInfo = m_mapLinkFTNode[oPath.TLinkID];
                    
                    //★ 시작 정류장 저장
                    if (i == 0) {
                        spPrepared->BindInt64(1, nxTransitID);
                        spPrepared->BindInt64(2, nxSeq); {
                            nxSeq++;
                        }
                        spPrepared->BindInt64(3, oInfo.TFromNodeID);
                        spPrepared->BindInt(4, 1);

                        //★ 처음 시작 정류장 저장
                        spPrepared->ExecuteUpdate();
                        spPrepared->Reset();
                    }

                    //★ To Node 기준 저장
                    spPrepared->BindInt64(1, nxTransitID);
                    if (i== nxCount-1) { //★ 마지막은 무조건 정류장
                        spPrepared->BindInt64(2, nxSeq); {
                            nxSeq++;
                        }
                        spPrepared->BindInt64(3, oInfo.TToNodeID);
                        spPrepared->BindInt(4, 1);
                    }
                    else {
                        spPrepared->BindInt64(2, nxSeq); {
                            nxSeq++;
                        }
                        spPrepared->BindInt64(3, oInfo.TToNodeID);
                        spPrepared->BindInt(4, oPath.TStationYN);
                    }

                    spPrepared->ExecuteUpdate();
                    spPrepared->Reset();
                }
            }

            spDBase->Commit();
        }
        catch (KExceptionPtr ex)
        {
            if (spDBase)
                spDBase->RollBack();
            TxLogDebugException();
        }
        catch (...)
        {
            if (spDBase)
                spDBase->RollBack();
            TxLogDebugException();
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

    try
    {
        pIOTable->Notify();
    	pIOTable = m_pTarget->Tables()->FindTable(TABLE_TRANSITNODES);
        pIOTable->Notify();
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

void KTransitPathInsertDlg::OnBnClickedButtonReverse()
{
    ReverseTransitPath();
}

void KTransitPathInsertDlg::ReverseTransitPath()
{   
    try
    {
        int nPreCount = (int)m_vecTransitPath.size();

        int nFirstSelectIndex = m_cboReverseSeq.GetCurSel();        
        if (nFirstSelectIndex < 0)
        {
            AfxMessageBox(_T("구간(반대방향 종점)을 선택해 주세요."));
            m_cboReverseSeq.SetFocus();
            return;
        }
        //<LinkID> <FNode-TNode> 
        std::map<Integer, TLinkFTNodeID> mapLinkIDFTNode;
        KBulkDBaseLink::AllLinkFTNode(m_pTarget, mapLinkIDFTNode);

        //<FNode-TNode> <LinkID>
        std::map<KODKey, Integer> mapFTNodeLinkID;
        KBulkDBaseLink::FTLinkMap(m_pTarget, mapFTNodeLinkID);

        AutoType iterLinkIDFTNode = mapLinkIDFTNode.begin();
        AutoType endLinkIDFTNode  = mapLinkIDFTNode.end();

        AutoType iterFTNodeLinkID = mapFTNodeLinkID.begin();
        AutoType endFTNodeLinkID  = mapFTNodeLinkID.end();

        size_t nxCount = m_vecTransitPath.size();
        if (nxCount == 0)
            return;

        std::vector<TTransitPath> vecReverseTransitPath;
        for(int i=(int)nxCount-1; i >= 0; i--)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[i];
            Integer nxLinkID  = oTransitPath.TLinkID;
            // 현재 링크의 FNode, TNode
            iterLinkIDFTNode = mapLinkIDFTNode.find(nxLinkID);
            if (iterLinkIDFTNode == endLinkIDFTNode)
            {
                // 여기는 걸리면 안된다.
                break;
            }

            //대항링크 찾기
            TLinkFTNodeID& oLinkIDFTNode = iterLinkIDFTNode->second;
            Integer nxFromNodeID = oLinkIDFTNode.TFromNodeID;
            Integer nxToNodeID   = oLinkIDFTNode.TToNodeID;
            KODKey oKey(nxToNodeID, nxFromNodeID);

            iterFTNodeLinkID = mapFTNodeLinkID.find(oKey);
            if (iterFTNodeLinkID == endFTNodeLinkID)
            {
                // 대항링크가 없다.
                break;
            }

            Integer nxBackLinkID = iterFTNodeLinkID->second;

            TTransitPath oBackTransitPath;
            oBackTransitPath.TLinkID = nxBackLinkID;
            oBackTransitPath.TStationYN = 0/*GenerateOppositeStation() 함수에서 계산, X-oTransitPath.TStationYN*/;
            oBackTransitPath.TTransitID = oTransitPath.TTransitID;

            vecReverseTransitPath.push_back(oBackTransitPath);

            if (nFirstSelectIndex >= i)
                break;
        }

        size_t nxReverseCount = vecReverseTransitPath.size();
        int nIndex = m_vecTransitPath[nxCount-1].TSeq;

        for (size_t i=0; i<nxReverseCount; i++)
        {
            nIndex++;
            TTransitPath& oBackTransitPath = vecReverseTransitPath[i];
            oBackTransitPath.TSeq = nIndex;

            m_vecTransitPath.push_back(oBackTransitPath);
        }

        GenerateOppositeStation(nPreCount-1);
        TransitPathPopulate();

        if (m_pOpener)
        {
            m_pOpener->TransitPathEditFeedback();
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

int KTransitPathInsertDlg::FirstRecordSelectIndex()
{
    int nFirstSelectIndex = -1;

    try
    {
        CXTPReportRecords*	  pRecords    = NULL;
        CXTPReportRecord*     pRecord     = NULL;
        CXTPReportRecordItem* pItemCheck  = NULL;

        pRecords = m_wndReport.GetRecords();
        int nRecordCount = pRecords->GetCount();


        for (int i=0; i<nRecordCount; i++)
        {
            pRecord    = pRecords->GetAt(i);
            pItemCheck = pRecord->GetItem(0);
            if (pItemCheck->IsChecked() == TRUE)
            {
                nFirstSelectIndex = i;
                break;
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

    return nFirstSelectIndex;
}


void KTransitPathInsertDlg::OnBnClickedCancel()
{
    try
    {
        if (m_pOpener)
        {
            m_pOpener->TransitPathCommand(-1);
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
    KResizeDialogEx::OnCancel();
}

void KTransitPathInsertDlg::OnCbnSelchangeCombo1()
{
    UnCheckedAllSelect();
}

void KTransitPathInsertDlg::UnCheckedAllSelect()
{
    try
    {
        CXTPReportRecords*	  pRecords    = NULL;
        CXTPReportRecord*     pRecord     = NULL;
        CXTPReportRecordItem* pItemCheck  = NULL;

        pRecords = m_wndReport.GetRecords();
        int nRecordCount = pRecords->GetCount();


        for (int i=0; i<nRecordCount; i++)
        {
            pRecord    = pRecords->GetAt(i);
            pItemCheck = pRecord->GetItem(0);

            pItemCheck->SetChecked(FALSE);
        }

        m_wndReport.RedrawControl();
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


void KTransitPathInsertDlg::OnBnClickedRadio1()
{
    try
    {
        m_cboDeleteSeq.EnableWindow(TRUE);
        m_cboDeleteOption.EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);

        m_cboReverseSeq.EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_REVERSE)->EnableWindow(FALSE);	
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

void KTransitPathInsertDlg::OnBnClickedRadio2()
{
    try
    {
        m_cboDeleteSeq.EnableWindow(FALSE);
        m_cboDeleteOption.EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

        m_cboReverseSeq.EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_REVERSE)->EnableWindow(TRUE);
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

void KTransitPathInsertDlg::UpdatePathInfo()
{
    try
    {
        CString strTemp(_T(""));        
    	int nCount  = (int)m_vecTransitPath.size();
        {
            int nCurSel = m_cboReverseSeq.GetCurSel();
            m_cboReverseSeq.ResetContent();

            for (int i=0; i<nCount; i++)
            {            
                //strTemp.Format(_T("%d Seq"), i+1);
				strTemp.Format(_T("%d"), i+1);
                m_cboReverseSeq.AddString(strTemp);
            }           

            if (nCount > 0)
            {
                if(nCount > nCurSel && nCurSel >= 0)
                {
                    m_cboReverseSeq.SetCurSel(nCurSel);
                }
                else
                {
                    m_cboReverseSeq.SetCurSel(0);
                }

                //strTemp.Format(_T("%d Seq"), nCount);
				strTemp.Format(_T("%d"), nCount);
                SetDlgItemText(IDC_EDIT1, strTemp);
            }
            else
            {
                SetDlgItemText(IDC_EDIT1, _T(""));
            }
        }
        {
            int nCurSel = m_cboDeleteSeq.GetCurSel();
            m_cboDeleteSeq.ResetContent();

            for (int i=0; i<nCount; i++)
            {            
                //strTemp.Format(_T("%d Seq"), i+1);
				strTemp.Format(_T("%d"), i+1);
                m_cboDeleteSeq.AddString(strTemp);
            }

            if (nCount > 0)
            {
                if(nCount > nCurSel && nCurSel >= 0)
                {
                    m_cboDeleteSeq.SetCurSel(nCurSel);
                }
                else
                {
                    m_cboDeleteSeq.SetCurSel(0);
                }
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

void KTransitPathInsertDlg::GenerateOppositeStation(int nIndex)
{
    try
    {
        if (IsDlgButtonChecked(IDC_CHECK1) != BST_CHECKED)
            return;

        std::map<Integer, TLinkFTNodeID>::iterator iter = m_mapLinkFTNode.begin();
        std::map<Integer, TLinkFTNodeID>::iterator end  = m_mapLinkFTNode.end();

        std::set<Integer> setStation;
        size_t nxCount = m_vecTransitPath.size();
        if ((size_t)nIndex > nxCount)
            nIndex = (int)nxCount;

        for (size_t i=0; i<=(size_t)nIndex; i++)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[i];
            if (oTransitPath.TStationYN == 1)
            {
                Integer nxLinkID = oTransitPath.TLinkID;                
                iter = m_mapLinkFTNode.find(nxLinkID);
                if (iter != end)
                {
                    TLinkFTNodeID& oLinkFTNodeID = iter->second;
                    setStation.insert(oLinkFTNodeID.TToNodeID);
                }
            }
        }

        std::set<Integer>::iterator iterSet = setStation.begin();
        std::set<Integer>::iterator endSet  = setStation.end();

        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[i];

            Integer nxLinkID = oTransitPath.TLinkID;
            iter = m_mapLinkFTNode.find(nxLinkID);
            if (iter != end)
            {
                TLinkFTNodeID& oLinkFTNodeID = iter->second;
                Integer nxTNodeID = oLinkFTNodeID.TToNodeID;

                iterSet = setStation.find(nxTNodeID);
                if (iterSet != endSet)
                {
                    oTransitPath.TStationYN = 1;
                }
                else
                {
                    oTransitPath.TStationYN = 0;
                }
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

void KTransitPathInsertDlg::OppositeStationStatusChange( Integer a_nxStationID, int a_nStationCode )
{
    try
    {
        std::map<Integer, TLinkFTNodeID>::iterator iter = m_mapLinkFTNode.begin();
        std::map<Integer, TLinkFTNodeID>::iterator end  = m_mapLinkFTNode.end();

        std::set<size_t> setLinkIndex;
        size_t nxCount = m_vecTransitPath.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[i];
            Integer nxLinkID = oTransitPath.TLinkID;                
            iter = m_mapLinkFTNode.find(nxLinkID);
            if (iter != end)
            {
                TLinkFTNodeID& oLinkFTNodeID = iter->second;
                if (oLinkFTNodeID.TToNodeID == a_nxStationID)
                    setLinkIndex.insert(i);
            }
        }

        for (std::set<size_t>::iterator iter=setLinkIndex.begin(); iter != setLinkIndex.end(); ++iter)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[*iter];
            oTransitPath.TStationYN    = a_nStationCode;
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

void KTransitPathInsertDlg::RedrawReportTransitPath()
{
    try
    {
        int nCount = m_wndReport.GetRecords()->GetCount();
        CXTPReportRecords* pRecords = m_wndReport.GetRecords();
        for (int i=0; i<nCount; i++)
        {
            TTransitPath& oTransitPath = m_vecTransitPath[(size_t)i];

            CXTPReportRecord*     pRecord = pRecords->GetAt(i);
            CXTPReportRecordItem* pItem   = pRecord->GetItem(3);

            if (oTransitPath.TStationYN == 1)
                pItem->SetChecked(TRUE);
            else
                pItem->SetChecked(FALSE);

            if (i == nCount-1)
                pItem->SetChecked(TRUE);
        }

        m_wndReport.RedrawControl();
    }
    catch (...)
    {
        TxLogDebugException();
    }	
}

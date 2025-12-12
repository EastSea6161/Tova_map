// TransitPathEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Target.h"
#include "TransitPathViewDlg.h"
#include "afxdialogex.h"
#include "BulkDBaseTransit.h"
#include "ImChampFrameWindow.h"
#include "MapView.h"



IMPLEMENT_DYNAMIC(KTransitPathViewDlg, KResizeDialogEx)

KTransitPathViewDlg::KTransitPathViewDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent)
    : KResizeDialogEx(KTransitPathViewDlg::IDD, pParent), m_nxTransitID(0),
      m_pTransitLinksTable(nullptr), m_pTarget(a_pTarget), m_pMapView(a_pMapView)
{
    try
    {
        m_pTransitLinksTable = m_pTarget->Tables()->FindTable(TABLE_TRANSITLINKS);	//★ 필요
        m_pTransitLinksTable->Register(this);
    }
    catch (KExceptionPtr ex)
    {
        m_pTransitLinksTable = nullptr;
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        m_pTransitLinksTable = nullptr;
    	TxLogDebugException();
    }

    m_spTransitTheme = KThemeTransitPathInfoPtr(new KThemeTransitPathInfo(a_pMapView, true));
}

KTransitPathViewDlg::~KTransitPathViewDlg()
{
    ClearTransitPath();
    try
    {   
        if (m_pTransitLinksTable != nullptr)
            m_pTransitLinksTable->Unregister(this);
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

void KTransitPathViewDlg::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KTransitPathViewDlg, KResizeDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KTransitPathViewDlg::OnReportRowDblClick)
    ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KTransitPathViewDlg::OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KTransitPathViewDlg::OnBnClickedOk)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDCANCEL, &KTransitPathViewDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CHECK1, &KTransitPathViewDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK_SHOW_PATH, &KTransitPathViewDlg::OnBnClickedCheckVisiblePath)
    ON_BN_CLICKED(IDC_BUTTON1, &KTransitPathViewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL KTransitPathViewDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    KBulkDBaseLink::AllLinkFTNode(m_pTarget, m_mapLinkFTNode);
    InitialControl();
    return TRUE;  
}

void KTransitPathViewDlg::InitialControl()
{
    try
    {
        KReportCtrlSetting::Default(m_wndReport);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReport);

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

		SetResize(IDC_STATIC_INFO,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
        SetResize(IDC_REPORT,         SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
        SetResize(IDC_STATIC_HBar1,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
        SetResize(IDC_STATIC_HBar2,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
        SetResize(IDCANCEL,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
        SetResize(IDC_BUTTON1,        SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
        SetResize(IDC_CHECK_SHOW_PATH,SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
        SetResize(IDC_CHECK1,         SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

        CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SHOW_PATH);
        pButton->SetCheck(BST_CHECKED);

        pButton = (CButton*)GetDlgItem(IDC_CHECK1);
        pButton->SetCheck(BST_CHECKED);
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KTransitPathViewDlg::NotifyProcess( LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    try
    {
        KBulkDBaseLink::AllLinkFTNode(m_pTarget, m_mapLinkFTNode);
        TransitPathPopulate();
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

void KTransitPathViewDlg::SetViewTransit(Integer a_nxTransitID )
{
    try
    {
        ClearTransitPath();
        m_nxTransitID = a_nxTransitID;        
        
        CString strCaption;
        //strCaption.Format(_T("::: %s     Transit : %I64d"), m_pTarget->Caption(), a_nxTransitID);
		strCaption.Format(_T("%s     Line ID : %I64d"), m_pTarget->Caption(), a_nxTransitID);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strCaption.Format(_T("%s     노선 ID : %I64d"), m_pTarget->Caption(), a_nxTransitID);
		}
        SetDlgItemText(IDC_STATIC_INFO, strCaption);

        KBulkDBaseTransit::TransitPath(m_pTarget, m_nxTransitID, m_vecTransitPath);
		KBulkDBaseNode::AllNodeFixedValue(m_pTarget->GetDBaseConnection(), m_mapAllNode);
        TransitPathPopulate();
    }
    catch (...)
    {
        TxLogDebugException();
    }	
}

void KTransitPathViewDlg::TransitPathPopulate()
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

            CString strStationID(_T("|"));
            // 정류장여부			
            if (1 == oTransitPath.TStationYN) 
            {
                bCheck = TRUE;
                if (iter != end)
                {
                    TLinkFTNodeID& oTLinkFTNodeID = iter->second;

					CString strNodeName(_T(" "));
					AutoType findNode = m_mapAllNode.find(oTLinkFTNodeID.TToNodeID);
					AutoType endNode  = m_mapAllNode.end();
					if (findNode != endNode)
					{
						strNodeName = findNode->second.strName;
					}


                    if (oTransitPath.TSeq == 1)
                    {
                        //명지대 요구사항으로 삭제
                        //strStationID.Format(_T("%I64d, %I64d"), oTLinkFTNodeID.TFromNodeID, oTLinkFTNodeID.TToNodeID);

                        strStationID.Format(_T("%I64d[%s]"), oTLinkFTNodeID.TToNodeID, strNodeName);
                    }
                    else
                    {
                        strStationID.Format(_T("%I64d[%s]"), oTLinkFTNodeID.TToNodeID, strNodeName);
                    }                    
                }                
            }
            else
            {
                if (oTransitPath.TSeq == 1)
                {
                    //명지대 요구사항으로 삭제
                    //TLinkFTNodeID& oTLinkFTNodeID = iter->second;
                    //strStationID.Format(_T("%I64d"), oTLinkFTNodeID.TFromNodeID);
                    strStationID = _T("");
                }
                bCheck = FALSE;
            }

            pItem   = pRecord->AddItem(new CXTPReportRecordItem);
            pItem->SetEditable(FALSE);
            pItem->SetAlignment(xtpColumnTextCenter);
            pItem->SetCaption(strStationID);

            if (bCheck == TRUE)
            {
                pItem->SetBackgroundColor(RGB(153, 204, 255));
            }
        }

        m_wndReport.Populate();
    }
    catch (...)
    {
        TxLogDebugException();
    }	

    RedrawTransitPath();
}

void KTransitPathViewDlg::RedrawTransitPath()
{
    if (m_spTransitTheme == nullptr)
        return;

    try
    {
        CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1);
        if (pButton->GetCheck() == BST_CHECKED)
        {
            m_spTransitTheme->SetTransitPath(m_nxTransitID, m_vecTransitPath, true);
        }
        else
        {
            m_spTransitTheme->SetTransitPath(m_nxTransitID, m_vecTransitPath, false);
        }        

        m_pMapView->MapRefresh();
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

void KTransitPathViewDlg::OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult )
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

void KTransitPathViewDlg::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{    
    
}

void KTransitPathViewDlg::OnBnClickedCheckVisiblePath()
{
    try
    {
    	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SHOW_PATH);
        if (pButton->GetCheck() == BST_CHECKED)
        {
            RedrawTransitPath();
            pButton = (CButton*)GetDlgItem(IDC_CHECK1);
            pButton->EnableWindow(TRUE);
        }
        else
        {
            ClearTransitPath();
            pButton = (CButton*)GetDlgItem(IDC_CHECK1);
            pButton->EnableWindow(FALSE);
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


void KTransitPathViewDlg::OnBnClickedCheck1()
{
    if (m_spTransitTheme == nullptr)
        return;

    try
    {
        CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1);
        if (pButton->GetCheck() == BST_CHECKED)
        {
            m_spTransitTheme->ShowLabel(true);
        }
        else
        {
            m_spTransitTheme->ShowLabel(false);
        }        

        m_pMapView->MapRefresh();
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


void KTransitPathViewDlg::OnBnClickedOk()
{
    ClearTransitPath();
    KResizeDialogEx::OnOK();
}

void KTransitPathViewDlg::OnBnClickedCancel()
{
    ClearTransitPath();
    KResizeDialogEx::OnCancel();
}


void KTransitPathViewDlg::ClearTransitPath()
{
    try
    {
        m_spTransitTheme->RemoveTransitPath();
        m_pMapView->MapRefresh();
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

void KTransitPathViewDlg::OnBnClickedButton1()
{
    try
    {
        m_pMapView->ZoomTransitExtent(m_nxTransitID);	
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

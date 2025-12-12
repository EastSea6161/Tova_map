// KLinkViaTransitSearch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LinkViaTransitSearchDlg.h"
//^#include "ParseString.h"
#include "Target.h"
#include "MapView.h"
#include "afxdialogex.h"
#include "IOTableView.h"
#include "IOTransitTableView.h"
#include "ImChampFrameWindow.h"
#include "TCoordinate.h"

// KLinkViaTransitSearch 대화 상자입니다.

IMPLEMENT_DYNAMIC(KLinkViaTransitSearchDlg, KResizeDialogEx)

KLinkViaTransitSearchDlg::KLinkViaTransitSearchDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KLinkViaTransitSearchDlg::IDD, pParent)
      , m_pTarget(a_pTarget)
      , m_pMapView(a_pMapView)
{
    m_nType    = 0;
    m_strLinks = _T("");
    m_setSelLink.clear();
}

KLinkViaTransitSearchDlg::~KLinkViaTransitSearchDlg()
{
}

void KLinkViaTransitSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KLinkViaTransitSearchDlg, KResizeDialogEx)
    ON_BN_CLICKED(IDOK, &KLinkViaTransitSearchDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KLinkViaTransitSearchDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_MAP, &KLinkViaTransitSearchDlg::OnBnClickedButtonMap)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KLinkViaTransitSearchDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON1, &KLinkViaTransitSearchDlg::OnBnClickedButton1)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KLinkViaTransitSearchDlg::OnBnClickedRadioType)
END_MESSAGE_MAP()


// KLinkViaTransitSearch 메시지 처리기입니다.
BOOL KLinkViaTransitSearchDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
    SetDlgItemText(IDC_EDIT1, m_strPreSelectionInfo);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KLinkViaTransitSearchDlg::OnBnClickedOk()
{
    if ( GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1 )
    {
        m_nType = 1;
    }
    else
    {
        m_nType = 2;
    }

    GetDlgItemText(IDC_EDIT1, m_strLinks);
    m_strLinks.Trim();
    
    int nLength = m_strLinks.GetLength();

    if (nLength == 0)
    {
        m_setSelLink.clear();
        SetDlgItemText(IDC_EDIT1, _T(""));
        GetDlgItem(IDC_EDIT1)->SetFocus();
        return;
    }

    int nLastComma = m_strLinks.Find(_T(","), nLength-1);
    if (nLastComma == nLength-1)
    {
        m_strLinks = m_strLinks.Left(nLength-1);
        SetDlgItemText(IDC_EDIT1, m_strLinks);
    }

    std::vector<CString> vecLink = KParseString::ParseString(m_strLinks, _T(','));
    size_t nxCount = vecLink.size();
    if (nxCount == 0)
    {
        m_setSelLink.clear();
        SetDlgItemText(IDC_EDIT1, _T(""));
        GetDlgItem(IDC_EDIT1)->SetFocus();
        return;
    }

    m_strLinks = _T("");

    for (size_t i=0; i<nxCount; i++)
    {
        CString strTemp = vecLink[i];
        strTemp.Trim();
        if (strTemp.GetLength() == 0)
        {
            GetDlgItem(IDC_EDIT1)->SetFocus();
            return;
        }

        if (i==0)
        {
            m_strLinks.Append(strTemp);
        }
        else
        {
            m_strLinks.Append(_T(","));
            m_strLinks.Append(strTemp);
        }
    }

    TransitLinkViaTransit();
    //KResizeDialogEx::OnOK();
}

void KLinkViaTransitSearchDlg::TransitLinkViaTransit()
{
    try
    {   
        int nCount(0);
        if (m_nType == 1)
        {
            std::vector<CString> vecLink = KParseString::ParseString(m_strLinks, _T(','));
            size_t nxCount = vecLink.size();
            nCount = (int)nxCount;
        }

        KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TRANSIT);
        if (pTableView == nullptr)
            return;

        CString strFilter;
        strFilter.Format(
            _T(" transit_id in ")
            _T(" ( ")
            _T("   Select Transit_ID ")
            _T("   From ")
            _T("   (    ")
            _T("      Select Transit_ID, count(*) as Cnt From Transit_Links Where Link_ID in (%s) ")
            _T("      Group by Transit_ID ")
            _T("   )    ")
            _T("   Where Cnt >= %d ")
            _T(" ) "), m_strLinks, nCount);        

        pTableView->SetFilter(strFilter);
        pTableView->ReloadData();
        KIOTransitTableView* pTransitTableView = dynamic_cast<KIOTransitTableView*>(pTableView);
        if (pTransitTableView == nullptr)
            return;
        pTransitTableView->OnTransitSelectAll();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }   

    m_strPreSelectionInfo = m_strLinks;
}

void KLinkViaTransitSearchDlg::OnBnClickedCancel()
{
    try
    {
    	ImChampFrameWindow::GetMapView(m_pTarget)->ClearScreenDrawEdit();
        m_pMapView->RefreshAll();
		SetDlgItemText(IDC_EDIT1, _T(""));
        m_setSelLink.clear();
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

void KLinkViaTransitSearchDlg::ActivatedFrameWndNotifyProcess()
{

}

void KLinkViaTransitSearchDlg::DeActivatedFrameWndNotifyProcess()
{

}

void KLinkViaTransitSearchDlg::AddLinkID( Integer a_nxLinkID )
{
    try
    {
        GetDlgItemText(IDC_EDIT1, m_strLinks);
        m_strLinks.Trim();

        // 이미 선택한 링크인지 검사
        if (m_setSelLink.find(a_nxLinkID) == m_setSelLink.end()) {
            m_setSelLink.insert(a_nxLinkID);

            CString strAddLinkID(_T(""));

            if (m_strLinks.GetLength() > 0)
            {
                strAddLinkID.Format(_T(",%I64d"), a_nxLinkID);
            }
            else
            {
                strAddLinkID.Format(_T("%I64d"), a_nxLinkID);
            }

            m_strLinks += strAddLinkID;
        }

        SetDlgItemText(IDC_EDIT1, m_strLinks);

        OnBnClickedOk();
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

void KLinkViaTransitSearchDlg::AddLinkID(std::vector<Integer> &a_vecLinkID)
{
    try
    {

        GetDlgItemText(IDC_EDIT1, m_strLinks);
        m_strLinks.Trim();

        int nAddIdx(0);
        CString strAddLinkID(_T(""));
        for (size_t i = 0; i < a_vecLinkID.size(); i++)
        {
            // 이미 선택한 링크인지 검사
            const Integer &nxLinkID = a_vecLinkID[i];

            if (m_setSelLink.find(nxLinkID) == m_setSelLink.end()) {
                m_setSelLink.insert(nxLinkID);

                if (0 == nAddIdx) {
                    strAddLinkID.AppendFormat(_T("%I64d"), nxLinkID);
                }
                else {
                    strAddLinkID.AppendFormat(_T(",%I64d"), nxLinkID);
                }

                nAddIdx++;
            }
        }

        if (strAddLinkID.IsEmpty() == false) {

            if (m_strLinks.GetLength() > 0)
            {
                m_strLinks.Append(_T(","));
                m_strLinks.Append(strAddLinkID);
            }
            else
            {
                m_strLinks.Append(strAddLinkID);
            }
        }

        SetDlgItemText(IDC_EDIT1, m_strLinks);

        OnBnClickedOk();
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

void KLinkViaTransitSearchDlg::OnBnClickedButtonMap()
{
    try
    {
        m_pMapView->ClearScreenDrawEdit(false);
        m_pMapView->MapRefresh();

        ImChampFrameWindow::GetMapView(m_pTarget)->SearchTransitByLink(true);
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

void KLinkViaTransitSearchDlg::OnBnClickedButtonClear()
{
    SetDlgItemText(IDC_EDIT1, _T(""));
    m_setSelLink.clear();
}

CString KLinkViaTransitSearchDlg::m_strPreSelectionInfo = _T("");

void KLinkViaTransitSearchDlg::OnBnClickedButton1()
{
    try
    {
        m_pMapView->TxClearCompositeThemes(false);
        m_pMapView->ClearScreenDrawEdit(true);

//         std::map<Integer, std::set<Integer>>	mapStationLineInfo;
//         std::map<Integer, int>					mapStationModeInfo;
//         KDBaseAccessibilityModify::TransitStationNodeViaInfo(m_pTarget, mapStationLineInfo, mapStationModeInfo);
// 
//         std::vector<TDrawNodeInfo> vecDrawTotalNode;
// 
//         AutoType iter = mapStationModeInfo.begin();
//         AutoType end = mapStationModeInfo.end();
//         while (iter != end)
//         {
//             Integer nxNodeID = iter->first;
// 
//             TDrawNodeInfo oNodeInfo;
//             oNodeInfo.TNodeID = nxNodeID;
// 
//             CString strNodeLable(_T(""));
//             strNodeLable.Format(_T("%I64d"), nxNodeID);
//             oNodeInfo.TLabel = strNodeLable;
// 
//             oNodeInfo.TSymbol = 48;
//             oNodeInfo.TSize = 12;
//             oNodeInfo.TColorRGB = RGB(0, 0, 255);
//             oNodeInfo.TColorAlpha = 80;
//             // KScreenDrawFlashNode::SetSelectable의 스타일과 동일해야 한다.
// 
//             vecDrawTotalNode.push_back(oNodeInfo);
// 
//             ++iter;
//         }
//         m_pMapView->TxDrawCompositeNode(vecDrawTotalNode, false, true);

        m_pMapView->TargetRegionInsert(0, this, LINK);
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

void KLinkViaTransitSearchDlg::TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate)
{
    try
    {
        m_pMapView->ClearScreenDrawEdit(false);
        //m_pMapView->TxClearCompositeThemes(false);
        m_pMapView->RefreshAll();

        AddLinkID(a_vecSelection);
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

void KLinkViaTransitSearchDlg::OnBnClickedRadioType(UINT nID)
{
    OnBnClickedOk();
}
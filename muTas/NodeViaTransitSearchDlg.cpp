// NodeViaTransitSearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NodeViaTransitSearchDlg.h"
//^#include "ParseString.h"
#include "Target.h"
#include "MapView.h"
#include "afxdialogex.h"
#include "IOTableView.h"
#include "IOTransitTableView.h"
#include "ImChampFrameWindow.h"
#include "KDBaseAccessibilityModify.h"
#include "TCoordinate.h"

// KLinkViaTransitSearch.cpp : 구현 파일입니다.

IMPLEMENT_DYNAMIC(KNodeViaTransitSearchDlg, KResizeDialogEx)

KNodeViaTransitSearchDlg::KNodeViaTransitSearchDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
                        : KResizeDialogEx(KNodeViaTransitSearchDlg::IDD, pParent)
                          ,m_pTarget(a_pTarget)
                          , m_pMapView(a_pMapView)

{
    m_nType    = 0;
    m_strNodes = _T("");
    m_setSelNode.clear();
}

KNodeViaTransitSearchDlg::~KNodeViaTransitSearchDlg()
{
}

void KNodeViaTransitSearchDlg::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KNodeViaTransitSearchDlg, KResizeDialogEx)
    ON_BN_CLICKED(IDOK, &KNodeViaTransitSearchDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KNodeViaTransitSearchDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_MAP, &KNodeViaTransitSearchDlg::OnBnClickedButtonMap)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KNodeViaTransitSearchDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON1, &KNodeViaTransitSearchDlg::OnBnClickedButton1)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KNodeViaTransitSearchDlg::OnBnClickedRadioType)
END_MESSAGE_MAP()


// KLinkViaTransitSearch 메시지 처리기입니다.
BOOL KNodeViaTransitSearchDlg::OnInitDialog()
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


void KNodeViaTransitSearchDlg::OnBnClickedOk()
{
    if ( GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1 )
    {
        m_nType = 1;
    }
    else
    {
        m_nType = 2;
    }

    GetDlgItemText(IDC_EDIT1, m_strNodes);
    m_strNodes.Trim();

    int nLength = m_strNodes.GetLength();

    if (nLength == 0)
    {
        m_setSelNode.clear();
        SetDlgItemText(IDC_EDIT1, _T(""));
        GetDlgItem(IDC_EDIT1)->SetFocus();
        return;
    }

    int nLastComma = m_strNodes.Find(_T(","), nLength-1);
    if (nLastComma == nLength-1)
    {
        m_strNodes = m_strNodes.Left(nLength-1);
        SetDlgItemText(IDC_EDIT1, m_strNodes);
    }

    std::vector<CString> vecLink = KParseString::ParseString(m_strNodes, _T(','));
    size_t nxCount = vecLink.size();
    if (nxCount == 0)
    {
        m_setSelNode.clear();
        SetDlgItemText(IDC_EDIT1, _T(""));
        GetDlgItem(IDC_EDIT1)->SetFocus();
        return;
    }

    m_strNodes = _T("");

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
            m_strNodes.Append(strTemp);
        }
        else
        {
            m_strNodes.Append(_T(","));
            m_strNodes.Append(strTemp);
        }
    }

    TransitNodeViaTransit();
}

void KNodeViaTransitSearchDlg::TransitNodeViaTransit()
{
    try
    {   
        std::map<Integer, std::set<Integer>>	mapStationLineInfo;
        std::map<Integer, int>					mapStationModeInfo;
        KDBaseAccessibilityModify::TransitStationNodeViaInfo(m_pTarget, mapStationLineInfo, mapStationModeInfo);

        std::vector<CString> vecNode = KParseString::ParseString(m_strNodes, _T(','));

        KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TRANSIT);
        if (pTableView == nullptr)
            return;
        
        std::map<Integer, int> mapTransitIdCount;
        for (size_t i=0; i<vecNode.size(); i++)
        {
            AutoType iterStationLineInfo = mapStationLineInfo.find(_ttoi64(vecNode[i]));
            if (iterStationLineInfo != mapStationLineInfo.end())
            {
                std::set<Integer>& setTransitID = iterStationLineInfo->second;
                AutoType iterSetTransitID = setTransitID.begin();
                AutoType endSetTransitID  = setTransitID.end();

                while (iterSetTransitID != endSetTransitID)
                {
                    Integer  nxTransitID = *iterSetTransitID;

                    AutoType iter = mapTransitIdCount.find(nxTransitID);
                    AutoType end  = mapTransitIdCount.end();

                    if (iter != end)
                    {
                        int& nCount = iter->second;
                        nCount++;
                    }
                    else
                    {
                        int nCount(1);
                        mapTransitIdCount.insert(std::make_pair(nxTransitID, nCount));
                    }
                    ++iterSetTransitID;
                }
            }
        }

        CString strFilter(_T(""));
                strFilter.Format(_T(" transit_id in ( "));

        int nIndex = 0;
        int nCount = (int)vecNode.size();
        AutoType iter = mapTransitIdCount.begin();
        AutoType end  = mapTransitIdCount.end();

        while (iter != end)
        {
            Integer nxTransitID = iter->first;
            int     nPassCount  = iter->second;
            if (m_nType == 1)
            {
                if (nPassCount>=nCount)
                {
                    if (nIndex == 0)
                    {
                        strFilter.AppendFormat(_T(" %I64d "), *iter);
                    }
                    else
                    {
                        strFilter.AppendFormat(_T(" ,%I64d "), *iter);
                    }
                    nIndex++;
                }
            }
            else
            {
                if (nIndex == 0)
                {
                    strFilter.AppendFormat(_T(" %I64d "), *iter);
                }
                else
                {
                    strFilter.AppendFormat(_T(" ,%I64d "), *iter);
                }
                nIndex++;
            }
            ++iter;
        }
        strFilter.AppendFormat(_T(" ) "));

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

    m_strPreSelectionInfo = m_strNodes;
}

void KNodeViaTransitSearchDlg::OnBnClickedCancel()
{
    try
    {
        m_pMapView->TxClearCompositeThemes(false);
        ImChampFrameWindow::GetMapView(m_pTarget)->ClearScreenDrawEdit();
        m_pMapView->RefreshAll();
		SetDlgItemText(IDC_EDIT1, _T(""));
        m_setSelNode.clear();
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

void KNodeViaTransitSearchDlg::ActivatedFrameWndNotifyProcess()
{

}

void KNodeViaTransitSearchDlg::DeActivatedFrameWndNotifyProcess()
{

}

void KNodeViaTransitSearchDlg::AddNodeID( Integer a_nxNodeID )
{
    try
    {

        GetDlgItemText(IDC_EDIT1, m_strNodes);
        m_strNodes.Trim();

        // 이미 선택한 노드인지 검사
        if (m_setSelNode.find(a_nxNodeID) == m_setSelNode.end()) {
            m_setSelNode.insert(a_nxNodeID);

            CString strAddNodeID(_T(""));

            if (m_strNodes.GetLength() > 0)
            {
                strAddNodeID.Format(_T(",%I64d"), a_nxNodeID);
            }
            else
            {
                strAddNodeID.Format(_T("%I64d"), a_nxNodeID);
            }

            m_strNodes += strAddNodeID;
        }

        SetDlgItemText(IDC_EDIT1, m_strNodes);

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

void KNodeViaTransitSearchDlg::AddNodeID(std::vector<Integer> &a_vecNodeID)
{
    try
    {

        GetDlgItemText(IDC_EDIT1, m_strNodes);
        m_strNodes.Trim();

        int nAddIdx(0);
        CString strAddNodeID(_T(""));
        for (size_t i = 0; i < a_vecNodeID.size(); i++)
        {
            // 이미 선택한 노드인지 검사
            const Integer &nxNodeID = a_vecNodeID[i];
            
            if (m_setSelNode.find(nxNodeID) == m_setSelNode.end()) {
                m_setSelNode.insert(nxNodeID);

                if (0 == nAddIdx) {
                    strAddNodeID.AppendFormat(_T("%I64d"), nxNodeID);
                }
                else {
                    strAddNodeID.AppendFormat(_T(",%I64d"), nxNodeID);
                }

                nAddIdx++;
            }
        }

        if (strAddNodeID.IsEmpty() == false) {

            if (m_strNodes.GetLength() > 0)
            {
                m_strNodes.Append(_T(","));
                m_strNodes.Append(strAddNodeID);
            }
            else
            {
                m_strNodes.Append(strAddNodeID);
            }
        }

        SetDlgItemText(IDC_EDIT1, m_strNodes);

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

void KNodeViaTransitSearchDlg::OnBnClickedButtonMap()
{
    try
    {
        m_pMapView->TxClearCompositeThemes(false);
        m_pMapView->ClearScreenDrawEdit(false);
        m_pMapView->MapRefresh();

        ImChampFrameWindow::GetMapView(m_pTarget)->SearchTransitByNode(true);
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

void KNodeViaTransitSearchDlg::OnBnClickedButtonClear()
{
    SetDlgItemText(IDC_EDIT1, _T(""));
    m_setSelNode.clear();
}

CString KNodeViaTransitSearchDlg::m_strPreSelectionInfo = _T("");



void KNodeViaTransitSearchDlg::OnBnClickedButton1()
{
    try
    {
        m_pMapView->TxClearCompositeThemes(false);
        m_pMapView->ClearScreenDrawEdit(false);

        std::map<Integer, std::set<Integer>>	mapStationLineInfo;
        std::map<Integer, int>					mapStationModeInfo;
        KDBaseAccessibilityModify::TransitStationNodeViaInfo(m_pTarget, mapStationLineInfo, mapStationModeInfo);
        
        std::vector<TDrawNodeInfo> vecDrawTotalNode;

        AutoType iter = mapStationModeInfo.begin();
        AutoType end  = mapStationModeInfo.end();
        while (iter != end)
        {
            Integer nxNodeID = iter->first;

            TDrawNodeInfo oNodeInfo;
            oNodeInfo.TNodeID = nxNodeID;

            CString strNodeLable(_T(""));
            strNodeLable.Format(_T("%I64d"), nxNodeID);
            oNodeInfo.TLabel = strNodeLable;

            oNodeInfo.TSymbol = 48;
            oNodeInfo.TSize = 12;
            oNodeInfo.TColorRGB = RGB(0, 0, 255);
            oNodeInfo.TColorAlpha = 80;
            // KScreenDrawFlashNode::SetSelectable의 스타일과 동일해야 한다.

            vecDrawTotalNode.push_back(oNodeInfo);

            ++iter;
        }

        m_pMapView->TxDrawCompositeNode(vecDrawTotalNode, false, true);
        m_pMapView->TargetRegionInsert(0, this);
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

void KNodeViaTransitSearchDlg::TargetRegionFeedback(std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate)
{
    try
    {
        m_pMapView->ClearScreenDrawEdit(false);
        //m_pMapView->TxClearCompositeThemes(false);
        m_pMapView->RefreshAll();

        AddNodeID(a_vecSelection);
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

void KNodeViaTransitSearchDlg::OnBnClickedRadioType(UINT nID)
{
    OnBnClickedOk();
}
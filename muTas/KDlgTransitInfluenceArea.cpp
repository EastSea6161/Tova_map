#include "stdafx.h"
#include "KDlgTransitInfluenceArea.h"
#include "MapView.h"
#include "Target.h"

IMPLEMENT_DYNAMIC(KDlgTransitInfluenceArea, KResizeDialogEx)
BEGIN_MESSAGE_MAP(KDlgTransitInfluenceArea, KResizeDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &KDlgTransitInfluenceArea::OnBnClickedButton1)
    ON_BN_CLICKED(IDOK, &KDlgTransitInfluenceArea::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON2, &KDlgTransitInfluenceArea::OnBnClickedClear)
    ON_BN_CLICKED(IDCANCEL, &KDlgTransitInfluenceArea::OnBnClickedCancel)
END_MESSAGE_MAP()

KDlgTransitInfluenceArea::KDlgTransitInfluenceArea(KMapView* pMapView, CWnd* pParent) : KResizeDialogEx(KDlgTransitInfluenceArea::IDD, pParent)
{
    m_pMapView = pMapView;
}

KDlgTransitInfluenceArea::~KDlgTransitInfluenceArea()
{

}

void KDlgTransitInfluenceArea::DoDataExchange( CDataExchange* pDX )
{
    KResizeDialogEx::DoDataExchange(pDX);
}

BOOL KDlgTransitInfluenceArea::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
    KResizeDialogEx::UseKeyEscEnter(true, true);

    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
    SetIcon( hIcon, TRUE );
    SetIcon( hIcon, FALSE );

    SetDlgItemText(IDC_EDIT2, _T("0.5"));
    KTarget* pTarget = m_pMapView->GetTarget();
    KBulkDBaseTransit::TransitAllIdCaptionType(pTarget, m_vecLineIDCaption, m_mapLineData);

    return TRUE; 
}

void KDlgTransitInfluenceArea::OnBnClickedButton1()
{
    try
    {
        CWaitCursor cw;

        CString strSelect(_T("")); 
        GetDlgItemText(IDC_EDIT1, strSelect);

        m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
        m_spImTasMultiIDFind->SetModelessReceiver(this);

        m_spImTasMultiIDFind->SetIDCaption(m_vecLineIDCaption);

        m_spImTasMultiIDFind->SetSelectedID(strSelect);
        m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
        m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
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
void KDlgTransitInfluenceArea::ModelessOKFeedback( UINT nDialogID )
{
    if (KImChampMultiIDFind::IDD == nDialogID)
    {
        if (m_spImTasMultiIDFind)
        {
            CString strIDs = m_spImTasMultiIDFind->GetSelectedID();
            SetDlgItemText(IDC_EDIT1, strIDs);

            std::set<CString> setID  = m_spImTasMultiIDFind->GetSelectedIDSet();            
        }
    }
}

void KDlgTransitInfluenceArea::ModelessCancleFeedback( UINT nDialogID )
{

}

void KDlgTransitInfluenceArea::ActivatedFrameWndNotifyProcess()
{
    try
    {
        if(nullptr != m_spImTasMultiIDFind)
        {
            if(!m_spImTasMultiIDFind->IsUserClosed())
            {
                if(!m_spImTasMultiIDFind->IsWindowVisible())
                    m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
            }
        }
    }
    catch (...)
    {
        TxLogDebugException();	
    }
}

void KDlgTransitInfluenceArea::DeActivatedFrameWndNotifyProcess()
{
    try
    {
        if(nullptr != m_spImTasMultiIDFind)
        {
            if(!m_spImTasMultiIDFind->IsUserClosed())
            {
                if(m_spImTasMultiIDFind->IsWindowVisible())
                    m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
            }
        }
    }
    catch (...)
    {
        TxLogDebugException();	
    }
}


void KDlgTransitInfluenceArea::OnBnClickedOk()
{
    CString strSelect(_T("")); {
        GetDlgItemText(IDC_EDIT1, strSelect);
    }    

    std::vector<CString> vecID = QBicSplit::Split(strSelect, _T(','));
    if (vecID.size() == 0) {
        AfxMessageBox(_T("노선을 선택해 주세요"));
        return;
    }

    double dRadius(0); {
        CString strRadius(_T(""));
        GetDlgItemText(IDC_EDIT2, strRadius);

        dRadius = _ttof(strRadius);
    }
    
    if (dRadius <= 0) {
        AfxMessageBox(_T("반경 정보를 선택해 주세요"));
        CWnd* pWnd = GetDlgItem(IDC_EDIT2);
        if (pWnd) {
            pWnd->SetFocus();
        }

        return;
    }

    Draw(vecID, dRadius * 1000);
}

void KDlgTransitInfluenceArea::Draw( std::vector<CString>& vecID, double dRadius )
{
    CString strTransit(_T(""));
    for (size_t i=0; i<vecID.size(); i++) {
        CString strID = vecID[i];

        if (i == 0) {
            strTransit.AppendFormat(_T("'%s'"), strID);
        }
        else {
            strTransit.AppendFormat(_T(", '%s'"), strID);
        }
    }

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  T1.Link_ID, T1.Seq, T1.Station_YN, T2.fnode_id, T2.tnode_id "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Link_ID, Seq, Station_YN "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Transit_Links "));
        strSQL.Append(_T(" Where ")); 
        strSQL.AppendFormat(_T(" Transit_ID In (%s) "), strTransit);
        strSQL.Append(_T(" ) T1, Link T2 "));
        strSQL.Append(_T(" Where "));
        strSQL.Append(_T(" T1.Link_ID = T2.Link_ID "));
    }

    std::set<__int64> setID;

    KTarget* pTarget = m_pMapView->GetTarget();
    KDBaseConPtr  spDBase  = pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

    while (spResult->Next()) {
        __int64 nxLinkID  = spResult->GetValueI64(0);
        int     nSeq      = spResult->GetValueInt(1);
        int     nFlag     = spResult->GetValueInt(2);
        __int64 nxFNodeID = spResult->GetValueI64(3);
        __int64 nxTNodeID = spResult->GetValueI64(4);

        if (nSeq == 1) {
            setID.insert(nxFNodeID);
        }

        if (nFlag == 1) {
            setID.insert(nxTNodeID);
        }
    }

    int nLayerID = KLayerID::LayerID_TransitInfluenceArea();
    
    ITxFeatureLayerPtr spLayerNode = m_pMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spLayerNode == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(nLayerID);
    if (spLayer == nullptr && setID.size() == 0)
        return;

    if (spLayer == nullptr)
        spLayer = m_pMapView->AddUserLayer(nLayerID);

    if (spLayer == nullptr)
        return;

    spLayer->Delete();

    TxSymbolPointCirclePtr spSymbol(new TxSymbolPointCircle(dRadius, Gdiplus::Color::White, Gdiplus::Color::Black, 1)); {
        TxHatchInfoPtr spHatch  = TxHatchInfoPtr(new TxHatchInfo(HatchStyleBackwardDiagonal, Gdiplus::Color::Red));       
        spSymbol->SetHatch(spHatch);
        spSymbol->Fill(true);
    }            
    spSymbol->UsePixelDist(false);
    spLayer->LayerDispSymbol(spSymbol);

    for (auto iter = setID.begin(); iter != setID.end(); ++iter) {
        __int64 nxID = *iter;

        ITxGeometryPtr spGeometry = spLayerNode->GetGeometry(nxID);
        if (spGeometry == nullptr)
            continue;

        spLayer->Insert(nxID, spGeometry);
    }

    m_pMapView->MapRefresh();
}

void KDlgTransitInfluenceArea::OnBnClickedClear()
{
    int nLayerID = KLayerID::LayerID_TransitInfluenceArea();
    ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(nLayerID);
    
    if (spLayer != nullptr) {
        m_pMapView->MapRemoveLayer(nLayerID);
        m_pMapView->MapRefresh();
    }
}


void KDlgTransitInfluenceArea::OnBnClickedCancel()
{
    OnBnClickedClear();
    KResizeDialogEx::OnCancel();
}

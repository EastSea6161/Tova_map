#include "stdafx.h"
#include "ThunderMap.h"
#include "ThunderMapCommandPan.h"
#include "ThunderMapCommandZoomIn.h"
#include "ThunderMapCommandZoomOut.h"
#include "ThunderMapCommandWheel.h"
#include "TxLayerInfo.h"
#include "ITxLayer.h"
#include "TxFeatureUserLayer.h"
#include "TxTooltipView.h"

ThunderMap::ThunderMap() : IThunderMap()
{
    TESTID = 0;
    m_bEnableTooltip = false;
    m_spMapLayerManager = TxMapLayerManagerPtr(new TxMapLayerManager());
    m_hwnd = nullptr;
	m_crBackground = Gdiplus::Color::White;
    try {
        ::CreateDirectory(_T(".\\TMSCache"), NULL);
    } catch (...) {
    }
}

ThunderMap::~ThunderMap()
{
    m_bEnableTooltip = false;

    try {
        m_hwnd = nullptr;
    }catch(...) {
    }    
}

ThunderMapdrawProcessPtr ThunderMap::GetThunderDxDraw()
{
    return m_spThunderDraw;
}

void ThunderMap::SetWindow( HWND hwnd )
{
    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hwnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr) {
        m_hwnd = nullptr;
        return;
    }

    m_hwnd = hwnd;

    RECT rc;
    GetClientRect(hwnd, &rc);

    m_spThunderDraw = ThunderMapdrawProcessPtr(new ThunderMapdrawProcess(hwnd));
    m_spThunderDraw->SetLayerManager(m_spMapLayerManager);

	CClientDC oDC(CWnd::FromHandle(m_hwnd));
	
	double dWidthRes  = (double)oDC.GetDeviceCaps(HORZRES);
	double dHeightRes = (double)oDC.GetDeviceCaps(VERTRES);
	double dWidth     = (double)oDC.GetDeviceCaps(HORZSIZE) * 0.001;
	double dHeight    = (double)oDC.GetDeviceCaps(VERTSIZE) * 0.001;

	double dMonitorPixelDist = ((dWidth / dWidthRes) + (dHeight / dHeightRes)) * 0.5;

    if (m_spThunderMapDisp == nullptr)
        m_spThunderMapDisp = ThunderMapDispPtr(new ThunderMapDisp(dMonitorPixelDist));

    m_spThunderMapDisp->SetScreenRect(TxEnvelope(rc));
    m_spThunderMapDisp->SetMapRect(TxEnvelope(0, 0, 1000000, 1000000), false);
    
    // 디폴트 명령
    m_spMapCommand = IThunderMapCommandPtr(new ThunderMapCommandPan(this, m_crBackground));
}

EnumTxMapCommand ThunderMap::MapCommand()
{
    if (m_spMapCommand)
        return m_spMapCommand->ThunderMapCommand();

    return MapUserCommand;
}

void ThunderMap::MapCommand( EnumTxMapCommand enumMapCommand )
{
    switch (enumMapCommand)
    {
        case ZoomInCommand :
            m_spMapCommand = IThunderMapCommandPtr(new ThunderMapCommandZoomIn(this, m_crBackground));
            break;
        case PanCommand :
            m_spMapCommand = IThunderMapCommandPtr(new ThunderMapCommandPan(this, m_crBackground));
            break;
        case ZoomOutCommand:
            m_spMapCommand = IThunderMapCommandPtr(new ThunderMapCommandZoomOut(this, m_crBackground));
            break;
        default:
            m_spMapCommand.reset();
            break;
    }    
}

void ThunderMap::Draw()
{
    if (m_spMapCommand) {
        if (m_spMapCommand->IsCommandRun() == true) {            
            return;
        }
    }

    if (m_spThunderDraw) {        
        m_spThunderDraw->Draw(m_spThunderMapDisp);
    }
}

void ThunderMap::ClearForegroundLayer()
{
    m_mapForeLayer.clear();
    m_mapFixForeLayer.clear();
}

void ThunderMap::RemoveForegroundLayer( int nZorder )
{
    if(true) {
        auto iter = m_mapForeLayer.find(nZorder);
        if (iter != m_mapForeLayer.end())
            m_mapForeLayer.erase(iter);
    }
    
    if(true) {
        auto iter = m_mapFixForeLayer.find(nZorder);
        if (iter != m_mapFixForeLayer.end())
            m_mapFixForeLayer.erase(iter);
    }
}

TxFeatureUserLayerPtr ThunderMap::AddForegroundLayer( std::vector<TxFeatureDispPtr>& _vecFeature, bool bShowLabel, double dLabelMaxScale )
{   
    TxLayerInfoPtr        spLayerInfo(new TxLayerInfo(GeoShapeType, _T("FlashLayer"), _T("FlashLayer")));
    TxFeatureUserLayerPtr spLayer(new TxFeatureUserLayer(spLayerInfo)); {
        spLayer->LayerOn(true);
        spLayer->ShowLabel(bShowLabel);
        spLayer->DrawLabelMaxScale(dLabelMaxScale);
        spLayer->PutFeatureDisp(_vecFeature);
    }    
    
    int  nLayerID(10000);
    auto iter = m_mapForeLayer.rbegin();
    if (iter != m_mapForeLayer.rend()) {
        int nTemp = iter->first;
        if (nLayerID < nTemp) {
            nLayerID = nTemp;
        }
    }

    nLayerID++;
    m_mapForeLayer[nLayerID] = spLayer;

    return spLayer;
}

TxFeatureUserLayerPtr ThunderMap::AddForegroundLayer( TxFeatureDispPtr _spFeature, bool bShowLabel, double dLabelMaxScale)
{
    std::vector<TxFeatureDispPtr> vecFeature; {
        vecFeature.push_back(_spFeature);
    }    

    return AddForegroundLayer(vecFeature, bShowLabel, dLabelMaxScale);
}

void ThunderMap::SetForegroundLayer(ITxFeatureUserLayerPtr spLayer, int nZOrder, bool bFlash )
{
    m_mapForeLayer[nZOrder] = spLayer;

    if (bFlash == false) {
        m_mapFixForeLayer[nZOrder] = spLayer;
    }
}

void ThunderMap::ForegroundRefresh()
{
    if (m_spMapCommand) {
        if (m_spMapCommand->IsCommandRun()) {
            m_mapForeLayer.clear();
            m_mapForeLayer = m_mapFixForeLayer;
            return;
        }
    }

    if (m_spThunderDraw == nullptr)
        return;

    if (m_spThunderDraw->GetMapDrawStatus() != 2) {
        m_mapForeLayer.clear();
        m_mapForeLayer = m_mapFixForeLayer;
        return;
    }

    std::vector<ITxLayerPtr> vecLayer;
    for (auto iter = m_mapForeLayer.begin(); iter != m_mapForeLayer.end(); ++iter) {
        vecLayer.push_back(iter->second);
    }
    m_spThunderDraw->ForegroundRefresh(vecLayer, m_spThunderMapDisp);
    m_mapForeLayer.clear();
    m_mapForeLayer = m_mapFixForeLayer;
}

void ThunderMap::Flash( std::vector<TxFeatureDispPtr>& vecFeatureDisp, int nTimeInterval, int nCount)
{
    if (m_spThunderDraw == nullptr)
        return;

    if (vecFeatureDisp.size() < 1)
        return;
            
    m_spThunderDraw->Flash(vecFeatureDisp, nTimeInterval, nCount);
}

int ThunderMap::AddLayer( ITxLayerPtr spLayer )
{
    //★ 외부에서 레이어를 추가한다.
    int nLayerID = m_spMapLayerManager->MaxLayerID(); {
        nLayerID++;
    }
    return m_spMapLayerManager->AddLayer(spLayer, nLayerID);
}

int ThunderMap::AddLayer( ITxLayerPtr spLayer, int nLayerID, int nLayerOrder )
{
    return m_spMapLayerManager->AddLayer(spLayer, nLayerID, nLayerOrder);
}

ITxLayerPtr ThunderMap::GetLayer( int nLayerID )
{
    return m_spMapLayerManager->GetLayer(nLayerID);
}

void ThunderMap::RemoveLayer( int nLayerID )
{
    m_spMapLayerManager->RemoveLayer(nLayerID);
}


void ThunderMap::MoveLayerOrder( int nLayerID, int nLayerOrder/*=0*/ )
{
    m_spMapLayerManager->MoveLayerOrder(nLayerID, nLayerOrder);
}


void ThunderMap::LayerOrderByID()
{
    m_spMapLayerManager->LayerOrderByID();
}

void ThunderMap::Resize( UINT width, UINT height )
{
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    m_spThunderMapDisp->SetScreenRect(TxEnvelope(rc));

    // 2015-04-04 : 여기서 문제가 있을 수 있음.
    // Redraw하지 않도록 주석 처리 - Sizing 동안 계속 발생
    //Draw();
}

void ThunderMap::OnLButtonDown( UINT nFlags, CPoint point )
{
    ::SetCapture(m_hwnd);

    m_spThunderDraw->StopDraw();

    if (m_spMapCommand) {
        m_spMapCommand->OnLButtonDown(nFlags, point);
    }
}

void ThunderMap::OnLButtonUp( UINT nFlags, CPoint point )
{
    if (m_spMapCommand)
        m_spMapCommand->OnLButtonUp(nFlags, point);

    ::ReleaseCapture();
}

void ThunderMap::OnLButtonDblClk( UINT nFlags, CPoint point )
{
    if (m_spMapCommand)
        m_spMapCommand->OnLButtonDblClk(nFlags, point);
}

void ThunderMap::OnMouseMove( UINT nFlags, CPoint point )
{
    if (m_spThunderMapMPanCommand) {
        m_spThunderMapMPanCommand->OnMouseMove(nFlags, point);
    } 
    else if (m_spMapCommand) {
        m_spMapCommand->OnMouseMove(nFlags, point);
    }
}

void ThunderMap::OnRButtonDown( UINT nFlags, CPoint point )
{
    ::SetCapture(m_hwnd);

    if (m_spMapCommand)
        m_spMapCommand->OnRButtonDown(nFlags, point);
}

void ThunderMap::OnRButtonUp( UINT nFlags, CPoint point )
{
    if (m_spMapCommand)
        m_spMapCommand->OnRButtonUp(nFlags, point);

    ::ReleaseCapture();
}

void ThunderMap::OnRButtonDblClk( UINT nFlags, CPoint point )
{
    if (m_spMapCommand)
        m_spMapCommand->OnRButtonDblClk(nFlags, point);
}

void ThunderMap::OnMButtonDown( UINT nFlags, CPoint point )
{
    ::SetCapture(m_hwnd);

    m_spThunderMapMPanCommand = ThunderMapCommandMPanPtr(new ThunderMapCommandMPan(this, m_crBackground));
    m_spThunderMapMPanCommand->OnMButtonDown(nFlags, point);

    if (m_spMapCommand)
        m_spMapCommand->OnMButtonDown(nFlags, point);
}

void ThunderMap::OnMButtonUp( UINT nFlags, CPoint point )
{
    if (m_spThunderMapMPanCommand)
        m_spThunderMapMPanCommand->OnMButtonUp(nFlags, point);

    if (m_spMapCommand)
        m_spMapCommand->OnMButtonUp(nFlags, point);

    m_spThunderMapMPanCommand.reset();

    ::ReleaseCapture();
}

void ThunderMap::OnMouseWheel( UINT nFlags, short zDelta, CPoint point )
{
    if (nFlags != 0)
        return;
        
    //휠이 연속적으로 발생하여 드로잉 중에는 휠 적용 불가    
    if (m_spThunderDraw->GetMapDrawStatus() != 2)
        return; 

    m_spThunderDraw->StopDraw();    

    ThunderMapCommandWheelPtr spThunderMapCommandWheel(new ThunderMapCommandWheel(this, m_crBackground));
    spThunderMapCommandWheel->OnMouseWheel(nFlags, zDelta, point);

    if (m_spMapCommand)
        m_spMapCommand->OnMouseWheel(nFlags, zDelta, point);
}

void ThunderMap::OnMouseHover( UINT nFlags, CPoint point )
{
    CWnd* pHandleWnd = CWnd::FromHandlePermanent (m_hwnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    if (m_bEnableTooltip == false) {
        return;
    }

    if (m_spMapCommand != nullptr) {
        if (m_spMapCommand->IsCommandRun() == true) {
            return;
        }
    }

    if (m_spThunderMapMPanCommand != nullptr)
        return;

    size_t nxCnt(0);
    std::vector<int> vecLayerID = m_spMapLayerManager->LayerOrder(); {
        nxCnt = vecLayerID.size();
        if (nxCnt == 0)
            return;
    }    

    double dOffset = GetMapDisplayPixelToDist() * 5; 
    if (false) {
        if (dOffset <= 2) {
            dOffset = 2;
        } else if (dOffset >= 10) {
            dOffset = 10;
        }
    }
    
    TxPoint    oPt       = ScreenToMap(point);
    TxEnvelope oEnvelope = oPt.Envelope(); {
        oEnvelope.Offset(dOffset);
    }
        
    std::map<size_t, std::vector<CString>> mapTooltip;
    for (size_t i=0; i<nxCnt; i++) {
        ITxLayerPtr spLayer = m_spMapLayerManager->GetLayer(vecLayerID[i]);

        if (spLayer == nullptr)
            continue;

        if (spLayer->IsFeatureLayer() == false) {
            continue;
        }

        if (spLayer->LayerOn() == false) {
            continue;
        }

        if (spLayer->IsDraw(GetMapDisplayScale()) == false)
            continue;

        ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<ITxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            continue;

        if (spFLayer->EnableMapTooltip() == false) {
            continue;
        }
        
        bool bUseLabelTip = spFLayer->IsUseLabelMapTooltip();
        std::vector<TxFeatureDispPtr> vecFeature; {
            spFLayer->FindFeatureDisp(oEnvelope, vecFeature, false);          
        }
        
        for (size_t idx=0; idx<vecFeature.size(); idx++) {
            TxFeatureDispPtr spFeatureDisp  = vecFeature[idx];

            TxFeaturePtr   spFeature  = spFeatureDisp->Feature();
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            ITxSymbolPtr   spSymbol   = spFeatureDisp->Symbol();
            ITxLabelPtr    spLabel    = spFeatureDisp->Label(); 

            if (spGeometry == nullptr || spSymbol->Show() == false)
                continue;
            
            ITxExtendTooltipPtr spExTooltip = nullptr;
            //★ 필터링
            if (bUseLabelTip == true) {
                if (spLabel == nullptr)
                    continue;
            }
            else {
                spExTooltip = spFLayer->FeatureExTooltip(spFeature->TxObjectID);
                if (spExTooltip == nullptr)
                    continue;
            }
            
            //★ 교차 여부 체크
            if ( spGeometry->Intersect(oEnvelope) == false ) {
                continue;
            }  
            
            EnumTxGeoType enGeoType = spGeometry->GeometryType();
            if (enGeoType == GeoPolylineType) {
                if (spSymbol->GetType() == enSymbolLineDirectionType) 
                {                    
                    CString strTooltip(_T(""));
                    if (spExTooltip != nullptr) {
                        strTooltip.Format(_T("%s"), spExTooltip->Tooltip());
                    } 
                    else {
                        if (spLabel->LabelType() == TxDirectionLabelType) {
                            strTooltip.Format(_T("%s:%s"), spLabel->Label2Str(EnumForward), spLabel->Label2Str(EnumBackward));
                        } 
                        else {
                            strTooltip.Format(_T("%s"),spLabel->Label2Str());
                        }
                    }                    
                    
                    if (strTooltip.GetLength() > 0) {
                        std::vector<CString>& vecTooltip = mapTooltip[i];
                        vecTooltip.push_back(strTooltip);
                    }  
                } 
                else {
                    CString strTooltip(_T(""));
                    if (spExTooltip != nullptr) {
                        strTooltip.Format(_T("%s"), spExTooltip->Tooltip());
                    } else {
                        strTooltip = spLabel->Label2Str();
                    }
                    
                    if (strTooltip.GetLength() > 0) {
                        std::vector<CString>& vecTooltip = mapTooltip[i];
                        vecTooltip.push_back(strTooltip);
                    }     
                }                
            } 
            else {
                CString strTooltip(_T(""));
                if (spExTooltip != nullptr) {
                    strTooltip.Format(_T("%s"), spExTooltip->Tooltip());
                } else {
                    strTooltip = spLabel->Label2Str();
                }

                if (strTooltip.GetLength() > 0) {
                    std::vector<CString>& vecTooltip = mapTooltip[i];
                    vecTooltip.push_back(strTooltip);
                }   
            }
        }
    }

    if (m_spTxTooltipView == nullptr) {
        CWnd* pWnd = CWnd::FromHandle(m_hwnd);
        m_spTxTooltipView = TxTooltipViewPtr(new TxTooltipView(pWnd));
        m_spTxTooltipView->Create(TxTooltipView::IDD, pWnd);
    }
    
    CString strTooltip(_T("")); {
        nxCnt = mapTooltip.size();
        int  nCheck(0);
        bool bR(false);
        for (auto iter = mapTooltip.begin(); iter != mapTooltip.end(); ++iter) {
            std::vector<CString>& vecTooltip = iter->second;
                   
            //★ 레이어 단위로 툴팁을 묶는다.
            if (nCheck % 2 == 0) 
            {
                for (size_t i=0; i<vecTooltip.size(); i++) {
                    if (bR == false) {                        
                        strTooltip.AppendFormat(_T("* %s"), vecTooltip[i]); 
                        bR = true;
                    } else {
                        strTooltip.AppendFormat(_T("\n* %s"), vecTooltip[i]); 
                    }
                }
            } 
            else 
            {
                for (size_t i=0; i<vecTooltip.size(); i++) {
                    if (bR == false) {                        
                        strTooltip.AppendFormat(_T("# %s"), vecTooltip[i]); 
                        bR = true;
                    } else {
                        strTooltip.AppendFormat(_T("\n# %s"), vecTooltip[i]); 
                    }
                }
            }            

            nCheck++;
        }
    }
    
    if (nxCnt > 0) {
        m_spTxTooltipView->ShowWindow(SW_SHOW);
        m_spTxTooltipView->SetSimpleTooltip(strTooltip, point);
    } else {
        m_spTxTooltipView->ShowWindow(SW_HIDE);
    }   
}

void ThunderMap::OnMouseLeave()
{
    if (m_spTxTooltipView != nullptr) {
        m_spTxTooltipView->ShowWindow(SW_HIDE);
    }
}

void ThunderMap::SetMapExtent(const TxEnvelope& _oEnvelope)
{
    m_spThunderMapDisp->SetMapRect(_oEnvelope);
}

void ThunderMap::Refresh()
{
    Draw();
}

TxPoint ThunderMap::ScreenToMap( CPoint point)
{
    return m_spThunderMapDisp->ScreenToMap(point);
}

void ThunderMap::MapDisplayUndo()
{
    if ( m_spThunderMapDisp->Undo() == true )
        Refresh();
}

void ThunderMap::MapDisplayRedo()
{
    if ( m_spThunderMapDisp->Redo() == true )
        Refresh();
}

bool ThunderMap::CanMapDisplayUndo()
{
	bool bCanUndo = m_spThunderMapDisp->CanUndo();
	return bCanUndo;
}

bool ThunderMap::CanMapDisplayRedo()
{
	bool bCanRedo = m_spThunderMapDisp->CanRedo();
	return bCanRedo;
}

double ThunderMap::GetMapDisplayScale()
{
	return m_spThunderMapDisp->GetScale();
}

double ThunderMap::GetMapDisplayPixelToDist()
{
	return m_spThunderMapDisp->GetPixelToDist();
}

void ThunderMap::SetMapDisplayScale(double dScale)
{
	m_spThunderMapDisp->SetScale(dScale);
}

TxEnvelope ThunderMap::GetMapDisplayEnvelope()
{
	return m_spThunderMapDisp->MapEnvelope();
}

double ThunderMap::PixelDist()
{
    return m_spThunderMapDisp->GetPixelToDist();
}

double ThunderMap::PointDist()
{
    return m_spThunderMapDisp->GetPointToDist();
}

HRESULT ThunderMap::GetGdiplusEncoderClsid( __in LPCWSTR pwszFormat, __out GUID *pGUID )
{
    HRESULT hr = E_FAIL;
    UINT  nEncoders = 0;          // number of image encoders
    UINT  nSize = 0;              // size of the image encoder array in bytes
    CAutoVectorPtr<BYTE> spData;
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
    Gdiplus::Status status;
    bool fFound = false;

    // param check

    if ((pwszFormat == NULL) || (pwszFormat[0] == 0) || (pGUID == NULL))
    {
        return E_POINTER;
    }

    *pGUID = GUID_NULL;

    status = Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

    if ((status != Gdiplus::Ok) || (nSize == 0))
    {
        return E_FAIL;
    }

    spData.Allocate(nSize);

    if (spData == NULL)
    {
        return E_FAIL;
    }

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(BYTE*)spData;

    status = Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

    if (status != Gdiplus::Ok)
    {
        return E_FAIL;
    }

    for(UINT j = 0; j < nEncoders; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, pwszFormat) == 0 )
        {
            *pGUID = pImageCodecInfo[j].Clsid;
            fFound = true;
            break;
        }    
    }

    hr = fFound ? S_OK : E_FAIL;

    return hr;
}

bool ThunderMap::MapWriteImage(CString strPath)
{
    try {
        if (m_spThunderDraw == nullptr)
            return false;
        
        CLSID encoderClsid;
        if ( GetGdiplusEncoderClsid(L"image/bmp", &encoderClsid) != S_OK)
            return false;

        Bitmap* pBitmap = m_spThunderDraw->MapImage();
        if (pBitmap == nullptr) 
            return false;

        pBitmap->Save(strPath, &encoderClsid, NULL);
        delete pBitmap;

        return true;
    }
    catch(...) {
        int m = 9;
    }

    return false;
}

void ThunderMap::ShowLoadingMessage( CString strMsg /*= _T("Loading...")*/ )
{
    if (m_spThunderDraw) {        
        m_spThunderDraw->ShowLoadingMessage(strMsg);
    }
}

void ThunderMap::HideLoadingMessage()
{
    if (m_spThunderDraw) {        
        m_spThunderDraw->HideLoadingMessage();
    }
}

void ThunderMap::MapDrawOptionFlicker( bool bFlicker )
{
    if (m_spThunderDraw) {        
        m_spThunderDraw->MapDrawOptionFlicker(bFlicker);
    }
}

int ThunderMap::GetMapDrawStatus()
{
    if (m_spThunderDraw)
        return m_spThunderDraw->GetMapDrawStatus();

    return -1;
}



/*****************************************************************************
* 2019.03.08 : 지도 배경색상 변경기능 추가
*****************************************************************************/
void ThunderMap::SetBackgroundColor(Gdiplus::Color crBackground)
{
	m_crBackground = crBackground;
	if (m_spThunderDraw != NULL)
	{
		m_spThunderDraw->SetBackgroundColor(m_crBackground);
		Refresh();
	}

	if (nullptr != m_spMapCommand)
	{
		m_spMapCommand->SetBackgroundColor(m_crBackground);
	}

	if (nullptr != m_spThunderMapMPanCommand)
	{
		m_spThunderMapMPanCommand->SetBackgroundColor(m_crBackground);
	}
}


Gdiplus::Color ThunderMap::GetBackgroundColor()
{
	if (m_spThunderDraw != NULL)
	{
		return m_spThunderDraw->GetBackgroundColor();
	}
	
	return Gdiplus::Color(0, 0, 0, 0);
}
/****************************************************************************/
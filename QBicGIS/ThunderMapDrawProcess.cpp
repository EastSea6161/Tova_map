#include "stdafx.h"
#include "TxMath.h"

#include "ThunderMapDrawProcess.h"
#include "TxFeature.h"
#include "ITxLayer.h"
#include "ITxImageLayer.h"
#include "TxFeatureUserLayer.h"

#include "TxPoint.h"
#include "TxPolyline.h"
#include "TxPolygon.h"
#include "TxProcessInfo.h"
#include "TxPixelKey.h"

#include "TxSymbolPoint.h"
#include "TxSymbolPointCircle.h"
#include "TxSymbolPointQuad.h"
#include "TxSymbolPointRect.h"
#include "TxSymbolPointImage.h"

#include "TxSymbolLine.h"
#include "TxSymbolDirectionLine.h"

#include "TxSymbolPolygon.h"

using namespace Gdiplus;

ThunderMapdrawProcess::ThunderMapdrawProcess(HWND hWnd, Gdiplus::Color crBackground) 
	: MapProcessStatus(2), MapCommandID(0), m_crBackground(crBackground)
{       
    m_gdiClrMap.oldColor = Gdiplus::Color::White;
    m_gdiClrMap.newColor = Gdiplus::Color(0,0,0,0);
        
    m_gdiImgAtt00.SetRemapTable(1, &m_gdiClrMap);

    /*Point m_pArrowPts[4];*/ {
        //★ 심볼 모양 :  ▲ - 상단 꼭지점이 중심
        m_pArrowPts[0].X = -3 ;
        m_pArrowPts[0].Y = 12 ;
                
        m_pArrowPts[1].X = 0;
        m_pArrowPts[1].Y = 0;                            

        m_pArrowPts[2].X = 3;
        m_pArrowPts[2].Y = 12;

        m_pArrowPts[3].X = -3;
        m_pArrowPts[3].Y = 12;
    }
      
    m_bFirstTime = true;
    m_bFlicker   = true;

    m_nFlickerGap   = 1000;
    m_bAlphaFlicker = false;

    m_bShowLoadingMessage = true;
    m_strLoadingMessage   = _T("Loading...");

    m_hMapDrawingProcess = nullptr;
    m_hTerminateEvent    = nullptr;
    m_hDrawStartEvent    = nullptr;
    m_hDrawEndEvent      = nullptr;

    m_hStopDrawEndEvent  = nullptr;

    m_pBitmapBackground  = nullptr;
    m_pCachedBackground  = nullptr;

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (hWnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr) {
        m_hwnd = nullptr;
        return;
    }
    
    m_hwnd = hWnd;

    //★ GDI Blending 
    {
        m_bf.BlendOp = AC_SRC_OVER;
        m_bf.BlendFlags = 0;
        m_bf.SourceConstantAlpha = 30;
        m_bf.AlphaFormat = 0;
    }    

    ThreadStart();
}

ThunderMapdrawProcess::~ThunderMapdrawProcess()
{
    SetMapDrawStatus(0);
    ThreadClose();
    Sleep(100);

    DeleteNull(m_pBitmapBackground);
    DeleteNull(m_pCachedBackground);
}

void ThunderMapdrawProcess::ThreadStart()
{   
    ThreadClose();

    if(true) {
        CWnd* pHandleWnd = CWnd::FromHandlePermanent (m_hwnd); //CWnd::FromHandle(m_hwnd)
        if (pHandleWnd == nullptr)
            return;

        RECT rc; GetClientRect(m_hwnd, &rc); CRect rect(rc);    
        Gdiplus::RectF gRect(0.f, 0.f, (Gdiplus::REAL)(rect.Width()), (Gdiplus::REAL)(rect.Height()));

        CClientDC oMapDC(pHandleWnd);  

        Bitmap gdiBmpStack(rect.Width(), rect.Height()/*, PixelFormat32bppPARGB*/); {
            Graphics graphicsBack(&gdiBmpStack);
            graphicsBack.Clear(Color::White);
        }

        Graphics graphics(oMapDC);         
        graphics.DrawImage(&gdiBmpStack, 0, 0);   

        Bitmap* pTemp = m_pBitmapBackground; {
            Lock.Lock();
            m_pBitmapBackground = gdiBmpStack.Clone(0, 0, rect.Width(), rect.Height(), gdiBmpStack.GetPixelFormat());
            Lock.Unlock();
        }          
        delete pTemp;
    }

    //★ 자동 Reset Event
    m_hTerminateEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if( m_hTerminateEvent == NULL ) throw 1;

    m_hDrawStartEvent  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(m_hDrawStartEvent == NULL) throw 2;

    m_hDrawEndEvent  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(m_hDrawEndEvent == NULL) throw 2;

    m_hStopDrawEndEvent= ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(m_hStopDrawEndEvent == NULL) throw 2;

    unsigned int nThreadID = 0;
    m_hMapDrawingProcess = (HANDLE)::_beginthreadex(NULL, 0, MapDrawingThreadProcess, this, 0, &nThreadID);

    if(m_hMapDrawingProcess == NULL) throw 3;

    m_hEvents[0] = m_hTerminateEvent;
    m_hEvents[1] = m_hDrawStartEvent;

    MapProcessStatus = 0;
}

void ThunderMapdrawProcess::ThreadClose()
{
    if( NULL != m_hMapDrawingProcess ) {
        ::SetEvent(m_hTerminateEvent);
        ::WaitForSingleObject(m_hMapDrawingProcess, INFINITE);
        ::CloseHandle(m_hMapDrawingProcess);
        m_hMapDrawingProcess = NULL;
    }

    if( NULL != m_hDrawStartEvent )    {
        ::CloseHandle(m_hDrawStartEvent);
        m_hDrawStartEvent = NULL;
    }

    if( NULL != m_hDrawEndEvent )      {
        ::CloseHandle(m_hDrawEndEvent);
        m_hDrawEndEvent = NULL;
    }

    if( NULL != m_hStopDrawEndEvent )    {
        ::CloseHandle(m_hStopDrawEndEvent);
        m_hStopDrawEndEvent = NULL;
    }

    if( NULL != m_hTerminateEvent )    {
        ::CloseHandle(m_hTerminateEvent);
        m_hTerminateEvent = NULL;
    }
}

void ThunderMapdrawProcess::Draw( ThunderMapDispPtr spThunderMapDisp )
{  
    //★ Main Start : 
    //★ 지도 엔진은 Thread 상태로 응답 대기 상태가 되어 있다.
    //★             <m_hTerminateEvent, m_hDrawStartEvent>
    //★ 화면에서 그리기 요청 작업이 들어온 경우

    //-------------------------------------------------------------------
    //★ 그리기 작업을 멈춘다.
    StopDraw(0);  

    /* 불필요 <StopDraw에서 증가시킴>
    Lock.Lock();
    ++MapCommandID;
    Lock.Unlock();
    */

    //-------------------------------------------------------------------
    //★ 스마트 포인터이기 때문에 한번만 연결해주면 되지만....안전하게
    m_spThunderMapDisp = spThunderMapDisp;

    //-------------------------------------------------------------------
    //★ 응답 대기 중인 그리기 Thread 동작하세요..
    ::SetEvent(m_hDrawStartEvent);   
}

void ThunderMapdrawProcess::StopDraw(int nWho)
{
    Lock.Lock();
    ++MapCommandID;
    Lock.Unlock();

    //★ 마우스 클릭(이벤트), 신규드로잉요청
    int nMapStauts = GetMapDrawStatus();
    if (nMapStauts == 1) {    //★ 1번은 그리고 있다        
        SetMapDrawStatus(0);  //★ 그리는작업을 멈추어라.
        ::WaitForSingleObject(m_hDrawEndEvent, INFINITE); 
    }

    SetMapDrawStatus(2);   
    //::SetEvent(m_hStopDrawEndEvent);    
}

unsigned __stdcall ThunderMapdrawProcess::MapDrawingThreadProcess( void *pParam )
{
    ThunderMapdrawProcess* pMapdrawProcess = static_cast<ThunderMapdrawProcess*>(pParam);
    if (pMapdrawProcess == NULL) return 0;
        
    unsigned rValue =  pMapdrawProcess->MapDrawingProcess();

    return rValue;
}

unsigned ThunderMapdrawProcess::MapDrawingProcess()
{
    bool bRun = true;
    try
    {
        while(bRun)
        {
            //★ 대기 상태로 멈추어 있다.
            //★ m_hEvents<m_hTerminateEvent, m_hDrawStartEvent>
            DWORD dwIndex = ::WaitForMultipleObjects(2, m_hEvents, FALSE, INFINITE);
            
            bool bSendDrawEndEvent(false);
            if (dwIndex == WAIT_OBJECT_0) 
            {//★ 종료 요청
                bRun = false;
            }
            else if (dwIndex == WAIT_OBJECT_0 +1 && ::IsWindow(m_hwnd) == TRUE)
            {   
                Lock.Lock(); {
                    DeleteNull(m_pCachedBackground);
                    DeleteNull(m_pBitmapBackground);
                }                
                Lock.Unlock();

                //★ 현재의 드로잉 커맨드 정보
                Lock.Lock();
                size_t nxCommandID = MapCommandID;
                Lock.Unlock();

                SetMapDrawStatus(1);
                bSendDrawEndEvent = false;
                     
                RECT rc; GetClientRect(m_hwnd, &rc); CRect rect(rc);    
                Gdiplus::RectF gRect(0.f, 0.f, (Gdiplus::REAL)(rect.Width()), (Gdiplus::REAL)(rect.Height()));

                CClientDC oMapDC(CWnd::FromHandle(m_hwnd));  
                    
                //★ 지도 배경 : White
                Bitmap gdiBmpStack(rect.Width(), rect.Height()/*, PixelFormat32bppPARGB*/); {
                    Graphics graphics(&gdiBmpStack);
					//graphics.Clear(Color::White);
					graphics.Clear(m_crBackground);
					/*****************************************************************************
					* 2019.03.08 : 지도 배경색상 변경기능 추가에 따른 수정
					*****************************************************************************/
					//if (m_crBackground.GetValue() != Color::White)
					//{
					//	Gdiplus::SolidBrush sb(m_crBackground);
					//	graphics.FillRectangle(&sb, gRect);
					//	
					//}
					/****************************************************************************/
                }

                //★ 지도의 영역이 너무 작은 경우 그릴 필요가 없다.
                bool bVerySmallSize(false); 
                if (rect.Width() <= 1 || rect.Height() <= 1) {
                    bVerySmallSize = true;                    
                }
                
                //★ 처음 지도가 보일때는 화면에 흰색 배경 즉시 출력
                if (m_bFirstTime == true) { 
                    Graphics graphics(oMapDC); 
                    //graphics.Clear(Color::White);
					graphics.Clear(m_crBackground);
                }

                TxEnvelope oDrawMapEnv = m_spThunderMapDisp->MapEnvelope();
                TxEnvelope oDrawScrEnv = m_spThunderMapDisp->ScreenEnvelope();
                
                //★ 지도를 그린다.
                if (bVerySmallSize == false) 
                {
                    size_t nxSumDrawCount(0);

                    Graphics xGraphics(&gdiBmpStack);  
                    double dMapScale = m_spThunderMapDisp->GetScale();
                    std::vector<ITxLayerPtr>& vecLayer = m_spLayerManager->m_vecDrawOrderLayers;   
                    
                    __int64 nxFlickerTick = GetMapTickCount();

                    for (size_t i=0; i<vecLayer.size(); i++) 
                    {
                        //★ 그리기 종료 상태 인가 ?
                        if (GetMapDrawStatus() == 0 )
                            break;

                        ITxLayerPtr spLayer; {
                            try {
                                spLayer = vecLayer[i]; 
                                if (spLayer == nullptr) {
                                    continue;
                                }
                            } catch (...) {
                                continue;
                            }                        
                        }        

                        if (spLayer->IsDraw(dMapScale)) {
                            if (spLayer->IsFeatureLayer() == true) {
                                double dPixelDist = m_spThunderMapDisp->GetPixelToDist();

                                double dMinExtent(0);
                                if (dPixelDist > 1000) {
                                    dMinExtent = 500.0;
                                } else {
                                    dMinExtent = dPixelDist * 0.5;
                                }
                                
                                ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer>(spLayer);
                                std::vector<TxFeatureDispPtr> vecDrawFeatureDisp;
                                if (GetMapDrawStatus() != 0) {
                                    //★ Drawing 보다 조회 속도가 문제이다.
                                    //TxLogDebug(_T("\n"));
                                    //TxLogDebugStartMsg();

                                    bool bExpired = spFeatureLayer->DrawExpired(oDrawMapEnv, oDrawScrEnv); {
                                        //★ 안전하게 처리한다. 
                                        //★ 정상적으로 그렸다면 ... 하위 루틴에서 Expired를 false로 설정한다.
                                        //★ spFeatureLayer->SetLayerImage(spLayerBitmap, oDrawMapEnv, oDrawScrEnv);
                                        spFeatureLayer->SetDrawExpire();
                                    }

                                    if (bExpired == false) {
                                        TxGdiplusBitmapPtr spLayerBitmap = spFeatureLayer->GetLayerImage();
                                        Bitmap* pBitmap = spLayerBitmap->GetBitmap();
                                        xGraphics.DrawImage(pBitmap, 0, 0);
                                    }
                                    else 
                                    {
                                        spFeatureLayer->FindFeatureDisp(m_spThunderMapDisp->MapEnvelope(), vecDrawFeatureDisp, dMinExtent, this);
                                        
                                        __int64 nxCurTick  = GetMapTickCount();
                                        __int64 nxInterval = nxCurTick - nxFlickerTick; 
                                        if (nxInterval > m_nFlickerGap) {
                                            nxFlickerTick = nxCurTick - m_nFlickerGap + m_nFlickerGap / 10;
                                        }

                                        //TxLogDebugStartMsg();

                                        if (GetMapDrawStatus() != 0 && nxCommandID == MapCommandID) {
                                            TxGdiplusBitmapPtr spLayerBitmap = TxGdiplusBitmapPtr(new TxGdiplusBitmap(rect.Width(), rect.Height()));
                                            Bitmap* pBitmap = spLayerBitmap->GetBitmap();
                                            Graphics oGraphic(pBitmap);  

                                            MapGDIPlusDrawing(nxSumDrawCount, m_bFlicker, nxFlickerTick, nxCommandID, oMapDC, oGraphic, spFeatureLayer, vecDrawFeatureDisp, rect, &gdiBmpStack, pBitmap); 

                                            xGraphics.DrawImage(pBitmap, 0, 0);
                                            if (GetMapDrawStatus() != 0 && nxCommandID == MapCommandID) {
                                                spFeatureLayer->SetLayerImage(spLayerBitmap, oDrawMapEnv, oDrawScrEnv);
                                            }
                                        }
                                        //TxLogDebugEndMsg();
                                    }                                    
                                }
                            }
                            else
                            {//ImageLayer....        
                                DWORD dwFlag(0);                            
                                ITxImageLayerPtr spImgLayer = std::dynamic_pointer_cast<ITxImageLayer>(spLayer);
                                if (spImgLayer && InternetGetConnectedState(&dwFlag, 0) == TRUE && GetMapDrawStatus() != 0) {
                                    try {
                                        TxGdiplusBitmapPtr spBitmap = spImgLayer->GetImage(m_spThunderMapDisp);
                                        if (spBitmap) {
                                            //Graphics xGraphics(&gdiBmpStack);
                                            if (spImgLayer->IsGrayScale() == true) {
                                                Bitmap* pOrigin = spBitmap->GetBitmap();
                                                Bitmap  bmpGray(pOrigin->GetWidth(), pOrigin->GetHeight());
                                                MakeGrayScale(bmpGray, pOrigin);
                                                xGraphics.DrawImage(&bmpGray, 0, 0);
                                            } else {
                                                xGraphics.DrawImage(spBitmap->GetBitmap(), 0, 0);
                                            }
                                        }
                                    } catch(...) {
                                        TxLogDebugException();
                                    }                                
                                }                            
                            }                        
                        }
                    }
                }
                                
                //★ 마지막 드로잉을 해준다.
                //★ 정상적인 드로잉 완료의 경우

                if (true) {
                    CDC bufferDC; {
                        bufferDC.CreateCompatibleDC(&oMapDC);
                    }
                    CBitmap oBufferBmp; {
                        oBufferBmp.CreateCompatibleBitmap(&oMapDC, rect.Width(), rect.Height());
                    }    

                    if (bufferDC.m_hDC != nullptr) {
                        // 메모리 비트맵 생성
                        CBitmap* pOldBitmap = bufferDC.SelectObject(&oBufferBmp);  
                        Gdiplus::Graphics xGraphics(bufferDC); {
                            xGraphics.DrawImage(&gdiBmpStack, 0, 0);
                        }

                        oMapDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);
                        bufferDC.SelectObject(pOldBitmap);           
                        bufferDC.DeleteDC();

                        DeleteNull(m_pCachedBackground);
                        m_pCachedBackground = new CachedBitmap(&gdiBmpStack, &xGraphics);
                    }
                }                 

                Lock.Lock(); {
                    DeleteNull(m_pBitmapBackground);
                    m_pBitmapBackground = gdiBmpStack.Clone(0, 0, rect.Width(), rect.Height(), gdiBmpStack.GetPixelFormat());    
                }                                    
                Lock.Unlock();

                if (nxCommandID == MapCommandID) {                      
                    //★ 중간 Stopping 없이 그리기를 완료했다.
                    bSendDrawEndEvent = true;
                    
                    if (bVerySmallSize == true) {
                        m_bFirstTime = true;
                    }
                    else {
                        m_bFirstTime = false;
                    }

                    //★ 지도의 상태 업데이트(그리기 작업이 완료/다른 요청가능)
                    SetMapDrawStatus(2);  
                }
            }
            else {
                TxLogDebugException();
            }
                                    
            //★ 통보 : 그리기 작업 완료 되었다. 
            SetEvent(m_hDrawEndEvent);                                
            
            try {
                //★ 고민이다..::PostMessage(m_hwnd, WM_MAP_BACKGROUND_DRAW_END, NULL, NULL);// Back Ground Draw가 종료 되었다. 
                if (bSendDrawEndEvent)
                    ::SendMessage(m_hwnd, WM_MAP_BACKGROUND_DRAW_END, NULL, NULL); 
            } catch(...) 
            {
                TxLogDebugException();
            }
        }
    }
    catch(...) {
        TxLogDebugException();
        return 0;
    }

    return 1;
}


void ThunderMapdrawProcess::ForegroundRefresh( std::vector<ITxLayerPtr>& vecFlashLayer, ThunderMapDispPtr spThunderMapDisp )
{
#pragma region comment
    //★ 더블 버퍼링 방식을 변경 - 레이어 드로잉 방식과 다름
#pragma endregion comment

    if (m_bFirstTime == true || m_hwnd == nullptr) {        
        return;
    }

    if (GetMapDrawStatus() != 2) {   
        return;
    }

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (m_hwnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    RECT rc;  GetClientRect(m_hwnd, &rc); CRect rect(rc);
    CClientDC oMapDC(pHandleWnd);      

    CDC bufferDC; {
        bufferDC.CreateCompatibleDC(&oMapDC);
    }
    CBitmap oBufferBmp; {
        oBufferBmp.CreateCompatibleBitmap(&oMapDC, rect.Width(), rect.Height());
    }    

    // 메모리 비트맵 생성
    CBitmap* pOldBitmap = bufferDC.SelectObject(&oBufferBmp);  
    Gdiplus::Graphics xGraphics(bufferDC); 

    if (m_pBitmapBackground == nullptr){
        xGraphics.Clear(Color::White);
    }
    else {
        if (m_pCachedBackground != nullptr)
            if (m_pCachedBackground->GetLastStatus() == Ok) {
                xGraphics.DrawCachedBitmap(m_pCachedBackground, 0, 0);
            }
            else {
                xGraphics.DrawImage(m_pBitmapBackground, 0, 0);  
            }            
        else {
            xGraphics.DrawImage(m_pBitmapBackground, 0, 0);  
        }
    } 

    //★ 조심. 조심.. 0 개가 올수 있음 -> Clear의 의미 
    size_t nxCount = vecFlashLayer.size();   

    Lock.Lock();
    size_t nxCommandID = MapCommandID;
    Lock.Unlock();

    for (size_t i=0; i<nxCount; i++)
    {
        ITxLayerPtr&   spLayer     = vecFlashLayer[i];
        TxLayerInfoPtr spLayerInfo = spLayer->LayerInfo();

        if (spLayer->LayerOn()) {
            if (GetMapDrawStatus() != 2) {   
                break;;
            }

            ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer>(spLayer);
            if (spFeatureLayer) {
                std::vector<TxFeatureDispPtr> vecDrawFeatureDisp;
                spFeatureLayer->FindFeatureDisp(m_spThunderMapDisp->MapEnvelope(), vecDrawFeatureDisp, 0.0, this);      
                if (GetMapDrawStatus() != 2) {   
                    break;;
                }
                MapGDIPlusDrawing(nxCommandID, oMapDC, xGraphics, spFeatureLayer, vecDrawFeatureDisp, rect); 
            }
        }
    }
     
    if (oMapDC.m_hDC != nullptr && GetMapDrawStatus() == 2 && nxCommandID == MapCommandID) {
        oMapDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);  
        bufferDC.SelectObject(pOldBitmap);
    }
    
    bufferDC.DeleteDC();
}

void ThunderMapdrawProcess::MapGDIPlusDrawing( size_t nxCommandID, CDC& oMapDC, Graphics& oGraphicLayer, ITxFeatureLayerPtr spLayer, std::vector<TxFeatureDispPtr>& vecDrawFeature, CRect& rect )
{
    __int64 nxFlickerTick = GetMapTickCount();
    size_t nxSumDrawCount(0);
    MapGDIPlusDrawing(nxSumDrawCount, false, nxFlickerTick, nxCommandID, oMapDC, oGraphicLayer, spLayer, vecDrawFeature, rect, nullptr, nullptr);
}

void ThunderMapdrawProcess::MapGDIPlusDrawing( size_t& nxSumDrawCount, bool bFlicker, __int64& nxFlickerTick, 
                                               size_t nxCommandID, 
                                               CDC& oMapDC, 
                                               Graphics& oGraphicLayer, 
                                               ITxFeatureLayerPtr spLayer, 
                                               std::vector<TxFeatureDispPtr>& vecDrawFeature, CRect& rect, 
                                               Bitmap* pBmpStack, Bitmap* pBmpLayer)
{   
    if (nxFlickerTick <= 0) {
        nxFlickerTick = GetMapTickCount();
    }

    //★ 드로잉 데이터 정렬 -> 향후 Option으로 처리 필요
    if (spLayer->UseSizeSortDraw())
        std::sort(vecDrawFeature.begin(), vecDrawFeature.end(), DrawSizeDesc);
    else
        std::sort(vecDrawFeature.begin(), vecDrawFeature.end(), DrawIDAsc);

    bool bDrawFilter = spLayer->DrawOverlapFilter();

    TxLayerInfoPtr spLayerInfo = spLayer->LayerInfo();
    
    // Drawing Tunning
    std::set<TxPixelKey> PointPixelGrid;
    std::set<TxPixelKey> LinePixelGrid;
    
    std::set<TxPixelKey>  PointLabelGrid;
    std::map<TxPixelKey, int> LineLabelGrid; /*라인은 2개가 같이 보이는 것이 좋다.*/

    size_t nxFilterOutCount(0);    
    size_t nxCount = vecDrawFeature.size();
    
    //★ Label 필터링(충돌 제거)
    bool bLabelFilter = spLayer->LabelFilter();    
    
    int nDispFlag(2); {        
        if (spLayerInfo->GeoType() == GeoPolygonType || spLayerInfo->GeoType() == GeoMPolygonType) {
            //★ 폴리곤
            if (nxCount <= 1000) {
                nDispFlag = 1;
            }
            else {
                nDispFlag = 2;
            }
        }
        else if (spLayerInfo->GeoType() == GeoPointType) {
            //★ 포인트
            if (nxCount <= 1000) {
                nDispFlag = 1;
            } 
            else {
                nDispFlag = 2;
            }
        }
        else if (spLayerInfo->GeoType() == GeoPolylineType){
            //★ 라인
            if (nxCount <= 10000) {
                nDispFlag = 0;
            } 
            else if (nxCount <= 50000) {
                nDispFlag = 1;
            }
            else {
                nDispFlag = 2;
            }
        }   
        else {
            //★ Unknown
            if (nxCount <= 1000) {
                nDispFlag = 0;
            } 
            else if (nxCount <= 5000) {
                nDispFlag = 1;
            }
            else {
                nDispFlag = 2;
            }
        }        
    }
    //★ 디스플레이 품질 설정
    DisplayQuality(oGraphicLayer, nDispFlag);

    CString strTemp(_T(""));
    if (false) {        
        strTemp.Format(_T("[%d]- MapGDIPlusDrawing Start-->[%d] - DrawOption : %d"), nxCommandID, nxCount, nDispFlag);
        TxLogDebug((LPCTSTR)strTemp);
    }    
    
    double dMapScale  = m_spThunderMapDisp->GetScale();
    double dPixelDist = m_spThunderMapDisp->GetPixelToDist();
    double dPointDist = m_spThunderMapDisp->GetPointToDist();
    double dLabelAgg  = dPixelDist * 50; 
      
    double dHideEnvArea  = dPixelDist * 0.5; 
    if (dHideEnvArea > 500.0)
        dHideEnvArea = 500.0;    

    //★ 포인트/라인 데코레이션
    std::vector<TPointDecoInfoPtr> vecPointDeco;
    std::vector<TLineDecoInfoPtr>  vecLineDeco;

    //★ 라벨 정보
    std::vector<TLabelInfoPtr> vecPointLabel;
    std::vector<TLabelInfoPtr> vecLineLabel;
    std::vector<TLabelInfoPtr> vecPolyLabel;

    // 라벨 처리     
    bool  bShowLabel = spLayer->ShowLabel();
    if (bShowLabel == true) {
        if ( dMapScale > spLayer->DrawLabelMaxScale() )
            bShowLabel = false;
    }

    ITxLabelSymbolPtr spLabelSymbol = spLayer->LabelSymbol();

    Gdiplus::SolidBrush   oLabelFontBrush(spLabelSymbol->Color());            
    Gdiplus::StringFormat oLabelStrFormat; {
        oLabelStrFormat.SetAlignment    (StringAlignmentCenter);
        oLabelStrFormat.SetLineAlignment(StringAlignmentCenter);
    }    

    Gdiplus::FontFamily oLabelFontFamily(spLabelSymbol->FontName()); 
    Gdiplus::Font       oLabelFont(&oLabelFontFamily, (Gdiplus::REAL)spLabelSymbol->LabelSize(), spLabelSymbol->FontStyle()/*FontStyleRegular*/, UnitPoint);
    
    //★ 텍스트 백그라운드 그리기 : 
    bool bLabelBackFill = spLabelSymbol->BackgoundFill(); 
   
    int  nMapStauts(0);
    bool bStopping(false);

    //★ enSymbolPointType - 포인트 심볼 드로잉 속도 개선
    std::map<TxSymbolPointPtr, TxCachedBitmapPtr> mapPointSymbolCache;

    for (size_t i=0; i<nxCount; i++)
    {
        nMapStauts = GetMapDrawStatus();
        if (nMapStauts == 0 || nxCommandID != MapCommandID) {  
            bStopping = true;
            break;
        }

        TxFeatureDispPtr spFeatureDisp = vecDrawFeature[i];
        if (spFeatureDisp == nullptr) {
            continue;
        }

        TxFeaturePtr spFeature = spFeatureDisp->Feature();
        if (spFeature == nullptr)
            continue;

        ITxSymbolPtr spSymbol = spFeatureDisp->Symbol();
        if (spSymbol == nullptr)
            continue;

        if (spSymbol->Show() == false) 
            continue;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;

        if (spGeometry->GeometryType() == GeoPointType)
        {    
#pragma region DrawPoint   
            TxPointPtr spOrgPt   = std::dynamic_pointer_cast<TxPoint>(spGeometry);
            if (spOrgPt == nullptr)
                continue;

            TxPointPtr spPoint   = TxPointPtr(new TxPoint(spOrgPt->X, spOrgPt->Y));
            TxPoint    oPtScreen = m_spThunderMapDisp->MapToScreen(spPoint);
            
            if (nxCount >= 10000) {
                __int64 nxFilterLeft   = (__int64)(oPtScreen.X * 0.5f);
                __int64 nxFilterBottom = (__int64)(oPtScreen.Y * 0.5f);

                if (bDrawFilter && PointPixelGrid.insert(TxPixelKey(nxFilterLeft, nxFilterBottom)).second == false) {
                    nxSumDrawCount++;
                    nxFilterOutCount++;
                    continue;
                }
            }
            
            if (spSymbol->GetType() == enSymbolPointType) {
                TxSymbolPointPtr spPointSymbol = std::dynamic_pointer_cast<TxSymbolPoint>(spSymbol);                        
                if ( spPointSymbol == nullptr ) {         
                    continue;
                }
                
                char cSymbol = spPointSymbol->Symbol();
                Gdiplus::REAL dPointSize = (Gdiplus::REAL)spPointSymbol->Size();
                //★ 포인트 사이즈가 작은 경우 1로 치환
                //   Cache Bitmap의 사이즈가 0으로 되는 경우 발생(2016-10-19)
                if (dPointSize < 1)
                    dPointSize = 1;

                Gdiplus::SolidBrush oFontBrush(spPointSymbol->Color()/*Gdiplus::Color(0,0,0)*/);      

                CString strTextSymbol(_T("")); {
                    strTextSymbol.Format(_T("%c"), cSymbol);
                }               
                
                //★ 향후 다양한 폰트 적용을 위해서 멤머 변수로 선언하지 않음.
                Gdiplus::FontFamily oFontFamily(L"QBicF"); 
                Gdiplus::Font       oFont(&oFontFamily, dPointSize, spLabelSymbol->FontStyle()/*FontStyleRegular*/, UnitPoint);

                Gdiplus::StringFormat stringFormat; {
                    stringFormat.SetAlignment    (StringAlignmentCenter);
                    stringFormat.SetLineAlignment(StringAlignmentCenter);
                }
                
                //★ Decoration 정보 
                std::map<int, ITxPointDecoSymbolPtr>& mapDeco = spPointSymbol->GetDecoSymbol(); 
                for (auto iter = mapDeco.begin(); iter != mapDeco.end(); ++iter) 
                {
                    ITxPointDecoSymbolPtr spDeco = iter->second;
                    if (spDeco == nullptr) 
                        continue;

                    if (dMapScale > spDeco->DrawMaxScale()) 
                        continue;

                    if (true/*Drawing*/) {
                        double dX = spPoint->X + spDeco->ShiftX() * dPixelDist;
                        double dY = spPoint->Y + spDeco->ShiftY() * dPixelDist;

                        TxPoint oPtScreen = m_spThunderMapDisp->MapToScreen(TxPoint(dX, dY));                        
                        vecPointDeco.push_back(TPointDecoInfoPtr(new TPointDecoInfo(spDeco, oPtScreen)));
                    }
                }

                auto iFind = mapPointSymbolCache.find(spPointSymbol);
                if (iFind != mapPointSymbolCache.end()) {
                    TxCachedBitmapPtr spCacheBmp = iFind->second;
                    
                    CachedBitmap* pBitmap = spCacheBmp->CachedBitmap();
                                        
                    double dHalfWidth  = spCacheBmp->GetWidth()  * 0.5;
                    double dHalfHeight = spCacheBmp->GetHeight() * 0.5;
                    double dShift(0.5);  //★ 이미지는 짝수이다.                  
                    oGraphicLayer.DrawCachedBitmap(pBitmap, (int)(oPtScreen.X - dHalfWidth + dShift), (int)(oPtScreen.Y - dHalfHeight + dShift));
                }
                else 
                {
                    //★ 1000개 이하의 경우만 적용
                    if (mapPointSymbolCache.size() <= 1000) {
                        //★ Make Cache 
                        RectF oBound; {
                            oGraphicLayer.MeasureString(strTextSymbol, -1, &oFont, PointF(0,0), &stringFormat, &oBound);                        
                        }
                        //★ 2배정도
                        int nWidth  = (int) (2.0 * oBound.Width);
                        int nHeight = (int) (2.0 * oBound.Height);

                        //★ 짝수로 만들어 준다.
                        if (nWidth % 2 != 0) {
                            nWidth++;
                        }

                        if (nHeight % 2 != 0) {
                            nHeight++;
                        }

                        Bitmap gdiBmp(nWidth, nHeight); 
                        Graphics oGraphic(&gdiBmp);  {
                            //★ Cache의 품질은 유지해야 한다. 복사할때 깨진다.
                            oGraphic.SetTextRenderingHint(TextRenderingHintAntiAlias);
                        }                        
                        oGraphic.DrawString(strTextSymbol, (INT)wcslen(strTextSymbol), &oFont, TxPoint(0.5*nWidth, 0.5*nHeight), &stringFormat, &oFontBrush); 

                        TxCachedBitmapPtr spCacheBmp = TxCachedBitmapPtr(new TxCachedBitmap(&gdiBmp, &oGraphicLayer));
                        if (spCacheBmp->IsOk())
                            mapPointSymbolCache.insert(std::make_pair(spPointSymbol, spCacheBmp));

                        oGraphicLayer.DrawCachedBitmap(spCacheBmp->CachedBitmap(), (int)(oPtScreen.X - nWidth*0.5), (int)(oPtScreen.Y - nHeight*0.5));   
                    }    
                    else {
                        oGraphicLayer.DrawString(strTextSymbol, (INT)wcslen(strTextSymbol), &oFont, oPtScreen, &stringFormat, &oFontBrush); 
                    }                       
                }                

                //★ xGraphics.DrawString(strTextSymbol, (INT)wcslen(strTextSymbol), &oFont, oPtScreen, &stringFormat, &oFontBrush); 

                if (bShowLabel) {
                    ITxLabelPtr spLabel = spFeatureDisp->Label();
                    if (spLabel && spLabel->Show()) {
                        CString strLabel = spLabel->Label2Str();
                        if (strLabel.GetLength() >= 1) 
                        {   
                            if (bLabelFilter) {
                                __int64 nxFilterX = (__int64)(spPoint->X / dLabelAgg);
                                __int64 nxFilterY = (__int64)(spPoint->Y / dLabelAgg);

                                if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) 
                                {
                                    double dLabelHeightMt(0); {
                                        RectF oBound; {
                                            oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0,0), &oLabelStrFormat, &oBound);
                                        }

                                        double dLabelHeightPx = oBound.Height;
                                        dLabelHeightMt = dLabelHeightPx * dPixelDist;
                                    }   

                                    RectF oBound; {
                                        oGraphicLayer.MeasureString(strTextSymbol, -1, &oFont, PointF(0,0), &stringFormat, &oBound);
                                    }
                                    double dSymHeight = oBound.Height * dPixelDist;

                                    TxPoint oLabelPoint(spPoint->X, spPoint->Y); {
                                        if (bLabelBackFill) {
                                            oLabelPoint.Y = oLabelPoint.Y - 0.5 * dLabelHeightMt - 0.5 * dSymHeight;
                                        } else {
                                            oLabelPoint.Y = oLabelPoint.Y - 0.4 * dLabelHeightMt - 0.4 * dSymHeight;
                                        }                                        
                                    }    

                                    oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                                    TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                    vecPointLabel.push_back(spLabelInfo);
                                }
                            }
                            else {
                                double dLabelHeightMt(0); {
                                    RectF oBound; {
                                        oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0,0), &oLabelStrFormat, &oBound);
                                    }

                                    double dLabelHeightPx = oBound.Height;
                                    dLabelHeightMt = dLabelHeightPx * dPixelDist;
                                }   

                                RectF oBound; {
                                    oGraphicLayer.MeasureString(strTextSymbol, -1, &oFont, PointF(0,0), &stringFormat, &oBound);
                                }
                                double dSymHeight = oBound.Height * dPixelDist;

                                TxPoint oLabelPoint(spPoint->X, spPoint->Y); {
                                    if (bLabelBackFill) {
                                        oLabelPoint.Y = oLabelPoint.Y - 0.5 * dLabelHeightMt - 0.5 * dSymHeight;
                                    } else {
                                        oLabelPoint.Y = oLabelPoint.Y - 0.4 * dLabelHeightMt - 0.4 * dSymHeight;
                                    }                                        
                                }    

                                oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                vecPointLabel.push_back(spLabelInfo);
                            }                            
                        }
                    }                
                }
            } 
            else if (spSymbol->GetType() == enSymbolPointTypeCircle) {
                TxSymbolPointCirclePtr spPointSymbol = std::dynamic_pointer_cast<TxSymbolPointCircle>(spSymbol);   
                if (spPointSymbol == nullptr)
                    continue;
                                
                double dPointSize(0);
                if (spPointSymbol->UsePixelDist() == true) {
                    dPointSize = 2.0 * spPointSymbol->Radius();

                    double dXShiftRatio = spPointSymbol->XShiftRatio(); 
                    double dYShiftRatio = spPointSymbol->YShiftRatio(); 
                    
                    if (dXShiftRatio != 0.0) {
                        spPoint->X = spPoint->X + dPointSize * dXShiftRatio * dPixelDist;                        
                    }

                    if (dYShiftRatio != 0.0) {
                        spPoint->Y = spPoint->Y + dPointSize * dYShiftRatio * dPixelDist;                        
                    }
                    //★ 그냥 한번더 할수도 있음
                    oPtScreen = m_spThunderMapDisp->MapToScreen(spPoint);
                } 
                else {
                    double dXShiftRatio = spPointSymbol->XShiftRatio(); 
                    double dYShiftRatio = spPointSymbol->YShiftRatio(); 

                    if (dXShiftRatio != 0.0) {
                        spPoint->X = spPoint->X + dXShiftRatio * spPointSymbol->Radius() * 2.0;                        
                    }

                    if (dYShiftRatio != 0.0) {
                        spPoint->Y = spPoint->Y + dYShiftRatio * spPointSymbol->Radius() * 2.0;                        
                    }

                    //★ 그냥 한번더 할수도 있음
                    oPtScreen  = m_spThunderMapDisp->MapToScreen(spPoint);

                    TxPoint oPt2Screen = m_spThunderMapDisp->MapToScreen(TxPoint(spPoint->X + spPointSymbol->Radius(), spPoint->Y));
                    dPointSize = 2.0 * (oPt2Screen.X - oPtScreen.X);
                }                

                if (spPointSymbol->Fill() == true) {
                    TxHatchInfoPtr spHatch = spPointSymbol->HatchInfo();
                    if (spHatch != nullptr) {
                        Gdiplus::HatchBrush oHatchBrush(spHatch->HatchStyle(), spHatch->HatchColor(), spPointSymbol->FillColor());                            
                        oGraphicLayer.FillEllipse(&oHatchBrush, oPtScreen.ToScreenGdiPlusRectF(dPointSize, dPointSize));
                    } else {
                        Gdiplus::SolidBrush oSolidBrush = spPointSymbol->FillColor();
                        oGraphicLayer.FillEllipse(&oSolidBrush, oPtScreen.ToScreenGdiPlusRectF(dPointSize, dPointSize));
                    }                    
                }

                double dLineWidth = spPointSymbol->OutlineWidth();
                if (spPointSymbol->DrawOutLine() == true && dLineWidth > 0) {
                    Pen oLinePen(spPointSymbol->OutLineColor(), (Gdiplus::REAL)dLineWidth); 
                    oGraphicLayer.DrawEllipse(&oLinePen, oPtScreen.ToScreenGdiPlusRectF(dPointSize, dPointSize));
                }

                if (bShowLabel) {
                    ITxLabelPtr spLabel = spFeatureDisp->Label();
                    if (spLabel && spLabel->Show()){
                        CString strLabel = spLabel->Label2Str();
                        if (strLabel.GetLength() >= 1) {
                            TxPoint oLabelPoint(spPoint->X, spPoint->Y);
                            
                            if (bLabelFilter) {
                                __int64 nxFilterX = (__int64)(oLabelPoint.X / dLabelAgg);
                                __int64 nxFilterY = (__int64)(oLabelPoint.Y / dLabelAgg);

                                if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) {
                                    oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                      

                                    TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                    vecPointLabel.push_back(spLabelInfo);
                                }
                            }
                            else {
                                oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);         
                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                vecPointLabel.push_back(spLabelInfo);
                            }                            
                        }
                    }                
                }
            } 
            else if (spSymbol->GetType() == enSymbolPointTypeRect) {
                TxSymbolPointRectPtr spPointSymbol = std::dynamic_pointer_cast<TxSymbolPointRect>(spSymbol);   
                if (spPointSymbol == nullptr)
                    continue;

                double dPointWidth(0);
                if (spPointSymbol->UsePixelDist() == true) {
                    dPointWidth = spPointSymbol->Width();
                } 
                else {
                    TxPoint oPt2Screen = m_spThunderMapDisp->MapToScreen(TxPoint(spPoint->X + spPointSymbol->Width(), spPoint->Y));
                    dPointWidth = oPt2Screen.X - oPtScreen.X;
                }                

                if (spPointSymbol->Fill() == true) {
                    TxHatchInfoPtr spHatch = spPointSymbol->HatchInfo();
                    if (spHatch != nullptr) {
                        Gdiplus::HatchBrush oHatchBrush(spHatch->HatchStyle(), spHatch->HatchColor(), spPointSymbol->FillColor());                            
                        oGraphicLayer.FillRectangle(&oHatchBrush, oPtScreen.ToScreenGdiPlusRectF(dPointWidth, dPointWidth));
                    } else {
                        Gdiplus::SolidBrush oSolidBrush = spPointSymbol->FillColor();
                        oGraphicLayer.FillRectangle(&oSolidBrush, oPtScreen.ToScreenGdiPlusRectF(dPointWidth, dPointWidth));
                    }                    
                }

                double dLineWidth = spPointSymbol->OutlineWidth();
                if (spPointSymbol->DrawOutLine() == true && dLineWidth > 0) {
                    Pen oLinePen(spPointSymbol->OutLineColor(), (Gdiplus::REAL)dLineWidth); 
                    oGraphicLayer.DrawRectangle(&oLinePen, oPtScreen.ToScreenGdiPlusRectF(dPointWidth, dPointWidth));
                }

                if (bShowLabel) {
                    ITxLabelPtr spLabel = spFeatureDisp->Label();
                    if (spLabel && spLabel->Show()){
                        CString strLabel = spLabel->Label2Str();
                        if (strLabel.GetLength() >= 1) {
                            TxPoint oLabelPoint(spPoint->X, spPoint->Y);

                            if (bLabelFilter) {
                                __int64 nxFilterX = (__int64)(oLabelPoint.X / dLabelAgg);
                                __int64 nxFilterY = (__int64)(oLabelPoint.Y / dLabelAgg);

                                if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) {
                                    oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                      

                                    TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                    vecPointLabel.push_back(spLabelInfo);
                                }
                            }
                            else {
                                oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                      

                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                vecPointLabel.push_back(spLabelInfo);
                            }                            
                        }
                    }                
                }
            } 
            else if (spSymbol->GetType() == enSymbolPointTypeQuad) {
                TxSymbolPointQuadPtr spPointSymbol = std::dynamic_pointer_cast<TxSymbolPointQuad>(spSymbol);   
                if (spPointSymbol == nullptr)
                    continue;

                double dSize     = spPointSymbol->Size();
                double dHalfSize = dSize * 0.5;
                double dShift    = dPixelDist * dHalfSize * 0.5;

                TxPoint o50LeftTop     = m_spThunderMapDisp->MapToScreen(TxPoint(spPoint->X - dShift, spPoint->Y + dShift));
                TxPoint o50RightTop    = m_spThunderMapDisp->MapToScreen(TxPoint(spPoint->X + dShift, spPoint->Y + dShift));

                TxPoint o50LeftBottom  = m_spThunderMapDisp->MapToScreen(TxPoint(spPoint->X - dShift, spPoint->Y - dShift));
                TxPoint o50RightBottom = m_spThunderMapDisp->MapToScreen(TxPoint(spPoint->X + dShift, spPoint->Y - dShift));
                
                if (true/*0*/) {
                    Gdiplus::SolidBrush oSolidBrush = spPointSymbol->FillColor(0);
                    oGraphicLayer.FillRectangle(&oSolidBrush, o50LeftTop.ToScreenGdiPlusRectF(dHalfSize, dHalfSize));
                }
                if (true/*1*/) {
                    Gdiplus::SolidBrush oSolidBrush = spPointSymbol->FillColor(1);
                    oGraphicLayer.FillRectangle(&oSolidBrush, o50RightTop.ToScreenGdiPlusRectF(dHalfSize, dHalfSize));
                }
                if (true/*2*/) {
                    Gdiplus::SolidBrush oSolidBrush = spPointSymbol->FillColor(2);
                    oGraphicLayer.FillRectangle(&oSolidBrush, o50LeftBottom.ToScreenGdiPlusRectF(dHalfSize, dHalfSize));
                }
                if (true/*3*/) {
                    Gdiplus::SolidBrush oSolidBrush = spPointSymbol->FillColor(3);
                    oGraphicLayer.FillRectangle(&oSolidBrush, o50RightBottom.ToScreenGdiPlusRectF(dHalfSize, dHalfSize));
                }

                Pen oLinePen(spPointSymbol->OutLineColor(), (Gdiplus::REAL)spPointSymbol->OutlineWidth());
                if (true/*0*/) {
                    TxPoint oPtScreen = m_spThunderMapDisp->MapToScreen(spPoint);
                    oGraphicLayer.DrawRectangle(&oLinePen, oPtScreen.ToScreenGdiPlusRectF(dSize, dSize));
                }

                if (true) {
                    Gdiplus::SolidBrush oSolidBrush(Color(0, 0, 0));
                    double dPointSize = 5.0;

                    oGraphicLayer.FillEllipse(&oSolidBrush, oPtScreen.ToScreenGdiPlusRectF(dPointSize, dPointSize));
                }                

                if (bShowLabel) {
                    ITxLabelPtr spLabel = spFeatureDisp->Label();
                    if (spLabel && spLabel->Show()) {
                        CString strLabel = spLabel->Label2Str();
                        if (strLabel.GetLength() >= 1) {
                            TxPoint oLabelPoint(spPoint->X, spPoint->Y - dSize*dPixelDist);

                            if (bLabelFilter) {
                                __int64 nxFilterX = (__int64)(oLabelPoint.X / dLabelAgg);
                                __int64 nxFilterY = (__int64)(oLabelPoint.Y / dLabelAgg);

                                if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) {
                                    oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                                    TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                    vecPointLabel.push_back(spLabelInfo);
                                }
                            }
                            else {
                                oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                vecPointLabel.push_back(spLabelInfo);
                            }                            
                        }
                    }                
                }
            } 
            else if (spSymbol->GetType() == enSymbolPointTypeImage) {
                TxSymbolPointImagePtr spPointSymbol = std::dynamic_pointer_cast<TxSymbolPointImage>(spSymbol);   
                if (spPointSymbol == nullptr)
                    continue;

                Gdiplus::Bitmap* pBitmap = spPointSymbol->GetBitmap();
                if (pBitmap == nullptr)
                    continue;

                double dWidth  = spPointSymbol->Width();
                double dHeight = spPointSymbol->Height();

                oGraphicLayer.DrawImage(pBitmap, oPtScreen.ToScreenGdiPlusRectF(dWidth, dHeight), 0, 0, (Gdiplus::REAL)pBitmap->GetWidth(), (Gdiplus::REAL)pBitmap->GetHeight(), Gdiplus::UnitPixel, &m_gdiImgAtt00);
            
                if (bShowLabel) {
                    ITxLabelPtr spLabel = spFeatureDisp->Label();
                    if (spLabel && spLabel->Show()){
                        CString strLabel = spLabel->Label2Str();
                        if (strLabel.GetLength() >= 1) {
                            double dLabelHeightMt(0); {
                                RectF oBound; {
                                    oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0,0), &oLabelStrFormat, &oBound);
                                }

                                double dLabelHeightPx = oBound.Height;
                                dLabelHeightMt = dLabelHeightPx * dPixelDist;
                            }   

                            TxPoint oLabelPoint(spPoint->X, spPoint->Y - 0.5 * dHeight*dPixelDist - 0.4 * dLabelHeightMt);

                            if (bLabelFilter) {
                                __int64 nxFilterX = (__int64)(oLabelPoint.X / dLabelAgg);
                                __int64 nxFilterY = (__int64)(oLabelPoint.Y / dLabelAgg);

                                if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) {
                                    oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                      

                                    TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                    vecPointLabel.push_back(spLabelInfo);
                                }
                            }
                            else {
                                oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                      

                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                vecPointLabel.push_back(spLabelInfo);
                            }
                        }
                    }                
                }
            } 

            nxSumDrawCount++;
#pragma endregion DrawPoint
        }
        else if (spGeometry->GeometryType() == GeoPolylineType)
        {  
#pragma region DrawLine            
            TxPolylinePtr spPolyline = std::dynamic_pointer_cast<TxPolyline> (spGeometry);  
            if (spPolyline == nullptr)
                continue;

            TxEnvelope oEnvelope = spPolyline->Envelope();
            double     dFakeArea = oEnvelope.Area();

            //★ 폴리라인(면적이 0인 경우도 있다)            
            if (dFakeArea <= 0) {
                dFakeArea = oEnvelope.GetWidth() + oEnvelope.GetHeight();
            }

            if (dFakeArea <= dHideEnvArea) { //★ 픽셀거리의 50% 이하, 최대 500
                nxSumDrawCount++;
                nxFilterOutCount++;
                continue;
            }

            bool   bOffsetDraw(false);
            double dCenterLineOffsetPx(0.0);            
            double dLineWidth(1.0);
            Gdiplus::Color clrLine(Color(255,0,0));

#pragma region DrawSingleSymbol
            
            TxSymbolDirectionLinePtr spDirectionSymbol;
            bool bDirectionSymbol(false);
            int nLoop(1);
            if ( spSymbol->GetType() == enSymbolLineDirectionType ) {
                bDirectionSymbol = true;
                nLoop = 2;
                spDirectionSymbol = std::dynamic_pointer_cast<TxSymbolDirectionLine>(spSymbol);

                if (spDirectionSymbol == nullptr) {
                    nxSumDrawCount++;
                    nxFilterOutCount++;
                    continue;
                }
            }

            // ★★★ 1개의 Feature에 대해서 두번 그린다. ★★★
            for (int nDraw=0; nDraw < nLoop; nDraw++) { /*nDraw(0) : Single-Forward, nDraw(1) : Backward*/
                TxSymbolLinePtr spLineSymbol;
                if (bDirectionSymbol) 
                {
                    if (nDraw == 1) {
                        ITxSymbolPtr spBackwardSymbol = spDirectionSymbol->BackwardSymbol();
                        if (spBackwardSymbol == nullptr) 
                            continue;
                        if (spBackwardSymbol->Show() == false)
                            continue;

                        spLineSymbol = std::dynamic_pointer_cast<TxSymbolLine>(spBackwardSymbol);
                        spPolyline   = spPolyline->BackwardLine(); //★★★
                    }
                    else {
                        ITxSymbolPtr spForwardSymbol = spDirectionSymbol->ForwardSymbol();
                        if (spForwardSymbol == nullptr) 
                            continue;
                        if (spForwardSymbol->Show() == false)
                            continue;

                        spLineSymbol = std::dynamic_pointer_cast<TxSymbolLine>(spDirectionSymbol->ForwardSymbol());
                    }
                } else {
                    spLineSymbol = std::dynamic_pointer_cast<TxSymbolLine>(spSymbol);
                }                
            
                if (spLineSymbol) {
                    dLineWidth = spLineSymbol->LineWidth();
                    clrLine    = spLineSymbol->Color();
                } else {
                    continue;
                }
            
                if (dLineWidth <= 0.0) {
                    continue;
                }
            
				// ★ 물리적인 거리로 환산 . 20180409 변경 박태진 : offset 거리를 pixel이 아니라 meter 단위로 주고 싶을때의 옵션 추가.. 
				// 문제 있으면 아래 버전으로 되돌리기....
				double dOffsetDist(0.0);

                TxOffsetInfoPtr spOffsetInfo = spLineSymbol->m_spOffsetInfo;
                if (spOffsetInfo != nullptr) 
				{ 
                    if (dMapScale <= spOffsetInfo->OffsetScale()) 
					{                    
						if (spOffsetInfo->UseMeterDist())
						{
							dOffsetDist = spOffsetInfo->OffsetDist();
							if (spOffsetInfo->UseRightDirectionWidth())
							{
								dOffsetDist = dOffsetDist + (dLineWidth * 0.5 * dPixelDist);
							}
						}
						else
						{
							dCenterLineOffsetPx = spOffsetInfo->OffsetDist();

							if (spOffsetInfo->UseRightDirectionWidth())
							{
								dCenterLineOffsetPx = dCenterLineOffsetPx + dLineWidth * 0.5;
							}

							dOffsetDist = dPixelDist * dCenterLineOffsetPx;
						}
                        
                    } else {
                        dOffsetDist = 0;
                    }
                } else {
					dOffsetDist = 0;
                }

				// ★ 물리적인 거리로 환산 . 20180409 
// 				TxOffsetInfoPtr spOffsetInfo = spLineSymbol->m_spOffsetInfo;
// 				if (spOffsetInfo != nullptr) {
// 					if (dMapScale <= spOffsetInfo->OffsetScale()) {
// 						dCenterLineOffsetPx = spOffsetInfo->OffsetDist();
// 						if (spOffsetInfo->UseRightDirectionWidth()) {
// 							dCenterLineOffsetPx = dCenterLineOffsetPx + dLineWidth * 0.5;
// 						}
// 					}
// 					else {
// 						dCenterLineOffsetPx = 0;
// 					}
// 				}
// 				else {
// 					dCenterLineOffsetPx = 0;
// 				}
// 
// 				double dOffsetDist = dPixelDist * dCenterLineOffsetPx;
                
				if (dOffsetDist <= 0) {
                    bOffsetDraw = false;
                } else {
                    bOffsetDraw = true; 
                }

                //★ Filtering - 2016.08.02
                if (bDrawFilter && dFakeArea <= dPixelDist * dPixelDist) {
                    double dEnvCx(0.0), dEnvCy(0.0);
                    oEnvelope.GetCenter(dEnvCx, dEnvCy);

                    __int64 nxFilterLeft   = (__int64)(dEnvCx * 0.05);
                    __int64 nxFilterBottom = (__int64)(dEnvCy * 0.05);

                    if (LinePixelGrid.insert(TxPixelKey(nxFilterLeft, nxFilterBottom)).second == false) {
                        nxSumDrawCount++;
                        nxFilterOutCount++;
                        continue;
                    }             
                }
                        
                std::vector<TxPoint> vecPoint;
                if (bOffsetDraw == true) {                
                    vecPoint = spPolyline->GetOffsetPoint(dOffsetDist, 1.5 * dOffsetDist/*일반화 거리*/);     
                }
                else {   
                    vecPoint = spPolyline->GetPoints(1.0 * dPixelDist); // 1픽셀이하 일반화               
                }
                        
                size_t nxVertexCount = vecPoint.size();
                if (nxVertexCount < 2) //★ 두점 미만은 그리지 않는다.
                    continue;
                        
                PointF* pPoints = new PointF[nxVertexCount];
                for (size_t nPline=0; nPline<nxVertexCount; nPline++) {
                    TxPoint& oPt       = vecPoint[nPline];
                    TxPoint  oPtScreen = m_spThunderMapDisp->MapToScreen(oPt);

                    pPoints[nPline].X = (float)oPtScreen.X;
                    pPoints[nPline].Y = (float)oPtScreen.Y;
                }
            
                Pen oLinePen(clrLine, (Gdiplus::REAL)dLineWidth); {
                    Gdiplus::DashStyle enDashStyle = spLineSymbol->GetDashStyle();
                    oLinePen.SetDashStyle(enDashStyle);
                }   
                oGraphicLayer.DrawLines(&oLinePen, pPoints, (INT)nxVertexCount);    

                delete[] pPoints;

                std::map<int, ITxLineDecoSymbolPtr> mapDeco = spLineSymbol->GetDecoSymbol();
                for (auto iter = mapDeco.begin(); iter != mapDeco.end(); ++iter) {
                    ITxLineDecoSymbolPtr spDeco = iter->second;
                    if (spDeco == nullptr) 
                        continue;

                    if (dMapScale > spDeco->DrawMaxScale()) 
                        continue;

                    TxPolyline oPline(vecPoint);
                    TxPoint oDecoPt;
                    double  dAzimuth(0); {                            
                        double dDecoOffset(0); // dArrowOffset(0)-라인중심
                        if (spDeco->RightShift()) {
                            if (dDecoOffset <= 0) {
                                dDecoOffset = 0.5 * dLineWidth * dPixelDist;
                            }
                            else {
                                dDecoOffset = dOffsetDist; 
                            }
                            // 강제 우편향
                            dDecoOffset += (spDeco->HalfWidth()+1) * dPixelDist; //★ 3 + 1 = 4       
                        }                   

                        oPline.LinePercentPt(spDeco->PosRate(), oDecoPt, dAzimuth, dDecoOffset);
                        dAzimuth = TxMath::Radian2Degree(dAzimuth);
                    }
                    
                    if (true/*Drawing*/) {
                        TxPoint oPtScreen = m_spThunderMapDisp->MapToScreen(oDecoPt);                        
                        vecLineDeco.push_back(TLineDecoInfoPtr(new TLineDecoInfo(spDeco, oPtScreen, dAzimuth)));
                    }
                }
                
                if (bShowLabel) {
                    ITxLabelPtr spLabel = spFeatureDisp->Label();
                    if (spLabel == nullptr) 
                        continue;
                    if (spLabel->Show() == false) 
                        continue;

                    CString strLabel(_T(""));
                    if (bDirectionSymbol) {
                        TxDirectionLabelPtr spDirectionLabel = std::dynamic_pointer_cast<TxDirectionLabel>(spLabel);
                        if (spDirectionLabel == nullptr)
                            continue;

                        if (nDraw == 1) {
                            strLabel = spDirectionLabel->BackwardLabel;
                        } else {
                            strLabel = spDirectionLabel->ForwardLabel;
                        }
                    } else {
                        strLabel = spLabel->Label2Str();
                    }
                    
                    if (strLabel.GetLength() >= 1) {
                        double dLabelOffset(0); 
                        if (dOffsetDist <= 0) {
                            dLabelOffset = (0.5 * dLineWidth * dPixelDist);
                        } else {
                            dLabelOffset = (dLineWidth * dPixelDist);
                        }

                        double dLabelHeightMt(0); {
                            RectF oBound; {
                                oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0,0), &oLabelStrFormat, &oBound);
                            }

                            double dLabelHeightPx = oBound.Height;
                            dLabelHeightMt = dLabelHeightPx * dPixelDist;
                        }   

                        double dPointOffset(0);
                        if (bLabelBackFill) {
                            dPointOffset = dLabelOffset + 0.50 * dLabelHeightMt + dPixelDist * 2.5;
                        } else {
                            dPointOffset = dLabelOffset + 0.50 * dLabelHeightMt;
                        }
                        double dAngle(0.0);
                    
                        TxPoint oOnPoint, oOffsetPoint;
                        spPolyline->LineOnPoint(spPolyline->Length() * 0.5, oOnPoint, oOffsetPoint, dAngle, dPointOffset);
                        TxPoint oLabelPoint = m_spThunderMapDisp->MapToScreen(oOffsetPoint);
                    
                        if (bLabelFilter) {
                            __int64 nxFilterX = (__int64)(oOffsetPoint.X / dLabelAgg);
                            __int64 nxFilterY = (__int64)(oOffsetPoint.Y / dLabelAgg);

                            int& nLabelCount = LineLabelGrid[TxPixelKey(nxFilterX, nxFilterY)];
                            nLabelCount++;

                            if (nLabelCount <= 2) {
                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint, dAngle));
                                vecLineLabel.push_back(spLabelInfo);
                            }
                        }
                        else {
                            TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint, dAngle));
                            vecLineLabel.push_back(spLabelInfo);
                        }                        
                    }
                }
            }
#pragma endregion DrawSingleSymbol
            nxSumDrawCount++;                 
#pragma endregion DrawLine
        }
        else if (spGeometry->GeometryType() == GeoPolygonType)
        {
#pragma region DrawGeoPolygonType     
            TxPolygonPtr spSimplePolygon = std::dynamic_pointer_cast<TxPolygon>(spGeometry);  
            if (spSimplePolygon == nullptr)
                continue;
            
            TxEnvelope oEnvelope = spSimplePolygon->Envelope();
            double     dEnvArea  = oEnvelope.Area();
            if (dEnvArea <= dHideEnvArea) {
                nxSumDrawCount++;
                nxFilterOutCount++;
                continue;
            }

            std::vector<TxPoint> vecPoint = spSimplePolygon->OutRing();
            size_t outRingVertexCount = vecPoint.size();

            if (outRingVertexCount < 4)
                continue; 

            /*마지막제외*/
            outRingVertexCount--;

            PointF* pOutRingPoints = new PointF[outRingVertexCount];
            for (size_t nxVertex=0; nxVertex<outRingVertexCount; nxVertex++)
            {
                TxPoint& oPt       = vecPoint[nxVertex];
                TxPoint  oPtScreen = m_spThunderMapDisp->MapToScreen(oPt);

                pOutRingPoints[nxVertex].X = (float)oPtScreen.X;
                pOutRingPoints[nxVertex].Y = (float)oPtScreen.Y;
            }

            size_t nxHoleCount = spSimplePolygon->GetHoleCount();
            if (nxHoleCount > 0) 
            {
                GraphicsPath path;
                path.AddPolygon(pOutRingPoints, (INT)outRingVertexCount);                

                for (size_t nxHole=0; nxHole<nxHoleCount; nxHole++) {
                    std::vector<TxPoint> vecHolePoint = spSimplePolygon->GetHolePoints(nxHole);
                    size_t nxHoleVertexCount = vecHolePoint.size();

                    if (nxHoleVertexCount < 4)
                        continue; 

                    /*마지막제외*/
                    nxHoleVertexCount--;

                    PointF* pHolePoints = new PointF[nxHoleVertexCount];
                    for (size_t nxVertex=0; nxVertex<nxHoleVertexCount; nxVertex++)
                    {
                        TxPoint& oPt       = vecHolePoint[nxVertex];
                        TxPoint  oPtScreen = m_spThunderMapDisp->MapToScreen(oPt);

                        pHolePoints[nxVertex].X = (float)oPtScreen.X;
                        pHolePoints[nxVertex].Y = (float)oPtScreen.Y;
                    }

                    path.AddPolygon(pHolePoints, (INT)nxHoleVertexCount);
                    delete[] pHolePoints;
                }   
                
                TxSymbolType oSymbolType = spSymbol->GetType();
                if (oSymbolType == enSymbolPolygonType){
                    TxSymbolPolygonPtr spSymbolPoly = std::dynamic_pointer_cast<TxSymbolPolygon>(spSymbol);
                    if (spSymbolPoly) {
                        if (spSymbolPoly->Fill()) {
                            TxHatchInfoPtr spHatch = spSymbolPoly->HatchInfo();
                            if (spHatch != nullptr) {
                                Gdiplus::HatchBrush oHatchBrush(spHatch->HatchStyle(), spHatch->HatchColor(), spSymbolPoly->FillColor());                            
                                oGraphicLayer.FillPath(&oHatchBrush, &path);
                            } else {
                                Gdiplus::SolidBrush oSolidBrush(spSymbolPoly->FillColor());
                                oGraphicLayer.FillPath(&oSolidBrush, &path);
                            }                            
                        }

                        double dOutLineWidth = spSymbolPoly->OutLineWidth();
                        if (spSymbolPoly->DrawOutLine() == true && dOutLineWidth > 0) {
                            Pen oLinePen(spSymbolPoly->OutLineColor(), (Gdiplus::REAL)dOutLineWidth);   
                            oGraphicLayer.DrawPath(&oLinePen, &path);
                        }
                    }
                }
            }
            else 
            {
                TxSymbolType oSymbolType = spSymbol->GetType();
                if (oSymbolType == enSymbolPolygonType) {
                    TxSymbolPolygonPtr spSymbolPoly = std::dynamic_pointer_cast<TxSymbolPolygon>(spSymbol);
                    if (spSymbolPoly) {
                        if (spSymbolPoly->Fill()) {
                            TxHatchInfoPtr spHatch = spSymbolPoly->HatchInfo();
                            if (spHatch != nullptr) {
                                Gdiplus::HatchBrush oHatchBrush(spHatch->HatchStyle(), spHatch->HatchColor(), spSymbolPoly->FillColor());                            
                                oGraphicLayer.FillPolygon(&oHatchBrush, pOutRingPoints, (INT)outRingVertexCount);
                            } else {
                                Gdiplus::SolidBrush oSolidBrush(spSymbolPoly->FillColor());
                                oGraphicLayer.FillPolygon(&oSolidBrush, pOutRingPoints, (INT)outRingVertexCount);
                            }                            
                        }

                        double dOutLineWidth = spSymbolPoly->OutLineWidth();
                        if (spSymbolPoly->DrawOutLine() == true && dOutLineWidth > 0) {
                            Pen oLinePen(spSymbolPoly->OutLineColor(), (Gdiplus::REAL)dOutLineWidth);   
                            oGraphicLayer.DrawPolygon(&oLinePen, pOutRingPoints, (INT)outRingVertexCount);
                        }
                    }
                }
            }

            if (bShowLabel) {
				ITxLabelPtr spLabel = spFeatureDisp->Label();

				CString strLabel(_T(""));
				if (spLabel && spLabel->Show())
					strLabel = spLabel->Label2Str();
                
                if (strLabel.GetLength() >= 1) {  
                    TxPoint oLabelPoint = spSimplePolygon->Centroid();

                    if (bLabelFilter) {
                        __int64 nxFilterX = (__int64)(oLabelPoint.X / dLabelAgg);
                        __int64 nxFilterY = (__int64)(oLabelPoint.Y / dLabelAgg);

                        if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) {
                            oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                            TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                            vecPolyLabel.push_back(spLabelInfo);
                        }
                    }
                    else {
                        oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                        TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                        vecPolyLabel.push_back(spLabelInfo);
                    }
                }
            }
            
            delete[] pOutRingPoints;
            nxSumDrawCount++;   
#pragma endregion DrawGeoPolygonType
        } 
        else if (spGeometry->GeometryType() == GeoMPolygonType) {
#pragma region DrawGeoMPolygonType
            TxMPolygonPtr spPolygon = std::dynamic_pointer_cast<TxMPolygon>(spGeometry);  
            if (spPolygon == nullptr)
                continue;

            size_t nxSubCount = spPolygon->SubPolygonCount();
            for (size_t nxPoly=0; nxPoly<nxSubCount; nxPoly++)
            {
                TxPolygon* pSubPolygon = spPolygon->SubPolygon(nxPoly);
                TxEnvelope oEnvelope = pSubPolygon->Envelope();
                double     dEnvArea  = oEnvelope.Area();
                if (dEnvArea <= dHideEnvArea) {
                    continue;
                }

                std::vector<TxPoint> vecPoint = pSubPolygon->OutRing();
                size_t outRingVertexCount = vecPoint.size();

                if (outRingVertexCount < 4)
                    continue; 

                /*마지막제외*/
                outRingVertexCount--;

                PointF* pOutRingPoints = new PointF[outRingVertexCount];
                for (size_t nxVertex=0; nxVertex<outRingVertexCount; nxVertex++)
                {
                    TxPoint& oPt       = vecPoint[nxVertex];
                    TxPoint  oPtScreen = m_spThunderMapDisp->MapToScreen(oPt);

                    pOutRingPoints[nxVertex].X = (float)oPtScreen.X;
                    pOutRingPoints[nxVertex].Y = (float)oPtScreen.Y;
                }

                size_t nxHoleCount = pSubPolygon->GetHoleCount();
                if (nxHoleCount > 0)
                {
                    GraphicsPath path;
                    path.AddPolygon(pOutRingPoints, (INT)outRingVertexCount);                

                    for (size_t nxHole=0; nxHole<nxHoleCount; nxHole++)
                    {
                        std::vector<TxPoint> vecHolePoint = pSubPolygon->GetHolePoints(nxHole);
                        size_t nxHoleVertexCount = vecHolePoint.size();

                        if (nxHoleVertexCount < 4)
                            continue; 

                        /*마지막제외*/
                        nxHoleVertexCount--;

                        PointF* pHolePoints = new PointF[nxHoleVertexCount];
                        for (size_t nxVertex=0; nxVertex<nxHoleVertexCount; nxVertex++)
                        {
                            TxPoint& oPt       = vecHolePoint[nxVertex];
                            TxPoint  oPtScreen = m_spThunderMapDisp->MapToScreen(oPt);

                            pHolePoints[nxVertex].X = (float)oPtScreen.X;
                            pHolePoints[nxVertex].Y = (float)oPtScreen.Y;
                        }

                        path.AddPolygon(pHolePoints, (INT)nxHoleVertexCount);
                        delete[] pHolePoints;
                    }   

                    TxSymbolType oSymbolType = spSymbol->GetType();
                    if (oSymbolType == enSymbolPolygonType){
                        TxSymbolPolygonPtr spSymbolPoly = std::dynamic_pointer_cast<TxSymbolPolygon>(spSymbol);
                        if (spSymbolPoly) {
                            if (spSymbolPoly->Fill()) {
                                TxHatchInfoPtr spHatch = spSymbolPoly->HatchInfo();
                                if (spHatch != nullptr) {
                                    Gdiplus::HatchBrush oHatchBrush(spHatch->HatchStyle(), spHatch->HatchColor(), spSymbolPoly->FillColor());                            
                                    oGraphicLayer.FillPath(&oHatchBrush, &path);
                                } else {
                                    Gdiplus::SolidBrush oSolidBrush(spSymbolPoly->FillColor());
                                    oGraphicLayer.FillPath(&oSolidBrush, &path);
                                }                                
                            }

                            double dOutLineWidth = spSymbolPoly->OutLineWidth();
                            if (dOutLineWidth > 0) {
                                Pen oLinePen(spSymbolPoly->OutLineColor(), (Gdiplus::REAL)dOutLineWidth);   
                                oGraphicLayer.DrawPath(&oLinePen, &path);
                            }
                        }
                    }
                }
                else
                {
                    TxSymbolType oSymbolType = spSymbol->GetType();
                    if (oSymbolType == enSymbolPolygonType) {
                        TxSymbolPolygonPtr spSymbolPoly = std::dynamic_pointer_cast<TxSymbolPolygon>(spSymbol);
                        if (spSymbolPoly) {
                            if (spSymbolPoly->Fill()) {
                                TxHatchInfoPtr spHatch = spSymbolPoly->HatchInfo();
                                if (spHatch != nullptr) {
                                    Gdiplus::HatchBrush oHatchBrush(spHatch->HatchStyle(), spHatch->HatchColor(), spSymbolPoly->FillColor());                            
                                    oGraphicLayer.FillPolygon(&oHatchBrush, pOutRingPoints, (INT)outRingVertexCount);
                                } else {
                                    Gdiplus::SolidBrush oSolidBrush(spSymbolPoly->FillColor());
                                    oGraphicLayer.FillPolygon(&oSolidBrush, pOutRingPoints, (INT)outRingVertexCount);
                                }                                
                            }

                            double dOutLineWidth = spSymbolPoly->OutLineWidth();
                            if (spSymbolPoly->DrawOutLine() == true && spSymbolPoly->DrawOutLine() == true && dOutLineWidth > 0) {
                                Pen oLinePen(spSymbolPoly->OutLineColor(), (Gdiplus::REAL)dOutLineWidth);   
                                oGraphicLayer.DrawPolygon(&oLinePen, pOutRingPoints, (INT)outRingVertexCount);
                            }
                        }
                    }
                }

                if (bShowLabel) {
					ITxLabelPtr spLabel = spFeatureDisp->Label();

					CString strLabel(_T(""));
					if (spLabel && spLabel->Show())
						strLabel = spLabel->Label2Str();

                    if (strLabel.GetLength() >= 1) {
                        double dPtx, dPty;
                        pSubPolygon->Envelope().GetCenter(dPtx, dPty);
                        TxPoint oLabelPoint(dPtx, dPty);

                        if (bLabelFilter) {
                            __int64 nxFilterX = (__int64)(oLabelPoint.X / dLabelAgg);
                            __int64 nxFilterY = (__int64)(oLabelPoint.Y / dLabelAgg);

                            if (PointLabelGrid.insert(TxPixelKey(nxFilterX, nxFilterY)).second == true) {
                                oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                                TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                                vecPolyLabel.push_back(spLabelInfo);
                            }
                        }
                        else {
                            oLabelPoint = m_spThunderMapDisp->MapToScreen(oLabelPoint);                        

                            TLabelInfoPtr spLabelInfo = TLabelInfoPtr(new TLabelInfo(strLabel, oLabelPoint));
                            vecPolyLabel.push_back(spLabelInfo);
                        }                        
                    }
                }
                delete[] pOutRingPoints;
            }
            nxSumDrawCount++;  
#pragma endregion DrawGeoMPolygonType
        }

        // 중간 그려진 것을 업데이트    
        if (bFlicker && pBmpStack != nullptr && pBmpLayer != nullptr && bStopping == false) 
        {  
            __int64 nxCurTick  = GetMapTickCount();
            __int64 nxInterval = nxCurTick - nxFlickerTick; 

            if (nxInterval >= m_nFlickerGap) 
            {   
                nMapStauts = GetMapDrawStatus();
                if (nMapStauts == 0 || nxCommandID != MapCommandID) {  
                    bStopping = true;
                }

                if (bStopping == false) {
                    TxLogDebugVisitor();
                    CDC bufferDC; {
                        bufferDC.CreateCompatibleDC(&oMapDC);
                    }
                    CBitmap oBufferBmp; {
                        oBufferBmp.CreateCompatibleBitmap(&oMapDC, rect.Width(), rect.Height());
                    }    
                                        
                    int nAlpha(255);                    
                    if (m_bAlphaFlicker) {
                        //★ 100 ... 200 : 만이 그려질 수로 진하게...
                        nAlpha = (int)nxSumDrawCount / 10000 * 5 + 145;
                        if (nAlpha > 200) {
                            nAlpha = 200;
                        }
                    }
                    
                    // 메모리 비트맵 생성
                    if (bufferDC.m_hDC != nullptr) {
                        CBitmap* pOldBitmap = bufferDC.SelectObject(&oBufferBmp);  
                        Gdiplus::Graphics xGraphics(bufferDC); 
                                                                        
                        xGraphics.DrawImage(pBmpStack, 0, 0);
                        xGraphics.DrawImage(pBmpLayer, 0, 0);    
                        
                        //★ 최초 한번은 바탕을 흰색으로 그린다. 안그러면 이전 뷰가 겹쳐보인다.
                        if (m_bAlphaFlicker && nAlpha <= 150) {
                            CDC bufferDC; {
                                bufferDC.CreateCompatibleDC(&oMapDC);
                            }
                            CBitmap oBufferBmp; {
                                oBufferBmp.CreateCompatibleBitmap(&oMapDC, rect.Width(), rect.Height());
                            }    

                            if (bufferDC.m_hDC != nullptr) {
                                // 메모리 비트맵 생성
                                CBitmap* pOldBitmap = bufferDC.SelectObject(&oBufferBmp);  
                                Gdiplus::Graphics xGraphics(bufferDC); {
                                    xGraphics.Clear(Gdiplus::Color::White);
                                    DrawLoadingMessage(xGraphics);
                                }

                                oMapDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);
                                bufferDC.SelectObject(pOldBitmap);           
                                bufferDC.DeleteDC();
                            }
                        }

                        if (m_bAlphaFlicker) {
                            if (nAlpha >= 255) {
                                oMapDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);    
                            }
                            else {
                                m_bf.SourceConstantAlpha = nAlpha;
                                oMapDC.AlphaBlend(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, rect.Width(), rect.Height(), m_bf);
                            }
                        }
                        else {
                            oMapDC.BitBlt(0, 0, rect.Width(), rect.Height(), &bufferDC, 0, 0, SRCCOPY);   
                        }                        

                        bufferDC.SelectObject(pOldBitmap);
                    }

                    bufferDC.DeleteDC();
                    TxLogDebugVisitor();
                }

                nxFlickerTick = GetMapTickCount();
            }
        }
    }

    //★ 포인트 데코레이션 그리기
    nMapStauts = GetMapDrawStatus();
    if (nMapStauts == 0 || nxCommandID != MapCommandID) {  
        bStopping = true;
    }
    for (size_t i=0; bStopping == false && i<vecPointDeco.size(); i++) {
        TPointDecoInfoPtr spInfo = vecPointDeco[i]; 
        TxPoint& oPtScreen = spInfo->m_oPt;

        oGraphicLayer.TranslateTransform((Gdiplus::REAL)oPtScreen.X, (Gdiplus::REAL)oPtScreen.Y);  
        //★ 
        ITxPointDecoSymbolPtr spDeco = spInfo->m_spDecoSymbol; {
            spDeco->Draw(oGraphicLayer);
        }

        oGraphicLayer.ResetTransform();
    }
    
    //★ 라인데코레이션 그리기
    nMapStauts = GetMapDrawStatus();
    if (nMapStauts == 0 || nxCommandID != MapCommandID) {  
        bStopping = true;
    }
    for (size_t i=0; bStopping == false && i<vecLineDeco.size(); i++) {
        TLineDecoInfoPtr spInfo = vecLineDeco[i]; 
        TxPoint& oPtScreen = spInfo->m_oPt;
        
        oGraphicLayer.TranslateTransform((Gdiplus::REAL)oPtScreen.X, (Gdiplus::REAL)oPtScreen.Y);  
        oGraphicLayer.RotateTransform((Gdiplus::REAL)spInfo->m_dAngle);
        //★ 
        ITxLineDecoSymbolPtr spDeco = spInfo->m_spDecoSymbol; {
            spDeco->Draw(oGraphicLayer);
        }        

        oGraphicLayer.ResetTransform();
    }

    nMapStauts = GetMapDrawStatus();
    if (nMapStauts == 0 || nxCommandID != MapCommandID) {  
        bStopping = true;
    }

    if (bStopping == false) 
    {
        Gdiplus::SolidBrush oSolidBrush(spLabelSymbol->BackgroundColor());
        Gdiplus::Pen  oPen(Gdiplus::Color(128, 128, 128), 2);       

        //★ 라벨 품질 설정
        DisplayQualityLabel(oGraphicLayer);
        //DisplayQuality(xGraphics, 2);

        for (size_t m=0; bStopping == false && m<vecPointLabel.size(); m++)
        {
            TLabelInfoPtr spLabelInfo = vecPointLabel[m];
            {
                CString strLabel = spLabelInfo->m_strLabel;            
                TxPoint oLabelPt = spLabelInfo->m_oPt;
                
                oGraphicLayer.TranslateTransform((Gdiplus::REAL)oLabelPt.X, (Gdiplus::REAL)oLabelPt.Y);

                if (bLabelBackFill) {
                    RectF oBound; {
                        oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0, 0), &oLabelStrFormat, &oBound);                    
                        oGraphicLayer.DrawRectangle(&oPen, oBound);
                        oGraphicLayer.FillRectangle(&oSolidBrush, oBound);                    
                    }
                }                

                Gdiplus::PointF zeroPt(0.0f, 0.0f);
                oGraphicLayer.DrawString(strLabel, (INT)wcslen(strLabel), &oLabelFont, zeroPt, &oLabelStrFormat, &oLabelFontBrush);
                oGraphicLayer.ResetTransform();
            }

            nxSumDrawCount++;
        }

        nMapStauts = GetMapDrawStatus();
        if (nMapStauts == 0 || nxCommandID != MapCommandID) {  
            bStopping = true;
        }

        for (size_t m=0; bStopping == false && m<vecLineLabel.size(); m++)
        {
            TLabelInfoPtr spLabelInfo = vecLineLabel[m];
            {
                CString strLabel = spLabelInfo->m_strLabel;
                double  dAzimuth = TxMath::Radian2Degree(spLabelInfo->m_dAngle, true);
                TxPoint oLabelPt = spLabelInfo->m_oPt;

                oGraphicLayer.TranslateTransform((Gdiplus::REAL)oLabelPt.X, (Gdiplus::REAL)oLabelPt.Y);            
                if ( 0.0 <= dAzimuth && dAzimuth <= 180.0)
                    oGraphicLayer.RotateTransform((Gdiplus::REAL)dAzimuth-90.0f);
                else
                    oGraphicLayer.RotateTransform((Gdiplus::REAL)dAzimuth-270.0f);
                
                if (bLabelBackFill) {
                    RectF oBound; {
                        oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0, 0), &oLabelStrFormat, &oBound);                    
                        oGraphicLayer.DrawRectangle(&oPen, oBound);
                        oGraphicLayer.FillRectangle(&oSolidBrush, oBound);                    
                    }
                }       

                Gdiplus::PointF zeroPt(0.0f, 0.0f);
                oGraphicLayer.DrawString(strLabel, (INT)wcslen(strLabel), &oLabelFont, zeroPt, &oLabelStrFormat, &oLabelFontBrush);
                oGraphicLayer.ResetTransform();
            }

            nxSumDrawCount++;
        }

        nMapStauts = GetMapDrawStatus();
        if (nMapStauts == 0 && nxCommandID != MapCommandID) {  
            bStopping = true;
        }
               
        for (size_t m=0; bStopping == false && m<vecPolyLabel.size(); m++)
        {
            TLabelInfoPtr spLabelInfo = vecPolyLabel[m];
            {
                CString strLabel = spLabelInfo->m_strLabel;            
                TxPoint oLabelPt = spLabelInfo->m_oPt;

                oGraphicLayer.TranslateTransform((Gdiplus::REAL)oLabelPt.X, (Gdiplus::REAL)oLabelPt.Y);

                if (bLabelBackFill) {
                    RectF oBound; {
                        oGraphicLayer.MeasureString(strLabel, -1, &oLabelFont, PointF(0, 0), &oLabelStrFormat, &oBound);                    
                        oGraphicLayer.DrawRectangle(&oPen, oBound);
                        oGraphicLayer.FillRectangle(&oSolidBrush, oBound);                    
                    }
                }       

                Gdiplus::PointF zeroPt(0.0f, 0.0f);
                oGraphicLayer.DrawString(strLabel, (INT)wcslen(strLabel), &oLabelFont, zeroPt, &oLabelStrFormat, &oLabelFontBrush);
                oGraphicLayer.ResetTransform();
            }

            nxSumDrawCount++;
        }  
    }

    if (false) {
        strTemp.Format(_T("DrawCommand[%d]- MapGDIPlusDrawing End[%s]->[%d] \t- Fiter[%d] \t= %d]<--"), 
                           nxCommandID, spLayer->LayerInfo()->Name(), nxCount, nxFilterOutCount, nxCount-nxFilterOutCount);
        TxLogDebug((LPCTSTR)strTemp);
    }
}

void ThunderMapdrawProcess::Resize( UINT width, UINT height )
{
    return;
}

CString ThunderMapdrawProcess::ToTime()
{
    SYSTEMTIME   now;
    GetLocalTime(&now);

    CString strTime(_T(""));
    strTime.Format(_T("%4d-%2d-%2d %2d:%2d:%2d %3d"), now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond, now.wMilliseconds);

    return strTime;
}

__int64 ThunderMapdrawProcess::GetMapTickCount()
{
    //GetTickCount64()
    return GetTickCount();
}

void ThunderMapdrawProcess::SetMapDrawStatus(int nStatus )
{
    //★ 0 : 그리는 작업을 종료 명령 요청
    //★ 1 : 지도가 그려지고 있는 상태
    //★ 2 : 지도가 그리는 작업을 완료했다. 
    //       그리기 대기 상태이다.

    Lock.Lock();
    MapProcessStatus = nStatus;
    Lock.Unlock();    
}

int ThunderMapdrawProcess::GetMapDrawStatus()
{
    //★ 0 : 그리는 작업을 종료 명령 요청
    //★ 1 : 지도가 그려지고 있는 상태
    //★ 2 : 지도가 그리는 작업을 완료했다. 
    //       그리기 대기 상태이다.

    int nStatus(0);
    Lock.Lock();    
    nStatus = MapProcessStatus;
    Lock.Unlock();

    return nStatus;
}

Bitmap* ThunderMapdrawProcess::MapImage()
{    
    //★ 리턴 받은 곳에서 delete
    Bitmap* pBitmap = nullptr;
    // 지도(레이어)
    if (m_pBitmapBackground != nullptr) {
        Lock.Lock();
        pBitmap = m_pBitmapBackground->Clone(0, 0, m_pBitmapBackground->GetWidth(), m_pBitmapBackground->GetHeight(), m_pBitmapBackground->GetPixelFormat());
        Lock.Unlock();
    }

    return pBitmap;
}

void ThunderMapdrawProcess::Flash( std::vector<TxFeatureDispPtr>& vecFeatureDisp, int nTimeInterval, int nCount )
{
    if (m_bFirstTime == true || m_hwnd == nullptr)
        return;

    if (GetMapDrawStatus() != 2) {           
        return;
    }

    CWnd* pHandleWnd = CWnd::FromHandlePermanent (m_hwnd); //CWnd::FromHandle(m_hwnd)
    if (pHandleWnd == nullptr)
        return;

    // 시간간격을 두고 반복한다.
    RECT rc;  GetClientRect(m_hwnd, &rc); CRect rect(rc);
    CClientDC oMapDC(pHandleWnd);      

    Bitmap gdiBmpMap(rect.Width(), rect.Height()/*, PixelFormat32bppPARGB*/);
    Graphics oMapGraphics(&gdiBmpMap);   
    DisplayQuality(oMapGraphics, 2);

    Bitmap gdiBmpStack(rect.Width(), rect.Height()/*, PixelFormat32bppPARGB*/);
    Graphics xGraphics(&gdiBmpStack);   
    DisplayQuality(xGraphics, 2);
    
    Bitmap* pBitmap = MapImage();    
    if (pBitmap == nullptr) {
        oMapGraphics.Clear(Color::White);
        xGraphics.Clear(Color::White);
    } 
    else {
        oMapGraphics.DrawImage(pBitmap, 0, 0);  
        xGraphics.DrawImage(pBitmap, 0, 0);  
    }     
    DeleteNull(pBitmap);

    TxLayerInfoPtr spLayerInfo(new TxLayerInfo(GeoShapeType, _T("Flash"), _T("Flash")));
    TxFeatureUserLayerPtr spLayer(new TxFeatureUserLayer(spLayerInfo));
    if (false)
        spLayer->PutFeatureDisp(vecFeatureDisp);

    Lock.Lock();
    size_t nxCommandID = MapCommandID;
    Lock.Unlock();

    MapGDIPlusDrawing(nxCommandID, oMapDC, xGraphics, spLayer, vecFeatureDisp, rect); 

    int nFlashCount(0);
    for (int i=0; true; i++) {
        if (i % 2 == 0) { // Flash Image
            Graphics graphics(oMapDC);
            DisplayQuality(graphics, 2);
            graphics.DrawImage(&gdiBmpStack, 0, 0);
            nFlashCount++;
            if (nFlashCount == nCount) {
                ::Sleep(nTimeInterval);
                break;
            }
        } else { // Origin Image
            Graphics graphics(oMapDC);
            DisplayQuality(graphics, 2);
            graphics.DrawImage(&gdiBmpMap, 0, 0);
        }

        ::Sleep(nTimeInterval);
    }

    Graphics graphics(oMapDC);
    DisplayQuality(graphics, 1);
    graphics.DrawImage(&gdiBmpMap, 0, 0);
}

void ThunderMapdrawProcess::DisplayQuality( Graphics& graphics, int nFlag )
{
    switch (nFlag)
    {    
    case 0: //★ High-High Quality(최적품질)
        {
            graphics.SetCompositingMode   (CompositingModeSourceOver);     //★ 고정  
            graphics.SetCompositingQuality(CompositingQualityHighQuality); //★ 민감옵션
            graphics.SetPixelOffsetMode   (PixelOffsetModeNone);           //★ PixelOffsetModeNone
            graphics.SetSmoothingMode     (SmoothingModeAntiAlias);
            graphics.SetInterpolationMode (InterpolationModeDefault);
        }
        break;
    case 1 : //★ 속도와 품질
        {
            graphics.SetCompositingMode   (CompositingModeSourceOver);    //★ 고정  
            graphics.SetCompositingQuality(CompositingQualityHighSpeed);  //★ 민감옵션
            graphics.SetPixelOffsetMode   (PixelOffsetModeNone);          //★ PixelOffsetModeNone
            graphics.SetSmoothingMode     (SmoothingModeAntiAlias);
            graphics.SetInterpolationMode (InterpolationModeDefault);
        }
        break;   
    case 2: //★ High Speed - GDI
        {
            graphics.SetCompositingMode   ( CompositingModeSourceOver );
            graphics.SetCompositingQuality( CompositingQualityInvalid );
            graphics.SetPixelOffsetMode   ( PixelOffsetModeNone );
            graphics.SetSmoothingMode     ( SmoothingModeNone );
            graphics.SetInterpolationMode ( InterpolationModeInvalid );
        }
    case 3: //★ Reserved (1~2 사이)
        {
            graphics.SetCompositingMode   (CompositingModeSourceOver);   //★ 고정  
            graphics.SetCompositingQuality(CompositingQualityHighSpeed); //★ 민감옵션
            graphics.SetPixelOffsetMode   (PixelOffsetModeNone);   
            graphics.SetSmoothingMode     (SmoothingModeAntiAlias);
            graphics.SetInterpolationMode (InterpolationModeDefault);
        }
        break; 
    default: 
        {
            graphics.SetCompositingMode   (CompositingModeSourceOver);
            graphics.SetCompositingQuality(CompositingQualityInvalid);
            graphics.SetPixelOffsetMode   (PixelOffsetModeNone ); 
            graphics.SetSmoothingMode     (SmoothingModeNone);
            graphics.SetInterpolationMode (InterpolationModeDefault);            
        }
        break;
    }
}

void ThunderMapdrawProcess::DisplayQualityLabel( Graphics& graphics )
{
    graphics.SetCompositingMode   ( CompositingModeSourceOver );
    graphics.SetCompositingQuality( CompositingQualityHighSpeed );
    graphics.SetPixelOffsetMode   ( PixelOffsetModeHighSpeed );
    graphics.SetSmoothingMode     ( SmoothingModeNone );
    graphics.SetInterpolationMode ( InterpolationModeDefault );
    graphics.SetTextRenderingHint ( TextRenderingHintAntiAlias);
}

void ThunderMapdrawProcess::MakeGrayScale( Bitmap& oDest, Bitmap* pOrigin )
{
    Graphics graphics(&oDest);
    ColorMatrix  colorMatrix  = { 
        .3f, .3f, .3f, 0.f, 0,
        .59f, .59f, .59f, 0, 0,
        .11f, .11f, .11f, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 1
    };
    
    ImageAttributes attributes;
    attributes.SetColorMatrix(&colorMatrix);

    graphics.DrawImage(pOrigin, Rect(0,0,oDest.GetWidth(), oDest.GetHeight()), 0, 0, pOrigin->GetWidth(), pOrigin->GetHeight(), Gdiplus::UnitPixel, &attributes);
}

bool ThunderMapdrawProcess::DrawSizeDesc( TxFeatureDispPtr& spLeft, TxFeatureDispPtr& spRight )
{
    double dLeftWidth(0), dRightWidth(0);

    if (spLeft == nullptr || spRight == nullptr) {
        if (spLeft != 0)
            dLeftWidth = 1;

        if (spRight != 0)
            dRightWidth = 1;

        return dLeftWidth > dRightWidth;
    }            
    
    if (true) {
        ITxSymbolPtr spSymbol = spLeft->Symbol(); 
        if (spSymbol != nullptr) {
            dLeftWidth = spSymbol->DrawOrderOptionValue();
        }        
    }
    
    if (true) {
        ITxSymbolPtr spSymbol = spRight->Symbol();
        if (spSymbol != nullptr) {
            dRightWidth = spSymbol->DrawOrderOptionValue();
        }
    }

    return dLeftWidth > dRightWidth;
}

bool ThunderMapdrawProcess::DrawIDAsc( TxFeatureDispPtr& spLeft, TxFeatureDispPtr& spRight )
{
    __int64 dLeftID(0), dRightID(0);

    if (spLeft == nullptr || spRight == nullptr) {
        if (spLeft != 0)
            dLeftID = 1;

        if (spRight != 0)
            dRightID = 1;

        return dLeftID < dRightID;
    }            

    if (true) {
        TxFeaturePtr spFeature = spLeft->Feature();
        if (spFeature)
            dLeftID = spFeature->TxObjectID;
    }

    if (true) {
        TxFeaturePtr spFeature = spRight->Feature();
        if (spFeature)
            dRightID = spFeature->TxObjectID;
    }

    return dLeftID < dRightID;
}

void ThunderMapdrawProcess::DrawLoadingMessage(Gdiplus::Graphics& graphics)
{
    if (m_bShowLoadingMessage == false || m_strLoadingMessage.GetLength() < 1)
        return;

    RECT rc;  
    GetClientRect(m_hwnd, &rc); 
    CRect rect(rc);
    Gdiplus::PointF pointF( (Gdiplus::REAL)5, (Gdiplus::REAL)(rect.Height()-20) );

    Gdiplus::FontFamily oFontFamily(L"나눔고딕코딩");
    Gdiplus::Font oFont(&oFontFamily, 10, FontStyleBoldItalic, UnitPixel);

    SolidBrush   oFontBrush(Color(220, 0, 0, 255));
    StringFormat stringFormat; {
        stringFormat.SetAlignment(StringAlignmentNear);
        stringFormat.SetLineAlignment(StringAlignmentNear);
    }    

    graphics.DrawString(m_strLoadingMessage, (INT)wcslen(m_strLoadingMessage), &oFont, pointF, &stringFormat, &oFontBrush);
}




/*****************************************************************************
* 2019.03.08 : 지도 배경색상 변경기능 추가
*****************************************************************************/
void ThunderMapdrawProcess::SetBackgroundColor(Gdiplus::Color crBackground)
{
	m_crBackground = crBackground;
}


Gdiplus::Color ThunderMapdrawProcess::GetBackgroundColor()
{
	return m_crBackground;
}
/****************************************************************************/

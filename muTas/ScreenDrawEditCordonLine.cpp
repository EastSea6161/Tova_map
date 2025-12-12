#include "StdAfx.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "ScreenDrawEditCordonLine.h"
#include "TDrawScreenCordonInfo.h"
#include "BulkDBaseScreenCordon.h"
#include "NameInputDlg.h"

KScreenDrawEditCordonLine::KScreenDrawEditCordonLine( IMapView* a_mapRequest ) : IScreenDrawEdit(a_mapRequest)
{
    m_emCordonLineEditStatus = KEMCordonLineEditStatusNothing;
    a_mapRequest->SetMapMode(KEMMapModeScreenDrawEdit);

    m_spSymbolLine = TxSymbolLine::CreateLineSymbol();
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

KScreenDrawEditCordonLine::~KScreenDrawEditCordonLine( void )
{
	StopEditing();
}

void KScreenDrawEditCordonLine::StartEditing()
{
	KNameInputDlg dlg;
	dlg.SetCaption(_T("Cordon Line"));
	if (KmzSystem::GetLanguage() == KEMKorea) {
        dlg.SetCaption(_T("Cordon Line 그리기"));
    }

	if (dlg.DoModal() == IDOK)
	{
		m_bScreenDrawEditing = true;
		m_strName = dlg.GetName();
		// 명칭을 받고 -> 시점 입력 상태
		m_emCordonLineEditStatus  = KEMCordonLineEditStatusFirstPoint;
		FirstPointCommandMsg();
        m_pIMapView->SetMapMode(KEMMapModeScreenDrawEdit);
	}
	else
	{
		StopEditing();
	}
}

void KScreenDrawEditCordonLine::Draw()
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_spGeoDraw == nullptr)
        return;

    TxFeatureDispPtr spFeatureDisp; 
    if (m_spGeoDraw->GeometryType() == GeoPolylineType) {
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolLine);
    } else {
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawEditCordonLine::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    switch (m_emCordonLineEditStatus)
    {
    case KEMCordonLineEditStatusNameInput:
        // 명칭을 받는 단계는 리턴
        return;
        break;
    case KEMCordonLineEditStatusFirstPoint:
        m_emCordonLineEditStatus = KEMCordonLineEditStatusAdd;
        break;
    case KEMCordonLineEditStatusAdd:
        // 입력 중
        m_emCordonLineEditStatus = KEMCordonLineEditStatusAdd;
        AddPointCommandMsg();
        break;
    default:
        return;
        break;
    }

    m_vecPt.push_back(TxPoint(mapX, mapY));

    if (m_emCordonLineEditStatus >= KEMCordonLineEditStatusNameInputed) {
        AddPointCommandMsg();
    }

    m_emCordonLineEditStatus = KEMCordonLineEditStatusAdd;
}

void KScreenDrawEditCordonLine::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_emCordonLineEditStatus == KEMCordonLineEditStatusAdd) {
        SaveEditing();
    }
}

void KScreenDrawEditCordonLine::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (MK_MBUTTON & nFlags)
        return;

    if (m_vecPt.size() < 1)
        return;

    std::vector<TxPoint> vecPt = m_vecPt; {
        vecPt.push_back(TxPoint(mapX, mapY));
    }

    TxFeatureDispPtr spFeatureDisp; 
    size_t nxCnt = vecPt.size();
    if (nxCnt == 2) {
        m_spGeoDraw  = TxPolylinePtr(new TxPolyline(vecPt));
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolLine);
    } else {
        vecPt.push_back(m_vecPt[0]); //★ 시점추가
        m_spGeoDraw = TxPolygonPtr(new TxPolygon(vecPt));
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}


void KScreenDrawEditCordonLine::Sleep()
{
}


void KScreenDrawEditCordonLine::CommandFeedback( CString a_strResult )
{
    switch (m_emCordonLineEditStatus)
    {
    case KEMCordonLineEditStatusNameInput:
        m_strName = a_strResult;
        // 명칭을 받고 -> 시점 입력 상태
        m_emCordonLineEditStatus  = KEMCordonLineEditStatusFirstPoint;
        FirstPointCommandMsg();
        break;
    case KEMCordonLineEditStatusFirstPoint:
        // TODO : 시점 입력 단계(x,y)를 받을 수 있다.
        break;
    case KEMCordonLineEditStatusFirstPointInputed:
        // TODO : 시점 입력 후 다음 포인트 입력 단계
        break;
    case KEMCordonLineEditStatusAdd:
        if (a_strResult.CompareNoCase(_T("F")) == 0)
        {
            SaveEditing();
        }
        else if (a_strResult.CompareNoCase(_T("C")) == 0)
        {
            StopEditing();
        }
        else
        {
        }
        break;
    case KEMCordonLineEditStatusComplete:
        // 완료 단계, 이메세지는 절대 받을 수 없음.
        break;
    }
}

void KScreenDrawEditCordonLine::Canceled()
{
	SendLogMsg(_T("코든라인 작업 종료"));
    StopEditing();
}

void KScreenDrawEditCordonLine::FirstPointCommandMsg()
{
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("시점 선택(지도), 취소(C):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditCordonLine::AddPointCommandMsg()
{    
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));
    vecSelectableInfo.push_back(_T("F"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("절점 선택(지도), 취소(C), 완료(F):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditCordonLine::SaveEditing()
{
	if (m_vecPt.size() < 3) {
		Canceled();
		return;
	}

    std::vector<TxPoint> vecPt = m_vecPt; {
        vecPt.push_back(m_vecPt[0]); //★ 시점 추가
    }

    m_vecPt.clear();
    m_spGeoDraw = nullptr;   
        
    std::vector<TDrawScreenCordonXY> vecPoints;
    for (size_t i=0; i<vecPt.size(); i++) {
        TxPoint& oPt = vecPt[i];
        TDrawScreenCordonXY oDrawScreenCordonXY; {
            oDrawScreenCordonXY.Tx = oPt.X;
            oDrawScreenCordonXY.Ty = oPt.Y;
        }

        vecPoints.push_back(oDrawScreenCordonXY);
    }

    Integer nxID = 1;
    try
    {
        KBulkDBaseScreenCordon::SaveCordon(m_pIMapView->GetTarget(), m_strName, vecPoints, nxID);
        CString strOut;
        strOut.Format(_T("%s[ID:%I64d] 관련 정보를 저장하였습니다.\n도구 > 페쇄선 분석 > 페쇄선 관리에서 확인하실 수 있습니다."), m_strName, nxID);
        AfxMessageBox(strOut);

        m_pIMapView->UpdateScreenCordonManage(2, nxID);
    }
    catch(...)
    {
        AfxMessageBox(_T("저장중 오류가 발생하였습니다."));
    }

    StopEditing();
}

void KScreenDrawEditCordonLine::StopEditing()
{
	try
	{
		m_emCordonLineEditStatus = KEMCordonLineEditStatusNothing;
		m_bScreenDrawEditing     = false;

        m_vecPt.clear();
        m_spGeoDraw = nullptr;   

		m_pIMapView->SetMapMode(KEMMapModePan);
        m_pIMapView->SetMapCursor(IDC_PAN);

		KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
		if (pCommandWindow)
			pCommandWindow->SetDefaultCommand();
		
        m_pIMapView->MapForegroundRefresh();
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}
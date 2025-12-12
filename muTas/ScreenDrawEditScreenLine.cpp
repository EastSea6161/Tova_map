#include "StdAfx.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "ScreenDrawEditScreenLine.h"
#include "TDrawScreenCordonInfo.h"
#include "BulkDBaseScreenCordon.h"
#include "NameInputDlg.h"

KScreenDrawEditScreenLine::KScreenDrawEditScreenLine( IMapView* a_mapRequest ) : IScreenDrawEdit(a_mapRequest)
{
    m_emScreenLineEditStatus = KEMScreenLineEditStatusNothing;
    a_mapRequest->SetMapMode(KEMMapModeScreenDrawEdit);
    m_spSymbolLine = TxSymbolLine::CreateLineSymbol(1, Gdiplus::Color::Red );
}

KScreenDrawEditScreenLine::~KScreenDrawEditScreenLine( void )
{
	StopEditing();
}

void KScreenDrawEditScreenLine::StartEditing()
{
    m_emScreenLineEditStatus = KEMScreenLineEditStatusNameInput;
    
	KNameInputDlg dlg;
	dlg.SetCaption(_T("Screen Line"));
	if (KmzSystem::GetLanguage() == KEMKorea) {
        dlg.SetCaption(_T("Screen Line 그리기"));
    }

	if (dlg.DoModal() == IDOK)
	{
		m_bScreenDrawEditing = true;
		m_strName = dlg.GetName();
		// 명칭을 받고 -> 시점 입력 상태
		m_emScreenLineEditStatus  = KEMScreenLineEditStatusFirstPoint;
		FirstPointCommandMsg();
        m_pIMapView->SetMapMode(KEMMapModeScreenDrawEdit);
	}
	else
	{
		StopEditing();
	}
}

void KScreenDrawEditScreenLine::Draw()
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_vecPt.size() < 2)
        return;

    TxPolylinePtr spPLine = TxPolylinePtr(new TxPolyline(m_vecPt));
    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, spPLine));
    spFeatureDisp->Symbol(m_spSymbolLine);
    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawEditScreenLine::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    m_vecPt.push_back(TxPoint(mapX, mapY));
}

void KScreenDrawEditScreenLine::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_vecPt.size() >= 2)
        SaveEditing();
}

void KScreenDrawEditScreenLine::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_vecPt.size() == 0)
        return;

    std::vector<TxPoint> vecPt = m_vecPt;
    vecPt.push_back(TxPoint(mapX, mapY));

    TxPolylinePtr spPLine = TxPolylinePtr(new TxPolyline(vecPt));
    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, spPLine));
    spFeatureDisp->Symbol(m_spSymbolLine);
    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}


void KScreenDrawEditScreenLine::CommandFeedback( CString a_strResult )
{
    switch (m_emScreenLineEditStatus)
    {
    case KEMScreenLineEditStatusNameInput:
        m_strName = a_strResult;
        // 명칭을 받고 -> 시점 입력 상태
        m_emScreenLineEditStatus  = KEMScreenLineEditStatusFirstPoint;
        FirstPointCommandMsg();
        break;
    case KEMScreenLineEditStatusFirstPoint:
        // TODO : 시점 입력 단계(x,y)를 받을 수 있다.
        break;
    case KEMScreenLineEditStatusFirstPointInputed:
        // TODO : 시점 입력 후 다음 포인트 입력 단계
        break;
    case KEMScreenLineEditStatusAdd:
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
    case KEMScreenLineEditStatusComplete:
        // 완료 단계, 이메세지는 절대 받을 수 없음.
        break;
    }
}

void KScreenDrawEditScreenLine::Canceled()
{
	SendLogMsg(_T("스크린라인 작업 종료"));
    StopEditing();
}

void KScreenDrawEditScreenLine::FirstPointCommandMsg()
{
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("시점 선택(지도), 취소(C):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditScreenLine::AddPointCommandMsg()
{    
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));
    vecSelectableInfo.push_back(_T("F"));

    KCommandWnd*         pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("절점 선택(지도), 취소(C), 완료(F):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditScreenLine::SaveEditing()
{
    if (m_vecPt.size() < 2) {
        Canceled();
        return;
    }

    std::vector<TDrawScreenCordonXY> vecPoints;
    for (size_t i=0; i<m_vecPt.size(); i++)
    {        
        TxPoint& oPt = m_vecPt[i];
        TDrawScreenCordonXY oDrawScreenCordonXY; {
            oDrawScreenCordonXY.Tx = oPt.X;
            oDrawScreenCordonXY.Ty = oPt.Y;
        }

        vecPoints.push_back(oDrawScreenCordonXY);
    }

    m_vecPt.clear();

    Integer nxID = 1;
    try
    {
        KBulkDBaseScreenCordon::SaveScreen(m_pIMapView->GetTarget(), m_strName, vecPoints, nxID);
        CString strOut;
        strOut.Format(_T("%s[ID:%I64d] 관련 정보를 저장하였습니다.\n도구 > 페쇄선 분석 > 페쇄선 관리에서 확인하실 수 있습니다."), m_strName, nxID);
        AfxMessageBox(strOut);

        m_pIMapView->UpdateScreenCordonManage(1, nxID);
    }
    catch(...)
    {
        AfxMessageBox(_T("저장중 오류가 발생하였습니다."));
    }

    StopEditing();    
}

void KScreenDrawEditScreenLine::StopEditing()
{
	try
	{
        m_vecPt.clear();

		m_emScreenLineEditStatus  = KEMScreenLineEditStatusNothing;
		m_bScreenDrawEditing      = false;

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
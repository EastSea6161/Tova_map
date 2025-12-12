#include "StdAfx.h"
#include "MapView.h"
#include "NodeSelectionInfo.h"
#include "ScreenDrawEditNodeSelect.h"
#include "KDBaseAccessibilityModify.h"
#include "AccessNodeEditorDlg.h"
#include "TransitLineThemesDlg.h"
#include "StationODCalculatorDlg.h"
#include "LinkViaTransitSearchDlg.h"
#include "NodeViaTransitSearchDlg.h"
#include "KDlgTransitInfluenceArea.h"

void KMapView::MapNodeSelectAction(ImChampMapSelectionReceiver* a_pMapSelectionReceiver, std::vector<TNodeSelectionInfo>& ar_vecTNodeStation, bool a_bLable, bool a_bRefresh, double a_dScale)
{
	try
	{
		m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditNodeSelect(this, a_pMapSelectionReceiver, ar_vecTNodeStation, a_bLable, a_dScale));

		KScreenDrawEditNodeSelect* pScreenDrawEdit = (KScreenDrawEditNodeSelect*)m_spScreenDrawEdit.get();
		pScreenDrawEdit->StartEditing();
	}
	catch (...)
	{
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
	}	
}

void KMapView::ClearScreenDrawEdit(bool a_bRefresh)
{      
	ResetScreenDrawEdit();

    if (a_bRefresh) {
        MapForegroundRefresh();
    }
}

void KMapView::OnAccessibilityModify()
{
	ShowMapView();

	bool bUseAccessDB(false);
	KDBaseAccessibilityModify::CheckUsingAssessData(m_pTarget, bUseAccessDB);
	if (false == bUseAccessDB)
	{
		AfxMessageBox(_T("경로 만들기 > 대중교통 > 이용가능 역/정류장 생성을 먼저 실행해 주세요."));
		return;
	}

	try
	{	
        // 마지막 작업을 저장하지 않는다. Modal 다이얼로그 처럼.
        m_spAccessStopNodeEditorDlg = KAccessNodeEditorDlgPtr(new KAccessNodeEditorDlg(m_pTarget, this, this));
        m_spAccessStopNodeEditorDlg->Create(KAccessNodeEditorDlg::IDD, this);
        m_spAccessStopNodeEditorDlg->ShowWindow(SW_SHOW);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMapView::ShowTransitLineThemes( Integer a_nxTransitID )
{
	try
	{
		ShowMapView();

		/*if(nullptr != m_spTransitLineThemesDlg)
		{
			m_spTransitLineThemesDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spTransitLineThemesDlg = KTransitLineThemesDlgPtr(new KTransitLineThemesDlg(m_pTarget, this, this));
			m_spTransitLineThemesDlg->Create(KTransitLineThemesDlg::IDD, this);
			m_spTransitLineThemesDlg->ShowWindow(SW_SHOW);
		}*/

        m_spTransitLineThemesDlg = KTransitLineThemesDlgPtr(new KTransitLineThemesDlg(m_pTarget, this, this));
        m_spTransitLineThemesDlg->Create(KTransitLineThemesDlg::IDD, this);
        m_spTransitLineThemesDlg->ShowWindow(SW_SHOW);

//		if (a_nxTransitID > 0)
		{
			m_spTransitLineThemesDlg->SetViewTransit(a_nxTransitID);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMapView::ShowTransitODCalculator( Integer a_nxTransitID )
{
	try
	{
		if (!IsExistMultiModalAssignResultFile())
		{
			return;
		}

		ShowMapView();

        m_spStationODCalculatorDlg = KStationODCalculatorDlgPtr(new KStationODCalculatorDlg(m_pTarget, this, this));
        m_spStationODCalculatorDlg->Create(KStationODCalculatorDlg::IDD, this);
        m_spStationODCalculatorDlg->ShowWindow(SW_SHOW);

		m_spStationODCalculatorDlg->SetViewTransit(a_nxTransitID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMapView::TransitLinkSelect( Integer a_nxLinkID )
{
    try
    {
        if( nullptr == m_spLinkViaTransitSearchDlg)
        {
            m_spLinkViaTransitSearchDlg = KLinkViaTransitSearchDlgPtr(new KLinkViaTransitSearchDlg(m_pTarget, this));
            m_spLinkViaTransitSearchDlg->Create(KLinkViaTransitSearchDlg::IDD, this);
        }

        m_spLinkViaTransitSearchDlg->ShowWindow(SW_SHOW);

        if (a_nxLinkID < 0)
            return;

        m_spLinkViaTransitSearchDlg->AddLinkID(a_nxLinkID);
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

void KMapView::TransitNodeSelect( Integer a_nxNodeID )
{
    try
    {
        if( nullptr == m_spNodeViaTransitSearchDlg)
        {
            m_spNodeViaTransitSearchDlg = KNodeViaTransitSearchDlgPtr(new KNodeViaTransitSearchDlg(m_pTarget, this));
            m_spNodeViaTransitSearchDlg->Create(KNodeViaTransitSearchDlg::IDD, this);
        }

        m_spNodeViaTransitSearchDlg->ShowWindow(SW_SHOW);

        if (a_nxNodeID < 0)
            return;

        m_spNodeViaTransitSearchDlg->AddNodeID(a_nxNodeID);
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

void KMapView::TransitInfluenceArea()
{
    try
    {
        ShowMapView();

        if( nullptr == m_spTransitInfluenceAreaDlg)
        {
            m_spTransitInfluenceAreaDlg = KDlgTransitInfluenceAreaPtr(new KDlgTransitInfluenceArea(this));
            m_spTransitInfluenceAreaDlg->Create(KDlgTransitInfluenceArea::IDD, this);
        }

        m_spTransitInfluenceAreaDlg->ShowWindow(SW_SHOW);
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



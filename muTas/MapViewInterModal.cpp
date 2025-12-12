#include "StdAfx.h"
#include "MapView.h"
#include "NodeSelectionInfo.h"
#include "ScreenDrawEditNodeSelect.h"
#include "DBaseSubNetwork.h"
#include "DBaseCheckData.h"
#include "ImChampFileExist.h"

#include "TargetRegionSetting.h"
#include "SubNetworkDlg.h"

#include "AccessTermianlEditerDlg.h"
#include "GlobalTerminalSettingDlg.h"
#include "InterTransitPathEditDlg.h"
#include "TerminalScheduleDesireDlg.h"
#include "TerminalScheduleEditDlg.h"

void KMapView::OnInterModalTargetRegionSetting()
{
	ShowMapView();

	try
	{

		m_spTargetRegionSettingDlg = KTargetRegionSettingPtr(new KTargetRegionSetting(m_pTarget, this, this));
		m_spTargetRegionSettingDlg->Create(KTargetRegionSetting::IDD, this);
		m_spTargetRegionSettingDlg->ShowWindow(SW_SHOW);

	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KMapView::OnMenuToolsSubNetWork( void )
{
	ShowMapView();
	
	try
	{
		int nRecords = KDBaseSubNetwork::GetDBModeInfoTableRecords(m_pTarget);

		if (nRecords == 0)
		{
			CString strMsg(_T(""));

			strMsg.Append(_T("Path Analysis 결과 정보가 없습니다.\r\n"));
			strMsg.Append(_T("메뉴 > Models > Highway Assignment Models를 실행하세요.\r\n"));
			strMsg.Append(_T("(실행시,\r\n"));
			strMsg.Append(_T(" > User Equilibrium(OB) Model 선택 \r\n"));
			strMsg.Append(_T(" > Run Options에서 Include Path Analysis를 선택해야 함)\r\n"));
			ThrowException(strMsg);
		}

		if ( !ImChampFileExist::BushFileCheckMessage(m_pTarget))
			return;

		m_spSubNetworkDlg = KSubNetworkDlgPtr(new KSubNetworkDlg(m_pTarget, this, this));
		m_spSubNetworkDlg->Create(KSubNetworkDlg::IDD, this);
		m_spSubNetworkDlg->ShowWindow(SW_SHOW);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KMapView::OnInterModalAccessTerminalEditor()
{
	if (KDBaseCheckData::GetCheckTerminaNodeData(m_pTarget) == false)
	{
		AfxMessageBox(_T("이용가능 터미널 생성 결과가 존재하지 않습니다.\n경로 만들기 > 복합수단 > 이용가능 터미널 생성을 먼저 실행해 주세요."));
		return;
	}

	ShowMapView();

	try
	{
		m_spAccessTerminalEditerDlg = KAccessTermianlEditerPtr(new KAccessTermianlEditerDlg(m_pTarget, this, this));
		m_spAccessTerminalEditerDlg->Create(KAccessTermianlEditerDlg::IDD, this);
		m_spAccessTerminalEditerDlg->ShowWindow(SW_SHOW);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KMapView::OnInterModalGlobalTerminalSetting()
{
	ShowMapView();
	try
	{
		m_spGlobalTerminalSettingDlg = KGlobalTerminalSettingPtr(new KGlobalTerminalSettingDlg(m_pTarget, this, this));
		m_spGlobalTerminalSettingDlg->Create(KGlobalTerminalSettingDlg::IDD, this);
		m_spGlobalTerminalSettingDlg->ShowWindow(SW_SHOW);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMapView::OnInterModalPathEdit()
{
	try
	{
		if ( !KDBaseCheckData::GetCheckDataInterTerminalTimeInfo(m_pTarget))
		{
			AfxMessageBox(_T("경로 만들기 > 복합수단 > 복합수단 경로 생성을 먼저 실행해 주세요."));
			return;
		}

		ShowMapView();

		if(nullptr != m_spInterTransitPathEditDlg)
		{
			m_spInterTransitPathEditDlg->InitializeNode();
			m_spInterTransitPathEditDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spInterTransitPathEditDlg = KInterTransitPathEditDlgPtr(new KInterTransitPathEditDlg(m_pTarget, this, this));
			m_spInterTransitPathEditDlg->Create(KInterTransitPathEditDlg::IDD, this);
			m_spInterTransitPathEditDlg->InitializeNode();
			m_spInterTransitPathEditDlg->ShowWindow(SW_SHOW);
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


void KMapView::OnInterModalTerminalDesireLine()
{
	try
	{
		try
		{
			KDBaseConPtr spDBaseConnetion = m_pTarget->GetDBaseConnection();

			CString strSQL(_T(""));
			strSQL.AppendFormat(_T(" SELECT terminal_trip FROM %s where type = 6 ORDER BY run_seq DESC "), TABLE_ASSIGN_MODEL);
		
			KResultSetPtr pResult  = spDBaseConnetion->ExecuteQuery(strSQL);

			CString strFieldName(_T(""));

			if( pResult->Next() )
			{
				strFieldName = pResult->GetValueString(0);
			}

			if (strFieldName.Compare(_T("")) == 0)
				throw 1;
		}
		catch (...)
		{
			AfxMessageBox(_T("복합수단 통행배정 결과가 존재하지 않습니다.\n모형 > 복합수단 통행배정 모형을 먼저 실행해 주세요. "));
			return;	
		}

		ShowMapView();

		if(nullptr != m_spTerminalScheduleDesireDlg)
		{
			m_spTerminalScheduleDesireDlg->Initialize();
			m_spTerminalScheduleDesireDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spTerminalScheduleDesireDlg = KTerminalScheduleDesireDlgPtr(new KTerminalScheduleDesireDlg(m_pTarget, this, this));
			m_spTerminalScheduleDesireDlg->Create(KTerminalScheduleDesireDlg::IDD, this);
			m_spTerminalScheduleDesireDlg->Initialize();
			m_spTerminalScheduleDesireDlg->ShowWindow(SW_SHOW);
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


void KMapView::OnTerminalScheduleEdit()
{
	ShowTerminalScheduleEditView();
}


void KMapView::ShowTerminalScheduleEditView()
{
	try
	{
		ShowMapView();

		if(nullptr != m_spTerminalScheduleEditDlg)
		{
			m_spTerminalScheduleEditDlg->InitializeSet();
			m_spTerminalScheduleEditDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spTerminalScheduleEditDlg = KTerminalScheduleEditDlgPtr(new KTerminalScheduleEditDlg(m_pTarget, this, this));
			m_spTerminalScheduleEditDlg->Create(KTerminalScheduleEditDlg::IDD, this);
			m_spTerminalScheduleEditDlg->InitializeSet();
			m_spTerminalScheduleEditDlg->ShowWindow(SW_SHOW);
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
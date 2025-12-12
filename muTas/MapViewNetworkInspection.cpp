#include "stdafx.h"
#include "KmzApp.h"
#include "MapView.h"
#include "Project.h"
#include "Target.h"
//^^ #include "TableOwner.h"
#include "ImChampFrameWindow.h"
#include "NetworkInspectionDlg.h"

void KMapView::OnNetworkInspectionRun()
{
	try
	{		
		KProject* pProject  = ImChampFrameWindow::GetProject();
    	KTarget*  pTarget   = ImChampFrameWindow::GetActiveFrameTarget();

		if (nullptr == pTarget)
			throw 1;

		if (nullptr != m_spNetworkInspectionDlg)
		{
			m_spNetworkInspectionDlg->Initialize();
			m_spNetworkInspectionDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spNetworkInspectionDlg = KNetworkInspectionDlgPtr(new KNetworkInspectionDlg(pProject, pTarget, this));
			m_spNetworkInspectionDlg->Create(KNetworkInspectionDlg::IDD, this);
			//m_spNetworkInspectionDlg->Initialize();
			m_spNetworkInspectionDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KMapView::OnUpdateNetworkInspectionRun(CCmdUI *pCmdUI)
{
	try
	{
		CChildFrame* pChildFrame = ImChampFrameWindow::GetActiveFrame();

		if (nullptr == pChildFrame) {
			pCmdUI->Enable(FALSE);
			return;
		}

		pCmdUI->Enable(TRUE);
	}
	catch (...)
	{
		pCmdUI->Enable(FALSE);
		TxLogDebugException();
	}
}


void KMapView::OnNetworkInspectionHighwayError()
{
	NetworkInspectionRelationFileOpen(T_DAT_OUT_HIGHWAY_ERROR);
}


void KMapView::OnNetworkInspectionHighwayWarning()
{
	NetworkInspectionRelationFileOpen(T_DAT_OUT_HIGHWAY_WARNING);
}


void KMapView::OnNetworkInspectionTransitError()
{
	NetworkInspectionRelationFileOpen(T_DAT_OUT_TRANSIT_ERROR);
}


void KMapView::OnNetworkInspectionTransitWarning()
{
	NetworkInspectionRelationFileOpen(T_DAT_OUT_TRANSIT_WARNING);
}


void KMapView::OnNetworkInspectionViewLog()
{
	NetworkInspectionRelationFileOpen(T_TXT_OUT_DATAINSPECTION_LOG);
}


void KMapView::NetworkInspectionRelationFileOpen(CString a_strFile)
{
	try
	{
		KProject* pProject = ImChampFrameWindow::GetProject();
		KTarget*  pTarget  = ImChampFrameWindow::GetActiveFrameTarget();

		if (nullptr == pTarget)
			throw 1;

		CString strFileName(_T(""));
		strFileName.Format(_T("%s%s%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFile);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFileName) == FALSE)
		{
			CString strMsg(_T(""));
			strMsg.AppendFormat(_T("%s 파일이 존재 하지 않습니다.\r\n"), a_strFile);
			strMsg.AppendFormat(_T("경로:%s%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH);
			ThrowException(strMsg);
		}
		ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KMapView::OnUpdateNetworkInspectionHighwayError(CCmdUI *pCmdUI)
{
	try
	{
		UpdateNetworkInspection(T_DAT_OUT_HIGHWAY_ERROR);
		pCmdUI->Enable(TRUE);
	}
	catch(...)
	{
		pCmdUI->Enable(FALSE);
	}
}


void KMapView::OnUpdateNetworkInspectionHighwayWarning(CCmdUI *pCmdUI)
{
	try
	{
		UpdateNetworkInspection(T_DAT_OUT_HIGHWAY_WARNING);
		pCmdUI->Enable(TRUE);
	}
	catch(...)
	{
		pCmdUI->Enable(FALSE);
	}
}


void KMapView::OnUpdateNetworkInspectionTransitError(CCmdUI *pCmdUI)
{
	try
	{
		UpdateNetworkInspection(T_DAT_OUT_TRANSIT_ERROR);
		pCmdUI->Enable(TRUE);
	}
	catch(...)
	{
		pCmdUI->Enable(FALSE);
	}
}


void KMapView::OnUpdateNetworkInspectionTransitWarning(CCmdUI *pCmdUI)
{
	try
	{
		UpdateNetworkInspection(T_DAT_OUT_TRANSIT_WARNING);
		pCmdUI->Enable(TRUE);
	}
	catch(...)
	{
		pCmdUI->Enable(FALSE);
	}
}


void KMapView::OnUpdateNetworkInspectionViewLog(CCmdUI *pCmdUI)
{
	try
	{
		UpdateNetworkInspection(T_TXT_OUT_DATAINSPECTION_LOG);
		pCmdUI->Enable(TRUE);
	}
	catch(...)
	{
		pCmdUI->Enable(FALSE);
	}
}


void KMapView::UpdateNetworkInspection(CString a_strFile)
{
	try
	{
		CChildFrame*   pChildFrame = ImChampFrameWindow::GetActiveFrame();
		if (nullptr == pChildFrame)
			throw 1;

		KTarget*       pTarget     = ImChampFrameWindow::GetActiveFrameTarget();
		if (nullptr == pTarget)
			throw 1;

		CString strFileName(_T(""));
		strFileName.Format(_T("%s%s%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFile);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFileName) == FALSE)
		{
			throw 1;
		}

		oFileFinder.FindNextFile();
		if (oFileFinder.GetLength() <= 0)
		{
			throw 1;
		}
	}
	catch (...)
	{
		throw 1;
	}
}
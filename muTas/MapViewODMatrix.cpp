#include "stdafx.h"
#include "KmzApp.h"
#include "MapView.h"
#include "Project.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "ODMatrixDlg.h"

void KMapView::ShowODMatrix(CString a_strTableName)
{
    try
    {
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (nullptr == pTarget)
            throw 1;

        KIOTable* pIOTable  = pTarget->Tables()->FindTable(a_strTableName);
        if (nullptr == pIOTable)
            throw 1;

        m_spODMatrixDlg = KODMatrixDlgPtr(new KODMatrixDlg(pTarget, pIOTable, this));
        m_spODMatrixDlg->Create(KODMatrixDlg::IDD, this);
        m_spODMatrixDlg->Initialize();
        m_spODMatrixDlg->ShowWindow(SW_SHOW);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KMapView::OnMatrixPurposeOD()
{
	ShowODMatrix(TABLE_PURPOSE_OD);
}

void KMapView::OnMatrixModeOD()
{
	ShowODMatrix(TABLE_MODE_OD);
}

void KMapView::OnMatrixChainOD()
{
	ShowODMatrix(TABLE_CHAIN_OD);
}

void KMapView::OnMatrixVehicleModeOD()
{
	ShowODMatrix(TABLE_VEHICLE_MODE_OD);
}


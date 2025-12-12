
#include "stdafx.h"
#include "TableOwner.h"
#include "IOParameterTableView.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "KmzApp.h"
#include "IOTransitTableView.h"
#include "IOItem.h"
#include "IORecordset.h"
#include "IORow.h"
#include "IOTable.h"
#include "IOTableController.h"
#include "IOTableRecord.h"
#include "CalculatePathLengthDlg.h"

IMPLEMENT_DYNCREATE(KIOParameterTableView, KIOTableView)

KIOParameterTableView::KIOParameterTableView(void)
{
}


KIOParameterTableView::~KIOParameterTableView(void)
{
}


BEGIN_MESSAGE_MAP(KIOParameterTableView, KIOTableView)
	
END_MESSAGE_MAP()

void KIOParameterTableView::OnTableviewAddField()
{
	if(NULL != m_pController)
	{
		m_pController->AddField();
	}
}

void KIOParameterTableView::OnTableViewCalculatedPathLength()
{
	if (NULL != m_pController)
	{
		m_pController->ViewCalculatePathLength();
	}
}

void KIOParameterTableView::OnTableViewCalculateTimeCost()
{
	if (NULL != m_pController)
	{
		m_pController->ViewCalculateTimeCost();
	}
}



#include "StdAfx.h"
#include "CustomReportRecordCtrl.h"


KCustomReportRecordCtrl::KCustomReportRecordCtrl(void)
{
}


KCustomReportRecordCtrl::~KCustomReportRecordCtrl(void)
{
}


DROPEFFECT KCustomReportRecordCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point, int nState)
{
	DROPEFFECT dropeffect = CXTPReportControl::OnDragOver(pDataObject,dwKeyState,point,nState);

	point.y += 9;// CXTPReportControl::OnDraw 에서 DrawDropMarker offset이 -4 ~ +4 인 관계로 실제 마우스 over 위치를 이에 맞게 조절 함: jyk 
	CXTPReportRow *pRow = HitTest(point);
	SetFocusedRow(pRow);

	return dropeffect;
}
#include "stdafx.h"
#include "MapView.h"

void KMapView::CommandFeedback(CString a_strResult)
{
    AfxMessageBox(a_strResult);
}

void KMapView::Canceled()
{
    AfxMessageBox(_T("취소되었습니다."));
}
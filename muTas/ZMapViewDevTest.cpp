#include "StdAfx.h"
#include "MapView.h"
#include "NombaraTester.h"

void KMapView::OnHelpNombara()
{
    KNombaraTester oDlg(this);
    oDlg.DoModal();
}

void KMapView::OnNombaraTesterA()
{    
  
}

void KMapView::OnNombaraTesterB()
{
}

unsigned __stdcall KMapView::MapViewThreadTester(void* p)
{
    return 0;
}


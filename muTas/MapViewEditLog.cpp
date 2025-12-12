#include "stdafx.h"
#include "MapView.h"
#include "NetworkEditLog.h"

void KMapView::NetworkEditLog( int a_nLayer, Integer a_nxID, int a_nEditType )
{
	try
	{
		KNetworkEditLog::WriteNetworkEditLog(m_pTarget, a_nLayer, a_nxID, a_nEditType);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

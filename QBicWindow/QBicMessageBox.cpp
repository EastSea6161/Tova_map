#include "stdafx.h"
#include "QBicMessageBox.h"

void QBicMessageBox::MessageBox( CString a_strMsg, CString a_strCaption /*= _T("Information")*/ )
{
    ::MessageBox(NULL, a_strMsg, a_strCaption, MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_TOPMOST);
}

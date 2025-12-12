
#include "stdafx.h"
#include "model_common.h"



CString GetPurposeGroupName(KEMPurposeGroup& a_emPurposeGroup)
{
	CString strResult = _T("");

	switch(a_emPurposeGroup)
	{
	case KEMPurposeGroupODBase:
		strResult = PurposeGroupODBase_NAME;
		break;

	case KEMPurposeGroupHomeBase:
		// jyk미정 - 추후 정의
		break;

	case KEMPurposeGroupNoneHomeBase:
		// jyk미정 - 추후 정의
		break;

	case KEMPurposeGroupFreightOD:
		strResult = PurposeGroupFreightOD_NAME;
		break;

	default:
		break;
	}

	return strResult;
}


CString GetModeGroupName(KEMModeGroup& a_emModeGroup)
{
	CString strResult = _T("");

	switch(a_emModeGroup)
	{
	case KEMModeGroupODBase:
		strResult = ModeGroupODBase_NAME;
		break;

	case KEMModeGroupHomeBase:
		// jyk미정 - 추후 정의
		break;

	case KEMModeGroupNoneHomeBase:
		// jyk미정 - 추후 정의
		break;

	case KEMModeGroupFreightOD:
		strResult = ModeGroupFreightOD_NAME;
		break;

	default:
		break;
	}

	return strResult;
}

///* Define table names */
//const TCHAR* TABLE_NODE = _T("node");
//const TCHAR* TABLE_LINK = _T("link");
//const TCHAR* TABLE_ZONE = _T("zone");
//const TCHAR* TABLE_TURN = _T("turn");
//const TCHAR* TABLE_PURPOSE_OD = _T("purpose_od");
//const TCHAR* TABLE_PARAMETER_OD = _T("parameter_od");
//const TCHAR* TABLE_MODE_OD  = _T("mode_od");
//
//
///* Define column names */
//const TCHAR* COLUMN_LINKTYPE = _T("link_type");
//const TCHAR* COLUMN_OZONE = _T("ozone_id");
//const TCHAR* COLUMN_DZONE = _T("dzone_id");

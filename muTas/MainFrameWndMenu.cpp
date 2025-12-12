#include "stdafx.h"
#include "MainFrameWnd.h"
#include "ImChampDir.h"

void CMainFrameWnd::On_PATH_MULTIMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

    strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\PathListAppend_MM.exe"));
	
	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_PATH_MULTIMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\PathListAppend_MM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_PATH_INTERMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\PathListAppend_IM.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_PATH_INTERMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\PathListAppend_IM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_BUS_MULTIMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\BRTransferVolAnalysis_MM.exe"));

	TxLogDebugInfo(strDir);

	ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_BUS_MULTIMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\BRTransferVolAnalysis_MM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_BUS_INTERMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\BRTransferVolAnalysis_IM.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_BUS_INTERMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\BRTransferVolAnalysis_IM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_STATION_MULTIMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\StaToStaVolAnalysis_MM.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_STATION_MULTIMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\StaToStaVolAnalysis_MM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_STATION_INTERMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

    strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\StaToStaVolAnalysis_IM.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_STATION_INTERMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\StaToStaVolAnalysis_IM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_URBAN_MULTIMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\URailTransitPA_MM.exe"));
	
	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_URBAN_MULTIMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\URailTransitPA_MM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::On_URBAN_INTERMODAL()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\URailTransitPA_IM.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);   
}

void CMainFrameWnd::OnUpdate_URBAN_INTERMODAL(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\URailTransitPA_IM.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::OnEXE_ICBOUNDVOL_ANALYSIS()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

    strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\ICBoundVolumeAnalysis.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);
}

void CMainFrameWnd::OnUpdateEXE_ICBOUNDVOL_ANALYSIS(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\ICBoundVolumeAnalysis.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrameWnd::OnEXE_ICBOUNDVOLBAT_ANALYSIS()
{
    CString strPath(_T("")); 
	CString strDir = _T("");

	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
	strDir = strPath + _T("\\");
    strPath.Append(_T("\\ICBoundVolumeAnalysis_bat.exe"));

	TxLogDebugInfo(strDir);

    ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);
}

void CMainFrameWnd::OnUpdateEXE_ICBOUNDVOLBAT_ANALYSIS(CCmdUI* pCmdUI)
{
    CString strPath(_T("")); {
        strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
        strPath.Append(_T("\\ICBoundVolumeAnalysis_bat.exe"));
    }

    CFileFind oFinder;
    if (oFinder.FindFile(strPath) == FALSE) {
        pCmdUI->Enable(FALSE);
    }
    else {
        pCmdUI->Enable(TRUE);
    }
}


void CMainFrameWnd::OnUpdateEXE_TRIPOD_TO_VEHICLEOD(CCmdUI* pCmdUI)
{
	// 2020.02.09 - Han He : 영문 버전일 경우, 숨기기
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
        pCmdUI->Enable(TRUE);
		//CString strPath(_T("")); {
		//	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
		//	strPath.Append(_T("\\TripODtoVehOD.exe"));
		//}

		//CFileFind oFinder;
		//if (oFinder.FindFile(strPath) == FALSE) {
		//	pCmdUI->Enable(FALSE);
		//}
		//else {
		//	pCmdUI->Enable(TRUE);
		//}
	}
	else // if (KmzSystem::GetLanguage() == KEMEnglish)
	{
		//if (pCmdUI->m_pMenu != NULL)
		//	pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID, MF_BYCOMMAND);
		CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
		if (pControl) pControl->SetVisible(False);
	}

}


void CMainFrameWnd::OnUpdateEXE_FREIGHTOD_DIVISION(CCmdUI* pCmdUI)
{
	// 2020.02.09 - Han He : 영문 버전일 경우, 숨기기
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
        pCmdUI->Enable(TRUE);
		//CString strPath(_T("")); {
		//	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
		//	strPath.Append(_T("\\FODDiv.exe"));
		//}

		//CFileFind oFinder;
		//if (oFinder.FindFile(strPath) == FALSE) {
		//	pCmdUI->Enable(FALSE);
		//}
		//else {
		//	pCmdUI->Enable(TRUE);
		//}
	}
	else // if (KmzSystem::GetLanguage() == KEMEnglish)
	{
		//if (pCmdUI->m_pMenu != NULL)
		//	pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID, MF_BYCOMMAND);
		CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
		if (pControl) pControl->SetVisible(False);
	}
}

void CMainFrameWnd::OnUpdateEXE_ACCESSTRIPOD_TO_VEHICLEOD(CCmdUI* pCmdUI)
{
    if (KmzSystem::GetLanguage() == KEMKorea)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        CXTPControl* pControl = CXTPControl::FromUI(pCmdUI);
        if (pControl) pControl->SetVisible(False);
    }
}


//void CMainFrameWnd::OnEXE_TRIPOD_TO_VEHICLEOD()
//{
//	CString strPath(_T(""));
//	CString strDir = _T("");
//
//	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
//	strDir = strPath + _T("\\");
//	strPath.Append(_T("\\TripODtoVehOD.exe"));
//
//	TxLogDebugInfo(strDir);
//
//	ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);
//}



//void CMainFrameWnd::OnEXE_FREIGHTOD_DIVISION()
//{
//	CString strPath(_T(""));
//	CString strDir = _T("");
//
//	strPath.AppendFormat(_T("%s\\Utility"), ImChampDir::GetAppDirectory());
//	strDir = strPath + _T("\\");
//	strPath.Append(_T("\\FODDiv.exe"));
//
//	TxLogDebugInfo(strDir);
//
//	ShellExecute(NULL, _T("open"), strPath, NULL, strDir, SW_SHOW);
//}
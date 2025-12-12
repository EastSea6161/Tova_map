// kmz.h : main header file for the kmz application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "NetworkCompareDlg.h"

/* Forward declarations */
class KProject;
class KTarget;
class KScenario;

/**
 * @brief 분석 시스템 app
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.07.29
 */
class CKmzApp : public CWinApp
{
public:
	CKmzApp();
	~CKmzApp();
public :
	inline HMENU getMenuHandle( void )
	{
		return m_hMDIMenu;
	}

	inline HACCEL getAcceleratorHandle( void )
	{
		return m_hMDIAccel;
	}

	inline KProject* GetProject( void )
	{
		return m_pProject;
	}

public :
	bool CreateUserPassengerProject( void );
	bool CreateUserFreightProject( void );

	bool OpenProject( const CString& strFile );
	bool SaveProject( void );
	void CloseProject( void );

	void DeleteOldProjectFile(CString a_strOldProjectName);

protected :
	void PerformEditProject( void );
	void performAddScenario( void );
	void PerformImportBLANK( void );
	void PerformScenarioCopyTo( void );
	void performScenarioDelete( void );
	void performScenarioAddYear( void );
	void performScenarioRename( void );
	void performTargetCopyTo( void );
	void performTargetDelete( void );
	void performtargetRename( void );
	void PerformTargetImportKTDB( void );
	void PerformTargetImportBLANK( void );

	void DeletePath(CString strPath);
public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
protected:
	HMENU   m_hMDIMenu;
	HACCEL  m_hMDIAccel;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

private:  
	KProject*       m_pProject;
	
	// 2019.05.24 Free 버전 내용 추가
	bool m_bFreeVersion;

public: 
	static const int MAX_NODE_OF_FREEVERSION;
	bool IsFreeVersion();


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
    afx_msg void OnOnlineHelp();
	afx_msg void OnUserProjectNew();
	afx_msg void OnProjectEdit();
	afx_msg void OnUpdateProjectEdit( CCmdUI* pCmdUI );
	afx_msg void OnProjectOpen();
	afx_msg void OnProjectClose();
	afx_msg void OnUpdateProjectClose( CCmdUI* pCmdUI );
	afx_msg void OnProjectSave();
	afx_msg void OnUpdateProjectSave( CCmdUI* pCmdUI );
	afx_msg void OnProjectExport();
	afx_msg void OnUpdateProjectExport( CCmdUI* pCmdUI );
	afx_msg void OnProjectImport();
	afx_msg void OnScenarioAdd();
	afx_msg void OnUpdateScenarioAdd( CCmdUI* pCmdUI );
	afx_msg void OnScenarioDelete();
	afx_msg void OnUpdateScenarioDelete( CCmdUI* pCmdUI );
	afx_msg void OnScenarioCopyto();
	afx_msg void OnUpdateScenarioCopyto( CCmdUI* pCmdUI );
	afx_msg void OnScenarioRename();
	afx_msg void OnUpdateScenarioRename( CCmdUI* pCmdUI );
	afx_msg void OnTargetyearAdd();
	afx_msg void OnUpdateTargetyearAdd( CCmdUI* pCmdUI );
	afx_msg void OnTargetyearDelete();
	afx_msg void OnUpdateTargetyearDelete( CCmdUI* pCmdUI );
	afx_msg void OnTargetyearImportuserdata();
	afx_msg void OnUpdateTargetyearImportuserdata( CCmdUI* pCmdUI );
	afx_msg void OnTargetyearRename();
	afx_msg void OnUpdateTargetyearRename( CCmdUI* pCmdUI );
	afx_msg void OnTargetyearInformation();
	afx_msg void OnUpdateTargetyearInformation( CCmdUI* pCmdUI );
	afx_msg void OnPopupProjectAddscenario();
	afx_msg void OnPopupProjectEditproject();
	afx_msg void OnPopupScenarioImportBlank();
	afx_msg void OnUpdatePopupScenarioImportBlank( CCmdUI* pCmdUI );
	afx_msg void OnPopupProjectRefresh();
	afx_msg void OnPopupScenarioCopyto();
	afx_msg void OnPopupScenarioDelete();
	afx_msg void OnPopupScenarioAddyear();
	afx_msg void OnPopupScenarioRename();
	afx_msg void OnUpdatePopupScenarioDelete( CCmdUI* pCmdUI );
	afx_msg void OnUpdatePopupScenarioRename( CCmdUI* pCmdUI );
	afx_msg void OnUpdatePopupScenarioAddYear( CCmdUI* pCmdUI );
	afx_msg void OnUpdatePopupScenarioCopyto( CCmdUI* pCmdUI );
	afx_msg void OnPopupTargetCopyto();
	afx_msg void OnPopupTargetDelete();
	afx_msg void OnPopupTargetRename();
	afx_msg void OnPopupNaviItemAdd(void);
	afx_msg void OnPopupNaviItemRename(void);
	afx_msg void OnPopupNaviItemDelete(void);

	afx_msg void OnPopupTargetImport();
	afx_msg void OnUpdatePopupTargetImport( CCmdUI* pCmdUI );

    afx_msg void OnPopupTargetExport();
    afx_msg void OnUpdatePopupTargetExport( CCmdUI* pCmdUI );

	afx_msg void OnUpdatePopupTargetCopyto( CCmdUI* pCmdUI );
	afx_msg void OnPopupTargetImportBlank();

    afx_msg void OnNetworkCompare();
    afx_msg void OnUpdateNetworkCompare(CCmdUI *pCmdUI);
	afx_msg void OnProjectCopy(void);
	afx_msg void OnUpdateProjectCopy(CCmdUI *pCmdUI);
private:
    KNetworkCompareDlgPtr m_spNetworkCompareDlg;
private:
    afx_msg void OnMenuPreference();	
	void UpdateAppTitle(void);
public:
    afx_msg void OnProjectVaccum();
public:
	HANDLE m_hLauncherProc;
	void KillKlauncher(void);
};

extern CKmzApp theApp;
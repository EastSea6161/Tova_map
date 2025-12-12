#pragma once

//^#include "ResizeDialogEx.h"
#include <fstream>
#include <iostream>
//#include "StdioFileEx.h"
#include "DBaseAssignment.h"
#include "DBaseDefaultParaAccessSetting.h"
#include "DBaseDefaultParaPathGenerateConstraint.h"
#include "DBaseDefaultParaMultimodalPathGeneration.h"

class KProject;
class KTarget;
class KODKey;
class KIOColumn;
class KGenerationBusPathInfo;

struct TBaseSetData
{
	int     nMaxRouteCnt;
	double  dDwellTimeatBus;
	double	dDWellTimeatRail;
	double	dBusBATime;
	double  dAlpha;
	double  dBeta;
	double  dPathTime;
	int		nInVehicleTime;
	int     nWatingTime;
	int     nTransferTime;
	int     nAETime;
	double  dPathLength;

};

struct TUtilityFunctionCoeff
{
	double  dTimeInVeh;
	double  dTimeWait;
	double  dTimeTransfer;
	double  dTimeAccessWalk;
	double  dTimeEgressWalk;
	double  dTransferCnt;
	double  dCost;
	double  dStopCnt;
};

struct TTransitPathCost
{
	int     nTransitType;
	int     nTransitDetailType;
	CString strDetailName;
	double  dBaseCost;
	double  dBaseLength;
	double  dAddLength;
	double  dAddCost;
	int     nCostMethod;
};




// KGenerationBusPathStep2Dlg 대화 상자입니다.

class KGenerationBusPathStep2Dlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KGenerationBusPathStep2Dlg)

public:
	KGenerationBusPathStep2Dlg(	KProject* a_pProject, KTarget* a_pTarget, int a_nTimeMethod, std::list<TGenerationPathInfo> a_lstPathInfo,
								 KIOColumn* a_pIOColumnUseLink, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGenerationBusPathStep2Dlg();
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_6916_GenerationBusPathStep2Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline int GetStatus( void )
	{
		return m_nStatus;
	}

protected:
	void         LoadBusPathInput(TBaseSetData &a_oTBaseSetData);
	void         LoadAccessClassInfo(std::vector<TAccessClassInfo> &a_vecLoadAccessClassInfo);
	void         DefaultBusPathCost(std::vector<TTransitPathCost> &a_vecTransitPathCost);
	void         InitReportB(void);
	void         InitReportBRecord(std::vector<TTransitPathCost> &a_vecTransitPathCost);
	void         ResizeComponent();
	void         SetDecimalRange(int a_nDecimalPlace, CString &a_strOutText);
	bool         InvalidateInputInfo();
	void         Run();
	void         CreateArgument();
	void	     ClearFiles();
	void         ArgumentWriteProjectName       (CStdioFileEx& of);
	void		 ArgumentWriteModeOD			(CStdioFileEx& of);
	void         ArgumentWriteTimeMethod        (CStdioFileEx& of);
	void		 ArgumentWriteMaxTransitPath	(CStdioFileEx& of);
	void         ArgumentWriteTimeOfBoardAlight (CStdioFileEx& of);
	void         ArgumentWriteWaitVDF           (CStdioFileEx& of);
	void         ArgumentWriteBusPathCost       (CStdioFileEx& of);
	void         ArgumentWriteClass             (CStdioFileEx& of);
	void		 ArgumentWriteCondition			(CStdioFileEx& of);
	void         ArgumentWriteFolder            (CStdioFileEx& of);
	void         InsertBusPathInput();
	void         InsertBusPathCoefficient();
	void         InsertBusPathCost();
	void		 InsertBusMatrixInfo();
	CString      ConvertSavePath(CString a_strFileName);
	void		 GetUseModeData(void);
	void		 DeleteAllFolderFile(CString a_strLocation);
	void		 InitSystemConstraintConditions();
	void		 DefaultConstraintConditions();
	void		 InitSystemPathInfo();
	void		 DefultPathInfo();

public:
	static unsigned __stdcall GenerationBusPathThreadCaller(void* p);
	static void ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg);
	static void AddStatusMessage(CString a_strMsg);

protected:
	HINSTANCE m_hGenerationBusPath;
	typedef int (*FGENERATEBUSPATH)(void *pParam, void* pCallback);
	FGENERATEBUSPATH SetGenerationBusPathProcPtr;

protected:

	bool		 m_bUseBus;
	bool		 m_bUseSubway;
	bool		 m_bUseBusSubway;

	int			 m_nResultCode;
	KProject*    m_pProject;
	KTarget*     m_pTarget;
	CString      m_strOutputFolerPath;
	bool         m_bRunResult;
	int			 m_nStatus;		// 0 = OK, 1 = Cancel, 2 = back
	std::vector<TAccessClassInfo> m_vecLoadAccessClassInfo;
	std::map<int, KPathGenerateConstraint> mapDefaultPathGenerateConstraint;
	std::map<int, KMultimodalPathGenerationInfo> mapDefaultMultimodalPathInfo;

	CString		 m_argFilePath;

public:
	int          m_nTimeMethod;
	KIOColumn*   m_pIOColumnBusOD;
	KIOColumn*   m_pIOColumnBusSubwayOD;
	KIOColumn*   m_pIOColumnUseLink;
	std::list<TGenerationPathInfo> m_lstGenerationpathInfo;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedDefault();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};

typedef std::shared_ptr<KGenerationBusPathStep2Dlg> KGenerationBusPathStep2DlgPtr;
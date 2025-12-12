// DefaultHighwayAssignment.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultHighwayAssignment.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "ABPRFnManagerDlg.h"
#include "DefineNamesDefaultPara.h"

#include "Project.h"

enum ModePceOccupancy
{
	_0_COLUMN_Matrix,
	_1_COLUMN_Occupancy,
	_2_COLUMN_PCE
};

enum VDFFunction
{
	_0_COLUMN_Function,
	_1_COLUMN_Alpha,
	_2_COLUMN_Beta,
	_3_COLUMN_A,
	_4_COLUMN_B
};


// KDefaultHighwayAssignment 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultHighwayAssignment, KResizeDialogEx)

KDefaultHighwayAssignment::KDefaultHighwayAssignment(KTarget* a_pTarget, KProject* a_pProject, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultHighwayAssignment::IDD, pParent)
	, m_pTarget(a_pTarget)
	,m_nComboFnCursor(0)
    ,m_nFnGroupID(0)
	,m_strUseFunctionGroup(_T(""))
	,m_nComboLinkAllAlphaCursor(0)
	,m_nComboLinkAllBetaCursor(0)
	,m_nComboLinkAllACursor(0)
	,m_nComboLinkAllBCursor(0)
	,m_nRadiobtnFunction(0)
	, m_pProject(a_pProject)
{

}

KDefaultHighwayAssignment::~KDefaultHighwayAssignment()
{
}

void KDefaultHighwayAssignment::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT2,         m_wndModeReport);
	DDX_Control(pDX, IDC_REPORT,          m_wndVDFReport);
 	DDX_Control(pDX, IDC_EDIT_TRAVELTIME, m_edtTravelTime);
 	DDX_Control(pDX, IDC_EDIT_TIME,       m_edtTime);
 	DDX_Control(pDX, IDC_EDIT_ALPHA,      m_edtAlpha);
 	DDX_Control(pDX, IDC_EDIT_BETA,       m_edtBeta);

	DDX_Control(pDX, IDC_COMBO,       m_cboClass);
	DDX_Control(pDX, IDC_COMBO_ALPHA, m_cboAlpha);
	DDX_Control(pDX, IDC_COMBO_BETA,  m_cboBeta);
	DDX_Control(pDX, IDC_COMBO_A,     m_cboA);
	DDX_Control(pDX, IDC_COMBO_B,     m_cboB);
}

BEGIN_MESSAGE_MAP(KDefaultHighwayAssignment, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO,	       &KDefaultHighwayAssignment::OnCbnSelchangeComboFunction)
 	ON_BN_CLICKED   (IDOK,             &KDefaultHighwayAssignment::OnBnClickedOk)
 	ON_BN_CLICKED   (IDCANCEL,         &KDefaultHighwayAssignment::OnBnClickedCancel)
 	ON_BN_CLICKED   (IDDEFAULT,        &KDefaultHighwayAssignment::OnBnClickedDefault)
	ON_BN_CLICKED(ID_BUTTON_SETBPRFN, &KDefaultHighwayAssignment::OnBnClickedButtonSetbprfn)

END_MESSAGE_MAP()

// KDefaultHighwayAssignment 메시지 처리기입니다.

BOOL KDefaultHighwayAssignment::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	KReportCtrlSetting::Default(m_wndVDFReport, TRUE);

	KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);
	KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(m_mapDefaultODTime);
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFPara(m_mapDefaultVDFPara);
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntion(m_mapDefaultVDFFuntion);
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntionGroup(m_mapDefaultVDFFuntionGroup);
	if (KmzSystem::IsFreightSystem())
	{
		KDBaseDefaultParaHighwayAssign::GetFModeInfo(m_pTarget, m_mapGetModeinfo);
	}
	else
	{
		KDBaseDefaultParaHighwayAssign::GetModeInfo(m_pTarget, m_mapGetModeinfo);
	}

	InitVDFReportHeader();
	InitModeReportHeader();

	AutoType iterMode = m_mapDefaultOccupancyPCEMode.begin();
	AutoType endMode  = m_mapDefaultOccupancyPCEMode.end();

	if(iterMode != endMode)
	{
		AutoType iterDefaultMode = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterDefaultMode != endMode)
		{
			GetDefaultModeInfo(KDefaultParameterMasterCode::USER_MASETER_CODE, m_mapGetDefaultModeID);
			InitModeReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			GetDefaultModeInfo(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, m_mapGetDefaultModeID);
			InitModeReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else 
	{
		SetModeReportRecord();
	}

	AutoType iterVDFPara = m_mapDefaultVDFPara.begin();
	AutoType endVDFPara  = m_mapDefaultVDFPara.end();
	if(iterVDFPara != endVDFPara)
	{
		AutoType iterDefaultVDFPara = m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterDefaultVDFPara != endVDFPara)
		{
			InitComboBoxEachFn(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitComboBoxEachLink(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitVDFParaSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitVDFReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitComboBoxEachFn(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitComboBoxEachLink(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitVDFParaSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitVDFReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetComboBoxEachLink();
		SetComboBoxEachFn();
		SetVDFParaSetting();
		SaveVDFParaSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	AutoType iterODTime = m_mapDefaultODTime.begin();
	AutoType endODTime  = m_mapDefaultODTime.end();
	if(iterODTime != endODTime)
	{
		AutoType iterDefaultODTime = m_mapDefaultODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		if(iterDefaultODTime != endODTime)
		{
			InitODTimeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitODTimeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	else
	{
		SetODTimeSetting();
		SaveODTimeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	}

	return TRUE;
}

void KDefaultHighwayAssignment::InitModeReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_0_COLUMN_Matrix,   _T("Mode"),    30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("통행수단"));
	}

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_1_COLUMN_Occupancy,_T("Occupancy"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("평균 재차인원"));
	}

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_2_COLUMN_PCE,      _T("PCE"),       30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("승용차 환산계수"));
	}
}

void KDefaultHighwayAssignment::InitVDFReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndVDFReport.AddColumn(new CXTPReportColumn(_0_COLUMN_Function, _T("Function No."), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("함수번호"));
	}

	pColumn = m_wndVDFReport.AddColumn(new CXTPReportColumn(_1_COLUMN_Alpha,    _T("α"),		    30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);

	pColumn = m_wndVDFReport.AddColumn(new CXTPReportColumn(_2_COLUMN_Beta,     _T("β"),			30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);

	pColumn = m_wndVDFReport.AddColumn(new CXTPReportColumn(_3_COLUMN_A,        _T("a"),			30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);

	pColumn = m_wndVDFReport.AddColumn(new CXTPReportColumn(_4_COLUMN_B,        _T("b"),			30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
}

void KDefaultHighwayAssignment::UpdateInitVDFReportCtrl()
{
	KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntion(m_mapDefaultVDFFuntion);

	int nFnGroupID = m_cboClass.GetItemData(m_cboClass.GetCurSel());
    CString strFnGroupName = (_T(""));

	try
	{
		m_wndVDFReport.ResetContent();

		CXTPReportRecords*		pRecords		 = m_wndVDFReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		AutoType findVDFFn = m_mapDefaultVDFFuntion.find(nFnGroupID);
		AutoType endVDFFn  = m_mapDefaultVDFFuntion.end();

		if (findVDFFn != endVDFFn)
		{
			std::vector<KVDFFunction>& vecVDFFunction = findVDFFn->second;
			size_t nUserCount   = vecVDFFunction.size();

			for (size_t i =0; i < nUserCount; i++)
			{
				KVDFFunction oVDFFunction = vecVDFFunction[i];
				int nFn					  = oVDFFunction.nFn;
				double dAlpha			  = oVDFFunction.dAlpha;
				double dBeta			  = oVDFFunction.dBeta;
				double dA				  = oVDFFunction.dA;
				double dB				  = oVDFFunction.dB;

				pRecord = m_wndVDFReport.AddRecord(new CXTPReportRecord());

				pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(nFn));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBeta, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dA, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dB, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);
			}
		}
		m_wndVDFReport.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}

void KDefaultHighwayAssignment::GetDefaultModeInfo(int a_nMasterCode, std::map<int, CString>& a_mapGetDefaultModeID)
{
	try
	{
		AutoType iterDefault = m_mapDefaultOccupancyPCEMode.find(a_nMasterCode);
		AutoType endDefault  = m_mapDefaultOccupancyPCEMode.end();
		std::vector<KOccupancyPCEMode>& vecOccupancyPCEMode = iterDefault->second;
		int nCount = vecOccupancyPCEMode.size();

		if (iterDefault != endDefault)
		{
			for (int i = 0; i < nCount; i++)
			{
				KOccupancyPCEMode oOccupancyPCEMode  = vecOccupancyPCEMode[i];
				int				  nModeID			 = oOccupancyPCEMode.nModeID;
				CString           strMatrix		     = oOccupancyPCEMode.strModeName;
				double            dOccupancy		 = oOccupancyPCEMode.dOccupancy;
				double            dPCE			     = oOccupancyPCEMode.dPCE;

				a_mapGetDefaultModeID.insert(std::make_pair(nModeID, strMatrix));
			}
		}
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


void KDefaultHighwayAssignment::InitModeReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndModeReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndModeReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;
		int						a                = 0;
		
		AutoType iterDefault = m_mapDefaultOccupancyPCEMode.find(a_nMasterCode);
		AutoType endDefault  = m_mapDefaultOccupancyPCEMode.end();
		std::vector<KOccupancyPCEMode>& vecOccupancyPCEMode = iterDefault->second;

		AutoType iterGetModeCheck = m_mapGetModeinfo.begin();
		AutoType endGetModeCheck  = m_mapGetModeinfo.end();
		size_t nGetModeCount = m_mapGetModeinfo.size();

		if(iterDefault != endDefault && iterGetModeCheck != endGetModeCheck)
		{
			for (size_t i = 0; i < nGetModeCount; i++)
			{
				int     nGetModeID     = iterGetModeCheck->first;
				CString strGetModename = iterGetModeCheck->second;

				AutoType iterDefaultModeID = m_mapGetDefaultModeID.find(nGetModeID);
				AutoType endDefaultModeID  = m_mapGetDefaultModeID.end();

				if(iterDefaultModeID != endDefaultModeID)
				{
					KOccupancyPCEMode oOccupancyPCEMode  = vecOccupancyPCEMode[a];
					int				  nModeID			 = oOccupancyPCEMode.nModeID;
					CString           strMatrix		     = oOccupancyPCEMode.strModeName;
					double            dOccupancy		 = oOccupancyPCEMode.dOccupancy;
					double            dPCE			     = oOccupancyPCEMode.dPCE;

					pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMatrix));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);
					a++;
				}
				else
				{
					double  dOccupancy			 = 0.0;
					double  dPCE				 = 0.0;

					if(strGetModename == KDefaultParaPCEOccupancy::DEFAULT_AUTO)
					{
						dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_AUTO;
						dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_AUTO;
					}
					else if(strGetModename == KDefaultParaPCEOccupancy::DEFAULT_TAXI)
					{
						dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_TAXI;
						dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_TAXI;
					}
					else if(strGetModename == KDefaultParaPCEOccupancy::DEFAULT_BUS)
					{
						dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_BUS;
						dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_BUS;
					}
					else
					{
						dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_OTHER;
						dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_OTHER;
					}

					pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strGetModename));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(TRUE);
				}
                iterGetModeCheck++;
			}
		}
		m_wndModeReport.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}

void KDefaultHighwayAssignment::InitVDFReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndVDFReport.ResetContent();

		CXTPReportRecords*		pRecords		 = m_wndVDFReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		AutoType iterVDFFn = m_mapDefaultVDFPara.find(a_nMasterCode);
		AutoType endVDFFn  = m_mapDefaultVDFPara.end();

		if (iterVDFFn != endVDFFn)
		{
			KVDFPara oVDFPara = iterVDFFn->second;
			m_strUseFunctionGroup	= oVDFPara.strUseFunctionID;
		}

		AutoType iterGroup = m_mapDefaultVDFFuntionGroup.find(m_strUseFunctionGroup);
		AutoType endGroup  = m_mapDefaultVDFFuntionGroup.end();

		if (iterGroup != endGroup)
		{
			int nUseFunctionID = iterGroup->second;

			AutoType iter = m_mapDefaultVDFFuntion.find(nUseFunctionID);
			AutoType end  = m_mapDefaultVDFFuntion.end();

			std::vector<KVDFFunction>& vecVDFFunction = iter->second;
			size_t nUserCount   = vecVDFFunction.size();

			for (size_t i =0; i < nUserCount; i++)
			{
				KVDFFunction oVDFFunction = vecVDFFunction[i];
				int nFn					  = oVDFFunction.nFn;
				double dAlpha			  = oVDFFunction.dAlpha;
				double dBeta			  = oVDFFunction.dBeta;
				double dA				  = oVDFFunction.dA;
				double dB				  = oVDFFunction.dB;

				pRecord = m_wndVDFReport.AddRecord(new CXTPReportRecord());

				pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(nFn));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBeta, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dA, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dB, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);
			}
		}
		m_wndVDFReport.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}

void KDefaultHighwayAssignment::InitComboBoxEachFn(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}
	try
	{
		AutoType iterVDFFn = m_mapDefaultVDFPara.find(a_nMasterCode);
		AutoType endVDFFn  = m_mapDefaultVDFPara.end();

		CString strFNSelect (_T("--------Select--------"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strFNSelect = _T("---------선택---------");
		}

		if (iterVDFFn != endVDFFn)
		{
			KVDFPara oVDFPara = iterVDFFn->second;
			m_strUseFunctionGroup	= oVDFPara.strUseFunctionID;
		}

		AutoType iterGroup = m_mapDefaultVDFFuntionGroup.find(m_strUseFunctionGroup);
		AutoType endGroup  = m_mapDefaultVDFFuntionGroup.end();

		if (iterGroup != endGroup)
		{
			int nUseFunctionID = iterGroup->second;

			CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
			KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			m_cboClass.ResetContent();
	
			int nCursorIndex = m_cboClass.InsertString(0, strFNSelect);
			m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)0 );
						
			if (m_strUseFunctionGroup == strFNSelect)
			{
				m_cboClass.SetCurSel(nCursorIndex);
			}

			while (oResultSetPtr->Next())
			{
				CString strFnName = oResultSetPtr->GetValueString(1);
				nCursorIndex = m_cboClass.AddString(strFnName);
				m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0));

				if (m_strUseFunctionGroup == strFnName)
				{
					m_cboClass.SetCurSel(nCursorIndex);
				}
				else
				{
					m_cboClass.SetCurSel(0);
				}
			}
		}
		else
		{
			CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
			KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			m_cboClass.ResetContent();

			int nCursorIndex = m_cboClass.InsertString(0, strFNSelect);
			m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)0 );

			if (m_strUseFunctionGroup == strFNSelect)
			{
				m_cboClass.SetCurSel(nCursorIndex);
			}

			while (oResultSetPtr->Next())
			{
				CString strFnName = oResultSetPtr->GetValueString(1);
				nCursorIndex = m_cboClass.AddString(strFnName);
				m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );

				if (m_strUseFunctionGroup == strFnName)
				{
					m_cboClass.SetCurSel(nCursorIndex);
				}
				else
				{
					m_cboClass.SetCurSel(0);
				}
			}
		}
		// 빵꾸처리
		if (m_cboClass.GetCount() > 0) {
			int nIndex = m_cboClass.GetCurSel();
			if (-1 == nIndex) {
				m_cboClass.SetCurSel(0);
			}
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultHighwayAssignment::InitComboBoxEachLink(int a_nMasterCode)
{
	try
	{
		m_cboAlpha.ResetContent();
		m_cboBeta.ResetContent();
		m_cboA.ResetContent();
		m_cboB.ResetContent();

		int nCursorIndex = 0;

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboAlpha.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboAlpha.InsertString(-1, _T("----Select----"));
		}
		m_cboAlpha.SetItemData(nCursorIndex, (DWORD_PTR)0);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboBeta.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboBeta.InsertString(-1, _T("----Select----"));
		}
		m_cboBeta.SetItemData(nCursorIndex, (DWORD_PTR)0);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboA.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboA.InsertString(-1, _T("----Select----"));
		}
		m_cboA.SetItemData(nCursorIndex, (DWORD_PTR)0);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboB.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboB.InsertString(-1, _T("----Select----"));
		}
		m_cboB.SetItemData(nCursorIndex, (DWORD_PTR)0);

		AutoType iter = m_mapDefaultVDFPara.find(a_nMasterCode);
		AutoType end  = m_mapDefaultVDFPara.end();

		if ( iter != end )
		{
			KVDFPara oVDFPara = iter->second;

			double  dAllLinkAlpha		= oVDFPara.dAllLinkAlpha;
			double	dAllLinkBeta		= oVDFPara.dAllLinkBeta;
			CString strFunctionID		= oVDFPara.strUseFunctionID;
			CString strAlpha		    = oVDFPara.strLinkColumnAlpha;
			CString strBeta		        = oVDFPara.strLinkColumnBeta;
			CString strA    		    = oVDFPara.strLinkColumnA;
			CString strB		        = oVDFPara.strLinkColumnB;
			int     nVDFChoice			= oVDFPara.nVDFChoice;
			CString strSelect           = (_T("----Select----"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strSelect = _T("-----선택-----");
			}

			KIOTables* pTables = m_pTarget->Tables();
			KIOTable*  pTable = pTables->FindTable(TABLE_LINK);

			const KIOColumns* pColumns = pTable->Columns();

			int nColumnCount = pColumns->ColumnCount();

			for(int i = 0; i<nColumnCount; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);
				if(pColumn->DataType() == KEMIODataTypeDouble)
				{
					nCursorIndex = m_cboAlpha.InsertString(-1, pColumn->Caption());
					m_cboAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					if (pColumn->Caption() == strAlpha )
					{
						m_cboAlpha.SetCurSel(nCursorIndex);
					}

					nCursorIndex = m_cboBeta.InsertString(-1, pColumn->Caption());
					m_cboBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					if (pColumn->Caption() == strBeta)
					{
						m_cboBeta.SetCurSel(nCursorIndex);
					}

					nCursorIndex = m_cboA.InsertString(-1, pColumn->Caption());
					m_cboA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
					if (pColumn->Caption() == strA)
					{
						m_cboA.SetCurSel(nCursorIndex);
					}

					nCursorIndex = m_cboB.InsertString(-1, pColumn->Caption());
					m_cboB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

					if (pColumn->Caption() == strB)
					{
						m_cboB.SetCurSel(nCursorIndex);
					}

				}
			}
			if (strAlpha == strSelect )
			{
				m_cboAlpha.SetCurSel(0);
			}

			if (strBeta == strSelect )
			{
				m_cboBeta.SetCurSel(0);
			}

			if (strA == strSelect )
			{
				m_cboA.SetCurSel(0);
			}

			if (strB == strSelect )
			{
				m_cboB.SetCurSel(0);
			}
		}
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

void KDefaultHighwayAssignment::InitVDFParaSetting(int a_nMasterCode)
{
	try
	{
		AutoType iterDefault = m_mapDefaultVDFPara.find(a_nMasterCode);
		AutoType endDefault  = m_mapDefaultVDFPara.end();

		if ( iterDefault != endDefault )
		{
			KVDFPara oVDFPara = iterDefault->second;

			double  dAllLinkAlpha		= oVDFPara.dAllLinkAlpha;
			double	dAllLinkBeta		= oVDFPara.dAllLinkBeta;

			CString strValue(_T(""));
			strValue.Format(_T("%.4f"), dAllLinkAlpha);
			m_edtAlpha.SetWindowText(strValue);
			m_edtAlpha.EnableRealNumber(true);

			strValue.Format(_T("%.4f"), dAllLinkBeta);
			m_edtBeta.SetWindowText(strValue);
			m_edtBeta.EnableRealNumber(true);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return; 
}

void KDefaultHighwayAssignment::InitODTimeSetting(int a_nMasterCode)
{
	try
	{
		AutoType iterDefault = m_mapDefaultODTime.find(a_nMasterCode);
		AutoType endDefault = m_mapDefaultODTime.end();
		if (iterDefault != endDefault)
		{
			KODTime oODTime = iterDefault->second;

			double  dTravelTime		= oODTime.dTravelTime;
			double	dDurationTime	= oODTime.dTimeDuration;

			CString strValue(_T(""));
			strValue.Format(_T("%.3f"), dTravelTime);
			m_edtTravelTime.SetWindowText(strValue);
			m_edtTravelTime.EnableRealNumber(true);
			m_edtTravelTime.EnableMinMax(0.0, 10000.0);

			strValue.Format(_T("%.3f"), dDurationTime);
			m_edtTime.SetWindowText(strValue);
			m_edtTime.EnableRealNumber(true);
			m_edtTime.EnableMinMax(0.0, 24.0);
		}
		else
		{
			AutoType iterSystem = m_mapDefaultODTime.find(0);
			AutoType endSystem = m_mapDefaultODTime.end();
			if(iterSystem != iterSystem)
			{
				KODTime oODTime = iterSystem->second;

				double  dTravelTime		= oODTime.dTravelTime;
				double	dDurationTime	= oODTime.dTimeDuration;

				CEdit* pEdit;
				CString strValue(_T(""));

				strValue.Format(_T("%.3f"), dTravelTime);
				pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TRAVELTIME);
				SetDlgItemText(IDC_EDIT_TRAVELTIME, strValue);

				strValue.Format(_T("%.3f"), dDurationTime);
				pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
				SetDlgItemText(IDC_EDIT_TIME, strValue);
			}
			else
			{
				CEdit* pEdit;
				CString strValue(_T(""));

				double  dTravelTime(1.0);
				double	dDurationTime(1.0);

				strValue.Format(_T("%.3f"), dTravelTime);
				pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TRAVELTIME);
				SetDlgItemText(IDC_EDIT_TRAVELTIME, strValue);

				strValue.Format(_T("%.3f"), dDurationTime);
				pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
				SetDlgItemText(IDC_EDIT_TIME, strValue);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return; 
}

void KDefaultHighwayAssignment::SetModeReportRecord()
{
	try
	{
		m_wndModeReport.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndModeReport.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		AutoType iter = m_mapGetModeinfo.begin();
		AutoType end  = m_mapGetModeinfo.end();

		if(iter != end)
		{
			size_t	nUserCount = m_mapGetModeinfo.size();

			for (size_t i = 0; i < nUserCount; i++)
			{
				CString strMatrix	= iter->second;
				double  dOccupancy	= 0.0;
				double  dPCE		= 0.0;
				int     nModeCode   = iter->first;
#pragma region old
// 				if(nModeCode == KDefaultParaPCEOccupancy::DEFAULT_AUTO)
// 				{
// 					dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_AUTO;
// 					dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_AUTO;
// 				}
// 				else if(nModeCode == KDefaultParaPCEOccupancy::DEFAULT_TAXI)
// 				{
// 					dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_TAXI;
// 					dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_TAXI;
// 				}
// 				else if(nModeCode == KDefaultParaPCEOccupancy::DEFAULT_BUS)
// 				{
// 					dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_BUS;
// 					dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_BUS;
// 				}
// 				else
// 				{
// 					dOccupancy	= KDefaultParaPCEOccupancy::DEFAULT_OCCUPANCY_OTHER;
// 					dPCE		= KDefaultParaPCEOccupancy::DEFAULT_PCE_OTHER;
// 				}
// 
// 				if ( m_pProject->GetProjectType() == KEMUserDefineProject)
// 				{
// 					dOccupancy  = 1.0;
// 					dPCE        = 1.0;
// 				}
#pragma endregion old
				dOccupancy = 1.0;
				dPCE       = 1.0;

				pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMatrix));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dOccupancy, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dPCE, _T("%.3f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(TRUE);

				++iter;
			}
			m_wndModeReport.Populate();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}

void KDefaultHighwayAssignment::SetComboBoxEachFn()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}
	try
	{
		CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		m_cboClass.ResetContent();

		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboClass.InsertString(0, _T("---------선택---------"));
		}
		else {
			nCursorIndex = m_cboClass.InsertString(0, _T("--------Select--------"));
		}
		//int nCursorIndex = m_cboClass.InsertString(0, _T("------ Select --------"));
		m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		while (oResultSetPtr->Next())
		{
			CString strFnName = oResultSetPtr->GetValueString(1);
			nCursorIndex = m_cboClass.AddString(strFnName);
			m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );
		}

		int nComboCount = m_cboClass.GetCount();

		for (int i = 0; i < nComboCount; ++i)
		{
			int nFnGroupId = m_cboClass.GetItemData(i);

			if ( nFnGroupId == m_nFnGroupID )
			{
				m_cboClass.SetCurSel(i);
			}
		}

		// 빵꾸처리
		if (m_cboClass.GetCount() > 0) {
			int nIndex = m_cboClass.GetCurSel();
			if (-1 == nIndex) {
				m_cboClass.SetCurSel(0);
			}
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultHighwayAssignment::SetComboBoxEachLink()
{
	try
	{
		m_cboAlpha.ResetContent();
		m_cboBeta.ResetContent();
		m_cboA.ResetContent();
		m_cboB.ResetContent();

		int nCursorIndex = 0;

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboAlpha.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboAlpha.InsertString(-1, _T("----Select----"));
		}
		//nCursorIndex = m_cboAlpha.InsertString(-1, _T("--- Select ---"));
		m_cboAlpha.SetItemData(nCursorIndex, (DWORD_PTR)0);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboBeta.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboBeta.InsertString(-1, _T("----Select----"));
		}
		//nCursorIndex = m_cboBeta.InsertString(-1, _T("--- Select ---"));
		m_cboBeta.SetItemData(nCursorIndex, (DWORD_PTR)0);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboA.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboA.InsertString(-1, _T("----Select----"));
		}
		//nCursorIndex = m_cboA.InsertString(-1, _T("--- Select ---"));
		m_cboA.SetItemData(nCursorIndex, (DWORD_PTR)0);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboB.InsertString(-1, _T("-----선택-----"));
		}
		else {
			nCursorIndex = m_cboB.InsertString(-1, _T("----Select----"));
		}
		//nCursorIndex = m_cboB.InsertString(-1, _T("--- Select ---"));
		m_cboB.SetItemData(nCursorIndex, (DWORD_PTR)0);

		double  dAllLinkAlpha		= 0.15;
		double	dAllLinkBeta		= 4.0;
		CString strAlpha		    (_T("Alpha"));
		CString strBeta		        (_T("Beta"));
		CString strA    		    (_T("Constant_Cost"));
		CString strB		        (_T("Variable_Cost"));
		CString strSelect           = (_T("----Select----"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strSelect = _T("-----선택-----");
		}

		KIOTables* pTables = m_pTarget->Tables();
		KIOTable*  pTable = pTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns = pTable->Columns();
		
		int nColumnCount = pColumns->ColumnCount();

		for(int i = 0; i<nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				nCursorIndex = m_cboAlpha.InsertString(-1, pColumn->Caption());
				m_cboAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				if (pColumn->Caption() == strAlpha )
				{
					m_cboAlpha.SetCurSel(nCursorIndex);
				}

				nCursorIndex = m_cboBeta.InsertString(-1, pColumn->Caption());
				m_cboBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				if (pColumn->Caption() == strBeta)
				{
					m_cboBeta.SetCurSel(nCursorIndex);
				}

				nCursorIndex = m_cboA.InsertString(-1, pColumn->Caption());
				m_cboA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
				if (pColumn->Caption() == strA)
				{
					m_cboA.SetCurSel(nCursorIndex);
				}

				nCursorIndex = m_cboB.InsertString(-1, pColumn->Caption());
				m_cboB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);

				if (pColumn->Caption() == strB)
				{
					m_cboB.SetCurSel(nCursorIndex);
				}
			}
		}
		if (strAlpha == strSelect )
		{
			m_cboAlpha.SetCurSel(0);
		}

		if (strBeta == strSelect )
		{
			m_cboBeta.SetCurSel(0);
		}

		if (strA == strSelect )
		{
			m_cboA.SetCurSel(0);
		}

		if (strB == strSelect )
		{
			m_cboB.SetCurSel(0);
		}
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

void KDefaultHighwayAssignment::SetVDFParaSetting()
{
	try
	{
		double  dAllLinkAlpha(0.15);
		double	dAllLinkBeta (4);

		CString strValue(_T(""));
		strValue.Format(_T("%.4f"), dAllLinkAlpha);
		m_edtAlpha.SetWindowText(strValue);
		m_edtAlpha.EnableRealNumber(true);

		strValue.Format(_T("%.4f"), dAllLinkBeta);
		m_edtBeta.SetWindowText(strValue);
		m_edtBeta.EnableRealNumber(true);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return; 
}

void KDefaultHighwayAssignment::SetODTimeSetting()
{
	try
	{
		double  dTravelTime		= 1.0;
		double	dDurationTime	= 10.0;
		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.3f"), dTravelTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TRAVELTIME);
		SetDlgItemText(IDC_EDIT_TRAVELTIME, strValue);

		strValue.Format(_T("%.3f"), dDurationTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		SetDlgItemText(IDC_EDIT_TIME, strValue);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return; 
}

void KDefaultHighwayAssignment::SaveOccupancyPCEMode(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_PCE_MODE);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_PCE_MODE);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( master_code, detail_mode_id, mode_name, occupancy, pce ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_PCE_MODE);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndModeReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		int							nRecordCount = pRecords->GetCount();

		CString                     strMatrix    (_T(""));
		CString						strOccupancy (_T(""));
		CString						strPCE       (_T(""));
		double						dOccupancy   = 0.0;	
		double						dPCE         = 0.0;	

		AutoType iter = m_mapGetModeinfo.begin();
		AutoType end  = m_mapGetModeinfo.end();

		if(iter != end)
		{
			for (int i = 0; i < nRecordCount; i++)
			{
				int nModeid	      = iter->first;

				pRecord           = pRecords->GetAt(i);

				pItemText         = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_Matrix);
				strMatrix         = pItemText ->GetCaption(NULL);

				pItemNum          = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_Occupancy);
				strOccupancy       = pItemNum ->GetCaption(NULL);
				dOccupancy        = _ttof(strOccupancy);

				pItemNum          = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_PCE);
				strPCE            = pItemNum ->GetCaption(NULL);
				dPCE              = _ttof(strPCE);

				spPrepareStatement->BindInt    (1, a_nMasterCode);
				spPrepareStatement->BindInt    (2, nModeid);
				spPrepareStatement->BindText   (3, strMatrix);
				spPrepareStatement->BindDouble (4, dOccupancy);
				spPrepareStatement->BindDouble (5, dPCE);
				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();

				++iter;
			}
			spDBaseConnection->Commit();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultHighwayAssignment::SaveVDFParaSetting(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_VDF_PARA);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_VDF_PARA);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		int nSelCur     = 0;

		CString strAlpha  (_T(""));
		GetDlgItemText(IDC_EDIT_ALPHA, strAlpha);
		double dAlpha = _ttof(strAlpha);

		CString strBeta	 (_T(""));
		GetDlgItemText(IDC_EDIT_BETA,  strBeta);
		double dBeta = _ttof(strBeta);

		nSelCur = m_cboClass.GetCurSel();
		CString strFunctionID = (_T(""));
		m_cboClass.GetLBText(nSelCur, strFunctionID);

		nSelCur = m_cboAlpha.GetCurSel();
		CString strColumnAlpha = (_T(""));
		m_cboAlpha.GetLBText(nSelCur, strColumnAlpha);

		nSelCur = m_cboBeta.GetCurSel();
		CString strColumnBeta = (_T(""));
		m_cboBeta.GetLBText(nSelCur, strColumnBeta);

		nSelCur = m_cboA.GetCurSel();
		CString strColumnA = (_T(""));
		m_cboA.GetLBText(nSelCur, strColumnA);

		nSelCur = m_cboB.GetCurSel();
		CString strColumnB = (_T(""));
		m_cboB.GetLBText(nSelCur, strColumnB);

		int nVDFChoice = m_nRadiobtnFunction;

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_VDF_PARA);
		strSQL.AppendFormat(_T(" (Master_Code, AllLink_Alpha, AllLink_Beta, UseFunction_ID, LinkColumn_Alpha, "));
		strSQL.AppendFormat(_T(" LinkColumn_Beta, LinkColumn_a, LinkColumn_b, VDFChoice) "));
		strSQL.AppendFormat(_T(" VALUES (%d, %.6f, %.6f, '%s', '%s', '%s', '%s', '%s', %d )"), a_nMasterCode, dAlpha, dBeta, strFunctionID, strColumnAlpha, strColumnBeta, strColumnA, strColumnB, nVDFChoice);

		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}

void KDefaultHighwayAssignment::SaveODTimeSetting(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_OD_TIME);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_OD_TIME);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strTravelTime  (_T(""));
		GetDlgItemText(IDC_EDIT_TRAVELTIME, strTravelTime);
		double dTravelTime = _ttof(strTravelTime);

		CString strDurationTime	 (_T(""));
		GetDlgItemText(IDC_EDIT_TIME,	  strDurationTime);
		double dDurationTime = _ttof(strDurationTime);


		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_OD_TIME);
		strSQL.AppendFormat(_T(" (Master_Code, Time_Duration, Travel_Time ) "));
		strSQL.AppendFormat(_T(" VALUES (%d, %.3f, %.3f)"),a_nMasterCode, dDurationTime, dTravelTime);

		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}


void KDefaultHighwayAssignment::OnCbnSelchangeComboFunction()
{
	int nFnGroupID = m_cboClass.GetItemData(m_cboClass.GetCurSel());

   try
   {
		UpdateInitVDFReportCtrl();
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultHighwayAssignment::OnBnClickedOk()
{
	try
	{
		SaveODTimeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveVDFParaSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveOccupancyPCEMode(KDefaultParameterMasterCode::USER_MASETER_CODE);

		KResizeDialogEx::OnOK();
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


void KDefaultHighwayAssignment::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KDefaultHighwayAssignment::OnBnClickedDefault()
{
	try
	{
		KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(m_mapDefaultOccupancyPCEMode);
		KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(m_mapDefaultODTime);
		KDBaseDefaultParaHighwayAssign::LoadDefaultVDFPara(m_mapDefaultVDFPara);
		KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntion(m_mapDefaultVDFFuntion);
		KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntionGroup(m_mapDefaultVDFFuntionGroup);

		AutoType iterMode = m_mapDefaultOccupancyPCEMode.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endMode  = m_mapDefaultOccupancyPCEMode.end();

		if(iterMode != endMode)
		{
			InitModeReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else 
		{
			InitModeReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}

		AutoType iterVDFPara = m_mapDefaultVDFPara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endVDFPara  = m_mapDefaultVDFPara.end();
		if(iterVDFPara != endVDFPara)
		{
			InitComboBoxEachFn(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitComboBoxEachLink(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitVDFParaSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitVDFReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitComboBoxEachFn(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitComboBoxEachLink(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitVDFParaSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitVDFReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}

		AutoType iterODTime = m_mapDefaultODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endODTime  = m_mapDefaultODTime.end();
		if(iterODTime != endODTime)
		{
			InitODTimeSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitODTimeSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultHighwayAssignment::OnBnClickedButtonSetbprfn()
{
	m_nComboFnCursor = m_cboClass.GetCurSel();
	int nFnGroupID   = m_cboClass.GetItemData(m_nComboFnCursor);

	KABPRFnManagerDlg oDlg(m_pTarget, nFnGroupID);

	if(oDlg.DoModal() == IDOK)
	{
		m_nFnGroupID = oDlg.FnGroupID();

		KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntionGroup(m_mapDefaultVDFFuntionGroup);
		UpdateComBoxEachFn();
	}

	UpdateInitVDFReportCtrl();	
}

void KDefaultHighwayAssignment::UpdateComBoxEachFn()
{
	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		if (spDBaseConnection == NULL)
		{
			TxLogDebugException();
			return;
		}

		CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		m_cboClass.ResetContent();

		int nCursorIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCursorIndex = m_cboClass.InsertString(0, _T("---------선택---------"));
		}
		else {
			nCursorIndex = m_cboClass.InsertString(0, _T("--------Select--------"));
		}
		//int nCursorIndex = m_cboClass.InsertString(0, _T("------ Select --------"));
		m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		while (oResultSetPtr->Next())
		{
			CString strFnName = oResultSetPtr->GetValueString(1);
			nCursorIndex = m_cboClass.AddString(strFnName);
			m_cboClass.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );
		}

		if( m_nFnGroupID != 0)
		{
			int nComboCount = m_cboClass.GetCount();

			for (int i = 0; i < nComboCount; ++i)
			{
				int nFnGroupId = m_cboClass.GetItemData(i);

				if ( nFnGroupId == m_nFnGroupID )
				{
					m_cboClass.SetCurSel(i);
				}
			}
		}

		// 빵꾸처리
		if (m_cboClass.GetCount() > 0) {
			int nIndex = m_cboClass.GetCurSel();
			if (-1 == nIndex) {
				m_cboClass.SetCurSel(0);
			}
		}
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

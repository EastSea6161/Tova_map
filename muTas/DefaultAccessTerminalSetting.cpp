// DefaultAccessTerminalSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultAccessTerminalSetting.h"
#include "afxdialogex.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"

enum TerminalSetting
{
	_0_COLUMN_Type,
	_1_COLUMN_Name,
	_2_COLUMN_Boundary
};

// KDefaultAccessTerminalSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultAccessTerminalSetting, KResizeDialogEx)

KDefaultAccessTerminalSetting::KDefaultAccessTerminalSetting(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultAccessTerminalSetting::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KDefaultAccessTerminalSetting::~KDefaultAccessTerminalSetting()
{
}

void KDefaultAccessTerminalSetting::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT,  m_wndReport);
}


BEGIN_MESSAGE_MAP(KDefaultAccessTerminalSetting, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KDefaultAccessTerminalSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultAccessTerminalSetting::OnBnClickedDefault)
END_MESSAGE_MAP()


// KDefaultAccessTerminalSetting 메시지 처리기입니다.


BOOL KDefaultAccessTerminalSetting::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KReportCtrlSetting::Default(m_wndReport, TRUE);

		InitReportHeader();

		KDBaseDefaultParaAccessTerminalSetting::LoadDefaultAccessTerminal(m_mapDefaultAccessTerminal);

		if (m_mapDefaultAccessTerminal.size()< 1)
		{
			SetReportRecord();
			SaveAccessTerminalSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
		else
		{
			AutoType iterDefault = m_mapDefaultAccessTerminal.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

			int nCode(0);
			if (iterDefault != m_mapDefaultAccessTerminal.end())
				nCode = KDefaultParameterMasterCode::USER_MASETER_CODE;
			else
				nCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;

			UpdateReportRecord(nCode);
			SaveAccessTerminalSetting(nCode);
			//InitReportRecord(nCode);
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

	return TRUE;
}

void KDefaultAccessTerminalSetting::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(_0_COLUMN_Type,     _T("Type"), 15));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		pColumn->SetVisible(FALSE);		// 명지대 추가

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(_1_COLUMN_Name,     _T("Terminal Type"), 50));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("터미널 타입"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(_2_COLUMN_Boundary, _T("Boundary Distance"), 35));
		pColumn->SetHeaderAlignment(xtpColumnTextCenter);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("반경(Km)"));
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

void KDefaultAccessTerminalSetting::InitReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		AutoType iterDefault = m_mapDefaultAccessTerminal.find(a_nMasterCode);
		AutoType endDefault  = m_mapDefaultAccessTerminal.end();

		if(iterDefault != endDefault)
		{
			std::vector<KAccessTerminal>& vecAccessTerminal = iterDefault->second;
			size_t nDefaultCount = vecAccessTerminal.size();

			for(size_t i = 0; i < nDefaultCount; i++)
			{
				KAccessTerminal oAccessTerminal = vecAccessTerminal[i];
				int				nType			= oAccessTerminal.nType;
				CString			strName			= oAccessTerminal.strCodeName;
				double			dBoundaryDist	= oAccessTerminal.dBoundaryDist;

				CString strType(_T(""));
				strType.Format(_T("%d"), nType);

				pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strType));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strName));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dBoundaryDist, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);
			}
		}

		m_wndReport.Populate();
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

void KDefaultAccessTerminalSetting::SetReportRecord()
{
	try
	{
		m_wndReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		KDBaseDefaultParaAccessTerminalSetting::GetTerminalInfo(m_pTarget, m_mapSetAccessTerminalInfo);
		AutoType iter = m_mapSetAccessTerminalInfo.begin();
		AutoType end  = m_mapSetAccessTerminalInfo.end();

		if(iter != end)
		{
			size_t nCount = m_mapSetAccessTerminalInfo.size();

			for(size_t i = 0; i < nCount; i++)
			{
			    std::vector<KAccessTerminal>& vecAccessTerminal = iter->second;
				KAccessTerminal oAccessTerminal = vecAccessTerminal[0];
				int				nType			= iter->first;
				CString			strName			= oAccessTerminal.strCodeName;
				double			dBoundaryDist	= oAccessTerminal.dBoundaryDist;

				CString strType(_T(""));
				strType.Format(_T("%d"), nType);

				pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

				pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strType));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strName));
				pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem ->SetAlignment(xtpColumnTextCenter);
				pItem ->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dBoundaryDist, _T("%.2f")));
				pItem ->SetAlignment(xtpColumnTextCenter);

				++iter;
			}
		}

		m_wndReport.Populate();
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

void KDefaultAccessTerminalSetting::SaveAccessTerminalSetting(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_ACCESS_TERMINAL_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_ACCESS_TERMINAL_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strSQL(_T(""));
		strSQL.Format(
			_T(" INSERT INTO %s ")
			_T(" ( Master_Code, Type, Terminal_Name, Boundary_Dist ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_ACCESS_TERMINAL_SETTING);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		CXTPReportRecords*          pRecords  = m_wndReport.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		size_t	 nRecordCount = pRecords->GetCount();
		int      nType(0);
		CString  strType(_T(""));
		CString	 strCodeName(_T(""));
		CString  strBoundary(_T(""));
		double	 dBoundary(0.0);	

		for(size_t i = 0; i < nRecordCount; i++)
		{
			pRecord       = pRecords->GetAt(i);

			pItemText     = (CXTPReportRecordItemText*)pRecord->GetItem(_0_COLUMN_Type);
			strType       = pItemText ->GetCaption(NULL);
			nType		  = _ttoi(strType);

			pItemText     = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_Name);
			strCodeName   = pItemText ->GetCaption(NULL);
			
			pItemNum      = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_Boundary);
			strBoundary   = pItemNum ->GetCaption(NULL);
			dBoundary     = _ttof(strBoundary);

			spPrepareStatement->BindInt    (1, a_nMasterCode);
			spPrepareStatement->BindInt    (2, nType);
			spPrepareStatement->BindText   (3, strCodeName);
			spPrepareStatement->BindDouble (4, dBoundary);
			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
		spDBaseConnection->Commit();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}

bool KDefaultAccessTerminalSetting::InvalidateInputInfo()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i= 0; i< nRecordCount; i++)
	{
		CXTPReportRecord*           pRecord      = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItemNum  = nullptr;
		
		pItemNum             = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_Boundary);
		CString strBoundary  = pItemNum ->GetCaption(NULL);
		double dBoundary     = _ttof(strBoundary);

		if( !QBicStringChecker::IsNumeric(strBoundary))
		{
			AfxMessageBox(_T("접근 반경에 숫자 값을 입력해 주세요."));
			return false;
		}

		if(dBoundary <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			return false;
		}
	}
	return true;
}

void KDefaultAccessTerminalSetting::UpdateReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndReport.ResetContent(TRUE);

		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		KDBaseDefaultParaAccessTerminalSetting::MakeTerminalNode(m_mapTerminalNode);
		KDBaseDefaultParaAccessTerminalSetting::NodeCodeMatching(m_pTarget, m_setNodeSystemUserCodeMatching);
		KDBaseDefaultParaAccessTerminalSetting::GetTableCode(a_nMasterCode, m_setAccessTerminalTableCode);
		KDBaseDefaultParaAccessTerminalSetting::GetAccessTerminalTable(a_nMasterCode, m_mapGetAccessTerminalTable);
				
		AutoType iterCheckTerminal = m_mapTerminalNode.begin();
		AutoType endCheckTerminal  = m_mapTerminalNode.end();
		if(iterCheckTerminal!=endCheckTerminal)
		{
			size_t nCount = m_mapTerminalNode.size();

			for(size_t i = 0; i < nCount; i++)
			{
				int nSystemCode = iterCheckTerminal->first;

				AutoType iterFindNode = m_setNodeSystemUserCodeMatching.find(nSystemCode);
				AutoType endFindNode  = m_setNodeSystemUserCodeMatching.end();

				AutoType iterFindTable = m_setAccessTerminalTableCode.find(nSystemCode);
				AutoType endFindTable  = m_setAccessTerminalTableCode.end();

				if ((iterFindNode != endFindNode) && (iterFindTable != endFindTable))
				{
					AutoType iter = m_mapGetAccessTerminalTable.find(nSystemCode);
					AutoType end  = m_mapGetAccessTerminalTable.end();

					if(iter != end)
					{
						KAccessTerminal oAccessTerminal = iter->second;
						int				nType			= oAccessTerminal.nType;
						CString			strName			= oAccessTerminal.strCodeName;
						double			dBoundaryDist	= oAccessTerminal.dBoundaryDist;

						CString strType(_T(""));
						strType.Format(_T("%d"), nType);

						pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

						pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strType));
						pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
						pItem ->SetAlignment(xtpColumnTextCenter);
						pItem ->SetEditable(FALSE);

						pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strName));
						pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
						pItem ->SetAlignment(xtpColumnTextCenter);
						pItem ->SetEditable(FALSE);

						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dBoundaryDist, _T("%.2f")));
						pItem ->SetAlignment(xtpColumnTextCenter);
					}
				}
				else if(iterFindNode != endFindNode)
				{
					CString strType(_T(""));
					strType.Format(_T("%d"), nSystemCode);
					CString strName       = pCodeGroup->SingleCodeValue(nSystemCode);
				    double  dBoundaryDist = iterCheckTerminal->second;
					
					pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strType));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemText  (strName));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dBoundaryDist, _T("%.2f")));
					pItem ->SetAlignment(xtpColumnTextCenter);
				}
				iterCheckTerminal++;
			}
		}
	    m_wndReport.Populate();
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

void KDefaultAccessTerminalSetting::OnBnClickedOk()
{
	try
	{
		if ( !InvalidateInputInfo())
		{
			return;
		}

		SaveAccessTerminalSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	KResizeDialogEx::OnOK();
}

void KDefaultAccessTerminalSetting::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultAccessTerminalSetting::OnBnClickedDefault()
{
	try
	{
		KDBaseDefaultParaAccessTerminalSetting::LoadDefaultAccessTerminal(m_mapDefaultAccessTerminal);

		AutoType iter = m_mapDefaultAccessTerminal.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType end  = m_mapDefaultAccessTerminal.end();

		if(iter != end)
		{
			InitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			InitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
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

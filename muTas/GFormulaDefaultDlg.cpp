// GFormulaDefaultDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GFormulaDefaultDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"

enum
{
	_0_COLUMN_OBJECT     = 0,
	_1_COLUMN_FORMULA    = 1
};
enum 
{
	COLUMN_TOTAL_COUNT     = 2
};

// KGFormulaDefaultDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGFormulaDefaultDlg, KResizeDialogEx)

KGFormulaDefaultDlg::KGFormulaDefaultDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KGFormulaDefaultDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	if (KmzSystem::IsPassengerSystem())
	{
		m_emPurposeGroup = KEMPurposeGroupODBase;
	}
	else
	{
		m_emPurposeGroup = KEMPurposeGroupFreightOD;
	}
	
	if (nullptr != pParent)
	{
		pParent->GetWindowRect(&m_rectParent);
	}
}

KGFormulaDefaultDlg::~KGFormulaDefaultDlg()
{
}

void KGFormulaDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_cboMasterGroup);
}


BEGIN_MESSAGE_MAP(KGFormulaDefaultDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KGFormulaDefaultDlg::OnCbnSelchangeComboGroup)
	ON_BN_CLICKED(IDCANCEL, &KGFormulaDefaultDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void KGFormulaDefaultDlg::InitializeData()
{
	LoadDetailObject();
	LoadFormulaMasterData();
	LoadFormula();
}

void KGFormulaDefaultDlg::LoadFormulaMasterData()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		AfxMessageBox(_T("TOVA System File(tova) Not Found"));
		return;
	}

	std::vector<TFormulaMaster> vecTFormulaMaster;
	KImTasDBase::FormulaMasterData(spDBaseConnection, vecTFormulaMaster);

	m_cboMasterGroup.ResetContent();

	for (size_t i= 0; i< vecTFormulaMaster.size(); i++)
	{
		TFormulaMaster &oTFormulaMaster = vecTFormulaMaster[i];

		CString strDisplay(_T(""));
		/*strDisplay.Format(_T("%d: %s"), oTFormulaMaster.TCode, oTFormulaMaster.TName);*/
		strDisplay.Format(_T("%s"), oTFormulaMaster.TName);

		int nIndex = m_cboMasterGroup.AddString(strDisplay);
		m_cboMasterGroup.SetItemData(nIndex, (DWORD_PTR)oTFormulaMaster.TCode);
	}

	if(vecTFormulaMaster.size() > 0)
	{
		m_cboMasterGroup.SetCurSel(0);
	}
}


void KGFormulaDefaultDlg::LoadFormula()
{
	int nCur = m_cboMasterGroup.GetCurSel();
	if(nCur < 0)
	{
		return;
	}

	int nMasterCode = m_cboMasterGroup.GetItemData(nCur);

	KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		AfxMessageBox(_T("TOVA System File(tova) Not Found"));
		return;
	}

	std::map<int, std::map<int, std::vector<CString>>> mapFormula;
	KImTasDBase::GenerationFormula(spDBaseConnection, m_emPurposeGroup, nMasterCode, mapFormula);

	UpdateReportData(mapFormula);
}


void KGFormulaDefaultDlg::LoadDetailObject()
{
	m_mapObjectName.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	
	CString strSQL;
	strSQL.Format(
		_T(" SELECT Detail_Object_ID, Object_Name ")
		_T(" FROM detail_Object where Object_Group_Code = %d order by Detail_Object_Id "), m_emPurposeGroup);

	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			int     nObjectID      = spResultSet->GetValueInt   (0);
			CString strObjectName  = spResultSet->GetValueString(1);

			m_mapObjectName.insert(std::make_pair(nObjectID, strObjectName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		m_mapObjectName.clear();
	}
	catch (...)
	{
		TxLogDebugWarning();
		m_mapObjectName.clear();
	} 
}


// KGFormulaDefaultDlg 메시지 처리기입니다.
BOOL KGFormulaDefaultDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultC(m_wndReportCtrl);

	InitReportHeader();

	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KGFormulaDefaultDlg::InitReportHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_OBJECT,       _T("Object"),          40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetTreeColumn(TRUE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_FORMULA,       _T("Default_Formula"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	m_wndReportCtrl.Populate();
}


void KGFormulaDefaultDlg::ResizeComponent()
{
	SetResize(IDC_STATIC,       			      SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_COMBO1,       			      SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,       			      SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR3,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KGFormulaDefaultDlg::UpdateReportData(std::map<int, std::map<int, std::vector<CString>>> &a_mapFormula)
{
	m_wndReportCtrl.ResetContent();

	try
	{
		CXTPReportRecord*     pParRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = a_mapFormula.begin();
		AutoType itEnd = a_mapFormula.end();

		while(iter != itEnd)
		{
			//#1
			pParRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			int nObjectID = iter->first;
			CString strObjectName(_T(""));

			AutoType iterB  = m_mapObjectName.find(nObjectID);
			AutoType itEndB = m_mapObjectName.end();

			if(iterB != itEndB)
			{
				strObjectName = iterB->second;
			}

			pItem = pParRecord->AddItem(new CXTPReportRecordItemText(strObjectName));
			for (int i= 0; i< COLUMN_TOTAL_COUNT-1; i++)
			{
				pParRecord->AddItem(new CXTPReportRecordItemText());
			}
			pItem->SetAlignment(DT_LEFT);

			//#2
			CXTPReportRecords* pSeParRecords = pParRecord->GetChilds();

			std::map<int, std::vector<CString>> &mapFormulaText = iter->second;
			AutoType iterC  = mapFormulaText.begin();
			AutoType itEndC = mapFormulaText.end();
			while(iterC != itEndC)
			{
				int     nPAType   = iterC->first;
				CString strPAType(_T(""));
				if(KEMPATypeProduction == nPAType)
				{
					strPAType = PRODUCTION_STRING;
				}
				else
				{
					if (KmzSystem::IsPassengerSystem())
					{
						strPAType = ATTRACTION_STRING;
					}
					else
					{
						strPAType = CONSUMPTION_STRING;
					}
				}

				CXTPReportRecord*  pSeParRecord  = pSeParRecords->Add(new CXTPReportRecord); 
				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(strPAType));
				for (int i= 0; i< COLUMN_TOTAL_COUNT-1; i++)
				{
					pSeParRecord->AddItem(new CXTPReportRecordItemText());
				}

				//#3
				CXTPReportRecords* pThParRecords = pSeParRecord->GetChilds();
				std::vector<CString> &vecFormulaText = iterC->second;
				for (size_t k= 0; k< vecFormulaText.size(); k++)
				{
					CXTPReportRecord* pThParentRecord = pThParRecords->Add(new CXTPReportRecord());

					CString strIndex(_T(""));
					strIndex.Format(_T("#%d"), (k+1));
					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemText(strIndex));
					pItem->SetAlignment(DT_LEFT);

					CString &strFormular = vecFormulaText[k];
					pItem = pThParentRecord->AddItem(new CXTPReportRecordItemText(strFormular));
					pItem->SetAlignment(DT_LEFT);
				}

				++iterC;
			}

			++iter;
		}

		m_wndReportCtrl.Populate();
		m_wndReportCtrl.ExpandAll(TRUE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	} 
}


void KGFormulaDefaultDlg::OnCbnSelchangeComboGroup()
{
	UpdateData(TRUE);

	LoadFormula();
}


void KGFormulaDefaultDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

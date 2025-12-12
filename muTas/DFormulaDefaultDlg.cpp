// DFormulaDefaultDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DFormulaDefaultDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseConnector.h"

enum
{
	_0_COLUMN_OBJECT          = 0,
	_1_COLUMN_FUNTION_TYPE    = 1,
	_2_COLUMN_ALPHA           = 2,
	_3_COLUMN_BETA            = 3,
	_4_COLUMN_THETA           = 4,
	_5_COLUMN_VAR_RIJ         = 5
};
enum 
{
	COLUMN_TOTAL_COUNT     = 6
};

// KDFormulaDefaultDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDFormulaDefaultDlg, KResizeDialogEx)

KDFormulaDefaultDlg::KDFormulaDefaultDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDFormulaDefaultDlg::IDD, pParent)
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

KDFormulaDefaultDlg::~KDFormulaDefaultDlg()
{
}

void KDFormulaDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_cboMasterGroup);
}


BEGIN_MESSAGE_MAP(KDFormulaDefaultDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KDFormulaDefaultDlg::OnCbnSelchangeComboGroup)
	ON_BN_CLICKED(IDCANCEL, &KDFormulaDefaultDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KDFormulaDefaultDlg 메시지 처리기입니다.

void KDFormulaDefaultDlg::LoadDetailObject()
{
	m_mapObjectName.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format(
			_T(" SELECT Detail_Object_ID, Object_Name ")
			_T(" FROM detail_Object where Object_Group_Code = %d order by Detail_Object_Id "), m_emPurposeGroup);

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


void KDFormulaDefaultDlg::LoadFormulaMasterData()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		AfxMessageBox(_T("TOVA System File(tova) Not Found"));
		return;
	}

	try
	{
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
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDFormulaDefaultDlg::LoadFormula()
{
	try
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

		std::map<int, std::vector<TImpedanceFunction>> mapImpedanceFunction;
		KImTasDBase::DistributionFormula(spDBaseConnection, m_emPurposeGroup, nMasterCode, mapImpedanceFunction);

		UpdateReportData(mapImpedanceFunction);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


BOOL KDFormulaDefaultDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	LoadDetailObject();
	LoadFormulaMasterData();
	LoadFormula();

	KReportCtrlSetting::DefaultC(m_wndReportCtrl);

	InitReportHeader();
	ResizeComponent();

	CRect rect;
	GetWindowRect(rect);

	MoveWindow(m_rectParent.left + m_rectParent.Width(), m_rectParent.top, rect.Width(), rect.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDFormulaDefaultDlg::InitReportHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_OBJECT,          _T("Object"),        25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetTreeColumn(TRUE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_FUNTION_TYPE,    _T("Function_Type"), 15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_ALPHA,           _T("Alpha"),         15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_BETA,            _T("Beta"),          15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_THETA,           _T("Theta"),         15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_VAR_RIJ,         _T("Variable(Rij)"), 15));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	m_wndReportCtrl.Populate();
}


void KDFormulaDefaultDlg::ResizeComponent()
{
	SetResize(IDC_COMBO1,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_REPORT,       			      SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR3,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KDFormulaDefaultDlg::UpdateReportData(std::map<int, std::vector<TImpedanceFunction>> &a_mapImpedanceFunction)
{
	m_wndReportCtrl.ResetContent();

	try
	{
		CXTPReportRecord*     pParRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = a_mapImpedanceFunction.begin();
		AutoType itEnd = a_mapImpedanceFunction.end();

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
			std::vector<TImpedanceFunction> &vecImpedanceFunction = iter->second;
			for (size_t k= 0; k< vecImpedanceFunction.size(); k++)
			{
				CXTPReportRecord* pSeParRecord = pSeParRecords->Add(new CXTPReportRecord());

				CString strIndex(_T(""));
				strIndex.Format(_T("#%d"), (k+1));
				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(strIndex));
				pItem->SetAlignment(DT_LEFT);

				TImpedanceFunction &oTImpedanceFunction = vecImpedanceFunction[k];
				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(oTImpedanceFunction.strFunctionName));
				pItem->SetAlignment(DT_LEFT);

				CString strAlpha(_T(""));
				CString strBeta(_T(""));
				CString strTheta(_T(""));

				if(1 == oTImpedanceFunction.nFunctionType) 
				{
					strBeta.Format(_T("%f"), oTImpedanceFunction.dBeta);
				}
				else if (2 == oTImpedanceFunction.nFunctionType)
				{
					strAlpha.Format(_T("%f"), oTImpedanceFunction.dAlpha);
					strBeta.Format (_T("%f"), oTImpedanceFunction.dBeta);
				}
				else if(3 == oTImpedanceFunction.nFunctionType)
				{
					strAlpha.Format(_T("%f"), oTImpedanceFunction.dAlpha);
					strBeta.Format (_T("%f"), oTImpedanceFunction.dBeta);
					strTheta.Format(_T("%f"), oTImpedanceFunction.dTheta);
				}
				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(strAlpha));
				pItem->SetAlignment(DT_RIGHT);

				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(strBeta));
				pItem->SetAlignment(DT_RIGHT);

				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(strTheta));
				pItem->SetAlignment(DT_RIGHT);

				pItem = pSeParRecord->AddItem(new CXTPReportRecordItemText(oTImpedanceFunction.strVarRij));
				pItem->SetAlignment(DT_LEFT);
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


void KDFormulaDefaultDlg::OnCbnSelchangeComboGroup()
{
	UpdateData(TRUE);

	LoadFormula();
}


void KDFormulaDefaultDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}
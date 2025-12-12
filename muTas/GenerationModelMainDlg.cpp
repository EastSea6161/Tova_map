// KGenerationModelMainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KUIDefGeneration.h"
#include "GenerationModelMainDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationModelInfo.h"
#include "PurposeManager.h"
#include "Purpose.h"

#include "GenerationSelectModelDlg.h"
#include "GenerationInputTreeNavigator.h"

#include "GenerationZoneGroupPage.h"
#include "GenerationUnitPage.h"
#include "GenerationRegressionPage.h"
#include "GenerationClassificationPage.h"

using namespace UIGeneration;

// KGenerationModelMainDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationModelMainDlg, KResizeDialogEx)

KGenerationModelMainDlg::KGenerationModelMainDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KGenerationModelMainDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	m_pPurposeMgr = m_pTarget->PurposeManager();
}

KGenerationModelMainDlg::~KGenerationModelMainDlg()
{

}

void KGenerationModelMainDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL, m_btnModel);
	DDX_Control(pDX, IDC_INPUT, m_btnInput);
	DDX_Control(pDX, IDC_OUTPUTDESIGN, m_btnOutputDesign);
	DDX_Control(pDX, IDC_OUTPUTTABLE, m_btnViewTable);
	DDX_Control(pDX, IDC_RUN, m_btnRun);
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);    
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_BACK, m_uiBackImage);
}


BEGIN_MESSAGE_MAP(KGenerationModelMainDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_MODEL, &KGenerationModelMainDlg::OnBnClickedModel)
	ON_BN_CLICKED(IDC_INPUT, &KGenerationModelMainDlg::OnBnClickedInput)
END_MESSAGE_MAP()


// KGenerationModelMainDlg 메시지 처리기입니다.


BOOL KGenerationModelMainDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		((CButton*)GetDlgItem(IDC_RUN))->EnableWindow(FALSE);

		//KBulkDBase::BulkSedPopSelect(m_pTarget->GetDBaseConnection(), m_vecSEDPopRecords);

		UINT nID_GENERATION_INPUT  = 90903;
		XTPImageManager()->SetIcons(IDB_GENERATION_INPUT, &nID_GENERATION_INPUT, 1, 0); 

		UINT nID_GENERATION_MODEL  = 90905;
		XTPImageManager()->SetIcons(IDB_GENERATION_MODEL, &nID_GENERATION_MODEL, 1, 0); 

		UINT nID_GENERATION_OUTPUT = 90907;
		XTPImageManager()->SetIcons(IDB_GENERATION_OUTPUT, &nID_GENERATION_OUTPUT, 1, 0); 

		CRect rcClient;
		GetClientRect(&rcClient);
		rcClient.DeflateRect(4, 5);

		m_uiBackImage.MoveWindow(&rcClient);
		m_uiBackImage.SetMarkupText(UIXAML_G_BACKIMAGE);

		m_uiArrow1.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);      
		m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

		InitButtonControl();
		InitGenerationInfo();

		m_btnViewTable.EnableWindow(FALSE);
		m_btnRun.EnableWindow(FALSE);    

		SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, (XTPControlTheme)1); // 1, 
		KRedrawWindow();

		TxLogDebugEndMsg();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KGenerationModelMainDlg::InitButtonControl()
{
	try
	{
		m_btnModel.EnableMarkup(TRUE);
		m_btnModel.SetWindowText(UIXAML_G_MODEL_DEFAULT);

		m_btnInput.EnableWindow(FALSE);
		m_btnInput.EnableMarkup(TRUE);
		m_btnInput.SetWindowText(UIXAML_G_INPUT_FALSE);

		m_btnOutputDesign.EnableWindow(FALSE);
		m_btnOutputDesign.EnableMarkup(TRUE);
		m_btnOutputDesign.SetWindowText(UIXAML_G_OUTPUT_FALSE);	
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


void KGenerationModelMainDlg::KRedrawWindow()
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}


void KGenerationModelMainDlg::InitGenerationInfo()
{
	try
	{
		m_mapGenerationInfo.clear();

		int nCount = m_pTarget->PurposeManager()->GetPurposeCount();
		for (int i =0 ; i < nCount; i++)
		{
			KGenerationModelInfo oInfo;
			KPAPurpose* pPurpose = m_pTarget->PurposeManager()->GetPurpose(i);
			oInfo.Purpose(pPurpose);
			m_mapGenerationInfo.insert(std::make_pair(i+1, oInfo));
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

void KGenerationModelMainDlg::OnBnClickedModel()
{
	try
	{
		KGenerationSelectModelDlg oDlg(m_pTarget, m_mapGenerationInfo);
		if (oDlg.DoModal() == IDOK)
		{
			m_btnModel.SetWindowText(UIXAML_G_MODEL_SELECTED);
			m_btnInput.SetWindowText(UIXAML_G_INPUT_DEFAULT);
			m_btnInput.EnableWindow(TRUE);  
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


void KGenerationModelMainDlg::OnBnClickedInput()
{
	try
	{
		CXTPPropertySheet     oPropertySheet(_T("Input Design"));
		KGenerationInputTreeNavigator* pTreeNav = new KGenerationInputTreeNavigator;
		pTreeNav->SetGenerationInfo(m_mapGenerationInfo);

		std::vector<CXTPPropertyPage*> vecPageArray;

		AutoType iter = m_mapGenerationInfo.begin();
		AutoType end  = m_mapGenerationInfo.end();
		while(iter != end)
		{
			KGenerationModelInfo oInfo = iter->second;
			if (oInfo.UseGeneration() == false)
			{
				++iter;
				continue;
			}

			if (oInfo.UseZoneGroup())
			{
				KGenerationZoneGroupPage* pPage = new KGenerationZoneGroupPage(m_pTarget, oInfo);
				oPropertySheet.AddPage(pPage);
				vecPageArray.push_back(pPage);
			}
			else if (oInfo.GenerationModel() == KEMGenerationUnit)
			{
				KGenerationUnitPage* pPage = new KGenerationUnitPage(m_pTarget, oInfo);
				oPropertySheet.AddPage(pPage);
				vecPageArray.push_back(pPage);
			}
			else if (oInfo.GenerationModel() == KEMGenerationRegression)
			{
				KGenerationRegressionPage* pPage = new KGenerationRegressionPage(m_pTarget, oInfo);
				oPropertySheet.AddPage(pPage);
				vecPageArray.push_back(pPage);
			}
			else if (oInfo.GenerationModel() == KEMGenerationCrossClassification)
			{
				KGenerationClassificationPage* pPage = new KGenerationClassificationPage(m_pTarget, oInfo);
				oPropertySheet.AddPage(pPage);
				vecPageArray.push_back(pPage);
			}

			++iter;
		}

		oPropertySheet.SetNavigator(pTreeNav);
		oPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
		oPropertySheet.RecalcLayout();
		oPropertySheet.SetResizable(TRUE);
		oPropertySheet.SetPageSize(CSize(254, 184));

		oPropertySheet.DoModal();

	}
	catch(...)
	{
		TxLogDebugException();
	}
}

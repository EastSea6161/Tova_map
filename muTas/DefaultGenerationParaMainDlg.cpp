// GenerationParaMainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultGenerationParaMainDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "PurposeInfoCollection.h"
#include "Purpose.h"

#include "XmlManager.h"
#include "ImChampFrameWindow.h"
#include "Project.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseGeneration.h"

#define TAB_LEFT_MARGIN    2/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  5
#define TAB_HEIGHT_MARGIN 30

// GenerationParaMainDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultGenerationParaMainDlg, KResizeDialogEx)

KDefaultGenerationParaMainDlg::KDefaultGenerationParaMainDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultGenerationParaMainDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KDefaultGenerationParaMainDlg::~KDefaultGenerationParaMainDlg()
{
	CleareDefaultData();
}

void KDefaultGenerationParaMainDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(KDefaultGenerationParaMainDlg, KResizeDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KDefaultGenerationParaMainDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &KDefaultGenerationParaMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &KDefaultGenerationParaMainDlg::OnBnClickedButtonInitPro)
	ON_BN_CLICKED(IDC_BUTTON2, &KDefaultGenerationParaMainDlg::OnBnClickedButtonInitAtt)
END_MESSAGE_MAP()


// GenerationParaMainDlg 메시지 처리기입니다.


BOOL KDefaultGenerationParaMainDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	InitDefaultValue();
	CreateChildPage();
	DisplayTabControl(m_tabCtrl.GetCurSel());
	ResizeComponet();

	if (KDBaseGeneration::IsPopTableNotEmpty(m_pTarget) == false)
	{
		//AfxMessageBox(_T("카테고리분석법을 수행하려면 성별/연령별 인구 테이블에 데이터가 필요합니다."));
		//GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("※ Cross-Classification 모델을 사용하기 위해서 Pop Table에 데이터가 필요합니다"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("※ Cross-Classification 모델을 사용하기 위해서 Pop Table에 데이터가 필요합니다."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T("※ To use the Cross-Classification model, you need to import data into Pop Table first."));
		}
	}
	else
	{
		SetDlgItemText(IDC_STATIC_INFO, _T(""));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDefaultGenerationParaMainDlg::OnSize(UINT nType, int cx, int cy)
{
	KResizeDialogEx::OnSize(nType, cx, cy);
	SubPageRedrawWindow();
}

void KDefaultGenerationParaMainDlg::SubPageRedrawWindow()
{
	try
	{
		if (nullptr != m_spCurrentShowClientDlg)
		{        
			CRect oTabRect;
			m_tabCtrl.GetClientRect(&oTabRect);
			m_spCurrentShowClientDlg->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDefaultGenerationParaMainDlg::InitDefaultValue()
{
	try
	{
		CleareDefaultData();
		KDBaseGenerationParaModel::InitDefaultGenerationData(m_pTarget, m_mapGenerationInfo);
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

void KDefaultGenerationParaMainDlg::CreateChildPage()
{
	try
	{
		int nTabIndex(0);
		KPurposeInfoCollection oInfoCollection(m_pTarget);
		AutoType iter = oInfoCollection.begin();
		AutoType end  = oInfoCollection.end();
		while(iter != end)
		{
			CRect oTabRect;
			KPurpose* pPurpsoe = *iter;
			m_tabCtrl.GetClientRect(&oTabRect);
			
			KIntKey oPIntKey(pPurpsoe->PurposeID(), 1);
			KIntKey oAIntKey(pPurpsoe->PurposeID(), 2);

			AutoType iterP = m_mapGenerationInfo.find(oPIntKey);
			AutoType iterA = m_mapGenerationInfo.find(oAIntKey);
			AutoType end   = m_mapGenerationInfo.end();
			if (iterP != end && iterA != end)
			{
				KDefaultGenerationParaSubPagePtr spSubPage = KDefaultGenerationParaSubPagePtr(new KDefaultGenerationParaSubPage(m_pTarget, nTabIndex, pPurpsoe->PurposeID(),iterP->second, iterA->second));
				spSubPage->Create(IDD_8515_DefaultTripGenerationParaSubPage, &m_tabCtrl);
				spSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);

				m_tabCtrl.InsertItem(nTabIndex, pPurpsoe->PurposeName());
				m_mapSubPage.insert(std::make_pair(nTabIndex, spSubPage));
			}

			nTabIndex++;
			++iter;
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

void KDefaultGenerationParaMainDlg::CleareDefaultData()
{
	try
	{
		AutoType iter = m_mapGenerationInfo.begin();
		AutoType end  = m_mapGenerationInfo.end();
		while(iter != end)
		{
			std::map<KIntKey, KGenerationInfo*> mapGeneration = iter->second;

			AutoType iterSub = mapGeneration.begin();
			AutoType endSub	 = mapGeneration.end();
			while(iterSub != endSub)
			{
				KGenerationInfo* pInfo = iterSub->second;
				QBicDelete(pInfo);
				iterSub++;
			}
			mapGeneration.clear();
			++iter;
		}
		m_mapGenerationInfo.clear();
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

void KDefaultGenerationParaMainDlg::OnTcnSelchangeTab1( NMHDR *pNMHDR, LRESULT *pResult )
{
	DisplayTabControl(m_tabCtrl.GetCurSel());
	*pResult = 0;
}

void KDefaultGenerationParaMainDlg::DisplayTabControl( int a_nTabIndex )
{
	try
	{
		if (nullptr != m_spCurrentShowClientDlg)
		{
			m_spCurrentShowClientDlg->ShowWindow(SW_HIDE);
		}

		AutoType iter = m_mapSubPage.find(a_nTabIndex);
		if (iter != m_mapSubPage.end())
		{
			m_spCurrentShowClientDlg = iter->second;
			m_spCurrentShowClientDlg->ShowWindow(SW_SHOW);
			SubPageRedrawWindow();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefaultGenerationParaMainDlg::OnBnClickedOk()
{
	try
	{
		KXmlManager oXmlManager;
		TiXmlElement* pRootElement = nullptr;
		TiXmlElement* pPurposeElement = nullptr;

		KProject* pProject = ImChampFrameWindow::GetProject();
		CString strDir = pProject->GetLocation();
		strDir.AppendFormat(_T("\\%s\\"),  KDefaultParaFolder::FOLDER_PARAMETER);

		CFileFind odirFind;
		if (odirFind.FindFile(strDir) == 0)
		{
			CreateDirectory(strDir, NULL);
		}

		if (oXmlManager.LoadTargetXml(strDir, KDefaultParaFile::FILE_GENERATION) == false)
		{
			oXmlManager.CreateDeclaration(_T("1.0"),  _T("euc-kr"), _T(""));
		}

		pRootElement = oXmlManager.CheckRootElement(_T("Generation-Default"));
		if (pRootElement == nullptr)
		{
			AfxMessageBox(_T("Root Error"));
			return;
		}

		pRootElement->Clear();

		pPurposeElement = oXmlManager.CreateChildElement(_T("PurposeGroup"), pRootElement);
		if (nullptr == pPurposeElement)
		{
			AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		}

		AutoType iter = m_mapSubPage.begin();
		AutoType end =  m_mapSubPage.end();
		while(iter != end)
		{
			KDefaultGenerationParaSubPagePtr spSubPage = iter->second;
			spSubPage->WriteDefaultData(oXmlManager, pPurposeElement);
			++iter;
		}

		oXmlManager.SaveTargetXml(strDir, KDefaultParaFile::FILE_GENERATION);

// 		if (oXmlMansger.SaveTargetXml(strDir, KDefaultParaFile::FILE_GENERATION))
// 		{
// 			AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
// 			return;
// 		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	KResizeDialogEx::OnOK();
}

void KDefaultGenerationParaMainDlg::ResizeComponet()
{
	SetResize(IDC_STATIC_SETTING,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
	SetResize(IDC_TAB1,					SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR11,			SZ_BOTTOM_LEFT,     SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_BUTTON1,				SZ_BOTTOM_LEFT,     SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,				SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);

	SetResize(IDOK,						SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
}


void KDefaultGenerationParaMainDlg::OnBnClickedButtonInitPro()
{
	try
	{
		m_spCurrentShowClientDlg->InitProductioinPage();
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


void KDefaultGenerationParaMainDlg::OnBnClickedButtonInitAtt()
{
	try
	{
		m_spCurrentShowClientDlg->InitAttationPage();
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

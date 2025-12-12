// DImpedanceFunctionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "DImpedanceFunctionDlg.h"
#include "afxdialogex.h"
#include "DistributionInfo.h"
#include "DistributionFunction1.h"
#include "DistributionFunction2.h"
#include "DistributionFunction3.h"
#include "XmlManager.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"

#include "Target.h"
#include "KmzSystem.h"
#include "ImTasDBase.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"
#include "DefineNamesModels.h"


// KDImpedanceFunctionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDImpedanceFunctionDlg, KDialogEx)

KDImpedanceFunctionDlg::KDImpedanceFunctionDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDImpedanceFunctionDlg::IDD, pParent)
{

}

KDImpedanceFunctionDlg::~KDImpedanceFunctionDlg()
{
	clearElementList();
}

void KDImpedanceFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstDistInfo);
	DDX_Control(pDX, IDC_COMBO1, m_cmbFunction);
	DDX_Control(pDX, IDC_COMBO2, m_cmbVariable);
	DDX_Control(pDX, IDC_EDIT1, m_edtParameter1);
	DDX_Control(pDX, IDC_EDIT2, m_edtParameter2);
	DDX_Control(pDX, IDC_EDIT3, m_edtParameter3);
}


BEGIN_MESSAGE_MAP(KDImpedanceFunctionDlg, KDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &KDImpedanceFunctionDlg::OnLvnItemchangedList1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KDImpedanceFunctionDlg::OnCbnSelchangeCombo1)
    ON_LBN_SELCHANGE(IDC_LIST1, &KDImpedanceFunctionDlg::OnLbnSelchangeList1)
    ON_LBN_SELCANCEL(IDC_LIST1, &KDImpedanceFunctionDlg::OnLbnSelcancelList1)
// 	ON_BN_CLICKED(ID_DEFAULT, &KDImpedanceFunctionDlg::OnBnClickedDefault)
// 	ON_BN_CLICKED(ID_LOAD, &KDImpedanceFunctionDlg::OnBnClickedLoad)
// 	ON_BN_CLICKED(ID_SAVE, &KDImpedanceFunctionDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_FUNCTION, &KDImpedanceFunctionDlg::OnBnClickedBtnDefaultFunction)
END_MESSAGE_MAP()


void KDImpedanceFunctionDlg::SetTarget( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
	ASSERT(NULL != a_pTarget);
}


void KDImpedanceFunctionDlg::AddParaODColumns(std::list<KSTParaColumn>& paraODColumnNames )
{
    m_paraODColumnNames.clear();
    
    std::list<KSTParaColumn>::iterator iter = paraODColumnNames.begin();
    std::list<KSTParaColumn>::iterator end  = paraODColumnNames.end();

    while (iter != end)
    {
        m_paraODColumnNames.push_back(*iter);
        ++iter;
    }
}


void KDImpedanceFunctionDlg::AddDistributionInfo(KDistributionInfo* pInfo)
{
	std::list<KSTImpedanceElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KSTImpedanceElement* pElement = *itElement;
		if(pElement->pInfo == pInfo)
		{
			return;
		}
	}

	KSTImpedanceElement* pElement = new KSTImpedanceElement;
	pElement->pInfo = pInfo;
	pElement->pFunction = createAndCopyFunction(pInfo->ImpedanceFunction());
	
	m_ElementList.push_back(pElement);
}


void KDImpedanceFunctionDlg::SetDistributionInfo(std::list<KDistributionInfo*>& infoList)
{
	clearElementList();
	std::list<KDistributionInfo*>::iterator itInfo, itEnd = infoList.end();
	for(itInfo = infoList.begin(); itInfo != itEnd; ++itInfo)
	{
		KDistributionInfo* pInfo = *itInfo;
		KSTImpedanceElement* pElement = new KSTImpedanceElement;
		pElement->pInfo = pInfo;
		pElement->pFunction = createAndCopyFunction(pInfo->ImpedanceFunction());

		m_ElementList.push_back(pElement);
	}
}


void KDImpedanceFunctionDlg::updateListControl(void)
{
	std::list<KSTImpedanceElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KSTImpedanceElement* pElement = *itElement;
		int nIndex = m_lstDistInfo.AddString(pElement->pInfo->PurposeName());
		m_lstDistInfo.SetItemData(nIndex, (DWORD_PTR)pElement);
	}

    if ( m_lstDistInfo.GetCount() > 0)
    {
        m_lstDistInfo.SetCurSel(0);
    }
}


void KDImpedanceFunctionDlg::updateVariableControl()
{
    m_cmbVariable.Clear();

    std::list<KSTParaColumn>::iterator iter = m_paraODColumnNames.begin();
    std::list<KSTParaColumn>::iterator end  = m_paraODColumnNames.end();

    while ( iter != end )
    {
        m_cmbVariable.AddString(iter->ColumnCaption);
        ++iter;
    }
}


void KDImpedanceFunctionDlg::updateGetFunction()
{
    if (m_nCurSelected < 0)
    {
        return;
    }

    int nIndex = m_nCurSelected;
    CString strParam = _T("");

    KSTImpedanceElement* pElement = (KSTImpedanceElement*)(m_lstDistInfo.GetItemData(nIndex));
    KDistributionFunctionBase* pFunctionBase = pElement->pFunction;

    nIndex = m_cmbVariable.GetCurSel();
    pFunctionBase->RijColumnName(ParaODColumnName(nIndex));

    if(NULL != pFunctionBase)
    {
        KEMDistributionFunction emFunction = pFunctionBase->FunctionType();
        if(KEMDistributionFunction1 == emFunction)
        {
            KDistributionFunction1* pFunction1 = (KDistributionFunction1*)pFunctionBase;
            m_edtParameter2.GetWindowText(strParam);
            pFunction1->Parameter2(_ttof(strParam));
        }
        else if(KEMDistributionFunction2 == emFunction)
        {
            KDistributionFunction2* pFunction2 = (KDistributionFunction2*)pFunctionBase;         

            m_edtParameter1.GetWindowText(strParam);
            pFunction2->Parameter1(_ttof(strParam));

            m_edtParameter2.GetWindowText(strParam);
            pFunction2->Parameter2(_ttof(strParam));
        }
        else if(KEMDistributionFunction3 == emFunction)
        {
            KDistributionFunction3* pFunction3 = (KDistributionFunction3*)pFunctionBase;
            m_edtParameter1.GetWindowText(strParam);
            pFunction3->Parameter1(_ttof(strParam));

            m_edtParameter2.GetWindowText(strParam);
            pFunction3->Parameter2(_ttof(strParam));

            m_edtParameter3.GetWindowText(strParam);
            pFunction3->Parameter3(_ttof(strParam));
        }
    }
}


void KDImpedanceFunctionDlg::updateSetFunction(void)
{
    updateGetFunction();

	int nIndex = m_lstDistInfo.GetCurSel();
	if(LB_ERR == nIndex)
	{
		return;
	}

    m_nCurSelected = nIndex;

	KSTImpedanceElement* pElement = (KSTImpedanceElement*)(m_lstDistInfo.GetItemData(nIndex));
	ASSERT(NULL != pElement);

	CString strParam = _T("");
	KDistributionFunctionBase* pFunctionBase = pElement->pFunction;
	if(NULL != pFunctionBase)
	{
        int nParaODIndex = ParaODColumnIndex (pFunctionBase->RijColumnName());

        if (nParaODIndex >= 0)
        {
            m_cmbVariable.SetCurSel(nParaODIndex);
        }
		else 
		{
			m_cmbVariable.SetCurSel(nParaODIndex);
		}

		KEMDistributionFunction emFunction = pFunctionBase->FunctionType();
		if(KEMDistributionFunction1 == emFunction)
		{
			KDistributionFunction1* pFunction1 = (KDistributionFunction1*)pFunctionBase;

			m_cmbFunction.SetCurSel(0);

			strParam.Format(_T("%f"), pFunction1->Parameter2());
			
            m_edtParameter1.SetWindowText(_T(""));
            m_edtParameter1.EnableWindow(FALSE);

			m_edtParameter2.SetWindowText(strParam);
			m_edtParameter2.EnableWindow(TRUE);

			m_edtParameter3.SetWindowText(_T(""));
			m_edtParameter3.EnableWindow(FALSE);
		}
		else if(KEMDistributionFunction2 == emFunction)
		{
			KDistributionFunction2* pFunction2 = (KDistributionFunction2*)pFunctionBase;

			m_cmbFunction.SetCurSel(1);

			strParam.Format(_T("%f"), pFunction2->Parameter1());
			m_edtParameter1.SetWindowText(strParam);
			m_edtParameter1.EnableWindow(TRUE);

			strParam.Format(_T("%f"), pFunction2->Parameter2());
			m_edtParameter2.SetWindowText(strParam);
			m_edtParameter2.EnableWindow(TRUE);

			m_edtParameter3.SetWindowText(_T(""));
			m_edtParameter3.EnableWindow(FALSE);
		}
		else if(KEMDistributionFunction3 == emFunction)
		{
			KDistributionFunction3* pFunction3 = (KDistributionFunction3*)pFunctionBase;

			m_cmbFunction.SetCurSel(2);

			strParam.Format(_T("%f"), pFunction3->Parameter1());
			m_edtParameter1.SetWindowText(strParam);
			m_edtParameter1.EnableWindow(TRUE);

			strParam.Format(_T("%f"), pFunction3->Parameter2());
			m_edtParameter2.SetWindowText(strParam);
			m_edtParameter2.EnableWindow(TRUE);

			strParam.Format(_T("%f"), pFunction3->Parameter3());
			m_edtParameter3.SetWindowText(strParam);
			m_edtParameter3.EnableWindow(TRUE);
		}
	}
}


void KDImpedanceFunctionDlg::clearElementList(void)
{
	try
	{
		std::list<KSTImpedanceElement*>::iterator itElement, itEnd = m_ElementList.end();
		for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
		{
			KSTImpedanceElement* pElement = *itElement;
			if(NULL != pElement)
			{
				if(NULL != pElement->pFunction)
				{
					KDistributionFunctionBase* pFunction = pElement->pFunction;
					QBicDelete(pFunction);
				}
			}
			QBicDelete(pElement);
		}
		m_ElementList.clear();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


KDistributionFunctionBase* KDImpedanceFunctionDlg::
	createAndCopyFunction(KDistributionFunctionBase* pFunction)
{
	KDistributionFunctionBase* pRet = NULL;
	if(NULL == pFunction)
	{
		KDistributionFunction1* pFunction1 = new KDistributionFunction1;
		pFunction1->Parameter2(0.0);
		pRet = pFunction1;
	}
	else
	{
		KEMDistributionFunction emFunctionType = pFunction->FunctionType();
		if(KEMDistributionFunction1 == emFunctionType)
		{
			KDistributionFunction1* pFunction1 = new KDistributionFunction1;
			*pFunction1 = *((KDistributionFunction1*)pFunction);
			pRet = pFunction1;
		}
		else if(KEMDistributionFunction2 == emFunctionType)
		{
			KDistributionFunction2* pFunction2 = new KDistributionFunction2;
			*pFunction2 = *((KDistributionFunction2*)pFunction);
			pRet = pFunction2;
		}
		else if(KEMDistributionFunction3 == emFunctionType)
		{
			KDistributionFunction3* pFunction3 = new KDistributionFunction3;
			*pFunction3 = *((KDistributionFunction3*)pFunction);
			pRet = pFunction3;
		}
	}
	return pRet;
}

int KDImpedanceFunctionDlg::ParaODColumnIndex(CString strColumnName)
{
    std::list<KSTParaColumn>::iterator iter = m_paraODColumnNames.begin();
    std::list<KSTParaColumn>::iterator end  = m_paraODColumnNames.end();

    int nIndex = 0;

    while (iter != end)
    {
        if (strColumnName.CompareNoCase(iter->ColumnName) == 0)
        {
            return nIndex;
        }

        nIndex++;
        ++iter;
    }

    return -1;
}


int KDImpedanceFunctionDlg::ParaODColumnIndexToCaption( CString strColumnCaption, CString& ar_strColumnName )
{
	std::list<KSTParaColumn>::iterator iter = m_paraODColumnNames.begin();
	std::list<KSTParaColumn>::iterator end  = m_paraODColumnNames.end();

	int nIndex = 0;

	while (iter != end)
	{
		if (strColumnCaption.CompareNoCase(iter->ColumnCaption) ==  0)
		{
			ar_strColumnName = iter->ColumnName; 
			return nIndex;
		}

		nIndex++;
		++iter;
	}

	return -1;
}



CString KDImpedanceFunctionDlg::ParaODColumnName(int index)
{
    std::list<KSTParaColumn>::iterator iter = m_paraODColumnNames.begin();
    std::list<KSTParaColumn>::iterator end  = m_paraODColumnNames.end();

    int nLoop = 0;
    while (iter != end )
    {
        if (nLoop == index)
        {
            return iter->ColumnName;
        }
        nLoop++;
        ++iter;
    }

    return _T("");
}

// KDImpedanceFunctionDlg 메시지 처리기입니다.
BOOL KDImpedanceFunctionDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    m_nCurSelected = -1;
	
	updateListControl();
	updateVariableControl();
	updateSetFunction();

// 	if(KmzSystem::IsPassengerSystem())
// 	{
// 		GetDlgItem(IDC_BTN_DEFAULT_FUNCTION)->ShowWindow(SW_HIDE);
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDImpedanceFunctionDlg::OnOK()
{
    updateGetFunction();

	int nIdx = 0;
    std::list<KSTImpedanceElement*>::iterator iter = m_ElementList.begin();
    std::list<KSTImpedanceElement*>::iterator end  = m_ElementList.end();
    while ( iter != end )
    {
        KSTImpedanceElement* pElement          = *iter;
        KDistributionInfo*   pInfo             = pElement->pInfo;
        KDistributionFunctionBase* pFunction   = pElement->pFunction;
        KEMDistributionFunction emFunctionType = pFunction->FunctionType();

        //KDistributionFunctionBase*pInfo->ImpedanceFunction()
        
        KDistributionFunctionBase* pFunctionNew = pInfo->SetImpedanceFunctionType(emFunctionType);
                
        if(KEMDistributionFunction1 == emFunctionType)
        {
            KDistributionFunction1* pFunction1    = (KDistributionFunction1*)pFunction;
            KDistributionFunction1* pFunctionNew1 = (KDistributionFunction1*)pFunctionNew;

            *pFunctionNew1 = *pFunction1;
           
        }
        else if(KEMDistributionFunction2 == emFunctionType)
        {
            KDistributionFunction2* pFunction2    = (KDistributionFunction2*)pFunction;
            KDistributionFunction2* pFunctionNew2 = (KDistributionFunction2*)pFunctionNew;

            *pFunctionNew2 = *pFunction2;
        }
        else if(KEMDistributionFunction3 == emFunctionType)
        {
            KDistributionFunction3* pFunction3    = (KDistributionFunction3*)pFunction;
            KDistributionFunction3* pFunctionNew3 = (KDistributionFunction3*)pFunctionNew;

            *pFunctionNew3 = *pFunction3;
        }

		if(false == IsEmptyValue(pFunctionNew))
		{
			m_lstDistInfo.SetCurSel(nIdx);
			updateSetFunction();

			return;
		}
		
		++nIdx;
	    ++iter;
    }
	
    clearElementList();

	KDialogEx::OnOK();
}


bool KDImpedanceFunctionDlg::IsEmptyValue(KDistributionFunctionBase* pFunctionNew)
{
	if(pFunctionNew->RijColumnName() == _T(""))
	{
		AfxMessageBox(_T("변수(Rij)값을 선택해 주세요."));
		m_cmbVariable.SetFocus();

		return false;
	}

	return true;
}


void KDImpedanceFunctionDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if(NULL == pNMLV)
	{
		return ;
	}
	updateSetFunction();

	*pResult = 0;
}


void KDImpedanceFunctionDlg::OnCbnSelchangeCombo1()
{
	int nSelIndex = m_cmbFunction.GetCurSel();
	if(CB_ERR == nSelIndex)
	{
		return;
	}

	int nListIndex = m_lstDistInfo.GetCurSel();
	if(LB_ERR == nListIndex)
	{
		return;
	}

	KSTImpedanceElement* pElement = (KSTImpedanceElement*)(m_lstDistInfo.GetItemData(nListIndex));
	if(0 == nSelIndex)
	{
		if(pElement->pFunction->FunctionType() != KEMDistributionFunction1)
		{
			QBicDelete(pElement->pFunction);
			pElement->pFunction = new KDistributionFunction1;
			m_edtParameter1.SetWindowText(_T(""));
			m_edtParameter2.SetWindowText(_T(""));
			m_edtParameter3.SetWindowText(_T(""));
		}
		m_edtParameter1.EnableWindow(FALSE);
        m_edtParameter2.EnableWindow(TRUE);
		m_edtParameter3.EnableWindow(FALSE);
	}
	else if(1 == nSelIndex)
	{
		if(pElement->pFunction->FunctionType() != KEMDistributionFunction2)
		{
			QBicDelete(pElement->pFunction);
			pElement->pFunction = new KDistributionFunction2;
			m_edtParameter1.SetWindowText(_T(""));
			m_edtParameter2.SetWindowText(_T(""));
			m_edtParameter3.SetWindowText(_T(""));
		}
        m_edtParameter1.EnableWindow(TRUE);
		m_edtParameter2.EnableWindow(TRUE);
		m_edtParameter3.EnableWindow(FALSE);
	}
	else if(2 == nSelIndex)
	{
		if(pElement->pFunction->FunctionType() != KEMDistributionFunction3)
		{
			QBicDelete(pElement->pFunction);
			pElement->pFunction = new KDistributionFunction3;
			m_edtParameter1.SetWindowText(_T(""));
			m_edtParameter2.SetWindowText(_T(""));
			m_edtParameter3.SetWindowText(_T(""));
		}
        m_edtParameter1.EnableWindow(TRUE);
		m_edtParameter2.EnableWindow(TRUE);
		m_edtParameter3.EnableWindow(TRUE);
	}
	
}


void KDImpedanceFunctionDlg::OnLbnSelchangeList1()
{
    updateSetFunction();
}


void KDImpedanceFunctionDlg::OnLbnSelcancelList1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //AfxMessageBox(_T("ddd"));
}


void KDImpedanceFunctionDlg::OnBnClickedBtnDefaultFunction()
{
	if (m_nCurSelected < 0)
	{
		return;
	}

	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		if (nullptr == spDBaseConnection)
		{		
			AfxMessageBox(_T("Project.db Not Found"));
			return;
		}

		UpdataDefFunction();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KDImpedanceFunctionDlg::UpdataDefFunction()
{
	int nIndex = m_lstDistInfo.GetCurSel();
	if(LB_ERR == nIndex)
	{
		return;
	}

	KSTImpedanceElement* pElement = (KSTImpedanceElement*)(m_lstDistInfo.GetItemData(nIndex));
	ASSERT(NULL != pElement);

	KDistributionInfo*   pInfo    = pElement->pInfo;
	int nDetailObejctID			  = pInfo->PurposeID();

	CString strParam (_T(""));
	AutoType find = m_mapDefaultParameter.find(nDetailObejctID);
	AutoType end  = m_mapDefaultParameter.end();
	if (find != end)
	{
		KDistributionParaInfo oInfo = find->second;
		if (KEMDistributionFunction1 == oInfo.TnFunctionType)
		{
			QBicDelete(pElement->pFunction);
			pElement->pFunction = new KDistributionFunction1;

			KDistributionFunction1* pFunction1 = (KDistributionFunction1*)pElement->pFunction;
			pFunction1->Parameter2(oInfo.TdParameterBeta);

			strParam.Format(_T("%f"), pFunction1->Parameter2());

			m_edtParameter1.SetWindowText(_T(""));
			m_edtParameter1.EnableWindow(FALSE);

			m_edtParameter2.SetWindowText(strParam);
			m_edtParameter2.EnableWindow(TRUE);

			m_edtParameter3.SetWindowText(_T(""));
			m_edtParameter3.EnableWindow(FALSE);

		}
		else if (KEMDistributionFunction2 == oInfo.TnFunctionType)
		{
			QBicDelete(pElement->pFunction);
			pElement->pFunction = new KDistributionFunction2;
			
			KDistributionFunction2* pFunction2 = (KDistributionFunction2*)pElement->pFunction;
			pFunction2->Parameter1(oInfo.TdParameterAlpa);
			pFunction2->Parameter2(oInfo.TdParameterBeta);

			m_cmbFunction.SetCurSel(1);

			strParam.Format(_T("%f"), pFunction2->Parameter1());
			m_edtParameter1.SetWindowText(strParam);
			m_edtParameter1.EnableWindow(TRUE);

			strParam.Format(_T("%f"), pFunction2->Parameter2());
			m_edtParameter2.SetWindowText(strParam);
			m_edtParameter2.EnableWindow(TRUE);

			m_edtParameter3.SetWindowText(_T(""));
			m_edtParameter3.EnableWindow(FALSE);
		}
		else if (KEMDistributionFunction3 == oInfo.TnFunctionType)
		{
			QBicDelete(pElement->pFunction);
			pElement->pFunction = new KDistributionFunction3;

			KDistributionFunction3* pFunction3 = (KDistributionFunction3*)pElement->pFunction;
			pFunction3->Parameter1(oInfo.TdParameterAlpa);
			pFunction3->Parameter2(oInfo.TdParameterBeta);
			pFunction3->Parameter3(oInfo.TdParameterTheta);

			m_cmbFunction.SetCurSel(2);

			strParam.Format(_T("%f"), pFunction3->Parameter1());
			m_edtParameter1.SetWindowText(strParam);
			m_edtParameter1.EnableWindow(TRUE);

			strParam.Format(_T("%f"), pFunction3->Parameter2());
			m_edtParameter2.SetWindowText(strParam);
			m_edtParameter2.EnableWindow(TRUE);

			strParam.Format(_T("%f"), pFunction3->Parameter3());
			m_edtParameter3.SetWindowText(strParam);
			m_edtParameter3.EnableWindow(TRUE);
		}

		CString strColumnName(_T(""));
		int nParaODIndex = ParaODColumnIndexToCaption (oInfo.TstrVariable,strColumnName);
		if (nParaODIndex >= 0)
		{
			pElement->pFunction->RijColumnName(strColumnName);
			m_cmbVariable.SetCurSel(nParaODIndex);
		}
		else 
		{
			pElement->pFunction->RijColumnName(_T(""));
			m_cmbVariable.SetCurSel(nParaODIndex);
		}
	}
}



void KDImpedanceFunctionDlg::SetDefaultparameter( std::map<int, KDistributionParaInfo> a_mapParaInfo )
{
	m_mapDefaultParameter = a_mapParaInfo;
}

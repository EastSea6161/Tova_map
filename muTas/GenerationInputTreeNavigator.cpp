#include "StdAfx.h"
#include "GenerationInputTreeNavigator.h"
#include "GenerationModelInfo.h"
#include "PAPurpose.h"

KGenerationInputTreeNavigator::KGenerationInputTreeNavigator(void)
	:	CXTPPropertyPageTreeNavigator(TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT)
{
}


KGenerationInputTreeNavigator::~KGenerationInputTreeNavigator(void)
{
}

BEGIN_MESSAGE_MAP(KGenerationInputTreeNavigator, CXTPPropertyPageTreeNavigator)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &KGenerationInputTreeNavigator::OnTvnSelchanged)
END_MESSAGE_MAP()


BOOL KGenerationInputTreeNavigator::CreateTree( void )
{
	try
	{
		std::map<CString, HTREEITEM> handleMap;

		int nPageIndex = 0;
		AutoType iter = m_mapGenerationInfo.begin();
		AutoType end  = m_mapGenerationInfo.end();
		while(iter != end)
		{
			CXTPPropertyPage* pPage = m_pSheet->GetPage(nPageIndex++);
			KGenerationModelInfo oInfo = iter->second;
			HTREEITEM hRoot = NULL;

			KPAPurpose* pPurspoe = oInfo.Purpose();

			AutoType iterHandle = handleMap.find(pPurspoe->PurposeName());
			AutoType endHandle	= handleMap.end();
			if (iterHandle == endHandle)
			{
				hRoot = InsertItem(pPurspoe->PurposeName());
				handleMap.insert(std::make_pair(pPurspoe->PurposeName(), hRoot));
			}
			else
			{
				hRoot = iterHandle->second;
			}

			HTREEITEM hItem;

			if ( KmzSystem::IsPassengerSystem() )
			{
				hItem = InsertItem(
					pPurspoe->PAType() == KEMPATypeProduction ? PRODUCTION_STRING : ATTRACTION_STRING, 
					hRoot);
			}
			else
			{
				hItem = InsertItem(
					pPurspoe->PAType() == KEMPATypeProduction ? PRODUCTION_STRING : CONSUMPTION_STRING, 
					hRoot);
			}

			pPage->m_dwData = (DWORD_PTR)hItem;
			SetItemData(hItem, (DWORD_PTR)pPage);
			SelectItem(hItem);

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
	return TRUE;
}

void KGenerationInputTreeNavigator::OnTvnSelchanged( NMHDR *pNMHDR, LRESULT *pResult )
{
	try
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

		HTREEITEM hItem = GetSelectedItem();

		HTREEITEM hParenthItem = GetParentItem(hItem);
		if ( hParenthItem == NULL)
		{
			*pResult = 0;
			return;
		}

		if(NULL != hItem)
		{
			CXTPPropertyPage* pPage = (CXTPPropertyPage*)GetItemData(hItem);
			CString strModelName(_T(""));
			strModelName.Format(_T("Input Design[ Model : %s ]"),  pPage->GetCaption());
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strModelName.Format(_T("입력설정(%s)"),  pPage->GetCaption());
			}
			CString strCaption = pPage->GetCaption();
			m_pSheet->SetTitle(strModelName);

			HTREEITEM hSubItem = GetChildItem(hItem);
			if(NULL != hSubItem)
			{
				SelectItem(hSubItem);
			}
		}

		*pResult = 0;
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

void KGenerationInputTreeNavigator::SetGenerationInfo( std::map<int, KGenerationModelInfo> a_mapGenerationInfo )
{
	try
	{
		m_mapGenerationInfo = a_mapGenerationInfo;
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

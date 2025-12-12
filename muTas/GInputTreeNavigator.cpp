/**
 * @file GInputTreeNavigator.cpp
 * @brief KGInputTreeNavigator 구현 파일
 * @author 
 * @date 2011.05.30
 * @remark
 */


#include "StdAfx.h"
#include "GInputTreeNavigator.h"
#include "GenerationInfo.h"


KGInputTreeNavigator::KGInputTreeNavigator(void)
	:	CXTPPropertyPageTreeNavigator(TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT)
{
}


KGInputTreeNavigator::~KGInputTreeNavigator(void)
{
}


BEGIN_MESSAGE_MAP(KGInputTreeNavigator, CXTPPropertyPageTreeNavigator)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &KGInputTreeNavigator::OnTvnSelchanged)
END_MESSAGE_MAP()

#include "KmzSystem.h"

BOOL KGInputTreeNavigator::CreateTree(void)
{
	std::map<CString, HTREEITEM> handleMap;
	std::map<CString, HTREEITEM>::iterator itHandle;

	int nPageIndex = 0;
	std::list<KGenerationInfo*>::iterator itInfo, itEnd = m_lstGInfoList.end();
	for(itInfo = m_lstGInfoList.begin(); itInfo != itEnd; ++itInfo)
	{
		CXTPPropertyPage* pPage = m_pSheet->GetPage(nPageIndex++);
		KGenerationInfo* pInfo = *itInfo;
		HTREEITEM hRoot = NULL;

		itHandle = handleMap.find(pInfo->GetName());
		if(handleMap.end() == itHandle)
		{
			hRoot = InsertItem(pInfo->GetName());
			handleMap.insert(std::make_pair(pInfo->GetName(), hRoot));
		}
		else
		{
			hRoot = itHandle->second;
		}

        HTREEITEM hItem;

        if ( KmzSystem::IsPassengerSystem() ) {
            if (KmzSystem::GetLanguage() == KEMKorea) {
                hItem = InsertItem( pInfo->GetPAType() == KEMPATypeProduction ? _T("유출모형") : _T("유입모형"), hRoot);
            }
            else {
                hItem = InsertItem( pInfo->GetPAType() == KEMPATypeProduction ? PRODUCTION_STRING : ATTRACTION_STRING, hRoot);
            }            
        }
        else {
            hItem = InsertItem(
                pInfo->GetPAType() == KEMPATypeProduction ? PRODUCTION_STRING : CONSUMPTION_STRING, 
                hRoot);
        }

		pPage->m_dwData = (DWORD_PTR)hItem;
		SetItemData(hItem, (DWORD_PTR)pPage);
		SelectItem(hItem);
	}

	return TRUE;
}


void KGInputTreeNavigator::OnTvnSelchanged(NMHDR *a_pNMHDR, LRESULT *a_pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(a_pNMHDR);

	HTREEITEM hItem = GetSelectedItem();
	
	HTREEITEM hParenthItem = GetParentItem(hItem);
	if ( hParenthItem == NULL)
	{
		*a_pResult = 0;
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
		if (strCaption.CompareNoCase(GENERATION_IRIO_STRING)== 0)
		{	
		}
		m_pSheet->SetTitle(strModelName);

		HTREEITEM hSubItem = GetChildItem(hItem);
		if(NULL != hSubItem)
		{
			SelectItem(hSubItem);
		}
	}

	*a_pResult = 0;
}
#pragma once
#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"

class KxtpPropertyGridUtil
{
public:
	static CXTPPropertyGridItem* AddCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KTarget* a_pTarget, KIOColumn* a_pIOColumn )
	{	
		KCodeManager* pCodeMgr   = a_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );

		std::vector<int> vecCode;
		pCodeGroup->GetCodes( vecCode );
		size_t nxCodeCount = vecCode.size();

		CXTPPropertyGridItemEnum* pGridItem = NULL;
		pGridItem = (CXTPPropertyGridItemEnum*)(a_pParentGridItem->AddChildItem(new CXTPPropertyGridItemEnum(a_pIOColumn->Caption())));

		pGridItem->SetItemData((DWORD_PTR)a_pIOColumn);
		for(size_t i=0; i<nxCodeCount; i++)
		{		
			CString strCaption = pCodeGroup->SingleCodeValue(vecCode[i], 1);
			pGridItem->GetConstraints()->AddConstraint(strCaption, vecCode[i]);		
		}

		return pGridItem;
	}

    static CXTPPropertyGridItem* AddNodeTypeItemAtNodeEdit( CXTPPropertyGridItem* a_pParentGridItem, KTarget* a_pTarget, KIOColumn* a_pIOColumn, int nOnylZone= 0)
    {	
        //존노드 제외, 첫번째 자동 선택
        KCodeManager* pCodeMgr   = a_pTarget->CodeManager();
        KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );

        std::vector<int> vecCode; {
            pCodeGroup->GetCodes( vecCode );
        }
        
        size_t nxCodeCount = vecCode.size();

        CXTPPropertyGridItemEnum* pGridItem = NULL;
        pGridItem = (CXTPPropertyGridItemEnum*)(a_pParentGridItem->AddChildItem(new CXTPPropertyGridItemEnum(a_pIOColumn->Caption())));
        pGridItem->SetItemData((DWORD_PTR)a_pIOColumn);
                        
        //★ 존만 포함한 경우
        if (nOnylZone > 0) {
            CXTPPropertyGridItemConstraint* p = pGridItem->GetConstraints()->AddConstraint(_T("0:존노드"), 0);
            pGridItem->SetEnum(0);
            return pGridItem;
        }

        CXTPPropertyGridItemConstraint* pFirst = nullptr;
        for(size_t i=0; i<nxCodeCount; i++) {		
            int nCode = vecCode[i];

            //★ 존노드 제외
            if (nCode == 0 )
                continue;

            CString strCaption = pCodeGroup->SingleCodeValue(nCode, 1);
            CXTPPropertyGridItemConstraint* p = pGridItem->GetConstraints()->AddConstraint(strCaption, nCode);

            if (nCode != 0 && pFirst == nullptr) {               
                pFirst = p;
            }            	
        }

        if (pFirst != nullptr) {
            pGridItem->SetEnum(pFirst);
        }
        else {
            //★ 강제적으로 1번을 추가한다.
            //★ 존노드외에는 노드타입이 없다.
            CString strCaption(_T("1"));
            CXTPPropertyGridItemConstraint* pFirst = pGridItem->GetConstraints()->AddConstraint(strCaption, 1);
            pGridItem->SetEnum(pFirst);
        }

        return pGridItem;
    }

    static CXTPPropertyGridItem* AddNodeTypeItemAtLinkEdit( CXTPPropertyGridItem* a_pParentGridItem, KTarget* a_pTarget, KIOColumn* a_pIOColumn )
    {	
        //존노드 제외, 첫번째 자동 선택
        KCodeManager* pCodeMgr   = a_pTarget->CodeManager();
        KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );

        std::vector<int> vecCode;
        pCodeGroup->GetCodes( vecCode );
        size_t nxCodeCount = vecCode.size();

        CXTPPropertyGridItemEnum* pGridItem = NULL;
        pGridItem = (CXTPPropertyGridItemEnum*)(a_pParentGridItem->AddChildItem(new CXTPPropertyGridItemEnum(a_pIOColumn->Caption())));

        pGridItem->SetItemData((DWORD_PTR)a_pIOColumn);

        CXTPPropertyGridItemConstraint* pFirst = nullptr;
        for(size_t i=0; i<nxCodeCount; i++)
        {		
            int nCode = vecCode[i];
            //★ 존노드 제외
            if (nCode != 0) {
                CString strCaption = pCodeGroup->SingleCodeValue(nCode, 1);
                CXTPPropertyGridItemConstraint* p = pGridItem->GetConstraints()->AddConstraint(strCaption, nCode);	

                if (pFirst == nullptr)
                    pFirst = p;
            }            	
        }

        if (pFirst != nullptr) {
            pGridItem->SetEnum(pFirst);
        }
        else {
            //★ 강제적으로 1번을 추가한다.
            //★ 존노드외에는 노드타입이 없다.
            CString strCaption(_T("1"));
            CXTPPropertyGridItemConstraint* pFirst = pGridItem->GetConstraints()->AddConstraint(strCaption, 1);
            pGridItem->SetEnum(pFirst);
        }

        return pGridItem;
    }

	static CXTPPropertyGridItem* AddMultiCodeItem( CXTPPropertyGridItem* a_pParentGridItem, KTarget* a_pTarget, KIOColumn* a_pIOColumn)
	{
		KCodeManager* pCodeMgr   = a_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( a_pIOColumn->CodeGroup() );

		std::vector<int> vecCode;
		pCodeGroup->GetCodes( vecCode );
		size_t nxCodeCount = vecCode.size();

		CXTPPropertyGridItem* pItem = nullptr;

		pItem = a_pParentGridItem->AddChildItem(new CXTPPropertyGridItem(a_pIOColumn->Caption(), _T("")));
		pItem->SetItemData((DWORD_PTR)a_pIOColumn);
		pItem->SetFlags(xtpGridItemHasExpandButton);
		int nSumFlag = MultiCodeConvertSumFlag(_T(""));
		pItem->SetID(nSumFlag);

		return pItem;

// 		CXTPPropertyGridItemOption* pGridItemOption = NULL;
// 		#pragma region 설명
// 		/*	CXTPPropertyGridItemOption으로 만든 CheckBox는 아래와 같은 규칙을 가짐.
// 			추가한 체크박스의 Index값은 ( 0, 1, 2, 3 ...)이 아닌 2의 제곱근(pow(2.0, 0), pow(2.0, 1), pw(2.0, 2)...)을 Index로 함.
// 			체크박스의 선택을 위해서는 각 체크박스에 Selected 옵션을 주는 것이 아닌 생성자에서 아래의 값을 입력 함.
// 			new CXTPPropertyGridItemOption( Caption, int(선택하려는 각 Index들의 합) );*/
// 		#pragma endregion 설명
// 				
// 		pGridItemOption = (CXTPPropertyGridItemOption*)a_pParentGridItem->AddChildItem(new CXTPPropertyGridItemOption( a_pIOColumn->Caption(), 0));		
// 		pGridItemOption->SetItemData((DWORD_PTR)a_pIOColumn);
// 
// 		AutoType iter = vecCode.begin(), end = vecCode.end();
// 		while(iter != end)
// 		{
// 			CString strConst = pCodeGroup->SingleCodeValue(*iter, 1);
// 			pGridItemOption->GetConstraints()->AddConstraint(strConst, (int)pow(2.0, *iter));
// 				
// 			++iter;
// 		}
// 		pGridItemOption->SetCheckBoxStyle(TRUE);
//		return pGridItemOption;
	}

	static int MultiCodeConvertSumFlag( CString a_strMultiCodeVal )
	{
		a_strMultiCodeVal.Remove(' ');

		if(a_strMultiCodeVal.IsEmpty())
		{
			return 0;
		}
		CString strElement(_T(""));
		int nSumAllFlag(0), nSub(0);

		while (AfxExtractSubString(strElement, a_strMultiCodeVal, nSub++, _T(',')))
		{
			if(strElement.IsEmpty() == TRUE)
			{
				continue;
			}
			nSumAllFlag += (int)pow(2.0, _ttoi(strElement));
		}
		return nSumAllFlag;
	}

	static CString SumFlagConvertMultiCode( CString a_strSumFlag )
	{
		CString strValueMultiCode(_T(""));
		int		nSumFlag	= _ttol(a_strSumFlag);
		int		nLoopCount	= 0;
		bool	bFrist		= true;

		while (true)
		{
			CString strValue(_T(""));

			if( 0 != nSumFlag % 2)
			{
				if(true == bFrist)
					bFrist = false;
				else
					strValueMultiCode.Append(_T(","));

				strValue.Format(_T("%d"), nLoopCount);
				strValueMultiCode.Append(strValue);
			}
			nSumFlag = nSumFlag / 2;

			if ( 0 == nSumFlag )
				break;
			++nLoopCount;
		}

		return strValueMultiCode;
	}
};
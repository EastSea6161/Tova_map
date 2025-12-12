/**
 * @file AFunctionUser.cpp
 * @brief KAFunctionUser 구현 파일
 * @author 
 * @date 2011.06.20
 * @remark
 */


#include "StdAfx.h"
#include "AFunctionUser.h"
#include "AFunction.h"



KAFunctionUser::KAFunctionUser(void)
	:	KAFunctionBase(KEMAssignmentFunctionUser)
{
}


KAFunctionUser::KAFunctionUser(const KAFunctionUser& r)
	:	KAFunctionBase(KEMAssignmentFunctionUser)
{
	std::map<int, KAFunction*>::const_iterator itFunction, itEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KAFunction* pFunction = new KAFunction(*(itFunction->second));
		m_FunctionMap.insert(std::make_pair(itFunction->first, pFunction));
	}
}


KAFunctionUser::~KAFunctionUser(void)
{
	clearFunctionMap();
}


KAFunctionUser& KAFunctionUser::operator=(const KAFunctionUser& r)
{
	clearFunctionMap();

	std::map<int, KAFunction*>::const_iterator itFunction, itEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KAFunction* pFunction = new KAFunction(*(itFunction->second));
		m_FunctionMap.insert(std::make_pair(itFunction->first, pFunction));
	}

	return *this;
}

KAFunction* KAFunctionUser::Function(int nCode)
{
	KAFunction* pFunction = NULL;
	std::map<int, KAFunction*>::iterator itFunction = m_FunctionMap.find(nCode);
	if(m_FunctionMap.end() != itFunction)
	{
		pFunction = itFunction->second;
	}

	return pFunction;
}


void KAFunctionUser::Function(int nCode, KAFunction* pFunction)
{
	std::map<int, KAFunction*>::iterator itFunction = m_FunctionMap.find(nCode);
	if(m_FunctionMap.end() != itFunction)
	{
		*(itFunction->second) = *pFunction;
	}
	else
	{
		KAFunction* pNewFunction = new KAFunction(*pFunction);
		m_FunctionMap.insert(std::make_pair(nCode, pNewFunction));
	}
}


void KAFunctionUser::clearFunctionMap(void)
{
	std::map<int, KAFunction*>::iterator itFunction, itEnd = m_FunctionMap.end();
	for(itFunction = m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KAFunction* pFunction = itFunction->second;
		delete pFunction;
	}

	m_FunctionMap.clear();
}

void KAFunctionUser::GetArgumentInfo(CStringArray& strOutArray, KIOColumns& fixedLinkColumns, KIOColumns& userAddLinkColumns)
{
    int nFiexedLinkColumnCount = fixedLinkColumns.ColumnCount();
    userAddLinkColumns.Clear();
    CString      strOut;

    // m_FunctionMap : 코드와 함수식 정보
    std::map<int, KAFunction*>::iterator iter = m_FunctionMap.begin();
    std::map<int, KAFunction*>::iterator end  = m_FunctionMap.end();

    // 수식맵을 순회
    while (iter != end)
    {
        KAFunction* pFunction = iter->second;        
                        
        // 한개의 User Define Function 순회
        CStringArray strExpInfoArray;
        const std::list<KExpressionElement*>& expressionList = pFunction->GetExpressionList();
        std::list<KExpressionElement*>::const_iterator expIter = expressionList.begin();
        std::list<KExpressionElement*>::const_iterator expEnd  = expressionList.end();

        while ( expIter != expEnd )
        {
            KExpressionElement* pElement = *expIter;            
            
            int nColumnIndex      = -1;
            int nFixedColumnIndex = -1;
            int nUserColumnIndex  = -1;

            if ( KEMExpressionElementField == pElement->ElementType() )
            {
                KExpressionElementField* pField = (KExpressionElementField*)pElement;     

                CString strFieldName = pField->FieldName();
                nFixedColumnIndex = fixedLinkColumns.GetColumnIndex(strFieldName);

                if (nFixedColumnIndex < 0)
                {
                    nUserColumnIndex = userAddLinkColumns.GetColumnIndex(strFieldName);
                    if (nUserColumnIndex < 0)
                    {
                        KIOColumn* pColumn = userAddLinkColumns.AddNewColumn();
                        pColumn->Name(strFieldName);

                        nUserColumnIndex = userAddLinkColumns.GetColumnIndex(strFieldName);
                    }

                    nColumnIndex = nFiexedLinkColumnCount + nUserColumnIndex;
                }
                else
                {
                    nColumnIndex = nFixedColumnIndex;
                }
                
                CString tOut;
                tOut.Format(_T("%s\t%d\t"), strFieldName, nColumnIndex);
                strExpInfoArray.Add(tOut);
            }
            ++expIter;
        }

        CString strExpression = pFunction->GetExpression();
        int     nCount        = strExpInfoArray.GetCount();
        int     nCode         = iter->first;
        
        if (nCount > 0)
        {
            strOut.Format(_T("%d\t%d\t%s\t%d\t"), 2, nCode, strExpression, nCount);
            for (int i=0; i<nCount; i++)
            {
                strOut = strOut + strExpInfoArray[i];
            }
            strOut = strOut + _T("\r\n");
        }
        else
        {
            strOut.Format(_T("%d\t%d\t%s\t%d\r\n"), 2, nCode, strExpression, 0);
        }
        
        strOutArray.Add(strOut);

        ++iter;
    }
}
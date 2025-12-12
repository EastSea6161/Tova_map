#include "StdAfx.h"
#include "InterFunctionUser.h"

#include "InterFunctionUserEXP.h"
//^^ #include "IOColumns.h"
#include "ExpressionElementField.h"

//^^ #include "IOColumn.h"

KInterFunctionUser::KInterFunctionUser(void)
	:	KInterFunctionBase(KEMInterModalFunctionUser)
{
}

KInterFunctionUser::KInterFunctionUser(const KInterFunctionUser& r)
	:	KInterFunctionBase(KEMInterModalFunctionUser)
{
	std::map<int, KInterFunctionUserEXP*>::const_iterator itFunction, itEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KInterFunctionUserEXP* pFunction = new KInterFunctionUserEXP(*(itFunction->second));
		m_FunctionMap.insert(std::make_pair(itFunction->first, pFunction));
	}
}

KInterFunctionUser::~KInterFunctionUser(void)
{
	clearFunctionMap();
}

KInterFunctionUser& KInterFunctionUser::operator=(const KInterFunctionUser& r)
{
	clearFunctionMap();

	std::map<int, KInterFunctionUserEXP*>::const_iterator itFunction, itEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KInterFunctionUserEXP* pFunction = new KInterFunctionUserEXP(*(itFunction->second));
		m_FunctionMap.insert(std::make_pair(itFunction->first, pFunction));
	}

	return *this;
}

KInterFunctionUserEXP* KInterFunctionUser::Function(int nCode)
{
	KInterFunctionUserEXP* pFunction = NULL;
	std::map<int, KInterFunctionUserEXP*>::iterator itFunction = m_FunctionMap.find(nCode);
	if(m_FunctionMap.end() != itFunction)
	{
		pFunction = itFunction->second;
	}

	return pFunction;
}

void KInterFunctionUser::Function(int nCode, KInterFunctionUserEXP* pFunction)
{
	std::map<int, KInterFunctionUserEXP*>::iterator itFunction = m_FunctionMap.find(nCode);
	if(m_FunctionMap.end() != itFunction)
	{
		*(itFunction->second) = *pFunction;
	}
	else
	{
		KInterFunctionUserEXP* pNewFunction = new KInterFunctionUserEXP(*pFunction);
		m_FunctionMap.insert(std::make_pair(nCode, pNewFunction));
	}
}

void KInterFunctionUser::clearFunctionMap(void)
{
	std::map<int, KInterFunctionUserEXP*>::iterator itFunction, itEnd = m_FunctionMap.end();
	for(itFunction = m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KInterFunctionUserEXP* pFunction = itFunction->second;
		delete pFunction;
	}

	m_FunctionMap.clear();
}

void KInterFunctionUser::GetArgumentInfo(CStringArray& strOutArray, KIOColumns& fixedLinkColumns, KIOColumns& userAddLinkColumns)
{
	int nFiexedLinkColumnCount = fixedLinkColumns.ColumnCount();
	userAddLinkColumns.Clear();
	CString      strOut;

	// m_FunctionMap : 코드와 함수식 정보
	std::map<int, KInterFunctionUserEXP*>::iterator iter = m_FunctionMap.begin();
	std::map<int, KInterFunctionUserEXP*>::iterator end  = m_FunctionMap.end();

	// 수식맵을 순회
	while (iter != end)
	{
		KInterFunctionUserEXP* pFunction = iter->second;        

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
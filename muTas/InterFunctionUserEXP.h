#pragma once

/* Forward declarations */
class KExpressionElement;

class KInterFunctionUserEXP
{
public:
	KInterFunctionUserEXP(void);
	KInterFunctionUserEXP(const KInterFunctionUserEXP& r);
	~KInterFunctionUserEXP(void);

public:
	KInterFunctionUserEXP& operator=(const KInterFunctionUserEXP& r);

	void AddNumber(double dElement);

	void AddOperator(TCHAR szOeprator);

	void AddFunction(LPCTSTR strFunction);

	void AddField(LPCTSTR strFieldName, LPCTSTR strFieldCaption);

	void ClearElements(void);

	CString GetExpression(void);

	CString GetCaption(void);

	const std::list<KExpressionElement*>& GetExpressionList(void);

protected :
	std::list<KExpressionElement*> m_ElementList;
};


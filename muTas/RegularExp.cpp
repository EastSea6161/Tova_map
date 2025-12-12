#include "StdAfx.h"
#include "RegularExp.h"

#include <regex>

KRegularExp::KRegularExp(void)
{
}


KRegularExp::~KRegularExp(void)
{
}

bool KRegularExp::IsLowerAlphabet( CString a_strTarget )
{
	string sTarget((CStringA)a_strTarget);
	
	tr1::smatch match;
	tr1::regex  regPattern("[a-z]+");
	
	bool bMatched = tr1::regex_match(sTarget, match, regPattern);

	return bMatched;
}

#pragma once

using namespace std;

class KRegularExp
{
public:
	KRegularExp(void);
	~KRegularExp(void);

public:
	//소문자로만 구성된 문자열인지 Check
	static bool IsLowerAlphabet(CString a_strTarget);
};


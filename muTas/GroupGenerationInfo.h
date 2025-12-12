#pragma once

class KCodeGroup;
class KIOColumn;

struct TCodeGoupColumn
{
	KCodeGroup* pCodeGroup;
	KIOColumn*  pIOColumn;
};

struct TCodeName
{
	int     m_nCode;
	CString m_strName;
};

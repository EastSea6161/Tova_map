#pragma once
class KModelChoiceArgInfo
{
public:
	KModelChoiceArgInfo(void);
	~KModelChoiceArgInfo(void);

	struct TNestedInfo
	{
		int				nNestedIndex;
		std::set<int>	setChild;
		double			dInclusiveValue;
	};

public:

	void	PurposeIndex(int a_nPurposeIndex);

	int		PurposeIndex();

	void	FileName(CString a_strFileName);

	CString FileName();

	void	CountNotNested(int a_nNotNested);

	int		CountNotNested();

	void	SetMode(std::set<int> a_setMode);

	void	GetMode(std::set<int>& a_setMode);

	void	CountNested(int a_Nested);

	int		CoutNestd();

	void	SetNestedInfo(std::vector<TNestedInfo*> a_vecNestedInfo);

	void	GetNestedInfo(std::vector<TNestedInfo*>& a_vecNestedInfo);


protected:

	int							m_nPurposeINdex;
	CString						m_strFileName;
	int							m_nNotNestedCount;
	std::set<int>				m_setMode;
	int							m_nNestedCount;
	std::vector<TNestedInfo*>	m_vecNestedInfo;

};


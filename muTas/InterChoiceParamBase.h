#pragma once

/*forward declare*/
class KInterChoiceParam;

class KInterChoiceParamBase
{
public:
	KInterChoiceParamBase(void);
	~KInterChoiceParamBase(void);

public:

	bool	Selected(void);
	void	Selected(bool a_bSelected);

	void	Caption(LPCTSTR a_strCaption);
	LPCTSTR	Caption(void);

	void	SetInterChoiceParam(std::vector<KInterChoiceParam*> a_vecChoiceParam);
	std::vector<KInterChoiceParam*> GetInterChoiceParam(void);
	
	void	ClearChoiceParamVector(void);

	void	InitChoiceParamDataVector(void);

protected:

	LPCTSTR m_strCaption;
	bool	m_bSelected;

	std::vector<KInterChoiceParam*> m_vecChoiceParam;
};


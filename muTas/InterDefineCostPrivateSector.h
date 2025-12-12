#pragma once
class KInterDefineCostPrivateSector
{
public:
	KInterDefineCostPrivateSector(void);
	KInterDefineCostPrivateSector(int a_nIndex, CString a_strName);
	~KInterDefineCostPrivateSector(void);

public:

	int		GetIndex();
	void	SetIndex(int a_nIndex);
	
	CString GetName();
	void	SetName(CString a_strName);

	double	GetPrice();
	void	SetPrice(double a_dblPrice);

	double	GetRatio();
	void	SetRatio(double a_dblRatio);

	double	GetFuelEfficiency();
	void	SetFuelEfficiency(double a_dblFuelEfficiency);
	
private:
	
	/*Auto Type ID*/
	int m_nIndex;
	/*Auto Type Nm*/
	CString m_strName;
	/*가격*/
	double m_dblPrice;
	/*구성비율*/
	double m_dblRatio;
	/*평균연비*/
	double m_dblFuelEfficiency;
};


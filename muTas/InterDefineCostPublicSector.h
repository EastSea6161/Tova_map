#pragma once
class KInterDefineCostPublicSector
{
public:
	
	KInterDefineCostPublicSector(void);
	KInterDefineCostPublicSector(int a_nIndex, CString a_strModeName);
	~KInterDefineCostPublicSector(void);

public:

	int		GetIndex();
	void	SetIndex(int a_nIndex);

	CString GetModeName();
	void	SetModeName(CString a_strModeName);

	double	GetBaseCharge();
	void	SetBaseCharge(double a_dblBaseCharge);

	double	GetBaseDistance();
	void	SetBaseDistance(double a_dblBaseDistance);

	double	GetAddCharge();
	void	SetAddCharge(double a_dblAddCharge);

	double	GetAddDistance();
	void	SetAddDistance(double a_dblAddDistance);

	
private:
	
	/*Mode Type ID*/
	int		m_nIndex;
	/*Mode Type Nm*/
	CString m_strModeName;
	/*기본요금*/
	double	m_dBaseCharge;
	/*기본거리*/
	double	m_dBaseDistance;
	/*추가요금*/
	double	m_dAddCharge;
	/*추가거리*/
	double	m_dAddDistance;
};


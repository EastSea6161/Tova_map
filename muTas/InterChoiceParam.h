#pragma once
class KInterChoiceParam
{
public:
	KInterChoiceParam(void);
	KInterChoiceParam(CString a_strAttr);
	~KInterChoiceParam(void);


public:

	CString Attribute(void);
	void	Attribute(CString a_strAttr);

	double	Auto(void);
	void	Auto(double a_dblAuto);

	double	Taxi(void);
	void	Taxi(double a_dblTaxi);

	double	Bus(void);
	void	Bus(double a_dblAuto);

	double	Rail(void);
	void	Rail(double a_dblRail);

	void	InitParamData(void);

	
protected:

	CString m_strAttr;

	double m_dblAuto;
	double m_dblTaxi;
	double m_dblBus;
	double m_dblRail;
};


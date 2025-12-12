#pragma once

/*forward declare*/
class KIOColumn;
class KMode;

class KSCAnalysisInfo
{
public:
	KSCAnalysisInfo(void);
	KSCAnalysisInfo(const KSCAnalysisInfo& r);
	~KSCAnalysisInfo(void);

public:
	KSCAnalysisInfo& operator=(const KSCAnalysisInfo& r);

	bool		Selected(void);
	void		Selected(bool a_bSelected);

	double		Occupancy(void);
	void		Occupancy(double a_dOccupancy);

	double		ODPCU(void);
	void		ODPCU(double a_dODPCU);

	double		VolumePCU(void);
	void		VolumePCU(double a_dVolumePCU);

	KMode*		Mode(void);
	void		Mode(KMode* a_pMode);

	KIOColumn*	InputColumn(void);
	void		InputColumn(KIOColumn* a_pInputColumn);

	KIOColumn*	SurveyColumn(void);
	void		SurveyColumn(KIOColumn* a_pSurveyColumn);

	KIOColumn*	OutputColumn(void);
	void		OutputColumn(KIOColumn* a_pOutputColumn);

	CString		OutputColumnName(void);
	void		OutputColumnName(CString a_strOutputColumnCaption);

	CString		ColumnName(void);
	void		ColumnName(CString a_strColumnName);

private:
	bool		m_bSelected;
	double		m_dOccupancy;
	double		m_dVolumnPCU;
	double		m_dODPCU;
	KMode*		m_pMode;
	
	KIOColumn*	m_pInputColumn;
	KIOColumn*	m_pSurveyColumn;
	KIOColumn*	m_pOutputColumn;

	CString		m_strOutputColumnCaption;
	CString     m_strColumnName;
};


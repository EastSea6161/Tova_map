

#pragma once

class KIOColumn;
class KMode;

class KMultiModalInfo
{
public:
	KMultiModalInfo(void);
	KMultiModalInfo(const KMultiModalInfo& r);
	~KMultiModalInfo(void);

public:

	KMultiModalInfo& operator=(const KMultiModalInfo& r);

public:

	inline void Selected(bool a_bSelected)
	{
		m_bSelected = a_bSelected;
	}

	inline bool Selected(void)
	{
		return m_bSelected;
	}

	inline void NetworkType(int a_nNetworktype)
	{
		m_nNetworkType = a_nNetworktype;
	}

	inline int  NetworkType(void)
	{
		return m_nNetworkType;
	}

	inline void PreLoad(int a_nPreLoad)
	{
		m_nPreLoad = a_nPreLoad;
	}

	inline int  PreLoad(void)
	{
		return m_nPreLoad;
	}

	inline void Mode(KMode* a_pMode)
	{
		m_pMode = a_pMode;
	}
	
	inline KMode* Mode(void)
	{
		return m_pMode;
	}

	inline void Occupancy(double a_dOccupancy)
	{
		m_dOccupancy = a_dOccupancy;
	}

	inline double Occupancy(void)
	{
		return m_dOccupancy;
	}

	inline void PCU (double a_dPCU)
	{
		m_dPCU = a_dPCU;
	}

	inline double PCU (void)
	{
		return m_dPCU;
	}

	inline void ModeColumn(KIOColumn* a_pModeColumn)
	{
		m_pModeColumn = a_pModeColumn;
	}

	inline KIOColumn* ModeColumn(void)
	{
		return m_pModeColumn;
	}

	inline void TransitHighwayType(int a_nType)
	{
		m_nTransitHighType = a_nType;
	}

	inline int TransitHighwayType(void)
	{
		return m_nTransitHighType;
	}

	inline void Exclusive(int a_nExclusive)
	{
		m_nExclusive = a_nExclusive;
	}

	inline int Exclusive(void)
	{
		return m_nExclusive;
	}

protected:
	bool		m_bSelected;
	//* 
	int			m_nNetworkType;

	//* 0: Highway	1:Transit_Bus	2:Transit_Subway 3 : Transit_Bus+Highway
	int			m_nTransitHighType;

	//* 0: NotSelected 1:Selected
	int			m_nPreLoad;
	
	//* Type捞  HighWay老 版快 KMode 积己
	KMode*		m_pMode;
	
	double		m_dOccupancy;
	double		m_dPCU;

	KIOColumn*	m_pModeColumn;

	//* 0:NotSelected 1:Selected
	int			m_nExclusive;

};


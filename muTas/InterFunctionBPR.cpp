#include "StdAfx.h"
#include "InterFunctionBPR.h"

#include "InterFunctionBase.h"
//^^ #include "IOColumns.h"


KInterFunctionBPR::KInterFunctionBPR(void)
	:	KInterFunctionBase(KEMInterModalFunctionBPR),
		m_bForAllLink(TRUE),
		m_dAlpha(0.15f),
		m_dBeta(4.0f),
		m_dTheta(0.0f),
		m_pColumn(NULL)
{
}

KInterFunctionBPR::KInterFunctionBPR(const KInterFunctionBPR& r)
	:	KInterFunctionBase(KEMInterModalFunctionBPR),
	m_bForAllLink(r.m_bForAllLink),
	m_dAlpha(r.m_dAlpha),
	m_dBeta(r.m_dBeta),
	m_dTheta(r.m_dTheta),
	m_pColumn(r.m_pColumn)
{
	m_ParameterMap = r.m_ParameterMap;
}

KInterFunctionBPR::~KInterFunctionBPR(void)
{
	ClearParameter();
}

KInterFunctionBPR& KInterFunctionBPR::operator=(const KInterFunctionBPR& r)
{
	KInterFunctionBase::operator=(r);

	m_bForAllLink = r.m_bForAllLink;
	m_dAlpha = r.m_dAlpha;
	m_dBeta = r.m_dBeta;
	m_dTheta = r.m_dTheta;
	m_pColumn = r.m_pColumn;

	m_ParameterMap = r.m_ParameterMap;

	return *this;
}

bool KInterFunctionBPR::FunctionForAllLink(void)
{
	return m_bForAllLink;
}

void KInterFunctionBPR::FunctionForAllLink(bool bAll)
{
	m_bForAllLink = bAll;
}

void KInterFunctionBPR::GetDefaultParameter(double& dAlpha, double& dBeta, double& dTheta, KIOColumn*& pColumn)
{
	dAlpha = m_dAlpha;
	dBeta = m_dBeta;
	dTheta = m_dTheta;
	pColumn = m_pColumn;
}

void KInterFunctionBPR::SetDefaultParameter(const double& dAlpha, const double& dBeta, const double& dTheta, KIOColumn* pColumn)
{
	m_dAlpha = dAlpha;
	m_dBeta = dBeta;
	m_dTheta = dTheta;
	m_pColumn = pColumn;
}

void KInterFunctionBPR::GetParameter(int nCode, double& dAlpha, double& dBeta, double& dTheta, KIOColumn*& pColumn)
{
	std::map<int, KSTInterFunctionParameter>::iterator itParameter = m_ParameterMap.find(nCode);
	if(m_ParameterMap.end() != itParameter)
	{
		KSTInterFunctionParameter& p = itParameter->second;
		dAlpha = p.dAlpha;
		dBeta = p.dBeta;
		dTheta = p.dTheta;
		pColumn = p.pColumn;
	}
	else
	{
		dAlpha = 0.14f;
		dBeta = 4.0f;
		dTheta = 0.0f;
		pColumn = NULL;
	}
}

void KInterFunctionBPR::SetParameter(int nCode, const double& dAlpha, const double& dBeta, const double& dTheta, KIOColumn* pColumn)
{
	std::map<int, KSTInterFunctionParameter>::iterator itParameter = m_ParameterMap.find(nCode);
	if(m_ParameterMap.end() != itParameter)
	{
		itParameter->second.dAlpha = dAlpha;
		itParameter->second.dBeta = dBeta;
		itParameter->second.dTheta = dTheta;
		itParameter->second.pColumn = pColumn;
	}
	else
	{
		KSTInterFunctionParameter p;
		p.dAlpha = dAlpha;
		p.dBeta = dBeta;
		p.dTheta = dTheta;
		p.pColumn = pColumn;
		m_ParameterMap.insert(std::make_pair(nCode, p));
	}
}

void KInterFunctionBPR::ClearParameter(void)
{
	m_ParameterMap.clear();
}

void KInterFunctionBPR::GetArgumentInfo(CStringArray& strOutArray, KIOColumns& fixedLinkColumns, KIOColumns& userAddLinkColumns)
{
	int nFiexedLinkColumnCount = fixedLinkColumns.ColumnCount();
	userAddLinkColumns.Clear();

	CString      strOut;

	if (! m_bForAllLink )
	{
		// std::map<int, KSTAFunctionParameter> m_ParameterMap
		std::map<int, KSTInterFunctionParameter>::iterator iter = m_ParameterMap.begin();
		std::map<int, KSTInterFunctionParameter>::iterator end  = m_ParameterMap.end();

		KIOColumn* pColumn = NULL;
		while ( iter != end )
		{
			KSTInterFunctionParameter oParameter = iter->second;
			pColumn = oParameter.pColumn;

			int nColumnIndex      = -1;
			int nFixedColumnIndex = -1;
			int nUserColumnIndex  = -1;

			if (pColumn)
			{
				nFixedColumnIndex = fixedLinkColumns.GetColumnIndex(pColumn);
				if (nFixedColumnIndex < 0)
				{
					nUserColumnIndex = userAddLinkColumns.GetColumnIndex(pColumn);
					if (nUserColumnIndex < 0)
					{
						nUserColumnIndex = nFiexedLinkColumnCount + userAddLinkColumns.AddNewColumn(pColumn);
					}

					nColumnIndex = nFiexedLinkColumnCount + nUserColumnIndex;
				}
				else
				{
					nColumnIndex = nFixedColumnIndex;
				}

				// linkÀÇ °æ¿ì link_id = 0 
				strOut.Format(_T("%d\t%d\t%f\t%f\t%f\t%d\r\n"), 2, iter->first, oParameter.dAlpha, oParameter.dBeta, oParameter.dTheta, nColumnIndex);
			}
			else
			{
				strOut.Format(_T("%d\t%d\t%f\t%f\t%f\t%d\r\n"), 2, iter->first, oParameter.dAlpha, oParameter.dBeta, oParameter.dTheta, 0);
			}

			strOutArray.Add(strOut);

			++iter;
		}
	}
	else
	{
		if (m_pColumn)
		{
			int nColumnIndex      = -1;
			int nFixedColumnIndex = -1;
			int nUserColumnIndex  = -1;

			nFixedColumnIndex = fixedLinkColumns.GetColumnIndex(m_pColumn);

			if (nFixedColumnIndex < 0)
			{
				nUserColumnIndex = userAddLinkColumns.GetColumnIndex(m_pColumn);
				if (nUserColumnIndex < 0)
				{
					nUserColumnIndex = userAddLinkColumns.AddNewColumn(m_pColumn);
				}

				nColumnIndex = nFiexedLinkColumnCount + nUserColumnIndex;               
			}
			else
			{
				nColumnIndex = nFixedColumnIndex;
			}

			strOut.Format(_T("%d\t%f\t%f\t%f\t%d\r\n"), 1, m_dAlpha, m_dBeta, m_dTheta, nColumnIndex);
		}
		else
		{
			strOut.Format(_T("%d\t%f\t%f\t%f\t%d\r\n"), 1, m_dAlpha, m_dBeta, m_dTheta, 0);
		}

		strOutArray.Add(strOut);
	}

}
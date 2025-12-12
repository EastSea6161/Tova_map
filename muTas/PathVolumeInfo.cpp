#include "StdAfx.h"
#include "PathVolumeInfo.h"


KPathVolumeInfo::KPathVolumeInfo(void)
{
}


KPathVolumeInfo::~KPathVolumeInfo(void)
{
}


void KPathVolumeInfo::CalculateValue2Width(double a_dTrip, TPathVolumeSettingInfo &a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}


void KPathVolumeInfo::GetDrawBushVolumeInfo(std::map<Integer/*DZone*/, std::vector<TPathVolumeInfo>> &a_mapPathVolumeInfo, 
					                        TPathVolumeSettingInfo     &a_oSettingInfo,
	                                        std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, 
											std::vector<TDrawLinkInfo> &a_vecDrawLinkInfoNoLabel)
{
	a_vecDrawLinkInfo.clear();
	a_vecDrawLinkInfoNoLabel.clear();

	try
	{
		AutoType iter  = a_mapPathVolumeInfo.begin();
		AutoType itEnd = a_mapPathVolumeInfo.end();

		std::map<Integer, double> mapLinkVolume;

		while (iter != itEnd)
		{
			std::vector<TPathVolumeInfo> &vecPathVolumeInfo = iter->second;

			for (size_t i= 0; i< vecPathVolumeInfo.size(); i++)
			{
				TPathVolumeInfo      &oTPathVolumeInfo = vecPathVolumeInfo[i];
				std::vector<Integer> &vecLink          = oTPathVolumeInfo.TvecLink;
				
				for (size_t k= 0; k< vecLink.size(); k++)
				{
					Integer &nxLinkID = vecLink[k];

					AutoType iterB  = mapLinkVolume.find(nxLinkID);
					AutoType itEndB = mapLinkVolume.end();
					if (iterB != itEndB)
					{
						double &dVolume = iterB->second;
						dVolume += oTPathVolumeInfo.TdVolume;
					}
					else
					{
						mapLinkVolume.insert(std::make_pair(nxLinkID, oTPathVolumeInfo.TdVolume));
					}
				}
			}
			++iter;
		}
		
		AutoType iterLink  = mapLinkVolume.begin();
		AutoType itLinkEnd = mapLinkVolume.end();

		while (iterLink != itLinkEnd)
		{
			TDrawLinkInfo oLinkInfo;
			Integer nxLinkID		= iterLink->first;
			double  dVolume		    = iterLink->second;
			
			CString &strVolume      = oLinkInfo.TLabel;          
			strVolume.Format(_T("%.3f"), dVolume);

			oLinkInfo.TLinkID = nxLinkID;

			if (dVolume > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterLink;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth	= a_oSettingInfo.dMinSize;
			}
			else if (dVolume < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterLink;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth	= a_oSettingInfo.dMinSize;
			}
			else
			{
				oLinkInfo.TColorRGB = a_oSettingInfo.clrPath;
				double dWidth(0.0);
				CalculateValue2Width(dVolume, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;
			}

			a_vecDrawLinkInfo.push_back(oLinkInfo);

			oLinkInfo.TLabel = _T("");
			a_vecDrawLinkInfoNoLabel.push_back(oLinkInfo);

			++iterLink;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

}


void KPathVolumeInfo::GetDrawGogumaVolumeInfo(std::vector<TPathVolumeInfo> &a_vecPathVolumeInfo, 
	                                          TPathVolumeSettingInfo       &a_oSettingInfo, 
											  std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfo,
											  std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfoNoLabel)
{
	a_vecDrawLinkInfo.clear();
	a_vecDrawLinkInfoNoLabel.clear();

	try
	{
		std::map<Integer, double> mapLinkVolume;

		for (size_t i= 0; i< a_vecPathVolumeInfo.size(); i++)
		{
			TPathVolumeInfo      &oTPathVolumeInfo = a_vecPathVolumeInfo[i];
			std::vector<Integer> &vecLink          = oTPathVolumeInfo.TvecLink;

			for (size_t k= 0; k< vecLink.size(); k++)
			{
				Integer &nxLinkID = vecLink[k];

				AutoType iterB  = mapLinkVolume.find(nxLinkID);
				AutoType itEndB = mapLinkVolume.end();
				if (iterB != itEndB)
				{
					double &dVolume = iterB->second;
					dVolume += oTPathVolumeInfo.TdVolume;
				}
				else
				{
					mapLinkVolume.insert(std::make_pair(nxLinkID, oTPathVolumeInfo.TdVolume));
				}
			}
		}

		AutoType iterLink  = mapLinkVolume.begin();
		AutoType itLinkEnd = mapLinkVolume.end();

		while (iterLink != itLinkEnd)
		{
			TDrawLinkInfo oLinkInfo;
			Integer nxLinkID		= iterLink->first;
			double  dVolume		    = iterLink->second;

			CString &strVolume      = oLinkInfo.TLabel;          
			strVolume.Format(_T("%.3f"), dVolume);

			oLinkInfo.TLinkID = nxLinkID;

			if (dVolume > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterLink;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth	= a_oSettingInfo.dMinSize;
			}
			else if (dVolume < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterLink;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth	= a_oSettingInfo.dMinSize;
			}
			else
			{
				oLinkInfo.TColorRGB = a_oSettingInfo.clrPath;
				double dWidth(0.0);
				CalculateValue2Width(dVolume, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;
			}

			a_vecDrawLinkInfo.push_back(oLinkInfo);

			oLinkInfo.TLabel = _T("");
			a_vecDrawLinkInfoNoLabel.push_back(oLinkInfo);

			++iterLink;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeInfo::GetDrawPathVolumeInfo(std::vector<TPathVolumeInfo> &a_vecPathVolumeInfo, 
	                                        std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfo,
											std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfoNoLabel)
{
	a_vecDrawLinkInfo.clear();
	a_vecDrawLinkInfoNoLabel.clear();

	try
	{
		std::map<Integer, double> mapLinkVolume;

		for (size_t i= 0; i< a_vecPathVolumeInfo.size(); i++)
		{
			TPathVolumeInfo      &oTPathVolumeInfo = a_vecPathVolumeInfo[i];
			std::vector<Integer> &vecLink          = oTPathVolumeInfo.TvecLink;

			for (size_t k= 0; k< vecLink.size(); k++)
			{
				Integer &nxLinkID = vecLink[k];

				TDrawLinkInfo oLinkInfo;
				oLinkInfo.TLinkID          = nxLinkID;

				CString &strVolume         = oLinkInfo.TLabel;          
				strVolume.Format(_T("%.3f"), oTPathVolumeInfo.TdVolume);

				oLinkInfo.TColorRGB        = oTPathVolumeInfo.Tcolor;
				oLinkInfo.TWidth           = oTPathVolumeInfo.TdWidth;

				a_vecDrawLinkInfo.push_back(oLinkInfo);

				oLinkInfo.TLabel = _T("");
				a_vecDrawLinkInfoNoLabel.push_back(oLinkInfo);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeInfo::GetDrawVolumeAreaInfo( std::map<Integer, double>  &a_mapLinkVolume,
	TPathVolumeSettingInfo     &a_oSettingInfo,
	std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, 
	std::vector<TDrawLinkInfo> &a_vecDrawLinkInfoNoLabel)
{
	a_vecDrawLinkInfo.clear();
	a_vecDrawLinkInfoNoLabel.clear();

	try
	{
		AutoType iterLink  = a_mapLinkVolume.begin();
		AutoType itLinkEnd = a_mapLinkVolume.end();

		while (iterLink != itLinkEnd)
		{
			TDrawLinkInfo oLinkInfo;
			Integer nxLinkID		= iterLink->first;
			double  dVolume		    = iterLink->second;

			CString &strVolume      = oLinkInfo.TLabel;          
			strVolume.Format(_T("%.3f"), dVolume);

			oLinkInfo.TLinkID = nxLinkID;

			if (dVolume > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterLink;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth	= a_oSettingInfo.dMinSize;
			}
			else if (dVolume < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterLink;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth	= a_oSettingInfo.dMinSize;
			}
			else
			{
				oLinkInfo.TColorRGB = a_oSettingInfo.clrPath;
				double dWidth(0.0);
				CalculateValue2Width(dVolume, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;
			}

			a_vecDrawLinkInfo.push_back(oLinkInfo);

			oLinkInfo.TLabel = _T("");
			a_vecDrawLinkInfoNoLabel.push_back(oLinkInfo);

			++iterLink;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

}
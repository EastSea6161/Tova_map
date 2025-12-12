/**
 * @file CordonLine.cpp
 * @brief KCordonLine 구현 파일
 * @author 
 * @date 2011.09.05
 * @remark 
 */


#include "StdAfx.h"
#include "CordonLine.h"
#include "MapView.h"
#include "PersonalGeoDataBase.h"
#include "IOTableManager.h"
#include "IOTable.h"
#include "Target.h"
#include "NodeFeatureFunctions.h"
#include "LinkFeatureFunctions.h"
#include "ZoneFeatureFunctions.h"
#include "NodeLayerFunctions.h"


KCordonLine::KCordonLine(void)
{
	m_bFinish = true;

	m_spFillSymbol.CreateInstance(CLSID_SimpleFillSymbol);

	IColorPtr spFillColor(CLSID_RgbColor);
	spFillColor->put_RGB(RGB(0, 255, 0));
	m_spFillSymbol->put_Color(spFillColor);

	ISimpleLineSymbolPtr spLineSymbol(CLSID_SimpleLineSymbol);
	IColorPtr spLineColor(CLSID_RgbColor);
	spLineColor->put_RGB(RGB(0, 0, 0));
	spLineSymbol->put_Color(spLineColor);
	spLineSymbol->put_Width(1.0f);
	spLineSymbol->put_Style(esriSLSDash);
	m_spFillSymbol->put_Outline(spLineSymbol);	

	m_spMarkerSymbol.CreateInstance(CLSID_SimpleMarkerSymbol);
	IColorPtr spColor(CLSID_RgbColor);
	spColor->put_RGB(RGB(255, 0, 0));
	m_spMarkerSymbol->put_Color(spColor);
	m_spMarkerSymbol->put_OutlineSize(4);
	m_spMarkerSymbol->put_Style(esriSMSSquare);
}


KCordonLine::~KCordonLine(void)
{
}


void KCordonLine::SetMapView(KMapView* pMapView)
{
	m_pMapView = pMapView;
}


void KCordonLine::SetTarget(KTarget* pTarget)
{
	m_pTarget = pTarget;
}


void KCordonLine::SetLinkLayer(IFeatureLayerPtr spLayer)
{
	m_spLinkLayer = spLayer;
}


bool KCordonLine::StartCordonLine(void)
{
	m_bFinish = false;
	m_spPolygon.CreateInstance(CLSID_Polygon);

	return true;
}


void KCordonLine::MouseDown(long button, long shift, long X, long Y, double mapX, double mapY)
{
	if(VK_LBUTTON == button)
	{
		IPointPtr spPoint(CLSID_Point);
		spPoint->PutCoords(mapX, mapY);

		IPointCollectionPtr spPointCollection = m_spPolygon;
		spPointCollection->AddPoint(spPoint);


		IMapControl4Ptr spMapControl = m_pMapView->GetMapControl();
		spMapControl->Refresh(esriViewForeground);
	}
}


void KCordonLine::MouseMove(long button, long shift, long X, long Y, double mapX, double mapY)
{

}


void KCordonLine::MouseUp(long button, long shift, long X, long Y, double mapX, double mapY)
{

}


void KCordonLine::MouseDoubleClick(long button, long shift, long X, long Y, double mapX, double mapY)
{
	if(VK_LBUTTON == button)
	{
		m_spPolygon->SimplifyPreserveFromTo();
		m_spPolygon->Close();
		CWaitCursor cursor;
		processCordonLine();
		m_bFinish = true;
	}
}


void KCordonLine::DrawCordonLine(IActiveViewPtr spActiveView)
{
	if(NULL == spActiveView)
	{
		return;
	}

	IScreenDisplayPtr spScreenDisplay;
	spActiveView->get_ScreenDisplay(&spScreenDisplay);

	OLE_HANDLE hDC;
	spScreenDisplay->get_hDC(&hDC);
	spScreenDisplay->StartDrawing(hDC, esriNoScreenCache);

	spScreenDisplay->SetSymbol((ISymbolPtr)m_spFillSymbol);
	spScreenDisplay->DrawPolygon(m_spPolygon);

	IPointCollectionPtr spPointCollection = m_spPolygon;
	IPointPtr spPoint;
	spScreenDisplay->SetSymbol((ISymbolPtr)m_spMarkerSymbol);
	long nPointCount;
	spPointCollection->get_PointCount(&nPointCount);
	for(long i = 0; i < nPointCount; ++i)
	{
		spPointCollection->get_Point(i, &spPoint);
		spScreenDisplay->DrawPoint(spPoint);
	}

	spScreenDisplay->FinishDrawing();
}


bool KCordonLine::IsFinished(void)
{
	return m_bFinish;
}


void KCordonLine::processCordonLine()
{
	std::set<__int64> ZoneA, ZoneB;
	std::set<__int64> LinkA, LinkB;

	IFeatureCursorPtr spFeatureCursor;
	IFeaturePtr spFeature;
	IRelationalOperatorPtr spOper = m_spPolygon;
	VARIANT_BOOL bContain;

	KIOTableManager* pTableManager = m_pTarget->Tables();
	KIOTable* pZoneTable = pTableManager->FindTable(_T("ZONE"));
	if(NULL == pZoneTable)
	{
		return;
	}

	KPersonalGeoDatabase* pGeoData = m_pMapView->GetPersonalGeoDataBase();
	IMapControl4Ptr spMapControl = m_pMapView->GetMapControl();
	IFeatureLayerPtr spNodeLayer = pGeoData->FindFeatureLayer(_T("NODE"));

	// process zone
	IFeatureLayerPtr spZoneLayer = pGeoData->FindFeatureLayer(_T("ZONE"));
	if(NULL != spZoneLayer)
	{
		spZoneLayer->Search(NULL, VARIANT_FALSE, &spFeatureCursor);
		if(NULL != spFeatureCursor)
		{
			spFeatureCursor->NextFeature(&spFeature);
			while(NULL != spFeature)
			{
				__int64 nZoneID = KZoneFeatureFunctions::GetZoneID(spFeature);
				IPointPtr spZoneNode = KZoneFeatureFunctions::GetZoneNode(spFeature, spNodeLayer, pZoneTable);
				spOper->Contains((IGeometryPtr)spZoneNode, &bContain);
				if(VARIANT_TRUE == bContain)
				{
					ZoneA.insert(nZoneID);
				}
				else
				{
					ZoneB.insert(nZoneID);
				}
				spFeatureCursor->NextFeature(&spFeature);
			}
		}
	}

	// process link
	IFeatureLayerPtr spLinkLayer = pGeoData->FindFeatureLayer(_T("LINK"));
	if(NULL != spLinkLayer)
	{
		ISpatialFilterPtr spFilter(CLSID_SpatialFilter);
		spFilter->putref_Geometry((IGeometryPtr)m_spPolygon);
		spFilter->put_SpatialRel(esriSpatialRelCrosses);
		spFilter->put_SearchOrder(esriSearchOrderSpatial);
		std::list<IFeaturePtr> linkList;

		spLinkLayer->Search(spFilter, VARIANT_FALSE, &spFeatureCursor);
		if(NULL != spFeatureCursor)
		{
			spFeatureCursor->NextFeature(&spFeature);
			while(NULL != spFeature)
			{
				linkList.push_back(spFeature);
				spFeatureCursor->NextFeature(&spFeature);
			}
		}

		std::list<IFeaturePtr>::iterator itLink, itLinkEnd = linkList.end();
		for(itLink = linkList.begin(); itLink != itLinkEnd; ++itLink)
		{
			spFeature = *itLink;
			__int64 nLinkID = KLinkFeatureFunctions::GetLinkID(spFeature);
			IPolylinePtr spPolyline = KLinkFeatureFunctions::GetPolyline(spFeature);
			IPointPtr spToPoint(CLSID_Point);
			spPolyline->QueryToPoint(spToPoint);
			spOper->Contains(spToPoint, &bContain);
			if(VARIANT_TRUE == bContain)
			{
				LinkB.insert(nLinkID);
			}
			else
			{
				LinkA.insert(nLinkID);
			}
		}
	}

	// write file
	CString strFile = m_pTarget->GetLocation();
	strFile += _T("\\CordonLine.txt");

	FILE* pFile = NULL;
    _wfopen_s(&pFile, strFile, _T("w"));
	if(NULL == pFile)
	{
		return;
	}

	std::set<__int64>::iterator itID, itIDEnd;

	_ftprintf(pFile, _T("[Zone A]\n"));
	for(itID = ZoneA.begin(), itIDEnd = ZoneA.end(); itID != itIDEnd; ++itID)
	{
		_ftprintf(pFile, _T("%I64d\n"), *itID);
	}
	_ftprintf(pFile, _T("\n\n"));

	_ftprintf(pFile, _T("[Zone B]\n"));
	for(itID = ZoneB.begin(), itIDEnd = ZoneB.end(); itID != itIDEnd; ++itID)
	{
		_ftprintf(pFile, _T("%I64d\n"), *itID);
	}
	_ftprintf(pFile, _T("\n\n"));

	_ftprintf(pFile, _T("[Link A]\n"));
	for(itID = LinkA.begin(), itIDEnd = LinkA.end(); itID != itIDEnd; ++itID)
	{
		_ftprintf(pFile, _T("%I64d\n"), *itID);
	}
	_ftprintf(pFile, _T("\n\n"));

	_ftprintf(pFile, _T("[Link B]\n"));
	for(itID = LinkB.begin(), itIDEnd = LinkB.end(); itID != itIDEnd; ++itID)
	{
		_ftprintf(pFile, _T("%I64d\n"), *itID);
	}
	_ftprintf(pFile, _T("\n\n"));

	fclose(pFile);

}
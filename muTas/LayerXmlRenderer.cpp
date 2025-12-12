#include "StdAfx.h"
#include "LayerXmlRenderer.h"
#include "Target.h"

KLayerXmlRenderer::KLayerXmlRenderer(KTarget* a_pTarget)
{
	m_pTarget = a_pTarget;
}

KLayerXmlRenderer::~KLayerXmlRenderer()
{

}


void KLayerXmlRenderer::SaveLinkProperty(KLinkLayerDispInfo& oInfo)
{
	KDBaseConPtr spDBaseConnection;

	try
	{
		spDBaseConnection = m_pTarget->GetDBaseConnection();
		spDBaseConnection->BeginTransaction();

        if (true) {
            CString strSQL(_T(" Delete From LayerRender_Link "));
            spDBaseConnection->ExecuteUpdate(strSQL);
        }
		
        CString strSQL(_T("")); {
            strSQL.Append(_T("Insert Into ")); 
            strSQL.Append(_T(" LayerRender_Link ")); 
            strSQL.Append(_T(" ( ")); 
            strSQL.Append(_T("  Layer_On_Off, ColorR,     ColorG,       ColorB,       Alpha, ")); 
            strSQL.Append(_T("  LineWidth,    MinScale,   MaxScale,     OffsetScale,  ArrowScale, "));
            strSQL.Append(_T("  LabelShow,    LabelField, LabelScaleOn, LabelOnScale, UseType, "));
            strSQL.Append(_T("  TypeID "));
            strSQL.Append(_T(" ) ")); 
            strSQL.Append(_T(" Values ")); 
            strSQL.Append(_T(" (")); 
            if (true) {
                strSQL.AppendFormat(_T(" %d,   %d,  %d, %d, %d, "), oInfo.TLayerOn, (int)GetRValue(oInfo.TLineColor), (int)GetGValue(oInfo.TLineColor), (int)GetBValue(oInfo.TLineColor), 0);
                strSQL.AppendFormat(_T(" %.1f, %f,  %f, %f, %f, "), oInfo.TLineWidth, oInfo.TMinScale,   oInfo.TMaxScale,     oInfo.TOffsetScale,  oInfo.TArrowScale);
                strSQL.AppendFormat(_T(" %d,  '%s', %d, %f, %d, "), oInfo.TLabelShow, oInfo.TLabelField, oInfo.TUseLabelScale, oInfo.TLabelScale, oInfo.TUseDrawType);
                strSQL.AppendFormat(_T(" %d "), oInfo.TDrawTypeID);
            }
            strSQL.Append(_T(" )")); 
        }
        		
		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		if (spDBaseConnection)
			spDBaseConnection->RollBack();
		TxLogDebugException();
	}
	catch (...)
	{
		if (spDBaseConnection)
			spDBaseConnection->RollBack();
		TxLogDebugException();
	}
}

void KLayerXmlRenderer::LoadLinkProperty(KLinkLayerDispInfo& oInfo)
{
	// Default
    oInfo.TLayerOn          = 1;
    oInfo.TLineColor        = RGB(192, 192, 192);
    oInfo.TLineWidth        = 1.0;
    oInfo.TMinScale         = 0.0;
    oInfo.TMaxScale         = 0.0/*2000000000*/;

    oInfo.TOffsetScale      = 0;
    oInfo.TArrowScale       = 0;

    oInfo.TLabelShow        = 0;
    oInfo.TLabelField       = _T("");

    oInfo.TUseLabelScale    = 0;
    //                        2147483647
    oInfo.TLabelScale       = 2000000000;
    oInfo.TUseDrawType      = 0;
    oInfo.TDrawTypeID       = 1;

	try
	{
		TxLogDebugStartMsg();

		KDBaseConPtr spDBaseConnetion = m_pTarget->GetDBaseConnection();

		CString strSQL(_T("")); {
            strSQL.Append(_T(" Select "));
            strSQL.Append(_T("  Layer_On_Off, ColorR,     ColorG,       ColorB,       Alpha, "));
            strSQL.Append(_T("  LineWidth,    MinScale,   MaxScale,     OffsetScale,  ArrowScale, "));
            strSQL.Append(_T("  LabelShow,    LabelField, LabelScaleOn, LabelOnScale, UseType, "));
            strSQL.Append(_T("  TypeID "));
            strSQL.Append(_T(" From LayerRender_Link "));
        }

		KResultSetPtr spResult = spDBaseConnetion->ExecuteQuery(strSQL);

		if( spResult->Next() ) {
            oInfo.TLayerOn  = spResult->GetValueInt(0);
            if (true) {
                int nColorR = spResult->GetValueInt(1);
                int nColorG = spResult->GetValueInt(2);
                int nColorB = spResult->GetValueInt(3);
                int nAlpha  = spResult->GetValueInt(4);

                oInfo.TLineColor = RGB(nColorR, nColorG, nColorB);
            }
            oInfo.TLineWidth    = spResult->GetValueDouble(5);
            //oInfo.TMinScale     = spResult->GetValueDouble(6);
            //oInfo.TMaxScale     = spResult->GetValueDouble(7);
            
            oInfo.TOffsetScale = spResult->GetValueDouble(8);      
            if (oInfo.TOffsetScale < 2000000000) {
                oInfo.TOffsetScale = 0;
            }

            oInfo.TArrowScale = spResult->GetValueDouble(9);
            if (oInfo.TArrowScale < 2000000000) {
                oInfo.TArrowScale = 0;
            }

            oInfo.TLabelShow     = spResult->GetValueInt   (10);
            oInfo.TLabelField    = spResult->GetValueString(11);
            oInfo.TUseLabelScale = spResult->GetValueInt   (12);

            oInfo.TLabelScale    = 2000000000;/*spResult->GetValueDouble(13);*/

            oInfo.TUseDrawType   = spResult->GetValueInt   (14);
            oInfo.TDrawTypeID    = spResult->GetValueInt   (15);
		} else{
			SaveLinkProperty(oInfo);
		}

		TxLogDebugEndMsg();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KLayerXmlRenderer::SaveNodeProperty(KNodeLayerDispInfo& oInfo)
{
	KDBaseConPtr spDBaseConnection;

	try
	{
		spDBaseConnection = m_pTarget->GetDBaseConnection();
		spDBaseConnection->BeginTransaction();

        if (true) {
		    CString strSQL(_T(" Delete From LayerRender_Node ")); 
            spDBaseConnection->ExecuteUpdate(strSQL);
        }
		
		CString strSQL(_T("")); {
            strSQL.Append(_T("Insert Into "));
            strSQL.Append(_T(" LayerRender_Node"));
            strSQL.Append(_T(" ( "));
            strSQL.Append(_T("  Layer_On_Off, MarkerIndex,  ColorR,       ColorG,   ColorB, "));
            strSQL.Append(_T("  Alpha,        MarkerSize,   MinScale,     MaxScale, LabelShow,"));
            strSQL.Append(_T("  LabelField,   LabelScaleOn, LabelOnScale, UseType,  TypeID "));
            strSQL.Append(_T(" ) "));
            strSQL.Append(_T(" Values "));
            strSQL.Append(_T(" ( ")); 
            {
                strSQL.AppendFormat(_T("%d, %d, %d, %d, %d, "), oInfo.TLayerOn, oInfo.TMarker, GetRValue(oInfo.TMarkerColor), GetGValue(oInfo.TMarkerColor), GetBValue(oInfo.TMarkerColor));
                strSQL.AppendFormat(_T("%d, %f, %f, %f, %d, "), 0,              oInfo.TMarkerSize,  oInfo.TMinScale, oInfo.TMaxScale, oInfo.TLabelShow);
                strSQL.AppendFormat(_T("'%s', %d, %f, %d, %d  "), oInfo.TLabelField, oInfo.TUseLabelScale, oInfo.TLabelScale, oInfo.TUseDrawType, oInfo.TDrawTypeID);
            }
            strSQL.Append(_T(" ) "));
        }

		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		if (spDBaseConnection)
			spDBaseConnection->RollBack();
		TxLogDebugException();
	}
	catch (...)
	{
		if (spDBaseConnection)
			spDBaseConnection->RollBack();
		TxLogDebugException();
	}
}

void KLayerXmlRenderer::LoadNodeProperty(KNodeLayerDispInfo& oInfo)
{
	// Default
    oInfo.TLayerOn       = 1;
    oInfo.TMarker        = 48;
    oInfo.TMarkerColor   = RGB(192, 192, 912);
    oInfo.TMarkerSize    = 4.0;	
    oInfo.TMinScale      = 0.0;
    oInfo.TMaxScale      = 0/*2000000000*/;
    oInfo.TLabelShow     = 0;
    oInfo.TLabelField    = _T("");
    
    oInfo.TUseLabelScale = 0;
    oInfo.TLabelScale    = 2000000000;
    oInfo.TUseDrawType   = 0;
    oInfo.TDrawTypeID    = 1;
	try
	{
		TxLogDebugStartMsg();

		KDBaseConPtr spDBaseConnetion = m_pTarget->GetDBaseConnection();

		CString strSQL(_T("")); {
            strSQL.Append(_T(" Select "));
            strSQL.Append(_T("  Layer_On_Off, MarkerIndex,  ColorR,       ColorG,   ColorB, "));
            strSQL.Append(_T("  Alpha,        MarkerSize,   MinScale,     MaxScale, LabelShow,"));
            strSQL.Append(_T("  LabelField,   LabelScaleOn, LabelOnScale, UseType,  TypeID "));
            strSQL.Append(_T(" From LayerRender_Node "));
        }		

		KResultSetPtr spResult = spDBaseConnetion->ExecuteQuery(strSQL);

		if( spResult->Next() ) {
            oInfo.TLayerOn = spResult->GetValueInt(0);
			oInfo.TMarker  = spResult->GetValueInt(1); 
            if (true) {
                int nColorR = spResult->GetValueInt(2);
                int nColorG = spResult->GetValueInt(3);
                int nColorB = spResult->GetValueInt(4); 
                int nAlpha  = spResult->GetValueInt(5); 
                oInfo.TMarkerColor   = RGB(nColorR, nColorG, nColorB);                
            }		

            oInfo.TMarkerSize    = spResult->GetValueDbl( 6);
			//oInfo.TMinScale      = spResult->GetValueDbl( 7);
			//oInfo.TMaxScale      = spResult->GetValueDbl( 8);
			oInfo.TLabelShow     = spResult->GetValueInt( 9);
			oInfo.TLabelField    = spResult->GetValueStr(10);
			oInfo.TUseLabelScale = 0; /*spResult->GetValueInt(11);*/
			oInfo.TLabelScale    = spResult->GetValueDbl(12);
			oInfo.TUseDrawType   = spResult->GetValueInt(13);
            oInfo.TDrawTypeID    = spResult->GetValueInt(14);

            //if (oInfo.TUseLabelScale == 1)

		} else {
			SaveNodeProperty(oInfo);
		}

		TxLogDebugEndMsg();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KLayerXmlRenderer::SaveZoneProperty( TZoneDispInfo& oInfo )
{
	KDBaseConPtr spDBaseConnection;

	try
	{
		spDBaseConnection = m_pTarget->GetDBaseConnection();
		spDBaseConnection->BeginTransaction();

		CString strSQLDelete(_T(" Delete From LayerRender_Zone "));
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		CString strSQLInsert(_T(" Insert Into LayerRender_Zone "));

		strSQLInsert.Append (_T(" (FillColorR, FillColorG, FillColorB,  "));
		strSQLInsert.Append (_T("  LineColorR, LineColorG, LineColorB,  "));
		strSQLInsert.Append (_T("  LineWidth,  MinScale,   MaxScale, Layer_On_Off ) "));

		strSQLInsert.AppendFormat(_T(" Values ( %d, %d, %d, "),  (int)GetRValue(oInfo.TFillColor), (int)GetGValue(oInfo.TFillColor), (int)GetBValue(oInfo.TFillColor));
		strSQLInsert.AppendFormat(_T(" %d, %d, %d, "),           (int)GetRValue(oInfo.TLineColor), (int)GetGValue(oInfo.TLineColor), (int)GetBValue(oInfo.TLineColor));
		strSQLInsert.AppendFormat(_T(" %.1f, %f, %f, %d ) "),    oInfo.TLineWidth, oInfo.TMinScale, oInfo.TMaxScale, oInfo.TLayerOn);

		spDBaseConnection->ExecuteUpdate(strSQLInsert);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		if (spDBaseConnection)
			spDBaseConnection->RollBack();
		TxLogDebugException();
	}
	catch (...)
	{
		if (spDBaseConnection)
			spDBaseConnection->RollBack();
		TxLogDebugException();
	}
}

void KLayerXmlRenderer::LoadZoneProperty(TZoneDispInfo& oInfo )
{
	// Default
    oInfo.TLayerOn   = 1;
	oInfo.TFillColor = RGB(253, 250, 246);
	oInfo.TLineColor = RGB(160, 172, 93);
	oInfo.TLineWidth = 1.0;	
	oInfo.TMinScale  = 0.0;
	oInfo.TMaxScale  = 0.0;

	try
	{
		TxLogDebugStartMsg();

		KDBaseConPtr spDBaseConnetion = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Append(_T(" SELECT FillColorR, FillColorG, FillColorB, "));
		strSQL.Append(_T("        LineColorR, LineColorG, LineColorB, "));
		strSQL.Append(_T("        LineWidth,  MinScale,   MaxScale, Layer_On_Off "));
		strSQL.Append(_T(" FROM LayerRender_Zone "));

		KResultSetPtr spResult = spDBaseConnetion->ExecuteQuery(strSQL);

		if( spResult->Next() ) {
			int nColorR      = spResult->GetValueInt   (0);
			int nColorG      = spResult->GetValueInt   (1);
			int nColorB      = spResult->GetValueInt   (2);
			oInfo.TFillColor = RGB(nColorR, nColorG, nColorB);

			nColorR          = spResult->GetValueInt   (3);
			nColorG          = spResult->GetValueInt   (4);
			nColorB          = spResult->GetValueInt   (5);
			oInfo.TLineColor = RGB(nColorR, nColorG, nColorB);

			oInfo.TLineWidth = spResult->GetValueDouble(6);
			oInfo.TMinScale  = spResult->GetValueDouble(7);
			oInfo.TMaxScale  = 0;/*spResult->GetValueDouble(8);*/
            oInfo.TLayerOn   = spResult->GetValueInt(9);
		} else {
			SaveZoneProperty(oInfo);
		}

		TxLogDebugEndMsg();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

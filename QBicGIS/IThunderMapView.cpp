#include "stdafx.h"
#include "IThunderMapView.h"

EnumTxMapCommand IThunderMapView::ThunderMapCommand()
{
	EnumTxMapCommand emMapCommand = MapUserCommand;

	ThunderMapPtr spThunderMap = GetThunderMap();
	if (spThunderMap)
	{
		emMapCommand = spThunderMap->MapCommand();
	}

	return emMapCommand;
}

void IThunderMapView::MapCommandZoomIn()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->MapCommand(ZoomInCommand);
    }
}

void IThunderMapView::MapCommandZoomOut()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->MapCommand(ZoomOutCommand);
    }
}

void IThunderMapView::MapCommandPan()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->MapCommand(PanCommand);
    }
}

void IThunderMapView::MapCommandZoomExtent( const TxEnvelope& _oEnvelope )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->SetMapExtent(_oEnvelope);
    }
}


void IThunderMapView::MapRefresh()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->Refresh();
    }
}

void IThunderMapView::MapCommandDispUndo()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->MapDisplayUndo();
    }
}

void IThunderMapView::MapCommandDispRedo()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->MapDisplayRedo();
    }
}

bool IThunderMapView::MapCanDispUndo()
{
	bool bCanUndo(true);

	ThunderMapPtr spThunderMap = GetThunderMap();
	if (spThunderMap)
	{
		bCanUndo = spThunderMap->CanMapDisplayUndo();
	}

	return bCanUndo;
}

bool IThunderMapView::MapCanDispRedo()
{
	bool bCanRedo(true);

	ThunderMapPtr spThunderMap = GetThunderMap();
	if (spThunderMap)
	{
		bCanRedo = spThunderMap->CanMapDisplayRedo();
	}

	return bCanRedo;
}

double IThunderMapView::MapGetScale()
{
	double dScale(0.0);

	ThunderMapPtr spThunderMap = GetThunderMap();
	if (spThunderMap)
	{
		dScale = spThunderMap->GetMapDisplayScale();
	}

	return dScale;
}

double IThunderMapView::MapGetPixelToDist()
{
	double dPixelToDist(0.0);

	ThunderMapPtr spThunderMap = GetThunderMap();
	if (spThunderMap)
		dPixelToDist = spThunderMap->GetMapDisplayPixelToDist();

	return dPixelToDist;
}

void IThunderMapView::MapSetScale(double dScale)
{
	ThunderMapPtr spThunderMap = GetThunderMap();
	if (spThunderMap)
	{
		spThunderMap->SetMapDisplayScale(dScale);
	}
}

int IThunderMapView::MapAddLayer( ITxLayerPtr spLayer)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    return spThunderMap->AddLayer(spLayer);
}

int IThunderMapView::MapAddLayer( ITxLayerPtr spLayer, int nLayerID, int nLayerOrder/* = -1*/ )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    return spThunderMap->AddLayer(spLayer, nLayerID, nLayerOrder);
}

ITxLayerPtr IThunderMapView::MapGetLayer( int nLayerID )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    return spThunderMap->GetLayer(nLayerID);
}

ITxFeatureLayerPtr IThunderMapView::MapGetFeatureLayer( int nLayerID )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    ITxLayerPtr spLayer = spThunderMap->GetLayer(nLayerID);
    if (spLayer == nullptr)
        return nullptr;

    if (spLayer->IsFeatureLayer() == false)
        return nullptr;

    ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<ITxFeatureLayer>(spLayer);
    return spFLayer;
}

ITxFeatureUserLayerPtr IThunderMapView::MapGetFeatureUserLayer( int nLayerID )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    ITxLayerPtr spLayer = spThunderMap->GetLayer(nLayerID);
    if (spLayer == nullptr)
        return nullptr;

    if (spLayer->IsFeatureLayer() == false)
        return nullptr;

    ITxFeatureUserLayerPtr spFLayer = std::dynamic_pointer_cast<ITxFeatureUserLayer>(spLayer);
    return spFLayer;
}

TxFeatureFileUserLayerPtr IThunderMapView::MapGetFileUserLayer( int nLayerID )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    ITxLayerPtr spLayer = spThunderMap->GetLayer(nLayerID);
    if (spLayer == nullptr)
        return nullptr;

    if (spLayer->IsFeatureLayer() == false)
        return nullptr;

    TxFeatureFileUserLayerPtr spULayer = std::dynamic_pointer_cast<TxFeatureFileUserLayer>(spLayer);
    return spULayer;
}

void IThunderMapView::MapRemoveLayer( int nLayerID )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->RemoveLayer(nLayerID);
}


void IThunderMapView::MapMoveLayerOrder( int nLayerID, int nLayerOrder /*= 0 */ )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->MoveLayerOrder(nLayerID, nLayerOrder);
}

void IThunderMapView::MapLayerOrderByID()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->LayerOrderByID();
}

TxFeatureUserLayerPtr IThunderMapView::MapAddForegroundLayer(std::vector<TxFeatureDispPtr>& vecFeatureDisp, bool bShowLabel, double dDrawLabelMaxScale)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    return spThunderMap->AddForegroundLayer(vecFeatureDisp, bShowLabel, dDrawLabelMaxScale);
}

TxFeatureUserLayerPtr IThunderMapView::MapAddForegroundLayer( TxFeatureDispPtr spFeatureDisp, bool bShowLabel, double dDrawLabelMaxScale)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    return spThunderMap->AddForegroundLayer(spFeatureDisp, bShowLabel, dDrawLabelMaxScale);
}

void IThunderMapView::MapForegroundLayer( ITxFeatureUserLayerPtr spLayer, int nZorder, bool bFlash )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->SetForegroundLayer(spLayer, nZorder, bFlash);
}

void IThunderMapView::MapClearForegroundLayer()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->ClearForegroundLayer();
}

void IThunderMapView::MapRemoveForegroundLayer( int nZorder )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->RemoveForegroundLayer(nZorder);
}

void IThunderMapView::MapForegroundRefresh()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap == nullptr)
        throw 1;

    spThunderMap->ForegroundRefresh();
}

void IThunderMapView::MapLayerLabel( int nLayerID, bool bShow, bool bRefresh)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (nullptr == spThunderMap)
        return;

    ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);
    if (spTxLayer) {
		if ( spTxLayer->IsFeatureLayer() ) {
			ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer> (spTxLayer);
			spFeatureLayer->ShowLabel(bShow);
		}
	}

	if (bRefresh)
		MapRefresh();
}

void IThunderMapView::MapShowLayer( int nLayerID, bool bShow )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (nullptr == spThunderMap)
        return;

    ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);
    if (spTxLayer) 
        spTxLayer->LayerOn(bShow);
}

void IThunderMapView::MapShowLayer( int nLayerID, bool bShow, double a_dMinDrawScale, double a_dMaxDrawScale )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (nullptr == spThunderMap)
        return;

    ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);

    if (spTxLayer) {
        spTxLayer->LayerOn(bShow);
        spTxLayer->SetDrawMinScale(a_dMinDrawScale);
        spTxLayer->SetDrawMaxScale(a_dMaxDrawScale);
    }
}

void IThunderMapView::MapLayerScale( int nLayerID, double a_dMinDrawScale, double a_dMaxDrawScale )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (nullptr == spThunderMap)
        return;

    ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);

    if (spTxLayer) {
        spTxLayer->SetDrawMinScale(a_dMinDrawScale);
        spTxLayer->SetDrawMaxScale(a_dMaxDrawScale);
    }
}

void IThunderMapView::MapClearLayer(int a_nLayerID)
{
	ThunderMapPtr spThunderMap = GetThunderMap();
	if (nullptr == spThunderMap)
		return;

	ITxLayerPtr spTxLayer = spThunderMap->GetLayer(a_nLayerID);
	if (spTxLayer) {
		if ( spTxLayer->IsFeatureLayer() ) {
			ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer> (spTxLayer);
			spFeatureLayer->ClearFeatureLabel();
			spFeatureLayer->ClearFeatureSymbol();
		}
	}
}

void IThunderMapView::MapSetSymbol( int nLayerID, ITxSymbolPtr spSymbol, bool a_bVisible)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (nullptr == spThunderMap)
        return;

    ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);
	if (spTxLayer) {
		if ( spTxLayer->IsFeatureLayer() ) {
			ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer> (spTxLayer);
			spFeatureLayer->LayerDispSymbol(spSymbol);
			spFeatureLayer->LayerOn(a_bVisible);
		}
	}
}

void IThunderMapView::MapSetSymbol( int nLayerID, __int64 nxFeatureID, ITxSymbolPtr spSymbol)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
	if (nullptr == spThunderMap)
		return;

    ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);
	if (spTxLayer) {
		if ( spTxLayer->IsFeatureLayer() ) {
			ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer> (spTxLayer);
			spFeatureLayer->FeatureSymbol(nxFeatureID, spSymbol);
		}
	}
}

void IThunderMapView::MapSetLabel( int nLayerID, __int64 nxFeatureID, ITxLabelPtr spLabel )
{
	try
	{
		ThunderMapPtr spThunderMap = GetThunderMap();
		if (nullptr == spThunderMap) return;

		ITxLayerPtr   spTxLayer    = spThunderMap->GetLayer(nLayerID);
		if (spTxLayer) {
			if ( spTxLayer->IsFeatureLayer() ) {
				ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer> (spTxLayer);
				spFeatureLayer->FeatureLabel(nxFeatureID, spLabel);
			}
		}
	}
	catch (...)
	{        
		TxLogDebugException();	
	}
}

void IThunderMapView::MapSetLabelSymbol( int nLayerID, ITxLabelSymbolPtr spLabelSymbol, double dLabelMaxScale )
{
	try
	{
		ThunderMapPtr spThunderMap = GetThunderMap();
		if (nullptr == spThunderMap) return;

		ITxLayerPtr spTxLayer = spThunderMap->GetLayer(nLayerID);
		if (spTxLayer) {
			if ( spTxLayer->IsFeatureLayer() ) {
				ITxFeatureLayerPtr spFeatureLayer = std::dynamic_pointer_cast<ITxFeatureLayer> (spTxLayer);
				spFeatureLayer->LabelSymbol(spLabelSymbol);
				spFeatureLayer->DrawLabelMaxScale(dLabelMaxScale);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void IThunderMapView::MapGetEnvelope(TxEnvelope &oEnvelope)
{
	try
	{
		ThunderMapPtr spThunderMap = GetThunderMap();
		if (nullptr == spThunderMap) return;

		oEnvelope = spThunderMap->GetMapDisplayEnvelope();
	}
	catch (...)	{ TxLogDebugException();	}
}

void IThunderMapView::MapBusinessMode()
{
    // ThunderMap과 MapView의 모드를 동기화한다.
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap)
    {
        spThunderMap->MapCommand(MapUserCommand);
    }
}

void IThunderMapView::Flash( std::vector<TxFeatureDispPtr>& vecFeatureDisp, int nTimeEllipse, int nCount)
{
    try
    {
        ThunderMapPtr spThunderMap = GetThunderMap();
        if (nullptr == spThunderMap) return;

        spThunderMap->Flash(vecFeatureDisp, nTimeEllipse, nCount);
    }
    catch (...)	{ TxLogDebugException();	}
}

TxMapLayerManagerPtr IThunderMapView::MapGetLayerManager()
{
    try
    {
        ThunderMapPtr spThunderMap = GetThunderMap();
        if (nullptr == spThunderMap) 
            return nullptr;

        return spThunderMap->LayerManager();
    }
    catch (...)	{ TxLogDebugException();	}

    return nullptr;
}

std::vector<ITxLayerPtr> IThunderMapView::MapLayers()
{
    std::vector<ITxLayerPtr> vecLyr;

    try
    {
        ThunderMapPtr spThunderMap = GetThunderMap();
        if (nullptr == spThunderMap)  {            
            return vecLyr;
        }

        TxMapLayerManagerPtr spLyrMgr = spThunderMap->LayerManager();        
        return spLyrMgr->Layers();
    }
    catch (...)	{ TxLogDebugException();	}

    return vecLyr;
}

size_t IThunderMapView::MapGetFullExtent( TxEnvelope& oEnvelope )
{
    size_t nxEntityCnt(0);
    if (true) {//지도-Blank(상태)일 수 있다.
        oEnvelope.SetEnvelope(100000, 100000, 200000, 200000);
    }    

    std::vector<ITxLayerPtr> vecLayer = MapLayers();
    
    for (size_t i=0; i<vecLayer.size(); i++) 
    {
        ITxLayerPtr spILayer = vecLayer[i];
        if (spILayer->IsFeatureLayer() == false)
            continue;

        ITxFeatureLayerPtr spLayer = std::dynamic_pointer_cast<ITxFeatureLayer>(spILayer);
        size_t nxCnt = (size_t)spLayer->EntityCount();
        
        if (nxCnt > 0) {
            TxEnvelope oEnv = spLayer->Extent();
            
            if (nxEntityCnt == 0) {//First
                oEnvelope = oEnv;
            } else {
                oEnvelope.Union(oEnv);
            }

            nxEntityCnt += nxCnt;
        }
    }
    
    return nxEntityCnt;
}

bool IThunderMapView::MapWriteImage(CString strPath)
{
    try
    {
        ThunderMapPtr spThunderMap = GetThunderMap();
        if (nullptr == spThunderMap) 
            return false;

        return spThunderMap->MapWriteImage(strPath);        
    }
    catch (...)	{ 
        TxLogDebugException();	
    }

    return false;
}

void IThunderMapView::ShowLoadingMessage( CString strMsg /*= _T("Loading...")*/ )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap) {
        spThunderMap->ShowLoadingMessage(strMsg);
    }
}

void IThunderMapView::HideLoadingMessage()
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap) {
        spThunderMap->HideLoadingMessage();
    }
}

void IThunderMapView::MapDrawOptionFlicker( bool bFlicker )
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap) {
        spThunderMap->MapDrawOptionFlicker(bFlicker);
    }
}

void IThunderMapView::MapTooltipEnable(bool bEnable)
{
    ThunderMapPtr spThunderMap = GetThunderMap();
    if (spThunderMap) {
        spThunderMap->EnableMapTooltip(bEnable);
    }
}

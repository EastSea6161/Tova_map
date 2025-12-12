#pragma once
#include "TxLayer.h"
#include "ITxFeatureLayer.h"

class AFX_EXT_CLASS TxFeatureUserLayer : public ITxFeatureLayer
{
public:
    TxFeatureUserLayer(TxLayerInfoPtr spLayerInfo);
    virtual ~TxFeatureUserLayer();

private:
    virtual int Initialize();
protected:
    virtual void Clear();
protected:
    ITxSpatialIndexPtr m_spSpatialIndex;
protected:
    std::map<__int64, TxFeatureDispPtr> m_mapFeatureDispData;
public:
    void PutFeatureDisp(std::vector<TxFeatureDispPtr>& _vecFeautre);
private:
    virtual TxFeatureDispPtr GetFeatureDisp(__int64 nxID);
public:
    virtual TxFeaturePtr GetFeature(__int64 nxID);
    virtual void FindFeature( TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature );
protected:
    virtual void FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent);
public:
    virtual void LayerOnOff(bool bVisible);
    virtual void LayerDispSymbol(ITxSymbolPtr spSymbol);

//*******************************************************************************
public:
    virtual void FeatureSymbol(__int64 nxID, ITxSymbolPtr spSymbol);
    virtual void ClearFeatureSymbol();
public:
    virtual void FeatureLabel(__int64 nxID, ITxLabelPtr spLabel);
    virtual void ClearFeatureLabel();
};

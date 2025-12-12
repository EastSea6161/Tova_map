#pragma once

#include "IncludeShapeEdit.h"
#include "KMapEvents.h"

class KMapView;

class KShapeEditController : public KMapEvents, public IShapeEditController
{
public:
    KShapeEditController(KMapView* a_pMapView);
    ~KShapeEditController();
public:
    void         StartMapEditing  ( );
    virtual void StartOperation   ();
    
    virtual void AbortOperation   ();
    
    void         StopMapEditing   ();    
	virtual void Sleep();
	virtual WORD GetCursorID();
	virtual void OnShapeEditPopupCommand(UINT nID);
	virtual bool GetShapeEditPopupInfo(UINT&  nMenuID, int& nPos);
private:
    void RefreshTargetTask();
public:
    void SetEditingTarget ( KEMShapeEditingTarget a_emShapeEditingLayer);
    KEMShapeEditingTarget GetEditingTarget() 
    {
        return m_emShapeEditingTarget;
    }

    void  SetEditingTask (KEMShapeEditingTask a_emShapeEditingTask);
    KEMShapeEditingTask   GetEditingTask()
    {
        return m_emShapeEditingTask;
    }
public:
protected:
    KShapeEditPtr          m_spShapeEdit;
    KEMShapeEditingTarget  m_emShapeEditingTarget;
    KEMShapeEditingTask    m_emShapeEditingTask;
private:
    KMapView*              m_pMapView;
    
public:
    virtual double           GetPixelToRealDist(double a_dPixelCount);

private:
    double m_dDownMapX;
    double m_dDownMapY;
private:
	std::map<CString/*Key*/, CString/*Value*/> m_mapDefaultNodeInfo;
	std::map<CString/*Key*/, CString/*Value*/> m_mapDefaultLinkInfo;
	std::map<CString/*Key*/, CString/*Value*/> m_mapDefaultZoneInfo;
	std::map<CString/*Key*/, CString/*Value*/> m_mapDefaultTurnInfo;
    std::map<CString/*Key*/, CString/*Value*/> m_mapDefaultTransitInfo;
public:
	/*virtual*/ void SetDefaultNodeInfo(std::map<CString, CString>& ar_mapKeyValue);
	/*virtual*/ void SetDefaultLinkInfo(std::map<CString, CString>& ar_mapKeyValue);
	/*virtual*/ void SetDefaultZoneInfo(std::map<CString, CString>& ar_mapKeyValue);
	/*virtual*/ void SetDefaultTurnInfo(std::map<CString, CString>& ar_mapKeyValue);
    /*virtual*/ void SetDefaultTransitInfo(std::map<CString, CString>& ar_mapKeyValue);
public:
	/*virtual*/ CString GetDefaultNodeValue(CString a_strKey);
	/*virtual*/ CString GetDefaultLinkValue(CString a_strKey);
	/*virtual*/ CString GetDefaultZoneValue(CString a_strKey);
	/*virtual*/ CString GetDefaultTurnValue(CString a_strKey);
    /*virtual*/ CString GetDefaultTransitValue(CString a_strKey);

    std::map<CString/*Key*/, CString/*Value*/> GetDefaultTransitValue()
    {
        return m_mapDefaultTransitInfo;
    }

private:
	Integer m_nxEditTransitID;
public:
	virtual void SetEditID(Integer a_nxEditTransitID)
	{
		m_nxEditTransitID = a_nxEditTransitID;
	}

public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
};

typedef std::shared_ptr<KShapeEditController> KShapeEditControllerPtr;
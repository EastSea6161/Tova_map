#pragma once

enum KEMShapeEditingTarget
{
    KEMShapeEditingLayerNothing     = 0, 
    KEMShapeEditingLayerNode,        
    KEMShapeEditingLayerLink,       
    KEMShapeEditingLayerDemarcation, 
    KEMShapeEditingLayerTurn,
	KEMShapeEditingLayerTransit
};

enum KEMShapeEditingTask
{
    KEMShapeEditingTaskNothing      = 0, 
    KEMShapeEditingTaskInsert       = 1,
    KEMShapeEditingTaskMultiSelect  = 2,
	KEMShapeEditingTaskSingleEdit   = 3,
	KEMShapeEditingTaskLinkOneWayInsert = 4,
	KEMShapeEditingTaskLinkTwoWayInsert = 5,
    KEMShapeEditingTaskDelete,
    KEMShapeEditingTaskMove,
    KEMShapeEditingTaskSingleDel,
    KEMShapeEditingTaskLinkOneWayVertex,
    KEMShapeEditingTaskLinkTwoWayVertex
};

class IShapeEditController
{
public:
    IShapeEditController          () {}
    virtual ~IShapeEditController () {}
public:
    virtual void StartOperation () = 0;
    
    virtual void AbortOperation () = 0;
	virtual void Sleep()       = 0;
	virtual WORD GetCursorID() = 0;
	virtual void OnShapeEditPopupCommand(UINT nID) = 0;
	virtual bool GetShapeEditPopupInfo(UINT&  nMenuID, int& nPos) = 0;
public:
    
public:
    virtual double  GetPixelToRealDist(double a_dPixelCount) = 0;
public:
	virtual CString GetDefaultNodeValue(CString a_strKey) = 0;
	virtual CString GetDefaultLinkValue(CString a_strKey) = 0;
	virtual CString GetDefaultZoneValue(CString a_strKey) = 0;
	virtual CString GetDefaultTurnValue(CString a_strKey) = 0;
    virtual std::map<CString,CString> GetDefaultTransitValue() = 0;
};
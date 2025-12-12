#pragma once

enum EnumTxMapCommand
{    
    MapUserCommand,
    ZoomInCommand,
    ZoomOutCommand,
    PanCommand,
    MPanCommand    
};

enum EnumTxGeoType
{
    GeoShapeType     = 0,
    GeoPointType     = 1,
    GeoPolylineType  = 3,
    GeoPolygonType   = 4,
    GeoMPolygonType  = 5,
    GeoMPointType    = 8,
    GeoMPolylineType = 9,
    GeoImageMap      = 1000
};

enum EnumTxMapDataServerType
{
    TxMapDataServerSqlite,
	TxMapDataServerSQL
    //TxMapDataServerMemory,
    //TxMapDataServerFile
};

enum EnumTxDirectionFlag
{
    EnumForward,
    EnumBackward
};

enum EnumTxLabelType
{
    TxLabelType   = 0,
    TxDirectionLabelType  = 1
};
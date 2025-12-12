#pragma once

class QBicPoint;

class KGeoTurnDisplayRect
{
public:
    KGeoTurnDisplayRect() 
    {
        m_nXMin = 0; m_nYMin = 0; m_nXMax = 0; m_nYMax = 0;
    }
    ~KGeoTurnDisplayRect(){}
public:
    int m_nXMin;
    int m_nYMin;
    int m_nXMax;
    int m_nYMax;
public:
    int GetCenterX()
    {
        return (m_nXMax - m_nXMin) / 2;
    }

    int GetCenterY()
    {
        return (m_nYMax - m_nYMin) / 2;
    }
public:
    bool IntersectLine(const QBicPoint& ar_oP1, const QBicPoint& ar_oP2, QBicPoint& ar_oResult);
};

class TDrawNodeXY
{
public:
    TDrawNodeXY() 
    {
        NodeID = 0;
        Label  = _T("");
        X = 0.0;
        Y = 0.0;
        GeoScaleX = 0;
        GeoScaleY = 0;
        Direction = 0.0;
        RotatedDirection = 0.0;
    }
    ~TDrawNodeXY(){}
public:
    Integer NodeID;
    CString Label;
    double  X;
    double  Y;

    int     GeoScaleX;
    int     GeoScaleY;
    double  Direction;
private:
    double  RotatedDirection;
public:
    void SetRotateDirection(double dDirection)
    {
        RotatedDirection = Direction - dDirection;
        if (RotatedDirection < 0)
            RotatedDirection += 360.0;
    }

    double GetRotateDirection()
    {
        return RotatedDirection;
    }

public:
    static bool RotateDirectionAscennding( TDrawNodeXY& left, TDrawNodeXY& right )
    {
        return left.RotatedDirection < right.RotatedDirection;
    }

    static bool DirectionAscennding(TDrawNodeXY& left, TDrawNodeXY& right)
    {
        return left.Direction < right.Direction;
    }
};

class TDrawTurnVolume
{
public:
    TDrawTurnVolume()  
    {
        Label = _T("");
        DisplayWidth = 1.0;
        LineColor = Gdiplus::Color(100, 0, 0, 0);
    };
    ~TDrawTurnVolume() {}
public:
    Integer  FromNodeID;
    Integer  ToNodeID;
    CString  Label;
    double   DisplayWidth;
    Gdiplus::Color LineColor;
};

class KGraphicTurnVolumeDisp 
{
public:
    KGraphicTurnVolumeDisp(bool bDrawAllPath=false, bool bDrawCenterLine=true);
    ~KGraphicTurnVolumeDisp(void);

private:
    int  m_nNodeSize;
    int  m_nRoadWidth;
    bool m_bDrawAllPath;
    bool m_bDrawCenterLine;
    double m_dOffset;
public:
    void DrawTurnVolume(CWnd* pStaticView, CRect oViewRect);
private:
    void DrawStaticViewBackGround(Graphics& graphics);
    void CalculatePosition();
    void RotateSmallGap();

    void OptimizeOffset();
    void OptimizeOffset(TDrawNodeXY ar_oDrawNode, std::vector<TDrawNodeXY>& vecDrawNodeInfo);
    void DrawCenterLine(Graphics& graphics);
    void DrawNode(Graphics& graphics);
    void DrawVolume(Graphics& graphics);
    void DrawVolume(Graphics& graphics, TDrawNodeXY oDrawNode, std::vector<TDrawNodeXY>& vecDrawNodeInfo);
    void DrawVolumeLabel(Graphics& graphics, QBicPoint oLabelPt, double dAzimuth, CString strLabel, bool bStart=true);
    bool FindData(Integer nFromNodeID, Integer nToNodeID, TDrawTurnVolume& ar_oDrawTurnVolume);
    void DrawRoundRect(Graphics& graphics, SolidBrush& oFillBrush, Pen& oPen, Gdiplus::REAL X, Gdiplus::REAL Y, Gdiplus::REAL width, Gdiplus::REAL height, Gdiplus::REAL radius);
private:
    TDrawNodeXY m_oCenterNodeXY;
    std::vector<TDrawTurnVolume> m_vecDrawTurnVolume;
    std::map<Integer, TDrawNodeXY>   m_mapNodes;
private:
    CRect m_oTurnViewRect;
    CRect m_oDisplayRect;
    KGeoTurnDisplayRect m_oGeoDisplayRect;
public:
    void InitialData(TDrawNodeXY& ar_oDrawNodeXY, std::vector<TDrawNodeXY>& ar_vecNodes, std::vector<TDrawTurnVolume>& ar_vecDrawTurnVolume);
public:
    bool IsDrawAllPath()
    {
        return m_bDrawAllPath;
    }
    void SetDrawAllPath(bool bDraw=true)
    {
        m_bDrawAllPath = bDraw;
    }
public:
    bool IsDrawCenterLine()
    {
        return m_bDrawCenterLine;
    }
    void SetDrawCenterLine(bool bDraw=true)
    {
        m_bDrawCenterLine = bDraw;
    }
public:
    double GetMaxWidth()
    {
        return m_dOffset;
    }    
private:
    std::vector<Gdiplus::Color> m_vecColor;
private:
    bool m_bDrawLabel;
    RectF m_oLabelRect;
public:
    void SetLabel(bool bLabel=true)
    {
        m_bDrawLabel = bLabel;
    }
	bool IsDrawLabel()
	{
		return m_bDrawLabel;
	}
};


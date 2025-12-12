#pragma once

class TCompareNodeField 
{
public:
    TCompareNodeField () {};
    ~TCompareNodeField() {};
public:
    Integer TnxNodeID  ; 
    int     TnNodeType ;
    double  TdX        ; 
    double  TdY        ; 
    Integer TnxZoneID  ; 
    CString TstrName;
	int     TnTargetRegion;
	double  TdParkingFee;

public:
    CString GetUpdateSQL()
    {
        CString strSQL(_T(""));
        try
        {
        	strSQL.Format(_T(" Update %s Set %s='%d', %s='%I64d', %s='%s', target_region = '%d', parking_fee = '%f'  Where %s='%I64d' "), TABLE_NODE, COLUMN_NODE_TYPE, TnNodeType, COLUMN_ZONE_ID, TnxZoneID, COLUMN_NODE_NAME, TstrName, TnTargetRegion, TdParkingFee, COLUMN_NODE_ID, TnxNodeID);
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
        	TxLogDebugException();
        }
        return strSQL;
    }

    CString GetInsertSQL()
    {
        CString strSQL(_T(""));
        try
        {
            strSQL.AppendFormat(_T(" Insert Into %s(%s, %s, %s, %s, %s, %s, target_region, parking_fee)"), TABLE_NODE, COLUMN_NODE_ID, COLUMN_NODE_TYPE, COLUMN_NODE_X, COLUMN_NODE_Y, COLUMN_ZONE_ID, COLUMN_NODE_NAME);
            strSQL.AppendFormat(_T("    Values ( '%I64d', '%d', '%f', '%f', '%I64d', '%s', '%d', '%f' ) "), TnxNodeID, TnNodeType, TdX, TdY, TnxZoneID, TstrName, TnTargetRegion, TdParkingFee);
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }
        return strSQL;
    }    

    TxRecord GetInsertRecord() 
    {
        TxRecord oRecord; {
            oRecord.AddItem(COLUMN_NODE_ID,   TnxNodeID);
            oRecord.AddItem(COLUMN_NODE_TYPE, TnNodeType);
            oRecord.AddItem(COLUMN_NODE_X,    TdX);
            oRecord.AddItem(COLUMN_NODE_Y,    TdY);
            oRecord.AddItem(COLUMN_ZONE_ID,   TnxZoneID);
            oRecord.AddItem(COLUMN_NODE_NAME, TstrName);
            oRecord.AddItem(_T("target_region"), TnTargetRegion);
            oRecord.AddItem(_T("parking_fee"),   TdParkingFee);
        }

        return oRecord;
    }
public:
    bool CompareGeometry(TCompareNodeField& ar_right)
    {
        try
        {
            double dDist(0.0);
            dDist = TxMath::Dist(TdX, TdY, ar_right.TdX, ar_right.TdY);

            if (dDist <0.5)
                return true;
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }        

        return false;
    }

    bool CompareAttribute(TCompareNodeField& ar_right)
    {
        try
        {
            if (TnxNodeID  != ar_right.TnxNodeID)
                return false;
            if (TnNodeType != ar_right.TnNodeType)
                return false;
            if (TnxZoneID  != ar_right.TnxZoneID)
                return false;
            if (_tcsicmp(TstrName, ar_right.TstrName) != 0)
                return false;
			if (TnTargetRegion != ar_right.TnTargetRegion)
				return false;
			if (TdParkingFee != ar_right.TdParkingFee)
				return false;
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }

        return true;
    }
};

class TCompareLinkField 
{
public:
    TCompareLinkField () {};
    ~TCompareLinkField() {};
public:
    Integer TnxLinkID     ; 
    Integer TnxFNodeID    ; 
    Integer TnxTNodeID    ; 
    CString TstrName      ;
    int     TnLinkType    ;
    double  TdLength      ; 
    double  TdLane        ; 
    double  TdCapa        ; 
    double  TdSpeed       ;
    double  TdSurveyVolume;
    int     TnFunctionID  ;
    double  TdAlpha       ;
    double  TdBeta        ;
    double  TdA           ;
    double  TdB           ;
public:
    CString GetUpdateSQL()
    {
        CString strSQL(_T(""));
        try
        {
            strSQL.AppendFormat(_T(" Update %s "), TABLE_LINK);
            strSQL.AppendFormat(_T("  Set %s = '%I64d', %s = '%I64d', "), COLUMN_FNODE_ID,    TnxFNodeID, COLUMN_TNODE_ID,   TnxTNodeID);
            strSQL.AppendFormat(_T("      %s = '%s',    %s = '%d',    "), COLUMN_LINK_NAME,   TstrName,   COLUMN_LINK_TYPE,  TnLinkType);
            strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f',    "), COLUMN_LINK_LENGTH, TdLength,   COLUMN_LINK_LANE,  TdLane);
            strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f',    "), COLUMN_LINK_CAPA,   TdCapa,     COLUMN_LINK_SPEED, TdSpeed);
            strSQL.AppendFormat(_T("      %s = '%f',    %s = '%d',    "), COLUMN_LINK_SURVEYVOLUME,TdSurveyVolume, COLUMN_LINK_FN_ID, TnFunctionID);
            strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f',    "), COLUMN_LINK_ALPHA,  TdAlpha,    COLUMN_LINK_BETA,  TdBeta);
            strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f'     "), COLUMN_LINK_A,      TdA,        COLUMN_LINK_B,     TdB);
            strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_LINK_ID, TnxLinkID);
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }
        return strSQL;
    }

    CString GetInsertSQL()
    {
        CString strSQL(_T(""));
        try
        {
            strSQL.AppendFormat(_T(" Insert Into %s "), TABLE_LINK);
            strSQL.AppendFormat(_T("  (%s, "), COLUMN_LINK_ID);
            strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_FNODE_ID,    COLUMN_TNODE_ID  );
            strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_NAME,   COLUMN_LINK_TYPE );
            strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_LENGTH, COLUMN_LINK_LANE );
            strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_CAPA,   COLUMN_LINK_SPEED);
            strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_SURVEYVOLUME,COLUMN_LINK_FN_ID);
            strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_ALPHA,  COLUMN_LINK_BETA);
            strSQL.AppendFormat(_T("   %s, %s) "), COLUMN_LINK_A,      COLUMN_LINK_B    );
            strSQL.Append(_T(" Values ("));
            strSQL.AppendFormat(_T(" '%I64d', "), TnxLinkID);
            strSQL.AppendFormat(_T(" '%I64d', '%I64d', "), TnxFNodeID, TnxTNodeID);
            strSQL.AppendFormat(_T(" '%s',    '%d',    "), TstrName,   TnLinkType);
            strSQL.AppendFormat(_T(" '%f',    '%f',    "), TdLength,   TdLane);
            strSQL.AppendFormat(_T(" '%f',    '%f',    "), TdCapa,     TdSpeed);
            strSQL.AppendFormat(_T(" '%f',    '%d',    "), TdSurveyVolume, TnFunctionID);
            strSQL.AppendFormat(_T(" '%f',    '%f',    "), TdAlpha,    TdBeta);
            strSQL.AppendFormat(_T(" '%f',    '%f'     "), TdA,        TdB);
            strSQL.Append(_T("        )"));
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }
        return strSQL;
    }

    TxRecord GetInsertRecord() 
    {
        TxRecord oRecord; {
            oRecord.AddItem(COLUMN_LINK_ID,  TnxLinkID);
            oRecord.AddItem(COLUMN_FNODE_ID, TnxFNodeID);            
            oRecord.AddItem(COLUMN_TNODE_ID, TnxTNodeID);

            oRecord.AddItem(COLUMN_LINK_NAME,   TstrName);
            oRecord.AddItem(COLUMN_LINK_TYPE,   TnLinkType);
            oRecord.AddItem(COLUMN_LINK_LENGTH, TdLength);

            oRecord.AddItem(COLUMN_LINK_LANE,  TdLane);
            oRecord.AddItem(COLUMN_LINK_CAPA,  TdCapa);
            oRecord.AddItem(COLUMN_LINK_SPEED, TdSpeed);

            oRecord.AddItem(COLUMN_LINK_SURVEYVOLUME, TdSurveyVolume);

            oRecord.AddItem(COLUMN_LINK_FN_ID, TnFunctionID);

            oRecord.AddItem(COLUMN_LINK_ALPHA, TdAlpha);
            oRecord.AddItem(COLUMN_LINK_BETA,  TdBeta);

            oRecord.AddItem(COLUMN_LINK_A, TdA);
            oRecord.AddItem(COLUMN_LINK_B, TdB);
        }

        return oRecord;
    }

public:
    bool CompareGeometry(TCompareLinkField& ar_right)
    {
        try
        {
            if (TnxFNodeID == ar_right.TnxFNodeID && TnxTNodeID == ar_right.TnxTNodeID)
                return true;
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }        

        return false;
    }

    bool CompareAttribute(TCompareLinkField& ar_right)
    {
        try
        {
            if (TnxLinkID  != ar_right.TnxLinkID)
                return false;
            if (TnxFNodeID  != ar_right.TnxFNodeID)
                return false;
            if (TnxTNodeID  != ar_right.TnxTNodeID)
                return false;
            if (TstrName  != ar_right.TstrName)
                return false;
            if (TnLinkType  != ar_right.TnLinkType)
                return false;
            
            if (TdLength  != ar_right.TdLength)
            {
                int nTime = (int)(TdLength * 1000.0);
                int rTime = (int)(ar_right.TdLength * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TdLane  != ar_right.TdLane)
                return false;

            if (TdCapa  != ar_right.TdCapa)
            {
                int nTime = (int)(TdCapa * 1000.0);
                int rTime = (int)(ar_right.TdCapa * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TdSpeed  != ar_right.TdSpeed)
            {
                int nTime = (int)(TdSpeed * 1000.0);
                int rTime = (int)(ar_right.TdSpeed * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TdSurveyVolume  != ar_right.TdSurveyVolume)
            {
                int nTime = (int)(TdSurveyVolume * 1000.0);
                int rTime = (int)(ar_right.TdSurveyVolume * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TnFunctionID  != ar_right.TnFunctionID)
                return false;

            if (TdAlpha  != ar_right.TdAlpha)
            {
                int nTime = (int)(TdAlpha * 1000.0);
                int rTime = (int)(ar_right.TdAlpha * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TdBeta  != ar_right.TdBeta)
            {
                int nTime = (int)(TdBeta * 1000.0);
                int rTime = (int)(ar_right.TdBeta * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TdA  != ar_right.TdA)
            {
                int nTime = (int)(TdA * 1000.0);
                int rTime = (int)(ar_right.TdA * 1000.0);

                if (nTime != rTime)
                    return false;
            }

            if (TdB  != ar_right.TdB)
            {
                int nTime = (int)(TdB * 1000.0);
                int rTime = (int)(ar_right.TdB * 1000.0);

                if (nTime != rTime)
                    return false;
            }
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            TxLogDebugException();
        }

        return true;
    }
};

//È­¹°

class TFCompareNodeField 
{
public:
	TFCompareNodeField () {};
	~TFCompareNodeField() {};
public:
	Integer TnxNodeID  ; 
	int     TnNodeType ;
	double  TdX        ; 
	double  TdY        ; 
	Integer TnxZoneID  ; 
	CString TstrName;
	int		TnFacility_type;
	double  TdFacility_Fee;
	double	TdLoading_Fee;
	double  TdUnLoading_Fee;
	CString TstrAvailialbe_item;
	CString	TstrTraffic_mean;
	int		TnOccurrence_via;
	double	TdCapa;

public:
	CString GetUpdateSQL()
	{
		CString strSQL(_T(""));
		try
		{
			strSQL.Format(_T(" Update %s set %s = '%d', "), TABLE_NODE, COLUMN_NODE_TYPE , TnNodeType);
			strSQL.AppendFormat(_T(" %s = '%I64d', "), COLUMN_ZONE_ID , TnxZoneID);
			strSQL.AppendFormat(_T(" %s = '%s', "), COLUMN_NODE_NAME, TstrName);
			strSQL.AppendFormat(_T(" %s = '%d',  "), COLUMN_NODE_FACILITY_TYPE, TnFacility_type);
			strSQL.AppendFormat(_T(" %s = '%f',  "), COLUMN_NODE_FACILITY_FEE, TdFacility_Fee);
			strSQL.AppendFormat(_T(" %s = '%f',  "), COLUMN_NODE_LOADING_FEE, TdLoading_Fee);
			strSQL.AppendFormat(_T(" %s = '%f',  "), COLUMN_NODE_UNLOADING_FEE, TdUnLoading_Fee);
			strSQL.AppendFormat(_T(" %s = '%s',  "), COLUMN_NODE_AVAILABLE_ITEM, TstrAvailialbe_item);
			strSQL.AppendFormat(_T(" %s = '%s',  "), COLUMN_NODE_TRAFFIC_MEAN, TstrTraffic_mean);
			strSQL.AppendFormat(_T(" %s = '%d',  "), COLUMN_NODE_OCCURRENCE_VIA, TnOccurrence_via);
			strSQL.AppendFormat(_T(" %s = '%f'  Where %s = '%I64d'"), COLUMN_NODE_CAPA, TdCapa, COLUMN_NODE_ID, TnxNodeID);
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}
		return strSQL;
	}

	CString GetInsertSQL()
	{
		CString strSQL(_T(""));
		try
		{
			strSQL.AppendFormat(_T(" Insert Into %s( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s )"), TABLE_NODE, 
				COLUMN_NODE_ID, COLUMN_NODE_TYPE, COLUMN_NODE_X, COLUMN_NODE_Y, COLUMN_ZONE_ID, COLUMN_NODE_NAME,
				COLUMN_NODE_FACILITY_TYPE, COLUMN_NODE_FACILITY_FEE, COLUMN_NODE_LOADING_FEE, COLUMN_NODE_UNLOADING_FEE,
				COLUMN_NODE_AVAILABLE_ITEM, COLUMN_NODE_TRAFFIC_MEAN, COLUMN_NODE_OCCURRENCE_VIA, COLUMN_NODE_CAPA);
			
			strSQL.AppendFormat(_T("    Values ( '%I64d', '%d', '%f', '%f', '%I64d', '%s', '%d', '%f', '%f', '%f', '%s', '%s', '%d', '%f') "), 
				TnxNodeID, TnNodeType, TdX, TdY, TnxZoneID, TstrName ,TnFacility_type, TdFacility_Fee, 
				TdLoading_Fee, TdUnLoading_Fee, TstrAvailialbe_item, TstrTraffic_mean, TnOccurrence_via, TdCapa);
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}
		return strSQL;
	}

public:
	bool CompareGeometry(TFCompareNodeField& ar_right)
	{
		try
		{
			double dDist(0.0);
			dDist = TxMath::Dist(TdX, TdY, ar_right.TdX, ar_right.TdY);

			if (dDist <0.5)
				return true;
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}        

		return false;
	}

	bool CompareAttribute(TFCompareNodeField& ar_right)
	{
		try
		{
			if (TnxNodeID  != ar_right.TnxNodeID)
				return false;
			if (TnNodeType != ar_right.TnNodeType)
				return false;
			if (TnxZoneID  != ar_right.TnxZoneID)
				return false;
			if (_tcsicmp(TstrName, ar_right.TstrName) != 0)
				return false;
			if (TnFacility_type != ar_right.TnFacility_type)
			{
				return false;
			}

			if (TdFacility_Fee  != ar_right.TdFacility_Fee)
			{
				int nTime = (int)(TdFacility_Fee * 1000.0);
				int rTime = (int)(ar_right.TdFacility_Fee * 1000.0);

				if (nTime != rTime)
					return false;
			}
			
			if (TdLoading_Fee  != ar_right.TdLoading_Fee)
			{
				int nTime = (int)(TdLoading_Fee * 1000.0);
				int rTime = (int)(ar_right.TdLoading_Fee * 1000.0);

				if (nTime != rTime)
					return false;
			}
			
			if (TdUnLoading_Fee  != ar_right.TdUnLoading_Fee)
			{
				int nTime = (int)(TdUnLoading_Fee * 1000.0);
				int rTime = (int)(ar_right.TdUnLoading_Fee * 1000.0);

				if (nTime != rTime)
					return false;
			}
		
			if (_tcsicmp(TstrAvailialbe_item, ar_right.TstrAvailialbe_item) != 0)
				return false;
			if (_tcsicmp(TstrTraffic_mean, ar_right.TstrTraffic_mean) != 0)
				return false;
			if (TnOccurrence_via  != ar_right.TnOccurrence_via)
				return false;

			if (TdCapa  != ar_right.TdCapa)
			{
				int nTime = (int)(TdCapa * 1000.0);
				int rTime = (int)(ar_right.TdCapa * 1000.0);

				if (nTime != rTime)
					return false;
			}

		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}

		return true;
	}
};

class TFCompareLinkField 
{
public:
	TFCompareLinkField () {};
	~TFCompareLinkField() {};
public:
	Integer TnxLinkID     ; 
	Integer TnxFNodeID    ; 
	Integer TnxTNodeID    ; 
	CString TstrName      ;
	int     TnLinkType    ;
	double  TdLength      ; 
	double  TdLane        ; 
	double  TdCapa        ; 
	double  TdSpeed       ;
	double  TdSurveyVolume;
	int     TnFunctionID  ;
	double  TdAlpha       ;
	double  TdBeta        ;
	double  TdA           ;
	double  TdB           ;
public:
	CString GetUpdateSQL()
	{
		CString strSQL(_T(""));
		try
		{
			strSQL.AppendFormat(_T(" Update %s "), TABLE_LINK);
			strSQL.AppendFormat(_T("  Set %s = '%I64d', %s = '%I64d', "), COLUMN_FNODE_ID,    TnxFNodeID, COLUMN_TNODE_ID,   TnxTNodeID);
			strSQL.AppendFormat(_T("      %s = '%s',    %s = '%d',    "), COLUMN_LINK_NAME,   TstrName,   COLUMN_LINK_TYPE,  TnLinkType);
			strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f',    "), COLUMN_LINK_LENGTH, TdLength,   COLUMN_LINK_LANE,  TdLane);
			strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f',    "), COLUMN_LINK_CAPA,   TdCapa,     COLUMN_LINK_SPEED, TdSpeed);
			strSQL.AppendFormat(_T("      %s = '%f',    %s = '%d',    "), COLUMN_LINK_SURVEYVOLUME,TdSurveyVolume, COLUMN_LINK_FN_ID, TnFunctionID);
			strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f',    "), COLUMN_LINK_ALPHA,  TdAlpha,    COLUMN_LINK_BETA,  TdBeta);
			strSQL.AppendFormat(_T("      %s = '%f',    %s = '%f'     "), COLUMN_LINK_A,      TdA,        COLUMN_LINK_B,     TdB);
			strSQL.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_LINK_ID, TnxLinkID);
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}
		return strSQL;
	}

	CString GetInsertSQL()
	{
		CString strSQL(_T(""));
		try
		{
			strSQL.AppendFormat(_T(" Insert Into %s "), TABLE_LINK);
			strSQL.AppendFormat(_T("  (%s, "), COLUMN_LINK_ID);
			strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_FNODE_ID,    COLUMN_TNODE_ID  );
			strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_NAME,   COLUMN_LINK_TYPE );
			strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_LENGTH, COLUMN_LINK_LANE );
			strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_CAPA,   COLUMN_LINK_SPEED);
			strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_SURVEYVOLUME,COLUMN_LINK_FN_ID);
			strSQL.AppendFormat(_T("   %s, %s, "), COLUMN_LINK_ALPHA,  COLUMN_LINK_BETA);
			strSQL.AppendFormat(_T("   %s, %s) "), COLUMN_LINK_A,      COLUMN_LINK_B    );
			strSQL.Append(_T(" Values ("));
			strSQL.AppendFormat(_T(" '%I64d', "), TnxLinkID);
			strSQL.AppendFormat(_T(" '%I64d', '%I64d', "), TnxFNodeID, TnxTNodeID);
			strSQL.AppendFormat(_T(" '%s',    '%d',    "), TstrName,   TnLinkType);
			strSQL.AppendFormat(_T(" '%f',    '%f',    "), TdLength,   TdLane);
			strSQL.AppendFormat(_T(" '%f',    '%f',    "), TdCapa,     TdSpeed);
			strSQL.AppendFormat(_T(" '%f',    '%d',    "), TdSurveyVolume, TnFunctionID);
			strSQL.AppendFormat(_T(" '%f',    '%f',    "), TdAlpha,    TdBeta);
			strSQL.AppendFormat(_T(" '%f',    '%f'     "), TdA,        TdB);
			strSQL.Append(_T("        )"));
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}
		return strSQL;
	}

public:
	bool CompareGeometry(TFCompareLinkField& ar_right)
	{
		try
		{
			if (TnxFNodeID == ar_right.TnxFNodeID && TnxTNodeID == ar_right.TnxTNodeID)
				return true;
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}        

		return false;
	}

	bool CompareAttribute(TFCompareLinkField& ar_right)
	{
		try
		{
			if (TnxLinkID  != ar_right.TnxLinkID)
				return false;
			if (TnxFNodeID  != ar_right.TnxFNodeID)
				return false;
			if (TnxTNodeID  != ar_right.TnxTNodeID)
				return false;
			if (TstrName  != ar_right.TstrName)
				return false;
			if (TnLinkType  != ar_right.TnLinkType)
				return false;

			if (TdLength  != ar_right.TdLength)
			{
				int nTime = (int)(TdLength * 1000.0);
				int rTime = (int)(ar_right.TdLength * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TdLane  != ar_right.TdLane)
				return false;

			if (TdCapa  != ar_right.TdCapa)
			{
				int nTime = (int)(TdCapa * 1000.0);
				int rTime = (int)(ar_right.TdCapa * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TdSpeed  != ar_right.TdSpeed)
			{
				int nTime = (int)(TdSpeed * 1000.0);
				int rTime = (int)(ar_right.TdSpeed * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TdSurveyVolume  != ar_right.TdSurveyVolume)
			{
				int nTime = (int)(TdSurveyVolume * 1000.0);
				int rTime = (int)(ar_right.TdSurveyVolume * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TnFunctionID  != ar_right.TnFunctionID)
				return false;

			if (TdAlpha  != ar_right.TdAlpha)
			{
				int nTime = (int)(TdAlpha * 1000.0);
				int rTime = (int)(ar_right.TdAlpha * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TdBeta  != ar_right.TdBeta)
			{
				int nTime = (int)(TdBeta * 1000.0);
				int rTime = (int)(ar_right.TdBeta * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TdA  != ar_right.TdA)
			{
				int nTime = (int)(TdA * 1000.0);
				int rTime = (int)(ar_right.TdA * 1000.0);

				if (nTime != rTime)
					return false;
			}

			if (TdB  != ar_right.TdB)
			{
				int nTime = (int)(TdB * 1000.0);
				int rTime = (int)(ar_right.TdB * 1000.0);

				if (nTime != rTime)
					return false;
			}
		}
		catch (KExceptionPtr ex)
		{
			TxExceptionPrint(ex);
		}
		catch (...)
		{
			TxLogDebugException();
		}

		return true;
	}
};
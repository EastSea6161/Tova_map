#pragma once

enum KEMPSYSTEMTYPE
{
    // 여객모형
    KEMPASSENGER_SYSTEM = 0,	
    // 화물모형
    KEMFREIGHT_SYSTEM   = 1
};

enum KEMEsriAppType
{
	// ArcGIS Engine
	KEMEsriApp_ENGINE = 0,	
	// ArcGIS DESKTOP
	KEMEsriApp_DESKTOP   = 1
};

enum KEMLanguageType
{
    KEMKorea   = 0,
    KEMEnglish = 1
};


class KmzSystem
{
public:
    KmzSystem(void);
    ~KmzSystem(void);
private:
    static KEMPSYSTEMTYPE m_emSystemType;
public:
    static KEMPSYSTEMTYPE GetSystemType() ;
    static void           SetSystemType(KEMPSYSTEMTYPE systemType);
    static bool           IsPassengerSystem();
    static bool           IsFreightSystem();

private:
	static KEMEsriAppType m_emEsriAppType;
public:
	static KEMEsriAppType GetEsriAppType() ;
	static void           SetEsriAppType(KEMEsriAppType systemType);
	static bool           IsArcGISEngineSystem();

public:
    static KEMLanguageType GetLanguage() {
        return KEMKorea;
		//return KEMEnglish;
    }
};

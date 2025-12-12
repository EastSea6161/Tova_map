## 프로젝트 목록

   - ImTasLicenseCreater (exe)
   - muTas (exe) => Main Project
   - KLauncher (exe)
   - QBicExtStaticCLib (lib)
   - QBicExtStaticCppLib (lib)
   - QBicDataStructure (lib)
   - kmzModel (lib)
   - ZControls (dll)
   - QBicWindow (dll)
   - QBicCommon (dll) *Boost Library  사용
   - QBicGIS (dll) *Boost Library  사용

## 독립 프로젝트

   - ZControls
   - QBicExtStaticCLib
   - QBicExtStaticCppLib
   - QBicWindow
   - KLauncher

## 종속성 

1) ImTasLicenseCreater
   - QBicCommon

2) QBicCommon
   - QBicExtStaticCLib
   - QBicExtStaticCppLib

3) QBicGIS
   - QBicCommon 
   - QBicExtStaticCLib

4) kmzModel
   - QBicDataStructure
   - QBicExtStaticCppLib

5) muTas
   - QBicDataStructure
   - QBicExtStaticCppLib
   - QBicWindow
   - ZControls
   - QBicCommon
   - kmzModel
   - QBicGIS
   
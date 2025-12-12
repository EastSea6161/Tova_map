#include "stdafx.h"
#include "QBicMd5.h"

QBicMd5::QBicMd5()
{
}

QBicMd5::~QBicMd5()
{
}

bool QBicMd5::GetMd5String(CString a_strBuffer, CString& ar_strResult)
{
    a_strBuffer;// += "Hanmac";//
    HCRYPTPROV hCryptProv; 
    HCRYPTHASH hHash; 
    BYTE bHash[0x7f]; 
    DWORD dwHashLen= 16; // The MD5 algorithm always returns 16 bytes. 
    DWORD cbContent= a_strBuffer.GetLength(); 
    BYTE* pbContent= (BYTE*)a_strBuffer.GetBuffer(cbContent); 
 
    bool bResult(true);
    
    if(CryptAcquireContext(&hCryptProv, 
        NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) 
    {
        if(CryptCreateHash(hCryptProv, 
            CALG_MD5,	// algorithm identifier definitions see: wincrypt.h
            0, 0, &hHash)) 
        {
            if(CryptHashData(hHash, pbContent, cbContent, 0))
            {
                if(CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0)) 
                {
                    // Make a string version of the numeric digest value
                    ar_strResult.Empty();
                    CStringA tmp;
                    for (int i = 0; i<16; i++)
                    {
                        tmp.Format("%02x", bHash[i]);
                        ar_strResult += CA2T(tmp);
                    }
                }
                else 
                {
                    ar_strResult=_T("Error getting hash param"); 
                    bResult = false;
                }
            }
            else 
            {
                ar_strResult=_T("Error hashing data"); 
                bResult = false;
            }
        }
        else 
        {
            ar_strResult=_T("Error creating hash"); 
            bResult = false;
        }
    }
    else 
    {
        ar_strResult=_T("Error acquiring context"); 
        bResult = false;
    }

    CryptDestroyHash(hHash); 
    CryptReleaseContext(hCryptProv, 0); 
    a_strBuffer.ReleaseBuffer();
    return bResult; 
}

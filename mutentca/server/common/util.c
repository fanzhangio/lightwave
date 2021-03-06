/*
 * Copyright © 2018 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the “License”); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <includes.h>

BOOLEAN
LwCAUtilIsValueIPAddr(
    PCSTR           pszValue
    )
{
    BOOLEAN         bIsIP = FALSE;

    if (!IsNullOrEmptyString(pszValue))
    {
        unsigned char buf[sizeof(struct in_addr)];

        bIsIP = (inet_pton(AF_INET, pszValue, &buf[0]) == 1);
    }

    return bIsIP;
}

/*
 * The IP Address in pszValue can be in either dot-decimal notation
 * or hexadecimal notation
 */
BOOLEAN
LwCAUtilIsValuePrivateOrLocalIPAddr(
    PSTR            pszValue
    )
{
    uint32_t        unIp = 0x0;

    if (!IsNullOrEmptyString(pszValue))
    {
        unIp = inet_network(pszValue);

        if (LWCA_IS_IP_IN_PRIVATE_NETWORK(unIp))
        {
            return TRUE;
        }
        else if (LWCA_IS_IP_IN_LOCAL_NETWORK(unIp))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;
}

DWORD
LwCAUtilIsValueFQDN(
    PCSTR           pszValue,
    PBOOLEAN        pbIsValid
    )
{
    DWORD           dwError = 0;
    DWORD           dwIdx = 0;
    PSTR            pszTempVal = NULL;
    PSTR            pszLabel = NULL;
    PSTR            pszNextTok = NULL;
    BOOLEAN         bIsValid = FALSE;
    int             count = 0;
    char            cTemp = 0;
    size_t          szLabelLen = 0;

    if (IsNullOrEmptyString(pszValue))
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAStringCountSubstring(
                        pszValue,
                        ".",
                        &count);
    BAIL_ON_LWCA_ERROR(dwError);
    if (count == 0)
    {
        bIsValid = FALSE;
        goto ret;
    }

    if (LwCAStringLenA(pszValue) > 255)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateStringPrintfA(
                            &pszTempVal,
                            pszValue);
    BAIL_ON_LWCA_ERROR(dwError);

    pszLabel = LwCAStringTokA(pszTempVal, ".", &pszNextTok);
    while (pszLabel)
    {
        szLabelLen = LwCAStringLenA(pszLabel);

        if (szLabelLen > 63)
        {
            bIsValid = FALSE;
            goto ret;
        }

        if (!isalpha(pszLabel[0]) &&
            !isalpha(pszLabel[szLabelLen - 1]) &&
            !isdigit(pszLabel[szLabelLen - 1]))
        {
            bIsValid = FALSE;
            goto ret;
        }

        while ((cTemp = *(pszLabel + dwIdx)) != '\0')
        {
            if (!isalpha(cTemp) &&
                !isdigit(cTemp) &&
                (cTemp != '-'))
            {
                bIsValid = FALSE;
                goto ret;
            }

            ++dwIdx;
        }

        pszLabel = LwCAStringTokA(NULL, ".", &pszNextTok);
    }

    bIsValid = TRUE;


ret:

    *pbIsValid = bIsValid;

cleanup:

    LWCA_SAFE_FREE_STRINGA(pszTempVal);

    return dwError;

error:

    if (pbIsValid)
    {
        *pbIsValid = FALSE;
    }

    goto cleanup;
}

BOOLEAN
LwCAUtilDoesValueHaveWildcards(
    PCSTR            pszValue
    )
{
    DWORD           dwError = 0;
    int             count = 0;

    if (IsNullOrEmptyString(pszValue))
    {
        return FALSE;
    }

    dwError = LwCAStringCountSubstring(
                        pszValue,
                        "*",
                        &count);
    if (count != 0)
    {
        return TRUE;
    }

    return FALSE;
}

DWORD
LwCACreateCertificate(
    PCSTR               pcszCertificate,
    PLWCA_CERTIFICATE   *ppCertificate
    )
{
    DWORD dwError = 0;
    PSTR pszCert = NULL;

    if (IsNullOrEmptyString(pcszCertificate) || !ppCertificate)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateStringA(pcszCertificate, &pszCert);
    BAIL_ON_LWCA_ERROR(dwError);

    *ppCertificate = (PLWCA_CERTIFICATE)pszCert;

cleanup:
    return dwError;

error:
    LWCA_SAFE_FREE_MEMORY(pszCert);

    if (ppCertificate)
    {
        *ppCertificate = NULL;
    }

    goto cleanup;
}

DWORD
LwCACreateCertArray(
    PSTR                        *ppszCertificates,
    DWORD                       dwCount,
    PLWCA_CERTIFICATE_ARRAY     *ppCertArray
    )
{
    DWORD dwError = 0;
    DWORD iEntry = 0;
    PLWCA_CERTIFICATE_ARRAY  pCertArray = NULL;

    if (!ppszCertificates || dwCount <=0 || !ppCertArray)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_CERTIFICATE_ARRAY), (PVOID*)&pCertArray);
    BAIL_ON_LWCA_ERROR(dwError);

    pCertArray->dwCount = dwCount;

    dwError = LwCAAllocateMemory(sizeof(PLWCA_CERTIFICATE) * dwCount, (PVOID*)&pCertArray->ppCertificates);
    BAIL_ON_LWCA_ERROR(dwError);

    for(; iEntry < dwCount; ++iEntry)
    {
        dwError = LwCACreateCertificate(ppszCertificates[iEntry], &pCertArray->ppCertificates[iEntry]);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    *ppCertArray = pCertArray;

cleanup:
    return dwError;

error:
    LwCAFreeCertificates(pCertArray);
    if (ppCertArray)
    {
        *ppCertArray = NULL;
    }

    goto cleanup;
}

DWORD
LwCACreateKey(
    PBYTE     pData,
    DWORD     dwLength,
    PLWCA_KEY *ppKey
    )
{
    DWORD dwError = 0;
    PLWCA_KEY pKey = NULL;

    if (!pData || dwLength <= 0 || !ppKey)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_KEY), (PVOID*)&pKey);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateMemory(dwLength * sizeof(BYTE), (PVOID*) &pKey->pData);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCACopyMemory((PVOID) pKey->pData, dwLength * sizeof(BYTE), pData, (size_t)dwLength);
    BAIL_ON_LWCA_ERROR(dwError);

    pKey->dwLength = dwLength;

    *ppKey = pKey;

cleanup:
    return dwError;

error:
    LwCAFreeKey(pKey);
    if (ppKey)
    {
        *ppKey = NULL;
    }

    goto cleanup;
}

DWORD
LwCACopyCertArray(
    PLWCA_CERTIFICATE_ARRAY     pCertArray,
    PLWCA_CERTIFICATE_ARRAY     *ppCertArray
    )
{
    DWORD dwError = 0;
    DWORD iEntry = 0;
    PLWCA_CERTIFICATE_ARRAY  pTempCertArray = NULL;

    if (!pCertArray || !pCertArray->ppCertificates || pCertArray->dwCount <=0 || !ppCertArray)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_CERTIFICATE_ARRAY), (PVOID*)&pTempCertArray);
    BAIL_ON_LWCA_ERROR(dwError);

    pTempCertArray->dwCount = pCertArray->dwCount;

    dwError = LwCAAllocateMemory(sizeof(PLWCA_CERTIFICATE) * pCertArray->dwCount, (PVOID*)&pTempCertArray->ppCertificates);
    BAIL_ON_LWCA_ERROR(dwError);

    for(; iEntry < pCertArray->dwCount; ++iEntry)
    {
        dwError = LwCACreateCertificate(pCertArray->ppCertificates[iEntry], &pTempCertArray->ppCertificates[iEntry]);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    *ppCertArray = pTempCertArray;

cleanup:
    return dwError;

error:
    LwCAFreeCertificates(pTempCertArray);
    if (ppCertArray)
    {
        *ppCertArray = NULL;
    }

    goto cleanup;
}

DWORD
LwCACopyKey(
    PLWCA_KEY pKey,
    PLWCA_KEY *ppKey
    )
{
    DWORD dwError = 0;
    PLWCA_KEY pTempKey = NULL;

    if (!pKey || !ppKey)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCACreateKey(pKey->pData, pKey->dwLength, &pTempKey);
    BAIL_ON_LWCA_ERROR(dwError);

    *ppKey = pTempKey;

cleanup:
    return dwError;

error:
    LwCAFreeKey(pTempKey);
    if (ppKey)
    {
        *ppKey = NULL;
    }

    goto cleanup;
}

VOID
LwCAFreeCertificate(
    PLWCA_CERTIFICATE pCertificate
    )
{
    if (pCertificate)
    {
        LWCA_SAFE_FREE_MEMORY(pCertificate);
    }
}

VOID
LwCAFreeCertificates(
    PLWCA_CERTIFICATE_ARRAY pCertArray
    )
{
    DWORD iEntry = 0;

    if (pCertArray != NULL)
    {
        if (pCertArray->dwCount > 0 && pCertArray->ppCertificates)
        {
            for(; iEntry < pCertArray->dwCount; ++iEntry)
            {
                LwCAFreeCertificate(pCertArray->ppCertificates[iEntry]);
            }
        }
        LWCA_SAFE_FREE_MEMORY(pCertArray->ppCertificates);
        LWCA_SAFE_FREE_MEMORY(pCertArray);
    }
}

VOID
LwCAFreeKey(
    PLWCA_KEY pKey
    )
{
    if (pKey)
    {
        LWCA_SECURE_SAFE_FREE_MEMORY(pKey->pData, pKey->dwLength);
        LWCA_SAFE_FREE_MEMORY(pKey);
    }
}

DWORD
LwCADbCreateCAData(
    PCSTR                       pcszIssuer,
    PCSTR                       pcszSubject,
    PLWCA_CERTIFICATE_ARRAY     pCertificates,
    PLWCA_KEY                   pEncryptedPrivateKey,
    PLWCA_KEY                   pEncryptedEncryptionKey,
    PCSTR                       pcszTimeValidFrom,
    PCSTR                       pcszTimeValidTo,
    LWCA_CA_STATUS              status,
    PLWCA_DB_CA_DATA            *ppCAData
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CA_DATA pCAData = NULL;

    if (!pCertificates || IsNullOrEmptyString(pcszIssuer) ||
        IsNullOrEmptyString(pcszSubject) || !ppCAData)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_DB_CA_DATA), (PVOID*)&pCAData);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCACopyCertArray(pCertificates, &pCAData->pCertificates);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateStringA(pcszIssuer, &pCAData->pszIssuer);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateStringA(pcszSubject, &pCAData->pszSubject);
    BAIL_ON_LWCA_ERROR(dwError);

    if (!IsNullOrEmptyString(pcszTimeValidFrom))
    {
        dwError = LwCAAllocateStringA(pcszTimeValidFrom, &pCAData->pszTimeValidFrom);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (!IsNullOrEmptyString(pcszTimeValidTo))
    {
        dwError = LwCAAllocateStringA(pcszTimeValidTo, &pCAData->pszTimeValidTo);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (pEncryptedPrivateKey)
    {
        dwError = LwCACopyKey(pEncryptedPrivateKey, &pCAData->pEncryptedPrivateKey);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (pEncryptedEncryptionKey)
    {
        dwError = LwCACopyKey(pEncryptedEncryptionKey, &pCAData->pEncryptedEncryptionKey);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    pCAData->status = status;

    *ppCAData = pCAData;

cleanup:
    return dwError;

error:
    LwCADbFreeCAData(pCAData);
    if (ppCAData)
    {
        *ppCAData = NULL;
    }
    goto cleanup;
}

DWORD
LwCADbCreateCertData(
    PCSTR                   pcszSerialNumber,
    PCSTR                   pcszIssuer,
    PCSTR                   pcszTimeValidFrom,
    PCSTR                   pcszTimeValidTo,
    PCSTR                   pcszRevokedReason,
    PCSTR                   pcszRevokedDate,
    LWCA_CERT_STATUS        status,
    PLWCA_DB_CERT_DATA      *ppCertData
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CERT_DATA pCertData = NULL;

    if (IsNullOrEmptyString(pcszSerialNumber) || IsNullOrEmptyString(pcszRevokedReason) ||
            IsNullOrEmptyString(pcszRevokedDate) || IsNullOrEmptyString(pcszTimeValidFrom) ||
            IsNullOrEmptyString(pcszTimeValidTo) || !ppCertData)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_DB_CERT_DATA), (PVOID*)&pCertData);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateStringA(pcszSerialNumber, &(pCertData->pszSerialNumber));
    BAIL_ON_LWCA_ERROR(dwError);

    if (!IsNullOrEmptyString(pcszIssuer))
    {
        dwError = LwCAAllocateStringA(pcszIssuer, &pCertData->pszIssuer);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateStringA(pcszTimeValidFrom, &pCertData->pszTimeValidFrom);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateStringA(pcszTimeValidTo, &pCertData->pszTimeValidTo);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateStringA(pcszRevokedReason, &pCertData->pszRevokedReason);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCAAllocateStringA(pcszRevokedDate, &pCertData->pszRevokedDate);
    BAIL_ON_LWCA_ERROR(dwError);

    pCertData->status = status;

    *ppCertData = pCertData;

cleanup:
    return dwError;

error:
    LwCADbFreeCertData(pCertData);
    if (ppCertData)
    {
        *ppCertData = NULL;
    }
    goto cleanup;
}

DWORD
LwCADbCopyCertData(
    PLWCA_DB_CERT_DATA pCertData,
    PLWCA_DB_CERT_DATA *ppCertData
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CERT_DATA pTempCertData = NULL;

    if (pCertData || !ppCertData)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_DB_CERT_DATA), (PVOID*)&pTempCertData);
    BAIL_ON_LWCA_ERROR(dwError);

    if (!IsNullOrEmptyString(pCertData->pszSerialNumber))
    {
        dwError = LwCAAllocateStringA(pCertData->pszSerialNumber, &pTempCertData->pszSerialNumber);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (!IsNullOrEmptyString(pCertData->pszIssuer))
    {
        dwError = LwCAAllocateStringA(pCertData->pszIssuer, &pTempCertData->pszIssuer);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (!IsNullOrEmptyString(pCertData->pszRevokedReason))
    {
        dwError = LwCAAllocateStringA(pCertData->pszRevokedReason, &pTempCertData->pszRevokedReason);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (!IsNullOrEmptyString(pCertData->pszRevokedDate))
    {
        dwError = LwCAAllocateStringA(pCertData->pszRevokedDate, &pTempCertData->pszRevokedDate);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (!IsNullOrEmptyString(pCertData->pszTimeValidFrom))
    {
        dwError = LwCAAllocateStringA(pCertData->pszTimeValidFrom, &pTempCertData->pszTimeValidFrom);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    if (!IsNullOrEmptyString(pCertData->pszTimeValidTo))
    {
        dwError = LwCAAllocateStringA(pCertData->pszTimeValidTo, &pTempCertData->pszTimeValidTo);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    *ppCertData = pTempCertData;

cleanup:
    return dwError;

error:
    LwCADbFreeCertData(pTempCertData);
    if (ppCertData)
    {
        *ppCertData = NULL;
    }

    goto cleanup;
}

DWORD
LwCADbCopyCertDataArray(
    PLWCA_DB_CERT_DATA_ARRAY pCertDataArray,
    PLWCA_DB_CERT_DATA_ARRAY *ppCertDataArray
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CERT_DATA_ARRAY pTempCertDataArray = NULL;
    DWORD iEntry = 0;

    if (!pCertDataArray || pCertDataArray->dwCount <= 0 || !ppCertDataArray)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateMemory(sizeof(LWCA_DB_CERT_DATA_ARRAY), (PVOID*)&pTempCertDataArray);
    BAIL_ON_LWCA_ERROR(dwError);

    pTempCertDataArray->dwCount = pCertDataArray->dwCount;

    dwError = LwCAAllocateMemory(sizeof(PSTR) * pCertDataArray->dwCount, (PVOID*)&pTempCertDataArray->ppCertData);
    BAIL_ON_LWCA_ERROR(dwError);

    for(; iEntry < pCertDataArray->dwCount; ++iEntry)
    {
        dwError = LwCADbCopyCertData(pCertDataArray->ppCertData[iEntry], &pTempCertDataArray->ppCertData[iEntry]);
        BAIL_ON_LWCA_ERROR(dwError);
    }

    *ppCertDataArray = pTempCertDataArray;

cleanup:
    return dwError;

error:
    LwCADbFreeCertDataArray(pTempCertDataArray);
    if (ppCertDataArray)
    {
        *ppCertDataArray = NULL;
    }

    goto cleanup;
}

VOID
LwCADbFreeCAData(
    PLWCA_DB_CA_DATA pCAData
    )
{
    if (pCAData)
    {
        LWCA_SAFE_FREE_STRINGA(pCAData->pszIssuer);
        LWCA_SAFE_FREE_STRINGA(pCAData->pszSubject);
        LwCAFreeCertificates(pCAData->pCertificates);
        LwCAFreeKey(pCAData->pEncryptedPrivateKey);
        LwCAFreeKey(pCAData->pEncryptedEncryptionKey);
        LWCA_SAFE_FREE_STRINGA(pCAData->pszTimeValidFrom);
        LWCA_SAFE_FREE_STRINGA(pCAData->pszTimeValidTo);
        LWCA_SAFE_FREE_MEMORY(pCAData);
    }
}

VOID
LwCADbFreeCertData(
    PLWCA_DB_CERT_DATA pCertData
    )
{
    if (pCertData != NULL)
    {
        LWCA_SAFE_FREE_STRINGA(pCertData->pszSerialNumber);
        LWCA_SAFE_FREE_STRINGA(pCertData->pszIssuer);
        LWCA_SAFE_FREE_STRINGA(pCertData->pszRevokedReason);
        LWCA_SAFE_FREE_STRINGA(pCertData->pszRevokedDate);
        LWCA_SAFE_FREE_STRINGA(pCertData->pszTimeValidFrom);
        LWCA_SAFE_FREE_STRINGA(pCertData->pszTimeValidTo);
        LWCA_SAFE_FREE_MEMORY(pCertData);
    }
}

VOID
LwCADbFreeCertDataArray(
    PLWCA_DB_CERT_DATA_ARRAY pCertDataArray
    )
{
    DWORD iEntry = 0;

    if (pCertDataArray != NULL)
    {
        if (pCertDataArray->dwCount > 0 && pCertDataArray->ppCertData)
        {
            for(; iEntry < pCertDataArray->dwCount; ++iEntry)
            {
                LwCADbFreeCertData(pCertDataArray->ppCertData[iEntry]);
            }
        }
        LWCA_SAFE_FREE_MEMORY(pCertDataArray->ppCertData);
        LWCA_SAFE_FREE_MEMORY(pCertDataArray);
    }
}

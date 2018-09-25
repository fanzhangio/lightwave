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

#ifndef __MutentCADB_H__
#define __MutentCADB_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VMW_PSTR_DEFINED
#define VMW_PSTR_DEFINED 1
typedef char* PSTR;
#endif /* VMW_PSTR_DEFINED */

#ifndef VMW_PCSTR_DEFINED
#define VMW_PCSTR_DEFINED 1
typedef const char* PCSTR;
#endif /* VMW_PCSTR_DEFINED */

#ifndef VMW_VOID_DEFINED
#define VMW_VOID_DEFINED 1
typedef void VOID, *PVOID;
#endif /* VMW_VOID_DEFINED */

#ifndef VMW_DWORD_DEFINED
#define VMW_DWORD_DEFINED 1
typedef uint32_t DWORD, *PDWORD;
#endif /* VMW_DWORD_DEFINED */

#ifndef VMW_PBYTE_DEFINED
#define VMW_PBYTE_DEFINED 1
typedef unsigned char* PBYTE;
#endif /* VMW_PBYTE_DEFINED */

/*
 * Represents CA status
 */
typedef enum _LWCA_DB_CA_STATUS
{
    LWCA_DB_CA_STATUS_INACTIVE = 0,
    LWCA_DB_CA_STATUS_ACTIVE
} LWCA_DB_CA_STATUS,*PLWCA_DB_CA_STATUS;

/*
 * Represents Certificate status
 */
typedef enum _LWCA_DB_CERT_STATUS
{
    LWCA_DB_CERT_STATUS_INACTIVE = 0,
    LWCA_DB_CERT_STATUS_ACTIVE
} LWCA_DB_CERT_STATUS,*PLWCA_DB_CERT_STATUS;

/*
 * Defines Certificate array
 */
typedef struct _LWCA_DB_CERTIFICATE_ARRAY
{
    DWORD   dwCount;
    PSTR    *ppCertificates;
} LWCA_DB_CERTIFICATE_ARRAY, *PLWCA_DB_CERTIFICATE_ARRAY;

/*
 * Defines encrypted key
 */
typedef struct _LWCA_DB_ENCRYPTED_KEY
{
    PBYTE pData;
    DWORD dwLength;
} LWCA_DB_ENCRYPTED_KEY, *PLWCA_DB_ENCRYPTED_KEY;

/*
 * Defines CA data
 */
typedef struct _LWCA_DB_CA_DATA
{
    PSTR                            pszIssuer;
    PSTR                            pszSubject;
    PLWCA_DB_CERTIFICATE_ARRAY      pCertificates;
    PLWCA_DB_ENCRYPTED_KEY          pEncryptedPrivateKey;
    PLWCA_DB_ENCRYPTED_KEY          pEncryptedEncryptionKey;
    PSTR                            pszTimeValidFrom;
    PSTR                            pszTimeValidTo;
    LWCA_DB_CA_STATUS               status;
} LWCA_DB_CA_DATA, *PLWCA_DB_CA_DATA;

/*
 * Defines cert data
 */
typedef struct _LWCA_DB_CERT_DATA
{
    PSTR                    pszSerialNumber;
    PSTR                    pszIssuer;
    PSTR                    pszRevokedReason;
    PSTR                    pszRevokedDate;
    PSTR                    pszTimeValidFrom;
    PSTR                    pszTimeValidTo;
    LWCA_DB_CERT_STATUS     status;
} LWCA_DB_CERT_DATA, *PLWCA_DB_CERT_DATA;

/*
 * Defines cert data array
 */
typedef struct _LWCA_DB_CERT_DATA_ARRAY
{
    DWORD               dwCount;
    PLWCA_DB_CERT_DATA  *ppCertData;
} LWCA_DB_CERT_DATA_ARRAY, *PLWCA_DB_CERT_DATA_ARRAY;

/*
 * Plugin handle stores plugin context
 */
typedef struct _LWCA_DB_HANDLE *PLWCA_DB_HANDLE;

/*
 * Returns plugin version
 */
typedef PCSTR
(*PFN_LWCA_DB_GET_VERSION)(
    VOID
    );

/*
 * Assumption: Plugin will initialize by calling this API. And plugin is responsible for
 * loading its config file from file system (if required) and store its context in handle.
 */
typedef DWORD
(*PFN_LWCA_DB_INITIALIZE)(
    PLWCA_DB_HANDLE *ppHandle //OUT
    );

/*
 * Stores CA data with CA ID as the primary key.
 */
typedef DWORD
(*PFN_LWCA_DB_ADD_CA)(
    PLWCA_DB_HANDLE        pHandle,      //IN
    PCSTR                  pcszCAId,     //IN
    PLWCA_DB_CA_DATA       pCAData,      //IN
    PCSTR                  pcszParentCA  //IN (OPTIONAL)
    );

/*
 * Stores certificate data issued by CA.
 */
typedef DWORD
(*PFN_LWCA_DB_ADD_CERT_DATA)(
    PLWCA_DB_HANDLE        pHandle,  //IN
    PCSTR                  pcszCAId, //IN
    PLWCA_DB_CERT_DATA     pCertData //IN
    );

/*
 * Get CA stored certificates.
 */
typedef DWORD
(*PFN_LWCA_DB_GET_CA_CERTIFICATES)(
    PLWCA_DB_HANDLE            pHandle,     //IN
    PCSTR                      pcszCAId,    //IN
    PLWCA_DB_CERTIFICATE_ARRAY *ppCertArray //OUT
    );

/*
 * Get stored certificate data which is specific to CA.
 */
typedef DWORD
(*PFN_LWCA_DB_GET_CERT_DATA)(
    PLWCA_DB_HANDLE             pHandle,         //IN
    PCSTR                       pcszCAId,        //IN
    PLWCA_DB_CERT_DATA_ARRAY    *ppCertDataArray //OUT
    );

/*
 * Update CA data
 */
typedef DWORD
(*PFN_LWCA_DB_UPDATE_CA)(
    PLWCA_DB_HANDLE          pHandle,    //IN
    PCSTR                    pcszCAId,   //IN
    PLWCA_DB_CA_DATA         pCAData     //IN
    );

/*
 * Update CA status
 */
typedef DWORD
(*PFN_LWCA_DB_UPDATE_CA_STATUS)(
    PLWCA_DB_HANDLE          pHandle,     //IN
    PCSTR                    pcszCAId,    //IN
    LWCA_DB_CA_STATUS        status       //IN
    );

/*
 * Updates certificate data issued by CA.
 */
typedef DWORD
(*PFN_LWCA_DB_UPDATE_CERT_DATA)(
    PLWCA_DB_HANDLE        pHandle,  //IN
    PCSTR                  pcszCAId, //IN
    PLWCA_DB_CERT_DATA     pCertData //IN
    );

/*
 * Free memory allocated to plugin certificate data array
 */
typedef VOID
(*PFN_LWCA_DB_FREE_CERT_DATA_ARRAY)(
    PLWCA_DB_CERT_DATA_ARRAY  pCertDataArray       //IN
    );

/*
 * Free memory allocated to plugin certificate array
 */
typedef VOID
(*PFN_LWCA_DB_FREE_CERTIFICATE_ARRAY)(
    PLWCA_DB_CERTIFICATE_ARRAY  pCertArray     //IN
    );

/*
 * Free memory allocated to plugin handle
 */
typedef VOID
(*PFN_LWCA_DB_FREE_HANDLE)(
    PLWCA_DB_HANDLE    pHandle      //IN
    );

/*
 * It stores function pointers of various methods to perform db operations.
 */
typedef struct _LWCA_DB_FUNCTION_TABLE
{
    PFN_LWCA_DB_INITIALIZE              pFnInit;
    PFN_LWCA_DB_ADD_CA                  pFnAddCA;
    PFN_LWCA_DB_ADD_CERT_DATA           pFnAddCertData;
    PFN_LWCA_DB_GET_CA_CERTIFICATES     pFnGetCACertificates;
    PFN_LWCA_DB_GET_CERT_DATA           pFnGetCertData;
    PFN_LWCA_DB_UPDATE_CA               pFnUpdateCA;
    PFN_LWCA_DB_UPDATE_CA_STATUS        pFnUpdateCAStatus;
    PFN_LWCA_DB_UPDATE_CERT_DATA        pFnUpdateCertData;
    PFN_LWCA_DB_FREE_CERT_DATA_ARRAY    pFnFreeCertDataArray;
    PFN_LWCA_DB_FREE_CERTIFICATE_ARRAY  pFnFreeCertArray;
    PFN_LWCA_DB_FREE_HANDLE             pFnFreeHandle;
} LWCA_DB_FUNCTION_TABLE, *PLWCA_DB_FUNCTION_TABLE;

#ifdef __cplusplus
}
#endif

#endif // __MutentCADB_H__

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

#include "includes.h"

static LWCA_REST_RESOURCE resources[LWCA_REST_RSC_COUNT] =
{
    {
        LWCA_REST_RSC_API,
        "/v1/mutentca",
        TRUE,
        LwCARestUnknownSetResult,
        LwCARestUnknownGetHttpError,
        "application/json",
    },
    {
        LWCA_REST_RSC_UNKNOWN,
        NULL,
        FALSE,
        LwCARestUnknownSetResult,
        LwCARestUnknownGetHttpError,
        NULL
    }
};

PLWCA_REST_RESOURCE
LwCARestGetResource(
    PCSTR pcszPath
    )
{
    DWORD   dwIdx       = 0;
    BOOLEAN bValidPath  = FALSE;

    bValidPath = !IsNullOrEmptyString(pcszPath);

    for (; resources[dwIdx].pszEndpoint; ++dwIdx)
    {
        if (bValidPath)
        {
            if (resources[dwIdx].bIsEndpointPrefix)
            {
                if (LwCAStringStartsWith(pcszPath, resources[dwIdx].pszEndpoint, FALSE))
                {
                    break;
                }
            }
            else
            {
                if (!LwCAStringCompareA(pcszPath, resources[dwIdx].pszEndpoint, FALSE))
                {
                    break;
                }
            }
        }
    }

    return &resources[dwIdx];
}

DWORD
LwCARestUnknownSetResult(
    PLWCA_REST_RESULT   pRestRslt,
    PSTR                pszRequestId,
    DWORD               dwErr,
    PCSTR               pcszErrDetail
    )
{
    DWORD dwError = 0;

    if (!pRestRslt)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    dwError = LwCAAllocateStringA(LWCA_SAFE_STRING(pszRequestId), &pRestRslt->pszRequestId);
    BAIL_ON_LWCA_ERROR(dwError);

    dwError = LwCARestResultSetError(pRestRslt, (int)dwErr, pcszErrDetail);
    BAIL_ON_LWCA_ERROR(dwError);

cleanup:
    return dwError;

error:
    LWCA_LOG_ERROR(
            "%s failed, error (%d)",
            __FUNCTION__,
            dwError);

    goto cleanup;
}

DWORD
LwCARestUnknownGetHttpError(
    PLWCA_REST_RESULT   pRestRslt,
    PLWCA_HTTP_ERROR*   ppHttpError
    )
{
    DWORD               dwError = 0;
    int                 httpStatus = 0;
    PLWCA_HTTP_ERROR    pHttpError = NULL;

    if (!pRestRslt || !ppHttpError)
    {
        dwError = LWCA_ERROR_INVALID_PARAMETER;
        BAIL_ON_LWCA_ERROR(dwError);
    }

    switch ((DWORD)pRestRslt->errCode)
    {
    case LWCA_SUCCESS:
        httpStatus = HTTP_OK;
        break;

    case LWCA_ERROR_INVALID_REQUEST:
        httpStatus = HTTP_BAD_REQUEST;
        break;

    default:
        httpStatus = HTTP_INTERNAL_SERVER_ERROR;
        break;
    }

    pHttpError = LwCARestGetHttpError(httpStatus);

    *ppHttpError = pHttpError;

cleanup:
    return dwError;

error:
    LWCA_LOG_ERROR(
            "%s failed, error (%d)",
            __FUNCTION__,
            dwError);

    goto cleanup;
}

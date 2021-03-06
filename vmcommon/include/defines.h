/*
 * Copyright © 2017 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the ?~@~\License?~@~]); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ?~@~\AS IS?~@~] BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef __VM_COMMON_DEFINE_H__
#define __VM_COMMON_DEFINE_H__

#define BUFFER_SIZE 1024

#define VM_COMMON_SAFE_FREE_MEMORY(PTR)       \
    do {                                       \
        if ((PTR)) {                           \
            VmFreeMemory(PTR);          \
            (PTR) = NULL;                      \
        }                                      \
    } while(0)

#define VM_COMMON_SAFE_FREE_CURL_MEMORY(PTR)   \
    do {                                       \
        if ((PTR)) {                           \
            curl_free(PTR);          \
            (PTR) = NULL;                      \
        }                                      \
    } while(0)

#define BAIL_ON_VM_COMMON_ERROR(dwError)      \
    if (dwError)                               \
    {                                          \
        goto error;                            \
    }

#define VM_COMMON_SAFE_STRING(str) ((str) ? (str) : "")

#define BAIL_AND_LOG_ON_VM_COMMON_ERROR(x, str) \
    if ((x) != 0)                                            \
    {                                                        \
        fprintf(stderr, "error [%u] in file [%s] function [%s] at line [%d] with message [%s]\n", x, __FILE__, __FUNCTION__, __LINE__, VM_COMMON_SAFE_STRING(str)); \
        goto error;                                          \
    }

#define BAIL_AND_LOG_ON_VM_COMMON_CURL_ERROR(x, curlCode) \
    if (curlCode != CURLE_OK)                             \
    {                                                     \
        x = VM_COMMON_ERROR_CURL_FAILURE;                 \
        BAIL_AND_LOG_ON_VM_COMMON_ERROR(x, curl_easy_strerror(curlCode)); \
    }

#ifndef IsNullOrEmptyString
#define IsNullOrEmptyString(str) (!(str) || !*(str))
#endif

#endif /* __VM_COMMON_DEFINE_H__ */

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

typedef struct _LWCA_DB_CONTEXT
{
    pthread_rwlock_t                dbMutex;
    PSTR                            pszPlugin;
    PLWCA_DB_FUNCTION_TABLE         pFt;
    PLWCA_PLUGIN_HANDLE             pPluginHandle;
    PLWCA_DB_HANDLE                 pDbHandle;
    BOOLEAN                         isInitialized;
} LWCA_DB_CONTEXT, *PLWCA_DB_CONTEXT;

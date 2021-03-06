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

SECURITY_TEST_STATE _this_state = {0};

int
Security_Tests_Load (
    VOID **state
    )
{
    int error = 0;

    _this_state.module = dlopen(".libs/security_plugin_test.so", RTLD_NOW);

    if (!_this_state.module)
    {
        fprintf(stderr, "dlopen failed \n");
        goto error;
    }

    *state = &_this_state;

error:
    return error;
}

int
Security_Tests_Unload_Interface(
    VOID **state
    )
{
    int error = 0;
    PSECURITY_TEST_STATE pState = *state;

    PFN_LWCA_SECURITY_UNLOAD_INTERFACE pFnUnloadInterface = NULL;
    pFnUnloadInterface = dlsym(pState->module, LWCA_FN_NAME_SECURITY_UNLOAD_INTERFACE);
    if (!pFnUnloadInterface)
    {
        fail_msg("dlsym failed LwCASecurityUnloadInterface: %s \n", dlerror());
        goto error;
    }

    if (pState->pInterface)
    {
        pFnUnloadInterface(pState->pInterface);
    }

error:
    return error;
}

int
Security_Tests_Unload (
    VOID **state
    )
{
    int error = 0;
    PSECURITY_TEST_STATE pState = *state;

    if (Security_Tests_Unload_Interface(state))
    {
        fail_msg("dlsym failed LwCASecurityUnloadInterface: %s \n", dlerror());
        goto error;
    }

error:
    if (pState && pState->module)
    {
        dlclose(pState->module);
    }

    return error;
}

int
Security_Tests_Load_Interface(
    void **state
    )
{
    int error = 0;
    PFN_LWCA_SECURITY_LOAD_INTERFACE pFnLoadInterface = NULL;
    PSECURITY_TEST_STATE pState = *state;

    pFnLoadInterface = dlsym(pState->module, LWCA_FN_NAME_SECURITY_LOAD_INTERFACE);
    if (!pFnLoadInterface)
    {
        fail_msg("dlsym failed LwCASecurityLoadInterface: %s \n", dlerror());
        goto error;
    }

    error = pFnLoadInterface(&pState->pInterface);
    if (error)
    {
        fail_msg("LoadInterface call failed\n");
        goto error;
    }

error:
    return error;
}

int
Security_Tests_Initialize(
    void **state
    )
{
    int error = 0;
    PSECURITY_TEST_STATE pState = *state;

    if (Security_Tests_Load_Interface(state))
    {
        goto error;
    }

    error = pState->pInterface->pFnInitialize(&pState->pHandle);
    if (error)
    {
        fail_msg("initialize failed\n");
        goto error;
    }

error:
    return error;
}

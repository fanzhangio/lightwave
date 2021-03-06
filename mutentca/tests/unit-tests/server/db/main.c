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

int main(VOID)
{
    int ret = 0;

    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(Test_LwCADbInitCtx, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbCAData, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbCAData_Invalid, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbCertData, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbCertData_Invalid, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbAddCA, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbAddCertData, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbCheckCA, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbGetCACertificates, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbGetCertData, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbUpdateCA, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbUpdateCAStatus, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbUpdateCertData, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCADbFreeCtx, NULL, NULL),
    };

    ret = cmocka_run_group_tests(tests, NULL, NULL);

    if (ret)
    {
        fail_msg("%s", "MutentCA DB tests failed");
    }

    return ret;
}

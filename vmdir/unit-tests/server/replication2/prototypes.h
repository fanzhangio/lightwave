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
#ifndef METADATA_H_
#define METADATA_H_

//////////////////
// updatelist.c
//////////////////

//unit test functions
int
VmDirSetupReplUpdateListTest(
    VOID    **state
    );

int
VmDirTeardownReplUpdateListTest(
    VOID    **state
    );

VOID
VmDirReplUpdateListParseSyncDoneCtl_ValidInput(
    VOID    **state
    );

int
VmDirSetupReplUpdateListExpand_ModifyTest(
    VOID    **state
    );

int
VmDirSetupReplUpdateListExpand_AddTest(
    VOID    **state
    );

int
VmDirSetupReplUpdateListExpand_DeleteTest(
    VOID    **state
    );

int
VmDirSetupReplUpdateListExpand_AddTombstoneTest(
    VOID    **state
    );

VOID
VmDirReplUpdateListExpand_ModifyTest(
    VOID    **state
    );

VOID
VmDirReplUpdateListExpand_AddTest(
    VOID    **state
    );

VOID
VmDirReplUpdateListExpand_DeleteTest(
    VOID    **state
    );

VOID
VmDirReplUpdateListExpand_AddTombstoneTest(
    VOID    **state
    );
//////////////////
// update.c
//////////////////
VOID
VmDirReplUpdateToUSNList_NoDupMetaDataOnly(
    VOID**    state
    );
VOID
VmDirReplUpdateToUSNList_DupMetaDataOnly(
    VOID**    state
    );
VOID
VmDirReplUpdateToUSNList_MetaDataOnly(
    VOID**    state
    );
VOID
VmDirReplUpdateToUSNList_ValueMetaDataOnly(
    VOID**    state
    );

VOID
VmDirReplUpdateToUSNList_MetaDataAndValueMetaData(
    VOID**    state
    );

VOID
VmDirReplUpdateToUSNList_MetaDataAndValueMetaDataSame(
    VOID**    state
    );

VOID
VmDirReplUpdateLocalUsn_ValidInput(
    VOID**    state
    );

//////////////////
// extractevents.c
//////////////////
int
VmDirSetupExtractEventAttributeChanges(
    VOID    **state
    );

VOID
VmDirExtractEventAttributeChanges_ValidInput(
    VOID    **state
    );

int
VmDirSetupExtractEventAttributeValueChanges(
    VOID    **state
    );

VOID
VmDirExtractEventAttributeValueChanges_ValidInput(
    VOID    **state
    );

int
VmDirSetupExtractEventPopulateMustAttributes(
    VOID    **state
    );

VOID
VmDirExtractEventPopulateMustAttributes_ValidInput(
    VOID    **state
    );
int
VmDirSetupExtractEventPopulateOperationAttributes(
    VOID **state
    );

VOID
VmDirExtractEventPopulateOperationAttributes_ValidInput(
    VOID    **state
    );

int
VmDirTeardownExtractEvent(
    VOID    **state
    );

/////////////////////
// testcommon.c
////////////////////
PVMDIR_REPL_ATTRIBUTE_METADATA
VmDirFindAttrMetaData(
    PVDIR_LINKED_LIST    pList,
    PSTR                 pszAttrType
    );

VOID
VmDirAllocateAttrMetaData(
    PSTR                         pszAttrType,
    USN                          localUsn,
    USN                          origUsn,
    UINT64                       version,
    PVMDIR_REPLICATION_UPDATE    pUpdate
    );

VOID
VmDirAllocateAttrAndMetaData(
    PSTR                         pszAttrType,
    PSTR                         pszValue,
    USN                          localUsn,
    USN                          origUsn,
    UINT64                       version,
    PVMDIR_REPLICATION_UPDATE    pUpdate
    );

VOID
VmDirAllocateAttrValueMetaData(
    PSTR                         pszValueMetaData,
    PVMDIR_REPLICATION_UPDATE    pUpdate
    );

VOID
VmDirAllocateMultiValueAttr(
    PSTR                         pszAttrType,
    USN                          localUSN,
    USN                          origUSN,
    PVMDIR_REPLICATION_UPDATE    pUpdate
    );
#endif

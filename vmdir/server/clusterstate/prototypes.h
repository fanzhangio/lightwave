/*
 * Copyright ©2017 VMware, Inc.  All Rights Reserved.
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



#ifndef _CLUSTERSTATE_PROTOTYPES_H_
#define _CLUSTERSTATE_PROTOTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

// statecache.c
DWORD
VmDirClusterLoadCache(
    VOID
    );

VOID
VmDirClusterFreeCache(
    VOID
    );

DWORD
VmDirClusterNodeLDPConn(
    PVMDIR_NODE_STATE   pNode,
    BOOLEAN             bForce
    );

// clusterthr.c
DWORD
VmDirInitClusterStateThread(
    VOID
    );

DWORD
VmDirInitRaftPsState(
    VOID
    );

VOID
VmDirLoadRaftState(
    VOID
    );

DWORD
VmDirSetRaftClusterSize(
    BOOLEAN bNeedClusterStateLock
    );

VOID
VmDirPersistTerm(
    int term
    );

// libmain.c


#ifdef __cplusplus
}
#endif

#endif // _CLUSTERSTATE_PROTOTYPES_H_


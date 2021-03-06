/*
 * Copyright © 2012-2017 VMware, Inc.  All Rights Reserved.
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


#ifndef _PROTOTYPES_H_
#define _PROTOTYPES_H_

// add.c
int
VmDirAttributeDupValueCheck(
    PVDIR_ATTRIBUTE  pAttr,
    PSTR*            ppszDupAttributeName
    );

int
VmDirEntryCheckStructureRule(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry);

int
VmDirEntryAttrValueNormalize(
    PVDIR_ENTRY     pEntry,
    BOOLEAN         bIndexAttributeOnly
    );

// computedattribute.c
DWORD
VmDirBuildComputedAttribute(
    PVDIR_OPERATION     pOperation,
    PVDIR_ENTRY         pEntry
    );

// password.c
DWORD
VdirPasswordHash(
    PVDIR_PASSWORD_HASH_SCHEME  pHashScheme,
    PVDIR_BERVALUE              pBerv,          // in (clear text password)
    PVDIR_BERVALUE              pHashBerv,      // out (hashed code password)
    PBYTE                       pSaltByte       // in (optional salt)
    );

DWORD
VdirPasswordGenerateSalt(
    PVDIR_PASSWORD_HASH_SCHEME      pScheme,
    PBYTE*                          ppSalt
    );

DWORD
VdirPasswordVerifySupportedScheme(
    PVDIR_BERVALUE   pBerv
    );

DWORD
VdirPasswordAddSchemeCode(
    PVDIR_ATTRIBUTE  pAttrScheme,
    PVDIR_BERVALUE   pBerv,
    PVDIR_BERVALUE   pOutBerv
    );

PVDIR_PASSWORD_HASH_SCHEME
VdirDefaultPasswordScheme(
    VOID
    );

DWORD
VdirPasswordModifyPreCheck(
    PVDIR_OPERATION     pOperation
    );

// plugin.c
/*
 * Called before backend add
 */
DWORD
VmDirExecutePreAddPlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // entry been manipulated
    DWORD               dwResult         // latest call return value
    );

DWORD
VmDirExecutePostAddCommitPlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // entry been manipulated
    DWORD               dwResult         // latest call return value
    );

/*
 * Called before backend modify
 */
DWORD
VmDirExecutePreModifyPlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // entry been manipulated
    DWORD               dwResult         // latest call return value
    );

/*
 * Called at the beginning of internalModifyEntry (to manipulate Mod structure)
 */
DWORD
VmDirExecutePreModApplyModifyPlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // pEntry is NULL in this plugin
    DWORD               dwResult         // latest call return value
    );

/*
 * Called after backend modify commit/abort
 * NOTE, since this is called after backend commit/abort, we should NOT change
 * the return status. i.e. dwResult == return value
 */
DWORD
VmDirExecutePostModifyCommitPlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // entry been manipulated
    DWORD               dwResult         // latest call return value
    );

/*
 * Called at the beginning of internalDeleteEntry (to manipulate Mod structure)
 */
DWORD
VmDirExecutePreModApplyDeletePlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // pEntry is NULL in this plugin
    DWORD               dwResult         // latest call return value
    );

DWORD
VmDirExecutePostDeleteCommitPlugins(
    PVDIR_OPERATION     pOperation,      // current operation
    PVDIR_ENTRY         pEntry,          // entry been manipulated
    DWORD               dwResult         // latest call return value
    );

// indexplugin.c
DWORD
VmDirPluginIndexEntryPreAdd(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginIndexEntryPostAdd(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginIndexEntryPreModApplyModify(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginIndexEntryPreModify(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

// lockoutpolicy.c
DWORD
VdirPasswordFailEvent(
    PVDIR_OPERATION     pOperation,
    PCSTR               pszNormDN,
    PVDIR_ENTRY         pEntry
    );

DWORD
VdirGetPasswdAndLockoutPolicy(
    PCSTR                           pszNormEntryDN,
    PVDIR_PASSWD_LOCKOUT_POLICY     pPolicy
    );

DWORD
VdirLockoutPolicyIntegrityCheck(
    PVDIR_ENTRY     pPolicyEntry
    );

DWORD
VdirLoginBlocked(
    PVDIR_OPERATION     pOperation,
    PVDIR_ENTRY         pEntry
    );

VOID
VdirLockoutCacheRemoveRec(
    PCSTR           pszNormDN
    );

DWORD
VdirUserActCtlFlagSet(
    PVDIR_ENTRY         pEntry,
    int64_t             iTtargetFlag
    );


DWORD
VdirUserActCtlFlagUnset(
    PVDIR_ENTRY         pEntry,
    int64_t             iTargetFlag
    );

DWORD
VdirPasswordStrengthCheck(
    PVDIR_BERVALUE                  pPasswdBerv,
    PVDIR_PASSWD_LOCKOUT_POLICY     pPolicy,       // optional
    PVDIR_BERVALUE                  pDNBerv        // optional
    );

// krb.c
DWORD
VmDirKrbUPNKeySet(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    PVDIR_BERVALUE   pBervPasswd
    );

DWORD
VmDirKrbCreateKeyBlob(
    PVDIR_BERVALUE   pBervPrincipalName,
    PVDIR_BERVALUE   pBervPasswd,
    DWORD            dwKvno,
    PVDIR_BERVALUE   pOutKeyBlob
    );

// sasl.c
DWORD
VmDirSASLInit(
    VOID
    );

BOOLEAN
VmDirIsSupportedSASLMechanism(
    PCSTR   pszMech
    );

DWORD
VmDirSASLSessionInit(
    PVDIR_SASL_BIND_INFO    pSaslBindInfo
    );

DWORD
VmDirSASLSessionStart(
    PVDIR_SASL_BIND_INFO    pSaslBindInfo,
    PVDIR_BIND_REQ          pBindReq,
    PVDIR_BERVALUE          pBervSaslReply,
    PVDIR_LDAP_RESULT       pResult
    );

DWORD
VmDirSASLSessionStep(
    PVDIR_SASL_BIND_INFO    pSaslBindInfo,
    PVDIR_BIND_REQ          pBindReq,
    PVDIR_BERVALUE          pBervSaslReply,
    PVDIR_LDAP_RESULT       pResult
    );

VOID
VmDirSASLSessionClose(
    PVDIR_SASL_BIND_INFO    pSaslBindInfo
    );

// groupplugin.c
DWORD
VmDirPluginGroupTypePreAdd(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginGroupTypePreModify(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginGroupMemberPreModApplyDelete(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

// pscache.c
DWORD
VmDirPagedSearchCacheInit(
    VOID
    );

VOID
VmDirPagedSearchCacheFree(
    VOID
    );

DWORD
VmDirPagedSearchCacheInsert(
    PVDIR_OPERATION pOperation,
    DWORD dwCandidatesProcessed
    );

DWORD
VmDirPagedSearchCacheRead(
    PCSTR pszCookie,
    ENTRYID **ppValidatedEntries,
    DWORD *pdwEntryCount
    );

// pagesearch.c
VOID
VmDirPagedSearchContextFree(
    VOID
    );

DWORD
VmDirPagedSearchContextInit(
    VOID
    );

DWORD
VmDirProcessPagedSearch(
    VDIR_OPERATION *  pOperation
    );

// replplugin.c
DWORD
VmDirPluginReplAgrPostAddCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginReplAgrPostDeleteCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginServerEntryPostAddCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginServerEntryPostDeleteCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginDCAccountPostModifyCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

// saslsockbuf.c
DWORD
VmDirSASLSockbufInstall(
    Sockbuf*                pSockbuf,
    PVDIR_SASL_BIND_INFO    pSaslBindInfo
    );

VOID
VmDirSASLSockbufRemove(
    Sockbuf*        pSockbuf
    );

// schema.c
DWORD
VmDirPluginSchemaLibUpdatePreModify(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginSchemaLibUpdatePostModifyCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginSchemaEntryPreAdd(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginSchemaLibUpdatePreAdd(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwPriorResult
    );

DWORD
VmDirPluginSchemaLibUpdatePostAddCommit(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    DWORD            dwResult
    );

// srputil.c
DWORD
VmDirSRPSetSecret(
    PVDIR_OPERATION  pOperation,
    PVDIR_ENTRY      pEntry,
    PVDIR_BERVALUE   pBervClearTextPasswd
    );

// specialsearch.c
BOOLEAN
VmDirHandleSpecialSearch(
    PVDIR_OPERATION     pOperation,
    PVDIR_LDAP_RESULT   pResult
    );

BOOLEAN
VmDirIsSearchForDseRootEntry(
    PVDIR_OPERATION     pOp
    );

BOOLEAN
VmDirIsSearchForSchemaEntry(
    PVDIR_OPERATION     pOp
    );

BOOLEAN
VmDirIsSearchForServerStatus(
    PVDIR_OPERATION     pOp
    );

BOOLEAN
VmDirIsSearchForReplicationStatus(
    PVDIR_OPERATION     pOp
    );

BOOLEAN
VmDirIsSearchForSchemaReplStatus(
    PVDIR_OPERATION     pOp,
    PBOOLEAN            pbRefresh
    );

BOOLEAN
VmDirIsSearchForIntegrityCheckStatus(
    PVDIR_OPERATION                     pOp,
    PVMDIR_INTEGRITY_CHECK_JOB_STATE    pState
    );

// util.c
VOID
VmDirInternalMetricsUpdate(
    METRICS_LDAP_OPS        operation,
    VDIR_OPERATION_PROTOCOL protocol,
    VDIR_OPERATION_TYPE     opType,
    int                     errCode,
    uint64_t                iMLStartTime,
    uint64_t                iMLEndTime,
    uint64_t                iBEStartTime,
    uint64_t                iBEEndTime
    );

BOOLEAN
VmDirIsSearchForRaftPing(
    PVDIR_OPERATION     pOp
    );

BOOLEAN
VmDirIsSearchForRaftVote(
    PVDIR_OPERATION     pOp
    );

BOOLEAN
VmDirIsSearchForStatePing(
    PVDIR_OPERATION     pOp
    );

//writequeue.c
DWORD
VmDirWriteQueueElementAllocate(
    PVMDIR_WRITE_QUEUE_ELEMENT*    ppWriteQueueEle
    );

VOID
VmDirWriteQueueElementFree(
    PVMDIR_WRITE_QUEUE_ELEMENT    pWriteQueueEle
    );

DWORD
VmDirWriteQueuePush(
    PVDIR_BACKEND_CTX           pBECtx,
    PVMDIR_WRITE_QUEUE          pWriteQueue,
    PVMDIR_WRITE_QUEUE_ELEMENT  pWriteQueueEle
    );

VOID
VmDirWriteQueuePop(
    PVMDIR_WRITE_QUEUE          pWriteQueue,
    PVMDIR_WRITE_QUEUE_ELEMENT  pWriteQueueEle
    );

DWORD
VmDirWriteQueueWait(
    PVMDIR_WRITE_QUEUE          pWriteQueue,
    PVMDIR_WRITE_QUEUE_ELEMENT  pWriteQueueEle
    );

//usn.c
DWORD
VmDirInitMaxCommittedUSN(
    PVDIR_BACKEND_INTERFACE   pBE
    );

VOID
VmDirUpdateMaxCommittedUSNInLock(
    USN   committedUSN
    );

USN
VmDirGetMaxCommittedUSN(
    VOID
    );

#endif

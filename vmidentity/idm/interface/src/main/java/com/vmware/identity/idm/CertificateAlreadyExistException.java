/*
 *
 *  Copyright (c) 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not
 *  use this file except in compliance with the License.  You may obtain a copy
 *  of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, without
 *  warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 *
 */
package com.vmware.identity.idm;

/**
 * This exception is thrown when a tenant with the given name does not exist. */
public class CertificateAlreadyExistException extends IDMException {

    /**
     * Serial version id
     */
    private static final long serialVersionUID = -4117607950927085624L;

    public CertificateAlreadyExistException(String message) {
        super(message);
    }

    public CertificateAlreadyExistException(Throwable ex) {
        super(ex);
    }

    public CertificateAlreadyExistException(String message, Throwable ex) {
        super(message, ex);
    }
}

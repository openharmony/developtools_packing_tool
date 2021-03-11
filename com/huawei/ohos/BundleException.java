/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 */

package com.huawei.ohos;

/**
 * bundle tool exception class.
 *
 * @author huzeqi
 * @since 2019-10-25
 */
class BundleException extends Exception {
    private static final long serialVersionUID = 1813070042705457755L;

    /**
     * constructor for BundleException.
     */
    public BundleException() {
    }

    /**
     * constructor for BundleException.
     */
    public BundleException(String message) {
        super(message);
    }
}
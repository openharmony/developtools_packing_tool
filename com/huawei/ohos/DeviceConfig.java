/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

/**
 * App DeviceConfig info.
 *
 * @author dingyao
 * @since 2020-05-19
 */
public class DeviceConfig {
    /**
     * Indicates the target request Sdk Version of app DeviceConfig.
     */
    public String targetReqSdk = "";

    /**
     * Indicates the compatible request Sdk Version of app DeviceConfig.
     */
    public String compatibleReqSdk = "";

    /**
     * Indicates the jointUserid of app DeviceConfig.
     */
    public String jointUserid = "";

    /**
     * Indicates the process of app DeviceConfig.
     */
    public String process = "";

    /**
     * Indicates the arkFlag of app DeviceConfig.
     */
    public String arkFlag = "";

    /**
     * Indicates the targetArkVersion of app DeviceConfig.
     */
    public String targetArkVersion = "";

    /**
     * Indicates the compatibleArkVersion of app DeviceConfig.
     */
    public String compatibleArkVersion = "";

    /**
     * Indicates the directLaunch of app DeviceConfig.
     */
    public boolean directLaunch = false;
}

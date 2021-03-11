/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.HashMap;
import java.util.Map;

/**
 * App Profile info.
 *
 * @author dingyao
 * @since 2020-05-19
 */
public class ProfileInfo {
    /**
     * Indicates the file name of the hap.
     */
    public String hapName = "";

    /**
     * Indicates the appInfo of app.
     */
    public AppInfo appInfo = null;

    /**
     * Indicates the deviceConfig of app.
     */
    public Map<String, DeviceConfig> deviceConfig = new HashMap<>();

    /**
     * Indicates the hapInfo of app.
     */
    public HapInfo hapInfo = null;
}

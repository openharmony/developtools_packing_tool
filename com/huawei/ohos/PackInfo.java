/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * Package info.
 *
 * @author dingyao
 * @since 2020-04-22
 */
public class PackInfo {
    /**
     * Indicates the name of PackInfo.
     */
    public String name = "";

    /**
     * Indicates the moduleName of PackInfo.
     */
    public String moduleName = "";

    /**
     * Indicates the moduleType of PackInfo.
     */
    public String moduleType = "";

    /**
     * Indicates the deviceType of PackInfo.
     */
    public List<String> deviceType = new ArrayList<String>();

    /**
     * Indicates the deliveryWithInstall of PackInfo.
     */
    public boolean deliveryWithInstall = false;
}

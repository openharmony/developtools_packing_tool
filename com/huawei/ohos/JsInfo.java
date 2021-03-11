/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * JS Module info.
 *
 * @author dingyao
 * @since 2020-06-04
 */
public class JsInfo {
    /**
     * Indicates the name of JS Module.
     */
    public String name = "";

    /**
     * Indicates the window style of JS Module.
     */
    public String window = "";

    /**
     * Indicates the pages of JS Module.
     */
    public List<String> pages = new ArrayList<String>();
}
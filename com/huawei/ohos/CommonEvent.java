/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * App CommonEvent info.
 *
 * @author dingyao
 * @since 2020-05-19
 */
public class CommonEvent {
    /**
     * Indicates the name of app CommonEvent.
     */
    public String name = "";

    /**
     * Indicates the permission of app CommonEvent.
     */
    public String permission = "";

    /**
     * Indicates the data of app CommonEvent.
     */
    public List<String> data = new ArrayList<String>();

    /**
     * Indicates the type of app CommonEvent.
     */
    public List<String> type = new ArrayList<String>();

    /**
     * Indicates the events of app CommonEvent.
     */
    public List<String> events = new ArrayList<String>();
}

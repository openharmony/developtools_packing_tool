/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * App Shortcut info.
 *
 * @author dingyao
 * @since 2020-05-19
 */
public class Shortcut {
    /**
     * Indicates the shortcutId of app Shortcut.
     */
    public String shortcutId = "";

    /**
     * Indicates the label of app Shortcut.
     */
    public String label = "";

    /**
     * Indicates the icon of app Shortcut.
     */
    public String icon = "";

    /**
     * Indicates the intents of app Shortcut.
     */
    public List<IntentInfo> intents = new ArrayList<IntentInfo>();
}

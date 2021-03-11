/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * MetaData info.
 *
 * @author dingyao
 * @since 2020-05-09
 */
public class MetaData {
    /**
     * Indicates the parameters of MetaData.
     */
    public List<MetaDataInfo> parameters = new ArrayList<MetaDataInfo>();

    /**
     * Indicates the results of MetaData.
     */
    public List<MetaDataInfo> results = new ArrayList<MetaDataInfo>();

    /**
     * Indicates the CustomizeDatas of MetaData.
     */
    public List<CustomizeData> customizeDatas = new ArrayList<CustomizeData>();
}
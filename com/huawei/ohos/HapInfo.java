/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * Hap info.
 *
 * @author dingyao
 * @since 2020-04-22
 */
public class HapInfo {
    /**
     * Indicates the package of HapInfo.
     */
    public String packageStr = "";

    /**
     * Indicates the name of HapInfo.
     */
    public String name = "";

    /**
     * Indicates the description of HapInfo.
     */
    public String description = "";

    /**
     * Indicates the supportedModes of HapInfo.
     */
    public List<String> supportedModes = new ArrayList<String>();

    /**
     * Indicates the abilities of HapInfo.
     */
    public List<AbilityInfo> abilities = new ArrayList<AbilityInfo>();

    /**
     * Indicates the defPermissions of HapInfo.
     */
    public List<DefPermission> defPermissions = new ArrayList<DefPermission>();

    /**
     * Indicates the defPermissionsGroups of HapInfo.
     */
    public List<DefPermissionGroup> defPermissionsGroups = new ArrayList<DefPermissionGroup>();

    /**
     * Indicates the distro of HapInfo.
     */
    public Distro distro = null;

    /**
     * Indicates the reqCapabilities of HapInfo.
     */
    public List<String> reqCapabilities = new ArrayList<String>();

    /**
     * Indicates the deviceType of HapInfo.
     */
    public List<String> deviceType = new ArrayList<String>();

    /**
     * Indicates the metaData of HapInfo.
     */
    public MetaData metaData = new MetaData();

    /**
     * Indicates the HapInfo is Js app.
     */
    public boolean isJs = false;

    /**
     * Indicates the reqPermissions of HapInfo.
     */
    public List<ReqPermission> reqPermissions = new ArrayList<ReqPermission>();

    /**
     * Indicates the commonEvents of HapInfo.
     */
    public List<CommonEvent> commonEvents = new ArrayList<CommonEvent>();

    /**
     * Indicates the shortcuts of HapInfo.
     */
    public List<Shortcut> shortcuts = new ArrayList<Shortcut>();

    /**
     * Indicates the DistroFilter of HapInfo
     */
    public DistroFilter distroFilter = new DistroFilter();

}

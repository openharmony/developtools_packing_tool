/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

package com.huawei.ohos;

import java.util.List;
import java.util.ArrayList;

/**
 * Uncomperss result.
 *
 * @author dingyao
 * @since 2020-04-22
 */
public class UncomperssResult {
    private boolean result = true;
    private String message = "Success";
    private List<PackInfo> packInfos = new ArrayList<PackInfo>();
    private List<ProfileInfo> profileInfos = new ArrayList<ProfileInfo>();
    private String packInfoStr = "";
    private List<String> profileInfosStr = new ArrayList<String>();
    private String icon = "";
    private String label = "";

    void setResult(boolean result) {
        this.result = result;
    }

    void setMessage(String message) {
        this.message = message;
    }

    void setPackInfos(List<PackInfo> packInfos) {
        this.packInfos = packInfos;
    }

    void setProfileInfos(List<ProfileInfo> profileInfos) {
        this.profileInfos = profileInfos;
    }

    void setPackInfoStr(String packInfoStr) {
        this.packInfoStr = packInfoStr;
    }

    void setProfileInfosStr(List<String> profileInfosStr) {
        this.profileInfosStr = profileInfosStr;
    }

    void setLabel(String label) {
        this.label = label;
    }

    void setIcon(String icon) {
        this.icon = icon;
    }

    /**
     * Add the profileInfo to the profileInfo list.
     *
     * @param profileInfo the profileInfo to be added.
     */
    void addProfileInfo(ProfileInfo profileInfo) {
        if (profileInfos != null && profileInfo != null) {
            this.profileInfos.add(profileInfo);
        }
    }

    /**
     * Add the profileInfoStr to the profileInfoStr list.
     *
     * @param profileInfoStr the profileInfoStr to be added.
     */
    void addProfileInfoStr(String profileInfoStr) {
        if (profileInfosStr != null && profileInfoStr != null && !profileInfoStr.isEmpty()) {
            this.profileInfosStr.add(profileInfoStr);
        }
    }

    public boolean getResult() {
        return result;
    }

    public String getMessage() {
        return message;
    }

    public List<PackInfo> getPackInfos() {
        return packInfos;
    }

    public List<ProfileInfo> getProfileInfos() {
        return profileInfos;
    }

    public String getPackInfoStr() {
        return packInfoStr;
    }

    public List<String> getProfileInfosStr() {
        return profileInfosStr;
    }

    public String getLabel() {
        return label;
    }

    public String getIcon() {
        return icon;
    }
}

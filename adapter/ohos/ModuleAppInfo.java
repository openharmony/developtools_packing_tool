package ohos;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.List;

/**
 * ModuleJson AppJson info.
 *
 */
class ModuleAppInfo {
    private final Integer DEFAULT_VERSION_CODE = -1;
    private final String RELEASE = "Release";
    private final String UNSPECIFIED = "unspecified";
    /**
     * Indicates the bundleName of app AppJson.
     */
    public String bundleName = "";
    /**
     * Indicates the debug of app AppJson.
     */
    public boolean debug = false;
    /**
     * Indicates the icon of app AppJson.
     */
    public String icon = "";
    /**
     * Indicates the label of app AppJson.
     */
    public String label = "";
    /**
     * Indicates the description of app AppJson.
     */
    public String description = "";
    /**
     * Indicates the vendor of app AppJson.
     */
    public String vendor = "";
    /**
     * Indicates the versionCode of app AppJson.
     */
    public int versionCode = DEFAULT_VERSION_CODE;
    /**
     * Indicates the versionName of app AppJson.
     */
    public String versionName = "";
    /**
     * Indicates the minCompatibleVersionCode of app AppJson.
     */
    public int minCompatibleVersionCode = DEFAULT_VERSION_CODE;
    /**
     * Indicates the minAPIVersion of app AppJson.
     */
    public int minAPIVersion = DEFAULT_VERSION_CODE;
    /**
     * Indicates the targetAPIVersion of app AppJson.
     */
    public int targetAPIVersion = DEFAULT_VERSION_CODE;
    /**
     * Indicates the apiReleaseType of app AppJson.
     */
    public String apiReleaseType = RELEASE;
    /**
     * Indicates the distributedNotificationEnabled of app AppJson.
     */
    public boolean distributedNotificationEnabled = false;
    /**
     * Indicates the entityType of app AppJson.
     */
    public String entityType = UNSPECIFIED;
    /**
     * Indicates the deviceType of app AppJson.
     */
    public Map<String, ModuleDeviceType> deviceTypes = new HashMap<>();
}

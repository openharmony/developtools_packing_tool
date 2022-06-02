package ohos;

import java.util.ArrayList;
import java.util.List;

class VerifyCollection {
    /**
     * Indicates the bundleName of app.
     */
    public String bundleName = "";
    /**
     * Indicates the vendor of app.
     */
    public String vendor = "";
    /**
     * Indicates the versionCode of version.
     */
    public int versionCode = -1;
    /**
     * Indicates the versionName of version.
     */
    public String versionName = "";
    /**
     * Indicates the minCompatibleVersionCode of app.
     */
    public int minCompatibleVersionCode = -1;
    /**
     * Indicates the minApiVersion of app.
     */
    public int compatibleApiVersion = -1;
    /**
     * Indicates the targetApiVersion of app.
     */
    public int targetApiVersion = -1;
    /**
     * Indicates the apiReleaseType of app.
     */
    public String releaseType = "";
    /**
     * Indicates the moduleNames of app.
     */
    List<String> moduleNames = new ArrayList<>();
    /**
     * Indicates the packageNames of app.
     */
    List<String> packageNames = new ArrayList<>();
}

package ohos;

import java.util.List;
import java.util.ArrayList;

/**
 * Module result.
 *
 */
class ModuleResult {
    /**
     * Indicates the ModuleProfileInfo of ModuleJson.
     */
    public List<ModuleProfileInfo> moduleProfileInfos = new ArrayList<ModuleProfileInfo>();
    /**
     * Indicates the ModuleProfileInfo of ModuleJson.
     */
    public List<String> moduleProfileStr = new ArrayList<String>();
    /**
     * Indicates the icon of Module.
     */
    private String icon = "";
    /**
     * Indicates the label of Module.
     */
    private String label = "";
    /**
     * Indicates the result of ModuleJson.
     */
    private boolean result = true;
    /**
     * Indicates the message of ModuleJson.
     */
    private String message = "Success";

    public boolean getResult() {
        return result;
    }

    public void setResult(boolean result) {
        this.result = result;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getIcon() {
        return icon;
    }

    public void setIcon(String iconPath) {
        icon = iconPath;
    }

    public String getLabel() {
        return label;
    }

    public void setLabel(String labelRes) {
        label = labelRes;
    }

    /**
     * Add the ModuleProfileInfo to the profileInfoStr list.
     *
     * @param moduleProfileInfo the moduleProfileInfo to be added.
     */
    public void addModuleProfileInfo(ModuleProfileInfo moduleProfileInfo) {
        if (moduleProfileInfos != null && moduleProfileInfo != null) {
            this.moduleProfileInfos.add(moduleProfileInfo);
        }
    }
}

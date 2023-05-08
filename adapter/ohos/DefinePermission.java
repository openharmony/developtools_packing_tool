package ohos;

import java.util.HashMap;

/**
 * DefinePermission info.
 * @deprecated
 */
public class DefinePermission {
    /**
     * Indicates the name of DefPermission.
     */
    public String name = "";

    /**
     * Indicates the grantMode of DefPermission.
     */
    public String grantMode = "";

    /**
     * Indicates the availableLevel of DefPermission.
     */
    public String availableLevel = "";

    /**
     * Indicates the provisionEnable of ModuleDefinePermissions.
     */
    public boolean provisionEnable  = true;

    /**
     * Indicates the distributedSceneEnable of ModuleDefinePermissions.
     */
    public boolean distributedSceneEnable = false;

    /**
     * Indicates the label of DefPermission.
     */
    public String label = "";

    /**
     * Indicates the description of DefPermission.
     */
    public String description = "";

    private HashMap<String, String> descriptions = new HashMap<>();

    private HashMap<String, String> labels = new HashMap<>();

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }

    public HashMap<String, String> getLabels() {
        return labels;
    }

    public void setLabels(HashMap<String, String> labels) {
        this.labels = labels;
    }
}

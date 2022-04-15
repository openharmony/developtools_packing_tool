package ohos;

import java.sql.Array;
import java.util.ArrayList;
import java.util.List;

/**
 * ModuleJson ability info in module
 *
 */
public class ModuleAbilityInfo {
    /**
     * Indicates the name of module ModuleJson.
     */
    public String name = "";
    /**
     * Indicates the srcEntrance of module ModuleJson.
     */
    public String srcEntrance = "";
    /**
     * Indicates the launchType of module ModuleJson.
     */
    public String launchType = "standard";
    /**
     * Indicates the description of module ModuleJson.
     */
    public String description = "";
    /**
     * Indicates the icon of module ModuleJson.
     */
    public String icon = "";
    /**
     * Indicates the label of module ModuleJson.
     */
    public String label = "";
    /**
     * Indicates the permissions of module ModuleJson.
     */
    public List<String> permissions = new ArrayList<String>();
    /**
     * Indicates the metadata of module ModuleJson.
     */
    public List<ModuleMetadataInfo> metadata = new ArrayList<ModuleMetadataInfo>();
    /**
     * Indicates the visible of module ModuleJson.
     */
    public boolean visible = false;
    /**
     * Indicates the continuable of module ModuleJson.
     */
    public boolean continuable = false;
    /**
     * Indicates the skills of module ModuleJson.
     */
    public List<SkillInfo> skills = new ArrayList<SkillInfo>();
    /**
     * Indicates the backgroundModes of module ModuleJson.
     */
    public List<String> backgroundModes = new ArrayList<String>();
}

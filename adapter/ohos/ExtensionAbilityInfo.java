package ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * ModuleJson extensionAbility info.
 *
 */
public class ExtensionAbilityInfo {
    /**
     * Indicates the name of module ModuleJson.
     */
    public String name = "";
    /**
     * Indicates the srcEntrance of module ModuleJson.
     */
    public String srcEntrance = "";
    /**
     * Indicates the icon of module ModuleJson.
     */
    public String icon = "";
    /**
     * Indicates the label of module ModuleJson.
     */
    public String label = "";
    /**
     * Indicates the description of module ModuleJson.
     */
    public String description = "";
    /**
     * Indicates the type of module ModuleJson.
     */
    public String type = "";
    /**
     * Indicates the permissions of module ModuleJson.
     */
    public List<String> permissions = new ArrayList<>();
    /**
     * Indicates the readPermission of module ModuleJson.
     */
    public String readPermission = "";
    /**
     * Indicates the writePermission of module ModuleJson.
     */
    public String writePermission = "";
    /**
     * Indicates the visible of module ModuleJson.
     */
    public boolean visible = false;
    /**
     * Indicates the skill of module ModuleJson.
     */
    public List<SkillInfo> skills = new ArrayList<>();

    List<ModuleMetadataInfo> metadataInfos = new ArrayList<>();

    /**
     * Indicates the metadata of module ModuleJson.
     */
    public MetaData metadata = new MetaData();
    /**
     * Indicates the uri of module ModuleJson.
     */
    public String uri = "";
}

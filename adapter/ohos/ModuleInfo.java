package ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * Module ModuleJson info.
 *
 */
class ModuleInfo {
    private Integer INSTALLFREE_UNDEFINED = 2;
    private String DEFAULT = "default";
    private String HML = "hml";
    /**
     * Indicates the hap name of ModuleInfo.
     */
    public String hapName = "";
    /**
     * Indicates the name of ModuleInfo.
     */
    public String name = "";
    /**
     * Indicates the type of ModuleInfo.
     */
    public String type = "";
    /**
     * Indicates the srcEntrance of ModuleInfo.
     */
    public String srcEntrance = "";
    /**
     * Indicates the description of ModuleInfo.
     */
    public String description = "";
    /**
     * Indicates the process of ModuleInfo.
     */
    public String process = "";
    /**
     * Indicates the mainElement of ModuleInfo.
     */
    public String mainElement = "";
    /**
     * Indicates the devicesTypes of ModuleInfo.
     */
    public List<String> deviceTypes = new ArrayList<String>();
    /**
     * Indicates the deliveryWithInstall of ModuleInfo.
     */
    public boolean deliveryWithInstall = true;
    /**
     * Indicates the installationFree of ModuleInfo.
     */
    public int installationFree = INSTALLFREE_UNDEFINED;
    /**
     * Indicates the virtualMachine of ModuleInfo.
     */
    public String virtualMachine = DEFAULT;
    /**
     * Indicates the uiSyntax of ModuleInfo.
     */
    public String uiSyntax = HML;
    /**
     * Indicates the pages of ModuleInfo.
     */
    public List<String> pages = new ArrayList<String>();
    /**
     * Indicates the metadata of ModuleInfo.
     */
    public List<ModuleMetadataInfo> moduleMetadataInfos = new ArrayList<ModuleMetadataInfo>();
    /**
     * Indicates the abilities of ModuleInfo.
     */
    public List<ModuleAbilityInfo> abilities = new ArrayList<ModuleAbilityInfo>();
    /**
     * Indicates the extensionAbilities of ModuleInfo.
     */
    public List<ExtensionAbilityInfo> extensionAbilityInfos = new ArrayList<ExtensionAbilityInfo>();
    /**
     * Indicates the requestPermissions of ModuleInfo.
     */
    public List<ReqPermission> requestPermissions = new ArrayList<ReqPermission>();
    /**
     * Indicates the shortcuts of ModuleInfo.
     */
    public List<ModuleShortcut> moduleShortcuts = new ArrayList<>();
    /**
     * Indicates the distrofilter of ModuleInfo.
     */
    public DistroFilter distroFilter = new DistroFilter();
    /**
     * Indicates the common events of ModuleInfo.
     */
    public List<CommonEvent> commonEvents = new ArrayList<CommonEvent>();
    /**
     * Indicates the common events of ModuleInfo.
     */
    List<AbilityFormInfo> abilityFormInfos = new ArrayList<>();
    /**
     * Indicates the define permissions of ModuleInfo.
     */
    List<DefinePermission> definePermissions = new ArrayList<>();
}

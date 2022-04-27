package ohos;

import java.util.ArrayList;
import java.util.List;

/**
 * module Shortcut info.
 *
 */
class ModuleShortcut {
    /**
     * Indicates the shortcutId of module Shortcut.
     */
    public String shortcutId = "";

    /**
     * Indicates the label of module Shortcut.
     */
    public String label = "";

    /**
     * Indicates the icon of module Shortcut.
     */
    public String icon = "";

    /**
     * Indicates the intents of module Shortcut.
     */
    public List<Want> wants = new ArrayList<Want>();
}

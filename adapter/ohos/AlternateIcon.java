/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ohos;

/**
 * AlternateIcon info.
 *
 */
public class AlternateIcon {
    /**
     * Indicates the resource id of alternate icon.
     */
    public long iconId = 0L;

    /**
     * Indicates the name of alternate icon.
     */
    public String name = "";

    /**
     * Indicates the icon path of alternate icon.
     */
    public String icon = "";

    @Override
    public String toString() {
        return "AlternateIcon{"
                + "iconId=" + iconId
                + ", name='" + name + '\''
                + ", icon='" + icon + '\''
                + '}';
    }

    @Override
    public int hashCode() {
        int result = Long.hashCode(iconId);
        result = 31 * result + name.hashCode();
        result = 31 * result + icon.hashCode();
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        AlternateIcon other = (AlternateIcon) obj;
        return iconId == other.iconId && name.equals(other.name) && icon.equals(other.icon);
    }
}

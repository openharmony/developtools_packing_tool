/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import java.util.Objects;

public class MultiAppMode {
    private String multiAppModeType = "";
    private Integer maxCount = 0;

    public String getMultiAppModeType() {
        return multiAppModeType;
    }

    public void setMultiAppModeType(String multiAppModeType) {
        this.multiAppModeType = multiAppModeType;
    }

    public Integer getMaxCount() {
        return maxCount;
    }

    public void setMaxCount(Integer maxCount) {
        this.maxCount = maxCount;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        MultiAppMode that = (MultiAppMode) o;
        return Objects.equals(multiAppModeType, that.multiAppModeType) && Objects.equals(maxCount, that.maxCount);
    }

    @Override
    public int hashCode() {
        int result = Objects.hashCode(multiAppModeType);
        result = 31 * result + Objects.hashCode(maxCount);
        return result;
    }
}

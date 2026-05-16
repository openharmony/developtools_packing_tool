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

#ifndef PACKING_TOOL_ERROR_MSG_H
#define PACKING_TOOL_ERROR_MSG_H

#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace packing_tool {
namespace error {

class ErrorMsg {
public:
    using SolutionList = std::vector<std::string>;

    class Builder {
    public:
        explicit Builder(const std::string& sysCode);

        Builder& setErrCode(const std::string& errCode);
        Builder& setTypeCode(const std::string& typeCode);
        Builder& setDescription(const std::string& description);
        Builder& setCause(const std::string& cause);
        Builder& addSolution(const std::string& solution);

        ErrorMsg build() const;

    private:
        std::string sysCode_;
        std::string errCode_;
        std::string typeCode_;
        std::string description_;
        std::string cause_;
        SolutionList solutions_;
    };

    ErrorMsg(const std::string& code,
        const std::string& description,
        const std::string& cause,
        const SolutionList& solutions);

    const std::string& getCode() const { return code_; }
    const std::string& getDescription() const { return description_; }
    const std::string& getCause() const { return cause_; }
    const SolutionList& getSolutions() const { return solutions_; }

    std::string toString() const;
    std::string toStringWithArgs(const std::string& arg) const;
    std::string toStringWithArgs(const std::vector<std::string>& args) const;

    static Builder getPackingToolErrBuilder();

private:
    static constexpr const char* PACKING_TOOL_SUB_SYSTEM_CODE = "100";

    std::string code_;
    std::string description_;
    std::string cause_;
    SolutionList solutions_;
};

} // namespace error
} // namespace packing_tool

#endif // PACKING_TOOL_ERROR_MSG_H

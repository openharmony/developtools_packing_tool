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

#include "error_msg.h"
  #include <algorithm>

  namespace packing_tool {
  namespace error {

  // Builder 实现
  ErrorMsg::Builder::Builder(const std::string& sysCode) : sysCode_(sysCode) {}

  ErrorMsg::Builder& ErrorMsg::Builder::setErrCode(const std::string& errCode) {
      errCode_ = errCode;
      return *this;
  }

  ErrorMsg::Builder& ErrorMsg::Builder::setTypeCode(const std::string& typeCode) {
      typeCode_ = typeCode;
      return *this;
  }

  ErrorMsg::Builder& ErrorMsg::Builder::setDescription(const std::string& description) {
      description_ = description;
      return *this;
  }

  ErrorMsg::Builder& ErrorMsg::Builder::setCause(const std::string& cause) {
      cause_ = cause;
      return *this;
  }

  ErrorMsg::Builder& ErrorMsg::Builder::addSolution(const std::string& solution) {
      solutions_.push_back(solution);
      return *this;
  }

  ErrorMsg ErrorMsg::Builder::build() const {
      std::string fullCode = sysCode_ + typeCode_ + errCode_;
      return ErrorMsg(fullCode, description_, cause_, solutions_);
  }

  // ErrorMsg 实现
  ErrorMsg::ErrorMsg(const std::string& code,
                     const std::string& description,
                     const std::string& cause,
                     const SolutionList& solutions)
      : code_(code)
      , description_(description)
      , cause_(cause)
      , solutions_(solutions) {}

  ErrorMsg::Builder ErrorMsg::getPackingToolErrBuilder() {
      return Builder(PACKING_TOOL_SUB_SYSTEM_CODE);
  }

  std::string ErrorMsg::toString() const {
      std::ostringstream oss;
      oss << code_ << " " << description_ << "\n"
          << "Error Message: " << cause_ << "\n";

      if (!solutions_.empty()) {
          oss << "\n* Try the following:\n";
          for (const auto& s : solutions_) {
              oss << " > " << s << "\n";
          }
      }
      return oss.str();
  }

  std::string ErrorMsg::toStringWithArgs(const std::string& arg) const {
      return toStringWithArgs(std::vector<std::string>{arg});
  }

  std::string ErrorMsg::toStringWithArgs(const std::vector<std::string>& args) const {
      std::string result = toString();
      // 简单的占位符替换，将 %s 替换为参数
      size_t pos = 0;
      for (const auto& arg : args) {
          pos = result.find("%s", pos);
          if (pos != std::string::npos) {
              result.replace(pos, 2, arg);
              pos += arg.length();
          }
      }
      return result;
  }

  } // namespace error
  } // namespace packing_tool
 #ifndef PACKING_TOOL_ERROR_MSG_H
  #define PACKING_TOOL_ERROR_MSG_H

  #include <string>
  #include <vector>
  #include <sstream>
  #include <memory>

  namespace packing_tool {
  namespace error {

  /**
   * @brief 错误消息类
   *
   * 包含错误码、描述、原因和解决方案
   */
  class ErrorMsg {
  public:
      using SolutionList = std::vector<std::string>;

      /**
       * @brief Builder模式构建器
       */
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

      // 构造函数
      ErrorMsg(const std::string& code,
               const std::string& description,
               const std::string& cause,
               const SolutionList& solutions);

      // 获取器
      const std::string& getCode() const { return code_; }
      const std::string& getDescription() const { return description_; }
      const std::string& getCause() const { return cause_; }
      const SolutionList& getSolutions() const { return solutions_; }

      // 格式化输出
      std::string toString() const;
      std::string toStringWithArgs(const std::string& arg) const;
      std::string toStringWithArgs(const std::vector<std::string>& args) const;

      // 静态工厂方法
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
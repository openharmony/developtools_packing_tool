/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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
 import java.util.ArrayList;
 import java.util.List;
 import java.util.Locale;
 import java.util.MissingFormatArgumentException;
 
 /**
  * ErrorMsg
  *
  * @since 2025/01/21
  */
 public class ErrorMsg {
     private static final Log LOG = new Log(ErrorMsg.class.toString());
 
     private static final String PACKING_TOOL_SUB_SYSTEM_CODE = "100";
 
     private final String code;
 
     private final String description;
 
     private final String cause;

     private final List<String> solutions;
 
     /**
      * ErrorMsg constructor
      *
      * @param code code
      * @param description description
      * @param cause cause
      * @param solutions solutions
      */
     public ErrorMsg(String code, String description, String cause,
                     List<String> solutions) {
         this.code = code;
         this.description = description;
         this.cause = cause;
         this.solutions = solutions;
     }
 
     /**
      * getPackingToolErrBuilder
      *
      * @return Builder
      */
     public static Builder getPackingToolErrBuilder() {
         return new Builder(PACKING_TOOL_SUB_SYSTEM_CODE);
     }
 
 
     @Override
     public String toString() {
         final StringBuilder sb = new StringBuilder();
         sb.append(code)
                 .append(" ")
                 .append(description)
                 .append(System.lineSeparator())
                 .append("Error Message: ")
                 .append(cause)
                 .append(System.lineSeparator());
 
         if (solutions != null && !solutions.isEmpty()) {
             sb.append(System.lineSeparator()).append("* Try the following: ").append(System.lineSeparator());
             for (String s : solutions) {
                 sb.append(" > ").append(s).append(System.lineSeparator());
             }
         }
         return sb.toString();
     }
 
     /**
      * to String
      *
      * @param args args
      * @return String
      */
     public String toString(Object... args) {
         try {
             return String.format(Locale.ROOT, this.toString(), args);
         } catch (MissingFormatArgumentException e) {
             LOG.error("args format failed: " + args);
             return this.toString();
         }
     }
 
     /**
      * Builder
      *
      * @since 2025/01/21
      */
     public static class Builder {
         private String sysCode;
 
         private String errCode;
 
         private String typeCode;
 
         private String description;
 
         private String cause;
 
         private List<String> solutions = new ArrayList<>();
 
         /**
          * Builder Constructor
          *
          * @param sysCode sysCode
          */
         public Builder(String sysCode) {
             this.sysCode = sysCode;
         }
 
         /**
          * setErrCode
          *
          * @param errCode errCode
          * @return Builder
          */
         public Builder setErrCode(String errCode) {
             this.errCode = errCode;
             return this;
         }
 
         /**
          * setTypeCode
          *
          * @param typeCode typeCode
          * @return Builder
          */
         public Builder setTypeCode(String typeCode) {
             this.typeCode = typeCode;
             return this;
         }
 
         /**
          * setDescription
          *
          * @param description description
          * @return Builder
          */
         public Builder setDescription(String description) {
             this.description = description;
             return this;
         }
 
         /**
          * setCause
          *
          * @param cause cause
          * @return Builder
          */
         public Builder setCause(String cause) {
             this.cause = cause;
             return this;
         }
 
         /**
          * addSolution
          *
          * @param solution solution
          * @return Builder
          */
         public Builder addSolution(String solution) {
             this.solutions.add(solution);
             return this;
         }
 
         /**
          * build
          *
          * @return ErrorMsg
          */
         public ErrorMsg build() {
             return new ErrorMsg(sysCode + typeCode + errCode, description, cause, solutions);
         }
     }
 }
 
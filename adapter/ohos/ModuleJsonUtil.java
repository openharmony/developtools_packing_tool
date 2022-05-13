/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import ohos.utils.fastjson.JSON;
import ohos.utils.fastjson.JSONObject;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

import java.util.Locale;

public class ModuleJsonUtil {
    private static final String  VERSION_CODE = "\"versionCode\"";
    private static final String VERSION_NAME = "\"versionName\"";
    private static final String APP = "app";
    private static final String VERSIONCODE = "versionCode";
    private static final String VERSIONNAME = "versionName";
    private static final String VERSION = "version";
    private static final String CODE = "code";
    private static final String NAME = "name";
    private static final String MODULE = "module";
    private static final String MODULENAME = "moduleName";
    private static final String DISTRO = "distro";
    private static final String PACKAGE = "package";
    private static final Log LOG = new Log(ModuleJsonUtil.class.toString());

    /**
     * get the versionCode from json file.
     *
     * @param jsonString uncompress json object
     * @return the result
     */
     public static Version getVersion(String jsonString) throws BundleException {
         Version version = new Version();
         try {
             if (jsonString != null) {
                // find versionName
                int indexOfVersionName = jsonString.indexOf(VERSION_NAME);
                if (indexOfVersionName <= 0) {
                    LOG.error("ModuleJsonUtil::getVersion obtain index of VERSION_NAME failed");
                    return version;
                }
                indexOfVersionName += VERSION_NAME.length();
                int nameStart = jsonString.indexOf("\"", indexOfVersionName);
                int nameEnd = nameStart + 1;
                while(jsonString.charAt(nameEnd) != '\"') {
                    ++nameEnd;
                }
                version.versionName = jsonString.substring(nameStart + 1, nameEnd).trim();
                //find versionCode
                int indexOfVersionCode = jsonString.indexOf(VERSION_CODE);
                if (indexOfVersionCode <= 0) {
                    LOG.error("ModuleJsonUtil::getVersion obtain index of VERSION_CODE failed");
                    return version;
                }
                indexOfVersionCode += VERSION_CODE.length();
                while(!Character.isDigit(jsonString.charAt(indexOfVersionCode))) {
                    ++indexOfVersionCode;
                }
                String versionCode = "";
                while(Character.isDigit(jsonString.charAt(indexOfVersionCode))) {
                    versionCode += jsonString.charAt(indexOfVersionCode);
                    ++indexOfVersionCode;
                }
                version.versionCode = Integer.parseInt(versionCode.trim());
            }
         } catch (NumberFormatException exception) {
             LOG.error("ModuleJsonUtil::getVersion NumberFormatException exception: " + exception.getMessage());
             throw new BundleException("ModuleJsonUtil::getVersion failed");
         }
         return version;
     }

    /**
     * get the version from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static Version parseStageVersion(String jsonString) throws BundleException {
        Version version = new Version();
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
            if (jsonObject.containsKey(APP)) {
                JSONObject appObj = jsonObject.getJSONObject(APP);
                if (appObj.containsKey(VERSIONCODE) && appObj.containsKey(VERSIONNAME)) {
                    version.versionCode = appObj.getIntValue(VERSIONCODE);
                    version.versionName = appObj.getString(VERSIONNAME);
                } else {
                    LOG.error("parseStageVersion json file do not contain version.");
                    throw new BundleException("parseStageVersion json file do not contain version.");
                }
            } else {
                LOG.error("parseStageVersion json file do not contain app.");
                throw new BundleException("parseStageVersion json file do not contain app.");
            }
        } catch (BundleException e) {
            LOG.error("parseStageVersion failed!");
            throw new BundleException("parseStageVersion failed!");
        }

        return version;
    }

    /**
     * get the version from json file for fa module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static Version parseFaVersion(String jsonString) throws BundleException {
        Version version = new Version();
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
            JSONObject appObj = jsonObject.getJSONObject(APP);
            if (appObj == null) {
                LOG.error("parseFaVersion failed : json file do not contain app.");
                throw new BundleException("parseFaVersion failed : json file do not contain app.");
            }
            JSONObject versionObj = appObj.getJSONObject(VERSION);
            if (versionObj == null) {
                LOG.error("parseFaVersion failed : json file do not version.");
                throw new BundleException("parseFaVersion failed : json file do not version.");
            }
            if (versionObj.containsKey(CODE) && versionObj.containsKey(NAME)) {
                version.versionName = versionObj.getString(NAME);
                version.versionCode = versionObj.getIntValue(CODE);
            } else {
                LOG.error("parseFaVersion failed : json file do not version name or version code.");
                throw new BundleException("parseFaVersion failed : json file do not version name or version code.");
            }
        } catch (BundleException e) {
            throw new BundleException("parseFaVersion failed!");
        }
        return version;
    }

    /**
     * get the module name from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseStageModuleName(String jsonString) throws BundleException {
        String moduleName = "";
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
            JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
            if (moduleObj == null) {
                LOG.error("parseStageModuleName failed: json file do not contain module.");
                throw new BundleException("parseStageModuleName failed: json file do not contain module.");
            }
            if (moduleObj.containsKey(NAME)) {
                moduleName = moduleObj.getString(NAME);
            } else {
                LOG.error("parseStageModuleName failed: json file do not contain module name.");
                throw new BundleException("parseStageModuleName failed: json file do not contain module name.");
            }
        } catch (BundleException e) {
            LOG.error("parseStageModuleName failed!");
            throw new BundleException("parseStageModuleName failed!");
        }
        return moduleName;
    }

    /**
     * get the moduleName from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseFaModuleName(String jsonString) throws BundleException {
        String moduleName = "";
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
            JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
            if (moduleObj == null) {
                LOG.error("parseFaModuleName failed: json file do not contain module.");
                throw new BundleException("parseFaModuleName failed: json file do not contain module.");
            }
            JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
            if (distroObj == null) {
                LOG.error("parseFaModuleName failed: json file do not contain distro.");
                throw new BundleException("parseFaModuleName failed: json file do not contain distro.");
            }
            if (!distroObj.containsKey(MODULENAME)) {
                LOG.error("parseFaModuleName failed: json file do not contain moduleName.");
                throw new BundleException("parseFaModuleName failed: json file do not contain moduleName.");
            }
            moduleName = distroObj.getString(MODULENAME);
        } catch (BundleException e) {
            LOG.error("parseFaModuleName failed!");
            throw new BundleException("parseFaModuleName failed!");
        }
        return moduleName;
    }

    /**
     * get the package from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseFaPackageStr(String jsonString) throws BundleException {
        String packageStr = "";
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
            JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
            if (moduleObj == null) {
                LOG.error("parseFaPackageStr failed: json file do not contain module.");
                throw new BundleException("parseFaPackageStr failed: json file do not contain module.");
            }
            if (moduleObj.containsKey(PACKAGE)) {
                packageStr = moduleObj.getString(PACKAGE);
            } else {
                LOG.error("parseFaPackageStr failed: json file do not contain package.");
                throw new BundleException("parseFaPackageStr failed: json file do not contain package.");
            }
        } catch (BundleException e) {
            LOG.error("parseFaPackageStr failed!");
            throw new BundleException("parseFaPackageStr failed!");
        }
        return packageStr;
    }
}

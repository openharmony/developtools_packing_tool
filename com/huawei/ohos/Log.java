/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 */

package com.huawei.ohos;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Class used to print logs and will remove if the official log module is ready.
 *
 * @author huzeqi
 * @since 2019-10-25
 */
public class Log {
    private static Logger log;

    /**
     * A constructor used to create a Log.
     *
     * @param className class name
     */
    public Log(String className) {
        log = Logger.getLogger(className);
        log.setLevel(Level.WARNING);
    }

    /**
     * print log in DEBUG level.
     *
     * @param msg log's content
     */
    public void debug(String msg) {
        log.info(LogType.buildTag(LogType.DEBUG) + msg);
    }

    /**
     * print log in INFO level.
     *
     * @param msg log's content
     */
    public void info(String msg) {
        log.info(LogType.buildTag(LogType.INFO) + msg);
    }

    /**
     * print log in ERROR level.
     *
     * @param msg log's content
     */
    public void error(String msg) {
        log.warning(LogType.buildTag(LogType.ERROR) + msg);
    }
}

enum LogType {
    DEBUG("Debug"),
    INFO("Info"),
    ERROR("Error");

    private String value;

    LogType(String value) {
        this.value = value;
    }

    /**
     * get log tag
     *
     * @param logType log type
     * @return format log tag
     */
    public static String buildTag(LogType logType) {
        return "HarmonyOS BundleTool [" + logType.value + "]: ";
    }
}


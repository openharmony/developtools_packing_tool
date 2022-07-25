package ohos;

class ModuleApiVersion {
    /**
     * Indicates the compatibleApiVersion of app.
     */
    private int compatibleApiVersion = -1;
    /**
     * Indicates the compatibleApiVersion of app.
     */
    private int targetApiVersion = -1;
    /**
     * Indicates the releaseType of app.
     */
    private String releaseType = "";

    public String getReleaseType() {
        return releaseType;
    }

    public void setReleaseType(String releaseType) {
        this.releaseType = releaseType;
    }

    public int getCompatibleApiVersion() {
        return compatibleApiVersion;
    }

    public void setCompatibleApiVersion(int compatibleApiVersion) {
        this.compatibleApiVersion = compatibleApiVersion;
    }

    public int getTargetApiVersion() {
        return targetApiVersion;
    }

    public void setTargetApiVersion(int targetApiVersion) {
        this.targetApiVersion = targetApiVersion;
    }
}

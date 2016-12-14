package com.axolotl.presentation.model;

public class PackageColour {
    private final int code;
    private final String name;

    public PackageColour(int code, String name) {
        this.code = code;
        this.name = name;
    }

    public int getCode() {
        return code;
    }

    public String getName() {
        return name;
    }
}

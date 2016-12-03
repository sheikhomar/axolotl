package com.axolotl.presentation.model;

import android.graphics.Color;

public class PackageColour {
    private final int code;

    private final int paintColour;
    private final String name;
    private static PackageColour[] Colours = {
            new PackageColour(0, Color.RED, "Red"),
            new PackageColour(1, Color.BLUE, "Blue"),
            new PackageColour(2, Color.GREEN, "Green"),
            new PackageColour(128, Color.BLACK, "Unknown"),
    };

    private PackageColour(int code, int paintColour, String name) {
        this.code = code;
        this.paintColour = paintColour;
        this.name = name;
    }

    public static PackageColour parse(int code) {
        for (int i = 0; i < Colours.length; i++)
            if (Colours[i].getCode() == code)
                return Colours[i];

        return null;
    }

    public int getCode() {
        return code;
    }

    public int getPaintColour() {
        return paintColour;
    }

    public String getName() {
        return name;
    }
}

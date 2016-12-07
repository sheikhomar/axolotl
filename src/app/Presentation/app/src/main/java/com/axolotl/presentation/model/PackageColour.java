package com.axolotl.presentation.model;

import android.graphics.Color;

public class PackageColour {
    private final int code;

    private final int paintColour;
    private final String name;
    private static PackageColour[] Colours = {
            new PackageColour(0, Color.argb(255, 214, 24, 0), "Red"),
            new PackageColour(2, Color.argb(255, 0, 72, 181), "Blue"),
            new PackageColour(1, Color.argb(255, 0, 160, 18), "Green"),
            new PackageColour(3, Color.argb(255, 237, 197, 0), "Yellow"),
            new PackageColour(6, Color.argb(255, 244, 244, 244), "White"),
            new PackageColour(7, Color.argb(255, 35, 16, 0), "Black"),
            new PackageColour(128, Color.argb(255, 255, 0, 102), "Unknown"),
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

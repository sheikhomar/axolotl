package com.axolotl.presentation.model;

public class ColourCodeParser {
    private PackageColour[] Colours = {
            new PackageColour(0, "Red"),
            new PackageColour(2, "Blue"),
            new PackageColour(1, "Green"),
            new PackageColour(3, "Yellow"),
            new PackageColour(6, "White"),
            new PackageColour(7, "Black"),
            new PackageColour(128, "Unknown"),
    };

    public PackageColour parse(int code) {
        for (int i = 0; i < Colours.length; i++)
            if (Colours[i].getCode() == code)
                return Colours[i];

        return null;
    }

    public boolean exists(int colourCode) {
        return parse(colourCode) != null;
    }
}

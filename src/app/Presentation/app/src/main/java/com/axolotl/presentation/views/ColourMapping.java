package com.axolotl.presentation.views;

import android.graphics.Color;

import com.axolotl.presentation.model.PackageColour;

public class ColourMapping {
    private static ColourMapping[] Colours = {
            new ColourMapping(0, Color.argb(255, 214, 24, 0)),
            new ColourMapping(2, Color.argb(255, 0, 72, 181)),
            new ColourMapping(1, Color.argb(255, 0, 160, 18)),
            new ColourMapping(3, Color.argb(255, 237, 197, 0)),
            new ColourMapping(6, Color.argb(255, 244, 244, 244)),
            new ColourMapping(7, Color.argb(255, 35, 16, 0)),
            new ColourMapping(128, Color.argb(255, 255, 0, 102)),
    };

    private final int code;
    private final int paintColour;

    public ColourMapping(int code, int paintColour) {
        this.code = code;
        this.paintColour = paintColour;
    }

    public int getCode() {
        return code;
    }

    public int getPaintColour() {
        return paintColour;
    }

    public static int getPaintColour(PackageColour packageColour) {
        for (int i = 0; i < Colours.length; i++) {
            if (packageColour.getCode() == Colours[i].getCode()) {
                return Colours[i].getPaintColour();
            }
        }

        return Color.argb(250, 250, 0, 102);
    }
}

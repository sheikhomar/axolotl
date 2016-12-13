package com.axolotl.presentation.model;

public class PackedPackage {

    private final Package thePackage;
    private final int x;
    private final int y;

    public PackedPackage(Package thePackage, int x, int y) {
        this.thePackage = thePackage;
        this.x = x;
        this.y = y;
    }

    public Package getPackage() {
        return thePackage;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }
}

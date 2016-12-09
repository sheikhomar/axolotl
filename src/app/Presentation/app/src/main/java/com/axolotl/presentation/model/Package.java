package com.axolotl.presentation.model;

public class Package {
    private final int number;
    private final PackageDimension dimension;
    private final PackageDimension realDimension;
    private final PackageColour colour;
    private final boolean isFragile;

    public Package(int number, PackageDimension interpretedDimension, PackageDimension realDimension, PackageColour colour, boolean isFragile) {
        this.number = number;
        this.dimension = interpretedDimension;
        this.realDimension = realDimension;
        this.colour = colour;
        this.isFragile = isFragile;
    }

    public PackageColour getColour() {
        return colour;
    }

    public PackageDimension getDimension() {
        return dimension;
    }

    public PackageDimension getRealDimension() {
        return realDimension;
    }

    public boolean isFragile() {
        return isFragile;
    }

    public int getNumber() {
        return number;
    }
}

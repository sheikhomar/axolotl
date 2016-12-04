package com.axolotl.presentation.model;

// The PackageDimension class encapsulates the width, length and height of a package.
public class PackageDimension {
    private final int length;
    private final int width;
    private final int height;

    public PackageDimension(int length, int width, int height) {
        this.length = length;
        this.width = width;
        this.height = height;
    }

    public int getLength() {
        return length;
    }

    public int getHeight() {
        return height;
    }

    public int getWidth() {
        return width;
    }

    @Override
    public String toString() {
        StringBuffer buffer = new StringBuffer();
        buffer.append(getLength());
        buffer.append(" x ");
        buffer.append(getWidth());
        buffer.append(" x ");
        buffer.append(getHeight());
        return buffer.toString();
    }
}

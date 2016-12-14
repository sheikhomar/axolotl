package com.axolotl.presentation.model;

import java.util.ArrayList;

public class Layer {
    private final ArrayList<PackedPackage> packedPackages;
    private final int length;
    private final int width;

    public Layer(int length, int width) {
        if (length < 1 || width < 1) {
            throw new IllegalArgumentException("Length/width cannot be zero or negative.");
        }
        this.length = length;
        this.width = width;
        this.packedPackages = new ArrayList<>();
    }

    public void pack(Package thePackage, int x, int y) {
        packedPackages.add(new PackedPackage(thePackage, x, y));
    }

    public int getNumberOfPackages() {
        return packedPackages.size();
    }

    public PackedPackage get(int index) {
        return packedPackages.get(index);
    }

    public int getLength() {
        return length;
    }

    public int getWidth() {
        return width;
    }

    public Package select(int x, int y) {
        for (int i = 0; i < packedPackages.size(); i++) {
            PackedPackage packToCheck = packedPackages.get(i);

            if (packToCheck.getX() <= x &&
                    x < packToCheck.getX() + packToCheck.getPackage().getDimension().getLength() &&
                    packToCheck.getY() <= y &&
                    y < packToCheck.getY() + packToCheck.getPackage().getDimension().getWidth()) {

                return packToCheck.getPackage();
            }
        }

        return null;
    }
}

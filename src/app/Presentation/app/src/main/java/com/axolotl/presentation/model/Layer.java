package com.axolotl.presentation.model;

import java.util.ArrayList;

public class Layer {
    private final ArrayList<PackagedPackage> packedPackages;
    private final int length;
    private final int width;
    private int selectedIndex;

    public Layer(int length, int width) {
        this.length = length;
        this.width = width;
        this.packedPackages = new ArrayList<>();
        this.selectedIndex = -1;
    }

    public void pack(Package thePackage, int x, int y) {
        packedPackages.add(new PackagedPackage(thePackage, x, y));
        selectedIndex = packedPackages.size() - 1;
    }

    public int getNumberOfPackages() {
        return packedPackages.size();
    }

    public PackagedPackage get(int index) {
        return packedPackages.get(index);
    }

    public int getLength() {
        return length;
    }

    public int getWidth() {
        return width;
    }

    public boolean isPackageSelected(int index) {
        return selectedIndex == index;
    }

    public Package select(int x, int y) {
        for (int i = 0; i < packedPackages.size(); i++) {
            PackagedPackage packToCheck = packedPackages.get(i);

            if (packToCheck.getX() <= x &&
                    x < packToCheck.getX() + packToCheck.getPackage().getDimension().getLength() &&
                    packToCheck.getY() <= y &&
                    y < packToCheck.getY() + packToCheck.getPackage().getDimension().getWidth()) {

                this.selectedIndex = i;

                return packToCheck.getPackage();
            }
        }

        this.selectedIndex = -1;
        return null;
    }
}

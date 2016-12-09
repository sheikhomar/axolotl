package com.axolotl.presentation.model;

import java.util.ArrayList;

public class Repository {
    private final ArrayList<Bin> bins;
    private int selectedBinIndex = 0;
    private int currentPackageNumber = 0;
    private Package selectedPackage;
    private boolean autoSelectNewPackage;

    public Repository() {
        this.bins = new ArrayList<>();
        this.autoSelectNewPackage = true;
    }

    public void createBin(int id, int length, int width, int numberOfLayers, String destination) {
        bins.add(new Bin(id, length, width, numberOfLayers, destination));
    }

    public void packPackage(int x, int y, int translatedLength, int translatedWidth, int translatedHeight, int measuredLength, int measuredWidth, int measuredHeight, int colourCode, boolean isFragile, int layer, int binId) {
        PackageDimension translated = new PackageDimension(translatedLength, translatedWidth, translatedHeight);
        PackageDimension measured = new PackageDimension(measuredLength, measuredWidth, measuredHeight);

        Bin bin = getBinById(binId);
        if (bin != null) {
            this.currentPackageNumber++;
            Package p = new Package(currentPackageNumber,
                    translated,
                    measured,
                    PackageColour.parse(colourCode),
                    isFragile,
                    bin.getDestination());

            bin.pack(p, layer, x, y);
            if (autoSelectNewPackage) {
                this.selectedBinIndex = bins.indexOf(bin);
                selectedPackage = p;
            }
        }
    }

    public Bin getBinById(int binId) {
        for (int i = 0; i < bins.size(); i++)
            if (bins.get(i).getId() == binId)
                return bins.get(i);

        return null;
    }

    public boolean binExists(int binId) {
        return getBinById(binId) != null;
    }

    public ArrayList<Bin> getBins() {
        return bins;
    }

    public void selectBin(Bin bin) {
        int binIndex = bins.indexOf(bin);
        if (binIndex != -1) {
            selectedBinIndex = binIndex;

            // Clear selected package, when a bin is selected.
            selectedPackage = null;
        }
    }

    public void selectLater(Layer layer) {
        Bin bin = getSelectedBin();
        if (bin != null) {
            bin.selectLater(layer);
            // Clear selected package, when a layer is selected.
            selectedPackage = null;
        }
    }

    public void selectPackage(Package aPackage) {
        if (this.selectedPackage == aPackage) {
            this.selectedPackage = null;
            this.autoSelectNewPackage = true;
        } else {
            this.selectedPackage = aPackage;
            this.autoSelectNewPackage = false;
        }
    }

    public Bin getSelectedBin() {
        if (bins.size() > 0) {
            return bins.get(selectedBinIndex);
        }

        return null;
    }

    public int getSelectedBinIndex() {
        return selectedBinIndex;
    }

    public Package getSelectedPackage() {
        return this.selectedPackage;
    }

    public Layer getSelectedLayer() {
        Bin bin = getSelectedBin();
        if (bin != null) {
            return bin.getSelectedLayer();
        }

        return null;
    }
}
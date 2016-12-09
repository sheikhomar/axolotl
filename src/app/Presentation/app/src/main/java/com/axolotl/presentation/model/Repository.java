package com.axolotl.presentation.model;

import java.util.ArrayList;

public class Repository {
    private final ArrayList<Bin> bins;
    private int currentPackageNumber = 0;
    private Package selectedPackage;
    private boolean autoSelectNewPackage;
    private Layer selectedLayer;
    private Bin selectedBin;

    public Repository() {
        this.bins = new ArrayList<>();
        this.autoSelectNewPackage = true;
    }

    public void createBin(int id, int length, int width, int numberOfLayers, String destination) {
        bins.add(new Bin(id, length, width, numberOfLayers, destination));
    }

    public void packPackage(int x, int y, int translatedLength, int translatedWidth, int translatedHeight, int measuredLength, int measuredWidth, int measuredHeight, int colourCode, boolean isFragile, int layerNo, int binId) {
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

            Layer layer = bin.pack(p, layerNo, x, y);
            if (autoSelectNewPackage) {
                this.selectedBin = bin;
                this.selectedPackage = p;
                this.selectedLayer = layer;
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
        return this.bins;
    }

    public void selectBin(Bin bin) {
        this.selectedBin = bin;
        this.selectedLayer = bin.getLayerAt(0);
        this.selectedPackage = null;
    }

    public void selectLater(Layer layer) {
        this.selectedLayer = layer;
        this.selectedPackage = null;
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
        return this.selectedBin;
    }

    public Package getSelectedPackage() {
        return this.selectedPackage;
    }

    public Layer getSelectedLayer() {
        return this.selectedLayer;
    }
}
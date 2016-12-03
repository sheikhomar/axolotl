package com.axolotl.presentation.model;

import java.util.ArrayList;

public class Repository {
    private final ArrayList<Bin> bins;
    private int selectedBinIndex = 0;

    public Repository() {
        bins = new ArrayList<>();
    }

    public void createBin(int id, int length, int width, int numberOfLayers, String destination) {
        bins.add(new Bin(id, length, width, numberOfLayers, destination));
    }

    public Bin getBinById(int binId) {
        for (int i = 0; i < bins.size(); i++)
            if (bins.get(i).getId() == binId)
                return bins.get(i);

        return null;
    }

    public Layer getCurrentLayer() {
        if (bins.size() > 0) {
            Bin currentBin = bins.get(selectedBinIndex);
            return currentBin.getCurrentLayer();
        }

        return null;
    }

    public ArrayList<Bin> getBins() {
        return bins;
    }

    public void selectBin(Bin bin) {
        int binIndex = bins.indexOf(bin);
        if (binIndex != -1) {
            selectedBinIndex = binIndex;
        }
    }
}

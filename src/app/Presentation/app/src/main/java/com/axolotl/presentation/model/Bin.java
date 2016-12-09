package com.axolotl.presentation.model;

import android.support.annotation.NonNull;

import java.util.ArrayList;

public class Bin {
    private final int id;
    private final int width;
    private final int length;
    private final int numberOfLayers;
    private final String destination;
    private final ArrayList<Layer> layers;

    public Bin(int id, int length, int width, int numberOfLayers, String destination) {
        this.id = id;
        this.width = width;
        this.length = length;
        this.numberOfLayers = numberOfLayers;
        this.destination = destination;
        this.layers = new ArrayList<>();
        for (int i = 0; i < numberOfLayers; i++) {
            this.layers.add(new Layer(length, width));
        }
    }

    public Layer pack(@NonNull Package thePackage, int layerNo, int x, int y) {
        if (thePackage == null) {
            throw new IllegalArgumentException("Package cannot be null.");
        }

        if (layerNo < 1 && layerNo > this.numberOfLayers) {
            throw new IllegalArgumentException("Invalid layer number: " + layerNo);
        }

        Layer layer = this.layers.get(layerNo - 1);
        layer.pack(thePackage, x, y);
        return layer;
    }

    public int getId() {
        return id;
    }

    public int getWidth() {
        return width;
    }

    public int getLength() {
        return length;
    }

    public int getNumberOfLayers() {
        return numberOfLayers;
    }

    public String getDestination() {
        return destination;
    }

    public Layer getLayerAt(int index) {
        return this.layers.get(index);
    }
}

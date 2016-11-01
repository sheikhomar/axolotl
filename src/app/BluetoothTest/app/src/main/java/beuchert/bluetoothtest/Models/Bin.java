package beuchert.bluetoothtest.Models;


import java.util.ArrayList;
import java.util.List;

public class Bin {
    public List<Layer> layers;
    public int amountOfLayers;

    public Bin(int numberOfLayers){
        amountOfLayers = numberOfLayers;
        layers = new ArrayList<Layer>();
    }

    public void addLayerToBin(Layer l){
        layers.add(l);
    }
}

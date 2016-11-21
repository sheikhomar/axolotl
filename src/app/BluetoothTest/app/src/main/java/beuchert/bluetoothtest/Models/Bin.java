package beuchert.bluetoothtest.Models;


import java.util.ArrayList;
import java.util.List;

public class Bin {
    public List<Layer> layers;
    public int amountOfLayers;
    public String destination;

    public Bin(int numberOfLayers, String dest){
        amountOfLayers = numberOfLayers;
        destination = dest;
        layers = new ArrayList<Layer>();
    }

    public void addLayerToBin(Layer l){
        layers.add(l);
    }
}

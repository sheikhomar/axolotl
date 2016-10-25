package beuchert.bluetoothtest.Models;


import java.util.ArrayList;
import java.util.List;

public class Bin {
    public List<Layer> layers;

    public Bin(){
        layers = new ArrayList<Layer>();
    }

    public void addLayerToBin(Layer l){
        layers.add(l);
    }
}

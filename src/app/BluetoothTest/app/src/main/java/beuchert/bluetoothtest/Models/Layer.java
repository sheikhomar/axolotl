package beuchert.bluetoothtest.Models;

import java.util.ArrayList;
import java.util.List;

public class Layer {
    public List<Package> Packages;

    public Layer(){
        Packages = new ArrayList<Package>();
    }

    public void addPackageToLayer(Package p){
        Packages.add(p);
    }
}

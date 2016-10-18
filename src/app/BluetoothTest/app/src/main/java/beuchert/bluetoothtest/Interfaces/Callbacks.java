package beuchert.bluetoothtest.Interfaces;

public interface Callbacks {
    public void showBluetoothConnectionAlert();
    public void showBluetoothAdapterAlert();
    public void showBluetoothNotEnabledAlert();
    public void showPackageContent(String Package);
    public void handlePackage(String Package);
}

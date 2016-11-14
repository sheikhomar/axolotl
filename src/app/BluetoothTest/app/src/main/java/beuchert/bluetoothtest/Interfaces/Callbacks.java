package beuchert.bluetoothtest.Interfaces;

public interface Callbacks {
    void showBluetoothConnectionAlert();
    void showBluetoothAdapterAlert();
    void showBluetoothNotEnabledAlert();
    void handlePackage(String Package);
    void isServiceReady(boolean ready);
}

package beuchert.bluetoothtest.Interfaces;

public interface Callbacks {
    public void updateClient(long data);
    public void showBluetoothConnectionAlert();
    public void showBluetoothAdapterAlert();
    public void showBluetoothNotEnabledAlert();
}

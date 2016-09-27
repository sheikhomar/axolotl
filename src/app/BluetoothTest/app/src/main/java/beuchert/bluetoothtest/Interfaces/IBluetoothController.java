package beuchert.bluetoothtest.Interfaces;

import java.util.UUID;

public interface IBluetoothController {
    public void Connect();
    public void Write();
    public void Read();
    public void Disconnect();
}

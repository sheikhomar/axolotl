package beuchert.bluetoothtest;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView statusText = (TextView) findViewById(R.id.StatusText);
        Button connectButton = (Button) findViewById(R.id.ConnectButton);
        Button disconnectButton = (Button) findViewById(R.id.DisconnectButton);
    }

    public void connectClick(View view) throws IOException {
        BluetoothAdapter BA = null;
        BluetoothSocket mSocket = null;
        final UUID MY_UUID = UUID.fromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");

        BA = BluetoothAdapter.getDefaultAdapter();
        BluetoothDevice device = BA.getRemoteDevice("EC:55:F9:FD:AF:BB");

        mSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
        mSocket.connect();

        boolean i = true;
        while (i){
            if(mSocket.isConnected()){
                InputStream result = mSocket.getInputStream();
                BufferedReader r = new BufferedReader(new InputStreamReader(result));
                String realResult = r.readLine();
                TextView statusText = (TextView) findViewById(R.id.StatusText);
                statusText.setText(realResult);

                i = false;
                mSocket.close();
            }
        }
    }
}

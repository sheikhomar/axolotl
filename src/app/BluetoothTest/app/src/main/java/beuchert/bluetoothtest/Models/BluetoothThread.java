package beuchert.bluetoothtest.Models;

import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import android.os.Handler;
import android.util.Log;

public class BluetoothThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private Handler mHandler;

    public BluetoothThread(BluetoothSocket socket, Handler handler) {
        Log.d("BluetoothThread", "BluetoothThread was created");
        mHandler = handler;
        mmSocket = socket;
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        // Get the input and output streams, using temp objects because
        // member streams are final
        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) { }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;
    }

    public void run() {
        Log.d("BluetoothThread", "BluetoothThread was started");
        byte[] buffer = new byte[1024];  // buffer store for the stream
        int bytes; // bytes returned from read()


        while (true) {
            try {
                // Read from the InputStream
                BufferedReader r = new BufferedReader(new InputStreamReader(mmInStream));
                String realResult = r.readLine();
                // Send the obtained bytes to the UI activity
                Message msg = mHandler.obtainMessage();
                msg.obj = realResult;
                msg.sendToTarget();
            } catch (IOException e) {
                Log.d("BluetoothThread", "BluetoothThread was ended");
                break;
            }
        }
    }

    /* Call this from the main activity to send data to the remote device */
    public void write(String message) {
        try {
            mmOutStream.write(message.getBytes());
        } catch (IOException e) {

        }
    }
}

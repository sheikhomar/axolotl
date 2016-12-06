package com.axolotl.presentation.communication;

import android.bluetooth.BluetoothSocket;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class BluetoothThread extends Thread {
    private final BluetoothSocket mmSocket;
    private ArrayList<Messenger> mClients;

    public BluetoothThread(BluetoothSocket socket, ArrayList<Messenger> mClients) {
        mmSocket = socket;
        this.mClients = mClients;
    }

    public void run() {
        InputStream is = null;
        try {
            is = mmSocket.getInputStream();
        } catch (IOException e) {
            Log.d("BluetoothThread", "An error while getting input stream: " + e.getMessage());
        }
        BufferedReader r = new BufferedReader(new InputStreamReader(is));

        while (true) {
            try {
                // Read from the InputStream
                String realResult = r.readLine();
                Log.d("BluetoothThread", "Recieved realResult: " + realResult);


                sendMessageToUI(realResult);
            } catch (IOException e) {
                Log.d("BluetoothThread", "BluetoothThread was ended " + e.getMessage());
                break;
            }
        }
    }

    private void sendMessageToUI(String valueToSend) {
        Log.d("BluetoothThread", "Number of clients: " + mClients.size());

        for (int i=mClients.size()-1; i>=0; i--) {
            try {
                Message msg = Message.obtain(null, BluetoothService.MSG_DATA_RECEIVED);
                msg.obj = valueToSend;
                mClients.get(i).send(msg);

            }
            catch (RemoteException e) {
                // The client is dead. Remove it from the list;
                // we are going through the list from back to
                // front so this is safe to do inside the loop.
                mClients.remove(i);
                Log.d("BluetoothThread", "Sending message failed for client " + i, e);
            }
        }
    }
}
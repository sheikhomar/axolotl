package com.axolotl.presentation.communication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
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
import java.util.UUID;

public class BluetoothThread extends Thread {

    private final String REMOTE_SERVER_MAC_ADDRESS = "00:0C:78:33:A5:63";
    private final UUID REMOTE_SERVICE_ID = UUID.fromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");

    private final Messenger messenger;

    public BluetoothThread(Messenger messenger) {
        this.messenger = messenger;
    }

    public void run() {
        BluetoothSocket socket = establishConnection();
        if (socket != null) {
            receiveData(socket);
        }
    }

    private void receiveData(BluetoothSocket socket) {
        InputStream is = null;
        try {
            is = socket.getInputStream();
        } catch (IOException e) {
            Log.d("BluetoothThread", "An error while getting input stream", e);

            Message msg = Message.obtain(null, Messages.CONNECTION_ERROR);
            msg.obj = "Error getting input stream.";
            sendMessageToUI(msg);
            return;
        }

        BufferedReader r = new BufferedReader(new InputStreamReader(is));

        while (true) {
            try {
                String receivedData = r.readLine();
                Log.d("BluetoothThread", "Received: " + receivedData);

                Message msg = Message.obtain(null, Messages.DATA_RECEIVED);
                msg.obj = receivedData;
                sendMessageToUI(msg);
            } catch (IOException e) {
                Log.d("BluetoothThread", "Connection was ended", e);
                Message msg = Message.obtain(null, Messages.CONNECTION_ERROR);
                msg.obj = "Connection was ended.";
                sendMessageToUI(msg);
                break;
            }
        }
    }

    private BluetoothSocket establishConnection() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (adapter == null) {
            Message msg = Message.obtain(null, Messages.CONNECTION_ERROR);
            msg.obj = "Device does not support Bluetooth.";
            sendMessageToUI(msg);
        } else if (!adapter.isEnabled()) {
            Message msg = Message.obtain(null, Messages.CONNECTION_ERROR);
            msg.obj = "Bluetooth is disabled.";
            sendMessageToUI(msg);
        } else {
            BluetoothDevice device = adapter.getRemoteDevice(REMOTE_SERVER_MAC_ADDRESS);
            BluetoothSocket socket = null;
            try {
                socket = device.createRfcommSocketToServiceRecord(REMOTE_SERVICE_ID);
                socket.connect();
            } catch (IOException e) {
                e.printStackTrace();
                Message msg = Message.obtain(null, Messages.CONNECTION_ERROR);
                msg.obj = "Remote server is down.";
                sendMessageToUI(msg);
            }

            return socket;
        }

        return null;
    }

    private void sendMessageToUI(Message message) {
        try {
            messenger.send(message);
        }
        catch (RemoteException e) {
            throw new Error("Cannot send message. Client is probably dead.", e);
        }
    }
}
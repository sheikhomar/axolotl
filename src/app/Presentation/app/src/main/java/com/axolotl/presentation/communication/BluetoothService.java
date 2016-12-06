package com.axolotl.presentation.communication;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.support.annotation.Nullable;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayList;
import java.util.UUID;

public class BluetoothService extends Service {
    public static final int MSG_REGISTER_CLIENT = 1;
    public static final int MSG_UNREGISTER_CLIENT = 2;
    public static final int MSG_DATA_RECEIVED = 3;
    public static final int MSG_CONNECT = 4;

    private static boolean isRunning = false;


    // Keeps track of all current registered clients.
    private ArrayList<Messenger> mClients = new ArrayList<>();
    final Messenger mMessenger = new Messenger(new IncomingHandler()); // Target we publish for clients to send messages to IncomingHandler.

    public static boolean isRunning() {
        return isRunning;
    }

    // Handler of incoming messages from clients.
    private class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_REGISTER_CLIENT:
                    mClients.add(msg.replyTo);
                    try {
                        mSocket = mDevice.createRfcommSocketToServiceRecord(myUUID);
                        mSocket.connect();
                        mBlueThread = new BluetoothThread(mSocket, mClients);
                        mBlueThread.start();

                    } catch (IOException e) {
                        Log.d(BluetoothService.class.getName(), "Could not connect to Bluetooth", e);
                    }
                    break;
                case MSG_UNREGISTER_CLIENT:
                    mClients.remove(msg.replyTo);
                    break;
                default:
                    super.handleMessage(msg);
            }
        }
    }

    private final IBinder mBinder;
    private final UUID myUUID = UUID.fromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");
    private final String bluetoothMacAdress = "00:0C:78:33:A5:63";
    private BluetoothAdapter mAdapter = null;
    private BluetoothDevice mDevice = null;
    private BluetoothSocket mSocket = null;
    private BluetoothThread mBlueThread = null;

    public BluetoothService(){
        mBinder = new LocalBinder();
        initializeAdapter();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(BluetoothService.class.getName(), "Created BluetoothService");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        Log.d(BluetoothService.class.getName(), "Received start id " + startId + ": " + intent);


        // We want this service to continue running until it is explicitly
        // stopped, so return sticky.
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mMessenger.getBinder();
    }

    private void initializeAdapter() {
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mAdapter == null){
            Log.d(BluetoothService.class.getName(), "Could not get default Bluetooth adapter");
        }
        else if (!mAdapter.isEnabled()){
            Log.d(BluetoothService.class.getName(), "Bluetooth is not enabled.");
        } else {
            mDevice = mAdapter.getRemoteDevice(bluetoothMacAdress);
        }
    }

    // Internal classes:
    private class LocalBinder extends Binder {
        public BluetoothService getServiceInstance(){
            return BluetoothService.this;
        }
    }
}
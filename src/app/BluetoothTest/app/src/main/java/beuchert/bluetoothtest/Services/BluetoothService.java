package beuchert.bluetoothtest.Services;

import android.app.Activity;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

import beuchert.bluetoothtest.Interfaces.Callbacks;
import beuchert.bluetoothtest.Models.BluetoothHandler;
import beuchert.bluetoothtest.Models.BluetoothThread;

public class BluetoothService extends Service {
    // Fields:
    private final IBinder mBinder;
    Callbacks activity;
    private final UUID myUUID = UUID.fromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");
    private final String bluetoothMacAdress = "00:0C:78:33:A5:63";
    private BluetoothAdapter mAdapter = null;
    private BluetoothDevice mDevice = null;
    private BluetoothSocket mSocket = null;
    private BluetoothHandler mHandler = null;
    private BluetoothThread mBlueThread = null;

    // Constructor
    public BluetoothService(){
        mBinder = new LocalBinder();
        mHandler = new BluetoothHandler();
        initializeAdapter();
        mDevice = mAdapter.getRemoteDevice(bluetoothMacAdress);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(BluetoothService.class.getName(), "Created BluetoothService");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        // We want this service to continue running until it is explicitly
        // stopped, so return sticky.
        Log.d(BluetoothService.class.getName(), "Started BluetoothService");

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    //Here Activity register to the service as Callbacks client
    public void registerClient(Activity activity){
        this.activity = (Callbacks) activity;
        mHandler.setActivity(this.activity);
        this.activity.isServiceReady(true);
    }

    // Methods:
    public void connect(){
        Log.d(BluetoothService.class.getName(), "Started connecting");
        try {
            mSocket = mDevice.createRfcommSocketToServiceRecord(myUUID);
            mSocket.connect();
            mBlueThread = new BluetoothThread(mSocket, mHandler);
            mBlueThread.start();
        } catch (IOException e) {
            activity.showBluetoothConnectionAlert();
        }
    }

    public void disconnect(){
        try {
            mSocket.close();
        } catch (IOException e) {
            activity.showBluetoothConnectionAlert();
        }
    }

    private void initializeAdapter(){
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mAdapter == null){
            activity.showBluetoothAdapterAlert();
        }
        else if (!mAdapter.isEnabled()){
            activity.showBluetoothNotEnabledAlert();
        }
    }

    // Internal classes:
    public class LocalBinder extends Binder {
        public BluetoothService getServiceInstance(){
            return BluetoothService.this;
        }
    }


}

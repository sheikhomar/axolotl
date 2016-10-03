package beuchert.bluetoothtest.Models;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;
import java.util.logging.Handler;

import beuchert.bluetoothtest.Interfaces.IBluetoothController;
import beuchert.bluetoothtest.R;

import static android.R.id.message;

public class BluetoothController extends Thread implements IBluetoothController {
    // Fields:
    private final UUID myUUID = UUID.fromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");
    private final String bluetoothMacAdress = "00:0C:78:33:A5:63";
    private BluetoothAdapter bluetoothAdapter = null;
    private BluetoothDevice mDevice = null;
    private BluetoothSocket mSocket = null;
    private final InputStream mInStream;
    private final OutputStream mOutStream;
    private View viewCalling;

    // Constructor:
    public BluetoothController(View view){
        viewCalling = view;
        InitializeBluetoothAdapter();
        mDevice = bluetoothAdapter.getRemoteDevice(bluetoothMacAdress);
        try {
            mSocket = mDevice.createRfcommSocketToServiceRecord(myUUID);
        } catch (IOException e) { BluetoothErrorMessage(); }

        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try {
            tmpIn = mSocket.getInputStream();
            tmpOut = mSocket.getOutputStream();
        } catch (IOException e) { BluetoothErrorMessage(); }
        mInStream = tmpIn;
        mOutStream = tmpOut;
    }

    // Interface methods:
    @Override
    public void Connect() {
        try {
            mSocket = mDevice.createRfcommSocketToServiceRecord(myUUID);
            mSocket.connect();
        } catch (IOException e) {
            BluetoothErrorMessage();
        }

        TextView statusText = (TextView) viewCalling.findViewById(R.id.StatusText);
        statusText.setText("Connected to: " + mDevice.getName());
    }

    @Override
    public void Disconnect() {
        TextView statusText = (TextView) viewCalling.findViewById(R.id.StatusText);
        statusText.setText("Status: Not Connected");

        try {
            mSocket.close();
        } catch (IOException e) {
            BluetoothErrorMessage();
        }
    }

    @Override
    public void Write() {

    }

    @Override
    public void Read() {
        /*
        if(mSocket.isConnected()){
            InputStream result = null;
            try {
                result = mSocket.getInputStream();
                BufferedReader r = new BufferedReader(new InputStreamReader(result));
                String realResult = r.readLine();
                PackageContent.setText(realResult);
            } catch (IOException e) {
                BluetoothErrorMessage();
                statusText.setText("Status: Not connected");
            }
        }
         */
    }

    // Methods:
    private void BluetoothErrorMessage(){
        TextView statusText = (TextView) viewCalling.findViewById(R.id.StatusText);
        statusText.setText("Status: Not Connected");

        AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(viewCalling.getContext());
        alertDiaglogBuilder.setTitle("Connection Error");
        alertDiaglogBuilder.setMessage("There was a bluetooth connection error (IOException).");
        alertDiaglogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        AlertDialog alertDialog = alertDiaglogBuilder.create();
        alertDialog.show();
    }

    private void InitializeBluetoothAdapter(){
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null){
            AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(viewCalling.getContext());
            alertDiaglogBuilder.setTitle("No Bluetooth adapter");
            alertDiaglogBuilder.setMessage("Your device does not have a working bluetooth adapter.");
            alertDiaglogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    System.exit(0);
                }
            });
            alertDiaglogBuilder.setCancelable(false);
            AlertDialog alertDialog = alertDiaglogBuilder.create();
            alertDialog.show();
        }
        else{
            if(!bluetoothAdapter.isEnabled()){
                AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(viewCalling.getContext());
                alertDiaglogBuilder.setTitle("Bluetooth not turned on.");
                alertDiaglogBuilder.setMessage("Do you want to enable it?");
                alertDiaglogBuilder.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        bluetoothAdapter.enable();
                    }
                });

                alertDiaglogBuilder.setNegativeButton("No", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        System.exit(0);
                    }
                });
                alertDiaglogBuilder.setCancelable(false);
                AlertDialog alertDialog = alertDiaglogBuilder.create();
                alertDialog.show();
            }
        }
    }

}

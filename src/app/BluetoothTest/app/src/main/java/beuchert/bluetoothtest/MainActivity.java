package beuchert.bluetoothtest;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
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

    // Fields:
    private String MacAdress = "00:0C:78:33:A5:63";
    private BluetoothAdapter BA = null;
    private BluetoothSocket mSocket = null;

    // onClick methods:
    public void connectClick(View view) {
        Connect();
    }

    public void DisconnectOnClick(View view){
        TextView statusText = (TextView) findViewById(R.id.StatusText);
        statusText.setText("Status: Not Connected");

        try {
            mSocket.close();
        } catch (IOException e) {
            ErrorMessage();
        }
    }

    public void ChangeOnClick(View view){
        EditText MacAdressEditText = (EditText) findViewById(R.id.NewMacText);
        Editable NewMacAdress = MacAdressEditText.getText();
        MacAdress = NewMacAdress.toString();
    }


    // Help methods:
    private void Connect(){
        final UUID MY_UUID = UUID.fromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");

        BA = BluetoothAdapter.getDefaultAdapter();
        BluetoothDevice device = BA.getRemoteDevice(MacAdress);

        try {
            mSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
            mSocket.connect();
        } catch (IOException e) {
            ErrorMessage();
        }

        TextView statusText = (TextView) findViewById(R.id.StatusText);
        statusText.setText("Connected to: " + device.getName());
        TextView PackageContent = (TextView) findViewById(R.id.PackageContent);

        if(mSocket.isConnected()){
            InputStream result = null;
            try {
                result = mSocket.getInputStream();
                BufferedReader r = new BufferedReader(new InputStreamReader(result));
                String realResult = r.readLine();
                PackageContent.setText(realResult);

            } catch (IOException e) {
                ErrorMessage();
            }
        }
    }

    private void ErrorMessage(){
        TextView statusText = (TextView) findViewById(R.id.StatusText);
        statusText.setText("Status: Not Connected");
        AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(this);
        alertDiaglogBuilder.setTitle("Connection Error");
        alertDiaglogBuilder.setMessage("There was a bluetooth connection error (IOException). Do you want to try again?");
        alertDiaglogBuilder.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Connect();
            }
        });

        alertDiaglogBuilder.setNegativeButton("No", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                try {
                    mSocket.close();
                } catch (IOException e) {
                    ErrorMessage();
                }
            }
        });

        AlertDialog alertDialog = alertDiaglogBuilder.create();
        alertDialog.show();
    }
}

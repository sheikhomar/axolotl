package beuchert.bluetoothtest.Activities;



import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import beuchert.bluetoothtest.Interfaces.Callbacks;
import beuchert.bluetoothtest.R;
import beuchert.bluetoothtest.Services.BluetoothService;

public class MainActivity extends AppCompatActivity implements Callbacks {

    // Fields:
    private BluetoothService blueService;
    private Intent blueIntent;

    private ServiceConnection mServiceConnection= new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            Toast.makeText(MainActivity.this, "onServiceConnected called", Toast.LENGTH_SHORT).show();
            // We've binded to LocalService, cast the IBinder and get LocalService instance
            BluetoothService.LocalBinder binder = (BluetoothService.LocalBinder) service;
            blueService = binder.getServiceInstance(); //Get instance of your service!
            blueService.registerClient(MainActivity.this); //Activity register in the service as client for callbacks!
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0) {
            Toast.makeText(MainActivity.this, "onServiceDisconnected called", Toast.LENGTH_SHORT).show();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        blueIntent = new Intent(MainActivity.this, BluetoothService.class);
    }

    @Override
    protected void onStart(){
        super.onStart();
        Log.d("MainActivity", "MainActivity Started");
        Intent blueIntent = new Intent(this, BluetoothService.class);
        bindService(blueIntent, mServiceConnection, BIND_AUTO_CREATE);
    }

    // onClick methods:
    public void onConnectClick(View view) {
        blueService.connect();
    }

    public void DisconnectOnClick(View view){
        blueService.disconnect();
    }

    public void sendOnClick(View view){
        EditText message = (EditText) findViewById(R.id.communicationMessage);
        blueService.sendMessage(message.getText().toString());
    }

    @Override
    public void updateClient(long data) {

    }

    @Override
    public void showBluetoothConnectionAlert() {
        AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(this);
        alertDiaglogBuilder.setTitle("Connection Error");
        alertDiaglogBuilder.setMessage("There was a bluetooth connection error (IOException).");
        alertDiaglogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        alertDiaglogBuilder.setCancelable(false);
        AlertDialog alertDialog = alertDiaglogBuilder.create();
        alertDialog.show();
    }

    @Override
    public void showBluetoothAdapterAlert() {
        AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(this);
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

    @Override
    public void showBluetoothNotEnabledAlert() {
        AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(this);
        alertDiaglogBuilder.setTitle("Bluetooth not turned on.");
        alertDiaglogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
        });
        alertDiaglogBuilder.setCancelable(false);
        AlertDialog alertDialog = alertDiaglogBuilder.create();
        alertDialog.show();
    }

    @Override
    public void showPackageContent(String Package) {
        TextView PackageContent = (TextView) findViewById(R.id.PackageContent);
        PackageContent.append("\n" + Package);
    }
}

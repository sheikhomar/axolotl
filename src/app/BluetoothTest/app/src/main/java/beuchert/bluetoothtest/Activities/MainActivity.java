package beuchert.bluetoothtest.Activities;



import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import beuchert.bluetoothtest.R;
import beuchert.bluetoothtest.Services.BluetoothService;

public class MainActivity extends AppCompatActivity implements BluetoothService.Callbacks {

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
        // insert on click event
        blueService.connect();
    }

    public void DisconnectOnClick(View view){
        // Insert OnClick event
    }

    public void ChangeOnClick(View view){
        // Insert OnCLick event
    }

    @Override
    public void updateClient(long data) {

    }


    // Help methods:

}

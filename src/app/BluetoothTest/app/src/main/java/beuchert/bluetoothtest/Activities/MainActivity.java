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
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOError;
import java.io.IOException;

import beuchert.bluetoothtest.Interfaces.Callbacks;
import beuchert.bluetoothtest.Models.Package;
import beuchert.bluetoothtest.R;
import beuchert.bluetoothtest.Services.BluetoothService;
import beuchert.bluetoothtest.Views.DrawingView;

public class MainActivity extends AppCompatActivity implements Callbacks, AdapterView.OnItemSelectedListener {

    // Fields:
    private BluetoothService blueService;
    private Intent blueIntent;
    private DrawingView drawingView;
    private ArrayAdapter<String> spinnerAdapter;
    Spinner spinner;
    private boolean drawingViewCreated = false;
    private int binLayers;


    //Activity methods
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        blueIntent = new Intent(MainActivity.this, BluetoothService.class);

        spinner = (Spinner) findViewById(R.id.spinner);
        spinnerAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item);
        spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(spinnerAdapter);
        spinner.setOnItemSelectedListener(this);
    }

    @Override
    protected void onStart(){
        super.onStart();
        Log.d("MainActivity", "MainActivity Started");
        Intent blueIntent = new Intent(this, BluetoothService.class);
        bindService(blueIntent, mServiceConnection, BIND_AUTO_CREATE);

        createDrawingView(1, 1);
    }



    private int i = 0;
    // onClick methods:
    public void onConnectClick(View view) {
        //blueService.connect();

        // bin packing example
        if(i == 0){
            handlePackage("B: 500 250 3");
        }
        else if (i == 1){
            handlePackage("P: 0 0 100 150 1 1");
        }
        else if (i == 2){
            handlePackage("P: 0 150 100 100 1 2");
        }
        else if (i == 3){
            handlePackage("P: 100 0 100 200 1 3");
        }
        else if (i == 4){
            handlePackage("B: 500 250 3");
        }
        else if (i == 5){
            handlePackage("P: 300 0 100 200 1 1");
        }
        else if (i == 6){
            handlePackage("P: 200 150 100 100 0 2");
        }
        else if (i == 7){
            handlePackage("P: 400 0 100 200 1 3");
        }
        else if (i == 8){
            handlePackage("P: 0 0 100 100 2 2");
        }
        else
            showBluetoothConnectionAlert();
        i++;
    }

    public void DisconnectOnClick(View view){
        blueService.disconnect();
    }

    public void sendOnClick(View view){
        EditText message = (EditText) findViewById(R.id.communicationMessage);
        blueService.sendMessage(message.getText().toString());
    }

    public void rightArrowOnClick(View view){
        drawingView.shiftRight();
    }

    public void leftArrowOnClick(View view){
        drawingView.shiftLeft();
    }

    // interface methods:
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

    int currentBin = 0;
    boolean firstTime = true;

    @Override
    public void handlePackage(String Package) {
        String[] PackSplit = Package.split(" ");

        if(PackSplit[0].equals("B:")){
            currentBin++;
            Log.d(MainActivity.class.getName(), "Bin Recieved");
            PackSplit[2] = PackSplit[2].replaceAll("\n", "");
            int length = Integer.parseInt(PackSplit[1]);
            int width = Integer.parseInt(PackSplit[2]);
            binLayers = Integer.parseInt(PackSplit[3]);
            if(firstTime) {
                drawingView.setSize(width, length);
                firstTime = false;
            }

        }
        else if(PackSplit[0].equals("P:")){
            Log.d(MainActivity.class.getName(), "Package Recieved");
            PackSplit[6] = PackSplit[6].replaceAll("\n", "");
            int x = Integer.parseInt(PackSplit[1]);
            int y = Integer.parseInt(PackSplit[2]);
            int length = Integer.parseInt(PackSplit[3]);
            int width = Integer.parseInt(PackSplit[4]);
            int color = Integer.parseInt(PackSplit[5]);
            int layer = Integer.parseInt(PackSplit[6]);
            drawingView.addPackage(new Package(x, y, length, width, color, layer, currentBin));
        }
        else {
            showInvalidPackageError();
        }
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
        String s = parent.getItemAtPosition(pos).toString();
        String[] split = s.split(" ");
        int sAsInt1 = Integer.parseInt(split[1]);
        int sAsInt2 = Integer.parseInt(split[3]);
        drawingView.selectBinAndLayer(sAsInt1, sAsInt2);
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {

    }



    //Help methods:
    private ServiceConnection mServiceConnection = new ServiceConnection() {

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

    public void createDrawingView(int length, int width){
        if(drawingViewCreated == false){
            drawingViewCreated = true;
            RelativeLayout DrawLayout = (RelativeLayout) findViewById(R.id.rectCanvas);
            drawingView = new DrawingView(this,length,width);
            DrawLayout.addView(drawingView);
        }
        else{
            IOError e = new IOError(new Throwable("You cannot recreate DrawingView"));
            throw e;
        }


    }

    public void addElementToSpinner(String newString){
        spinnerAdapter.add(newString);
        spinnerAdapter.notifyDataSetChanged();
    }

    public void setSelectedElementInSpinner(int bin, int layer){
        spinner.setSelection((bin-1)*binLayers + layer - 1);
    }

    public void showInvalidPackageError(){
        AlertDialog.Builder alertDiaglogBuilder = new AlertDialog.Builder(this);
        alertDiaglogBuilder.setTitle("Package error");
        alertDiaglogBuilder.setMessage("The application don't know how to handle the message which was recieved.");
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

}

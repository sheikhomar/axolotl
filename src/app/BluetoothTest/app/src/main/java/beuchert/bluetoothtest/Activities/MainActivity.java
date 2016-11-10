package beuchert.bluetoothtest.Activities;



import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
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
import beuchert.bluetoothtest.Models.Bin;
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
    private ArrayAdapter<String> spinnerAdapter2;
    Spinner spinner, spinner2;
    private boolean drawingViewCreated = false;
    private int binLayers;
    boolean firstTime = true;
    public double drawingViewScale = 0;


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

        spinner2 = (Spinner) findViewById(R.id.spinner2);
        spinnerAdapter2 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item);
        spinnerAdapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner2.setAdapter(spinnerAdapter2);
        spinner2.setOnItemSelectedListener(this);
    }

    @Override
    protected void onStart(){
        super.onStart();
        Log.d("MainActivity", "MainActivity Started");
        Intent blueIntent = new Intent(this, BluetoothService.class);
        bindService(blueIntent, mServiceConnection, BIND_AUTO_CREATE);

        createDrawingView(1, 1);
    }


    // this int is used to test with onConnectClick without connecting with PI
    private int i = 0;
    // onClick methods:
    public void onConnectClick(View view) {
        blueService.connect();

        // bin packing example
        /*if(i == 0){
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
        i++;*/
    }

    public void DisconnectOnClick(View view){
        blueService.disconnect();
    }

    public void rightArrowOnClick(View view){ drawingView.shiftRight(); }

    public void leftArrowOnClick(View view){
        drawingView.shiftLeft();
    }

    public void rightArrow2OnClick(View view) { drawingView.shiftRight2(); }

    public void leftArrow2OnClick(View view) { drawingView.shiftLeft2(); }

    // interface methods:
    @Override
    public void showBluetoothConnectionAlert() {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle("Connection Error");
        alertDialogBuilder.setMessage("There was a bluetooth connection error (IOException).");
        alertDialogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        alertDialogBuilder.setCancelable(false);
        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }

    @Override
    public void showBluetoothAdapterAlert() {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle("No Bluetooth adapter");
        alertDialogBuilder.setMessage("Your device does not have a working bluetooth adapter.");
        alertDialogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                System.exit(0);
            }
        });
        alertDialogBuilder.setCancelable(false);
        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }

    @Override
    public void showBluetoothNotEnabledAlert() {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle("Bluetooth not turned on.");
        alertDialogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
        });
        alertDialogBuilder.setCancelable(false);
        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }

    @Override
    public void handlePackage(String Package) {
        String[] PackSplit = Package.split(" ");

        if(PackSplit[0].equals("B:")){
            Log.d(MainActivity.class.getName(), "Bin Recieved");
            PackSplit[2] = PackSplit[2].replaceAll("\n", "");
            int length = Integer.parseInt(PackSplit[1]);
            int width = Integer.parseInt(PackSplit[2]);
            binLayers = Integer.parseInt(PackSplit[3]);
            drawingView.addBin(new Bin(binLayers));

            if(firstTime) {
                drawingViewScale = (double)drawingView.getWidth() / (double)Math.max(width, length);
                drawingView.setSize((int)(width * drawingViewScale), (int)(length * drawingViewScale));
                firstTime = false;
            }
        }
        else if(PackSplit[0].equals("P:")){
            Log.d(MainActivity.class.getName(), "Package Recieved");
            PackSplit[6] = PackSplit[6].replaceAll("\n", "");
            int x = (int)(Integer.parseInt(PackSplit[1]) * drawingViewScale);
            int y = (int)(Integer.parseInt(PackSplit[2]) * drawingViewScale);
            int length = (int)(Integer.parseInt(PackSplit[3]) * drawingViewScale);
            int width = (int)(Integer.parseInt(PackSplit[4]) * drawingViewScale);
            int height = (int)(Integer.parseInt(PackSplit[5]) * drawingViewScale);
            int color = Integer.parseInt(PackSplit[6]);
            int fragile = Integer.parseInt(PackSplit[7]);
            int layer = Integer.parseInt(PackSplit[8]);
            int binID = Integer.parseInt(PackSplit[9]);
            drawingView.addPackage(new Package(x, y, length, width, height, color, fragile, layer, binID));
        }
        else {
            showInvalidPackageError();
        }
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
        Spinner spinner = (Spinner)parent;
        if(spinner.getId() == R.id.spinner) {
            String s = parent.getItemAtPosition(pos).toString();
            String[] split = s.split(" ");
            int sAsInt1 = Integer.parseInt(split[1]);
            int sAsInt2 = Integer.parseInt(split[3]);
            drawingView.selectBinAndLayer(sAsInt1, sAsInt2);
        }
        else if(spinner.getId() == R.id.spinner2){
            drawingView.redrawSelectedBinAndLayer();
        }
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
            RelativeLayout DrawLayout = (RelativeLayout) findViewById(R.id.rectCanvas);
            drawingView = new DrawingView(this,length,width);
            DrawLayout.addView(drawingView);
        }
        else{
            IOError e = new IOError(new Throwable("You cannot recreate DrawingView"));
            throw e;
        }
    }

    public void addElementToSpinner(int spinnerID, String newString){
        if(spinnerID == 1) {
            spinnerAdapter.add(newString);
            spinnerAdapter.notifyDataSetChanged();
        }
        else if(spinnerID == 2){
            spinnerAdapter2.add(newString);
            spinnerAdapter2.notifyDataSetChanged();
        }
    }

    public void setSelectedElementInSpinner(int bin, int layer){
        spinner.setSelection((bin - 1) * binLayers + layer - 1);
        setSelectedElementInSpinner(1);
    }

    public void setSelectedElementInSpinner(int pack){
        spinner2.setSelection(pack-1);
    }

    public int readCurrentPackage(){
        if(spinner2.getCount() != 0) {
            Log.d("Chosen pack", "Number " + Integer.toString((int) spinner2.getSelectedItemId() + 1));
            return (int) spinner2.getSelectedItemId() + 1;
        }
        else{
            Log.d("Chosen pack", "Number " + Integer.toString(0));
            updatePackInfoDisplay("", "", "");
            return 0;
        }
    }

    public void clearSpinner(int spinnerID){
        if(spinnerID == 1){

        }
        else if(spinnerID == 2){
            spinnerAdapter2.clear();
            spinnerAdapter2.notifyDataSetChanged();
        }
    }

    public int spinner2Count(){
        return spinnerAdapter2.getCount();
    }

    public void updatePackInfoDisplay(String dim, String colour, String fragile){
        TextView dimVal = (TextView) findViewById(R.id.textViewDimVal);
        TextView colVal = (TextView) findViewById(R.id.textViewColVal);
        TextView fraVal = (TextView) findViewById(R.id.textViewFraVal);
        dimVal.setText(dim);
        colVal.setText(colour);
        fraVal.setText(fragile);
    }

    public void showInvalidPackageError(){
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle("Package error");
        alertDialogBuilder.setMessage("The application don't know how to handle the message which was recieved.");
        alertDialogBuilder.setNeutralButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        alertDialogBuilder.setCancelable(false);
        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }
}

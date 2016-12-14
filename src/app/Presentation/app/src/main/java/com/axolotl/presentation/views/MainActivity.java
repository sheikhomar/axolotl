package com.axolotl.presentation.views;

import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import com.axolotl.presentation.App;
import com.axolotl.presentation.R;
import com.axolotl.presentation.communication.BluetoothService;
import com.axolotl.presentation.communication.Messages;
import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.Repository;

public class MainActivity extends AppCompatActivity {

    private Repository repository;
    private LayerView layerView;
    private BinSelectorView binSelector;
    private LayerSelectorView layerSelector;
    private PackageDetailsView packageDetailsView;
    private Messenger mService = null;
    private boolean mIsBound;

    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            Log.d("MainActivity", "Service connected.");
            mService = new Messenger(service);
            sendMessageToService(Messages.REGISTER);
        }

        public void onServiceDisconnected(ComponentName className) {
            // This is called when the connection with the service has
            // been unexpectedly disconnected - process crashed.
            mService = null;
        }
    };

    private final Messenger mMessenger = new Messenger(new ServiceMessageHandler());
    private TextView binDestinationView;

    private class ServiceMessageHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Messages.DATA_RECEIVED:
                    updateUI();
                    break;
                case Messages.EVENT_BLUETOOTH_DISABLED:
                    Log.d("MainActivity", "Event Bluetooth disabled.");
                    Toast.makeText(getApplicationContext(), "Bluetooth disabled", Toast.LENGTH_LONG).show();
                    showAlert("Bluetooth", "Bluetooth is disabled.");
                    break;
                case Messages.CONNECTION_ERROR:
                    Log.d("MainActivity", "Connection error.");
                    showAlert("Bluetooth", "Connection error.");
                    break;
                default:
                    super.handleMessage(msg);
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        repository = ((App)getApplication()).getRepository();

        startService(new Intent(this, BluetoothService.class));
        doBindService();

        this.layerView = (LayerView) findViewById(R.id.layer_view);
        this.layerSelector = (LayerSelectorView)findViewById(R.id.layer_selector);
        this.binSelector = (BinSelectorView)findViewById(R.id.bin_selector);
        this.packageDetailsView = (PackageDetailsView)findViewById(R.id.package_details);
        this.binDestinationView = (TextView)findViewById(R.id.bin_destination);

        this.binSelector.setBinSelectListener(new BinSelectorView.OnBinSelectListener() {
            @Override
            public void onBinSelect(Bin bin) {
                repository.selectBin(bin);
                updateUI();
            }
        });
        this.layerSelector.setLayerSelectListener(new LayerSelectorView.OnLayerSelectListener() {
            @Override
            public void onLayerSelect(Layer layer) {
                repository.selectLater(layer);
                updateUI();
            }
        });
        this.layerView.setPackageSelectListener(new LayerView.OnPackageSelectListener() {
            @Override
            public void onPackageSelect(Package aPackage) {
                repository.togglePackageSelection(aPackage);
                updateUI();
            }
        });
        updateUI();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_reconnect:
                this.establishConnection();
                return true;
            default:
                return super.onOptionsItemSelected(item);

        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        try {
            doUnbindService();
        }
        catch (Throwable t) {
            Log.e("MainActivity", "Failed to unbind from the service", t);
        }
    }

    private void updateUI() {
        this.binSelector.setData(repository.getBins(), repository.getSelectedBin());
        this.layerSelector.setData(repository.getSelectedBin(), repository.getSelectedLayer());
        this.layerView.setData(repository.getSelectedLayer(), repository.getSelectedPackage());
        this.packageDetailsView.setData(repository.getSelectedPackage());
        updateDestination();
    }

    private void updateDestination() {
        Bin selectedBin = repository.getSelectedBin();
        if (selectedBin != null) {
            this.binDestinationView.setText("To " + selectedBin.getDestination());
        } else {
            this.binDestinationView.setText("No bin selected.");
        }
    }

    private void doBindService() {
        bindService(new Intent(this, BluetoothService.class), mConnection, Context.BIND_AUTO_CREATE);
        mIsBound = true;
        Log.d("MainActivity", "Service is bound.");
    }

    private void doUnbindService() {
        if (mIsBound) {
            if (mService != null) {
                sendMessageToService(Messages.UNREGISTER);
            }
            // Detach our existing connection.
            unbindService(mConnection);
            mIsBound = false;
        }
    }

    private void showAlert(String title, String msg){
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle(title);
        alertDialogBuilder.setMessage(msg);
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

    private void establishConnection() {
        sendMessageToService(Messages.ESTABLISH_CONNECTION);
    }

    private void sendMessageToService(int message) {
        if (mService == null)
            return;

        try {
            Message msg = Message.obtain(null, message);
            msg.replyTo = mMessenger;
            mService.send(msg);
        }
        catch (RemoteException e) {
            Log.d("MainActivity", "Service has crashed.");
            showAlert("BluetoothService crashed.", "Service has crashed. Please restart the app.");
        }
    }
}
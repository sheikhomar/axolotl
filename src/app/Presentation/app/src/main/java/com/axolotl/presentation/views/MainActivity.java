package com.axolotl.presentation.views;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;

import com.axolotl.presentation.App;
import com.axolotl.presentation.R;
import com.axolotl.presentation.communication.BluetoothService;
import com.axolotl.presentation.communication.CommandTranslator;
import com.axolotl.presentation.communication.InvalidCommandException;
import com.axolotl.presentation.communication.Messages;
import com.axolotl.presentation.model.Bin;
import com.axolotl.presentation.model.Layer;
import com.axolotl.presentation.model.Package;
import com.axolotl.presentation.model.Repository;
import com.axolotl.presentation.views.BinSelectorView;
import com.axolotl.presentation.views.LayerSelectorView;
import com.axolotl.presentation.views.PackageDetailsView;
import com.axolotl.presentation.views.LayerView;

public class MainActivity extends AppCompatActivity {

    private Repository repository;
    private CommandTranslator commandTranslator = new CommandTranslator();
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
            try {
                Message msg = Message.obtain(null, Messages.REGISTER_CLIENT);
                msg.replyTo = mMessenger;
                mService.send(msg);
            }
            catch (RemoteException e) {
                // In this case the service has crashed before we could even do anything with it
            }
        }

        public void onServiceDisconnected(ComponentName className) {
            // This is called when the connection with the service has
            // been unexpectedly disconnected - process crashed.
            mService = null;
        }
    };
    private final Messenger mMessenger = new Messenger(new ServiceMessageHandler());

    private class ServiceMessageHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Messages.DATA_RECEIVED:
                    Log.d("MainActivity", "Data received.");
                    String cmd = (String)msg.obj;
                    try {
                        commandTranslator.translate(cmd, repository);
                    } catch (InvalidCommandException e) {
                        Log.d("MainActivity", "Invalid command: " + e.getMessage(), e);
                    }
                    updateUI();
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

        this.binSelector.setBinSelectListener(new BinSelectorView.OnBinSelectListener() {
            @Override
            public void onBinSelect(Bin bin) {
                repository.selectBin(bin);
                layerSelector.setBin(bin);
                layerView.setLayer(repository.getSelectedLayer());
            }
        });
        this.layerSelector.setLayerSelectListener(new LayerSelectorView.OnLayerSelectListener() {
            @Override
            public void onLayerSelect(Layer layer) {
                repository.selectLater(layer);
                layerView.setLayer(repository.getSelectedLayer());
            }
        });
        this.layerView.setPackageSelectListener(new LayerView.OnPackageSelectListener() {
            @Override
            public void onPackageSelect(Package aPackage) {
                packageDetailsView.setPackage(aPackage);
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
    protected void onDestroy() {
        super.onDestroy();
        try {
            doUnbindService();
        }
        catch (Throwable t) {
            Log.e("MainActivity", "Failed to unbind from the service", t);
        }
    }

    private void doBindService() {
        bindService(new Intent(this, BluetoothService.class), mConnection, Context.BIND_AUTO_CREATE);
        mIsBound = true;
        Log.d("MainActivity", "Service is bound.");
    }

    private void doUnbindService() {
        if (mIsBound) {
            // If we have received the service, and hence registered with it, then now is the time to unregister.
            if (mService != null) {
                try {
                    Message msg = Message.obtain(null, Messages.UNREGISTER_CLIENT);
                    msg.replyTo = mMessenger;
                    mService.send(msg);
                }
                catch (RemoteException e) {
                    // There is nothing special we need to do if the service has crashed.
                }
            }
            // Detach our existing connection.
            unbindService(mConnection);
            mIsBound = false;
        }
    }

    private void updateUI() {
        this.binSelector.setBins(repository.getBins(), repository.getSelectedBinIndex());
        this.layerSelector.setBin(repository.getSelectedBin());
        this.layerView.setLayer(repository.getSelectedLayer());
        this.packageDetailsView.setPackage(repository.getSelectedPackage());
    }
}
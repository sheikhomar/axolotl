package beuchert.bluetoothtest.Services;

import android.app.Activity;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.widget.Toast;

public class BluetoothService extends Service {
    // Fields:
    private final IBinder mBinder;
    Callbacks activity;


    // Constructor
    public BluetoothService(){
        mBinder = new LocalBinder();
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Toast.makeText(this, "OnCreate", Toast.LENGTH_SHORT).show();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        // We want this service to continue running until it is explicitly
        // stopped, so return sticky.
        Toast.makeText(this, "OnStartCommand " + intent.getDataString(), Toast.LENGTH_SHORT).show();

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    public class LocalBinder extends Binder {
        public BluetoothService getServiceInstance(){
            return BluetoothService.this;
        }
    }

    //Here Activity register to the service as Callbacks client
    public void registerClient(Activity activity){
        this.activity = (Callbacks)activity;
    }

    //callbacks interface for communication with service clients!
    public interface Callbacks{
        public void updateClient(long data);
    }

    public void connect(){
        Toast.makeText(this, "Connected Correctly", Toast.LENGTH_LONG).show();
    }
}

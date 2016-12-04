package com.axolotl.presentation.communication;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.support.annotation.Nullable;

public class BluetoothService extends Service {

    public static ServiceHandler getInstance() {
        return new ServiceHandler();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}

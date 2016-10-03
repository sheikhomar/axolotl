package beuchert.bluetoothtest.Models;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import beuchert.bluetoothtest.Interfaces.Callbacks;

public class BluetoothHandler extends Handler {
    private Callbacks mActivity;

    @Override
    public void handleMessage(Message msg){
        String data = (String) msg.obj;
        mActivity.showPackageContent(data);
    }

    public void setActivity(Callbacks activity){
        mActivity = activity;
    }
}

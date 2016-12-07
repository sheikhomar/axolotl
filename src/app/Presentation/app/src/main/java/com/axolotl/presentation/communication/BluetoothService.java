package com.axolotl.presentation.communication;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import com.axolotl.presentation.App;
import com.axolotl.presentation.model.Repository;

import java.util.ArrayList;

public class BluetoothService extends Service {
    // Keeps track of all current registered clients.
    private ArrayList<Messenger> clients = new ArrayList<>();
    // Target we publish for clients to send messages to IncomingHandler.
    final Messenger mMessenger = new Messenger(new IncomingHandler());
    private final Messenger threadMessenger = new Messenger(new ThreadMessageHandler());
    private Repository repository;
    private BluetoothThread thread;

    private class ThreadMessageHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            sendMessageToClients(msg);
        }
    }

    // Handler of incoming messages from clients.
    private class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Messages.REGISTER_CLIENT:
                    clients.add(msg.replyTo);

                    thread = new BluetoothThread(threadMessenger);
                    thread.start();
                    break;
                case Messages.UNREGISTER_CLIENT:
                    clients.remove(msg.replyTo);
                    break;
                default:
                    super.handleMessage(msg);
            }
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(BluetoothService.class.getName(), "Created BluetoothService");

        repository = ((App)getApplication()).getRepository();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        Log.d(BluetoothService.class.getName(), "Received start id " + startId + ": " + intent);


        // We want this service to continue running until it is explicitly
        // stopped, so return sticky.
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mMessenger.getBinder();
    }

    private void sendMessageToClients(Message msg) {
        for (int i = clients.size() - 1; i >= 0; i--) {
            try {
                Message msg2 = Message.obtain(null, msg.what);
                msg2.copyFrom(msg);
                clients.get(i).send(msg2);
            }
            catch (RemoteException e) {
                // The client is dead.
                Log.d("BluetoothThread", "Sending message failed for client " + i, e);
                clients.remove(i);
            }
        }
    }
}
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

public class BluetoothService extends Service {
    private Messenger clientMessenger;
    // Target we publish for clients to send messages to ServiceMessageHandler.
    final Messenger serviceMessenger = new Messenger(new ServiceMessageHandler());
    private final Messenger threadMessenger = new Messenger(new ThreadMessageHandler());
    private Repository repository;
    private BluetoothThread thread;
    private CommandTranslator commandTranslator;

    private class ThreadMessageHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Messages.DATA_RECEIVED:
                    String cmd = (String)msg.obj;
                    try {
                        commandTranslator.translate(cmd, repository);
                    } catch (InvalidCommandException e) {
                        Log.d("BluetoothService", "Invalid command: " + e.getMessage(), e);
                    }

                    Message newMsg = Message.obtain(null, Messages.DATA_RECEIVED);
                    sendMessageToClient(newMsg);
                    break;

                case Messages.EVENT_BLUETOOTH_DISABLED:
                    Message newMsg2 = Message.obtain(null, Messages.EVENT_BLUETOOTH_DISABLED);
                    sendMessageToClient(newMsg2);
                    break;

                default:
                    super.handleMessage(msg);
            }
        }
    }

    // Handler of incoming messages from clients.
    private class ServiceMessageHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Messages.ESTABLISH_CONNECTION:
                    clientMessenger = msg.replyTo;
                    if (thread == null)
                        thread = new BluetoothThread(threadMessenger);
                    if (thread.getState() == Thread.State.NEW)
                        thread.start();
                    break;
                case Messages.CLOSE_CONNECTION:
                    Log.d("BluetoothService", "Close connection event.");
                    clientMessenger = null;
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
        commandTranslator = new CommandTranslator();
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
        return serviceMessenger.getBinder();
    }

    private void sendMessageToClient(Message msg) {
        if (clientMessenger == null) {
            Log.d("BluetoothService", "No client to send message to.");
            return;
        }

        try {
            clientMessenger.send(msg);
        }
        catch (RemoteException e) {
            // The client is dead.
            Log.d("BluetoothThread", "Sending message failed for client.", e);
            clientMessenger = null;
        }
    }
}
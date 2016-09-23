using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Mime;
using System.Text;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Bluetooth;
using Android.Bluetooth.LE;
using Android.OS;
using Java.Util;

namespace BluetoothTestApp
{
    [Activity(Label = "BluetoothTestApp", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        int count = 1;
        private BluetoothSocket _socket;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);


            TextView Text = FindViewById<TextView>(Resource.Id.ResultText);
            Button SearchButton = FindViewById<Button>(Resource.Id.SearchButton);

            SearchButton.Click += (Object sender, EventArgs args) =>
            {
                BluetoothAdapter adapter = BluetoothAdapter.DefaultAdapter;
                if (adapter == null)
                    throw new Exception("No Bluetooth adapter found.");

                if (!adapter.IsEnabled)
                    throw new Exception("Bluetooth adapter is not enabled.");

                BluetoothDevice device = (from bd in adapter.BondedDevices
                                          where bd.Name == "DESKTOP-S7RVO4O"
                                          select bd).FirstOrDefault();

                if (device == null)
                    throw new Exception("Named device not found.");

                _socket = device.CreateRfcommSocketToServiceRecord(UUID.FromString("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd"));
                try
                {
                    _socket.Connect();
                }
                catch (Java.IO.IOException)
                {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    AlertDialog alert = builder.Create();
                    alert.SetTitle("Disconnected");
                    alert.SetMessage("Do you want to reconnect?");

                    alert.SetButton("YES!", (s, ev) =>
                    {
                        
                    });
                    alert.SetButton("NO!", (s, ev) =>
                    {

                    });
                }

                if (_socket.IsConnected)
                    Text.SetText("Succes!".ToCharArray(), 0, 7);
                else
                    Text.SetText("FAIL!".ToCharArray(), 0, 5);

                // Read data from the device
                byte[] buffer = new byte[1024];

                try
                {
                    _socket.InputStream.Read(buffer, 0, buffer.Length);
                }
                catch (Java.IO.IOException)
                {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    AlertDialog alert = builder.Create();
                    alert.SetTitle("Disconnected");
                    alert.SetMessage("Do you want to reconnect?");

                    alert.SetButton("YES!", (s, ev) =>
                    {

                    });
                    alert.SetButton2("NO!", (s, ev) =>
                    {

                    });
                    alert.Show();
                }

                string Message = Encoding.UTF8.GetString(buffer);
                TextView MessageText = FindViewById<TextView>(Resource.Id.MessageText);
                MessageText.SetText(Message.ToCharArray(), 0, Message.Length);


                // Write data to the device
                //_socket.OutputStream.Write(buffer, 0, buffer.Length);


            };
        }
    }
}


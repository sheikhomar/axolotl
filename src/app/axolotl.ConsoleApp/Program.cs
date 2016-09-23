using System;
using System.Text;
using InTheHand.Net;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;

namespace axolotl.ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Axolotl is now running ...");
            BluetoothComponent e;

            var serverId = new Guid("c3ffbcc2-ab89-4e56-94ed-2a8df65e45bd");
            
            BluetoothListener l = new BluetoothListener(serverId);
            l.Start(10);
            l.BeginAcceptBluetoothClient(AcceptConnection, l);

            Console.WriteLine("Listening on " + serverId);

            Console.ReadKey();
        }

        static void AcceptConnection(IAsyncResult result)
        {
            Console.WriteLine("New connection");

            if (result.IsCompleted)
            {
                BluetoothClient remoteDevice = ((BluetoothListener)result.AsyncState).EndAcceptBluetoothClient(result);
                var data = Encoding.UTF32.GetBytes("Axolotl calling earth");
                var stream = remoteDevice.GetStream();
                stream.Write(data, 0, data.Length);
            }
        }
    }
}

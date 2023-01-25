using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text;


internal class Program
{
    [DllImport("kernel32.dll", EntryPoint = "LoadLibrary", SetLastError = true)]
    static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

    private static void Main(string[] args)
    {
        int hDll = LoadLibrary("L2BotDll.dll");

        if (hDll == 0)
        {
            Console.WriteLine("Unable to load library L2BotDll.dll: " + Marshal.GetLastWin32Error().ToString());
            Console.ReadLine();
            return;
        }

        Console.WriteLine("L2BotDll.dll loaded\n");

        while (true)
        {
            var clientPipe = new NamedPipeClientStream("PipeL2Bot");
            clientPipe.Connect();
            clientPipe.ReadMode = PipeTransmissionMode.Message;

            Console.WriteLine("Connected to connection pipe");

            byte[] buffer = new byte[16384 * 2];
            int read = clientPipe.Read(buffer, 0, buffer.Length);

            if (clientPipe.IsConnected)
            {
                string pipeName = Encoding.Unicode.GetString(buffer).TrimEnd('\0');
                Console.WriteLine("Received connection pipe name " + pipeName);

                var mainPipe = new NamedPipeClientStream(pipeName);
                mainPipe.Connect();
                mainPipe.ReadMode = PipeTransmissionMode.Message;

                Console.WriteLine("Connected to main pipe\n");

                while (true)
                {
                    byte[] buffer1 = new byte[16384 * 2];
                    int read1 = mainPipe.Read(buffer1, 0, buffer1.Length);

                    if (mainPipe.IsConnected)
                    {
                        string message = Encoding.Unicode.GetString(buffer1).TrimEnd('\0');
                        Console.WriteLine(message);
                    }
                    else
                    {
                        Console.WriteLine("\nDisconnected from main pipe");
                        Console.WriteLine("Disconnected from connection pipe\n\n");

                        mainPipe.Close();
                        mainPipe.Dispose();
                        clientPipe.Close();
                        clientPipe.Dispose();
                        break;
                    }
                }
            }
        }
    }
}
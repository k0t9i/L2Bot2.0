using System.IO;
using System.IO.Pipes;
using System.Text;


internal class Program
{
    private static void Main(string[] args)
    {
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

                Console.WriteLine("Connected to main pipe");

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
                        Console.WriteLine("Disconnected from main pipe");
                        Console.WriteLine("Disconnected from connection pipe");

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
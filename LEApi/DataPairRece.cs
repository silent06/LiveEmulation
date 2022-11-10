using System;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.IO;
using LEApi.Helper;
using FullXboxAPI._0;

namespace LEApi
{
    class DataPairRece
    {
        public static int SaltCount = 0;
        public static int ENCKeyCount = 0;
        //public static int ServerPort = 5000;
        public static int CurrentSaltCount = 0;
        public static int FolderStartPoint = Program.SeedDirectoryCount;
        public static TcpListener tcpListener = new TcpListener(IPAddress.Any, Global.Port + 2);
        public static void PairRecvHandleClient(TcpClient client)
        {
            NetworkStream networkStream = client.GetStream();
            SecureStream secureStream = new SecureStream(networkStream);
            string IP = ((IPEndPoint)client.Client.RemoteEndPoint).Address.ToString();

            try
            {
                byte[] Header = new byte[8];
                if (networkStream.Read(Header, 0, 8) != 8)
                {
                    client.Close();
                    return;
                }

                EndianIO IO = new EndianIO(Header, EndianStyle.BigEndian);
                uint Command = IO.Reader.ReadUInt32();
                int Size = IO.Reader.ReadInt32();

                byte[] Data = new byte[Size];
                if ((Size > 0x4800) || (secureStream.Read(Data, 0, Size) != Size))
                {
                    client.Close();
                    return;
                }
                EndianIO dataStream = new EndianIO(Data, EndianStyle.BigEndian)
                {
                    Writer = new EndianWriter(secureStream, EndianStyle.BigEndian)
                };

                switch (Command)
                {
                    case 0x10:
                        {
                            byte[] CPUKey = dataStream.Reader.ReadBytes(0x10);
                            byte[] HVSalt = dataStream.Reader.ReadBytes(0x10);
                            byte[] ECCDigest = dataStream.Reader.ReadBytes(0x14);
                            byte[] HVEX = dataStream.Reader.ReadBytes(0x2);
                            Tools.AppendText(string.Concat(new object[] { IP, " > ", "Folder Being Used: '", FolderStartPoint, "'" }), ConsoleColor.White);
                            Tools.AppendText(string.Concat(new object[] { IP, " > ", "SALT: '", Tools.BytesToHexString(HVSalt), "' HVEX: '", Tools.BytesToHexString(HVEX), "'" }), ConsoleColor.White);
                            Tools.AppendText(string.Concat(new object[] { IP, " > ", "ECCDigest: '", Tools.BytesToHexString(ECCDigest), "'" }), ConsoleColor.White);

                            byte[] OutputData = new byte[0x17];
                            Buffer.BlockCopy(ECCDigest, 0x0, OutputData, 0, 0x14);
                            Buffer.BlockCopy(HVEX, 0x0, OutputData, 0x15, 0x2);

                            Directory.CreateDirectory(string.Concat(new object[] { "XKE/Packages/", FolderStartPoint }));
                            File.WriteAllBytes(string.Format("XKE/Packages/{0}/{1}.bin", FolderStartPoint, Tools.BytesToHexString(HVSalt)), OutputData);
                            byte[] Response = new byte[4];
                            Buffer.BlockCopy(BitConverter.GetBytes((uint)0x4A000000).Reverse().ToArray(), 0, Response, 0, 4);
                            dataStream.Writer.Write(Response);
                            client.Close();
                            SaltCount++;
                            CurrentSaltCount++;
                            Tools.AppendText(string.Concat(new object[] { IP, " > ", "DataRecved | RESP SENT | Salt 1-256 = '", CurrentSaltCount, "'" }), ConsoleColor.Cyan);
                            if (CurrentSaltCount == 256)
                            {
                                CurrentSaltCount = 0;
                            }
                            Console.Title = "ECCDgiest Recved: (" + SaltCount + ") | ENCKey's Recved: (" + ENCKeyCount + ")";
                            Console.ForegroundColor = ConsoleColor.Cyan;
                        }
                        break;
                    case 0x20:
                        {
                            byte[] ENCKey = dataStream.Reader.ReadBytes(0x30);
                            Directory.CreateDirectory(string.Concat(new object[] { "XKE/Packages/", FolderStartPoint }));
                            File.WriteAllBytes(string.Format("XKE/Packages/{0}/Key.bin", FolderStartPoint), ENCKey);
                            byte[] Response = new byte[4];
                            Buffer.BlockCopy(BitConverter.GetBytes((uint)0x4A000000).Reverse().ToArray(), 0, Response, 0, 4);
                            dataStream.Writer.Write(Response);
                            Tools.AppendText(string.Concat(new object[] { IP, " > ", "Folder Used: '", FolderStartPoint, "' Key Recved!" }), ConsoleColor.Cyan);
                            client.Close();
                            ENCKeyCount++;
                            FolderStartPoint++;
                            //Next part is incase we have to close the program and we want to carry on from where we left off
                            File.WriteAllText("FolderName.txt", FolderStartPoint.ToString());
                            //Next part is incase we have to close the program and we want to carry on from where we left off
                            Console.Title = "ECCDgiest Recved: (" + SaltCount + ") | ENCKey's Recved: (" + ENCKeyCount + ")";
                            Console.ForegroundColor = ConsoleColor.Cyan;
                        }
                        break;
                }
            }
            catch (Exception ex)
            {
                if (client.Connected) client.Close();
            }
        }
    }
}

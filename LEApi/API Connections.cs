using System;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Threading;
using System.Runtime.InteropServices;
using System.Linq;


namespace LEApi
{
    public class Program {



        [DllImport("XeCrypt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void XeCryptBnQwNeRsaPubCrypt_(byte[] input, byte[] output, byte[] key);

        public static int SeedDirectoryCount = Directory.GetDirectories("XKE/Packages", "*", SearchOption.AllDirectories).Length;
        public static int SeedDirectoryCount2 = Directory.GetDirectories("XKE/Packages/").Length;
        public static byte[] CurrentHVBytes = null;
        public static string CurrentHvLoc = "XKE/HV.bin";
        public static int NumberOfPairs;
        public static byte[] Hypervisor = null;
        public static byte[] ChallengeRSAPUB = null;
        public static byte[] XamHeaderData = null;
        public static byte[] KrnHeaderData = null;
        public static byte[] CurModuleHeaderData = null;


        public class XSCDATA_STRUCT {
            public byte[] SessionToken;
            public byte[] KVCpukey;
            public byte[] MacAdress;
            public byte[] TypeKV;
            public byte[] Title;
            public byte[] KvC89;
            public byte[] DriveIndentifier;
            public byte[] KvSerial;
            public byte[] GameRegion;
            public byte[] Policyflashsize;
            public byte[] HVStatusFlag;
            public bool FCRT;
            public byte[] ConsoleID;
            public byte[] PartNumber;
            public byte[] KVDigest;
        }

        public class XKEDATA_STRUCT {
            public byte[] SessionToken;
            public byte[] KVCpukey;
            public byte[] HVSalt;
            public byte[] MacAddress;
            public bool CRL;
            public byte[] PartNumber;
            public byte[] TypeKV;
            public byte[] HvStatusFlag;
            public bool FCRT;
        }

        private static string[] GetIPAddresses() {
            try {
                return File.ReadAllLines("AuthList.txt");
            } catch (Exception ex) {
                Console.Write(ex.Message);
                return new string[] { "0" };
            }
        }

        private static bool AuthorizedIP(TcpClient client) {
            try {
                string IP = client.Client.RemoteEndPoint.ToString().Split(new char[] { ':' })[0];
                string[] AuthLines = GetIPAddresses();
                foreach (string AuthedIp in AuthLines) {
                    // IP check 
                    if (AuthedIp == IP)
                        return true;
                }
                return false;
            } catch (Exception ex) {
                Console.Write(ex.Message);
                return false;
            }
        }

        private static bool HandleChallengeResponse(TcpClient Client, byte[] Data, NetworkStream Stream) {//xke responce
            try {

                XKEDATA_STRUCT XKEData = new XKEDATA_STRUCT();
                byte[] SessionToken = new byte[0x10];
                byte[] KvCpukey = new byte[0x10];
                byte[] HvSalt = new byte[0x10];
                byte[] MacAddress = new byte[0x6];
                byte[] Crl = new byte[0x1];
                byte[] PartNumber = new byte[0xB];
                byte[] TypeKV = new byte[0x100];
                byte[] HvStatusFlag = new byte[0x2];
                byte[] FCRT = new byte[0x1];

                Buffer.BlockCopy(Data, 0x0, SessionToken, 0x0, 0x10);//
                XKEData.SessionToken = SessionToken;

                Buffer.BlockCopy(Data, 0x10, KvCpukey, 0x0, 0x10);//
                XKEData.KVCpukey = KvCpukey;

                Buffer.BlockCopy(Data, 0x20, HvSalt, 0x0, 0x10);//
                XKEData.HVSalt = HvSalt;

                Buffer.BlockCopy(Data, 0x30, MacAddress, 0x0, 0x6);//
                XKEData.MacAddress = MacAddress;

                Buffer.BlockCopy(Data, 0x36, Crl, 0x0, 0x1);//
                XKEData.CRL = BitConverter.ToBoolean(Crl, 0);

                Buffer.BlockCopy(Data, 0x37, PartNumber, 0x0, 0xB);//
                XKEData.PartNumber = PartNumber;

                Buffer.BlockCopy(Data, 0x42, TypeKV, 0x0, 0x100);//
                XKEData.TypeKV = TypeKV;

                Buffer.BlockCopy(Data, 0x142, HvStatusFlag, 0x0, 0x2);//
                XKEData.HvStatusFlag = HvStatusFlag;

                Buffer.BlockCopy(Data, 0x144, FCRT, 0x0, 0x1);//
                XKEData.FCRT = BitConverter.ToBoolean(FCRT, 0);

                if (XKEData.HVSalt == null || XKEData.SessionToken == null) {
                    return false;
                }

                byte[] XKEResp = new byte[0x100];
                IPEndPoint ip = Client.Client.RemoteEndPoint as IPEndPoint;
                string IPAddress = ((IPEndPoint)Client.Client.RemoteEndPoint).Address.ToString();
                int LivePackageIndex = XKEresponce.FindRandomPackageSet(XKEData.SessionToken);
                XKEResp = XKEresponce.Build_CleanXKE_Response(XKEData.MacAddress, XKEData.KVCpukey, XKEData.HVSalt, XKEData.CRL, XKEresponce.GetMotherboardTypeFromKv(XKEData.PartNumber, XKEData.TypeKV), XKEData, LivePackageIndex);

                if (XKEResp.Length != 0x100 || XKEResp == null) {
                    return false;
                }

                Stream.Write(XKEResp, 0, XKEResp.Length);
                File.WriteAllBytes(string.Concat(new object[] { "Dumps/XKE/", IPAddress, ".bin" }), XKEResp);
                return true;
            } catch (Exception ex) {
                //Tools.Log(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
                Console.Write(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
                return false;
            }
        }

        private static void XKEClientConnectedThread(object ConnectedClientObj)
        {
            TcpClient Client = (TcpClient)ConnectedClientObj;
            IPEndPoint ip = Client.Client.RemoteEndPoint as IPEndPoint;
            DateTime CurrentTime = DateTime.Now;
            try
            {

                if (!AuthorizedIP(Client))
                {
                    Client.Close();
                    Tools.Log(string.Format("[{0}] - Connection from unauthorized Endpoint: {1}", CurrentTime.ToString(), ip.ToString()));
                    Console.WriteLine(string.Format("[{0}] - Connection from unauthorized Endpoint: {1}", CurrentTime.ToString(), ip.ToString()));
                    return;
                }

                byte[] Data = new byte[0x145];
                NetworkStream stream = Client.GetStream();
                int i = stream.Read(Data, 0, Data.Length);
                if (i == 0x145)
                {
                    byte[] SessionToken = new byte[0x10];
                    Buffer.BlockCopy(Data, 0x0, SessionToken, 0x0, 0x10);//
                    byte[] HvSalt = new byte[0x10];
                    Buffer.BlockCopy(Data, 0x20, HvSalt, 0x0, 0x10);
                    int LivePackageIndex = XKEresponce.FindRandomPackageSet(SessionToken);

                    if (!HandleChallengeResponse(Client, Data, stream))
                    {
                        // Send blank response if we have any issues
                        stream.Write(Tools.FillData(0, 0x100), 0, 0x100);
                        Client.Close();
                        Tools.Log(string.Format("[{0}] - Failed To Generate Responce", CurrentTime.ToString()));
                        Console.WriteLine(string.Format("[{0}] - Failed To Generate Responce", CurrentTime.ToString()));
                        return;
                    }

                    Tools.write(ConsoleColor.Magenta, "---------------XKE Resp Generation Initiated!------------------\n");
                    Tools.write(ConsoleColor.White, "Current Time: ");
                    Tools.write(ConsoleColor.White, CurrentTime.ToString() + "\n");
                    Tools.write(ConsoleColor.White, "Client Connected at: ");
                    Tools.write(ConsoleColor.White, ip.ToString() + "\n");
                    Tools.write(ConsoleColor.White, "Client HvSalt: ");
                    Tools.write(ConsoleColor.White, Tools.BytesToHexString(HvSalt) + "\n");
                    Tools.write(ConsoleColor.White, "Client Session Token: ");
                    Tools.write(ConsoleColor.White, Tools.BytesToHexString(SessionToken) + "\n");
                    Tools.write(ConsoleColor.White, "Using Lasting Set: ");
                    Tools.write(ConsoleColor.White, LivePackageIndex + "\n");
                    Tools.write(ConsoleColor.Magenta, "---------------XKE Resp Generation Success!------------------\n\n");
                    //Tools.Log(string.Format("[{0}] XKE Resp Generation Success! From: {1} for session {2}", CurrentTime.ToString(), ip.ToString(), Tools.BytesToHexString(SessionToken)));
                    //Tools.Log("========================================================");
                }
                else
                {
                    //Tools.Log(string.Format("Unknown Request Size: {0} at {1} from IP: {2}", i, CurrentTime.ToString(), ip.ToString()));
                    Console.WriteLine("Unknown Request Size: {0} at {1} from IP: {2}", i, CurrentTime.ToString(), ip.ToString());
                    //Tools.Log("========================================================");
                    Console.WriteLine("========================================================");
                    Client.Close();
                    return;
                }
            }
            catch (Exception ex)
            {
               // Tools.Log(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
                Console.Write(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
            }
            Client.Close();
        }

        private static bool HandleXOSCResponse(TcpClient Client, byte[] Data, NetworkStream Stream) {
            try {
                IPEndPoint ip = Client.Client.RemoteEndPoint as IPEndPoint;
                string IPAddress = ((IPEndPoint)Client.Client.RemoteEndPoint).Address.ToString();
                XSCDATA_STRUCT XSCData = new XSCDATA_STRUCT();

                
                byte[] SessionKey = new byte[0x10];
                byte[] Title = new byte[0x4];
                byte[] TypeKV = new byte[0x100];
                byte[] KvC89 = new byte[0x4];
                byte[] DriveIndentifier = new byte[0x24];
                byte[] Kvserial = new byte[0xC];
                byte[] GameRegion = new byte[0x2];
                byte[] Policyflashsize = new byte[0x4];
                byte[] HVStatusFlag = new byte[0x4];
                byte[] FCRT = new byte[0x1];
                byte[] ConsoleID = new byte[0x5];
                byte[] PartNumber = new byte[0xB];
                byte[] KVDigest = new byte[0x10];
                byte[] KVCpukey = new byte[0x10];
                byte[] MacAdress = new byte[0x6];

                Buffer.BlockCopy(Data, 0, SessionKey, 0, 0x10);
                XSCData.SessionToken = SessionKey;

                Buffer.BlockCopy(Data, 0x10, Title, 0x0, 0x4);
                XSCData.Title = Title;

                Buffer.BlockCopy(Data, 0x14, TypeKV, 0, 0x100);
                XSCData.TypeKV = TypeKV;

                Buffer.BlockCopy(Data, 0x114, KvC89, 0, 0x4);
                XSCData.KvC89 = KvC89;

                Buffer.BlockCopy(Data, 0x118, DriveIndentifier, 0, 0x24);
                XSCData.DriveIndentifier = DriveIndentifier;

                Buffer.BlockCopy(Data, 0x13C, Kvserial, 0, 0xC);
                XSCData.KvSerial = Kvserial;

                Buffer.BlockCopy(Data, 0x148, GameRegion, 0, 0x2);
                XSCData.GameRegion = GameRegion;

                Buffer.BlockCopy(Data, 0x14A, Policyflashsize, 0, 0x4);
                XSCData.Policyflashsize = Policyflashsize;

                Buffer.BlockCopy(Data, 0x14E, HVStatusFlag, 0, 0x4);
                XSCData.HVStatusFlag = HVStatusFlag;

                Buffer.BlockCopy(Data, 0x150, FCRT, 0x0, 0x1);
                XSCData.FCRT = BitConverter.ToBoolean(FCRT, 0);

                Buffer.BlockCopy(Data, 0x151, ConsoleID, 0, 0x5);
                XSCData.ConsoleID = ConsoleID;

                Buffer.BlockCopy(Data, 0x156, PartNumber, 0, 0xB);
                XSCData.PartNumber = PartNumber;

                Buffer.BlockCopy(Data, 0x161, KVDigest, 0, 0x10);
                XSCData.KVDigest = KVDigest;
 
                Buffer.BlockCopy(Data, 0x171, KVCpukey, 0, 0x10);
                XSCData.KVCpukey = KVCpukey;

                Buffer.BlockCopy(Data, 0x181, MacAdress, 0, 0x6);
                XSCData.MacAdress = MacAdress;


                if (XSCData.SessionToken == null)
                    return false;

                byte[] XSCResp = new byte[0x2E0];
                XSCResp = XOSCresponce.BuildCleanXOSCResponce(XSCData.KVCpukey, XOSCresponce.GetMotherboardTypeFromKv(XSCData.PartNumber, XSCData.TypeKV), XSCData);
                if (XSCResp.Length != 0x2E0) {
                    return false;
                }
                
                Stream.Write(XSCResp, 0, XSCResp.Length);
                File.WriteAllBytes(string.Concat(new object[] { "Dumps/XOSC/", IPAddress, ".bin" }), XSCResp);
                return true;
            } catch (Exception ex) {
                Console.Write(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
                return false;
            }
        }

        private static void XSCClientConnectedThread(object ConnectedClientObj)
        {
            TcpClient Client = (TcpClient)ConnectedClientObj;
            IPEndPoint ip = Client.Client.RemoteEndPoint as IPEndPoint;
            string IP = Client.Client.RemoteEndPoint.ToString().Split(new char[] { ':' })[0];
            DateTime CurrentTime = DateTime.Now;
            try
            {

                if (!AuthorizedIP(Client))
                {
                    Client.Close();
                    Tools.Log(string.Format("[{0}] - Connection from unauthorized Endpoint: {1}", CurrentTime.ToString(), ip.ToString()));
                    Console.WriteLine(string.Format("[{0}] - Connection from unauthorized Endpoint: {1}", CurrentTime.ToString(), ip.ToString()));
                    return;
                }

                byte[] Data = new byte[0x187];
                NetworkStream stream = Client.GetStream();
                int i = stream.Read(Data, 0, Data.Length);
                if (i == 0x187)
                {

                    byte[] Kvserial = new byte[0xC];
                    Buffer.BlockCopy(Data, 0x13C, Kvserial, 0, 0xC);
                    byte[] SessionKey = new byte[0x10];
                    Buffer.BlockCopy(Data, 0, SessionKey, 0, 0x10);
                    byte[] Title = new byte[0x4];
                    Buffer.BlockCopy(Data, 0x10, Title, 0x0, 0x4);

                    if (!HandleXOSCResponse(Client, Data, stream))
                    {
                        // Send blank response if we have any issues
                        stream.Write(Tools.FillData(0, 0x2E0), 0, 0x2E0);
                        Client.Close();
                        Tools.Log(string.Format("[{0}] - Failed To Generate Responce", CurrentTime.ToString()));
                        Console.WriteLine(string.Format("[{0}] - Failed To Generate Responce", CurrentTime.ToString()));
                    }

                    Tools.write(ConsoleColor.Magenta, "---------------XOSC Resp Generation Initiated!------------------\n");
                    Tools.write(ConsoleColor.White, "Current Time: ");
                    Tools.write(ConsoleColor.White, CurrentTime.ToString() + "\n");
                    Tools.write(ConsoleColor.White, "Client Connected at: ");
                    Tools.write(ConsoleColor.White, ip.ToString() + "\n");
                    Tools.write(ConsoleColor.White, "Client Session Token: ");
                    Tools.write(ConsoleColor.White, Tools.BytesToHexString(SessionKey) + "\n");
                    Tools.write(ConsoleColor.White, "Client Kvserial: ");
                    Tools.write(ConsoleColor.White, System.Text.Encoding.ASCII.GetString(Kvserial) + "\n");
                    Tools.write(ConsoleColor.White, "Title: ");
                    Tools.write(ConsoleColor.White, Tools.TitleID(Tools.BytesToHexString(Title)) + "\n");

                    Tools.write(ConsoleColor.Magenta, "---------------XOSC Resp Generation Success!------------------\n\n");
                    //Tools.Log(string.Format("[{0}] XSC Resp Generation Success! From: {1} for sessionSerial {2}", CurrentTime.ToString(), ip.ToString(), System.Text.Encoding.ASCII.GetString(Kvserial)));
                    //Tools.Log("========================================================");
                }
                else
                {
                    //Tools.Log(string.Format("Unknown Request Size: {0} at {1} from IP: {2}", i, CurrentTime.ToString(), ip.ToString()));
                    Console.WriteLine("Unknown Request Size: {0} at {1} from IP: {2}", i, CurrentTime.ToString(), ip.ToString());
                    //Tools.Log("========================================================");
                    Console.WriteLine("========================================================");
                    Client.Close();
                    return;
                }
            }
            catch (Exception ex)
            {
                //Tools.Log(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
                Console.Write(string.Format("Message: {0} | stacktrace: {1}", ex.Message, ex.StackTrace));
            }
            Client.Close();
        }

        private static void HandleClientConnectionThread() {
            Tools.XKEServer.Start();
            Tools.XSCServer.Start();
            DataPairRece.tcpListener.Start();
            while (true) {
                try {
                    Thread.Sleep(100);

                    if (Tools.XKEServer.Pending())
                        new Thread(new ParameterizedThreadStart(XKEClientConnectedThread)).Start(Tools.XKEServer.AcceptTcpClient());// xke connection

                    else if (Tools.XSCServer.Pending())
                        new Thread(new ParameterizedThreadStart(XSCClientConnectedThread)).Start(Tools.XSCServer.AcceptTcpClient());// xosc connection

                    else if (DataPairRece.tcpListener.Pending()) 
                        new Thread(new ThreadStart(() => DataPairRece.PairRecvHandleClient(DataPairRece.tcpListener.AcceptTcpClient()))).Start();// lasting set receiver
              
                } 
                catch (Exception ex) 
                {
                    Console.Write(ex.Message);
                }
            }
        }

        static void Main(string[] args) {


            string filePath = "Settings.ini";
            if (!File.Exists(filePath))
            {
                throw new Exception("Settings.ini file not found");
            }

            Global.host = Utils.GetIP();
            Global.Port = Utils.GetPort();

            Console.ForegroundColor = ConsoleColor.White;
            try { CurrentHVBytes = System.IO.File.ReadAllBytes(CurrentHvLoc); }
            catch { Console.WriteLine("File '" + CurrentHvLoc + "' Wasn't Located!"); Thread.Sleep(3500); Environment.Exit(1); }
            NumberOfPairs = SeedDirectoryCount2;
            Console.SetWindowSize(100, 30);
            byte[] DashVer = CurrentHVBytes.Skip(0x2).Take(0x2).ToArray(); string Version = Tools.BytesToHexString(DashVer);
            Console.WriteLine("Current Dash Version: '{0}'\n", Convert.ToInt32(Version, 16));
            Console.WriteLine("LEApi Accepting On Host: {0} XKE: {1} & XSC: {2} & DataDumper: {3}\n", Global.host, Global.Port, Global.Port + 1, Global.Port + 2);
            Console.WriteLine("=================================================================================================");
            Console.Title = "LEApi - Seed Folder Count: '" + NumberOfPairs + "' | Listening for Challenges!";
           

            ChallengeRSAPUB = File.ReadAllBytes("XKE/RSAKey.bin");
            Hypervisor = File.ReadAllBytes("XKE/HV.bin");
            XamHeaderData = File.ReadAllBytes("XSC/XamHeader.bin");
            KrnHeaderData = File.ReadAllBytes("XSC/KrnHeader.bin");
            CurModuleHeaderData = File.ReadAllBytes("XSC/CurModuleHeader.bin");

            if (ChallengeRSAPUB == null || Hypervisor == null || XamHeaderData == null || 
                KrnHeaderData == null || CurModuleHeaderData == null || !Directory.Exists("XKE/Packages")) {
                Console.WriteLine("Important APIModules Couldn't be loaded!");
                Console.ReadLine();
            }
            new Thread(new ThreadStart(HandleClientConnectionThread)).Start();
            Console.ReadKey();
        }
        
    }
}

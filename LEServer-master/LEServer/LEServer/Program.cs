using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Threading;

namespace LE
{
    class LEServer
    {
        public static string host;
        public static string username;
        public static string password;
        public static string database;

        public static int Port;
        public static bool DeveloperServer;
        public static bool Freemode;
        public static bool DebugModePrints;
        public static bool ModuleChecks;
        public static byte[] UpdateXexData;
        public static byte[] ApiKey;

        public static string LogPath = "Logs/" + "Console_Report " + DateTime.Now.ToString("M-d-yyyy") + ".log";
        public static MODULE_STRUCT ServerModuleObj = new MODULE_STRUCT();

        private static void EncryptIp(byte[] IP)
        {
            byte[] rawData = {
                0xFF, 0xAF, 0xA5, 0xE8, 0x1E, 0xFB, 0xB0, 0x8F, 0xB4, 0xFF, 0xC5, 0x99,
                0xE8, 0x53, 0x95, 0xCB, 0xE1, 0x23, 0xFD, 0x46
        };
            Crypto.RC4(ref IP, rawData);
            Console.WriteLine("IP:" + Utilities.BytesToString(IP));
            Console.ReadLine();
        }

        private static void EncryptPort(int Port, int Key)
        {
            int c = 0x60, s = 0x10, x = 0x101 - 5;
            for (c = 0; c < 2; c++)
            {
                x = (s << 4) | (s >> 3);
                x ^= s;
                s ^= 0x6 >> 1;
                x *= 0x5;
            }
            x ^= Key;
            Console.WriteLine("Encrypted Port: " + (Port ^= x));
            Console.ReadLine();
        }


        private static byte[] Data = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
        private static byte[] Key = { 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99 };
        static void Main(string[] Args)
        {

            //byte[] IP = { 192,99,71,19 };
            //EncryptIp(IP);
            //EncryptPort(4168, 0x9C);

            string[] INI = File.ReadAllLines("Settings.ini");
            string[] MysqlIni = File.ReadAllLines("Mysqlconfig.ini");

            host = MysqlIni[1].Replace("host=", "");
            username = MysqlIni[2].Replace("username=", "");
            password = MysqlIni[3].Replace("password=", "");
            database = MysqlIni[4].Replace("database=", "");

            Port = Convert.ToInt32(INI[1].Replace("ListenPort=", ""));
            DeveloperServer = Convert.ToBoolean(INI[2].Replace("DeveloperServer=", ""));

            Console.ForegroundColor = ConsoleColor.White;
            Console.Title = "LEmulation: Setting Up..";
            Console.Clear();
            try
            {
                if (InitThread())
                {
                    new Thread(() => {
                        for (; ; )
                        {
                            Console.Title = $"LE Server - Listening On: {Port}";
                            Thread.Sleep(5000);
                        }
                    }).Start();
                }
            }
            catch (Exception Ex)
            {
                List<Log.PrintQueue> SetupErrorid = Log.GetQueue();
                Log.Add(SetupErrorid, ConsoleColor.Red, "ERROR", null);
                Log.Add(SetupErrorid, ConsoleColor.Red, "Exception Message:", Ex.Message);
                Log.Add(SetupErrorid, ConsoleColor.Red, "StrackTrace:", Ex.StackTrace);
                Log.Print(SetupErrorid);
            }
        }

        private static bool InitThread()
        {
            new Thread(new ThreadStart(Utilities.RefreshLog)).Start();

            DbConnection.SetupConnection();
            MySql.UpdateServerSettings();
            new Thread(new ThreadStart(MySql.UpdateServerSettingsThread)).Start();

            List<Log.PrintQueue> Initid = Log.GetQueue();
            Log.Add(Initid, ConsoleColor.DarkCyan, "INIT", null);
            Log.Add(Initid, ConsoleColor.DarkGreen, "ClientHandling port:", $"{Port}");
            Log.Add(Initid, (ModuleChecks) ? ConsoleColor.Green : ConsoleColor.Red, "Xex updating:", $"{ModuleChecks}");
            Log.Add(Initid, (Freemode) ? ConsoleColor.Green : ConsoleColor.Red, "Freemode status:", $"{Freemode}");
            Log.Add(Initid, (DeveloperServer) ? ConsoleColor.Green : ConsoleColor.Red, "Developer status:", $"{DeveloperServer}");
            Log.Print(Initid);

            List<Log.PrintQueue> mysqlid = Log.GetQueue();
            Log.Add(mysqlid, ConsoleColor.DarkGreen, "Mysql host:", $"{host}");
            Log.Add(mysqlid, ConsoleColor.DarkGreen, "Mysql Username:", $"{username}");
            Log.Add(mysqlid, ConsoleColor.DarkGreen, "Mysql Password:", $"{password}");
            Log.Add(mysqlid, ConsoleColor.DarkGreen, "Mysql Database:", $"{database}");
            Log.Print(mysqlid);

            if (DeveloperServer)
            {
                if (!File.Exists("LiveEmulation.xex") /*&& Environment.MachineName != "DESKTOP-G7LRQM8"*/)
                {
                    List<Log.PrintQueue> SetupDevErrorid = Log.GetQueue();
                    Log.Add(SetupDevErrorid, ConsoleColor.Red, "ERROR", null);
                    Log.Add(SetupDevErrorid, ConsoleColor.Red, "Local xex for Dev server missing!", null);
                    Log.Print(SetupDevErrorid);

                    Thread.Sleep(1000);
                    Utilities.RestartServer();
                    return false;
                }
            }

            ApiKey = Utilities.ReadData("Data/PrivateKey.bin");
            ServerModuleObj.ClientINIData = Utilities.ReadData("Data/LE.ini");
            ServerModuleObj.B02BypassData = Utilities.ReadData("Data/Patches/B02Bypass.bin");
            ServerModuleObj.GhostsBypassData = Utilities.ReadData("Data/Patches/GhostsBypass.bin");
            ServerModuleObj.AWBypassData = Utilities.ReadData("Data/Patches/AwBypass.bin");

            if (UpdateXexData == null || ServerModuleObj.ClientINIData == null || ServerModuleObj.AWBypassData == null || ServerModuleObj.B02BypassData == null ||
                ServerModuleObj.GhostsBypassData == null || ApiKey == null)
            {
                List<Log.PrintQueue> MiscPatchesErrorid = Log.GetQueue();
                Log.Add(MiscPatchesErrorid, ConsoleColor.Red, "ERROR", null);
                Log.Add(MiscPatchesErrorid, ConsoleColor.Red, "One or more of the startup modules is missing!", null);
                Log.Print(MiscPatchesErrorid);

                Thread.Sleep(1000);
                Utilities.RestartServer();
                return false;
            }

            List<Log.PrintQueue> ModulesStatusid = Log.GetQueue();
            Log.Add(ModulesStatusid, ConsoleColor.White, "LOADED PATCHES", null);
            Log.Add(ModulesStatusid, ConsoleColor.DarkGreen, "BO2Patches:", string.Format("0x{0:X}", ServerModuleObj.B02BypassData.Length));
            Log.Add(ModulesStatusid, ConsoleColor.DarkGreen, "GhostsPatches:", string.Format("0x{0:X}", ServerModuleObj.GhostsBypassData.Length));
            Log.Add(ModulesStatusid, ConsoleColor.DarkGreen, "AwPatches:", string.Format("0x{0:X}", ServerModuleObj.AWBypassData.Length));
            Log.Print(ModulesStatusid);

            ClientHandler TcpListener = new ClientHandler();
            TcpListener.Start();
            return true;
        }
    }
}


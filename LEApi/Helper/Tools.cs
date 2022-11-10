using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Net.Sockets;
using System.Net;
namespace LEApi {

    class Global
    {
        public static int Port;
        public static string host;
    }



    class INIReader
    {
        string Path;
        string EXE = Assembly.GetExecutingAssembly().GetName().Name;
        string SettingsName = "Settings";


        [DllImport("kernel32")]
        static extern long WritePrivateProfileString(string Section, string Key, string Value, string FilePath);

        [DllImport("kernel32")]
        static extern int GetPrivateProfileString(string Section, string Key, string Default, StringBuilder RetVal, int Size, string FilePath);

        public INIReader(string IniPath = null)
        {
            //Path = new FileInfo(IniPath ?? EXE + ".ini").FullName.ToString();
            Path = new FileInfo(IniPath ?? SettingsName + ".ini").FullName.ToString();
        }

        public string Read(string Key, string Section = null)
        {
            var RetVal = new StringBuilder(255);
            GetPrivateProfileString(Section ?? EXE, Key, "", RetVal, 255, Path);
            return RetVal.ToString();
        }

        public void Write(string Key, string Value, string Section = null)
        {
            WritePrivateProfileString(Section ?? EXE, Key, Value, Path);
        }

        public void DeleteKey(string Key, string Section = null)
        {
            Write(Key, null, Section ?? EXE);
        }

        public void DeleteSection(string Section = null)
        {
            Write(null, null, Section ?? EXE);
        }

        public bool KeyExists(string Key, string Section = null)
        {
            return Read(Key, Section).Length > 0;
        }

    }

    internal class IniParsing
    {
        private string path;
        string SettingsName = "config";
        string EXE = Assembly.GetExecutingAssembly().GetName().Name;

        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);


        public IniParsing(string INIPath)
        {
            //path = INIPath;
            path = new FileInfo(INIPath ?? SettingsName + ".ini").FullName.ToString();
        }

        public void IniWriteValue(string Section, string Key, string Value)
        {
            WritePrivateProfileString(Section, Key, Value, this.path);
        }

        public string IniReadValue(string Section, string Key)
        {
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(Section, Key, "", temp, 255, this.path);
            return temp.ToString();
        }

    }

    class Utils
    {

        //public static IniParsing LoadedIni;

        public static INIReader INI = new INIReader("Settings.ini");


        public static int GetPort()
        {
            return Convert.ToInt32(INI.Read("Port", "Setting"));
        }

        public static string GetIP()
        {
            return INI.Read("Host", "Setting");
        }

    }

    class Tools {

        //private static int XKEPort = 3000;
        public static TcpListener XKEServer = new TcpListener(IPAddress.Parse(Global.host), Global.Port);
        public static TcpListener XSCServer = new TcpListener(IPAddress.Parse(Global.host), Global.Port + 1);

        public static byte[] SHA1(byte[] Buffer)
        {
            return System.Security.Cryptography.SHA1.Create().ComputeHash(Buffer);
        }

        public static byte[] XeCryptBnQw_SwapDwQwLeBe(byte[] input)
        {

            byte[] buffer = new byte[input.Length];
            int num = input.Length - 8;
            int num2 = 0;

            for (int i = 0; i < (input.Length / 8); i++)
            {

                for (int j = 0; j < 8; j++)
                {
                    buffer[num2 + j] = input[num + j];
                }
                num -= 8;
                num2 += 8;
            }
            return buffer;
        }

        public static string TitleID(string TitleId)
        {
            string ReturnVal = TitleId;

            switch (TitleId)
            {
                case "FFFE07D1": ReturnVal = "Dashboard"; break;
                case "FFFF0055": ReturnVal = "XexMenu"; break;
                case "00000170": ReturnVal = "XexMenu2"; break;
                case "FFFE07DE": ReturnVal = "Xbox Act Creator"; break;
                case "F5D10000": ReturnVal = "FSD"; break;
                case "394707D1": ReturnVal = "Netflix"; break;
                case "423607D3": ReturnVal = "Youtube"; break;
                case "4156081C": ReturnVal = "CODWaW"; break;
                case "415607E6": ReturnVal = "COD4"; break;
                case "41560855": ReturnVal = "BO1"; break;
                case "41560817": ReturnVal = "MW2"; break;
                case "415608CB": ReturnVal = "MW3"; break;
                case "415608C3": ReturnVal = "BO2"; break;
                case "415608FC": ReturnVal = "Ghosts"; break;
                case "41560914": ReturnVal = "AW"; break;
                case "4156091D": ReturnVal = "B03"; break;
                case "545408A7": ReturnVal = "GTAV"; break;
                case "545407F2": ReturnVal = "GTA IV"; break;
                case "575207FD": ReturnVal = "Mortal Kombat"; break;
                case "584111F7": ReturnVal = "MineCraft"; break;
                case "454109BA": ReturnVal = "BF4"; break;
                case "4D530919": ReturnVal = "Halo4"; break;
                case "4B4D07F2": ReturnVal = "Dead Island Riptide"; break;
                default: ReturnVal = TitleId; break;
            }
            return ReturnVal;
        }

        public static byte[] FillData(byte data, int length) {
            byte[] buffer = new byte[length];

            for (int i = 0; i < length; i++) {
                buffer[i] = data;
            }
            return buffer;
        }

        public static void Log(string Text)
        {

            if (!File.Exists("XboxArmorApi.log")) File.Create("XboxArmorApi.log");

            string message = "";
            message = "[" + DateTime.Now.ToString("M:d:yyyy h:mm:sstt") + "] " + Text;

            using (StreamWriter sw = File.AppendText("XboxArmorApi.log"))
                sw.WriteLine(message + "");
        }

        public static void AppendText(string str, ConsoleColor color)
        {
            Console.ForegroundColor = color;
            string time = string.Format("{0:hh:mm:ss tt}", DateTime.Now.ToUniversalTime().ToLocalTime());
            Console.WriteLine(string.Concat(new object[] { "[", time, "]", " ", str }));
        }

        private static void Swap(byte[] s, int i, int j)
        {
            byte c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
        private static byte[] RC4Initalize(byte[] key)
        {
            byte[] s = Enumerable.Range(0, 256).Select(i => (byte)i).ToArray();
            for (int i = 0, j = 0; i < 256; i++)
            {
                j = (j + key[i % key.Length] + s[i]) & 255;
                Swap(s, i, j);
            }
            return s;
        }

        private static IEnumerable<byte> RC4Output(byte[] key, IEnumerable<byte> data)
        {
            byte[] s = RC4Initalize(key);
            int i = 0, j = 0;
            return data.Select((b) =>
            {
                i = (i + 1) & 255;
                j = (j + s[i]) & 255;
                Swap(s, i, j);
                return (byte)(b ^ s[(s[i] + s[j]) & 255]);
            });
        }

        public static byte[] CRC4(byte[] key, byte[] data)
        {
            return RC4Output(key, data).ToArray();
        }

        public static byte[] FixEndian(uint Address) {
            byte[] tmp = BitConverter.GetBytes(Address);
            Array.Reverse(tmp);
            return tmp;
        }

        public static byte[] FixEndian(int Address) {
            byte[] tmp = BitConverter.GetBytes(Address);
            Array.Reverse(tmp);
            return tmp;
        }

        public static byte[] FixEndian(Int64 Address) {
            byte[] tmp = BitConverter.GetBytes(Address);
            Array.Reverse(tmp);
            return tmp;
        }

        public static byte[] FixEndianlong(long Address) {
            byte[] tmp = BitConverter.GetBytes(Address);
            Array.Reverse(tmp);

            return tmp;
        }

        public static byte[] StringToByteArray(String hex) {
            int NumberChars = hex.Length;
            byte[] bytes = new byte[NumberChars / 2];

            for (int i = 0; i < NumberChars; i += 2) bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);

            return bytes;
        }


        public static string BytesToHexString(byte[] buffer)
        {
            string str = "";
            for (int i = 0; i < buffer.Length; i++) str += buffer[i].ToString("X2");
            return str;
        }

        public static void write(ConsoleColor color, string msg)
        {
            Console.ForegroundColor = color;
            Console.Write(msg);
            Console.ForegroundColor = ConsoleColor.White;
        }
    }
}

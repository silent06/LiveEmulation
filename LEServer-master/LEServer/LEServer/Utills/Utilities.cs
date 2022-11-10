using Security;
using System;
using System.IO;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;
using System.Windows;
using static System.Net.Mime.MediaTypeNames;

namespace LE {
   class Utilities {

        public static byte[] FillData(byte data, int length) {
            byte[] buffer = new byte[length];

            for (int i = 0; i < length; i++) {
                buffer[i] = data;
            }
            return buffer;
        }

        public static byte[] CopyOfRange(byte[] src, int from, int to) {
            int length = 0;
            return CopyOfRange(src, from, to, ref length);
        }

        public static byte[] CopyOfRange(byte[] src, int from, int to, ref int length) {
            length = to - from;

            byte[] buffer = new byte[length];
            Buffer.BlockCopy(src, from, buffer, 0, Math.Min(src.Length - from, length));
            return buffer;
        }

        public static byte[] StringToBytes(string HexString) {
            byte[] buffer = new byte[HexString.Length / 2];
            for (int i = 0; i < HexString.Length; i += 2) {
                try {
                    buffer[i / 2] = Convert.ToByte(HexString.Substring(i, 2), 0x10);
                }
                catch {
                    buffer[i / 2] = 0;
                }
            }
            return buffer;
        }

        public static string BytesToString(byte[] Buffer) {
            string str = "";
            for (int i = 0; i < Buffer.Length; i++) str = str + Buffer[i].ToString("X2");
            return str;
        }

        public static bool IsEmpty(byte[] Buffer) {
            for (int i = 0; i < Buffer.Length; i++) {
                if (Buffer[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        public static bool CompareBytes(byte[] target1, byte[] target2) {
            if (target1.Length != target2.Length) {
                return false;
            }
            for (int i = 0; i < target1.Length; i++) {
                if (target1[i] != target2[i]) {
                    return false;
                }
            }
            return true;
        }
        
        public static string Validategamertag(byte[] Buffer) {
            string Gametag = Encoding.UTF8.GetString(TtrimZeros(Buffer));
            return Gametag;
        }
        
        public static CLIENT_KVSTATUS Update_LiveStatus(string status, ref CLIENT_STRUCT ClientObj) {
          
            switch (status) {   
                case "8015190D": return ClientObj.kvstatus = CLIENT_KVSTATUS.BANNED;
                case "80151001": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR; // XONLINE_E_LOGON_CANNOT_ACCESS_SERVICE
                case "80151002": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR; // XONLINE_E_LOGON_UPDATE_REQUIRED
                case "80151003": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR; // XONLINE_E_LOGON_SERVERS_TOO_BUSY
                case "80151004": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR; // XONLINE_E_LOGON_CONNECTION_LOST
                case "8015100B": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR;
                case "8015100C": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR;
                case "8015100D": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR;
                case "8015100E": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR;
                case "8015100F": return ClientObj.kvstatus = CLIENT_KVSTATUS.ERROR;

            }
            return ClientObj.kvstatus = CLIENT_KVSTATUS.UNBANNED;
        }

        public static string GetConsoleType(int Type) {
            string consoletype = "Error";
            switch (Type) {

                case 0x00000000: consoletype = "Xenon"; break;
                case 0x10000000: consoletype = "Zephyr"; break;
                case 0x20000000: consoletype = "Falcon"; break;
                case 0x30000000: consoletype = "Jasper"; break;
                case 0x40000000: consoletype = "Trinity"; break;
                case 0x50000000: consoletype = "Corona"; break;
                case 0x60000000: consoletype = "WinChester"; break;     
            } 
            return consoletype;
        }

        public static string TitleID(string TitleId) {
            string ReturnVal = TitleId;

            switch (TitleId) {
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
                case "584111F7": ReturnVal = "MineCraft"; break;
                case "454109BA": ReturnVal = "BF4"; break;
                case "4D530919": ReturnVal = "Halo4"; break;
                default: ReturnVal = TitleId; break;
            }
            return ReturnVal;
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

        public static byte[] FixEndian(short Address)
        {
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

        public static string FirstCharToUpper(string value) {
            char[] array = value.ToCharArray();
            if (array.Length >= 1) {
                if (char.IsLower(array[0])) {
                    array[0] = char.ToUpper(array[0]);
                }
            }
            for (int i = 1; i < array.Length; i++) {
                if (array[i - 1] == ' ') {
                    if (char.IsLower(array[i])) {
                        array[i] = char.ToUpper(array[i]);
                    }
                }
            }
            return new string(array);
        }

        public static byte[] TtrimZeros(byte[] input) {
            int i = input.Length;
            byte[] output = new byte[i + 1];

            while (i-- > 0 && input[i] == 0) { }
            Buffer.BlockCopy(input, 0, output, 0, i + 1);

            return output;
        }

        public static byte[] ReadData(string path) {
            byte[] Data = null;
            Data = File.ReadAllBytes(path);
            return Data;
        }
        
        public static void RestartServer() {
            Environment.Exit(0);
        }
        
        public static void WriteLog(string txt, bool lineBreak) {
            try {
                if (!File.Exists(LEServer.LogPath)) using (StreamWriter sw = File.CreateText(LEServer.LogPath)) sw.WriteLine("");
                if (lineBreak) using (StreamWriter sw = File.AppendText(LEServer.LogPath)) sw.WriteLine("[{0}]  {1}", DateTime.Now.ToString("M:d:yyyy h:mm:sstt"), txt);
                else using (StreamWriter sw = File.AppendText(LEServer.LogPath)) sw.Write(txt);
            } catch { }
        }
        
        public static void RefreshLog() {
            for (;;) {

                Thread.Sleep(86400000);
                LEServer.LogPath = "Logs/" + "Console_Report " + DateTime.Now.ToString("M-d-yyyy") + ".log";
                File.CreateText(LEServer.LogPath);
                Console.Clear();
            }
        }

        public static string WindowscmdExec(string cmd) {
            var process = new Process() {
                StartInfo = new ProcessStartInfo("cmd") {
                    UseShellExecute = false,
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true,
                    Arguments = string.Format("/c \"{0}\"", cmd)
                }
            };
            process.Start();
            return process.StandardOutput.ReadToEnd();
        } 

        public static void CheckForUpdate() {

            while (true) {
                try {

                    byte[] UpdatedInstance = File.ReadAllBytes("../../Data/ListenerModule/LEmulationServer.exe");
                    byte[] UpdatedInstanceHash = Crypto.SHA1(UpdatedInstance);
                    byte[] OurInstanceHash = Crypto.SHA1(File.ReadAllBytes("LEmulationServer.exe"));

                    if (!CompareBytes(OurInstanceHash, UpdatedInstanceHash)) {

                        List<Log.PrintQueue> UpdateListenerId = Log.GetQueue();
                        Log.Add(UpdateListenerId, ConsoleColor.DarkYellow, "LISTENER UPDATE", null);
                        Log.Add(UpdateListenerId, ConsoleColor.Yellow, "Updated Instance Size:", string.Format("0x{0:X}", UpdatedInstance.Length));
                        Log.Print(UpdateListenerId); 
                    }
                } catch(Exception Ex) {

                    List<Log.PrintQueue> UpdateListenerErrorId = Log.GetQueue();
                    Log.Add(UpdateListenerErrorId, ConsoleColor.DarkRed, "LISTENER UPDATE", null);
                    Log.Add(UpdateListenerErrorId, ConsoleColor.Yellow, "Exception Message:", Ex.Message);
                    Log.Add(UpdateListenerErrorId, ConsoleColor.Yellow, "Exception StackTrace:", Ex.StackTrace);
                    Log.Print(UpdateListenerErrorId);
                }
                Thread.Sleep(2000);
            }
        }

    }
}

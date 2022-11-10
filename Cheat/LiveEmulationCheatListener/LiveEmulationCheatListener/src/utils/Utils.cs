using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class Utils {
        public static string GetNameFromTitle(int title) {
            switch (title) {
                case 0x41560914:
                    return "Advanced Warfare";
                case 0x415608FC:
                    return "Ghosts";
                case 0x41560855:
                    return "Black Ops 1";
                case 0x415608C3:
                    return "Black Ops 2";
                case 0x4156091D:
                    return "Black Ops 3";
                case 0x415607E6:
                    return "Modern Warfare 1";
                case 0x41560817:
                    return "Modern Warfare 2";
                case 0x415608CB:
                    return "Modern Warfare 3";
                case 0x4156081C:
                    return "World at War";
            }

            return null;
        }

        public static string BytesToString(byte[] Buffer) {
            string str = "";
            for (int i = 0; i < Buffer.Length; i++) str = str + Buffer[i].ToString("X2");
            return str;
        }

        public static string BytesToStringSpaced(byte[] Buffer) {
            string str = "";
            for (int i = 0; i < Buffer.Length; i++) str = str + Buffer[i].ToString("X2") + " ";
            return str;
        }

        public static byte[] StringToBytes(string str) {
            Dictionary<string, byte> hexindex = new Dictionary<string, byte>();
            for (int i = 0; i <= 255; i++)
                hexindex.Add(i.ToString("X2"), (byte)i);

            List<byte> hexres = new List<byte>();
            for (int i = 0; i < str.Length; i += 2)
                hexres.Add(hexindex[str.Substring(i, 2)]);

            return hexres.ToArray();
        }

        public static long GetTimeStamp() {
            return DateTimeOffset.UtcNow.ToUnixTimeSeconds();
        }

        public static string WindowsCmdExec(string cmd) {
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

        public static void BanFromFirewall(string ip) {
            WindowsCmdExec(string.Format("netsh advfirewall firewall add rule name=\"" +
               "@{0}\" " + "dir=in interface=any action=block remoteip={0}", ip));

            Console.WriteLine("{0} has been banned from the firewall for spamming", ip);
        }

        public static void UnbanFromFirewall(string ip) {
            WindowsCmdExec(string.Format("netsh advfirewall firewall delete rule name=\"" +
               "@{0}\"", ip));

            Console.WriteLine("{0} has been unbanned from the firewall", ip);
        }
    }
}

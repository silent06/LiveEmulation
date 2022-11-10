using System;
using System.Net;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
//using System.IO;
namespace LE.Responces {
    class Auth {
        public static void ProcessAuthorization(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            byte[] FuseLineKey = io.reader.ReadBytes(0x10);
            string KvSerial = System.Text.Encoding.ASCII.GetString(io.reader.ReadBytes(0xC));
            int ConsoleType = io.reader.ReadInt32();
            byte[] SessionToken = Crypto.Generate_RandomData(0x10);
            string IP = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];

            bool ClientFound = MySql.GetClient(ref ClientObj, Utilities.BytesToString(FuseLineKey));
            ClientObj.consoletype = Utilities.GetConsoleType(ConsoleType);

            if (!ClientFound && ClientObj.consoletype != "Error") {
                MySql.InsertClient(Utilities.BytesToString(FuseLineKey), Utilities.BytesToString(SessionToken));
                ClientFound = MySql.GetClient(ref ClientObj, Utilities.BytesToString(FuseLineKey));
            }
            if (!ClientFound || ClientObj.consoletype == "Error") {
                Console.WriteLine("Console type error for client Cpu: " + Utilities.BytesToString(FuseLineKey));
                io.writer.Write((int)PACKET_STATUS.ERROR);
                return;
            }

            if (KvSerial != ClientObj.kvserial)
                MySql.UpdateKvThread(ClientObj, true);

            ClientObj.mapcordinates = "Not Set";
            WebClient Client = new WebClient();
            if (ClientObj.ip != IP || ClientObj.mapcordinates == "Not Set") {
                try {
                    string request = "http://www.geoplugin.net/json.gp?ip=" + IP;
                    string Resp = Client.DownloadString(request);
                    JObject obj = JObject.Parse(Resp);
                    JObject finalobj = JObject.Parse(("{latLng: [" + obj["geoplugin_latitude"].ToString() + ", " + obj["geoplugin_longitude"] + "], name: \'" + obj["geoplugin_city"].ToString().Replace("'", "\'") + "\'}"));
                    ClientObj.mapcordinates = finalobj.ToString();
                    
                } catch {
                    ClientObj.mapcordinates = "Not Set";
                }
            }

            ClientObj.sessiontoken = Utilities.BytesToString(SessionToken);
            ClientObj.ip = IP;
            ClientObj.lastloginTime = DateTime.Now;
            ClientObj.titleid = "Not Set";
            ClientObj.gamertag = "Not Set";
            ClientObj.kvserial = KvSerial;

            MySql.SaveClient(ClientObj, Utilities.BytesToString(FuseLineKey));
            io.writer.Write(SessionToken);
            TimeSpan ClientTime = ClientObj.time - DateTime.Now;
            List<Log.PrintQueue> Authid = Log.GetQueue();
            Log.Add(Authid, ConsoleColor.DarkMagenta, "|AUTH|", null);
            Log.Add(Authid, ConsoleColor.Magenta, "CPUKey:", $"{Utilities.BytesToString(FuseLineKey)}");
            Log.Add(Authid, (ClientObj.authstatus == CLIENT_AUTHSTATUS.DEVELOPER) ? ConsoleColor.Green : ConsoleColor.Red, "Dev enabled:", (ClientObj.authstatus == CLIENT_AUTHSTATUS.DEVELOPER) ? "Yes" : "No");
            Log.Add(Authid, ConsoleColor.Magenta, "IP:", $"{IP}");
            Log.Add(Authid, ConsoleColor.Magenta, "KvSerial:", $"{KvSerial}");
            Log.Add(Authid, ConsoleColor.Magenta, "Session Key:", $"{Utilities.BytesToString(SessionToken)}");
            Log.Add(Authid, ConsoleColor.Magenta, "Auth Status:", $"{ClientObj.authstatus}");
            Log.Add(Authid, ConsoleColor.Magenta, "Console Type:", $"{Utilities.GetConsoleType(ConsoleType)}");
           
            if (ClientObj.authstatus == CLIENT_AUTHSTATUS.NOTIME)
                Log.Add(Authid, ConsoleColor.DarkRed, "Time Remaining:", "Expired");
            else {
                if (ClientObj.authstatus == CLIENT_AUTHSTATUS.AUTHED)
                    Log.Add(Authid, ConsoleColor.DarkGreen, "Time Remaining:", $"D:{ClientTime.Days} | H:{ClientTime.Hours} | M:{ ClientTime.Minutes}");
                else if (ClientObj.authstatus == CLIENT_AUTHSTATUS.LIFETIME)
                    Log.Add(Authid, ConsoleColor.DarkYellow, "Time Remaining:", "Unlimited Access");
            }
            Log.Print(Authid);
        }
    }
}
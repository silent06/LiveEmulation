using System;
using LE;
using System.Threading;
using Security;
using System.Collections.Generic;

namespace LE.Responces {
    class Status {

        public static void ProcessStatus(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            byte[] ClientXeXHash = io.reader.ReadBytes(0x10);

            bool MisMatch = false;
            byte[] Resp = new byte[0x8];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;
            
            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                if (LEServer.ModuleChecks && ClientObj.authstatus != CLIENT_AUTHSTATUS.DEVELOPER) {
                    byte[] ServerXeXHash = Crypto.HMACSHA1(LEServer.UpdateXexData, Utilities.StringToBytes(ClientObj.sessiontoken), 0);
                    if (!Utilities.CompareBytes(ClientXeXHash, ServerXeXHash)) {

                        MisMatch = true;
                        Data.Write((int)PACKET_STATUS.UPDATE);

                        List<Log.PrintQueue> Statusid = Log.GetQueue();
                        Log.Add(Statusid, ConsoleColor.DarkYellow, "|UPDATE|", null);
                        Log.Add(Statusid, ConsoleColor.Yellow, "Session Token:", $"{SessionToken}");
                        Log.Add(Statusid, ConsoleColor.Yellow, "Server Hash:", $"{Utilities.BytesToString(ServerXeXHash)}");
                        Log.Add(Statusid, ConsoleColor.Yellow, "Client Hash:", $"{Utilities.BytesToString(ClientXeXHash)}");
                        Log.Print(Statusid);
                        
                    } 
                    else
                        Data.Write((int)PACKET_STATUS.SUCCESS);
                } 
                else
                    Data.Write((int)PACKET_STATUS.SUCCESS);
            } 
            else
            {
                Data.Write((int)PACKET_STATUS.ERROR);
                Console.WriteLine("Console not found" + ClientObj.cpukey);
            }

            Data.Write(LEServer.UpdateXexData.Length);
            io.writer.Write(Resp);
            if (MisMatch)
                io.writer.Write(LEServer.UpdateXexData);
            MySql.SaveClient(ClientObj, SessionToken);
        }
    }
}


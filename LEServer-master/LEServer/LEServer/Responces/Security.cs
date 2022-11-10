using Security;
using System;
using System.Collections.Generic;

namespace LE.Responces {
    class Security {

        public static void ProcessSecurity(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string KvCpu = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string HvCpuKey = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string FuseLineKey = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string ip = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];
           
            int ConsolesFoundUsingCpu = MySql.Countconsolesusingcpu(ClientObj);
            bool Flagged = false;

            byte[] Resp = new byte[0x4];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;

            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                if (!LEServer.ModuleChecks) {
                    Data.Write((int)PACKET_STATUS.SUCCESS);
                    io.writer.Write(Resp);
                    return;
                }

                // if another client is using this cpukey
                if (ConsolesFoundUsingCpu > 1) {
                    Flagged = true;

                    // ban every client using this cpukey
                    for (int i = 0; i < ConsolesFoundUsingCpu; i++)
                        MySql.BanClient(ClientObj);
                        ClientHandler.FireWallBanEvent(ip, "CPU spoofing");
                }

                // First ever connected cpu
                if (FuseLineKey != ClientObj.cpukey) {
                    List<Log.PrintQueue> BanId = Log.GetQueue();
                    Log.Add(BanId, ConsoleColor.DarkRed, "BAN EVENT RECORDED", null);
                    Log.Add(BanId, ConsoleColor.Red, "CPUKey:", FuseLineKey);
                    Log.Add(BanId, ConsoleColor.Red, "Reason: Connected cpu doesnt match stored cpu", null);
                    
                    Flagged = true;
                }
                
                // current clients fuse lines vs hvcpu
                if (FuseLineKey != HvCpuKey) {
                    Flagged = true;

                    List<Log.PrintQueue> BanId = Log.GetQueue();
                    Log.Add(BanId, ConsoleColor.DarkRed, "BAN EVENT RECORDED", null);
                    Log.Add(BanId, ConsoleColor.Red, "CPUKey:", FuseLineKey);
                    Log.Add(BanId, ConsoleColor.Red, "Reason: xke not running and fuseline does not equal hvcp", null);
                    
                }
                
                // end of checks punishment time
                if (Flagged) {
                    if (!LEServer.DeveloperServer) {
                        ClientObj.authstatus = CLIENT_AUTHSTATUS.BANNED;
                    }
                }
                Data.Write((int)PACKET_STATUS.SUCCESS);
            } else {
                Data.Write((int)PACKET_STATUS.ERROR);
            }

            io.writer.Write(Resp);
            MySql.SaveClient(ClientObj, SessionToken);
        }
    }
}

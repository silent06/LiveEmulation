using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class PacketHeartbeat {
        public static void ServerPacketHeartbeat(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketHeartbeat", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);
            Log.Print(logId);
        }
    }
}

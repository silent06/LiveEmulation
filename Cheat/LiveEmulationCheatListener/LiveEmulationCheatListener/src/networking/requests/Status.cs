using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class PacketStatus {
        public static void ServerPacketStatus(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketStatus", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            byte[] response = new byte[0x101];
            Array.Clear(response, 0, 0x101);

            char[] error = new char[0x100];
            Array.Clear(error, 0, 0x100);

            bool wasSuccessful = true;

            EndianWriter writer = new EndianWriter(new MemoryStream(response), EndianStyle.BigEndian);

            ClientData client = new ClientData();
            if (!MySQL.GetClientData(Utils.BytesToString(header.m_CPU), ref client)) {
                wasSuccessful = false;

                string err = "[CE] CPU not found";
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client CPU not found", ip);
                goto endshit;
            }

            string banReason = null;
            if(MySQL.IsClientBanned(Utils.BytesToString(header.m_CPU), ref banReason)) {
                wasSuccessful = false;

                string err = "[CE] Banned: " + banReason;
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client is banned - " + banReason, ip);
            }

        endshit:
            writer.Write(wasSuccessful);
            writer.Write(error);
            writer.Close();

            Security.SendPacket(serverWriter, header, response);
            Log.Add(logId, ConsoleColor.Green, "Status", "Response sent", ip);

            Log.Print(logId);
        }
    }
}
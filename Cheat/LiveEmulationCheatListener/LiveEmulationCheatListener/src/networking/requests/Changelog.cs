using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketChangelog {
        public static void ServerPacketChangelog(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketChangelog", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            byte[] response = new byte[0x301];
            Array.Clear(response, 0, 0x301);

            char[] msg = new char[0x300];
            Array.Clear(msg, 0, 0x300);

            bool wasSuccessful = false;

            EndianWriter writer = new EndianWriter(new MemoryStream(response), EndianStyle.BigEndian);

            ClientData client = new ClientData();
            if (!MySQL.GetClientData(Utils.BytesToString(header.m_CPU), ref client)) {
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client CPU not found", ip);
                goto endshit;
            }

            string banReason = null;
            if (MySQL.IsClientBanned(Utils.BytesToString(header.m_CPU), ref banReason)) {
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client is banned - " + banReason, ip);
                goto endshit;
            }

            wasSuccessful = true;

            string changelogMessage = File.ReadAllText("LatestChangelog-" + header.m_Title.ToString() + ".txt");
            if (changelogMessage.Length > 0) {
                Array.Copy(changelogMessage.ToCharArray(), 0, msg, 0, changelogMessage.Length);
            }

        endshit:
            writer.Write(wasSuccessful);
            writer.Write(msg);
            writer.Close();

            Security.SendPacket(serverWriter, header, response);
            Log.Add(logId, ConsoleColor.Green, "Status", "Response sent", ip);

            Log.Print(logId);
        }
    }
}

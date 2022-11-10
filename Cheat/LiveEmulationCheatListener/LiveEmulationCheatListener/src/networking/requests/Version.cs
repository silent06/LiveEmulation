using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketVersion {
        public static void ServerPacketVersion(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketVersion", ip);
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

            int version = reader.ReadInt32();

            int latest = MySQL.GetLatestCheatVersion(header.m_Title);
            if (latest.Equals(0)) {
                wasSuccessful = false;

                string err = "[CE] Title not found";
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", string.Format("Title not found ({0})", header.m_Title), ip);
                goto endshit;
            }

            if (version < latest) {
                wasSuccessful = false;

                string err = "[CE] XEX is outdated";
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", string.Format("XEX is outdated (latest: {0}, recieved: {1})", latest, version), ip);
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

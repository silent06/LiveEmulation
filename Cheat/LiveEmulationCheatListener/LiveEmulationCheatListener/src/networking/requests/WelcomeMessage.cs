using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketWelcomeMessage {
        public static void ServerPacketWelcomeMessage(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketWelcomeMessage", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            byte[] response = new byte[0x301];
            Array.Clear(response, 0, 0x301);

            char[] msg = new char[0x300];
            Array.Clear(msg, 0, 0x300);

            bool isMessage = false;

            EndianWriter writer = new EndianWriter(new MemoryStream(response), EndianStyle.BigEndian);

            ClientData client = new ClientData();
            if (!MySQL.GetClientData(Utils.BytesToString(header.m_CPU), ref client)) {
                isMessage = false;
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client CPU not found", ip);
                goto endshit;
            }

            string banReason = null;
            if (MySQL.IsClientBanned(Utils.BytesToString(header.m_CPU), ref banReason)) {
                isMessage = false;
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client is banned - " + banReason, ip);
                goto endshit;
            }

            if (!client.m_HasUsedBefore) {
                isMessage = true;
                string welcomeMessage = File.ReadAllText("Welcome.txt");
                if (welcomeMessage.Length > 0) {
                    Array.Copy(welcomeMessage.ToCharArray(), 0, msg, 0, welcomeMessage.Length);
                }

                MySQL.UpdateWelcomeMessageCheck(Utils.BytesToString(header.m_CPU));
            }

        endshit:
            writer.Write(isMessage);
            writer.Write(msg);
            writer.Close();

            Security.SendPacket(serverWriter, header, response);
            Log.Add(logId, ConsoleColor.Green, "Status", "Response sent", ip);

            Log.Print(logId);
        }
    }
}

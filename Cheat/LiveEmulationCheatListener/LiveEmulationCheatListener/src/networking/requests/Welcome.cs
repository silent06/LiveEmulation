using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketWelcome { 
        public static void ServerPacketWelcome(EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketWelcome", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            byte[] response = new byte[289];
            Array.Clear(response, 0, 289);

            byte[] token = new byte[32];
            Array.Clear(token, 0, 32);

            char[] error = new char[256];
            Array.Clear(error, 0, 256);
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

            token = Security.GenerateRandomData(32);
            MySQL.AddRequestToken(Utils.BytesToString(token), Utils.BytesToString(header.m_CPU));
            Log.Add(logId, ConsoleColor.Magenta, "Info", string.Format("Sending token to client: {0}", Utils.BytesToString(token)), ip);

        endshit:
            writer.Write(wasSuccessful);
            writer.Write(error);
            writer.Write(token);
            writer.Close();

            Security.SendPacket(serverWriter, header, response);
            Log.Add(logId, ConsoleColor.Green, "Status", "Response sent", ip);
            Log.Print(logId);
        }
    }
}

using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketGetData {
        public static void ServerPacketGetData(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketGetData", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            Dictionary<int, ulong> _addresses = MySQL.GetAllAddresses(header.m_Title);

            int count = _addresses.Count;
            int size = count + (count * 4) + 1;

            byte[] response = new byte[size + 1]; // bool, size, loop=enumIndex:address
            Array.Clear(response, 0, size + 1);

            bool wasSuccessful = true;

            EndianWriter writer = new EndianWriter(new MemoryStream(response), EndianStyle.BigEndian);

            ClientData client = new ClientData();
            if (!MySQL.GetClientData(Utils.BytesToString(header.m_CPU), ref client)) {
                wasSuccessful = false;

                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client CPU not found", ip);
                goto endshit;
            }

            string banReason = null;
            if (MySQL.IsClientBanned(Utils.BytesToString(header.m_CPU), ref banReason)) {
                wasSuccessful = false;

                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client is banned - " + banReason, ip);
                goto endshit;
            }

        endshit:
            byte[] addresses = new byte[size - 1];
            Array.Clear(addresses, 0, size - 1);

            if (wasSuccessful) {
                int patchPointer = 0;

                for (int i = 0; i < count; i++) {
                    addresses[patchPointer] = (byte)i; // id

                    byte[] address = BitConverter.GetBytes(_addresses[i]);
                    addresses[patchPointer + 1] = address[0];
                    addresses[patchPointer + 2] = address[1];
                    addresses[patchPointer + 3] = address[2];
                    addresses[patchPointer + 4] = address[3];

                    patchPointer += 5;
                }

                Log.Add(logId, ConsoleColor.Cyan, "Information", string.Format("Sending {0} addresses to client", count.ToString()), ip);
            }

            writer.Write(wasSuccessful);
            writer.Write((byte)count);
            writer.Write(addresses);
            writer.Close();

            Security.SendPacket(serverWriter, header, response);
            Log.Add(logId, ConsoleColor.Green, "Status", "Response sent", ip);

            Log.Print(logId);
        }
    }
}

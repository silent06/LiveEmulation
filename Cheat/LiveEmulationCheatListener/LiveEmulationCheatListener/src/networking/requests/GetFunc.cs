using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketGetFunc {
        private enum eFunctions {
            HookFunctionPatch
        }

        public static void ServerPacketGetFunc(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketGetFunc", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            Dictionary<int, string> funcs = MySQL.GetAllFunctions(header.m_Title);

            bool wasSuccessful = true;

            eFunctions index = (eFunctions)reader.ReadInt32();
            if (!Enum.IsDefined(typeof(eFunctions), index)) {
                wasSuccessful = false;
                Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent an invalid command (" + index + ")", ip);
            }

            byte[] functionBytes = Utils.StringToBytes(funcs[(int)index]);
            int functionSize = functionBytes.Length;
            byte[] response = new byte[functionSize + 2]; // bool, size, func

            EndianWriter writer = new EndianWriter(new MemoryStream(response), EndianStyle.BigEndian);

            if (!wasSuccessful) goto endshit; // c# is gay

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
            writer.Write(wasSuccessful);
            writer.Write((byte)functionSize);
            writer.Write(functionBytes);
            writer.Close();

            Security.SendPacket(serverWriter, header, response);
            Log.Add(logId, ConsoleColor.Green, "Status", "Response sent", ip);

            Log.Print(logId);
        }
    }
}

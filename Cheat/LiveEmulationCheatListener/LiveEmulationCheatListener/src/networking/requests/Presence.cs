using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;

namespace LiveEmulationCheatListener {
    class PacketPresence {
        public static void ServerPacketPresence(EndianReader reader, EndianWriter serverWriter, Header header, List<Log.PrintQueue> logId, string ip) {
            Log.Add(logId, ConsoleColor.Blue, "Command", "ServerPacketPresence", ip);
            Log.Add(logId, ConsoleColor.Cyan, "CPU Key", Utils.BytesToString(header.m_CPU), ip);
            Log.Add(logId, ConsoleColor.Magenta, "Game", Utils.GetNameFromTitle(header.m_Title), ip);

            MySQL.UpdateRequestTokenHeartbeat(Utils.BytesToString(header.m_token));

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
            if (MySQL.IsClientBanned(Utils.BytesToString(header.m_CPU), ref banReason)) {
                wasSuccessful = false;

                string err = "[CE] Banned: " + banReason;
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Reporting", "Client is banned - " + banReason, ip);
            }

            //byte[] unused = reader.ReadBytes(4); // EZ BYPASS
            bool isLiveEmulationServerRunning = reader.ReadBoolean();
            byte totalConnections = reader.ReadByte();
            byte functionsToCheck = reader.ReadByte();
            byte[] functions = reader.ReadBytes(functionsToCheck * 4);

            if (!isLiveEmulationServerRunning) {
                wasSuccessful = false;

                string err = "[CE] Security failed (ID: 2).";
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Security", "Server isn't running", ip);
                goto endshit;
            }

            if (totalConnections < 7) {
                wasSuccessful = false;

                string err = "[CE] Security failed (ID: 3).";
                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                Log.Add(logId, ConsoleColor.DarkYellow, "Security", "Connection count isn't what it should be", ip);
                goto endshit;
            }

            for(int i = 0; i < functionsToCheck; i++) {
                int offset = i * 4;
                byte one = functions[offset];
                byte two = functions[offset + 1];
                byte three = functions[offset + 2];
                byte four = functions[offset + 3];

                if(one == 0x4E) {
                    if(two == 0x80) {
                        if(three == 0x00) {
                            if(four == 0x20) {
                                MySQL.BanClient(Utils.BytesToString(header.m_CPU), "Security check in presence detected a BLR");
                                wasSuccessful = false;

                                string err = "[CE] Security failed (ID: 1). Enjoy the ban!";
                                Array.Copy(err.ToCharArray(), 0, error, 0, err.Length);
                                Log.Add(logId, ConsoleColor.DarkYellow, "Security", string.Format("Function at offset {0} has a BLR", i), ip);
                                goto endshit;
                            }
                        }
                    }
                }
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

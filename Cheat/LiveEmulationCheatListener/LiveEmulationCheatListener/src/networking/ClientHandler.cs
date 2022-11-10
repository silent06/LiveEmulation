using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.IO;
using System.Threading;
using System.Collections.Concurrent;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class ClientHandler {
        private static string m_LastBannedIP;
        private TcpListener m_Listener;
        private Thread m_ListenerThread;
        public static Dictionary<string, SocketSpam> m_Connections = new Dictionary<string, SocketSpam>();

        public ClientHandler() {
            m_Listener = new TcpListener(IPAddress.Any, Global.m_Port);
        }

        private bool SpamDetection(string ip) {
            if (ip == m_LastBannedIP)
                return true;

            Global.m_IsUsingSpamDetection = true;
            if (m_Connections.ContainsKey(ip)) {
                if (m_Connections.TryGetValue(ip, out SocketSpam spamOut)) {
                    if (spamOut.m_Banned) return true;

                    spamOut.m_ConnectionsMade++;
                    m_Connections[ip] = spamOut;

                    spamOut.m_ConnectionTimestamps.Add(Utils.GetTimeStamp());

                    int detection = 0;
                    for (int i = 0; i < spamOut.m_ConnectionTimestamps.Count; i++) {
                        if (i == spamOut.m_ConnectionTimestamps.Count - 1) {
                            // last iteration
                            break;
                        } else {
                            // if the current connection timestamp minus the last is within a second
                            if ((spamOut.m_ConnectionTimestamps[i + 1] - spamOut.m_ConnectionTimestamps[i]) <= 1) {
                                detection++;
                            }
                        }
                    }

                    if (detection >= 50) {
                        // if 50 detections of connections made within a second of eachother (highly unlikely)

                        Utils.BanFromFirewall(ip);
                        spamOut.m_BannedTimestamp = Utils.GetTimeStamp();
                        spamOut.m_Banned = true;
                        m_Connections[ip] = spamOut;

                        Console.WriteLine("Socket spam detected from {0}", ip);
                        Global.m_IsUsingSpamDetection = false;
                        m_LastBannedIP = ip;
                        return true;
                    }
                }
            } else {
                m_Connections.Add(ip, new SocketSpam(Utils.GetTimeStamp(), 0, false, 0));
            }

            Global.m_IsUsingSpamDetection = false;
            return false;
        }

        public void Start() {
            m_ListenerThread = new Thread(new ThreadStart(Listen));
            m_ListenerThread.Start();
        }

        private void Listen() {
            m_Listener.Start();

            while (true) {
                Thread.Sleep(100);
                if (m_Listener.Pending()) new Thread(new ParameterizedThreadStart(Handler)).Start(m_Listener.AcceptTcpClient());
            }
        }

        private void Handler(object client) {
            TcpClient tcpClient = (TcpClient)client;
            NetworkStream netStream = tcpClient.GetStream();
            string ip = tcpClient.Client.RemoteEndPoint.ToString().Split(new char[] { ':' })[0];

            if (!netStream.CanRead) {
                Console.WriteLine("Socket closed for {0} - CanRead was false!", ip);
                tcpClient.Close();
                return;
            }

            if (SpamDetection(ip)) {
                Console.WriteLine("Socket closed for {0} - SpamDetection was true!", ip);
                tcpClient.Close();
                return;
            }

            try {
                List<Log.PrintQueue> logId = Log.GetQueue();
                Header header = new Header();

                EndianWriter serverWriter = new EndianWriter(netStream, EndianStyle.BigEndian);

                byte[] neededHeaderData = new byte[0x8];
                if (netStream.Read(neededHeaderData, 0, 0x8) != 0x8) {
                    tcpClient.Close();

                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Failed to read header start", ip);
                    Log.Print(logId);
                    return;
                }

                EndianReader baseHeaderParse = new EndianReader(new MemoryStream(neededHeaderData), EndianStyle.BigEndian);
                header.m_Packet = (Packets)(baseHeaderParse.ReadInt32() ^ 0x10);

                if (!Enum.IsDefined(typeof(Packets), header.m_Packet)) {
                    tcpClient.Close();

                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent an invalid packet (" + header.m_Packet + ")", ip);
                    Log.Print(logId);
                    return;
                }

                header.m_Size = baseHeaderParse.ReadInt32();

                if (header.m_Size > Global.m_MaximumRequestSize) {
                    tcpClient.Close();

                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent a header size bigger than the max (" + header.m_Size + ")", ip);
                    Log.Print(logId);
                    return;
                }

                byte[] data = new byte[header.m_Size - 8];
                if (netStream.Read(data, 0, header.m_Size - 8) != header.m_Size - 8) {
                    tcpClient.Close();

                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Failed to read header (" + header.m_Size + ")", ip);
                    Log.Print(logId);
                    return;
                }

                EndianReader dataReader = new EndianReader(new MemoryStream(data), EndianStyle.BigEndian);

                header.m_Title = dataReader.ReadInt32();
                header.m_CPU = dataReader.ReadBytes(0x10);
                header.m_HvCPU = dataReader.ReadBytes(0x10);
                header.m_token = dataReader.ReadBytes(0x20);
                header.m_Key1 = dataReader.ReadInt32();
                header.m_Key2 = dataReader.ReadInt32();
                header.m_Key3 = dataReader.ReadInt32();
                header.m_Key4 = dataReader.ReadInt32();
                header.m_Key5 = dataReader.ReadInt32();
                header.m_Key6 = dataReader.ReadInt32();
                header.m_Key7 = dataReader.ReadInt32();
                header.m_Key8 = dataReader.ReadInt32();
                header.m_Key9 = dataReader.ReadInt32();
                header.m_Key10 = dataReader.ReadInt32();
                header.m_Hash = dataReader.ReadInt32();

                if (!header.m_CPU.SequenceEqual(header.m_HvCPU)) {
                    MySQL.BanClient(Utils.BytesToString(header.m_HvCPU), "CPU spoofed");
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent a mismatching cpu set (spoofed)", ip);
                    Log.Print(logId);
                    return;
                }

                int[] validTitles = new int[] { 0x415608CB, 0x41560855, 0x41560817, 0x4156081C, 0x415607E6, 0x415608C3, 0x4156091D, 0x415608FC, 0x41560914 };

                if (!validTitles.Contains(header.m_Title)) {
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent a title that wasn't valid", ip);
                    Log.Print(logId);
                    return;
                }

                if (header.m_Packet == Packets.PACKET_WELCOME) {
                    // Register clients ip as an end point to allow connection to below functions
                    PacketWelcome.ServerPacketWelcome(serverWriter, header, logId, ip);
                } else {
                    if (MySQL.DoesRequestTokenExist(Utils.BytesToString(header.m_token), Utils.BytesToString(header.m_CPU))) {
                        MySQL.IncrementRequestTokenConnectionCount(Utils.BytesToString(header.m_token));
                        switch (header.m_Packet) {
                            case Packets.PACKET_HEARTBEAT:
                                // Heartbeat to make sure client is still connected
                                PacketHeartbeat.ServerPacketHeartbeat(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_STATUS:
                                // Get status (mainly ban check)
                                PacketStatus.ServerPacketStatus(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_LEAVE:
                                // Unregister client
                                PacketLeave.ServerPacketLeave(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_VERSION:
                                // Check the cheat xex version with the servers (sanity only)
                                PacketVersion.ServerPacketVersion(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_GETDATA:
                                // Get addresses
                                PacketGetData.ServerPacketGetData(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_GETFUNC:
                                // Get functions
                                PacketGetFunc.ServerPacketGetFunc(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_GETWELCOMEMSG:
                                // Get welcome message (only shows the first time they use the CE)
                                PacketWelcomeMessage.ServerPacketWelcomeMessage(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_GETCHANGELOG:
                                // Get latest changelog
                                PacketChangelog.ServerPacketChangelog(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_GETNATIVES:
                                // Get native hashes for native registration
                                PacketGetNatives.ServerPacketGetNatives(dataReader, serverWriter, header, logId, ip);
                                break;
                            case Packets.PACKET_PRESENCE:
                                // Check security and update heartbeat
                                PacketPresence.ServerPacketPresence(dataReader, serverWriter, header, logId, ip);
                                break;
                        }
                    } else {
                        Log.Add(logId, ConsoleColor.DarkYellow, "Flag", string.Format("Client token doesn't exist - {0}", Utils.BytesToString(header.m_token)), ip);
                        Log.Print(logId);
                    }
                }

            } catch (Exception ex) {

            }

            tcpClient.Close();
        }
    }
}

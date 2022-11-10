using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Security;
using System.Collections.Generic;

namespace LE {
    class ClientHandler {
        public static void FireWallBanEvent(string ip, string Reason, bool flag = true) {
            if (flag) {
                string Status = Utilities.WindowscmdExec(string.Format("netsh advfirewall firewall add rule name=\"" +
               "LEmulation_Reason:{1}_InstancePort:{2} @{0}\" " +
               "dir=in interface=any action=block remoteip={0}",
              ip, Reason, LEServer.Port));

                List<Log.PrintQueue> BanEventId = Log.GetQueue();
                Log.Add(BanEventId, ConsoleColor.DarkRed, "FIREWALL EVENT RECORDED", null);
                Log.Add(BanEventId, ConsoleColor.Red, "Reason:", Reason);
                Log.Add(BanEventId, ConsoleColor.Red, "Status:", Status);
                Log.Print(BanEventId);

            }
            else {
                string Status = Utilities.WindowscmdExec(string.Format("netsh advfirewall firewall delete rule name=\"" +
               "LEmulation_Reason:{1}_InstancePort:{2} @{0}\"", ip, Reason, LEServer.Port));

                List<Log.PrintQueue> ReleaseEventId = Log.GetQueue();
                Log.Add(ReleaseEventId, ConsoleColor.DarkGreen, "FIREWALL EVENT RELEASED", null);
                Log.Add(ReleaseEventId, ConsoleColor.Green, "Reason:", Reason);
                Log.Add(ReleaseEventId, ConsoleColor.Green, "Status:", Status);
                Log.Print(ReleaseEventId);
            }

        }

        public static int MaxSendRec = 2048;
        private Thread thread_listen;
        private bool ServerRunning;
        private TcpListener listener = null;

        public ClientHandler() {
            listener = new TcpListener(IPAddress.Any, LEServer.Port);
        }

        public struct ioData {
            public EndianReader reader;
            public EndianWriter writer;
            public TcpClient client;
            public IPEndPoint ipaddr;
            public int payloadSize;
        }

        public void DisconnectClient(ioData io) {
            io.reader.Close();
            io.writer.Close();
            io.client.Close();
        }

        private void ListenForClients() {
            listener.Start();

            while (true) {
                if (!ServerRunning)
                    return;
                if (listener.Pending()) new Thread(new ParameterizedThreadStart(HandleClientComm)).Start(listener.AcceptTcpClient());
                Thread.Sleep(100);
            }
        }

        public void Start() {
            thread_listen = new Thread(new ThreadStart(ListenForClients));
            ServerRunning = true;
            thread_listen.Start();
        }

        private void HandleClientComm(object Client) {
            CLIENT_STRUCT ClientObj = new CLIENT_STRUCT();
            TOKEN_STRUCT TokenObj = new TOKEN_STRUCT();

            TcpClient TcpClient = (TcpClient)Client;
            TcpClient.SendBufferSize = MaxSendRec;
            TcpClient.ReceiveBufferSize = MaxSendRec;
            TcpClient.NoDelay = true;

            IPEndPoint IP_EndPoint = TcpClient.Client.RemoteEndPoint as IPEndPoint;
            string IP = TcpClient.Client.RemoteEndPoint.ToString().Split(new char[] { ':' })[0];

            NetworkStream NetStream = TcpClient.GetStream();
            serverStream ServerStream = new serverStream(NetStream);

            try {
                // Console.WriteLine(IP + " Connected!");

                byte[] TmpDataHeader = new byte[0x8];
                if (!NetStream.CanRead || NetStream.Read(TmpDataHeader, 0, 0x8) != 0x8) {
                    TcpClient.Close();
                    return;
                }

                EndianIO DataHeader = new EndianIO(TmpDataHeader, EndianStyle.BigEndian);
                PACKET_COMMAND CommandID = (PACKET_COMMAND)DataHeader.Reader.ReadUInt32();

                int DataCount = DataHeader.Reader.ReadInt32();
                if (DataCount >= 0x8000) {
                    TcpClient.Close();
                    return;
                }

                byte[] Data = new byte[DataCount];
                if (!ServerStream.CanRead || ServerStream.Read(Data, 0, DataCount) != DataCount) {
                    TcpClient.Close();
                    return;
                }

                EndianIO EndianData = new EndianIO(Data, EndianStyle.BigEndian) {
                    Writer = new EndianWriter(ServerStream, EndianStyle.BigEndian)
                };

                ioData ioData;
                ioData.reader = EndianData.Reader;
                ioData.writer = new EndianIO(ServerStream, EndianStyle.BigEndian).Writer;
                ioData.client = TcpClient;
                ioData.ipaddr = IP_EndPoint;
                ioData.payloadSize = DataCount;

                if (ioData.payloadSize > 0) {
                    if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_AUTH) {
                        int Start = DateTime.Now.Second;
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Auth.ProcessAuthorization(ioData, ref ClientObj);
                        int End = (DateTime.Now.Second - Start);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Completed in: {2} seconds", CommandID, ioData.ipaddr.ToString(), End);
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_TIME) {
                        int Start = DateTime.Now.Second;
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.TimeUpdate.ProcessTime(ioData, ref ClientObj);
                        int End = (DateTime.Now.Second - Start);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Completed in {2} seconds", CommandID, ioData.ipaddr.ToString(), End);
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_STATUS) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Status.ProcessStatus(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_PRES) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Presence.ProcessPresence(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_SECURITY) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Security.ProcessSecurity(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_TOKEN_CHECK) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Token.ProcessVerifyTkn(ioData, ref ClientObj, ref TokenObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_TOKEN_REDEEM) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Token.ProcessRedeemTkn(ioData, ref ClientObj, ref TokenObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_XKE) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.XKE.ProcessXKE(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_XSC) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.SuperVisor.ProcessXSC(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_OFFSETS) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Games.ProccessOffsets(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_CPI) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.CPI.ProcessCPI(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else if (CommandID == PACKET_COMMAND.PACKET_COMMAND_ID_SETTINGS) {
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0}", CommandID, ioData.ipaddr.ToString());
                        Responces.Settings.ProccessSettings(ioData, ref ClientObj);
                        if (LEServer.DebugModePrints)
                            Console.WriteLine("@{1} - Command Id: {0} Passed!", CommandID, ioData.ipaddr.ToString());
                    }
                    else
                        FireWallBanEvent(IP, "Requested Invalid Response");
                }
                else
                    FireWallBanEvent(IP, "Invalid Payload Size");

                DisconnectClient(ioData);
            }
            catch (Exception Ex) {
                List<Log.PrintQueue> HandleServCommErrorid = Log.GetQueue();
                Log.Add(HandleServCommErrorid, ConsoleColor.Red, "ERROR", null);
                Log.Add(HandleServCommErrorid, ConsoleColor.Red, "Exception Message:", Ex.Message);
                Log.Add(HandleServCommErrorid, ConsoleColor.Red, "StrackTrace:", Ex.StackTrace);
                Log.Print(HandleServCommErrorid);

                TcpClient.Close();
                return;
            }
            if (TcpClient.Connected)
                TcpClient.Close();
            return;
        }
    }
}


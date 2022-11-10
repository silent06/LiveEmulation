using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class FirewallBanHandler {
        public void Start() {
            new Thread(new ThreadStart(Handler)).Start();
        }

        private static void Handler() {
            while (true) {
                foreach (var con in ClientHandler.m_Connections) {
                    if (con.Value.m_Banned) {
                        if ((Utils.GetTimeStamp() - con.Value.m_BannedTimestamp) > 3600) {
                            // if it's been more than an hour since ban

                            // get the current socket spam struct from the connections dict, and update the ban details
                            SocketSpam spam = ClientHandler.m_Connections[con.Key];
                            spam.m_Banned = false;
                            spam.m_BannedTimestamp = 0;

                            // Update the connection handler with the unbanned socket, and unban the ip from the firewall.
                            ClientHandler.m_Connections[con.Key] = spam;
                            Utils.UnbanFromFirewall(con.Key);
                        }
                    }
                }

                Thread.Sleep(10000);
            }
        }
    }
}
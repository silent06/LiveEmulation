using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace LiveEmulationCheatListener {
    class ConnectionLogHandler {
        public void Start() {
            new Thread(new ThreadStart(Handler)).Start();
        }

        private static void Handler() {
            while (true) {
                Thread.Sleep(10000);

                List<string> indexToRemove = new List<string>();

                foreach (var v in ClientHandler.m_Connections) {
                    if ((Utils.GetTimeStamp() - v.Value.m_InitialTimestamp) > 120) {
                        if (!v.Value.m_Banned) {
                            indexToRemove.Add(v.Key);
                        }
                    }
                }

                while (Global.m_IsUsingSpamDetection) Thread.Sleep(100);

                foreach (var v in indexToRemove) {
                    ClientHandler.m_Connections.Remove(v);
                }
            }
        }
    }
}

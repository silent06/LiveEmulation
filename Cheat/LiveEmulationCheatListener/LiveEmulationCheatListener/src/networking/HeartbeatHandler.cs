using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace LiveEmulationCheatListener {
    class HeartbeatHandler {
        public void Start() {
            new Thread(new ThreadStart(Handler)).Start();
        }

        private static void Handler() {
            while (true) {
                Dictionary<string, string> tokensToRemove = new Dictionary<string, string>();

                List<ClientEndPoint> endPoints = MySQL.GetAllClientEndPoints();
                Global.m_ConnectedClients = endPoints.Count;

                foreach (var ep in endPoints) {
                    long timestamp = Utils.GetTimeStamp();

                    // check that they've sent a presense
                    if (!ep.m_HasReceivedPresence) {
                        if((timestamp - ep.m_WelcomeTime) > 60) {
                            // the client sent a request to init the token, but hasn't sent a request in 60 seconds
                            // with the same token. There's a chance that the client failed one of the request, so
                            // let's just remove the token.
                            tokensToRemove.Add(ep.m_Token, "Hasn't sent initial presence in over 60 seconds");
                            continue;
                        }
                    }

                    if ((timestamp - ep.m_LastHeartbeat) > 60) {
                        // the client hasn't sent us a presence request in 60 seconds or more. This will only happen
                        // if the client has disconnected, or prevented the presence from updating. Now, because we
                        // don't know which one it is, let's just remove the token.
                        tokensToRemove.Add(ep.m_Token, "Hasn't sent presence in over 60 seconds");
                        continue;
                    }

                    if (((timestamp - ep.m_LastHeartbeat) > 10 && (timestamp - ep.m_LastHeartbeat) < 60) && ep.m_MainConnectionIndex < 7) {
                        // if it's been at least 10 seconds, but not as many as 60, and the client hasn't sent the minimum
                        // amount of connections, then we'll flag the user and delete their token. This will usually only
                        // happen for a few reasons, and because if one of the requests fail it'll shut down, it's highly
                        // unlikely that this would occur on legit grounds. Usually, this would mean the client has tried 
                        // to patch some requests, but failed to patch the presence.
                        tokensToRemove.Add(ep.m_Token, "Connection index is below 8");
                        continue;
                    }
                }

                foreach(var token in tokensToRemove) {
                    Console.WriteLine("Deleting token: {0} - {1}", token.Key, token.Value);
                    MySQL.RemoveRequestToken(token.Key);
                }

                tokensToRemove.Clear();
                endPoints.Clear();

                Thread.Sleep(5000);
            }
        }
    }
}

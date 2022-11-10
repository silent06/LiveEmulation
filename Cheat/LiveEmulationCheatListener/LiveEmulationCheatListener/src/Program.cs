using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace LiveEmulationCheatListener {
    class Program {
        static void Main(string[] args) {
            Console.ForegroundColor = ConsoleColor.White;
            Console.WindowWidth = 150;
            Console.WindowHeight = 31;
            Console.Title = "LiveEmulation Cheat Listener";

            try {
                new ConnectionLogHandler().Start();
                new FirewallBanHandler().Start();
                new ClientHandler().Start();
                new HeartbeatHandler().Start();

                new Thread(() => {
                    while(true) {
                        Console.Title = string.Format("LiveEmulation Cheat Listener | Online Clients: {0}", Global.m_ConnectedClients);
                        Thread.Sleep(5000);
                    }
                }).Start();
            } catch(Exception exception) {
                Console.WriteLine("Exception: " + exception.Message);
            }
        }
    }
}

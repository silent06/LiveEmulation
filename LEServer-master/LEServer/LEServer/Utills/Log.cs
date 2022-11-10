using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace LE {
    class Log {

        public struct PrintQueue {
            //public int m_id;
            public ConsoleColor m_color;
            public string m_message;
            public string m_spec;
        }

        private static bool m_busy;

        public static List<PrintQueue> GetQueue() {
            return new List<PrintQueue>();
        }

        public static void ErrorReportingPrint(string data) {
            Console.WriteLine(data);
            Utilities.WriteLog(data, true);
        }

        public static void Add(List<PrintQueue> queue, ConsoleColor color, string spec, string message) {
            PrintQueue data = new PrintQueue();
            data.m_color = color;
            data.m_message = message;
            data.m_spec = spec;
            queue.Add(data);
        }

        public static void Print(List<PrintQueue> queue) {
            new Thread(() => {
                while (!Print2(queue)) {
                    Thread.Sleep(1000);
                }
            }).Start();
        }

        public static bool Print2(List<PrintQueue> queue) {
            if (m_busy) {
                return false;
            } else {
                m_busy = true;
                string time = "[" + DateTime.Now.ToString("dd-MM-yyyy HH:mm:ss") + "] ";

                int count = 0;
                foreach (var e in queue) {
                    int c = time.Length + e.m_spec.Length;
                    if (c > count) {
                        count = c;
                    }
                }

                foreach (var e in queue) {
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.Write(time);

                    Console.ForegroundColor = ConsoleColor.White;
                    Console.Write(e.m_spec + " ");

                    for (int i = 0; i < count - e.m_spec.Length - time.Length; i++) Console.Write(" ");

                    Console.ForegroundColor = e.m_color;
                    Console.Write(e.m_message + Environment.NewLine);

                    Utilities.WriteLog(e.m_spec + " " + e.m_message, true);
                }

                Console.ForegroundColor = ConsoleColor.White;
                Console.WriteLine("----------------------------------------------------------------------------------------------------------");

                Utilities.WriteLog(Environment.NewLine + "----------------------------------------------------------------------------------------------------------", true);

                m_busy = false;
                return true;
            }
        }
    }
}

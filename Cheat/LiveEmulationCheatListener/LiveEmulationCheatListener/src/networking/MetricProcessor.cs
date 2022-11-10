using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class MetricProcessor {
        public static void ProcessMetrics(string cpu) {
            List<ClientMetric> metrics = MySQL.GetClientMetrics(cpu);
            if (metrics.Count > 0) {
                int warningCount = 0;
                foreach (var metric in metrics) {
                    if (metric.m_Type == 3) {
                        MySQL.BanClient(cpu, "Receiving a bannable metric");
                        Console.WriteLine("Banned {0} for receiving a bannable metric", cpu);
                    }

                    if (metric.m_Type == 2) {
                        warningCount++;
                    }
                }

                if (warningCount >= 5) {
                    MySQL.BanClient(cpu, "Receiving 5 or more warning metrics");
                    Console.WriteLine("Banned {0} for receiving 5 warning metrics", cpu);
                }
            }
        }
    }
}

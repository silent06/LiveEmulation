using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using MySql.Data.MySqlClient;

namespace LiveEmulationCheatListener {
    class MySQL {
        public static MySqlConnection Setup() {
            return new MySqlConnection(String.Format("Server={0};Database={1};Uid={2};Pwd={3};", "localhost", "le", "root", "mysql"));
        }

        public static bool Connect(MySqlConnection connection) {
            try {
                connection.Open();
                return true;
            } catch (MySqlException exception) {
                Console.WriteLine(exception.Message);
                return false;
            }
        }

        public static void Disconnect(MySqlConnection connection) {
            try {
                connection.Close();
            } catch (MySqlException exception) {
                Console.WriteLine(exception.Message);
            }
        }

        public static bool IsClientBanned(string cpu, ref string reason) {
            ClientData data = new ClientData();
            if (GetClientData(cpu, ref data)) {
                return data.m_AuthStatus == 1;
            }
            return false;
        }

        public static bool GetClientData(string cpu, ref ClientData data) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM clients WHERE cpu = @key");
                    command.Parameters.AddWithValue("@key", cpu);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.Read()) {
                            data.m_Id = reader.GetInt32("id");
                            data.m_CPU = reader.GetString("cpu");
                            data.m_HasUsedBefore = reader.GetBoolean("has_used_before");
                            data.m_AuthStatus = reader.GetInt32("authstatus");
                            Disconnect(db);
                            return true;
                        }
                    }
                }
                Disconnect(db);
            }
            return false;
        }

        public static void RemoveRequestToken(string token) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("DELETE FROM cheat_endpoints WHERE `token` = @token");
                    command.Parameters.AddWithValue("@token", token);
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }

        public static bool DoesRequestTokenExist(string token, string cpu) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_endpoints WHERE `token` = @token AND `cpu` = @cpu");
                    command.Parameters.AddWithValue("@token", token);
                    command.Parameters.AddWithValue("@cpu", cpu);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.Read()) {
                            Disconnect(db);
                            return true;
                        }
                    }
                }
                Disconnect(db);
            }
            return false;
        }
        
        public static void AddRequestToken(string token, string cpu) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("INSERT INTO cheat_endpoints (`token`, `cpu`, last_heartbeat, welcome_time) VALUES (@token, @cpu, @last, @welcome)");
                    command.Parameters.AddWithValue("@token", token);
                    command.Parameters.AddWithValue("@cpu", cpu);
                    command.Parameters.AddWithValue("@last", Utils.GetTimeStamp());
                    command.Parameters.AddWithValue("@welcome", Utils.GetTimeStamp());
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }

        public static void AddMetric(string cpu, int index, int type) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("INSERT INTO cheat_metrics (`cpu`, ind, type) VALUES (@cpu, @ind, @type)");
                    command.Parameters.AddWithValue("@cpu", cpu);
                    command.Parameters.AddWithValue("@ind", index);
                    command.Parameters.AddWithValue("@type", type);
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }

        public static void UpdateRequestTokenHeartbeat(string token) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("UPDATE cheat_endpoints SET last_heartbeat = @last, has_received_presence = @has WHERE `token` = @token");
                    command.Parameters.AddWithValue("@last", Utils.GetTimeStamp());
                    command.Parameters.AddWithValue("@has", true);
                    command.Parameters.AddWithValue("@token", token);
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }

        public static void IncrementRequestTokenConnectionCount(string token) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("UPDATE cheat_endpoints SET main_connection_index=main_connection_index+1 WHERE `token` = @token");
                    command.Parameters.AddWithValue("@token", token);
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }

        public static List<ClientEndPoint> GetAllClientEndPoints() {
            List<ClientEndPoint> endPoints = new List<ClientEndPoint>();
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_endpoints");
                    using (var reader = command.ExecuteReader()) {
                        if (reader.HasRows) {
                            while (reader.Read()) {
                                ClientEndPoint ep = new ClientEndPoint();
                                ep.m_HasReceivedPresence = reader.GetBoolean("has_received_presence");
                                ep.m_Token = reader.GetString("token");
                                ep.m_LastHeartbeat = reader.GetInt64("last_heartbeat");
                                ep.m_WelcomeTime = reader.GetInt64("welcome_time");
                                ep.m_MainConnectionIndex = reader.GetInt32("main_connection_index");
                                endPoints.Add(ep);
                            }
                        }
                    }
                }
                Disconnect(db);
            }

            return endPoints;
        }

        public static Dictionary<int, string> GetAllFunctions(int title) {
            Dictionary<int, string> functions = new Dictionary<int, string>();
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_functions WHERE title = @title");
                    command.Parameters.AddWithValue("@title", title);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.HasRows) {
                            while (reader.Read()) {
                                functions.Add(reader.GetInt32("ind"), reader.GetString("bytes"));
                            }
                        }
                    }
                }
                Disconnect(db);
            }

            return functions;
        }

        public static Dictionary<int, ulong> GetAllAddresses(int title) {
            Dictionary<int, ulong> functions = new Dictionary<int, ulong>();
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_addreses WHERE title = @title");
                    command.Parameters.AddWithValue("@title", title);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.HasRows) {
                            while (reader.Read()) {
                                functions.Add(reader.GetInt32("ind"), ulong.Parse(reader.GetString("address").Substring(2), System.Globalization.NumberStyles.HexNumber));
                            }
                        }
                    }
                }
                Disconnect(db);
            }

            return functions;
        }

        public static Dictionary<int, ulong> GetAllNatives(int title) {
            Dictionary<int, ulong> functions = new Dictionary<int, ulong>();
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_natives WHERE title = @title");
                    command.Parameters.AddWithValue("@title", title);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.HasRows) {
                            while (reader.Read()) {
                                functions.Add(reader.GetInt32("ind"), ulong.Parse(reader.GetString("address").Substring(2), System.Globalization.NumberStyles.HexNumber));
                            }
                        }
                    }
                }
                Disconnect(db);
            }

            return functions;
        }

        public static void UpdateWelcomeMessageCheck(string cpu) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("UPDATE clients SET has_used_before = 1 WHERE cpu = @key");
                    command.Parameters.AddWithValue("@key", cpu);
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }

        public static int GetLatestCheatVersion(int title) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_data WHERE title = @key");
                    command.Parameters.AddWithValue("@key", title);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.Read()) {
                            int latest = reader.GetInt32("latest_version");
                            Disconnect(db);
                            return latest;
                        }
                    }
                }
                Disconnect(db);
            }
            return 0;
        }

        public static List<ClientMetric> GetClientMetrics(string cpu) {
            List<ClientMetric> list = new List<ClientMetric>();
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("SELECT * FROM cheat_metrics WHERE cpu = @cpu");
                    command.Parameters.AddWithValue("@cpu", cpu);
                    using (var reader = command.ExecuteReader()) {
                        if (reader.HasRows) {
                            while (reader.Read()) {
                                list.Add(new ClientMetric(reader.GetInt32("ind"), reader.GetInt32("type")));
                            }
                        }
                    }
                }
                Disconnect(db);
            }

            return list;
        }

        public static void BanClient(string cpu, string reason) {
            using (var db = Setup()) {
                Connect(db);
                using (var command = db.CreateCommand()) {
                    command.CommandText = string.Format("UPDATE clients SET authstatus = @status, bannedreason = @reason WHERE `cpu` = @cpu");
                    command.Parameters.AddWithValue("@authstatus", 1);
                    command.Parameters.AddWithValue("@bannedreason", reason);
                    command.Parameters.AddWithValue("@cpu", cpu);
                    command.ExecuteNonQuery();
                }
                Disconnect(db);
            }
        }
    }
}

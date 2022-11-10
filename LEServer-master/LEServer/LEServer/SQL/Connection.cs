using System;
using MySql.Data.MySqlClient;
using System.Threading;
using System.Collections.Generic;

namespace LE {
    class DbConnection {
        public static MySqlConnection SetupConnection() {
            string Host = LEServer.host;
            string Database = LEServer.database;
            string Username = LEServer.username;
            string Pass = LEServer.password;
            string Port = "3306";
            return new MySqlConnection($"Server={Host};Port={Port};Database={Database};Uid={Username};Pwd={Pass};");
        }

        public static void Connect(MySqlConnection Con) {
            try {
                Con.Open();
            } catch (MySqlException Ex) {
                List<Log.PrintQueue> MySqlOpenConId = Log.GetQueue();
                Log.Add(MySqlOpenConId, ConsoleColor.Red, "MySql Con Error", null);
                Log.Add(MySqlOpenConId, ConsoleColor.Red, "Exception Message:", Ex.Message);
                Log.Add(MySqlOpenConId, ConsoleColor.Red, "StrackTrace:", Ex.StackTrace);
                Log.Print(MySqlOpenConId);
                Thread.Sleep(7000);
                Utilities.RestartServer();
            }
        }

    }
}

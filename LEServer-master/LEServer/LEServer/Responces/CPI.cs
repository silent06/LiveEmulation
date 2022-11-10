using Newtonsoft.Json;
using Security;
using System;
using System.Collections.Generic;
using System.Text;

namespace LE.Responces {
    class CPI {

        public static void ProcessCPI(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));

            byte[] DiscordToken = new byte[0xC];
            byte[] CPIBuffer = new byte[DiscordToken.Length + 0x4]; // Discord token + size of int unsigned preferble
            EndianWriter Data = new EndianIO(CPIBuffer, EndianStyle.BigEndian).Writer;

            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                int DiscordPopup = 0;

                if (ClientObj.discord != null) {
                    Discord discord = JsonConvert.DeserializeObject<Discord>(ClientObj.discord);
                    if (discord.id != "0" && discord.primary && !discord.verified && discord.token != null) {
                        Buffer.BlockCopy(Encoding.ASCII.GetBytes(discord.token), 0, DiscordToken, 0, discord.token.Length);
                        DiscordPopup = Convert.ToInt32(discord.popup);
                        discord.popup = false;

                        ClientObj.discord = JsonConvert.SerializeObject(discord);
                    }
                }
                Data.Write(DiscordToken);
                Data.Write(DiscordPopup);
                io.writer.Write(CPIBuffer);
            } else
                Console.WriteLine("CPI Failed for:" + ClientObj.cpukey);
        }
    }
}

using Security;
using System;
using LE;
using System.Text;

namespace LE.Responces {
    class TimeUpdate {

        public static void ProcessTime(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            byte[] TimeBuffer = new byte[0x34];
            EndianWriter Data = new EndianIO(TimeBuffer, EndianStyle.BigEndian).Writer;

            if (ClientFound) {
                bool Expired = (ClientObj.time <= DateTime.Now ? true : false);
                TimeSpan Time;
                
                if (Expired) Time = new TimeSpan(0, 0, 0);
                else Time = ClientObj.time - DateTime.Now;

                int Days = (Expired) ? 0 : Time.Days;
                int Years = (Expired) ? 0 : Days / 365;
                Days = (Expired) ? 0 : Days % 365;
                int Months = (Expired) ? 0 : Days / 30;
                int r_days = (Expired) ? 0 : Days % 30;

                TimeSpan UnbanTime;
                if (ClientObj.kvstatus == CLIENT_KVSTATUS.UNBANNED && ClientObj.kvfirstunbanned.Year != 2009)
                    UnbanTime = (DateTime.Now - ClientObj.kvfirstunbanned);
                else UnbanTime = new TimeSpan(0, 0, 0);

                int UnBanDays = UnbanTime.Days;
                int UnBanYears = UnBanDays / 365;
                Days = UnBanDays % 365;
                int UnBanMonths = UnBanDays / 30;
                int UnBanr_days = UnBanDays % 30;
                
                Data.Write((int)PACKET_STATUS.SUCCESS);
                Data.Write(Years);
                Data.Write(Months);
                Data.Write(r_days);
                Data.Write(Time.Hours);
                Data.Write(Time.Minutes);
                
                Data.Write(0);
                Data.Write(0);
                Data.Write(UnbanTime.Days);
                Data.Write(UnbanTime.Hours);
                Data.Write(UnbanTime.Minutes);
                
                if (ClientObj.authstatus != CLIENT_AUTHSTATUS.BANNED)
                Data.Write((LEServer.Freemode) ? (int)CLIENT_AUTHSTATUS.FREEMODE : (int)ClientObj.authstatus);
            else
                Data.Write((int)ClientObj.authstatus);
                Data.Write(MySql.FetchKvUsedOn(ClientObj));
            } else {
                Console.WriteLine("client not found @{0} with SessionToken: {1}", ClientObj.cpukey, ClientObj.sessiontoken);
                io.writer.Write((int)PACKET_STATUS.ERROR);
            }
            io.writer.Write(TimeBuffer);

            byte[] KvFirstUnbannedBuffer = new byte[ClientObj.kvfirstunbanned.ToString().Length];
            string UnbanTimeDateTime = ClientObj.kvfirstunbanned.ToString();
            Buffer.BlockCopy(Encoding.ASCII.GetBytes(UnbanTimeDateTime), 0, KvFirstUnbannedBuffer, 0, UnbanTimeDateTime.Length);

           io.writer.Write(ClientObj.kvfirstunbanned.ToString().Length);
           io.writer.Write(KvFirstUnbannedBuffer);
        }
    }
}

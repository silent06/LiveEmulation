using Security;
using System;
using System.Text;
using Newtonsoft.Json;

namespace LE.Responces {
    class Presence {
        public static void ProcessPresence(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string TitleID = io.reader.ReadUInt32().ToString("X");
            byte[] GamerTag = io.reader.ReadBytes(0x10);
            string ConsoleKvStatus = io.reader.ReadUInt32().ToString("X");

            byte[] PresBuffer = new byte[0x8]; //0x8
            EndianWriter Data = new EndianIO(PresBuffer, EndianStyle.BigEndian).Writer;

            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                ClientObj.titleid = Utilities.TitleID(TitleID);
                ClientObj.ip = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];
                ClientObj.lastloginTime = DateTime.Now;
                ClientObj.gamertag = Utilities.Validategamertag(GamerTag);
                
                Data.Write((int)PACKET_STATUS.SUCCESS);

                if (ClientObj.consoleaction != CLIENT_ACTION.DEFAULT && ClientObj.actioncompleted == CLIENT_ACTION_COMPLETED.AWAITING) {
                    if (ClientObj.consoleaction == CLIENT_ACTION.DEFAULT)
                        Data.Write((int)CLIENT_ACTION.DEFAULT);
                    else if (ClientObj.consoleaction == CLIENT_ACTION.REBOOT)
                        Data.Write((int)CLIENT_ACTION.REBOOT);
                    else if (ClientObj.consoleaction == CLIENT_ACTION.RROD)
                        Data.Write((int)CLIENT_ACTION.RROD);
                    else if (ClientObj.consoleaction == CLIENT_ACTION.SENDTODASH)
                        Data.Write((int)CLIENT_ACTION.SENDTODASH);
                } else {
                    Data.Write((int)CLIENT_ACTION.DEFAULT);
                }
            } else {
                Data.Write((int)PACKET_STATUS.ERROR);
                Data.Write((int)CLIENT_ACTION.DEFAULT);
            }
            io.writer.Write(PresBuffer);

            Utilities.Update_LiveStatus(ConsoleKvStatus, ref ClientObj);
            MySql.SaveClient(ClientObj, SessionToken);
            MySql.UpdateKvThread(ClientObj);
        }
    }
}
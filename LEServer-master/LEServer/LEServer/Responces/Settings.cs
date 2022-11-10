using LE;
using Newtonsoft.Json;
using Security;
using System;
using System.Collections.Generic;
using System.Text;

namespace LE.Responces {
    class Settings {
        public static void ProccessSettings(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));

            byte[] Resp = new byte[0x8];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;

            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                if (ClientObj.settings != null) {
                    ClientSettings Settings = JsonConvert.DeserializeObject<ClientSettings>(ClientObj.settings);
                    ClientObj.settings = JsonConvert.SerializeObject(Settings);
                } else {
                    ClientSettings Settings = JsonConvert.DeserializeObject<ClientSettings>(ClientObj.settings);
                    Settings.Enable_BlockXblDns = false;
                    Settings.Enable_DrawGuideInformation = true;
                    Settings.Enable_CustomNotification = true;
                    Settings.Enable_LEImage = true;
                    Settings.Enable_Gradient = true;
                    Settings.Enable_GradientBG = true;
                    Settings.Enable_GradientBtns = true;
                    Settings.Enable_ColorFade = true;
                    Settings.Enable_MachineIDSpoof = true;
                    Settings.Enable_AW_Bypass = true;
                    Settings.Enable_BO2_Bypass = true;
                    Settings.Enable_BO3_Bypass = true;
                    ClientObj.settings = JsonConvert.SerializeObject(Settings);
                    MySql.SaveClient(ClientObj, SessionToken);
                }
            }
            else
                io.writer.Write((int)PACKET_STATUS.ERROR);
        }
    }
}

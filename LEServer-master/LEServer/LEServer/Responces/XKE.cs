using Security;
using System;
using System.Collections.Generic;
using System.Net.Sockets;

namespace LE.Responces {
    class XKE {
          public static void ProcessXKE(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            byte[] KvCpukey = io.reader.ReadBytes(0x10);
            byte[] HvSalt = io.reader.ReadBytes(0x10);
            byte[] SMAC = io.reader.ReadBytes(0x6);
            string IP = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];
            byte[] TmpCrl = io.reader.ReadBytes(0x1);
            bool Crl = BitConverter.ToBoolean(TmpCrl, 0);
            byte[] ConsoleCertificate = io.reader.ReadBytes(0xB);
            byte[] KeyvaultSignature = io.reader.ReadBytes(0x100);
            byte[] Oddfeatures = io.reader.ReadBytes(0x2); 

            byte[] TmpFCRT = io.reader.ReadBytes(0x1);
            bool FCRT = BitConverter.ToBoolean(TmpFCRT, 0);

            byte[] ApiData = new byte[0x145];
            Buffer.BlockCopy(Utilities.StringToBytes(SessionToken), 0, ApiData, 0, 0x10);
            Buffer.BlockCopy(KvCpukey, 0, ApiData, 0x10, 0x10);
            Buffer.BlockCopy(HvSalt, 0, ApiData, 0x20, 0x10);
            Buffer.BlockCopy(SMAC, 0, ApiData, 0x30, 0x6);
            Buffer.BlockCopy(TmpCrl, 0, ApiData, 0x36, 0x1);
            Buffer.BlockCopy(ConsoleCertificate, 0, ApiData, 0x37, 0xB);
            Buffer.BlockCopy(KeyvaultSignature, 0, ApiData, 0x42, 0x100);
            Buffer.BlockCopy(Oddfeatures, 0, ApiData, 0x142, 0x2);
            Buffer.BlockCopy(TmpFCRT, 0, ApiData, 0x144, 0x1);


            /*Buffer.BlockCopy(LEServer.ApiKey, 0, ApiData, 0, 0x90);
            Buffer.BlockCopy(Utilities.StringToBytes(SessionToken), 0, ApiData, 0x90, 0x10);
            Buffer.BlockCopy(KvCpukey, 0, ApiData, 0xA0, 0x10);
            Buffer.BlockCopy(HvSalt, 0, ApiData, 0xB0, 0x10);
            Buffer.BlockCopy(SMAC, 0, ApiData, 0xC0, 0x6);
            Buffer.BlockCopy(TmpCrl, 0, ApiData, 0xC6, 0x1);
            Buffer.BlockCopy(ConsoleCertificate, 0, ApiData, 0xC7, 0xB);
            Buffer.BlockCopy(KeyvaultSignature, 0, ApiData, 0xD2, 0x100);
            Buffer.BlockCopy(Oddfeatures, 0, ApiData, 0x1D2, 0x2);
            Buffer.BlockCopy(TmpFCRT, 0, ApiData, 0x1D4, 0x1);*/

            TcpClient Api = new TcpClient();
            Api.Connect("127.0.0.1", 3000);
            NetworkStream Stream = Api.GetStream();
            if (Api.Connected)
                Stream.Write(ApiData, 0, ApiData.Length);
            else 
                throw new Exception("Could not make a connection to the APIEndpoint Method[XKE]");

            byte[] XkeBuffer = new byte[0x100];
            if (Stream.CanRead) {
                Stream.Read(XkeBuffer, 0, 0x100);
                Stream.Close();
                Api.Close();
            }
            else
                throw new Exception("Stream Reader flag cannot read data, Method[XKE]");

            //Crypto.RC4(ref XkeBuffer, Utilities.StringToBytes(SessionToken));
             if (XkeBuffer[0x28] != 0x4E)
                Log.ErrorReportingPrint($"XKE decryption or null buffer failure for cpukey: {ClientObj.cpukey}");

            byte[] Resp = new byte[0x100 + sizeof(int)];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;
            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (XkeBuffer != null && ClientFound && ClientObj.authstatus != CLIENT_AUTHSTATUS.BANNED && (int)ClientObj.authstatus >= 3 || LEServer.Freemode) {
               
                Data.Write((int)PACKET_STATUS.SUCCESS);
                Data.Write(XkeBuffer);
                io.writer.Write(Resp);
                
                List<Log.PrintQueue> XkeId = Log.GetQueue();
                Log.Add(XkeId, ConsoleColor.DarkBlue, "|XKE|", null);
                Log.Add(XkeId, ConsoleColor.Blue, "CPUKey:", $"{ClientObj.cpukey}");
                Log.Add(XkeId, ConsoleColor.Blue, "HvSalt:", $"{Utilities.BytesToString(HvSalt)}");
                Log.Add(XkeId, ConsoleColor.Blue, "Challenges Ran:", $"{ClientObj.challengesran}");
                Log.Print(XkeId);

                ClientObj.challengesran += 1;
                MySql.SaveClient(ClientObj, SessionToken);
                MySql.IncrementChallengeRuns();
            } else {
                io.writer.Write((int)PACKET_STATUS.ERROR);
            }
        }
    }
}
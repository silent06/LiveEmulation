using Security;
using System;
using LE;
using System.Collections.Generic;
using System.Net.Sockets;

namespace LE.Responces {
    class SuperVisor {
        
        public static void ProcessXSC(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            byte[] KvCpukey = io.reader.ReadBytes(0x10);
            byte[] Title = io.reader.ReadBytes(4);
            byte[] SMAC = io.reader.ReadBytes(0x6);
            string IP = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];
            byte[] KeyvaultSignature = io.reader.ReadBytes(0x100);
            byte[] DrivePhaseLevel = io.reader.ReadBytes(0x4);
            byte[] DriveOsig = io.reader.ReadBytes(0x24);
            byte[] KvSerial = io.reader.ReadBytes(0xC);
            byte[] XamRegion = io.reader.ReadBytes(0x2);
            byte[] XamOdd = io.reader.ReadBytes(0x2);
            byte[] Policyflashsize = io.reader.ReadBytes(0x4);
            byte[] Oddfeatures = io.reader.ReadBytes(0x2);
            byte[] TmpFCRT = io.reader.ReadBytes(0x1);
            bool FCRT = BitConverter.ToBoolean(TmpFCRT, 0);
            byte[] ConsoleID = io.reader.ReadBytes(0x5);
            byte[] ConsoleCertificate = io.reader.ReadBytes(0xB);
            byte[] KvHMACDigest = io.reader.ReadBytes(0x10);




            byte[] ApiData = new byte[0x187];

            Buffer.BlockCopy(Utilities.StringToBytes(SessionToken), 0, ApiData, 0, 0x10);
            Buffer.BlockCopy(Title, 0, ApiData, 0x10, 0x4);
            Buffer.BlockCopy(KeyvaultSignature, 0, ApiData, 0x14, 0x100);
            Buffer.BlockCopy(DrivePhaseLevel, 0, ApiData, 0x114, 0x4);
            Buffer.BlockCopy(DriveOsig, 0, ApiData, 0x118, 0x24);
            Buffer.BlockCopy(KvSerial, 0, ApiData, 0x13C, 0xC);
            Buffer.BlockCopy(XamRegion, 0, ApiData, 0x148, 0x2);
            Buffer.BlockCopy(Policyflashsize, 0, ApiData, 0x14A, 0x4);
            Buffer.BlockCopy(XamOdd, 0, ApiData, 0x14E, 0x2);
            Buffer.BlockCopy(TmpFCRT, 0, ApiData, 0x150, 0x1);
            Buffer.BlockCopy(ConsoleID, 0, ApiData, 0x151, 0x5);
            Buffer.BlockCopy(ConsoleCertificate, 0, ApiData, 0x156, 0xB);
            Buffer.BlockCopy(KvHMACDigest, 0, ApiData, 0x161, 0x10);
            Buffer.BlockCopy(KvCpukey, 0, ApiData, 0x171, 0x10);
            Buffer.BlockCopy(SMAC, 0, ApiData, 0x181, 0x6);



            //byte[] ApiData = new byte[0x217];
            //SessionToken
            //Buffer.BlockCopy(Utilities.StringToBytes(SessionToken), 0, ApiData, 0, 0x10);
            //KeyvaultSignature
            //Buffer.BlockCopy(KeyvaultSignature, 0, ApiData, 0x10, 0x100);
            //DrivePhaseLevel
            //Buffer.BlockCopy(DrivePhaseLevel, 0, ApiData, 0x110, 0x4);
            //DriveOsig
            //Buffer.BlockCopy(DriveOsig, 0, ApiData, 0x114, 0x24);
            //KvSerial
            //Buffer.BlockCopy(KvSerial, 0, ApiData, 0x138, 0xC);
            //XamRegion
            //Buffer.BlockCopy(XamRegion, 0, ApiData, 0x144, 0x2);
            //XamOdd
            //Buffer.BlockCopy(XamOdd, 0, ApiData, 0x146, 0x2);
            //Policyflashsize
            //Buffer.BlockCopy(Policyflashsize, 0, ApiData, 0x148, 0x4);
            //Oddfeatures
            //Buffer.BlockCopy(Oddfeatures, 0, ApiData, 0x14C, 0x2);
            //TmpFCRT
            //Buffer.BlockCopy(TmpFCRT, 0, ApiData, 0x150, 0x1);
            //ConsoleID
            //Buffer.BlockCopy(ConsoleID, 0, ApiData, 0x151, 0x5);
            //ConsoleCertificate
            //Buffer.BlockCopy(ConsoleCertificate, 0, ApiData, 0x156, 0xB);
            //KvHMACDigest
            //Buffer.BlockCopy(KvHMACDigest, 0, ApiData, 0x161, 0x10);
            //apikey
            //Buffer.BlockCopy(LEServer.ApiKey, 0, ApiData, 0x171, 0x90);
            //KvCpukey
            //Buffer.BlockCopy(KvCpukey, 0, ApiData, 0x201, 0x10);
            //SMAC
            //Buffer.BlockCopy(SMAC, 0, ApiData, 0x211, 0x6);

            TcpClient Client = new TcpClient();
            Client.Connect("127.0.0.1", 3001);
            NetworkStream Stream = Client.GetStream();
            if (Client.Connected)
                Stream.Write(ApiData, 0, ApiData.Length);
            else throw new Exception("Could not make a connection to the APIEndpoint Method[XSC]");

            byte[] XoscBuffer = new byte[0x2E0];
            if (Stream.CanRead) {
                Stream.Read(XoscBuffer, 0, 0x2E0);
                Stream.Close();
                Client.Close();
            }
            else 
                throw new Exception("Stream Reader flag cannot read data, Method[XSC]");

            //Crypto.RC4(ref XoscBuffer, Utilities.StringToBytes(SessionToken));

            byte[] Resp = new byte[0x2E0 + sizeof(int)];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;
            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound && (int)ClientObj.authstatus >= 3 || LEServer.Freemode && ClientObj.authstatus != CLIENT_AUTHSTATUS.BANNED) {
                Data.Write((int)PACKET_STATUS.SUCCESS);
                Data.Write(XoscBuffer);
                io.writer.Write(Resp);

                List<Log.PrintQueue> XscId = Log.GetQueue();
                Log.Add(XscId, ConsoleColor.DarkGreen, "|XSC|", null);
                Log.Add(XscId, ConsoleColor.Blue, "CPUKey:", $"{ClientObj.cpukey}");
                Log.Print(XscId);
                MySql.SaveClient(ClientObj, SessionToken);
            } else {
                io.writer.Write((int)PACKET_STATUS.ERROR);
            }
        }
   }
}

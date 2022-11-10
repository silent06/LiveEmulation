using System;
using System.Linq;
using static LEApi.Program;
using System.IO;

namespace LEApi
{
    class XOSCresponce
    {

        public enum CLIENT_KVTYPE
        {
            CORONA,
            XENON,
            ZEPHYR,
            FALCON,
            JASPER,
            TRINITY
        }

        public static CLIENT_KVTYPE GetMotherboardTypeFromKv(byte[] ConsoleCertificate, byte[] KeyvaultSignature)
        {

            int mobo = (byte)(((ConsoleCertificate[2] << 4) & 0xF0) | (ConsoleCertificate[3] & 0xF));
            if (mobo < 0x10)
            {
                if (mobo == 0 || !Type1KvStatus(KeyvaultSignature)) return CLIENT_KVTYPE.CORONA;
                return CLIENT_KVTYPE.XENON;
            }
            else if (mobo < 0x14) return CLIENT_KVTYPE.ZEPHYR;
            else if (mobo < 0x18) return CLIENT_KVTYPE.FALCON;
            else if (mobo < 0x52) return CLIENT_KVTYPE.JASPER;
            else if (mobo < 0x58) return CLIENT_KVTYPE.TRINITY;
            else return CLIENT_KVTYPE.CORONA;
        }

        public static bool Type1KvStatus(byte[] KeyvaultSignature)
        {
            bool type1KV = true;
            for (int i = 0; i < 0x100; ++i)
            {
                if (KeyvaultSignature[i] != 0)
                    type1KV = false;
            }
            return type1KV;
        }

        public static byte[] CreateCpukeyDigest(byte[] KvCpukey)
        {
            return Tools.SHA1(KvCpukey);
        }

        public static byte[] Calculate_ZeroXSeventy(CLIENT_KVTYPE Mobo)
        {
            byte[] XenonHash = { 0x02, 0x24, 0xEE, 0xA6, 0x1E, 0x89, 0x8B, 0xA1, 0x55, 0xB5, 0xAF, 0x74, 0xAA, 0x78, 0xAD, 0x0B };
            byte[] ZephyrHash = { 0x02, 0x24, 0xEE, 0xA6, 0x1E, 0x89, 0x8B, 0xA1, 0x55, 0xB5, 0xAF, 0x74, 0xAA, 0x78, 0xAD, 0x0B };
            byte[] FalconHash = { 0x4E, 0xEA, 0xA3, 0x32, 0x3D, 0x9F, 0x40, 0xAA, 0x90, 0xC0, 0x0E, 0xFC, 0x5A, 0xD5, 0xB0, 0x00 };
            byte[] JasperHash = { 0xFF, 0x23, 0x99, 0x90, 0xED, 0x61, 0xD1, 0x54, 0xB2, 0x31, 0x35, 0x99, 0x0D, 0x90, 0xBD, 0xBC };
            byte[] TrinityHash = { 0xDB, 0xE6, 0x35, 0x87, 0x78, 0xCB, 0xFC, 0x2F, 0x52, 0xA3, 0xBA, 0xF8, 0x92, 0x45, 0x8D, 0x65 };
            byte[] CoronaHash = { 0xD1, 0x32, 0xFB, 0x43, 0x9B, 0x48, 0x47, 0xE3, 0x9F, 0xE5, 0x46, 0x46, 0xF0, 0xA9, 0x9E, 0xB1 };

            if (Mobo == CLIENT_KVTYPE.CORONA)
                return CoronaHash;
            else if (Mobo == CLIENT_KVTYPE.XENON)
                return XenonHash;
            else if (Mobo == CLIENT_KVTYPE.ZEPHYR)
                return ZephyrHash;
            else if (Mobo == CLIENT_KVTYPE.FALCON)
                return FalconHash;
            else if (Mobo == CLIENT_KVTYPE.JASPER)
                return JasperHash;
            else if (Mobo == CLIENT_KVTYPE.TRINITY)
                return TrinityHash;
            return CoronaHash;
        }

        private static uint ComputeHvStatusFlag(byte[] Oddfeatures, bool FCRT, bool CRL)
        {
            uint HvStatusFlag = 0x23289D3;

            if (CRL)
                HvStatusFlag = HvStatusFlag | 0x10000;

            if (FCRT)
                HvStatusFlag = HvStatusFlag | 0x1000000;
            return HvStatusFlag;
        }

        public static long GetPCIEFlag(CLIENT_KVTYPE Mobo)// updated
        {

            long CoronaID = 0x4158019002000380;
            long TrinityID = 0x4158016002000380;
            long FalconID = 2158003302000083;

            if (Mobo == CLIENT_KVTYPE.TRINITY)
                return TrinityID;
            else if (Mobo == CLIENT_KVTYPE.FALCON)
                return FalconID;
            else if (Mobo == CLIENT_KVTYPE.CORONA)
                return CoronaID;

            return 0x0000000000000000;
        }

        public static uint HWID(CLIENT_KVTYPE Mobo)
        {
            if (Mobo == CLIENT_KVTYPE.CORONA)
                return 0x50000227;
            else if (Mobo == CLIENT_KVTYPE.XENON)
                return 0x00000227;
            else if (Mobo == CLIENT_KVTYPE.ZEPHYR)
                return 0x10000227;
            else if (Mobo == CLIENT_KVTYPE.FALCON)
                return 0x20000227;
            else if (Mobo == CLIENT_KVTYPE.JASPER)
                return 0x30000227;
            else if (Mobo == CLIENT_KVTYPE.TRINITY)
                return 0x40000227;
            return 0x50000227; // corona
        }

        public static byte[] BuildCleanXOSCResponce(byte[] KvCpukey, CLIENT_KVTYPE Mobo, XSCDATA_STRUCT XSCData)
        {
            byte[] XoscBuff = new byte[0x2E0];

            // xoscMajor 0x4 -> 0x6  & xoscMinor 0x6 -> 0x8
            Buffer.BlockCopy(Tools.FixEndian(0x090002), 0x1, XoscBuff, 0x5, 0x3);

            // 0x8 operations 0x1BD for type 1 and 0x1BF for type 2
            Buffer.BlockCopy(Tools.FixEndian(Type1KvStatus(XSCData.TypeKV) ? (long)0x00000000000001BD : (long)0x00000000000001BF), 0, XoscBuff, 0x8, 0x8);

            // 0x20 sataResult
            Buffer.BlockCopy(Tools.FixEndian(0xC8003003), 0, XoscBuff, 0x20, 0x4);

            // 0x24 padding unknown 
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x10), 0, XoscBuff, 0x24, 0x10);

            // 0x34 daeResult
            Buffer.BlockCopy(Tools.FixEndian(0x40000012), 0, XoscBuff, 0x34, 0x4);

            // 0x38 excution data kernel version is in here!!
            byte[] ExecutionID = null; byte[] TitleHeader = null;
            if (File.Exists(string.Concat(new object[] { "XSC/", Tools.BytesToHexString(XSCData.Title), ".bin" })))
            {
                ExecutionID = File.ReadAllBytes(string.Concat(new object[] { "XSC/", Tools.BytesToHexString(XSCData.Title), ".bin" })).Take(0x18).ToArray();
                TitleHeader = File.ReadAllBytes(string.Concat(new object[] { "XSC/", Tools.BytesToHexString(XSCData.Title), ".bin" })).Skip(0x18).Take(File.ReadAllBytes(string.Concat(new object[] { "XSC/", Tools.BytesToHexString(XSCData.Title), ".bin" })).Length - 0x18).ToArray();
            }
            else
            {
                ExecutionID = File.ReadAllBytes("XSC/FFFE07D1.bin").Take(0x18).ToArray();
                TitleHeader = File.ReadAllBytes("XSC/FFFE07D1.bin").Skip(0x18).Take(File.ReadAllBytes("XSC/FFFE07D1.bin").Length - 0x18).ToArray();
            }
            Buffer.BlockCopy(ExecutionID, 0, XoscBuff, 0x38, 0x18);

            // 0x50 Cpukeydigest of 0x10
            Buffer.BlockCopy(CreateCpukeyDigest(KvCpukey), 0, XoscBuff, 0x50, 0x10);

            // 0x60 is done via proccess

            // 0x70 copy kv's smc hash 
            Buffer.BlockCopy(Calculate_ZeroXSeventy(Mobo), 0x0, XoscBuff, 0x70, 0x10);

            // 0x80 drivePhaseLevel 
            // Buffer.BlockCopy(XSCData.DrivePhaseLevel, 0, XoscBuff, 0x80, 0x4);
            Buffer.BlockCopy(XSCData.KvC89, 0, XoscBuff, 0x83, 0x1);//fixed

            // 0x88 set running title to dash
            Buffer.BlockCopy(Tools.FixEndian(0xFFFE07D1), 0, XoscBuff, 0x88, 0x4);
                                                                                
            // 0x8C unknown padding 
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x64), 0, XoscBuff, 0x8C, 0x64);

            // 0xF0 copy our drivedata
            Buffer.BlockCopy(XSCData.DriveIndentifier, 0, XoscBuff, 0xF0, 0x24);

            // 0x114 copy our drive data osig
            Buffer.BlockCopy(XSCData.DriveIndentifier, 0, XoscBuff, 0x114, 0x24);

            // 0x138 Copy our kv serial info
            Buffer.BlockCopy(XSCData.KvSerial, 0, XoscBuff, 0x138, 0xC);

            // 0x144 serialByte
            //Buffer.BlockCopy(Tools.FillData((byte)0x0, 0x2), 0, XoscBuff, 0x144, 0x2);// wasnt correct in buffer

            // 0x145 serialByte
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x1), 0, XoscBuff, 0x145, 0x1);// fixed for 0x144 above

            // 0x146 BOOTLOADER FLAGS
            Buffer.BlockCopy(Tools.FixEndian(Type1KvStatus(XSCData.TypeKV) ? 0xD81E : 0xD83E), 0x2, XoscBuff, 0x146, 0x2);

            // 0x148 copy our xam region info
            Buffer.BlockCopy(XSCData.GameRegion, 0, XoscBuff, 0x148, 0x2);

            // 0x14A copy our xam odd features
            Buffer.BlockCopy(XSCData.HVStatusFlag, 0, XoscBuff, 0x14A, 0x2);

            // 0x150 copy our policy flash size info
            Buffer.BlockCopy(XSCData.Policyflashsize, 0, XoscBuff, 0x150, 0x4);

            // XOSC region  0x155
            Buffer.BlockCopy(Tools.FillData((byte)0x07, 0x1), 0, XoscBuff, 0x155, 0x1);

            // 0x158 status flag 
            Buffer.BlockCopy(Tools.FixEndian(ComputeHvStatusFlag(XSCData.HVStatusFlag, XSCData.FCRT, true)), 0, XoscBuff, 0x158, 0x4);

            // 0x15C Unknown padding
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x4), 0, XoscBuff, 0x15C, 0x4);

            // 0x16C Unknown padding
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x4), 0, XoscBuff, 0x16C, 0x4);

            // 0x170 pciehardwareflags 
            Buffer.BlockCopy(Tools.FixEndianlong(GetPCIEFlag(Mobo)), 0, XoscBuff, 0x170, 0x8);

            // 0x198 hvProtectionFlags but we edit 0x19F to the correct value easier
            Buffer.BlockCopy(BitConverter.GetBytes(0x01), 0, XoscBuff, 0x19F, 0x1);

            // 0x1A0 copy our console id from kv 
            Buffer.BlockCopy(XSCData.ConsoleID, 0, XoscBuff, 0x1A0, 0x5);

            // 0x1D0 copy our hardware id
            Buffer.BlockCopy(Tools.FixEndian(HWID(Mobo)), 0, XoscBuff, 0x1D0, 0x4);

            // 0x21C unknown
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x8C), 0, XoscBuff, 0x21C, 0x8C);

            // 0x2C4 clr version
            Buffer.BlockCopy(Tools.FixEndian(0x00000006), 0, XoscBuff, 0x2C4, 0x4);

            // 0x2C8 unknown 
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x10), 0, XoscBuff, 0x2C8, 0x10);

            // 0x2D8 xosc_footer
            Buffer.BlockCopy(Tools.FixEndian(0x5F534750), 0, XoscBuff, 0x2D8, 0x4);

            // 0x2DC Unknown padding
            Buffer.BlockCopy(Tools.FillData((byte)0xAA, 0x4), 0, XoscBuff, 0x2DC, 0x4);
            return XoscBuff;
        }
    }
}

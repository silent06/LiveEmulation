using System;
using static LEApi.Program;
using System.Security.Cryptography;
using System.IO;

namespace LEApi
{
    class XKEresponce
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

        public static uint GetUpdateSeq(byte[] SMAC)
        {
            SHA1Managed SHA1 = new SHA1Managed();
            byte[] hash = SHA1.ComputeHash(SMAC, 0, 6);

            Array.Reverse(hash);

            uint upd = 0x13; // 0x13 1
            uint tmp = BitConverter.ToUInt32(hash, 16);
            upd |= (uint)(tmp & ~0xFF);
            return upd;
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

        public static uint GetConsoleType(CLIENT_KVTYPE Mobo)
        {
            if (Mobo == CLIENT_KVTYPE.CORONA)
                return 0x0304000E;
            else if (Mobo == CLIENT_KVTYPE.XENON)
                return 0x010C0FFB;
            else if (Mobo == CLIENT_KVTYPE.ZEPHYR)
                return 0x010B0524;
            else if (Mobo == CLIENT_KVTYPE.FALCON)
                return 0x010C0AD8;
            else if (Mobo == CLIENT_KVTYPE.JASPER)
                return 0x010C0AD0;
            else if (Mobo == CLIENT_KVTYPE.TRINITY)
                return 0x0304000D;
            return 0x0304000E; // corona
        }

        public static byte[] CreateCpukeyDigest(byte[] KvCpukey)
        {
            return Tools.SHA1(KvCpukey);
        }

        private static byte[] ComputeHVDigest(byte[] HvSalt)// 17559
        {
            SHA1Managed SHA1 = new SHA1Managed();

            SHA1.TransformBlock(HvSalt, 0, 0x10, null, 0);
            SHA1.TransformBlock(Hypervisor, 0x34, 0x40, null, 0);
            SHA1.TransformBlock(Hypervisor, 0x78, 0xFF88, null, 0);
            SHA1.TransformBlock(Hypervisor, 0x100C0, 0x40, null, 0);
            SHA1.TransformBlock(Hypervisor, 0x10350, 0x5F70, null, 0);
            SHA1.TransformBlock(Hypervisor, 0x16EA0, 0x9160, null, 0);
            SHA1.TransformBlock(Hypervisor, 0x20000, 0xFFFF, null, 0);
            SHA1.TransformFinalBlock(Hypervisor, 0x30000, 0xFFFF);
            return SHA1.Hash;
        }

        public static byte[] CalculateSignature(byte[] SessionToken, byte[] CacheKey)
        {
            byte[] RandKey = new byte[0x14];

            // 0x10 of session token as rand
            Buffer.BlockCopy(SessionToken, 0, RandKey, 0, 0x10);
            // first 0x4 of cachekey makes up 0x10 - 0x14 of randkey 
            Buffer.BlockCopy(CacheKey, 0, RandKey, 0x10, 0x4);

            byte[] Signature = CreateSignature(CacheKey, RandKey);
            Array.Reverse(Signature);

            Signature = Tools.XeCryptBnQw_SwapDwQwLeBe(Signature);
            Array.Reverse(Signature);

            XeCryptBnQwNeRsaPubCrypt_(Signature, Signature, Program.ChallengeRSAPUB);

            Signature = Tools.XeCryptBnQw_SwapDwQwLeBe(Signature);
            Array.Reverse(Signature);
            return Signature;
        }

        private static byte[] CreateSignature(byte[] Key, byte[] Rand)
        {
            byte[] Signature = new byte[0x80];

            Signature[0] = 0;

            Buffer.BlockCopy(Rand, 0, Signature, 1, 0x14);
            Buffer.BlockCopy(Tools.StringToByteArray("DA39A3EE5E6B4B0D3255BFEF95601890AFD80709"), 0, Signature, 0x15, 0x14);
            Buffer.BlockCopy(Tools.FillData((byte)0x00, 0x1A), 0, Signature, 0x29, 0x1A);

            Signature[0x4F] = 1;

            Buffer.BlockCopy(Key, 0, Signature, 0x50, 0x30);

            byte[] tmp = new byte[0x6B];
            Buffer.BlockCopy(Signature, 0x15, tmp, 0, 0x6B);
            Buffer.BlockCopy(CalculateCheckSum(Rand, 0x14, tmp, 0x6B), 0, Signature, 0x15, 0x6B);

            byte[] tmp2 = new byte[0x14];
            Buffer.BlockCopy(Signature, 1, tmp2, 0, 0x14);

            Buffer.BlockCopy(Signature, 0x15, tmp, 0, 0x6B);
            Buffer.BlockCopy(CalculateCheckSum(tmp, 0x6B, Rand, 0x14), 0, Signature, 1, 0x14);
            return Signature;
        }

        private static byte[] CalculateCheckSum(byte[] Data, int cbData, byte[] Checksum, int cbChecksum)
        {
            int Increment = 0;

            for (int s = 0; s < cbChecksum; s += 0x14)
            {
                int Subsize = (s + 0x14 > cbChecksum) ? cbChecksum - s : 0x14;
                byte[] output = new byte[0x14];

                SHA1Managed managed = new SHA1Managed();
                managed.TransformBlock(Data, 0, cbData, null, 0);

                byte[] buffer = BitConverter.GetBytes(Increment);
                Array.Reverse(buffer);

                managed.TransformFinalBlock(buffer, 0, 4);
                output = managed.Hash;

                for (int l = 0; l < Subsize; l++)
                    Checksum[s + l] ^= output[l];

                Increment += 1;
            }
            return Checksum;
        }

        public static byte[] FindMemoryKeyFromPackage(int index)
        {
            if (File.Exists("XKE/Packages/" + index + "/Key.bin"))
                return File.ReadAllBytes("XKE/Packages/" + index + "/Key.bin");
            else
            {
                //Tools.Log(string.Format("Couldn't find memory key from package with index: {0}", index));
                Console.WriteLine(string.Format("Couldn't find memory key from package with index: {0}", index));
                return null;
            }
        }

        public static byte[] FindLivePackage(int index, string HvSalt)
        {
            if (File.Exists("XKE/Packages/" + index + "//" + HvSalt + ".bin"))
                return File.ReadAllBytes("XKE/Packages/" + index + "//" + HvSalt + ".bin");
            else
            {
                //Tools.Log(string.Format("Couldn't find live package with index: {0} | HvSalt: {1}", index, HvSalt));
                Console.WriteLine(string.Format("Couldn't find live package with index: {0} | HvSalt: {1}", index, HvSalt));
                return null;
            }
        }

        public static int FindRandomPackageSet(byte[] ClientSession)
        {
            int inc = 0;
            for (int i = 0; i < 5; ++i)
            {
                inc += ClientSession[i];
                inc = inc % Directory.GetDirectories("XKE/Packages/").Length;
            }
            return inc;
        }

        public static byte[] Build_CleanXKE_Response(byte[] SMAC, byte[] KvCpukey, byte[] HvSalt, bool CRL, CLIENT_KVTYPE Mobo, XKEDATA_STRUCT XKEData, int LivePackageIndex)
        {
            byte[] XkeBuff = new byte[0x100];

            Buffer.BlockCopy(Tools.FixEndian(0x4E4E4497), 0, XkeBuff, 0x28, 0x4); // 0x28 in xke challenge header first 0x2 are HV_MAGIC and second 0x2 are HV_VERSION 
            Buffer.BlockCopy(Tools.FixEndian(Type1KvStatus(XKEData.TypeKV) ? 0xD81E : 0xD83E), 0x2, XkeBuff, 0x2E, 0x2); // BOOT LOADER  FLAGS 0x2E in XKE challenge header (starting at 0x2)
            Buffer.BlockCopy(Tools.FixEndian(0x07600000), 0, XkeBuff, 0x30, 0x4); // 0x30 in XKE challenge header BASE_KERNEL_VERSION

            Buffer.BlockCopy(Tools.FixEndian(GetUpdateSeq(SMAC)), 0, XkeBuff, 0x34, 0x4); // 0x34 in XKE updateseq

            Buffer.BlockCopy(Tools.FixEndian(ComputeHvStatusFlag(XKEData.HvStatusFlag, XKEData.FCRT, CRL)), 0, XkeBuff, 0x38, 0x4); // 0x38 in in XKE challenge header
            Buffer.BlockCopy(Tools.FixEndian(GetConsoleType(Mobo)), 0, XkeBuff, 0x3C, 0x4); // 0x3C in XKE consoletypeSeqAllow  
            Buffer.BlockCopy(Tools.FixEndian(0x0000000200000000), 0, XkeBuff, 0x40, 0x8); // 0x40 in XKE challenge header RTOC
            Buffer.BlockCopy(Tools.FixEndian(0x0000010000000000), 0, XkeBuff, 0x48, 0x8); // 0x48 in XKE challenge header HRMOR

            byte[] EccDigest = new byte[0x14];
            byte[] HvEx = new byte[0x2];
            byte[] CacheMemoryKey = FindMemoryKeyFromPackage(LivePackageIndex);
            byte[] LivePackage = FindLivePackage(LivePackageIndex, Tools.BytesToHexString(HvSalt));

            if (LivePackage == null)
                return null;

            Buffer.BlockCopy(LivePackage, 0, EccDigest, 0, 0x14);// get eccdigest from lasting set
            Buffer.BlockCopy(LivePackage, 0x15, HvEx, 0, 0x2);// get hvex from lasting set

            Buffer.BlockCopy(EccDigest, 0, XkeBuff, 0x50, 0x14); // 0x50
            Buffer.BlockCopy(CreateCpukeyDigest(KvCpukey), 0, XkeBuff, 0x64, 0x14); // 0x64 in XKE  kvcpukey digest
            Buffer.BlockCopy(CalculateSignature(XKEData.SessionToken, CacheMemoryKey), 0, XkeBuff, 0x78, 0x80); // 0x78 in XKE challenge header Challenge sig

            Buffer.BlockCopy(HvEx, 0, XkeBuff, 0xF8, 0x2); // 0xF8 hv excuting address must be out value here for sec engine 
            Buffer.BlockCopy(ComputeHVDigest(HvSalt), 0xE, XkeBuff, 0xFA, 0x6); // 0xFA hv hash 
            return XkeBuff;
        }

    }
}

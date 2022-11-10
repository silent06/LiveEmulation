using System;
using System.Security.Cryptography;

    public static class Crypto {

         public static void Aes_Ecb(ref byte[] input, byte[] key, bool protect) {
            var aesAlg = new AesManaged {
                KeySize = 128,
                Key = key,
                BlockSize = 128,
                Mode = CipherMode.ECB,
                Padding = PaddingMode.Zeros,
                IV = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
            };

            if (protect) {
                ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);
                input = encryptor.TransformFinalBlock(input, 0, input.Length);
            } else {
                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);
                input = decryptor.TransformFinalBlock(input, 0, input.Length);
             }
        }

        public static string Base64Encode(string plainText) {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(plainText);

            return Convert.ToBase64String(plainTextBytes);
        }
    
       public static byte[] XeCryptSha(byte[] Data1, uint Data1Len, byte[] Data2, int Data2Len , byte[] Output, int OutSize) {
            SHA1Managed SHA1 = new SHA1Managed();

            SHA1.TransformBlock(Data1, 0, Convert.ToInt32(Data1Len), null, Convert.ToInt32(Data1Len));
            SHA1.TransformBlock(Data2, 0, Data2Len, null, Data2Len);
            SHA1.TransformFinalBlock(Output, 0, OutSize);

            return SHA1.Hash;
        }

        public static byte[] XOR(byte[] Data, byte[] Key) {
            byte[] data = Data;

            for (int i = 0; i < data.Length; i++)
                data[i] = (byte)(data[i] ^ Key[0]);

            return data;
        }
        
        public static byte[] HMACSHA1(byte[] Data, byte[] Key, int Offset) {
            byte[] destinationArray = new byte[Data.Length];
            Array.Copy(Data, Offset, destinationArray, 0, Data.Length);

            byte[] array = new HMACSHA1(Key).ComputeHash(destinationArray);
            Array.Resize(ref array, 0x10);
            return array;
        }

        private static ulong loadDouble(ref byte[] data, int address) {
            ulong num = 0L;
            for (int i = 0; i < 8; i++) {
                num = num << 8;
                num |= data[address + i];
            }
            return num;
        }

        private static uint loadWord(ref byte[] data, int address) {
            uint num = 0;
            for (int i = 0; i < 4; i++) {
                num = num << 8;
                num |= data[address + i];
            }
            return num;
        }

        public static byte[] Generate_RandomData(int Count) {
            byte[] RandData = new byte[Count];
            new Random().NextBytes(RandData);

            return RandData;
        }

        public static void RC4(ref byte[] Data, byte[] Key) {
            byte num;
            int num2;
            int index = 0;
            byte[] buffer = new byte[0x100];
            byte[] buffer2 = new byte[0x100];

            for (num2 = 0; num2 < 0x100; num2++) {
                buffer[num2] = (byte) num2;
                buffer2[num2] = Key[num2 % Key.GetLength(0)];
            }

            for (num2 = 0; num2 < 0x100; num2++) {
                index = ((index + buffer[num2]) + buffer2[num2]) % 0x100;
                num = buffer[num2];
                buffer[num2] = buffer[index];
                buffer[index] = num;
            }
            num2 = index = 0;
            for (int i = 0; i < Data.GetLength(0); i++) {
                num2 = (num2 + 1) % 0x100;
                index = (index + buffer[num2]) % 0x100;
                num = buffer[num2];
                buffer[num2] = buffer[index];
                buffer[index] = num;
                int num5 = (buffer[num2] + buffer[index]) % 0x100;
                Data[i] = (byte) (Data[i] ^ buffer[num5]);
            }
        }
        
        private static ulong rotateDoubleLeft(ulong inputLong, int rotate) {
            return ((inputLong << rotate) | (inputLong >> (0x40 - rotate)));
        }

        public static byte[] SHA1(byte[] Buffer) {
            return System.Security.Cryptography.SHA1.Create().ComputeHash(Buffer);
        }

        public static byte[] MD5(byte[] Data) {
               return System.Security.Cryptography.MD5.Create().ComputeHash(Data);
        }
       
        private static void storeDouble(ref byte[] data, int address, ulong inLong) {
            data[address + 7] = (byte) (inLong & ((ulong) 0xffL));
            data[address + 6] = (byte) ((inLong & 0xff00L) >> 8);
            data[address + 5] = (byte) ((inLong & 0xff0000L) >> 0x10);
            data[address + 4] = (byte) ((inLong & 0xff000000L) >> 0x18);
            data[address + 3] = (byte) ((inLong & 0xff00000000L) >> 0x20);
            data[address + 2] = (byte) ((inLong & 0xff0000000000L) >> 40);
            data[address + 1] = (byte) ((inLong & 0xff000000000000L) >> 0x30);
            data[address] = (byte)((inLong & 0xFF00000000000000L) >> 0x38);
        }
        
        public static byte[] XeCryptRotSum(byte[] r3, byte[] r4, int r5) {

            int address = 0;
            ulong inputLong = 0L;
            ulong inLong = loadDouble(ref r3, 0);
            ulong num4 = loadDouble(ref r3, 8);
            ulong num5 = loadDouble(ref r3, 0x10);
            ulong num6 = loadDouble(ref r3, 0x18);

            if (r5 != 0) {
                for (int i = r5; i > 0; i--) {
                    ulong num8 = loadDouble(ref r4, address);
                    inputLong = num8 + num4;
                    num4 = 1L;

                    if (inputLong >= num8) {
                        num4 = 0L;
                    }

                    num6 -= num8;
                    inLong = num4 + inLong;
                    num4 = rotateDoubleLeft(inputLong, 0x1d);

                    if (num6 <= num8) {
                        num8 = 0L;
                    } else {
                        num8 = 1L;
                    }
                    num5 -= num8;
                    num6 = rotateDoubleLeft(num6, 0x1f);
                    address += 8;
                }
                storeDouble(ref r3, 0, inLong);
                storeDouble(ref r3, 8, num4);
                storeDouble(ref r3, 0x10, num5);
                storeDouble(ref r3, 0x18, num6);
            }
            return r3;
        }

    public static byte[] XeCryptRotSumSha(byte[] Data, int r4) {
        uint num = (uint) (r4 >> 3);
        byte[] inputBuffer = XeCryptRotSum(new byte[0x20], Data, (int) num);

        SHA1Managed SHA1 = new SHA1Managed();
        SHA1.TransformBlock(inputBuffer, 0, 0x20, null, 0);
        SHA1.TransformBlock(inputBuffer, 0, 0x20, null, 0);
        SHA1.TransformBlock(Data, 0, r4, null, 0);

        for (int i = 0; i < 0x20; i++)
            inputBuffer[i] = inputBuffer[i];
        SHA1.TransformBlock(inputBuffer, 0, 0x20, null, 0);
        SHA1.TransformFinalBlock(inputBuffer, 0, 0x20);
        return SHA1.Hash;
    }
    
    public static byte[] XeCryptBnQw_SwapDwQwLeBe(byte[] input) {
        byte[] buffer = new byte[input.Length];
        int num = input.Length - 8;
        int num2 = 0;

        for (int i = 0; i < (input.Length / 8); i++) {
            for (int j = 0; j < 8; j++)
                buffer[num2 + j] = input[num + j];
            num -= 8;
            num2 += 8;
        }
        return buffer;
    }
}


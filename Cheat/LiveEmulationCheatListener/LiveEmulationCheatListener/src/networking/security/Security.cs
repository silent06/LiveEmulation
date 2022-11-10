using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;

namespace LiveEmulationCheatListener {
    class Security {
        public static void RC4(ref byte[] Data, byte[] Key, int startOffset = 0) {
            byte num;
            int num2;
            int index = 0;

            byte[] buffer = new byte[0x100];
            byte[] buffer2 = new byte[0x100];

            for (num2 = 0; num2 < 0x100; num2++) {
                buffer[num2] = (byte)num2;
                buffer2[num2] = Key[num2 % Key.GetLength(0)];
            }

            for (num2 = 0; num2 < 0x100; num2++) {
                index = ((index + buffer[num2]) + buffer2[num2]) % 0x100;
                num = buffer[num2];
                buffer[num2] = buffer[index];
                buffer[index] = num;
            }

            num2 = index = 0;

            for (int i = startOffset; i < Data.GetLength(0); i++) {
                num2 = (num2 + 1) % 0x100;
                index = (index + buffer[num2]) % 0x100;
                num = buffer[num2];
                buffer[num2] = buffer[index];
                buffer[index] = num;
                int num5 = (buffer[num2] + buffer[index]) % 0x100;
                Data[i] = (byte)(Data[i] ^ buffer[num5]);
            }
        }

        public static byte[] GenerateRandomData(int count) {
            byte[] RandData = new byte[count];
            new Random().NextBytes(RandData);

            return RandData;
        }

        public static void SendPacket(EndianWriter serverWriter, Header header, byte[] dgram) {
            serverWriter.Write(dgram);
            serverWriter.Close();
        }
    }
}

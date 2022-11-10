using System;
using System.IO;
using System.Threading;

namespace LE {
    internal class serverStream : Stream {
        public static byte[] ServerEncryptionKey = new byte[] {
            0x16, 0xD8, 0x68, 0x8E, 0xF0, 0x1B, 0xE7, 0x66, 0x48, 0xC8, 0x94, 0x77,
            0x8F, 0xAD, 0xFE, 0x5D
        };

        public static byte[] MasterKey = new byte[] {
            0x0E, 0xE0, 0x83, 0xF4, 0xDB, 0x73, 0x57, 0xFF, 0x0A, 0x6B, 0x08, 0x1F,
            0x22, 0x62, 0x28, 0xEF
        };

        public Stream stream;
        public serverStream(Stream s) {
            stream = s;
        }

        public override void Flush() {
            stream.Flush();  
        }
        
        public override int Read(byte[] buffer, int Offset, int Count) {
            byte[] Data = new byte[Count];
            int num = stream.Read(Data, 0, Count);
            while (num != Count) {
                int num2 = stream.Read(Data, num, Count - num);
                if (num2 <= 0) 
                    break;
                num += num2;
            }

            //Crypto.Aes_Ecb(ref Data, ServerEncryptionKey, false);
            //Crypto.RC4(ref Data, ServerEncryptionKey);
            //Array.Reverse(Data);

            //for (int i = 0; i < Count; i++)
                //Data[i] ^= (byte)0x31;

            //Crypto.RC4(ref Data, MasterKey);
            //Array.Reverse(Data);

            Buffer.BlockCopy(Data, 0, buffer, Offset, num);
            return num;
        }

        public override long Seek(long offset, SeekOrigin origin) {
            return stream.Seek(offset, origin);
        }

        public override void SetLength(long value) {
            stream.SetLength(value);
        }
        
        public override void Write(byte[] buffer, int offset, int count) {
            byte[] Data = new byte[count];
            Buffer.BlockCopy(buffer, offset, Data, 0, count);

            //Crypto.Aes_Ecb(ref Data, ServerEncryptionKey, true);
            //Crypto.RC4(ref Data, ServerEncryptionKey);
            //Array.Reverse(Data);

            //for (int i = 0; i < count; i++)
                //Data[i] ^= (byte)0x31;

            //Crypto.RC4(ref Data, MasterKey);
            //Array.Reverse(Data);

            byte[] tmp = new byte[ClientHandler.MaxSendRec];
            for (int i = 0; i * ClientHandler.MaxSendRec < Data.Length; i++) {
                int Remaining = (Data.Length - (i * ClientHandler.MaxSendRec));

                if (Remaining >= ClientHandler.MaxSendRec) {
                    Buffer.BlockCopy(Data, (i * ClientHandler.MaxSendRec), tmp, 0, ClientHandler.MaxSendRec);
                    stream.Write(tmp, 0, ClientHandler.MaxSendRec);
                }
                else {
                    Buffer.BlockCopy(Data, (i * ClientHandler.MaxSendRec), tmp, 0, Remaining);
                    stream.Write(tmp, 0, Remaining);
                }
            }
        }

        public override bool CanRead {
            get {
                return stream.CanRead;
            }
        }

        public override bool CanSeek {
            get {
                return stream.CanSeek;
            }
        }

        public override bool CanWrite {
            get {
                return stream.CanWrite;
            }
        }

        public override long Length {
            get {
                return stream.Length;
            }
        }

        public override long Position {
            get {
                return stream.Position;
            } set {
                stream.Position = Position;
            }
        }
    }
}

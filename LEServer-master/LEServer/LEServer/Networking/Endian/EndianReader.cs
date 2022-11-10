using System;
using System.IO;

namespace Security {

    public class EndianReader : BinaryReader {
        private readonly EndianStyle _endianStyle;

        public EndianReader(Stream Stream, EndianStyle EndianStyle) : base(Stream) {
            _endianStyle = EndianStyle;
        }

        public override double ReadDouble() {
            return ReadDouble(_endianStyle);
        }

        public override bool ReadBoolean() {
            return this.ReadBoolean(this._endianStyle);
        }

        public bool ReadBoolean(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(1);
            if (EndianStyle == EndianStyle.BigEndian) 
                Array.Reverse(array);
            return BitConverter.ToBoolean(array, 0);
        }

        public double ReadDouble(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(8);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToDouble(array, 0);
        }

        public override short ReadInt16() {
            return ReadInt16(_endianStyle);
        }

        public short ReadInt16(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(2);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToInt16(array, 0);
        }

        public int ReadInt24() {
            return ReadInt24(_endianStyle);
        }

        public int ReadInt24(EndianStyle EndianStyle) {
            byte[] buffer = base.ReadBytes(3);
            if (EndianStyle == EndianStyle.BigEndian) {
                return (((buffer[0] << 0x10) | (buffer[1] << 8)) | buffer[2]);
            }
            return (((buffer[2] << 0x10) | (buffer[1] << 8)) | buffer[0]);
        }

        public override int ReadInt32() {
            return ReadInt32(_endianStyle);
        }

        public int ReadInt32(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(4);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToInt32(array, 0);
        }

        public override long ReadInt64() {
            return ReadInt64(_endianStyle);
        }

        public long ReadInt64(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(8);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToInt64(array, 0);
        }

        public override float ReadSingle() {
            return ReadSingle(_endianStyle);
        }

        public float ReadSingle(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(4);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToSingle(array, 0);
        }

        public string ReadStringNullTerminated() {
            string str = string.Empty;
            while (true) {
                byte num = ReadByte();
                if (num == 0) {
                    return str;
                }
                str = str + ((char) num);
            }
        }

        public override ushort ReadUInt16() {
            return ReadUInt16(_endianStyle);
        }

        public ushort ReadUInt16(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(2);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToUInt16(array, 0);
        }

        public override uint ReadUInt32() {
            return ReadUInt32(_endianStyle);
        }

        public uint ReadUInt32(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(4);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToUInt32(array, 0);
        }

        public override ulong ReadUInt64() {
            return ReadUInt64(_endianStyle);
        }

        public ulong ReadUInt64(EndianStyle EndianStyle) {
            byte[] array = base.ReadBytes(8);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(array);
            }
            return BitConverter.ToUInt64(array, 0);
        }

        public string ReadUnicodeNullTermString() {
            string str = string.Empty;
            while (true) {
                ushort num = ReadUInt16(EndianStyle.BigEndian);
                if (num == 0) {
                    return str;
                }
                str = str + ((char) num);
            }
        }

        public string ReadUnicodeString(int Length) {
            return ReadUnicodeString(Length, _endianStyle);
        }

        public string ReadUnicodeString(int length, EndianStyle endianStyle) {
            string str = string.Empty;
            int num = 0;
            for (int i = 0; i < length; i++) {
                ushort num3 = ReadUInt16(endianStyle);
                num++;
                if (num3 == 0) {
                    break;
                }
                str = str + ((char) num3);
            }
            int num4 = (length - num) * 2;
            BaseStream.Seek((long) num4, SeekOrigin.Current);
            return str;
        }

        public void Seek(long position) {
            base.BaseStream.Position = position;
        }

        public uint SeekNReed(long Address) {
            base.BaseStream.Position = Address;
            return ReadUInt32();
        }
    }
}


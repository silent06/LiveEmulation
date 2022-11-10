using System;
using System.IO;
using System.Text;

namespace Security {

    public class EndianWriter : BinaryWriter {
        private readonly EndianStyle _endianStyle;

        public EndianWriter(Stream Stream, EndianStyle EndianStyle) : base(Stream) {
            _endianStyle = EndianStyle;
        }

        public void Seek(long position) {
            base.BaseStream.Position = position;
        }

        public void SeekNWrite(long position, int Value)
        {
            base.BaseStream.Position = position;
            base.Write(Value);
        }

        public override void Write(double value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(short value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(int value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(long value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(float value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(ushort value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(uint value)
        {
            Write(value, _endianStyle);
        }

        public override void Write(ulong value)
        {
            Write(value, _endianStyle);
        }

        public void Write(double value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public override void Write(bool value)
        {
            this.Write(value, this._endianStyle);
        }

        public void Write(bool value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian) Array.Reverse(bytes);
            base.Write(bytes);
        }

        public void Write(short value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void Write(int value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void Write(long value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void Write(float value, EndianStyle EndianStyle) {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void Write(ushort value, EndianStyle EndianStyle) {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian) {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void Write(uint value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void Write(ulong value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void WriteInt24(int value)
        {
            WriteInt24(value, _endianStyle);
        }

        public void WriteInt24(int value, EndianStyle EndianStyle)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            Array.Resize<byte>(ref bytes, 3);
            if (EndianStyle == EndianStyle.BigEndian)
            {
                Array.Reverse(bytes);
            }
            base.Write(bytes);
        }

        public void WriteString(string value)
        {
            char[] chars = value.ToCharArray();
            base.Write(chars);
        }

        public void WriteUnicodeString(string Value)
        {
            byte[] bytes = Encoding.BigEndianUnicode.GetBytes(Value);
            base.Write(bytes);
            base.Write(new byte[2]);
        }

        public void WriteUnicodeString(string String, int length)
        {
            WriteUnicodeString(String, length, _endianStyle);
        }

        public void WriteUnicodeString(string String, int length, EndianStyle endianStyle) {
            int num = String.Length;
            for (int i = 0; i < num; i++)
            {
                if (i > length)
                {
                    break;
                }
                ushort num3 = String[i];
                Write(num3, endianStyle);
            }
            int num4 = (length - num) * 2;
            if (num4 > 0)
            {
                Write(new byte[num4]);
            }
        }
    }
}


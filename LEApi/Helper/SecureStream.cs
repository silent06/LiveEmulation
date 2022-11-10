using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace LEApi.Helper
{
    internal class SecureStream : Stream
    {
        public Stream _Stream;

        private byte[] NetworkKey = { 0xE3, 0x44, 0x49, 0x7D, 0x92, 0xF7, 0x0B, 0xE6, 0x98, 0x71, 0xEC, 0x07, 0xC7, 0x99, 0x18, 0x52 };

        public SecureStream(Stream stream)
        {
            this._Stream = stream;
        }

        public override void Flush()
        {
            this._Stream.Flush();
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            byte[] buffer2 = new byte[count];
            int received = this._Stream.Read(buffer2, 0, count);
            while (received != count)
            {
                int current = this._Stream.Read(buffer2, received, count - received);
                if (current <= 0) break;
                received += current;
            }
            Buffer.BlockCopy(Tools.CRC4(NetworkKey, buffer2), 0, buffer, offset, received);
            return received;
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            byte[] buffer2 = new byte[count];
            Buffer.BlockCopy(buffer, offset, buffer2, 0, count);
            this._Stream.Write(Tools.CRC4(NetworkKey, buffer2), 0, count);
        }

        public override long Length
        {
            get { return this._Stream.Length; }
        }

        public override void SetLength(long value)
        {
            this._Stream.SetLength(value);
        }

        public override long Position
        {
            get { return this._Stream.Position; }
            set { this._Stream.Position = this.Position; }
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            return this._Stream.Seek(offset, origin);
        }

        public override bool CanSeek
        {
            get { return this._Stream.CanSeek; }
        }

        public override bool CanRead
        {
            get { return this._Stream.CanRead; }
        }

        public override bool CanWrite
        {
            get { return this._Stream.CanWrite; }
        }
    }
}

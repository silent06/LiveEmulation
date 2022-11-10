using System.Runtime.InteropServices;

namespace LE {

    [StructLayout(LayoutKind.Sequential)]
    public struct TOKEN_STRUCT {
        public string Token;
        public int Days;
        public int Status;
    }
}

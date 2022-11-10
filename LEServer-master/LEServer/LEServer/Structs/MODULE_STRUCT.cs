using System.Runtime.InteropServices;

namespace LE {
        [StructLayout(LayoutKind.Sequential)]

        public struct MODULE_STRUCT {
            
            public byte[] ClientINIData;
            public byte[] B02BypassData;
            public byte[] GhostsBypassData;
            public byte[] AWBypassData;
        
            public byte[] HVData;
            public byte[] XamHeaderData;
            public byte[] KrnHeaderData;
            public byte[] CurModuleHeaderData;
            public byte[] XkeRsa;
    }
}

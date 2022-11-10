using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace LiveEmulationCheatListener {
    class Global {
        public static int m_Port = 15422;
        public static int m_MinimumRequestSize = 0x2C; // sizeof(Header)
        public static int m_MaximumRequestSize = 0x1000;
        public static int m_ConnectedClients = 0;
        public static bool m_IsUsingSpamDetection;
    }

    public enum Packets {
        PACKET_WELCOME = 1,
        PACKET_HEARTBEAT,
        PACKET_STATUS,
        PACKET_LEAVE,
        PACKET_CHALLENGE,
        PACKET_VERSION,
        PACKET_GETDATA,
        PACKET_GETFUNC,
        PACKET_GETWELCOMEMSG,
        PACKET_GETCHANGELOG,
        PACKET_GETNATIVES,
        PACKET_PRESENCE
    }

    public struct Header {
        public Packets m_Packet;
        public int m_Size;
        public int m_Title;
        public byte[] m_CPU; // 0x10
        public byte[] m_HvCPU; // 0x10
        public byte[] m_token; // 0x20

        // Encryption
        public int m_Key1;
        public int m_Key2;
        public int m_Key3;
        public int m_Key4;
        public int m_Key5;
        public int m_Key6;
        public int m_Key7;
        public int m_Key8;
        public int m_Key9;
        public int m_Key10;
        public int m_Hash;
    }

    public struct ClientData {
        public int m_Id;
        public string m_CPU;
        public bool m_HasUsedBefore;
        public int m_AuthStatus;
        public string m_BannedReason;
    }

    public struct ClientEndPoint {
        public int m_Id;
        public string m_Token;
        public long m_LastHeartbeat;
        public long m_WelcomeTime;
        public bool m_HasReceivedPresence;
        public int m_MainConnectionIndex;
    }

    public struct ClientMetric {
        public int m_Index;
        public int m_Type;

        public ClientMetric(int index, int type) {
            m_Index = index;
            m_Type = type;
        }
    }

    public struct SocketSpam {
        public long m_InitialTimestamp;
        public int m_ConnectionsMade;
        public bool m_Banned;
        public long m_BannedTimestamp;
        public List<long> m_ConnectionTimestamps;

        public SocketSpam(long init, int con, bool banned, long bannedInit) {
            m_InitialTimestamp = init;
            m_ConnectionsMade = con;
            m_Banned = banned;
            m_BannedTimestamp = bannedInit;
            m_ConnectionTimestamps = new List<long>();
        }
    }
}

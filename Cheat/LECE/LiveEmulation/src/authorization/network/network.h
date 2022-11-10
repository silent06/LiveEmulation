#pragma once

#define LOCAL_SERVER

class NetSocket {
public:
	bool Initialize() {
		if (m_connected) return true;

		XNetStartupParams xnsp;
		xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
		xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;

		if (Native::System::NetDll_XNetStartup(XNCALLER_SYSAPP, &xnsp) != S_OK) {
#ifdef DEVELOPER_BUILD
			LOG(false, "[Networking] Failed to start up XNet");
#endif
			return false;
		}

		WSADATA WsaData;
		if (Native::System::NetDll_WSAStartupEx(XNCALLER_SYSAPP, MAKEWORD(0x02, 0x02), &WsaData, 0x02) != S_OK) {
#ifdef DEVELOPER_BUILD
			LOG(false, "[Networking] Failed to start up WSA");
#endif
			return false;
		}
		return true;
	}

	NetSocket& CreateSocket() {
		Native::System::NetDll_closesocket(XNCALLER_SYSAPP, m_socket);

		for (int i = 0; i < 5; i++) {
			m_socket = Native::System::NetDll_socket(XNCALLER_SYSAPP, AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_socket == SOCKET_ERROR) {
#ifdef DEVELOPER_BUILD
				LOG(false, "[Networking] Failed to create socket to server (attempt: %i)", i);
#endif
			} else {
				break; 
			}

			Native::System::Sleep(100);
		}

		return *this;
	}

	NetSocket& Connect() {
		m_server.sin_family = AF_INET;
		m_server.sin_port = htons(15422);

#ifdef LOCAL_SERVER
		m_server.sin_addr.S_un.S_un_b.s_b1 = 192;
		m_server.sin_addr.S_un.S_un_b.s_b2 = 168;
		m_server.sin_addr.S_un.S_un_b.s_b3 = 0;
		m_server.sin_addr.S_un.S_un_b.s_b4 = 14;
#else
		m_server.sin_addr.S_un.S_un_b.s_b1 = 74;
		m_server.sin_addr.S_un.S_un_b.s_b2 = 93;
		m_server.sin_addr.S_un.S_un_b.s_b3 = 115;
		m_server.sin_addr.S_un.S_un_b.s_b4 = 161;
#endif

		for (int i = 0; i < 5; i++) {
			if (Native::System::NetDll_connect(XNCALLER_SYSAPP, m_socket, (struct sockaddr*)&m_server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
#ifdef DEVELOPER_BUILD
				LOG(false, "[Networking] Failed to connect to server (attempt: %i)", i);
#endif
			} else {
				break;
			}
			Native::System::Sleep(100);
		}

		return *this;
	}

	NetSocket& SetSocketOption(int name, const char* value, int size = 4) {
		setsockopt(m_socket, SOL_SOCKET, name, value, size);
		return *this;
	}

	NetSocket& SetRequiredSocketOptions() {
		BOOL sockOpt = TRUE;
		Native::System::NetDll_setsockopt(XNCALLER_SYSAPP, m_socket, SOL_SOCKET, 0x5801, (const char*)&sockOpt, 4);

		DWORD sendRecvSize = 0x1000;
		Native::System::NetDll_setsockopt(XNCALLER_SYSAPP, m_socket, SOL_SOCKET, SO_SNDBUF, (const char*)&sendRecvSize, 0x04);
		Native::System::NetDll_setsockopt(XNCALLER_SYSAPP, m_socket, SOL_SOCKET, SO_RCVBUF, (const char*)&sendRecvSize, 0x04);
		return *this;
	}

	NetSocket& Send(void* sendBuf, int size) {
		Native::System::NetDll_send(XNCALLER_SYSAPP, m_socket, (char*)sendBuf, size, 0);
		return *this;
	}

	NetSocket& Receive(void* recvBuf, int size, int* readBytes, bool decrypt = true) {
		DWORD remaining = size;
		DWORD received = 0;
		while (remaining > 0) {
			DWORD recvsize = 0x1000 < remaining ? 0x1000 : remaining;
			DWORD receive = Native::System::NetDll_recv(XNCALLER_SYSAPP, m_socket, (char*)recvBuf + received, recvsize, 0);
			if (receive == SOCKET_ERROR) {
				m_hasError = true;
				strcpy(m_lastError, "Receive failed");
				return *this;
			}

			if (receive == 0) break;

			remaining -= receive;
			received += receive;
		}

		*readBytes = received;

		return *this;
	}

	const char* GetLastError() { return m_lastError; }
	bool Error() { return m_hasError; }
private:
	bool m_connected;
	bool m_hasError;
	char m_lastError[260];

	sockaddr_in m_server;
	SOCKET m_socket;
};

NetSocket* GetNetSocket();
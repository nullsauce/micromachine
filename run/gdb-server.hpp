/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_GDB_SERVER_HPP
#define MICROMACHINE_EMU_GDB_SERVER_HPP


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <regex>
#include <cpu.hpp>

namespace {
	static uint8_t compute_checksum(const std::string& data) {
		uint64_t computed_checksum = 0;
		for(char c : data) {
			computed_checksum += c;
		}
		return (uint8_t)(computed_checksum % 256);
	}
}


class client_stream {
private:
	const int _socket;
public:
	client_stream(int socket)
		: _socket(socket)
	{}

	void send_ack() {
		send(_socket, "+45", 3, 0);
	}

	void send_packet(const std::string& data) {
		static char packet_buffer[2048];
		std::string packet_data = escape(data);
		uint8_t checksum = compute_checksum(packet_data);
		int packet_size = snprintf(packet_buffer, sizeof(packet_buffer), "$%s#%02x", packet_data.c_str(), checksum);
		send(_socket, packet_buffer, packet_size, 0);
		fprintf(stderr, "sent: %s\n", packet_buffer);
	}

	std::string escape(const std::string& data) {
		std::stringbuf ss;
		for(char c : data) {
			if(c == '$' || c == '#' || c == '}') {
				ss.sputc('}');
				ss.sputc(c ^ 0x20);
			} else {
				ss.sputc(c);
			}
		}
		return ss.str();
	}
};

class gdb_server {
private:
	cpu& _cpu;

public:
	gdb_server(cpu& cpu)
		: _cpu(cpu) {
			// Creating socket file descriptor
			int server_fd = 0;
			if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				throw "failed to create scoket";
		}

		// Force to reuse address
		int opt = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
		struct sockaddr_in address;
		int addrlen = sizeof(address);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(2345);

		// Forcefully attaching socket to the port 8080
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
			throw "bind failed";
		}

		if (listen(server_fd, 0) < 0) {
			throw "listen failed";
		}

		int client_socket;
		if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			throw "accept failed";
		}

		client_stream client(client_socket);
		std::string matchbuff;
		static char recvbuf[1024];
		while(true) {
			int received = read(client_socket , recvbuf, sizeof(recvbuf));
			if(received == 0) {
				break;
			}
			matchbuff += std::string(recvbuf, received);
			fprintf(stderr, "recvbuf: %s\n", std::string(recvbuf, received).c_str());
			std::smatch m;
			std::regex e ("\\$([^#]*)#([a-f0-9]{2})");
			while (std::regex_search(matchbuff, m, e)) {
				std::string data = m[1];
				std::string checksum = m[2];
				uint64_t computed_checksum = compute_checksum(data);
				matchbuff = m.suffix().str();
				fprintf(stderr, "parsed: %s\n", data.c_str());
				client.send_ack();
				handle_packet(data, client);
			}
		}
		fprintf(stderr, "connection lost\n");
	}

	void handle_packet(const std::string& packet, client_stream& client) {
		if(packet.rfind("qSupported:", 0) == 0) {
			client.send_packet("PacketSize=1024");
		} else if(packet.rfind("vMustReplyEmpty", 0) == 0) {
			client.send_packet("");
		} else if(packet.rfind("Hg", 0) == 0) {
			client.send_packet("OK");

		} else if(packet.compare("qTStatus") == 0) {
			client.send_packet("");
		} else if(packet.compare("?") == 0) {
			client.send_packet("S05");
		} else if(packet.compare("qfThreadInfo") == 0) {
			client.send_packet("m0");
		} else if(packet.compare("qsThreadInfo") == 0) {
			client.send_packet("l");
		} else if(packet.compare("qAttached") == 0) {
			client.send_packet("1");
		} else if(packet.compare("qC") == 0) {
			client.send_packet("QC0");
		} else if(packet.compare("g") == 0) {
			static char buf[16*8+1];
			sprintf(buf, "%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X",
				binops::swap32(_cpu.regs().get(0)),
				binops::swap32(_cpu.regs().get(1)),
				binops::swap32(_cpu.regs().get(2)),
				binops::swap32(_cpu.regs().get(3)),
				binops::swap32(_cpu.regs().get(4)),
				binops::swap32(_cpu.regs().get(5)),
				binops::swap32(_cpu.regs().get(6)),
				binops::swap32(_cpu.regs().get(7)),
				binops::swap32(_cpu.regs().get(8)),
				binops::swap32(_cpu.regs().get(9)),
				binops::swap32(_cpu.regs().get(10)),
				binops::swap32(_cpu.regs().get(11)),
				binops::swap32(_cpu.regs().get(12)),
				binops::swap32(_cpu.regs().get(13)),
				binops::swap32(_cpu.regs().get(14)),
				binops::swap32(_cpu.regs().get(15))
			);
			client.send_packet(buf);
		} else if(packet[0] == 'm') {
			uint32_t address = 0;
			uint32_t size = 0;
			if(2 == sscanf(packet.data(), "m%08X,%d", &address, &size)) {
				std::string hexbuf(size*2, '0');
				for(uint32_t i = 0; i < size; i++) {
					static const char* hex = "0123456789ABCDEF";
 					uint8_t byte = _cpu.mem().read8_unchecked(address + i);
 					char* str_data = ((char*)hexbuf.data());
					str_data[i*2+0] = hex[(byte >> 4)];
					str_data[i*2+1] = hex[(byte & 0xf)];
				}
				client.send_packet(hexbuf);
			} else {
				client.send_packet("E14"); // bad address;
			}

		} else if(packet[0] == 'p') {
			uint32_t regsiter_id = 0;
			if(1 == sscanf(packet.data(), "p%X", &regsiter_id)) {
				if(regsiter_id < 15) {
					static char buf[8+1];
					sprintf(buf, "%08X", binops::swap32(_cpu.regs().get(regsiter_id)));
					client.send_packet(buf);
				} else {
					client.send_packet("E2");
				}
			}
		} else {
			client.send_packet("");
		}
	}

};


#endif //MICROMACHINE_EMU_GDB_SERVER_HPP

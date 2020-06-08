

#pragma once


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <regex>
#include <cpu.hpp>
#include <stdarg.h>

#include "loader.hpp"

#define GDB_SERVER_MAX_PACKET_SIZE 2048

namespace {
	static uint8_t compute_checksum(const std::vector<uint8_t>& data) {
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
		send(_socket, "+", 1, 0);
	}

	void send_packet_binary(const uint8_t* data, size_t data_size) {
		const size_t size_of_checksum = 2;
		const size_t size_of_separators = 2;
		if(data_size > GDB_SERVER_MAX_PACKET_SIZE - (size_of_checksum + size_of_separators)) {
			fprintf(stderr, "Error: data is too large!");
			return;
		}
		static std::vector<uint8_t> packet_buffer(GDB_SERVER_MAX_PACKET_SIZE);
		std::vector<uint8_t> packet_data = escape(data, data_size);
		uint8_t checksum = compute_checksum(packet_data);
		packet_buffer.clear();
		packet_buffer.push_back('$');
		packet_buffer.insert(packet_buffer.end(), packet_data.begin(), packet_data.end());
		packet_buffer.push_back('#');
		char checksum_str[3];
		snprintf(checksum_str, 3, "%02x", checksum);
		packet_buffer.push_back(checksum_str[0]);
		packet_buffer.push_back(checksum_str[1]);
		send(_socket, packet_buffer.data(), packet_buffer.size(), 0);
		packet_buffer.push_back(0); // null terminator for printf
		fprintf(stderr, "sent: %s\n", packet_buffer.data());
	}

	void send_packet(const char* data, ...) {
		static uint8_t packet_buffer[2048];
		va_list argptr;
		va_start(argptr, data);
		size_t size = vsprintf((char*)packet_buffer, data, argptr);
		va_end(argptr);
		send_packet_binary(packet_buffer, size);
	}

	void send_packet(const std::string& data) {
		return send_packet(data.c_str());
	}
	/*
	void send_packet(const std::string& data) {
		static char packet_buffer[2048];
		std::string packet_data = escape(data);
		uint8_t checksum = compute_checksum(packet_data);
		int packet_size = snprintf(packet_buffer, sizeof(packet_buffer), "$%s#%02x", packet_data.c_str(), checksum);
		send(_socket, packet_buffer, packet_size, 0);
		fprintf(stderr, "sent: %s\n", packet_buffer);
	}*/

	std::vector<uint8_t> escape(const uint8_t* data, size_t size) {
		std::vector<uint8_t> escaped;
		escaped.reserve(size*2);

		for(size_t i = 0; i < size; i++) {
			char c = data[i];
			if(c == '$' || c == '#' || c == '*'  || c == '}') {
				escaped.push_back('}');
				escaped.push_back(c ^ 0x20);
			} else {
				escaped.push_back(c);
			}
		}
		return escaped;
	}

	std::vector<uint8_t> escape(const std::string& data) {
		return escape((uint8_t*)data.data(), data.size());
	}
};

class gdb_server {
private:
	cpu& _cpu;
	loader::program::ptr _program;

public:

	gdb_server(cpu& cpu, loader::program::ptr program)
		: _cpu(cpu)
		, _program(program) {
			// Creating socket file descriptor
			int server_fd = 0;
			if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				throw "failed to create socket";
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
		static char recvbuf[GDB_SERVER_MAX_PACKET_SIZE];
		while(true) {
			int received = read(client_socket , recvbuf, sizeof(recvbuf));
			if(received == 0) {
				break;
			}
			matchbuff += std::string(recvbuf, received);
			fprintf(stderr, "recvbuf: %s\n", std::string(recvbuf, received).c_str());
			std::smatch m;
			std::regex e ("(\\+)|(\\x03)|(\\$([^#]*)#([a-f0-9]{2}))");
			while (std::regex_search(matchbuff, m, e)) {
				std::string whole_match = m[0];


				// handle special cases for user interruption via CTRL+C (0x03)
				// and client ack (+)
				if(whole_match.compare("+") == 0) {
					// ack
					fprintf(stderr, "parsed: ack\n");
				} else if(whole_match.compare("\x03") == 0) {
					// CTRL+C
					fprintf(stderr, "User requested exit\n");
					client.send_packet("476f6f64627965210a");
				} else {
					std::string data = m[4];
					std::string checksum = m[5];
					//uint64_t computed_checksum = compute_checksum(data);
					fprintf(stderr, "parsed: %s\n", data.c_str());
					client.send_ack();
					handle_packet(data, client);
				}

				// 'consume' the matched expression by keeping
				// only whats after the match.
				matchbuff = m.suffix().str();
			}
		}
		fprintf(stderr, "connection lost\n");
	}

	void handle_packet(const std::string& packet, client_stream& client) {
		std::smatch smatch;
		if(packet.rfind("qSupported:", 0) == 0) {
			client.send_packet("PacketSize=800;vContSupported;qXfer:exec-file:read+");
			//client.send_packet("QStartNoAckMode");
		} else if(packet.rfind("vMustReplyEmpty", 0) == 0) {
			client.send_packet("");
		} else if(packet[0] == 'H') {
			if(packet[1] == 'c') {
				_cpu.step();
			}
			client.send_packet("OK");
		} else if(packet[0] == 'c') {
			client.send_packet("OK");
		} else if(packet[0] == 'Z') {
			client.send_packet("");
		} else if(packet[0] == 'z') {
			client.send_packet("");
		} else if(packet[0] == 'X') {
			uint32_t address = 0;
			uint32_t size = 0;
			sscanf(packet.data(), "X%x,%x:", &address, &size);
			fprintf(stderr, "write memory address=%u, size=%u\n", address, size);
			size_t data_begin = packet.find(':') + 1;
			for(size_t i = 0; i < size; i++) {
				uint32_t byte_address = address + i;
				uint8_t byte_value = packet.data()[data_begin + i];
				fprintf(stderr, "write byte address=%08x val=%02x\n", byte_address, byte_value);
				if(!_cpu.mem().write8(byte_address, byte_value)) {
					fprintf(stderr, "memory write failed: address=%08x, value=%02x\n", byte_address, byte_value);
				}
			}
			client.send_packet("OK");

		} else if(packet.compare("qTStatus") == 0) {
			client.send_packet("");
		} else if(packet.compare("vCont?") == 0) {
			client.send_packet("vCont;c;C;s;S;t");
		} else if(packet.rfind("vCont", 0) == 0) {
			_cpu.step();
			std::stringstream ss;
			for(int i = 0; i < 16; i++) {
				char buff[32];
				sprintf(buff, "%02x:%08x;", i, binops::swap32(_cpu.regs().get(i)));
				ss << std::string(buff);
			}
			//ss << "thread:0;";
			client.send_packet("T05" + ss.str());
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
		} else if(packet.compare("qOffsets") == 0) {
			client.send_packet("TextSeg=0");
		} else if(packet.compare("qSymbol::") == 0) {
			client.send_packet("OK");
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
			if(2 == sscanf(packet.data(), "m%X,%d", &address, &size)) {
				std::string hexbuf(size*2, '0');
				for(uint32_t i = 0; i < size; i++) {
					static const char* hex = "0123456789ABCDEF";
					uint8_t byte = _cpu.mem().read8_unchecked(address + i);
					fprintf(stderr, "read byte address=%08x val=%02x\n", address + i, byte);
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
		} else if(packet.rfind("qXfer:exec-file:read:", 0) == 0) {
			client.send_packet("l" + _program->path());
		} else if(packet.rfind("vFile:setfs:", 0) == 0) {
			client.send_packet("");
		} else if(packet.rfind("vFile:fstat", 0) == 0) {
			client.send_packet("");
		} else if(packet.rfind("vFile:open:6a7573742070726f62696e67", 0) == 0) {
			// fake open requested used for probing
			client.send_packet("F-1");
		} else if(packet.rfind("vFile:open:2f70726f6772616d2e656c66", 0) == 0) {
			// open request for /program.elf
			client.send_packet("F0"); // return virtual file descriptor '0'
		} else if(std::regex_match(packet, smatch, std::regex("vFile:pread:0,([a-f0-9]+),([a-f0-9]+)"))) {
			// read only implemented for virtual file descriptor '0'
			size_t offset = 0;
			size_t size = 0;
			sscanf(smatch[1].str().c_str(), "%zx", &size);
			sscanf(smatch[2].str().c_str(), "%zx", &offset);
			fprintf(stderr, "read file descriptor=0, offset=%zx, size=%zx\n", offset, size);

			// this is highly innefficient as it will open the file each time
			std::ifstream file(_program->path(), std::ios::binary | std::ios::ate);
			size_t fsize = file.tellg();
			file.seekg(0, std::ios::beg);

			std::vector<char> elf(fsize);
			file.read(elf.data(), fsize);

			if(offset + size > fsize) {
				size = fsize - offset;
			}

			static uint8_t packet_buff[1024];
			size_t data_size = std::min(size, sizeof(packet_buff) - 14);
			size_t header_size = sprintf((char*)packet_buff, "F%zX;", data_size);
			memcpy(packet_buff + header_size, elf.data() + offset, data_size);
			client.send_packet_binary(packet_buff, header_size + data_size);

		}
	}

};



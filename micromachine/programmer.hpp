/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_PROGRAMMER_HPP
#define MICROMACHINE_EMU_PROGRAMMER_HPP

#include "memory/memory.hpp"
#include "elfio/elfio.hpp"

class programmer {
private:
	struct membuf : std::streambuf {
		membuf(char* begin, char* end) {
			this->setg(begin, begin, end);
		}
	};

public:

	class program {
	private:
		uint8_t* const _data;
		const  size_t _size;

	public:
		using ptr = std::shared_ptr<program>;
		using memory_chunk = std::vector<uint8_t>;

		program(size_t size)
			: _data((uint8_t*)calloc(1, size))
			, _size(size)
		{}

		memory_chunk& allocate_chunk(size_t size) {
			_memory.emplace_back(size, (uint8_t)0);
			return _memory.at(_memory.size()-1);
		}

		bool is_null() const {
			return _memory.empty();
		}

		uint32_t entry_point() const {
			return _entry_point;
		}
		void set_entry_point(uint32_t address) {
			_entry_point = address;
		}

		const std::string& path() {
			return _path;
		}

		void set_path(const std::string& path) {
			_path = path;
		}

		uint8_t* data() {
			return _data;
		}

		const uint8_t* data() const {
			return _data;
		}

		size_t size() const {
			return _size;
		}

		membuf memory_buffer() const {
			return membuf((char*)data(), (char*)data() + size());
		}

	private:
		std::vector<memory_chunk> _memory;
		uint32_t _entry_point = 0;
		std::string _path;
	};


	static program::ptr load_elf(const std::string &path, memory& mem) {

		std::ifstream ifs(path, std::ios::binary|std::ios::ate);
		if(!ifs) {
			fprintf(stderr, "Failed to open file path=%s\n", path.c_str());
			return nullptr;
		}

		std::ifstream::pos_type size = ifs.tellg();
		program::ptr prog = std::make_shared<program>(size);
		prog->set_path(path);

		ifs.seekg(0, std::ios::beg);
		ifs.read((char*)prog->data(), prog->size());

		//membuf memory = prog->memory_buffer();
		//std::istream memory_stream(&memory);
		std::istringstream ss;
		ss.rdbuf()->pubsetbuf((char*)prog->data(),prog->size());

		ELFIO::elfio elfReader;
		if (!elfReader.load(ss)) {
			return prog;
		}

		if (elfReader.get_class() != ELFCLASS32) {
			return prog;
		}

		if (elfReader.get_encoding() != ELFDATA2LSB) {
			return prog;
		}

		ELFIO::Elf_Half sec_num = elfReader.sections.size();

		uint32_t entryPoint = (uint32_t)elfReader.get_entry();
		fprintf(stderr, "PC will be set to %08X\n", entryPoint);

		prog->set_entry_point(entryPoint);

		//std::cout << "Number of sections: " << sec_num << std::endl;
		for (int i = 0; i < sec_num; ++i) {
			ELFIO::section *section = elfReader.sections[i];

			if(section->get_flags() & (SHF_ALLOC | SHF_EXECINSTR)) {

				if(0 == section->get_size()) {
					fprintf(stderr, "Skipping empty section %s\n", section->get_name().c_str());
					continue;
				}

				fprintf(stderr, "Creating memory section for %s\n", section->get_name().c_str());

				program::memory_chunk& chunk = prog->allocate_chunk(section->get_size());

				// should we copy data to this section ?
				if(section->get_type() & SHT_PROGBITS) {
					memcpy(chunk.data(), section->get_data(), section->get_size());
				}
				mem.map(chunk.data(), section->get_address(), section->get_size(), section->get_name());
			}
		}

		return prog;
	}


};

#endif //MICROMACHINE_EMU_PROGRAMMER_HPP

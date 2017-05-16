//
// Created by fla on 17-5-8.
//

#ifndef MICROMACHINE_TYPES_HPP
#define MICROMACHINE_TYPES_HPP

#include "integer_type.hpp"


using word 		= integer_type<uint32_t>;
using halfword 	= integer_type<uint16_t>;
using byte 		= integer_type<uint8_t>;

using imm11_t 	= uint16_t;
using imm8_t 	= uint8_t;
using imm7_t 	= uint8_t;
using imm5_t	= uint8_t;
using imm3_t	= uint8_t;
using register_list_t = uint16_t;
#endif //MICROMACHINE_TYPES_HPP

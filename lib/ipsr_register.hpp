/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_IPSR_REGISTER_HPP
#define MICROMACHINE_IPSR_REGISTER_HPP


struct ipsr_register : public word {


	/* The processor updates the IPSR on exception entry and exit. Software can use an MRS instruction to read the
		IPSR, but the processor ignores writes to the IPSR by an MSR instruction. The IPSR Exception Number field
		is defined as follows:
		• in Thread mode, the value is 0
		• in Handler mode, holds the exception number of the currently-executing exception, see Exception
		number definition on page B1-220 and The vector table on page B1-22
	*/

	using word::integer_type;


	ipsr_register()




};
#endif //MICROMACHINE_IPSR_REGISTER_HPP

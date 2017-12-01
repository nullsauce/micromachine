
set(SRCS
    binops.hpp
    types.hpp
    instruction_pair.hpp
    integer_type.hpp
    instructions.hpp
    precond.hpp
    exec.hpp
    alu.hpp
    registers/registers.hpp
    registers/xpsr_reg.hpp
    registers/apsr_reg.hpp
    registers/ipsr_reg.hpp
    registers/epsr_reg.hpp
    registers/exec_mode_reg.hpp
    registers/ireg.hpp
    registers/pc_reg.hpp
    registers/sp_reg.hpp
    registers/standard_reg.hpp
    registers/control_reg.hpp
    memory.hpp
    cpu.hpp
    exception_vector.hpp
    exception.hpp
    string_format.hpp
    exec_dispatcher.hpp
    dispatcher.hpp
    disasm.hpp
    cpu.hpp
)

add_library(micro INTERFACE)

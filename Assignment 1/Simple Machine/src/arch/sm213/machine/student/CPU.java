package arch.sm213.machine.student;

import arch.sm213.machine.AbstractSM213CPU;
import machine.AbstractMainMemory;
import machine.RegisterSet;
import util.UnsignedByte;


/**
 * The Simple Machine CPU.
 *
 * Simulate the execution of a single cycle of the Simple Machine SM213 CPU. 
 */

public class CPU extends AbstractSM213CPU {
  
  /**
   * Create a new CPU.
   *
   * @param name   fully-qualified name of CPU implementation.
   * @param memory main memory used by CPU.
   */
  public CPU (String name, AbstractMainMemory memory) {
    super (name, memory);
  }
  
  /**
   * Fetch Stage of CPU Cycle.
   * Fetch instruction at address stored in "pc" register from memory into instruction register 
   * and set "pc" to point to the next instruction to execute.
   *
   * Input register:   pc.
   * Output registers: pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt
   * @see AbstractSM213CPU for pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt
   *
   * @throws MainMemory.InvalidAddressException when program counter contains an invalid memory address.
   */
  @Override protected void fetch () throws MainMemory.InvalidAddressException {
    ;
  }
  
  /**
   * Execution Stage of CPU Cycle.
   * Execute instruction that was fetched by Fetch stage.
   *
   * Input state: pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt, reg, mem
   * Ouput state: pc, reg, mem
   * @see AbstractSM213CPU for pc, instruction, insOpCode, insOp0, insOp1, insOp2, insOpImm, insOpExt
   * @see MainMemory       for mem
   * @see machine.AbstractCPU      for reg
   *
   * @throws InvalidInstructionException                when instruction format is invalid.
   * @throws MachineHaltException                       when instruction is the HALT instruction.
   * @throws RegisterSet.InvalidRegisterNumberException when instruction references an invalid register (i.e, not 0-7).
   * @throws MainMemory.InvalidAddressException         when instruction references an invalid memory address.
   */
  @Override protected void execute () throws InvalidInstructionException, MachineHaltException, RegisterSet.InvalidRegisterNumberException, MainMemory.InvalidAddressException {
    ;
  }
}

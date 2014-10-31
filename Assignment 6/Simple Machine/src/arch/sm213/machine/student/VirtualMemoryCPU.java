package arch.sm213.machine.student;

import machine.AbstractMainMemory;

/**
 * Virtual Memory enabled CPU
 */

public class VirtualMemoryCPU extends CPU {
  final static int PAGE_SIZE          = 32;
  final static int PTE_VALID_BIT_MASK = 0x80000000;
  final static int PTE_PFN_MASK       = 0x7fffffff;
  
  public VirtualMemoryCPU (String name, AbstractMainMemory memory) {
    super (name, memory);
    enableVirtualMemory();
  }
  
  /**
   * Translate virtual address to physical address.
   *
   * @param va                                          virtual address
   * @return                                            physical address
   * @throws AbstractMainMemory.InvalidAddressException if virtual address is not valid (i.e, not mapped)
   */
  
  @Override public int translateAddress (int va) throws AbstractMainMemory.InvalidAddressException {
    throw new AbstractMainMemory.InvalidAddressException();
  }
}

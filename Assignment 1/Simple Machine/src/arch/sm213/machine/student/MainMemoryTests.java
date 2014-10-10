package arch.sm213.machine.student;

import static org.junit.Assert.*;

import java.util.Random;

import machine.AbstractMainMemory.InvalidAddressException;

import org.junit.Before;
import org.junit.Test;

public class MainMemoryTests {

    private static final int SIZE = 64;
    private MainMemory mem;

    @Before
    public void setUp() throws Exception {
	mem = new MainMemory(SIZE);
    }

    @Test (expected=InvalidAddressException.class)
    public void testGet() throws InvalidAddressException {
	byte[] bytes = new byte[SIZE];

	// test maximum access size
	new Random().nextBytes(bytes);
	mem.set(0, bytes);
	assertArrayEquals(bytes, mem.get(0, SIZE));

	// test random insertion and access for size 2
	mem.set(12,  new byte[] {(byte) 0xCA, (byte) 0xFE});
	assertArrayEquals(
		new byte[] {(byte) 0xCA, (byte) 0xFE},
		mem.get(12, 2));

	// truly random access for size 4
	int rand = new Random().nextInt(SIZE - 4);
	bytes = new byte[4];
	new Random().nextBytes(bytes);
	mem.set(rand, bytes);
	assertArrayEquals(bytes, mem.get(rand,  4));

	// test end of memory
	bytes = new byte[2];
	new Random().nextBytes(bytes);
	mem.set(SIZE-2, bytes);
	assertArrayEquals(bytes, mem.get(SIZE-2, 2));

	// test out of bounds
	try {
	    mem.get(SIZE + 2, 2);
	}
	catch (InvalidAddressException e) {
	    mem.get(-2, 4);
	}
    }

    @Test (expected=InvalidAddressException.class)
    public void testSet() throws InvalidAddressException {
	// test set start of memory, should not throw exception
	mem.set(0, new byte[] {(byte) 0x69, (byte) 0xAF});
	assertEquals((byte) 0x69, mem.get(0, 1)[0]);
	
	// test set random value, should not throw exception
	mem.set(32, new byte[] {(byte) 0xFA, (byte) 0xCE});
	mem.set(34, new byte[] {(byte) 0xB0, (byte) 0x0C});
	assertArrayEquals(
		new byte[] {(byte) 0xFA, (byte) 0xCE, (byte) 0xB0, (byte) 0x0C},
		mem.get(32, 4));
	
	// test set end of memory, should not throw exception
	mem.set(SIZE - 2, new byte[] {(byte) 0xAC, (byte) 0xE5});
	assertArrayEquals(new byte[] {(byte) 0xAC, (byte) 0xE5},
		mem.get(SIZE - 2,  2));
	
	// test out of bounds
	try {
	    mem.set(-1, new byte[] {(byte) 0x69});
	}
	catch (InvalidAddressException e) {
	    mem.set(SIZE + 2, new byte[] {(byte) 0x96});
	}
    }

    @Test
    public void testIsAccessAligned() {
	// test addresses that are aligned to corresponding memory length
	assertTrue(mem.isAccessAligned(0, 2));		// short
	assertTrue(mem.isAccessAligned(8, 4));		// integer
	assertTrue(mem.isAccessAligned(32, 8));		// long

	// test addresses that are not aligned properly
	assertFalse(mem.isAccessAligned(1, 2));		// short
	assertFalse(mem.isAccessAligned(34, 4));	// integer
	assertFalse(mem.isAccessAligned(20, 8));	// long
    }

    @Test
    public void testBytesToInteger() {
	// test for 0x00000000 from | 0x00 | 0x00 | 0x00 | 0x00 | (big-endian)
	// to ensure that the minimum value works
	assertEquals(0, mem.bytesToInteger(
		(byte) 0, (byte) 0, (byte) 0, (byte) 0));

	// test for 0x5BA372C3 from | 0x5B | 0xA3 | 0x72 | 0xC3 | (big-endian)
	// to ensure that a random value works
	assertEquals(0x5BA372C3, mem.bytesToInteger(
		(byte) 0x5B, (byte) 0xA3, (byte) 0x72, (byte) 0xC3));

	// test for 0xD15EA5E5 from | 0xD1 | 0x5E | 0xA5 | 0xE5 | (big-endian)
	// to ensure that another random value works
	assertEquals(0xD15EA5E5, mem.bytesToInteger(
		(byte) 0xD1, (byte) 0x5E, (byte) 0xA5, (byte) 0xE5));

	// test for 0xFFFFFFFF from | 0xFF | 0xFF | 0xFF | 0xFF | (big-endian)
	// to ensure that the maximum value works
	assertEquals(0xFFFFFFFF, mem.bytesToInteger(
		(byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF));
    }

    @Test
    public void testIntegerToBytes() {
	byte[] bytes;
	// test min value
	bytes = new byte[] {0, 0, 0, 0};
	assertArrayEquals(bytes, mem.integerToBytes(0));

	// test random value
	bytes = new byte[] {(byte) 0xF0, (byte) 0x05, (byte) 0xBA, (byte) 0x11};
	assertArrayEquals(bytes, mem.integerToBytes(0xF005BA11));

	// test another random value
	bytes = new byte[] {(byte) 0xA4, (byte) 0xAB, (byte) 0xE1, (byte) 0x1E};
	assertArrayEquals(bytes, mem.integerToBytes(0xA4ABE11E));

	// test max value
	bytes = new byte[] {(byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF};
	assertArrayEquals(bytes, mem.integerToBytes(0xFFFFFFFF));
    }

    @Test
    public void testLength() {
	// test current size
	assertEquals(SIZE, mem.length());

	// test another size
	mem = new MainMemory(32);
	assertEquals(32, mem.length());

	// test yet another size
	mem = new MainMemory(0);
	assertEquals(0, mem.length());
    }
}

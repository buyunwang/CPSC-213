#ifndef __disk_h__
#define __disk_h__

void disk_scheduleRead (char* buf, int nBytes, int blockNo);
void disk_waitForReads ();
void disk_start        (void (*interruptServiceRoutine) ());

#endif
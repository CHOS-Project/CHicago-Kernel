// File author is Ítalo Lima Marconato Matias
//
// Created on July 15 of 2018, at 13:19 BRT
// Last edited on January 18 of 2020, at 15:58 BRT

#include <chicago/debug.h>
#include <chicago/device.h>
#include <chicago/panic.h>
#include <chicago/string.h>

Status ZeroDeviceRead(PDevice dev, UIntPtr off, UIntPtr len, PUInt8 buf, PUIntPtr read) {
	(Void)dev; (Void)off;														// Avoid compiler's unused parameter warning
	StrSetMemory(buf, 0, len);													// Just fill the buffer with zeroes (this is what Zero device does)
	*read = len;
	return STATUS_SUCCESS;
}

Status ZeroDeviceWrite(PDevice dev, UIntPtr off, UIntPtr len, PUInt8 buf, PUIntPtr write) {
	(Void)dev; (Void)off; (Void)buf;											// Avoid compiler's unused parameter warning
	*write = len;
	return STATUS_SUCCESS;
}

Void ZeroDeviceInit(Void) {
	if (!FsAddDevice(L"Zero", Null, ZeroDeviceRead, ZeroDeviceWrite, Null)) {	// Let's add ourself
		DbgWriteFormated("PANIC! Failed to add the Zero device\r\n");			// Failed...
		Panic(PANIC_KERNEL_INIT_FAILED);
	}
}

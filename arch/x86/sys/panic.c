// File author is Ítalo Lima Marconato Matias
//
// Created on October 27 of 2018, at 21:48 BRT
// Last edited on March 01 of 2019, at 18:07 BRT

#include <chicago/arch/registers.h>

#include <chicago/arch.h>
#include <chicago/console.h>
#include <chicago/display.h>
#include <chicago/nls.h>
#include <chicago/panic.h>
#include <chicago/process.h>

Void ArchPanicWriteHex(UInt32 val) {
	if (val < 0x10) {
		ConWriteFormated(L"0x0000000%x", val);
	} else if (val < 0x100) {
		ConWriteFormated(L"0x000000%x", val);
	} else if (val < 0x1000) {
		ConWriteFormated(L"0x00000%x", val);
	} else if (val < 0x10000) {
		ConWriteFormated(L"0x0000%x", val);
	} else if (val < 0x100000) {
		ConWriteFormated(L"0x000%x", val);
	} else if (val < 0x1000000) {
		ConWriteFormated(L"0x00%x", val);
	} else if (val < 0x10000000) {
		ConWriteFormated(L"0x0%x", val);
	} else {
		ConWriteFormated(L"0x%x", val);
	}
}

Void ArchPanic(UInt32 err, PVoid priv) {
	if (PsCurrentThread != Null) {																				// Tasking initialized?
		if (!((PsCurrentThread->id == 0) && (PsCurrentProcess->id == 0))) {										// Yes, this is the main kernel process?
			ConAcquireLock();																					// Nope, we don't want a dead lock, right?
			
			if (ConGetCursorX() != 0) {																			// Print the error to the screen
				ConWriteFormated(L"\r\n%s", NlsGetMessage(NLS_SEGFAULT));
			} else {
				ConWriteFormated(NlsGetMessage(NLS_SEGFAULT));
			}
			
			PsExitProcess(1);																					// And just PsExitProcess()
		}
	}
	
	PsLockTaskSwitch(old);																						// Lock
	ConAcquireLock();																							// We don't want a dead lock, right?
	ConSetRefresh(False);																						// Disable the automatic screen refresh
	PanicInt(err, False);																						// Print the "Sorry" message
	
	UInt32 cr2 = 0;																								// Get the CR2
	Asm Volatile("mov %%cr2, %0" : "=r"(cr2));
	
	PRegisters regs = (PRegisters)priv;																			// Cast the priv into the PRegisters struct
	
	ConWriteFormated(L"| EAX: "); ArchPanicWriteHex(regs->eax); ConWriteFormated(L" | ");						// Print the registers
	ConWriteFormated(L"EBX: "); ArchPanicWriteHex(regs->ebx); ConWriteFormated(L" | ");
	ConWriteFormated(L"ECX:    "); ArchPanicWriteHex(regs->ecx); ConWriteFormated(L" | ");
	ConWriteFormated(L"EDX: "); ArchPanicWriteHex(regs->edx); ConWriteFormated(L" |\r\n");
	
	ConWriteFormated(L"| ESI: "); ArchPanicWriteHex(regs->esi); ConWriteFormated(L" | ");
	ConWriteFormated(L"EDI: "); ArchPanicWriteHex(regs->edi); ConWriteFormated(L" | ");
	ConWriteFormated(L"ESP:    "); ArchPanicWriteHex(regs->esp); ConWriteFormated(L" | ");
	ConWriteFormated(L"EBP: "); ArchPanicWriteHex(regs->ebp); ConWriteFormated(L" |\r\n");
	
	ConWriteFormated(L"| EIP: "); ArchPanicWriteHex(regs->eip); ConWriteFormated(L" | ");
	ConWriteFormated(L"CR2: "); ArchPanicWriteHex(cr2); ConWriteFormated(L" | ");
	ConWriteFormated(L"EFLAGS: "); ArchPanicWriteHex(regs->eflags); ConWriteFormated(L" | ");
	ConWriteFormated(L"                |\r\n");
	
	ConWriteFormated(L"| CS:  "); ArchPanicWriteHex((UInt8)regs->cs); ConWriteFormated(L" | ");
	ConWriteFormated(L"DS:  "); ArchPanicWriteHex((UInt8)regs->ds); ConWriteFormated(L" | ");
	ConWriteFormated(L"ES:     "); ArchPanicWriteHex((UInt8)regs->es); ConWriteFormated(L" | ");
	ConWriteFormated(L"FS:  "); ArchPanicWriteHex((UInt8)regs->fs); ConWriteFormated(L" |\r\n");
	
	ConWriteFormated(L"| GS:  "); ArchPanicWriteHex((UInt8)regs->gs); ConWriteFormated(L" | ");
	ConWriteFormated(L"SS:  "); ArchPanicWriteHex((UInt8)regs->ss); ConWriteFormated(L" | ");
	ConWriteFormated(L"                   | ");
	ConWriteFormated(L"                |\r\n");
	
	PanicInt(err, True);																						// Print the error code
	DispRefresh();																								// Refresh the screen
	ArchHalt();																									// Halt
}

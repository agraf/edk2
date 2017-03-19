//
// Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

#include <Protocol/Cpu.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/DebugSupport.h>
#include <Protocol/X64InterpreterProtocol.h>

#ifdef MDE_CPU_AARCH64
#define X86_EMU_EXCEPTION_TYPE    EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS
#else
#error
#endif

typedef struct {
  LIST_ENTRY            Link;
  EFI_PHYSICAL_ADDRESS  ImageBase;
  UINT64                ImageSize;
} X86_IMAGE_RECORD;

VOID
EFIAPI
X86InterpreterSyncExceptionCallback (
  IN     EFI_EXCEPTION_TYPE   ExceptionType,
  IN OUT EFI_SYSTEM_CONTEXT   SystemContext
  );

X86_IMAGE_RECORD*
EFIAPI
FindImageRecord (
  IN  EFI_PHYSICAL_ADDRESS    Address
  );

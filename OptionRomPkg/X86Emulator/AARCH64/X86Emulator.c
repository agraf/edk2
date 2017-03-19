//
// Copyright (c) 2017, Linaro, Ltd. <ard.biesheuvel@linaro.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//

#include "X86Emulator.h"

#include <Library/DefaultExceptionHandlerLib.h>

extern CONST UINT64 X86EmulatorThunk[];

VOID
EFIAPI
X86InterpreterSyncExceptionCallback (
  IN     EFI_EXCEPTION_TYPE   ExceptionType,
  IN OUT EFI_SYSTEM_CONTEXT   SystemContext
  )
{
  EFI_SYSTEM_CONTEXT_AARCH64  *AArch64Context;
  X86_IMAGE_RECORD            *Record;
  UINTN                       Ec;

  AArch64Context = SystemContext.SystemContextAArch64;

  // instruction permission faults or PC misalignment faults only
  Ec = AArch64Context->ESR >> 26;
  if ((Ec == 0x21 && (AArch64Context->ESR & 0x3c) == 0xc) || Ec == 0x22) {
    Record = FindImageRecord (AArch64Context->ELR);
    if (Record != NULL) {
      AArch64Context->X16 = AArch64Context->ELR;
      AArch64Context->X17 = (UINT64)Record;
      AArch64Context->ELR = (UINT64)X86EmulatorThunk;
      return;
    }
  }
  DefaultExceptionHandler (ExceptionType, SystemContext);
}

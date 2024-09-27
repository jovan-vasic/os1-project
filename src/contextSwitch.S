.global _ZN3TCB13contextSwitchEPNS_7ContextES1_

_ZN3TCB13contextSwitchEPNS_7ContextES1_:
    # Save callee-saved registers to the old context
    sd ra, 0 * 8(a0)
    sd sp, 1 * 8(a0)

    # Load callee-saved registers from the new context
    ld ra, 0 * 8(a1)
    ld sp, 1 * 8(a1)

    ret
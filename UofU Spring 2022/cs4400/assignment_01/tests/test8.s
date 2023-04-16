    SECTION "code", ROM0
    ; call, ret

    ld a, $29
    ld b, $33
    add a, b
    ld [$400], a
    ;; treat the last add as if it were adding 2 2-digit decimal numbers (BCD)
    ;; daa uses the half carry flag to do so, so this check to make sure you set it correctly
    daa
    ld [$400], a
    cp a, $62
    jp z, ok
    jp failtest
ok:

    ld a, $24
    add a, a
    ld [$400], a
    daa
    ld [$400], a
    cp a, $48
    jp z, ok2
    jp failtest
ok2:

    ld a, $25
    ld b, $6
    sub a, b
    ld [$400], a
    daa
    ld [$400], a
    cp a, $19
    jp z, ok3
    jp failtest
ok3:
    sub a, $6
    ld [$400], a
    daa
    ld [$400], a
    cp a, $13
    jp z, ok4
    jp failtest
ok4:

    jp $400
failtest:
    halt

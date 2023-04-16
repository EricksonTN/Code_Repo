    SECTION "code", ROM0
    ; push, pop
    ld sp, $3FF

    ld bc, $1234
    push bc
    inc b
    inc c
    push bc
    inc b
    inc c
    push bc

    ld hl, sp+0                 ;copy the stack pointer to the hl register
    ld a, h
    ld [$400], a
    cp a, $3
    jp z, ok1
    jp failtest
ok1:
    ld a, l
    ld [$400], a
    cp a, $F9
    jp z, ok2
    jp failtest
ok2:
    ld a, [hl]
    ld [$400], a
    cp a, $36
    jp z, ok3
    jp failtest
ok3:

    pop de
    ld a, e
    ld [$400], a
    cp a, $36
    jp z, ok4
    jp failtest
ok4:
    pop de
    ld a, d
    ld [$400], a
    cp a, $13
    jp z, ok5
    jp failtest
ok5:
    pop de
    ld a, e
    ld [$400], a
    cp a, $34
    jp z, ok6
    jp failtest
ok6:

    jp $400
failtest:
    halt

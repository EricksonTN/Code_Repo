    SECTION "code", ROM0
    ;; conditional jumps
    ;; note these tests also exercise the flags of add/inc


    ld a, $24
    cp a, $24

    jp z, then1
    jp failtest

then1:

    add a, $DB
    jp c, failtest

    add a, $1
    jp c, then2
    jp failtest

then2:


    ld a, $FA
    ld b, $0
loop:
    inc b
    inc a
    jp z, done
    jr loop
done:
    ld a, b
    ld [$400], a


    jp $400
failtest:
    halt

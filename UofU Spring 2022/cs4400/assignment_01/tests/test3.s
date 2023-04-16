    SECTION " code", ROM0

    ld a, $66
    add a, $24
    ld [$400], a
    add a, $24
    ld [$400], a
    add a, $24
    ld [$400], a
    add a, $24
    ld [$400], a
    add a, $24
    ld [$400], a

    sub a, $24
    ld [$400], a
    sub a, $24
    ld [$400], a
    sub a, $24
    ld [$400], a
    sub a, $24
    ld [$400], a



    jp $400
    halt

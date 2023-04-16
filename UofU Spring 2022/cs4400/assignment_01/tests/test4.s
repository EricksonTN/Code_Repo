    SECTION " code", ROM0
    ld a, $36
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a
    rra
    ld [$400], a

    jp $400
    halt

    SECTION " code", ROM0
    ld a, $36
    ld [$400], a

    ld hl, $100                 ; implemented by instructor
    inc a
    ld [hl+], a
    inc a
    ld [hl+], a
    ld a, [hl-]
    ld [$400], a
    ld a, [hl-]
    ld [$400], a
    ld a, [hl-]
    ld [$400], a
    ld a, l                     ; implemented by instructor
    ld [$400], a

    ld a, $CD
    ld [hl-], a
    inc a
    ld [hl-], a
    inc a
    ld [hl-], a
    ld a, [hl+]
    ld [$400], a
    ld a, [hl+]
    ld [$400], a
    ld a, [hl+]
    ld [$400], a

    jp $400
    halt

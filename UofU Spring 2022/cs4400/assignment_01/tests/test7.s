    SECTION "code", ROM0
    ; call, ret

    ld sp, $3FF

    ld b, $6
    ld a, $0
    call func
    ld [$400], a

    jp $400
    halt


func:

    add a, b
    ld c, a
    ld a, b
    cp a, 0
    jp z, done
    ld a, c
    dec b
    call func
done:
    ld a, c
    ret

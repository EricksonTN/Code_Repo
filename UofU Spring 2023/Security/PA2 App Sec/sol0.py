import sys
from struct import pack
from shellcode import shellcode

# Implement your attack here!
payload = b'u1117838\0 A+'

# Launch the attack!
sys.stdout.buffer.write(payload)

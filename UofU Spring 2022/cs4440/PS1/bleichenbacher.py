from roots import *
import hashlib
import sys
import base64
message = sys.argv[1]

tmp = "0001FF003021300906052B0E03021A05000414"

e = 3
ending = "FF" * 217
m = hashlib.sha1()
m.update(message.encode())
msg = m.hexdigest()
sig = tmp + msg + ending

forged_signature = integer_nthroot(int(sig, 16), 3)[0]


print(integer_to_base64(forged_signature))

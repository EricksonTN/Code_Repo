#!/usr/bin/env python3
# coding: iso-8859-1

MSG = bytes(r"""� ϖ-!�h5�_�&�O�
��CC���Y�+��������T�_�����h�a�Wp���c?�n`jM��H���A��=�f6ab"��a���]ZY"�0��م�a���v{����Ⱥ��?N?`��h   
""", "iso-8859-1")
from hashlib import sha256
print(sha256(MSG).hexdigest())

if sha256(MSG).hexdigest() == "6f1e62663238fe1339bd3349aaaa125edb9dd00cb744e2e9d78053d423e6d729":
	print("I come in peace.")
else:
	print("Prepare to be destroyed!")

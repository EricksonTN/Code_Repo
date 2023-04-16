#!/usr/bin/env python3
# coding: iso-8859-1

MSG = bytes(r"""œ Ï–-!¶h5¯_í&ÕOÛ
¤¼CC’€ÍY°+îÔáïôîåÝTï_æÙÒÿÄh•aÍWp”õÉc?Ÿn`jMà…Hó†ÓAýÅ=…f6ab"õàaÑöÄ]ZY"Ã0×áÙ…ða°Ìï”v{¬ÁøÆÈº÷Ÿ?N?`¶»h   
""", "iso-8859-1")
from hashlib import sha256
print(sha256(MSG).hexdigest())

if sha256(MSG).hexdigest() == "6f1e62663238fe1339bd3349aaaa125edb9dd00cb744e2e9d78053d423e6d729":
	print("I come in peace.")
else:
	print("Prepare to be destroyed!")

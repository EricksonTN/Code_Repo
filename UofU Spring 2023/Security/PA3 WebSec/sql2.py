import hashlib
import random

target = b"'=0#"
test = "test"

while target not in hashlib.md5(bytes(test, 'utf-8')).digest():
	test = str(random.randint(1, 9999999999999999))
	
print (test)

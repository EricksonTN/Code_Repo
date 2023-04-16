import http.client as httplib
from urllib.parse import urlparse, quote
import sys, re
from pymd5 import md5, padding
url = sys.argv[1]

PASS = 8
command3 = "&command3=UnlockAllSafes"

token = url.split('=')[1].split('&')[0]
msg = url.split(token)[1][1:]

m = PASS + len(msg)
bits = (m + len(padding(m*8)))*8

h = md5(state=token, count=bits)

h.update(command3)
new = h.hexdigest()
padMsg = padding(m*8)
padMsgQuote = quote(padMsg)

parsedOldUrl = urlparse(url)
newUrl = f"http://cs4440.eng.utah.edu{parsedOldUrl.path}?token={new}&{msg}{padMsgQuote}{command3}"
parsedUrl = urlparse(newUrl)

conn = httplib.HTTPConnection(parsedUrl.hostname,parsedUrl.port)
conn.request("GET", parsedUrl.path + "?" + parsedUrl.query)
print(conn.getresponse().read())

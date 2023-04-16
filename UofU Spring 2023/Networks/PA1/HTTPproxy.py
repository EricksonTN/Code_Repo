# Place your imports here
import signal
import socket
import threading
import time
import sys
from optparse import OptionParser

# Global Variables
blockList = []               #string array
cache = {}                   #Dictionary [URI: (Date Accessed, Origin Server Response)]
blockIsEnabled = False
cacheIsEnabled = False


# Signal handler for pressing ctrl-c
def ctrl_c_pressed(signal, frame):
	sys.exit(0)

# TODO: Put function definitions here

#Take the message from the client and parse the HTTP/1.0 request
def handleReceive(clientSocket, clientAddress):
    global cache
    global cacheIsEnabled

    unmodURI = ''                                    #unmodified URI
    request = ''
    while True:
        #Receive everything from client until double carriage return EOF
        tmp = clientSocket.recv(1024).decode()
        request += tmp
        if ('\r\n\r\n' in request):
            break
    
    #Split the request by carriage returns
    parsedReq = request.split('\r\n')
    
    #delete two last entries that are empty strings caused by double carriage return
    del parsedReq[len(parsedReq) - 1]
    del parsedReq[len(parsedReq) - 1]
    checkedReq =[]                                    #Array holding all values in HTTP request from client

    #check that the string segments are valid
    for line in range(len(parsedReq)):

        #Split the current line based on whitespaces
        lineSplit = parsedReq[line].split()
        #handle the first line of the request
        if (line == 0):
            
            #checks that the first request line has the correct number of items
            if (len(lineSplit) != 3):
                clientSocket.send(formError(400).encode())
                clientSocket.close()
                return

            #Checks method
            elif (lineSplit[0] != 'GET'):
                clientSocket.send(formError(501).encode())
                clientSocket.close()
                return                

            ##uri check section
            #check what kind of uri it is
            
            #check that uri starts with http://
            elif (not lineSplit[1].startswith('http://')):
                if (not ':' in lineSplit[1]):
                    clientSocket.send(formError(400).encode())
                    clientSocket.close()
                    return

            #check the uri has a path
            tempURI = lineSplit[1].replace('http://', '')
            if (tempURI.count('/') == 0):
                if (tempURI.count(':') == 1):
                    pass
                else:    
                    clientSocket.send(formError(400).encode())
                    clientSocket.close()
                    return

            ##check the HTTP is of the right type
            if (lineSplit[2] != 'HTTP/1.0'):
                clientSocket.send(formError(400).encode())
                clientSocket.close()
                return
            unmodURI = lineSplit[1]
            checkedReq += lineSplit

        #handle all subsequent headers
        else:

            #checks that the header line has the correct number of items
            headSplit = parsedReq[line].split(' ', 1)
            if (not ':' in parsedReq[line]):
                clientSocket.send(formError(400).encode())
                clientSocket.close()
                return

            #checks that the header name has a colon right after it
            if (not headSplit[0].endswith(':')):
                clientSocket.send(formError(400).encode())
                clientSocket.close()
                return

            #checks for 'connection: close' header in request
            if (headSplit[0] == 'Connection:'):
                headSplit[1] = 'close'
                newConHead = headSplit[0] + ' ' + headSplit[1]
                checkedReq.append(newConHead)
                continue

            checkedReq.append(parsedReq[line])

    #format the request to send to the origin server
    cleanUrl = urlClean(checkedReq[1])

    #updates the URI with cleaned version
    checkedReq[1] = cleanUrl[0]

    #Attaches '/' to 'GET' string
    checkedReq[0] += ' /'

    # If there is a path, we concat it to the 'GET' string
    if ((type(cleanUrl[1]) == str)):

        ##Do any cache/blocklist commands if any
        #If it's a cache/blocklist command, send a 200 OK and return.
        if(cacheBlockSettings(cleanUrl[1])):
            clientSocket.send('HTTP/1.0 200 OK\r\n\r\n'.encode())
            clientSocket.close()
            return
        #Otherwise concat path to the 'GET' string
        checkedReq[0] += cleanUrl[1]            


    if (blockIsEnabled):
        #Checks to see if the host value is forbidden, if so send 403 Forbidden Error
        isForbidden = checkblockList(unmodURI)
        if (isForbidden):
            clientSocket.send(formError(403).encode())
            clientSocket.close()
            return
        
    #Builds the HTTP request to be sent to the origin server
    sendReq = messBuild(checkedReq, unmodURI)
    originMessage = ''                                          # Message from the origin server
    currTime = time.time()                                      # Current Time when message is sent
    
    #handles cases with port numbers
    if (len(cleanUrl) == 3):
        originMessage = handleSend(sendReq, checkedReq[1], int(cleanUrl[2]))
    #handles cases w/out port numbers
    else:
        originMessage = handleSend(sendReq, checkedReq[1], cleanUrl[1])
    #Use the cache if cacheIsEnabled = True
    if (cacheIsEnabled):
        
        #There exists a version of the request in the cache
        if (checkCache(unmodURI)):
            #If the cache holds most up to date version, grab from cache and set originMessage to cache item
            if ('304 Not Modified' in originMessage.decode()):
                originMessage = cache[unmodURI][1]

            #if the cache doesn't hold most up to date version, update version in cache and the time. send returned value from origin server to client
            else:

                #Update the last modified time.
                cache[unmodURI][0] = getTime(currTime)

                #update the content in cache
                cache[unmodURI][1] = originMessage

        # There doesn't exist a version of the request in the cache
        else:
            #send request to origin server and store returned value and time sent into cache
            cache[unmodURI] = (getTime(currTime), originMessage)

    #send entire request from origin server back to the client who requested it.
    clientSocket.sendall(originMessage)
    clientSocket.close()
        
           
#Handles sending the HTTP request to the origin server
def handleSend(message, serverName, serverPort):
    #checks if there is a port number or not
    if ((type(serverPort) == str) or (serverPort == None)):
        serverPort = 80


    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.connect((serverName, serverPort))
    serverMsg = b''
    clientSocket.send(message.encode())
    #Grabs everything sent back from the origin server and stores it in serverMsg
    while True:
        currReceive = clientSocket.recv(1024)
        if (currReceive == b''):
            break

        serverMsg += currReceive

    #close the socket once done receiving.
    clientSocket.close()
    return serverMsg

#Forms the error based on the error number and return the correctly formatted string
def formError(errNum):

    if (errNum == 400):
        return 'HTTP/1.0 400 Bad Request\r\n\r\n'
    elif (errNum == 501):
        return 'HTTP/1.0 501 Not Implemented\r\n\r\n'
    elif (errNum == 403):
        return 'HTTP/1.0 403 Forbidden\r\n\r\n'

#Cleans the URL of any unnecessary substrings
def urlClean(url):
    retUrl = ''
    port = None
    #removes the http:// from url, and split the resultant string by its hostname and path
    if ('http://' in url):
        url01 = url.replace('http://', '')
        retUrl = url01.split('/', 1)

        #Detects if there's a port, if so sets the port to be sent back
        if (retUrl[0].count(':') == 1):
            splitUrl = retUrl[0].split(':')
            port = splitUrl[1]
            retUrl[0] = splitUrl[0]
            retUrl.append(port)
        return retUrl
    
    #Handles URIs that are IP addresses
    else:
        url01 = url.split(':')
        retUrl = url01[0]
        port = int(url01[1])
        return (retUrl, port)
    
#Builds the HTTP request to be sent to the origin server
def messBuild (strList, unmodURL):
    global cacheIsEnabled
    global cache

    returnStrArr = []
    returnStr = ""

    for line in range(len(strList)):
        seg = strList[line]
        if (seg == 'HTTP/1.0'):
            continue

        if ('GET' in seg):
            returnStr += (seg + ' HTTP/1.0\r\n')
            continue
        
        #uri line
        if (line == 1):
            returnStr += 'Host: ' + seg + '\r\n'
            continue


        else:
            returnStr += seg + '\r\n'
            continue
    
    # Handles cache work if cache is enabled. If item exists in the cache, add a 'If-Modified-Since' header to the message
    if (cacheIsEnabled):
        if (checkCache(unmodURL)):
            returnStr += 'If-Modified-Since: ' + cache[unmodURL][0] + '\r\n'

    if ('Connection' in returnStr):
        returnStr += '\r\n'

    #adds a "Connection: close" line to the header section if not already existing
    else:
        returnStr += 'Connection: close\r\n\r\n'

    return returnStr

## handles absolute paths to modify the cache or blocklist
def cacheBlockSettings (path):
    global cacheIsEnabled
    global blockIsEnabled
    global cache
    global blockList

    lock = threading.Lock()
    isCommand = False
    cmds = path.split('/')

    #Code block that handles the cache commands
    if 'proxy/cache' in path:
        if path == 'proxy/cache/enable':
            #if cache isn't enabled, enable it
            if (not cacheIsEnabled):
                cacheIsEnabled = True
            
            isCommand = True

        elif path == 'proxy/cache/disable':
            #if cache is enabled, disable it
            if (cacheIsEnabled):
                cacheIsEnabled = False
            
            isCommand = True
        
        #clears cache
        elif path == 'proxy/cache/flush':
            lock.acquire(True, -1)
            cache.clear()
            lock.release()
            isCommand = True

    #code block that handles the blocklist commands
    elif 'proxy/blocklist' in path:
        if path == 'proxy/blocklist/enable':
            #if blocklist is disabled, enable it
            if (not blockIsEnabled):
                blockIsEnabled = True
            
            isCommand = True

        elif path == 'proxy/blocklist/disable':
            #if blocklist is enabled, disable it
            if (blockIsEnabled):
                blockIsEnabled = False
            
            isCommand = True

        #clears blocklist
        elif path == 'proxy/blocklist/flush':
            lock.acquire(True, -1)
            blockList.clear()
            lock.release()
            
            isCommand = True

        #adds specified value to blocklist
        elif 'proxy/blocklist/add/' in path:
            lock.acquire(True, -1)
            blockList.append(cmds[len(cmds) - 1])
            lock.release()
            
            isCommand = True

        #remove specified value from blocklist
        elif 'proxy/blocklist/remove/' in path:
            lock.acquire(True, -1)
            blockList.remove(cmds[len(cmds) - 1])
            lock.release()
            
            isCommand = True

    return isCommand

#checks the blocklist for any substring that exists in the url
def checkblockList (url):
    global blockList

    for item in blockList:
        if (item in url):
            return True

    return False

#checks to see if the url exists in the cache
def checkCache (url):
    global cache

    if url in cache:
        return True
    
    return False

#Formats the raw time input into format that HTTP head 'If-Modified-Since' requires
def getTime (timestamp):
    return time.strftime('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(timestamp))

# Start of program execution
# Parse out the command line server address and port number to listen to
parser = OptionParser()
parser.add_option('-p', type='int', dest='serverPort')
parser.add_option('-a', type='string', dest='serverAddress')
(options, args) = parser.parse_args()

port = options.serverPort
address = options.serverAddress
if address is None:
    address = 'localhost'
if port is None:
    port = 2100

# Set up signal handling (ctrl-c)
signal.signal(signal.SIGINT, ctrl_c_pressed)

# TODO: Set up sockets to receive requests
listeningSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listeningSock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

listeningSock.bind((address, port))

listeningSock.listen()
print("Server is ready to receive")


# IMPORTANT!
# Immediately after you create your proxy's listening socket add
# the following code (where "skt" is the name of the socket here):
# skt.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# Without this code the autograder may cause some tests to fail
# spuriously.

while True:
    #accept and handle connections
    clientSock, clientAddr = listeningSock.accept()
    threading.Thread(target=handleReceive, args=(clientSock, clientAddr)).start()
    

listeningSock.close()
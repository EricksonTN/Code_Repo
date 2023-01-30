import sys

def popFreq (data, meanVal, N):
    returnSet = []
    for value in data:
        v = pow((value[1] - meanVal), 2)
        returnSet.append(v)

    summedTotal = 0
    for i in returnSet:
        summedTotal += i
    
    return summedTotal / N



englishFreq = { "A": .08167, "B": .01492, "C": .02782, "D": .04253, "E": .12702, "F": .02228,
"G": .02015, "H": .06094, "I": .06966, "J": .00153, "K": .00772, "L": .04025,
"M": .02406, "N": .06749, "O": .07507, "P": .01929, "Q": .00095, "R": .05987,
"S": .06327, "T": .09056, "U": .02758, "V": .00978, "W": .02360, "X": .00150,
"Y": .01974, "Z": .00074 }

summ = []
mu = 0.384615385
for val in englishFreq.values():
    v = pow((val - mu), 2)
    summ.append(v)

summedVal = 0
for v in summ:
    summedVal += v

#print(summedVal)

Q2_1 = summedVal / 26
#print("{:.4E}".format(Q2_1))


#############################################################################################

def relativeFreq(x):
    freqs = [(value, x.count(value) / len(x)) for value in set(x)]
    return freqs


plaintext = "ethicslawanduniversitypolicieswarningtodefendasystemyouneedtobeabletothinklikeanattackerandthatincludesunderstandingtechniquesthatcanbeusedtocompromisesecurityhoweverusingthosetechniquesintherealworldmayviolatethelawortheuniversitysrulesanditmaybeunethicalundersomecircumstancesevenprobingforweaknessesmayresultinseverepenaltiesuptoandincludingexpulsioncivilfinesandjailtimeourpolicyincsfortyfourfortyisthatyoumustrespecttheprivacyandpropertyrightsofothersatalltimesorelseyouwillfailthecourseactinglawfullyandethicallyisyourresponsibilitycarefullyreadthecomputerfraudandabuseactcfaaoneofseveralfederallawsthatbroadlycriminalizescomputerintrusioniehackingunderstandwhatthelawprohibitsifindoubtwecanreferyoutoanattorneypleasereviewtheuniversitysacceptableusepolicyconcerningproperuseofinformationtechnologyaswellasthestudentcodeasmembersofthe"

ret = relativeFreq(plaintext)
#print(ret)
sumVal = 0
for i in ret:
    sumVal += i[1]

mean = sumVal / 26

ptSumm = []

for val in ret:
    v = pow((val[1] - mean), 2)
    ptSumm.append(v)

sm = 0
for vl in ptSumm:
    sm += vl

Q2_2 = sm / 26
#print("{:.4E}".format(Q2_2))

#############################################################################################

alphabet = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"]

#keys

yz = [('y', 24), ('z', 25)]

xyz = [('x', 23), ('y', 24), ('z', 25)]

wxyz = [('w', 22), ('x', 23), ('y', 24), ('z', 25)]

vwxyz = [('v', 21), ('w', 22), ('x', 23), ('y', 24), ('z', 25)]

uvwxyz = [('u', 20), ('v', 21), ('w', 22), ('x', 23), ('y', 24), ('z', 25)]


ptList = []

for x in range(0, len(plaintext), len(uvwxyz)):
    currX = x
    nextSeg = currX + len(uvwxyz)
    #print(cipher[currX:nextSeg])
    ptList.append(plaintext[currX:nextSeg])

#print(ptList)

cipherList = []

# Vigenere's Cipher encoding
for str in ptList:
    strList = list(str)
    s = ""
    for l in range(len(uvwxyz)):
        tmpChar = str[l]
        charInd = alphabet.index(tmpChar)

        pt = charInd + uvwxyz[l][1] 
        ind = 0
        if (pt > 25):
            ind = pt - 26
        else:
            ind = pt
        strList[l] = alphabet[ind]

    for j in strList:
        s += j
    cipherList.append(s)


cipherText = ""
for chr in cipherList:
    cipherText += chr

#print(cipherText)

keyRF = relativeFreq(cipherText)

summedValue = 0

for vlu in keyRF:
    summedValue += vlu[1]

meanKey = summedValue / len(ret)

print("{:.4E}".format(popFreq(keyRF, meanKey, len(ret))))



    


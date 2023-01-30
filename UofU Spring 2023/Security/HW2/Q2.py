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

meanKey = summedValue / len(keyRF)

#print("{:.4E}".format(popFreq(keyRF, meanKey, len(ret))))

yzCipher = "csfharjzuzlcsmgucqqhrxnnjhahcruzpmgmesmccecmbzqxqsclwnsmcdbsmaczzkcsmsfhljjhidymysrzajcqymbsfzrhlbjtbdqtlccqqsymbhlfrdaglhotcrrgysazlactqdbsmbmlnqmlgrcrcbsqgswgmvcucqsrgmesfnqdrdaglhotcrgmrgcqczjvmqjckzwugnjzrdrgckyvmqrgctlhtdprgswrptjdqzlcgskzwactldrggbyksmbdprmlcbgqatkrrzlbcrcucmnqmagmeemqudyjldqrcrkzwqcrskrhlrcucqcocmykrhcrsornymbhlbjtbhlfcwntjrgnlbgugkdhldqzlchzgkrhkdmtpomkgbwhlbqemqrxdnsqdnpswhqsfzrxmtktqspdqocbrsfdnqguybwzlcnqmocqrxphegrrmemsfdprysykjsglcrmqckqdwnsvgkjeyhjsfdansqqdybrhlfjzueskjxymbdrggbykjxgrwnsqpdqommqhzhjhrxazpddtjkwqczbsfdankosscqdqytbzlcyasrczasaeyzmmcndrcucqykddbdpzjkyvqsfzrapnycjxaqglgmykgycrankosscqgmrqsrgnlhcgybihlfsmbdprrzlcugysrgckyvnqmggagsqhdhlcmtzsudazlqcecqwnssmzlzrsmqldwojdyrcqcugdusfdsmgucqqhrxqzabcorzzkctqdnnjhaxanlbcqlhlfnqmocqsrcndhlemqkzrhmmrdaglnjnexyrudjkyrrgcrrtbdlsanbdyrkdkacqqndsfd"

xyzCipher = "brgfariyvxlcrlhscqpgsvnnigbfcrtyqkgmdrnaceblcxqxprdjwnrldbbslzdxzkbrnqfhkikfidxlzqrzzidoymargxrhkakrbdpsmacqprzkbhkesbagkgprcrqfzqazkzdrqdarnzmlmpnjgrbqdzsqfrxemvbtdosrflfqfnpcsbagkgprcrflsecqbyktmqiblxwufmkxrdqfdiyvlpsectkgubprfrxppticrxlcfrlxwabsmbrgfazismacqpmlbahoatjqsxlbbqdscmmpnygmddnoudximbqrbqlxwqbqtirhkqdscqbndkykqgdpsoqmzkbhkakrbhkeduntiqhllbfthidhkcrxlcgyhirhjcnrpoljhzwhkarcmqqwelsqcmqqwhprgxrxlslrqsocrmcbqrgbnqftzzwzkboomobpsvphdfspmelrgbprxrzijsfkdpmqbjrbwnruhijexgkqfdzmtoqdxasflfiyvcskiwzkbdqfhzykiwhpwnrpqbqollrfzhigsvazocerjkvpdxbsecblkorrdodqxscxlcxztpczzrbcyzlldldrbtdoykcccbpzijztqseysypnxbkvaqfkhkykfxdpanjntqcqflsosrfmmfcgxajflfrlcbprqymaugxrseckxuoomgfzhqqhcgmamtyrvbazkpdccqvmtqmzkysqmqkcxmjdxqdocufcvqfdrlhscqpgsvqzzadmrzyjdrqdmmkfaxzmmzcqkgmdnqlndosrbmeflelplxrhllsbagkmklexxqvbjkxqsecrqscblszmcbyrjclycqpmeqfd"
    
wxyzCipher = "aqfhypjzsxlcqkguaoqhpvnnhfahapuznkgmcqmcaccmzxqxoqclulsmabbskyczxicskqfhjhjhgbymwqrzyhcqwkbsdxrhjzjtzbqtjacqoqymzflfpbagjfotaprgwqazjyctobbskzmllomlepcrazsqeqwgktcuaosrekesdlqdpbagjfotapgmpecqaxjvkojcixwueljzpbrgaiyvkorgarlhrbpreqwrnrjdoxlceqkzuyctjbrgezykqkbdnpmlazgqyrkrpxlbapcuaknqkygmccmqsbyjjbqrapkzuocrqirhjpcuaocoakykpfcrqmrnwkbhjzjtzflfaunthpgnjzgueidhjbqzjahzeirhibmtnmmkezwhjzqekorxblsqblpsufqsdxrxkrktoqpdomcbpqfdloguwzwzjanqkmcqpvphcerrkcmsdbprwqykhqglapmqaiqdulsveijewfjsdbanqoqdwzrhjdjzscskhvymzbrgezykhvgrulsqnbqokkqhxfjhpvaznbdthiwqaxbsdbanimssaodqwrbzjayaqpczyqaewxmmaldrascqwiddzbpzhiyvoqfzpypnwajxyoglekykewcrylkoqqcqekrqqpgnjfcgwzihjdsmzbprpxlcseyspeckwtnqkegaeqqhbflckrzssbazjoceaownqqmzjxrskoldumjdwpcqasgdsqfdqkguaoqhpvqzyzcopxzkarqdlljhyvanjzcqjflflomoaosraldhjcmqixrhkkrdyelnhlexwpudhiyrpecrprbdjqanzbyribkaaoqnbqfd"

vwxyzCipher = "zpegbnhxuzizrlhqaoqhoummkdyfcrrwolhicqmczbblcvovqszivmtiabbsjxbyagaqmscekikdgbymvpqybfaoymypeysdjzjtyapsmyaoqsvjagmbpbagiensdnpeysxwkzdpobbsjylkomkjgrzobatmeqwgjsbtdmqpgmbpemrzpbagiensdnekrgznbykrkojchwvthjhxrdodbjzrkorgzqkguznpgstooskzoxlcdpjyxwarldodfazgqkbdmolkdxeoathoqymxapcuzjmpnwekeejntczfjbqrzojyxmapskoekqdqaocozjxjsdapsookxlcdjzjtyekedslrjrdkkahqeidhiapymyfxgkoejcnpnmmkdyvgmxocmqoucmtmblpsteprgvpvmthqprqzomcbopecomesybtwkbomkmcqouogfcppmejpecqnwqykgpfkdnkocknavmtreijeveirgzylsqnaxasdjdjzrbrjktwkbdodfazghvgrtkrpqzommmneygkdpvazmacskguoczypecbjimsszncpzpzxlcvxrqdvyqaevwlldjbpcuznxjezzbpzghxurodxramkxbktyogldjxjhuapanhlrrdmekrqpofmmdaeybfeketizbprowkbvcwqrgzhxuomkegadppgedjamtwptcbvjoceznvmtokxlzoplpmzumjdvobpdqebuscarlhqaoqhoupybxamrzwhbsrzlljhxuzmmxaolhicmpnkaosrzkcgmakokzoellszyelngkdwznsbjkvoqfdnprbdipzmczwpkdhxbprjbqfd"

uvwxyzCipher = "yodfarfvsxlcoiescqmdpvnnfdyfcrqvnkgmaokaceyizxqxmoajwnoiabbsiwaxzkyokqfhhfhfiduiwqrzwfaoymxodxrhhxhrbdmpjacqmowkbhhbpbaghdmrcrncwqazhwarqdxokzmljmkjgrynazsqcouemvyqaosrcicqfnmzpbaghdmrcrcipecqyvhtmqfyixwucjhxrdncaiyvimpecthdrbprcoupptfzoxlccoixwaypjbrgcxwismxznpmlyxeoatgnpxlbynascmjmkygmaakoudufjbqrynixwqynqirhhnascqykakykndapsonjwkbhhxhrbhhbauntfnellbcqeidhhzoxlcdveirhgzkrpoigezwhhxocmqntblsqzjnqwhmodxrxipirqslzomcbnodbnqcqwzwzhylomoympvphacppmeiodbpruowijschapmqygobwnoreijeudhqfdwjqoqduxpflffvscskftwkbdncezykftepwnomnbqoiiofzhfdpvazlzbrjksmaxbsbzylkoooaodqupzxlcuwqpczwoycyziialdryqaoykzzzbpzfgwtqsbvpypnuyhvaqchekykcuapangkqqcqciposrcjjfcguxgflfoizbprnvjauguopeckurlomgcweqqhzdjamtvosbazhmaccqsjqqmzhvpqmqhzumjdunaocuczsqfdoiescqmdpvqzwxamrzvgarqdjjhfaxwjjzcqhdjdnqikaosryjbfleimixrhiipbaghjhlexunsbjkunpecrnpzblswjzbyrgziycqmjbqfd"

#each cipher has a length of 840

## yz key case.

yzC1 = yzCipher[:420]

yzC2 = yzCipher[420:]

yzRF1 = relativeFreq(yzC1)
yzRF2 = relativeFreq(yzC2)
yzS1 = 0
yzS2 = 0

for v in yzRF1:
    yzS1 += v[1]

for o in yzRF2:
    yzS2 += o[1]

yzM1 = yzS1 / len(yzRF1)
yzM2 = yzS2 / len(yzRF2)

yzPF1 = popFreq(yzRF1, yzM1, len(yzRF1))
yzPF2 = popFreq(yzRF2, yzM2, len(yzRF2))

#print(yzPF1)
#print(yzPF2)

yzMean = (yzPF1 + yzPF2) / len(yz)

#print("{:.4E}".format(yzMean))


##########################
## xyz Case

xyzC1 = xyzCipher[:280]

xyzC2 = xyzCipher[280:560]

xyzC3 = xyzCipher[560:]

xyzRF1 = relativeFreq(xyzC1)
xyzRF2 = relativeFreq(xyzC2)
xyzRF3 = relativeFreq(xyzC3)
xyzS1 = 0
xyzS2 = 0
xyzS3 = 0

for v1 in xyzRF1:
    xyzS1 += v1[1]

for o1 in xyzRF2:
    xyzS2 += o1[1]

for r1 in xyzRF3:
    xyzS3 = r1[1]

xyzM1 = xyzS1 / len(xyzRF1)
xyzM2 = xyzS2 / len(xyzRF2)
xyzM3 = xyzS3 / len(xyzRF3)

xyzPF1 = popFreq(xyzRF1, xyzM1, len(xyzRF1))
xyzPF2 = popFreq(xyzRF2, xyzM2, len(xyzRF2))
xyzPF3 = popFreq(xyzRF3, xyzM3, len(xyzRF3))

#print(xyzPF1)
#print(xyzPF2)
#print(xyzPF3)
#print(len(xyz))
xyzMean = (xyzPF1 + xyzPF2 + xyzPF3) / len(xyz)

#print("{:.4E}".format(xyzMean))


##########################
## wxyz Case

wxyzC1 = wxyzCipher[:210]

wxyzC2 = wxyzCipher[210:420]

wxyzC3 = wxyzCipher[420:630]

wxyzC4 = wxyzCipher[630:]

wxyzRF1 = relativeFreq(wxyzC1)
wxyzRF2 = relativeFreq(wxyzC2)
wxyzRF3 = relativeFreq(wxyzC3)
wxyzRF4 = relativeFreq(wxyzC4)

wxyzS1 = 0
wxyzS2 = 0
wxyzS3 = 0
wxyzS4 = 0

for v2 in wxyzRF1:
    wxyzS1 += v2[1]

for o2 in wxyzRF2:
    wxyzS2 += o2[1]

for r2 in wxyzRF3:
    wxyzS3 = r2[1]

for e2 in wxyzRF4:
    wxyzS4 = e2[1]

wxyzM1 = wxyzS1 / len(wxyzRF1)
wxyzM2 = wxyzS2 / len(wxyzRF2)
wxyzM3 = wxyzS3 / len(wxyzRF3)
wxyzM4 = wxyzS4 / len(wxyzRF4)

wxyzPF1 = popFreq(wxyzRF1, wxyzM1, len(wxyzRF1))
wxyzPF2 = popFreq(wxyzRF2, wxyzM2, len(wxyzRF2))
wxyzPF3 = popFreq(wxyzRF3, wxyzM3, len(wxyzRF3))
wxyzPF4 = popFreq(wxyzRF4, wxyzM4, len(wxyzRF4))

#print(wxyzPF1)
#print(wxyzPF2)
#print(wxyzPF3)
#print(wxyzPF4)

#print(len(wxyz))
wxyzMean = (wxyzPF1 + wxyzPF2 + wxyzPF3 + wxyzPF4) / len(wxyz)

#print("{:.4E}".format(wxyzMean))


##########################
## vwxyz Case

vwxyzC1 = vwxyzCipher[:168]

vwxyzC2 = vwxyzCipher[168:336]

vwxyzC3 = vwxyzCipher[336:504]

vwxyzC4 = vwxyzCipher[504:672]

vwxyzC5 = vwxyzCipher[672:]

vwxyzRF1 = relativeFreq(vwxyzC1)
vwxyzRF2 = relativeFreq(vwxyzC2)
vwxyzRF3 = relativeFreq(vwxyzC3)
vwxyzRF4 = relativeFreq(vwxyzC4)
vwxyzRF5 = relativeFreq(vwxyzC5)

vwxyzS1 = 0
vwxyzS2 = 0
vwxyzS3 = 0
vwxyzS4 = 0
vwxyzS5 = 0

for v3 in vwxyzRF1:
    vwxyzS1 += v3[1]

for o3 in vwxyzRF2:
    vwxyzS2 += o3[1]

for r3 in vwxyzRF3:
    vwxyzS3 = r3[1]

for e3 in vwxyzRF4:
    vwxyzS4 = e3[1]

for n3 in vwxyzRF5:
    vwxyzS5 = n3[1]

vwxyzM1 = vwxyzS1 / len(vwxyzRF1)
vwxyzM2 = vwxyzS2 / len(vwxyzRF2)
vwxyzM3 = vwxyzS3 / len(vwxyzRF3)
vwxyzM4 = vwxyzS4 / len(vwxyzRF4)
vwxyzM5 = vwxyzS5 / len(vwxyzRF5)

vwxyzPF1 = popFreq(vwxyzRF1, vwxyzM1, len(vwxyzRF1))
vwxyzPF2 = popFreq(vwxyzRF2, vwxyzM2, len(vwxyzRF2))
vwxyzPF3 = popFreq(vwxyzRF3, vwxyzM3, len(vwxyzRF3))
vwxyzPF4 = popFreq(vwxyzRF4, vwxyzM4, len(vwxyzRF4))
vwxyzPF5 = popFreq(vwxyzRF5, vwxyzM5, len(vwxyzRF5))

#print(vwxyzPF1)
#print(vwxyzPF2)
#print(vwxyzPF3)
#print(vwxyzPF4)
#print(vwxyzPF5)
#print(len(vwxyz))
vwxyzMean = (vwxyzPF1 + vwxyzPF2 + vwxyzPF3 + vwxyzPF4 + vwxyzPF5) / len(vwxyz)

#print("{:.4E}".format(vwxyzMean))

##########################
## uvwxyz Case

uvwxyzC1 = uvwxyzCipher[:140]

uvwxyzC2 = uvwxyzCipher[140:280]

uvwxyzC3 = uvwxyzCipher[280:420]

uvwxyzC4 = uvwxyzCipher[420:560]

uvwxyzC5 = uvwxyzCipher[560:700]

uvwxyzC6 = uvwxyzCipher[700:840]

uvwxyzRF1 = relativeFreq(uvwxyzC1)
uvwxyzRF2 = relativeFreq(uvwxyzC2)
uvwxyzRF3 = relativeFreq(uvwxyzC3)
uvwxyzRF4 = relativeFreq(uvwxyzC4)
uvwxyzRF5 = relativeFreq(uvwxyzC5)
uvwxyzRF6 = relativeFreq(uvwxyzC6)

uvwxyzS1 = 0
uvwxyzS2 = 0
uvwxyzS3 = 0
uvwxyzS4 = 0
uvwxyzS5 = 0
uvwxyzS6 = 0

for v4 in uvwxyzRF1:
    uvwxyzS1 += v4[1]

for o4 in uvwxyzRF2:
    uvwxyzS2 += o4[1]

for r4 in uvwxyzRF3:
    uvwxyzS3 = r4[1]

for e4 in uvwxyzRF4:
    uvwxyzS4 = e4[1]

for n4 in uvwxyzRF5:
    uvwxyzS5 = n4[1]

for a4 in uvwxyzRF6:
    uvwxyzS6 = a4[1]

uvwxyzM1 = uvwxyzS1 / len(uvwxyzRF1)
uvwxyzM2 = uvwxyzS2 / len(uvwxyzRF2)
uvwxyzM3 = uvwxyzS3 / len(uvwxyzRF3)
uvwxyzM4 = uvwxyzS4 / len(uvwxyzRF4)
uvwxyzM5 = uvwxyzS5 / len(uvwxyzRF5)
uvwxyzM6 = uvwxyzS6 / len(uvwxyzRF6)

uvwxyzPF1 = popFreq(uvwxyzRF1, uvwxyzM1, len(uvwxyzRF1))
uvwxyzPF2 = popFreq(uvwxyzRF2, uvwxyzM2, len(uvwxyzRF2))
uvwxyzPF3 = popFreq(uvwxyzRF3, uvwxyzM3, len(uvwxyzRF3))
uvwxyzPF4 = popFreq(uvwxyzRF4, uvwxyzM4, len(uvwxyzRF4))
uvwxyzPF5 = popFreq(uvwxyzRF5, uvwxyzM5, len(uvwxyzRF5))
uvwxyzPF6 = popFreq(uvwxyzRF6, uvwxyzM6, len(uvwxyzRF6))

#print(uvwxyzPF1)
#print(uvwxyzPF2)
#print(uvwxyzPF3)
#print(uvwxyzPF4)
#print(uvwxyzPF5)
#print(uvwxyzPF6)
#print(len(uvwxyz))
uvwxyzMean = (uvwxyzPF1 + uvwxyzPF2 + uvwxyzPF3 + uvwxyzPF4 + uvwxyzPF5 + uvwxyzPF6) / len(uvwxyz)

#print("{:.4E}".format(uvwxyzMean))


###############################################################
##2.7

## len 2

yzC1 = uvwxyzCipher[:420]

yzC2 = uvwxyzCipher[420:]

yzRF1 = relativeFreq(yzC1)
yzRF2 = relativeFreq(yzC2)
yzS1 = 0
yzS2 = 0

for v in yzRF1:
    yzS1 += v[1]

for o in yzRF2:
    yzS2 += o[1]

yzM1 = yzS1 / len(yzRF1)
yzM2 = yzS2 / len(yzRF2)

yzPF1 = popFreq(yzRF1, yzM1, len(yzRF1))
yzPF2 = popFreq(yzRF2, yzM2, len(yzRF2))

#print(yzPF1)
#print(yzPF2)
#print(len(yz))

yzMean = (yzPF1 + yzPF2) / len(yz)

#print("{:.4E}".format(yzMean))


##########################
## len 3 Case

xyzC1 = uvwxyzCipher[:280]

xyzC2 = uvwxyzCipher[280:560]

xyzC3 = uvwxyzCipher[560:]

xyzRF1 = relativeFreq(xyzC1)
xyzRF2 = relativeFreq(xyzC2)
xyzRF3 = relativeFreq(xyzC3)
xyzS1 = 0
xyzS2 = 0
xyzS3 = 0

for v1 in xyzRF1:
    xyzS1 += v1[1]

for o1 in xyzRF2:
    xyzS2 += o1[1]

for r1 in xyzRF3:
    xyzS3 = r1[1]

xyzM1 = xyzS1 / len(xyzRF1)
xyzM2 = xyzS2 / len(xyzRF2)
xyzM3 = xyzS3 / len(xyzRF3)

xyzPF1 = popFreq(xyzRF1, xyzM1, len(xyzRF1))
xyzPF2 = popFreq(xyzRF2, xyzM2, len(xyzRF2))
xyzPF3 = popFreq(xyzRF3, xyzM3, len(xyzRF3))

#print(xyzPF1)
#print(xyzPF2)
#print(xyzPF3)
#print(len(xyz))
xyzMean = (xyzPF1 + xyzPF2 + xyzPF3) / len(xyz)

#print("{:.4E}".format(xyzMean))


##########################
## len 4 Case

wxyzC1 = uvwxyzCipher[:210]

wxyzC2 = uvwxyzCipher[210:420]

wxyzC3 = uvwxyzCipher[420:630]

wxyzC4 = uvwxyzCipher[630:]

wxyzRF1 = relativeFreq(wxyzC1)
wxyzRF2 = relativeFreq(wxyzC2)
wxyzRF3 = relativeFreq(wxyzC3)
wxyzRF4 = relativeFreq(wxyzC4)

wxyzS1 = 0
wxyzS2 = 0
wxyzS3 = 0
wxyzS4 = 0

for v2 in wxyzRF1:
    wxyzS1 += v2[1]

for o2 in wxyzRF2:
    wxyzS2 += o2[1]

for r2 in wxyzRF3:
    wxyzS3 = r2[1]

for e2 in wxyzRF4:
    wxyzS4 = e2[1]

wxyzM1 = wxyzS1 / len(wxyzRF1)
wxyzM2 = wxyzS2 / len(wxyzRF2)
wxyzM3 = wxyzS3 / len(wxyzRF3)
wxyzM4 = wxyzS4 / len(wxyzRF4)

wxyzPF1 = popFreq(wxyzRF1, wxyzM1, len(wxyzRF1))
wxyzPF2 = popFreq(wxyzRF2, wxyzM2, len(wxyzRF2))
wxyzPF3 = popFreq(wxyzRF3, wxyzM3, len(wxyzRF3))
wxyzPF4 = popFreq(wxyzRF4, wxyzM4, len(wxyzRF4))

#print(wxyzPF1)
#print(wxyzPF2)
#print(wxyzPF3)
#print(wxyzPF4)

#print(len(wxyz))
wxyzMean = (wxyzPF1 + wxyzPF2 + wxyzPF3 + wxyzPF4) / len(wxyz)

#print("{:.4E}".format(wxyzMean))


##########################
## len 5 Case

vwxyzC1 = uvwxyzCipher[:168]

vwxyzC2 = uvwxyzCipher[168:336]

vwxyzC3 = uvwxyzCipher[336:504]

vwxyzC4 = uvwxyzCipher[504:672]

vwxyzC5 = uvwxyzCipher[672:]

vwxyzRF1 = relativeFreq(vwxyzC1)
vwxyzRF2 = relativeFreq(vwxyzC2)
vwxyzRF3 = relativeFreq(vwxyzC3)
vwxyzRF4 = relativeFreq(vwxyzC4)
vwxyzRF5 = relativeFreq(vwxyzC5)

vwxyzS1 = 0
vwxyzS2 = 0
vwxyzS3 = 0
vwxyzS4 = 0
vwxyzS5 = 0

for v3 in vwxyzRF1:
    vwxyzS1 += v3[1]

for o3 in vwxyzRF2:
    vwxyzS2 += o3[1]

for r3 in vwxyzRF3:
    vwxyzS3 = r3[1]

for e3 in vwxyzRF4:
    vwxyzS4 = e3[1]

for n3 in vwxyzRF5:
    vwxyzS5 = n3[1]

vwxyzM1 = vwxyzS1 / len(vwxyzRF1)
vwxyzM2 = vwxyzS2 / len(vwxyzRF2)
vwxyzM3 = vwxyzS3 / len(vwxyzRF3)
vwxyzM4 = vwxyzS4 / len(vwxyzRF4)
vwxyzM5 = vwxyzS5 / len(vwxyzRF5)

vwxyzPF1 = popFreq(vwxyzRF1, vwxyzM1, len(vwxyzRF1))
vwxyzPF2 = popFreq(vwxyzRF2, vwxyzM2, len(vwxyzRF2))
vwxyzPF3 = popFreq(vwxyzRF3, vwxyzM3, len(vwxyzRF3))
vwxyzPF4 = popFreq(vwxyzRF4, vwxyzM4, len(vwxyzRF4))
vwxyzPF5 = popFreq(vwxyzRF5, vwxyzM5, len(vwxyzRF5))

print(vwxyzPF1)
print(vwxyzPF2)
print(vwxyzPF3)
print(vwxyzPF4)
print(vwxyzPF5)
print(len(vwxyz))
vwxyzMean = (vwxyzPF1 + vwxyzPF2 + vwxyzPF3 + vwxyzPF4 + vwxyzPF5) / len(vwxyz)

print("{:.4E}".format(vwxyzMean))

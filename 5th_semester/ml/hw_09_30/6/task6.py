def check(x, file):
    d = {}
    x = x.lower()
    word_list = x.split()
    for word in word_list:
        numb_of_occurrences = d.get(word, 0)
        d[word] = numb_of_occurrences + 1

    l = []
    for word, numb_of_occurrences in d.items():
        l.append([word, numb_of_occurrences])
    l.sort(key=lambda x: x[0])
    
    answ = ''
    for i in l:
        answ += str(i[0] + ' ' + str(i[1]) + '\n')

    with open(file, 'w') as f:
        f.write(answ)
    f.close()

if __name__ == "__main__":
    check("c c f d aaA a c d r c ccc cC c b b b b", "file.txt")
    with open("file.txt", "r") as f:
        print(''.join(f.readlines()))

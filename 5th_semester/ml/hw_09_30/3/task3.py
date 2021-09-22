def longestCommonPrefix(x):
    answ = ''
    if len(x) != 0:
        answ = x[0].lstrip(' ')
    for word in x:
        word = word.lstrip(' ')
        while answ != word[:len(answ)]:
            answ = answ[:len(answ)-1]
    return answ

if __name__ == "__main__":
    print(longestCommonPrefix(["flo","flow","flight"])) # fl
    print(longestCommonPrefix(["dog","racecar","car"])) #
    print(longestCommonPrefix([" flo","flow","fl"])) # fl

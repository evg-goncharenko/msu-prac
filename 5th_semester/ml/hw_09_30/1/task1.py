def hello(x = ''):
    res = 'Hello'
    if x != '':
        res += ', ' + x
    res += '!'
    return res

if __name__ == "__main__":
    print(hello('Hello, world'))
    print(hello())
    print(hello(''))
    print(hello('Masha'))

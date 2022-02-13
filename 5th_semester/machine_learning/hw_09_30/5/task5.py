def primes():
    prime_numb = 2 
    count = 0
    while True:
        for i in range (2, int(prime_numb ** 0.5) + 1):
            if prime_numb % i == 0:
                count += 1
        if count == 0:
            yield prime_numb
        prime_numb += 1
        count = 0


if __name__ == "__main__":
    for i in primes():
        print(i)
        if i > 30:
            break

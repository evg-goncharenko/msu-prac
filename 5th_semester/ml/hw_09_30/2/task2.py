def is_palindrome(x):
    init_val = x
    tmp_numb = 0
    while x != 0:
        tmp_numb = tmp_numb * 10 + (x % 10)
        x //= 10
    if tmp_numb != init_val:
        return "NO"
    return "YES"

if __name__ == "__main__":
   print(is_palindrome(121)) # YES
   print(is_palindrome(120)) # NO
   print(is_palindrome(10)) # NO

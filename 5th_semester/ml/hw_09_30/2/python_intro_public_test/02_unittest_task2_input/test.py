from task2 import is_palindrome
import pytest


@pytest.mark.parametrize(
    "num,ans",
    [
        [101, "YES"],
        [1, "YES"],
        [10, "NO"],
        [110, "NO"],
        [1001, "YES"],
        [2002, "YES"],
        [20302, "YES"],
        [100, "NO"],
        [2333, "NO"],
        [3333, "YES"],
        [111111111111, "YES"],
        [123456789, "NO"],
        [12345678987654321, "YES"],
        [100090001, "YES"],
        [10000001, "YES"],
        [10010001, "NO"],
        [0, "YES"]
    ]
)
def test_palindrome(num, ans):
    assert is_palindrome(num) == ans


def test_sequence():
    with open("task2.py", "r") as f:
        p = ''.join(f.readlines())
        is_seq = "str" in p or "list" in p or "repr" in p
        assert not is_seq, "You can't present a number as a sequence. If you do not do it, remove all sequence variable names containing type names."

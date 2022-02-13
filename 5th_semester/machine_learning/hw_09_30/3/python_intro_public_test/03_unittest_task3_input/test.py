from task3 import longestCommonPrefix
import pytest


@pytest.mark.parametrize(
    "arg,res",
    [
        [["flower","flow","flight"], "fl"],
        [["      flower","  flow"," flight", "flight    "], "fl"],
        [["dog","racecar","car"], ""],
        [["c","cc","ccc"], "c"],
        [["","  ","   "], ""],
        [[" ","  ","   "], ""],
        [["123"," 1 23","12 3"], "1"],
        [["1" for _ in range(100)], "1"],
        [["23" + str(i) for i in range(100)], "23"]
    ]
)
def test_prefix(arg, res):
    assert longestCommonPrefix(arg) == res

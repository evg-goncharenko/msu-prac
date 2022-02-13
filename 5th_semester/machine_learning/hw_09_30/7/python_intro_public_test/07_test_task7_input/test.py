from task7 import process
import pytest


@pytest.mark.parametrize(
    "arg,res",
    [
        [[[1], [4, 6, 9], [9, 6], [4], [0]], [81, 36, 16, 1, 0]],
        [[[1, 2], [3], [4], [3, 1]], [16, 9, 4, 1]],
        [[[2, 2], [2], [2, 2, 2], [1]], [4, 1]],
        [[[-1], [-2], [-4]], [16, 4, 1]],
        [[[1], [1], [1]], [1]],
        [[[], [], []], []]
    ]
)
def test_process(arg, res):
    assert process(arg) == res

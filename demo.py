#!/usr/bin/env python3

from typing import Iterable

N = 11


class MBI:
    letters = tuple('ACDEFGHJKMNPQRTUVWXYZ')
    numbers = tuple(str(i) for i in range(10))
    digits = (
        numbers[1:],
        letters,
        numbers + letters,
        numbers,
        letters,
        numbers + letters,
        numbers,
        letters,
        letters,
        numbers,
        numbers,
    )
    assert len(digits) == N
    sizes = tuple(len(d) for d in digits)

    def __init__(self, indices: Iterable[int] = (0,)*N):
        indices = list(indices)
        assert len(indices) == N
        self.indices = indices

    def copy(self):
        return MBI(self.indices)

    @classmethod
    def parse(cls, as_str: str):
        assert len(as_str) == N
        return MBI((
            digs.index(s)
            for digs, s in zip(cls.digits, as_str)
        ))

    @classmethod
    def is_valid(cls, as_str: str) -> bool:
        try:
            cls.parse(as_str)
            return True
        except Exception as e:
            return False

    def __str__(self):
        return ''.join(digs[i] for digs, i in zip(self.digits, self.indices))

    def add(self, addend=1):
        for pos in range(N-1, -1, -1):
            carry, digit = divmod(self.indices[pos] + addend, self.sizes[pos])
            self.indices[pos] = digit
            if carry:
                if pos == 0:
                    raise ValueError()
                addend = carry
            else:
                break


def increasing_same(mbi: MBI, n: int, addend: int = 1) -> Iterable[MBI]:
    for _ in range(n):
        yield mbi
        mbi.add(addend)


def increasing_copy(mbi: MBI, n: int, addend: int = 1) -> Iterable[MBI]:
    for _ in range(n):
        yield mbi
        mbi = mbi.copy()
        mbi.add(addend)


def test():
    assert MBI.is_valid('1A00A00GA98')
    assert not MBI.is_valid('2')
    assert not MBI.is_valid('00000000000')
    assert MBI.parse('1A00A00GA98').indices == [
        0, 0, 0, 0, 0, 0, 0, 5, 0, 9, 8
    ]
    assert MBI.parse('1A00A00AA00').indices == [0]*N
    assert str(MBI()) == '1A00A00AA00'


def demo():
    for mbi in increasing_same(MBI.parse('4M44M44MN98'), 6):
        print(mbi)
    print()

    for mbi in increasing_same(MBI.parse('8ZZ9ZZ9ZZ95'), 10, 2):
        print(mbi)
    print()


if __name__ == '__main__':
    test()
    demo()


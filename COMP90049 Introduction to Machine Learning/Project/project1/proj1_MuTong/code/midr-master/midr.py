class Midr:
    """
    Takes m, i, d, r number parameters and calculates
    global / local edit distances
    """
    def __init__(self, match=0, insert=1, delete=1, replace=1):
        """
        Object constructor
        :param match: 0
        :param insert: 1
        :param delete: 1
        :param replace: 1
        """
        self.is_max = match > max(insert, delete, replace)
        self.match = match
        self.insert = insert
        self.delete = delete
        self.replace = replace

    def _is_equal(self, f, t):
        return self.match if f == t else self.replace

    def global_edit_distance(self, from_word, to_word):
        """
        Calculate global edit distance
        :param from_word:
        :param to_word:
        :return: int
        """
        lt = len(to_word)
        lf = len(from_word)

        a = [[x * self.delete for x in range(lf+1)]]
        for i in range(1, lt+1):
            a.append([i * self.insert])

        for t in range(1, lt+1):
            for f in range(1, lf+1):
                comp = [a[t][f - 1] + self.delete,
                        a[t - 1][f] + self.insert,
                        a[t - 1][f - 1] + self._is_equal(from_word[f - 1], to_word[t - 1])]
                a[t].append(max(comp) if self.is_max else min(comp))
        return a[lt][lf]

    def local_edit_distance(self, from_word, to_word):
        """
        Calculate local edit distance
        :param from_word:
        :param to_word:
        :return: int
        """
        lt = len(to_word)
        lf = len(from_word)

        a = [[x * self.delete for x in range(lf + 1)]]
        for i in range(1, lt + 1):
            a.append([i * self.insert])

        res = 0 if self.is_max else 999999
        for t in range(1, lt + 1):
            for f in range(1, lf + 1):
                comp = [0,
                        a[t][f - 1] + self.delete,
                        a[t - 1][f] + self.insert,
                        a[t - 1][f - 1] + self._is_equal(from_word[f - 1], to_word[t - 1])]
                a[t].append(max(comp) if self.is_max else min(comp))
                res = max(res, a[t][f]) if self.is_max else min(res, a[t][f])
        return res

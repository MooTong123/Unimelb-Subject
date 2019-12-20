
import ngram


def nGram():
    dict_file = open("../data/dict.txt", 'r')
    misspell_file = open("../data/misspell.txt", 'r')
    result_file = open("../result/ngram_4_result.txt", 'w')

    # to read the dictionary
    dict = []
    for line in dict_file.readlines():
        line = line.strip()
        dict.append(line)
    dict_file.close()

    # analyze the misspell file
    for line in misspell_file.readlines():
        line = line.strip()
        result = []
        max_value = -1
        if line in dict:
            result.clear()
            result.append(line)
        else:
            for i in range(len(dict)):
                distance = ngram.NGram.compare(str(dict[i]), str(line), N=4)

                if distance == 0:
                    continue
                elif distance > max_value:
                    max_value = distance
                    result.clear()
                    result.append(str(dict[i]))
                elif distance == max_value:
                    result.append(str(dict[i]))

        for i in range(len(result)):
            print(result[i] + " ")
            result_file.write(result[i] + " ")
        print('\n')
        result_file.write('\n')
    misspell_file.close()
    result_file.close()


def main():
    nGram()


if __name__ == "__main__":
    main()

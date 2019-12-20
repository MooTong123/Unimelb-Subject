

from py_stringmatching import Editex

def editex():
    dict_file = open("../data/dict.txt", 'r')
    misspell_file = open("../data/misspell.txt",'r')
    result_file = open("../result/editex_result.txt",'w')
    ed = Editex()

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
        min_value = 9999999999

        if line in dict:
            result.clear()
            result.append(line)
        else:
            for i in range(len(dict)):
                distance = ed.get_raw_score(dict[i],line)
                if distance < min_value:
                    min_value = distance
                    result.clear()
                    result.append(dict[i])
                elif distance == min_value:
                    result.append(dict[i])

        for i in range(len(result)):
            result_file.write(result[i] + " ")
            print(result[i] + " ")
        result_file.write('\n')
    misspell_file.close()
    result_file.close()

def main():
    editex()

if __name__ == "__main__":
    main()

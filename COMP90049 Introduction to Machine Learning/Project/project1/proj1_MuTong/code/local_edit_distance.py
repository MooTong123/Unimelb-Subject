
#import from https://github.com/bahriddin/midr/blob/master/midr.py

from midr import Midr

def local_edit_dis():
    dict_file = open("../data/dict.txt", 'r')
    misspell_file = open("../data/misspell.txt",'r')
    result_file = open("../result/local_edit_distance_result.txt",'w')
    obj = Midr(1,-1,-1,-1)

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
            result.append(line)
        else:
            if line.isalpha() == False:
                result.append(line)
            else:
                for i in range(len(dict)):
                    distance = obj.local_edit_distance(line,dict[i])

                    if distance == 0:
                        continue
                    elif distance > max_value:
                        max_value = distance
                        result.clear()
                        result.append(dict[i])
                    elif distance == max_value:
                        result.append(dict[i])

        for i in range(len(result)):
            result_file.write(result[i] + " ")
            print(result[i] + " ")
        result_file.write('\n')
        result.clear()
    misspell_file.close()
    result_file.close()

def main():
    local_edit_dis()

if __name__ == "__main__":
    main()

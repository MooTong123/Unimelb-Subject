

import editdistance

def global_edit_dis():
    dict_file = open("../data/dict.txt", 'r')
    misspell_file = open("../data/misspell.txt",'r')
    result_file = open("../result/global_edit_distance_result.txt",'w')

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
        min_value = 99999

        if line in dict:
            result.clear()
            result.append(line)
        else:
            for i in range(len(dict)):
                distance = editdistance.eval(dict[i],line)
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
    global_edit_dis()

if __name__ == "__main__":
    main()

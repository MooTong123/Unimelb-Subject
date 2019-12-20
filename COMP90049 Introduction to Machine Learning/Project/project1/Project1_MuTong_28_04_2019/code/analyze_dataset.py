

def analyze():
    # test_file = open("../result/test.txt", 'r')
    dict_file = open("../data/dict.txt", 'r')
    correct_file = open("../data/correct.txt", 'r')
    misspell_file = open("../data/misspell.txt", 'r')
    misspell_not_in_dict = open("../data/misspell_not_in_dict.txt","w")

    dict = []
    for line in dict_file.readlines():
        line = line.strip()
        dict.append(line)
    dict_file.close()

    misspell = []
    for line in misspell_file.readlines():
        line = line.strip()
        misspell.append(line)
    misspell_file.close()

    correct = []
    for line in correct_file.readlines():
        line = line.strip()
        correct.append(line)
    correct_file.close()

    correct_exact_match = 0
    wrong_exact_match = 0
    misspell_not_in_dictionary = 0
    misspell_OOV = []
    for i in range(len(misspell)):
        if misspell[i] in dict:
            if misspell[i] == correct[i]:
                correct_exact_match = correct_exact_match + 1
            else:
                wrong_exact_match = wrong_exact_match + 1
        else:
            misspell_not_in_dictionary = misspell_not_in_dictionary + 1
            misspell_OOV.append(misspell[i])


    print("correct exact match ",correct_exact_match)
    print("wrong exact match ",wrong_exact_match)
    print("misspell not in dict: ",misspell_not_in_dictionary)

    for line in misspell_OOV:
        misspell_not_in_dict.write(line)
        misspell_not_in_dict.write("\n")
    misspell_not_in_dict.close()

def main():
    analyze()

if __name__ == "__main__":
    main()

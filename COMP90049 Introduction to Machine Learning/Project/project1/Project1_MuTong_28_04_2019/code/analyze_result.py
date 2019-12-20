

def analyze_r():

    pre1_f = open("../result/global_edit_distance_result.txt", 'r')
    pre2_f = open("../result/local_edit_distance_result.txt", 'r')
    pre3_f = open("../result/ngram_1_result.txt", 'r')
    pre4_f = open("../result/ngram_2_result.txt", 'r')
    pre5_f = open("../result/ngram_3_result.txt", 'r')
    pre6_f = open("../result/ngram_4_result.txt", 'r')
    pre7_f = open("../result/soundex_result.txt", 'r')

    dict_file = open("../data/dict.txt", 'r')
    correct_file = open("../data/correct.txt", 'r')
    misspell_file = open("../data/misspell.txt", 'r')

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

    pre1 = []
    for line in pre1_f.readlines():
        list = line.split()
        pre1.append(list)
    pre1_f.close()

    pre2 = []
    for line in pre2_f.readlines():
        list = line.split()
        pre2.append(list)
    pre2_f.close()

    pre3 = []
    for line in pre3_f.readlines():
        list = line.split()
        pre3.append(list)
    pre3_f.close()

    pre4 = []
    for line in pre4_f.readlines():
        list = line.split()
        pre4.append(list)
    pre4_f.close()

    pre5 = []
    for line in pre5_f.readlines():
        list = line.split()
        pre5.append(list)
    pre5_f.close()

    pre6 = []
    for line in pre6_f.readlines():
        list = line.split()
        pre6.append(list)
    pre6_f.close()

    # pre7 = []
    # for line in pre7_f.readlines():
    #     list = line.split()
    #     pre7.append(list)
    # pre7_f.close()

    count1 = 0
    count2 = 0
    count3 = 0
    count4 = 0
    count5 = 0
    count6 = 0
    #count7 = 0

    for i in range(len(misspell)):
        if misspell[i] in dict:
            continue
        else:
            count1 = len(pre1[i]) + count1
            count2 = len(pre2[i]) + count2
            count3 = len(pre3[i]) + count3
            count4 = len(pre4[i]) + count4
            count5 = len(pre5[i]) + count5
            count6 = len(pre6[i]) + count6
            #count7 = len(pre7[i]) + count7

    print(count1,count2,count3,count4,count5,count6)

def main():
    analyze_r()

if __name__ == "__main__":
    main()

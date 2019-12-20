from pyphonetics import Soundex
from pyphonetics import RefinedSoundex

def soundex():
    dict_file = open("../data/dict.txt", 'r')
    misspell_file = open("../data/misspell_4795.txt",'r')
    result_file = open("../result/soundex_result_all_2.txt",'w')
    # to read the dictionary
    soundex = Soundex()
    dict = []
    dict_soundex = []
    for line in dict_file.readlines():
        line = line.strip()
        dict.append(line)
      #  dict_soundex.append(soundex.phonetics(line))
    dict_file.close()


    # analyze the misspell file
    for line in misspell_file.readlines():
        line = line.strip()
        # soundex_line = soundex.phonetics(line)
        result = []

        if line in dict:
            result.append(line)

        elif line.isalpha() == False:
            result.append(line)

        # if soundex_line in dict_soundex:
        #     index_soundex = [i for i, x in enumerate(dict_soundex) if x == soundex_line]
        #     for i in index_soundex:
        #         result.append(dict[index_soundex[i]])
        # else:

        else:
             for i in range(len(dict)):
                rs = RefinedSoundex()
                distance = rs.distance(line,dict[i])
                if distance == 0:
                    result.append(dict[i])
                #
                # elif distance < 2:
                #  result.append(str(dict[i]))

        for i in range(len(result)):
            print(result[i] + " ")
            result_file.write(result[i] + " ")
        print('\n')
        result_file.write('\n')
        result.clear()
    misspell_file.close()
    result_file.close()

def main():
    soundex()

if __name__ == "__main__":
    main()

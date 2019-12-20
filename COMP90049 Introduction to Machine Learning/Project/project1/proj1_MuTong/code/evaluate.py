
def evaluate():
    # test_file = open("../result/test.txt", 'r')
    predict_file = open("../result/editex_result.txt", 'r')
    correct_file = open("../data/correct.txt", 'r')
    correct_count = 0
    return_predict = 0

    # for line in test_file.readlines():
    #     list = line.split()
    #     print(list)
    pretict = []
    for line in predict_file.readlines():
        list = line.split()
        pretict.append(list)
    predict_file.close()

    correct = []
    for line in correct_file.readlines():
        line = line.strip()
        correct.append(line)
    correct_file.close()

    for i in range(len(correct)):
        if correct[i] in pretict[i]:
            correct_count = correct_count + 1
        return_predict = return_predict + len(pretict[i])

    precision = float(correct_count / return_predict )
    recall = float(correct_count / len(correct))

    print("the total number of correct answer:", len(correct))
    print("correct returned answer:" ,correct_count)
    print("returned prediction: ",return_predict)
    print("System precision: " , precision)
    print("System recall: " , recall)

def main():
    evaluate()

if __name__ == "__main__":
    main()

This tarball contains eight files (plus this README), in three types, as follows:

{train,dev,test}-tweets.txt: These files contain the raw text of the tweets, one tweet per line, in the following format:
tweet-id TAB tweet-text NEWLINE
Note that the text was pre-processed (folding case, and removing all characters that are not alphabetic ([a-z])) before feature engineering/selection was performed.

{train,dev}-labels.txt: These files contain the manually--assigned sentiment labels, one tweet per line, in the following format:
tweet-id TAB label NEWLINE
The labels are one of "positive", "negative", or "neutral". Note that you will not be given labels for the test data, but you can submit to Kaggle In-class to find the accuracy of your system.

{train,dev,test}.csv: These files combine the information contained within the previous two files. We applied feature selection to generate the frequencies of some tokens in the training data, and then summarised in a format suitable as input to Weka. (http://www.cs.waikato.ac.nz/ml/weka/)
Each line in the CSV file represents a instance, in CSV format.


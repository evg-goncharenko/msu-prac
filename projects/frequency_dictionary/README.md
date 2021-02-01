## Creating a frequency dictionary for some text

The program creates a frequency dictionary for a given text. It is started using the command line of the form:
```
./prog -i f.txt -o dict.txt
```
The `-i` option specifies the input text file, and the `-o` option specifies the output file to which the frequency dictionary is print. In the absence of the `-i` option, the input comes from the standard input channel, in the absence of the `-o` option, the output goes to the standard output channel. <br>
The input text is divided into words, with whitespace characters and line ends as delimiters. For each word, the number of its occurrences in the text is calculated. The program outputs to the dictionary all the words of the text without repetition, ordered in descending order of the number of occurrences in the text. Output format: <br>

<b> Word1 | number of occurrences | frequency of occurrences of word </b> <br>
<b> Word2 | number of occurrences | frequency of occurrences of word </b> <br><br>
For example:
```
.....
а 19 0.000432
в 12 0.000319
Moscow 6 0.00016
.....
```
`The frequency of occurrences of a word` is the ratio of the number of occurrences of a word to the total number of words in the text. <br>
Each word is issued only once. Punctuation marks (dot, dash, comma, etc.), as well as signs such as questions, exclamations, etc. are considered separate words. Words are considered different if there is at least one different character. There is no reduction of words to the same case. <br>
For the efficiency of the program, the dictionary is stored in a binary search tree, the key is the word itself, the information part is the occurrence counter.

## The program "The Writer" of the text

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [Main](./Main.py): </b> - graphical interface for demonstrating the operation and use of basic algorithms
and functions. <br>
<b> [TheWriter](./TheWriter.py): </b> - a file with implementation of the basic algorithms <br>

### About program:
The program is "The Writer" of a text of a given type. The specified text changes according to certain rules:
1) Phrases corresponding to a given type and style (length, parts of speech, length of words, etc.)
are selected from the text.
2) In each phrase, several words are replaced with more suitable ones (formally, in style, for example,
with longer ones).
3) The words are taken from unused phrases of the text, put in the right form.
4) The morphological characteristics are clearly matched (it is clear that semantically there may be nonsense,
but grammatically the text is correct).<br>
Example: "Петя рисует дом" - "Интеграл усиливает простуду".<br>
The graphical interface is made to adapt to different window sizes and screen resolutions.
<br><br>

<img src="main_page.png" width="100%">
<img src="result.png" width="100%">

### Installation:
Libraries used:<br>
- pymorphy2
- math
- os
- tkinter
Only pymorphy2 is not a built-in library. If it is not installed, then you can install it with
the following command: `pip install pymorphy2`
If pip is not installed, then you can put it manually: https://pypi.org/project/pip 
<br><br>

### Launch:
The launch is carried out via: `python Main.py` <br>
While the program is running, two text files are required - a text template for modification and a text
for a dictionary of words.<br>
For convenience, the test versions of the texts are located in the folder `/tests` <br>
Example of a text template to change: `tests/text_template_to_change.txt`
Examples of texts for a dictionary of words:<br>
`tests/text_for_dictionary_words.txt` - excerpt from Theodore Dreiser's book "The Financier".<br>
`tests/text_for_dictionary_words_2.txt` - Mikhail Lermontov's poem "Motherland".<br>
The result is printed on the screen and also formed as a text file `result.txt`, located at the root,
next to Main.py and others.<br><br>

### Settings:
In this project, in the `Settings` section, you can select the parameters of the "writer" of the text.<br>
For example, you can choose which parts of speech to consider for modification. Type of replacement:
flexible, strict and whether it is necessary to remember the matches.<br> 
And you can also customize the selection by the length of words (check for equality, do not check and
replace with a large one).

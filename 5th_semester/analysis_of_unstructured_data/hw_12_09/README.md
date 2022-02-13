## Text generator based on the Markov chain

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

### Description of the offer generation algorithm:
The text on which the program is taught is divided into sentences. Each sentence is processed by one of
filtering functions present in the program. The name of the function is set by the user. Next, each processed
sentence is divided into key words (by space). The beginning of each sentence is additionally marked with
key `{ss}`.<br><br>

A Markov chain is constructed based on the received keys, which can take into account an arbitrary number
of words in the chain. The number of words counted is set by the user.<br><br>

A Markov chain is a hash table (dictionary, dict in python), each key of which is a specific chain of
user-defined length. The elements corresponding to the keys contain hash tables, each key of which is a word,
and the element corresponding to the key is a number – how many times in the text on which the program is
trained, this word was encountered after this chain.<br><br>

<b>Example:</b> let the user set the length of the chain = 2. Let the word "business" occur 3 times in
the text after the combination of the words "hello how", the word "himself" 1 time and the beginning of
the next sentence 1 time, then the fragment of the resulting hash table corresponding to the chain
`("hello", "how")` will have the form:<br>
`{..., ("hello", "how"): {"business": 3, "myself" 1, "{ss}": 1}, ...}`<br><br>

Additionally, a hash table of chains is created from which a sentence can begin (such chains, where the first
element is the key `{ss}`). In it, the keys are chains from which a sentence can begin, and the elements
corresponding to the keys are numbers – how many times in the text on which the program is trained,
the sentence began with such a chain (if the length of the chain = 1, then this is only a chain of one element -
`({ss})`.<br><br>

<b>Example:</b> let the user set the length of the chain = 2. Let 3 sentences begin with the word "hello" in
the text, and 2 sentences begin with the word "hello". Let the sentences not begin with other words.
Then the resulting hash table will be as follows:<br>
`{("{ss}", "hello"): 3, ("{ss}", "hello"): 2}`<br><br>

The generation of the proposal begins with a weighted random selection of a chain of keys from the hash table
of the starting chains. For example, for the above option, the probability is to choose the starting chain
`("{ss}", "hello")` there will be 3/5, and the probability of choosing a starting chain `("{ss}", "hello")`
it will be 2/5. All words from the chain, except for the key `{ss}`, are entered into the generated sentence.
We mark the selected chain as being processed at the current step.<br><br>

Next, at each step of the generation algorithm, we look for the processed chain in the Markov chain (the hash
table described earlier), and select a word from it, again by weighted random selection. We enter this word
into the sentence, and change the chain processed at the current step so that all the words in it move to left
by 1, and the last word becomes the selected one. If the last word is the key `{ss}`, then we complete
the generation.<br><br>

<b>Example:</b> consider the following Markov chain (hash table):

```none
{
    ("{ss}", "Hello"): {"as": 3, "Boris": 1, "{ss}": 1},
    ("Hello", "how"): {"cases": 2, "Boris": 1},
    ("like", "do"): {"{ss}": 2},
    ("like", "Boris"): {"{ss}": 1},
    ("Hello", "Boris"): {"{ss}": 1}
}
```

The starting chain in this variant can only be selected `("{ss}", "hello")`. The next word with probability
3/5 will be `like`, and with probability 1/5 will be `Boris`. With a probability of 1/5, the next word will be
the key `{ss}`. If the key `{ss}` is selected, then the sentence will end, and you will get a sentence of
one word: "hello". Let the word `as` be chosen, then the chain in question will change from `("{ss}", "hello")`
on `("hello", "how")`. We look at it: according to it, the probability of choosing the next word `deeds` will
be 2/3, and the word `Boris` - 1/3. Let the word `Boris` be chosen, then the chain in question will change
from `("hello", "how")` on `("how", "Boris")`. This chain in our Markov chain has only one variant of the
following word - the key `{ss}`. So, only he will always be selected. After that, the generation algorithm
will be completed, since the key `{ss}` was selected as the last word, and we will get the sentence:
`hello, how is Boris`.<br><br>

There are also minor modifications in my algorithm – if the sentence length (in words) has reached the
maximum length set by the user, and we can choose the key word `{ss}`, then we always choose it.
If the length of the sentence (in words) is less than the minimum length set by the user, we chose the key
`{ss}`, but we can choose a key other than `{ss}`, then we try to choose the key until it becomes different
from `{ss}` (in general, we can optimize by conducting weighted random selection on keys that lack the key
`{ss}`, but I did not have serious performance problems because of this). If, in the end, the length of
the resulting sentence, taking into account these modifications, turned out to be less than the minimum
or greater than the maximum length specified by the user, then we throw out this sentence and try to generate
the sentence again. All "good" sentences satisfying the length restrictions are displayed on the screen and
in a file specified by the user.<br><br>

### User-defined parameters:
`settingsFileName` - name of the file from where the program will try to read the parameters listed below.
If the parameters do not need to be loaded from a file, then this parameter must be assigned the value `undef`.<br><br>

Further, if the parameter is set to `undef`, that the program will try to load it from the file whose name
is set in the `settingsFileName` parameter. Format of the data in the file: on each line
`parameter name = value`. Example of contents of file with parameters:

```none
filterFuncName = extendedFilterRU
trainFileName = train/war-and-peace.txt
previousCnt = 2
sentanceCnt = 10
maxSentanceLen = 15
minSentanceLen = 5
resultFileName = result.txt
```

`filterFuncName` - name of the filtering function. You can only use the filtering functions that are in the program
(noFilter, basicFilter, basicFilterRU, extendedFilterRU). For more information about them, see
functions section.<br>
`trainFileName` - name of the file with the text on which the program is trained.<br>
`resultFileName` - name of the file where we save the result of the program. If the result does not need to
be saved to a file, or this parameter needs to be read from a file, then this parameter must be assigned
the value `undef`.<br>
`previousCnt` – length of the chain (the number of previous words taken into account when constructing
the Markov chain).<br>
`sentanceCnt` - number of offers to be generated.<br>
`maxSentanceLen` - maximum sentence length (in words).<br>
`minSentanceLen` - minimum sentence length (in words).<br>

### Libraries used:
`import random` – hence the pseudorandom number generator is used (function random.randint).

### Filtering functions:
`noFilter` – no filtering. We divide the entire text into words in its original form (words from a single
punctuation mark or numbers, identifiers may turn out, I do not recommend using them).<br>
`basicFilter` – the most primitive filtering. Only letter characters are left, all sequences of
whitespace characters are replaced by one space.<br>
`basicFilterRU` – the same as basicFilter, but we leave only Russian letters. Also, all Russian letters
are translated into lowercase.<br>
`extendedFilterRU` – improved filtering function. Leaves Russian letters, translating them into lowercase,
as well as hyphens in words, commas that stand at the end of words, and punctuation marks of the strict end of
the sentence (?, !, ...). With this function, the generated text turns out to be the most qualitative and
readable, however, a little closer to the text on which the program was trained, since the key words are
taken into account along with punctuation marks.

### Other functions:
`loadParams` – function for loading parameters<br>
`RU` – function to check whether a character is a Russian letter<br>
`getSentances` – function for splitting text into sentences<br>
`sentanceToKeys` – function for splitting a sentence into words (keys). Splits by the space character.
Additionally, a key is added-the beginning of the sentence.<br>
`buildMarkovChain` – function for constructing a Markov chain<br>
`getRandomKey` – function for getting a random word from a dictionary (hash table) given to it.
Function takes into account the "weight" of each word when randomly selected.

### Conclusion:
The longer the chain length, the more meaningful the sentences are, but, at the same time, they correspond more
and more to the text on which the program was trained. With long chain lengths, we are likely to get sentences
from this text. For the algorithm to work well, a sufficiently voluminous text is needed on which it will
be trained, and, at the same time, not too long a chain when generating sentences.

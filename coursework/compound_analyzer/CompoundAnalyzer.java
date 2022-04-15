import java.io.*;
import java.util.Vector;

/**
 * Класс содержит набор методов
 * для выделения префикса в заданном слове,
 * а также методы работы со строками, которые
 * используются при обработке префиксов
 */
public class CompoundAnalyzer {

    /** Начало строки запроса на анализ данного слова */
    private final static String ANALYZE_BEFORE = "<analyze form=\"";

    /** Окончание строки запроса на анализ данного слова */
    private final static String ANALYZE_AFTER = "\">";

	/** Префиксы числительных */
    private static String[] numbers = {
    	"тысяче",
        "ста","двухсот","трехсот","четырехсот","пятисот",
		"шестисот","семисот","восьмисот","девятисот",
		"двадцати","тридцати","сорока","пятидесяти",
		"шестижесяти","семидесяти","восьмидесяти","девяноста",
		"одно","двухъ","трехъ","четырехъ","двух","трех","четырех",
		"пяти","шести","семи","восьим","девяти",
		"десяти","одиннадцати","двенадцати","тринадцати","четырнадцати",
		"пятнадцати","шестнадцати","семнадцати","восемнадцати","девятнадцати"
	};

    /** Вектор стандартных префиксов */
    private static Vector prefixList = new Vector();

    // блок статической инициализации вектора prefixList
    static {
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new FileReader("prefixes.dat"));
			String line;
			while ((line = reader.readLine()) != null) {
				prefixList.add(line);
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
			try {
				if (reader != null) {
					reader.close();
				}
			} catch (IOException ex) {
			}
		}
    }

	/**
    * Распознает префикс-числительное
	*/
    private static boolean findNumberPrefix(Word word) {
		for(int i = 0; i < numbers.length; i++) {
			if (word.whole.startsWith(numbers[i])) {
				word.prefix = numbers[i];
				word.suffix = word.whole.substring(numbers[i].length());
				return true;
			}
		}
		return false;
	}

	/**
    * Распознает префикс из стандартного списка префиксов
	*/
    private static boolean findStandartPrefix(Word word) {
        for(int i = 0; i < prefixList.size(); i++) {
            String prefix = (String)prefixList.get(i);
            if (word.whole.startsWith(prefix)) {
                word.prefix = prefix;
                word.suffix = word.whole.substring(prefix.length());
                return true;
            }
        }
        return false;
	}

	/**
    * Распознает префикс, отделенный дефисом
	*/
    private static boolean findDefisPrefix(Word word) {
        int i = word.whole.lastIndexOf("-");
        if (i != -1) {
            word.prefix = word.whole.substring(0, i + 1);
            word.suffix = word.whole.substring(i + 1);
            return true;
        } else {
            return false;
        }
	}

	/**
    * Распознает первый префикс в заданном слове
	*/
    public static boolean findPrefix(Word word) {
        return findDefisPrefix(word) || findStandartPrefix(word) || findNumberPrefix(word);
    }


	/**
    * В заданной строке (str) ко всем словам, взятым в кавычки
    * и начинающимся на ту же букву, что и заданное слово (word)
    * приписывает спереди заданный префикс (prefix).
    * Возвращает получившуюся строку
	*/
    public static String insertPrefix(String str, String word, String prefix) {
        String quotedWordLetter = "\"" + word.substring(0,1);
        int index = -2;
        while((index = str.indexOf(quotedWordLetter, index + 2)) != -1) {
            str = str.substring(0, index + 1) + prefix + str.substring(index + 1);
        }
        return str;
    }

    /** Выделяет из заданной строки подстроку, ограниченную кавычками */
    public static String extractQuotedString(String str) {
        return str.substring(str.indexOf("\"") + 1, str.lastIndexOf("\""));
    }

    /** Обращается к RMU-серверу с заданным словом */
    public static String composeXMLRequest(String word) throws IOException {
        return ANALYZE_BEFORE + word + ANALYZE_AFTER;
    }
}

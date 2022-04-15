/**
 * Класс, представляющий информацию о префиксе и суффиксе слова
 */
public class Word {
    
    /** Все слово целиком */
	public String whole;
    
    /** Префикс */
    public String prefix;
    
    /** Суффикс */
	public String suffix;

    /** Конструктор */
    public Word(String str) {
        whole = str;
    }
}

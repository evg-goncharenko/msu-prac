from pymorphy2 import MorphAnalyzer
morph = MorphAnalyzer()

f_accuracy = False     # отвечает за точность признаков и соответственно на частоту неудач при заменах
f_type_dict = False    # отвечает за вид словаря (построение соответствия между заменяемыми словами или нет)
f_length = 'no'        # отвечает за режим замены слова по длине ('no', 'eq', 'gr')

# Множество частей речи:
POS = set(['NOUN', 'ADJF', 'ADJS', 'COMP', 'VERB', 'INFN', 'PRTF', 'PRTS', 'GRND', 'NUMR', 'ADVB', 'NPRO', 'PRED', 'INTJ'])

# Множество частей речи, которые будем рассматривать для изменения слов:
MODE_POS = POS.copy()

# Создает из списка частей речи те, которые будут использованы для изменения слов:
def new_set(s):
    if 'ADJF' in s:
        s.update(set(['ADJF', 'ADJS', 'COMP']))
    if 'VERB' in s:
        s.update(set(['VERB', 'INF']))
    if 'PRTF' in s:
        s.update(set(['PRTF', 'PRTS']))
    s.discard('')
    return s

# Функция для создания ключа части речи и ее постоянных признаков для хранения в словаре
def get_key(p):
    pos = p.tag.POS
    form = str(pos)
    
    if pos in {'ADJS', 'COMP', 'ADJF'}:
        form = str('ADJF')
    elif pos == 'NOUN':
        form += ' ' + str(p.tag.gender)
        form += ' ' + str(p.tag.animacy)
    elif pos in {'INFN', 'VERB'}: 
        form = str('VERB')
        form += ' ' + str(p.tag.aspect)
        form += ' ' + str(p.tag.transitivity)
    elif pos in {'PRTS', 'PRTF'}:
        form = str('PRTF')
        form += ' ' + str(p.tag.aspect)
        form += ' ' + str(p.tag.tense)
        form += ' ' + str(p.tag.voice)
    elif pos == 'GRND': 
        form += ' ' + str(p.tag.aspect)
        form += ' ' + str(p.tag.transitivity)
    return form

# Функция для создания множества непостоянных признаков части речи
def get_signs(p):
    global f_accuracy

    if f_accuracy:
        pos = p.tag.POS
        form = set()
        if pos in {'ADJF', 'NOUN', 'PRTF'}:
            form.add(str(p.tag.case))
            form.add(str(p.tag.number))
            form.add(str(p.tag.gender))
        elif pos in {'VERB', 'GRND'}: 
            form.add(str(p.tag.person))
            form.add(str(p.tag.tense))
            form.add(str(p.tag.mood))
            form.add(str(p.tag.number))
            form.add(str(p.tag.gender))
        else:
            form = set(str(p.tag).replace(' ', ',').split(','))

        form.discard(str(None))
    
    else:
        form = set(str(p.tag).replace(' ', ',').split(','))
    
    return form

# Функция извлечения слова-пары из словаря 2-го текста
def get_pair(wset, l):
    global f_length
    pair = ''
    
    if f_length == 'no' and len(wset):
        pair = wset.pop()
    elif len(wset):
        wlist = list(wset)
        wlist.sort(key=len(wlist))
        i = 0
        if f_length == 'eq':            # когда длина должна быть равна
            for i in range(len(wlist)):
                if len(wlist[i]) == l:
                    break
        elif f_length == 'gr':          # когда длина должна быть больше
            for i in range(len(wlist)):
                if len(wlist[i]) > l:
                    break
        if i != len(wlist):
            pair = wlist.pop(i)
            wset = set(wlist)
    return pair

def forms(word, pair, pos):
    formw = pair
    if pos in {'ADJS', 'COMP', 'ADJF', 'NOUN', 'INFN', 'VERB'}:
        p = morph.parse(pair)[0]
        ppos =  p.tag.POS
        if pos == 'NOUN' and ppos != pos:
            if ppos in {'ADJS', 'COMP', 'ADJF'}:
                formw = pair[:len(pair)-2]
            else:
                formw = pair[:len(pair)-1]
        elif pos in {'ADJS', 'COMP', 'ADJF'} and ppos!= pos:
            if ppos == 'NOUN':
                if pair[len(pair)-1] in 'уеыаояиью':
                    formw = pair[:len(pair)-1] + 'ий'
                else:
                    formw = pair + 'ый'
            else:
                end = pair[len(pair)-2:]
                if end == 'ть':
                    formw = pair[:len(pair)-2] + 'нный'
                elif end == 'ти':
                    formw = pair + 'й'
                else:
                    formw = pair[:len(pair)-2] + 'чной'
        elif pos in {'INFN', 'VERB'} and ppos!= pos:
            if ppos == 'NOUN':
                if pair[len(pair)-1] in 'уеыаояию':
                    formw += 'ть'
                elif pair[len(pair)-1] == 'ь':
                    formw = pair[:len(pair)-1] + 'ить'
                else:
                    formw = pair[:len(pair)] + 'ый'
            else:
                end = pair[len(pair)-2:]
                if end == 'ый':
                    formw = pair[:len(pair)-2] + 'еть'
                elif end == 'ий':
                    formw = pair[:len(pair)-1] + 'ть'
                else:
                    formw = pair[:len(pair)-1] + 'чь'

    return formw


# Функция добавления слова в словарь (по частям речи)
def add_word(word, dic):
    global MODE_POS
    p = morph.parse(word)[0]
    word = p.normal_form
    if p.tag.POS in MODE_POS:
        pos = get_key(p)
        words = dic.get(pos, set())
        words.add(word)
        dic[pos] = words

# Функция добавления пары в словарь на основе словаря от 2-го текста
def make_pair(word, rel, dic):
    p = morph.parse(word)[0]
    nform = p.normal_form
    if not (nform in rel) and p.tag.POS in MODE_POS:
        pos = get_key(p)
        wset = dic.get(pos, set())
        pair = get_pair(wset, len(word))
        if len(pair):
            pair = forms(word, pair, p.tag.POS) 
            rel[nform] = pair
            dic[pos] = wset

# Функция для замены слова на его пару
def change_to_pair(word, dic):
    global f_type_dict
    p = morph.parse(word)[0]
    if p.tag.POS in MODE_POS:
        if f_type_dict:
            key = p.normal_form
            value = dic.get(key, '')
        else:
            value = ''
            key = get_key(p)
            wset = dic.get(key, set())
            pair = get_pair(wset, len(word))
            if len(pair):
                value = forms(word, pair, p.tag.POS)
                dic[key] = wset              
        
        if len(value):
            form = get_signs(p)
            p = morph.parse(value)[0]
            s = str(p.inflect(form))
            if s != 'None':
                word = s[12:s.find(',')-1]  # срез равный замененному слову
    return word
               
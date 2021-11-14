import TheWriter as tw # подключение нашего модуля
from math import floor # для округления в меньшую сторону
import os.path # для проверки существования пути к файлу
from tkinter import * # Tkinter - графический пользовательский интерфейс

result_text = '' # преобразованное предложение
basetext = '' # текст для базы

f1 = 'tests/text_template_to_change.txt'    # шаблон текста для изменения
f2 = 'tests/text_for_dictionary_words.txt'  # текст для словаря слов
resfile = 'result.txt'                      # текст для записи результата

# информация о программе
prog_info = 'Программа - "сочинитель" текста заданного вида. Изменяется заданный текст по определенным правилам:\n1) Из текста выбираются фразы, соответствующие заданному виду, стилю (длина, части речи, длина слов и т.п.).\n2) В каждой фразе несколько слов заменяются на более подходящие (формально, по стилю, например, на более длинные).\n3) Слова берутся из неиспользованных фраз текста, ставятся в нужной форме.\n4) Четко выполняется соответствие по морфологическим характеристикам (понятно, что семантически может быть бессмыслица,\nно грамматически текст верный).\nПример "Петя рисует дом" - "Интеграл усиливает простуду".'

# Пересчет координат по x:
def real_x(w):
    return floor(w * h_coefficient)

# Пересчет координат по y:
def real_y(w):
    return floor(w * w_coefficient)

# Пересчет размера шрифтов:
def real_font(f):
    f, s = f.split()
    real_size = floor(int(s) * h_coefficient)
    return f + " " + str(real_size)

# Динамическое пересчитывание высоты и ширины:
class ResizingCanvas(Canvas):
    def __init__(self,parent,**kwargs):
        Canvas.__init__(self,parent,**kwargs)
        self.bind("<Configure>", self.on_resize)
        self.height = self.winfo_reqheight()
        self.width = self.winfo_reqwidth()

    def on_resize(self,event):
        # Определение соотношений старой ширины/высоты к новой ширине/высоте
        wscale = float(event.width)/self.width
        hscale = float(event.height)/self.height
        self.width = event.width
        self.height = event.height
        self.config(width=self.width, height=self.height) # изменяем размер 
        self.scale("all",0,0,wscale,hscale) # изменяем масштаб всех объектов, помеченных тегом "all"

# Очистка экрана:
def cleaning():
    c.delete('clean')
    main_title.place_forget()
    rule_title.place_forget()
    settings_title.place_forget()
    rules.place_forget()
    settings.place_forget()
    play.place_forget()
    backtom.place_forget()
    generate.place_forget()
    soursef.place_forget()
    destf.place_forget()
    ok.place_forget()
    show.place_forget()
    hide.place_forget()
    textres.place_forget()
    textrules.place_forget()
    textbase.place_forget()

# Главная страница:
def home_page():
    cleaning() # очистка экрана
    main_title.place(relx=0.5, rely= 0.1, anchor = 'n')
    play.place(relx=0.5, rely= 0.3, anchor = 'n')
    settings.place(relx=0.5, rely=0.4, anchor = 'n')
    rules.place(relx=0.5, rely= 0.5, anchor = 'n')
    exit.place(relx=0.98, rely= 0.9, anchor = 'ne')
    c.update()

# Страница Запуск:
def launch():
    cleaning() # очистка экрана
    
    # генерация сообщения о состоянии настроек
    s = 'Замена: '
    s += 'гибкая' if F_change.get() == 1 else 'строгая'
    s += '\nДлина: '
    s += 'учитывается' if F_l.get() != 0 else 'не учитывается'
    s += '\nПары: '
    s += 'запоминать' if F_pairs.get() == 1 else 'не составлять'
    
    main_title.place(relx=0.5, rely= 0.1, anchor = 'n')
    c.create_rectangle(real_x(250), real_y(900), real_x(1600), real_y(300), fill = 'gray95', outline = 'light gray', width = real_x(3), tag = "clean")
    c.create_text(real_x(900), real_y(330), text = 'Выставлены параметры:', font = real_font('Arial 40'), anchor = 'n', justify = CENTER, fill="black", tag= "clean")
    c.create_text(real_x(800), real_y(430), text = s, font = real_font('Arial 20'), anchor = 'nw', fill="black", tag= "clean")
    generate.place(relx=0.5, rely= 0.65, anchor = 'n')
    backtom.place(relx=0.5, rely= 0.85, anchor = 'n')
    c.update()
   
# Страница Настройки:
def settings():
    cleaning() # очистка экрана
    
    settings_title.place(relx=0.5, rely= 0.1, anchor = 'n')
    c.create_rectangle(real_x(250), real_y(900), real_x(1600), real_y(300), fill = 'gray95', outline = 'light gray', width = real_x(3), tag = "clean")
    c.create_text(real_x(290), real_y(330), text = 'Части речи:', font = real_font('Arial 20'), anchor = 'nw', fill="Black", tag= "clean")
    checknoun.place(relx=0.15, rely= 0.32, anchor = 'nw')
    checkadg.place(relx=0.15, rely= 0.35, anchor = 'nw')
    checkverb.place(relx=0.15, rely= 0.38, anchor = 'nw')
    checkprtf.place(relx=0.15, rely= 0.41, anchor = 'nw')
    checkgrnd.place(relx=0.15, rely= 0.44, anchor = 'nw')
    checknumb.place(relx=0.15, rely= 0.47, anchor = 'nw')
    checkadvb.place(relx=0.15, rely= 0.5, anchor = 'nw')
    checknpro.place(relx=0.15, rely= 0.53, anchor = 'nw')
    checkpred.place(relx=0.15, rely= 0.56, anchor = 'nw')
    checkintw.place(relx=0.15, rely= 0.59, anchor = 'nw')
    checkpairs.place(relx=0.44, rely= 0.41, anchor = 'nw')
    c.create_text(real_x(820), real_y(330), text = 'Замена:', font = real_font('Arial 20'), anchor = 'nw', fill="Black", tag= "clean")
    rbutton1.place(relx=0.44, rely= 0.33, anchor = 'nw')
    rbutton2.place(relx=0.44, rely= 0.37, anchor = 'nw')
    c.create_text(real_x(1200), real_y(330), text = 'Выбор по длине слова:', font = real_font('Arial 20'), anchor = 'nw', fill="Black", tag= "clean")
    lbutton1.place(relx=0.65, rely= 0.33, anchor = 'nw')
    lbutton2.place(relx=0.65, rely= 0.37, anchor = 'nw')
    lbutton3.place(relx=0.65, rely= 0.41, anchor = 'nw')
    
    saves.place(relx=0.5, rely= 0.82, anchor = 'n')
    c.update()

# Страница О программе:
def about_the_program():
    cleaning() # очистка экрана

    rule_title.place(relx=0.5, rely= 0.1, anchor = 'n')
    textrules.place(relx=0.5, rely= 0.25, anchor = 'n')
    backtom.place(relx=0.5, rely= 0.65, anchor = 'n')
    c.update()

# Сохранение настроек:
def saving_settings():
    # Обновляем флаги в модуле TheWriter
    tw.MODE_POS = tw.new_set(set([noun.get(), adg.get(), verb.get(), prtf.get(), grnd.get(), numb.get(), advb.get(), npro.get(), pred.get(), intw.get()]))
    tw.f_type_dict = True if F_pairs.get() == 1 else False
    tw.f_accuracy = True if F_change.get() == 1 else False
    if F_l.get() == 1:
        tw.f_length = 'eq'
    elif F_l.get() == 2:
        tw.f_length = 'gr'
    else:
        tw.f_length = 'no'
    
    # Очищаем окно:
    saves.place_forget() 
    checknoun.place_forget()
    checkadg.place_forget()
    checkverb.place_forget()
    checkprtf.place_forget()
    checkgrnd.place_forget()
    checknumb.place_forget()
    checkadvb.place_forget()
    checknpro.place_forget()
    checkpred.place_forget()
    checkintw.place_forget()
    checkpairs.place_forget()
    rbutton1.place_forget()
    rbutton2.place_forget()
    lbutton1.place_forget()
    lbutton2.place_forget()
    lbutton3.place_forget()
    
    home_page() # переходим на главную страницу

# Генерация имен файлов и состояний:
def set_states():
    cleaning() # очистка экрана

    main_title.place(relx=0.5, rely= 0.1, anchor = 'n')
    c.create_rectangle(real_x(250), real_y(900), real_x(1600), real_y(300), fill = 'gray95', outline = 'light gray', width = real_x(3), tag = "clean")
    c.create_text(real_x(300), real_y(320), text = 'Шаблон текста для изменения:', font = real_font('Arial 40'), anchor = 'nw', fill="Black", tag= "clean")
    c.create_text(real_x(1050), real_y(320), text = 'Текст для словаря слов:', font = real_font('Arial 40'), anchor = 'nw', fill="Black", tag= "clean")
    c.create_text(real_x(300), real_y(400), text = 'Имя файла:', font = real_font('Arial 20'), anchor = 'nw', fill="Black", tag= "clean")
    c.create_text(real_x(1050), real_y(400), text = 'Имя файла:', font = real_font('Arial 20'), anchor = 'nw', fill="Black", tag= "clean")
    soursef.delete(0, last=END)
    destf.delete(0, last=END)
    soursef.insert(0, f1)
    destf.insert(0, f2)
    soursef.place(relx=0.163, rely= 0.4, anchor = 'nw')
    destf.place(relx=0.575, rely= 0.4, anchor = 'nw')
    ok.place(relx=0.5, rely= 0.65, anchor = 'n')
    c.update()

# Получение данных из файлов:
def get_data():
    global f1, f2
    s1, s2 = soursef.get(), destf.get()
    is1 = is2 = True
    c.delete('error')
    if s1:
        is1 = os.path.exists(s1)
        if is1:
            f1 = s1
        else:
            c.create_text(real_x(410), real_y(500), text = 'Нет такого файла!', font = real_font('System 20'), anchor = 'nw', justify = CENTER, fill="red", tag= "error")
    if s2:
        is2 = os.path.exists(s2)
        if is2:
            f2 = s2
        else:
            c.create_text(real_x(1160), real_y(500), text = 'Нет такого файла!', font = real_font('System 20'), anchor = 'nw', justify = CENTER, fill="red", tag= "error")
    if is1 and is2:
        c.delete('error')
        gen()

# Отображение базы:
def show_base():
    global basetext, c
    
    c.delete('clean')
    show.place_forget()
    hide.place(relx=0.5, rely= 0.9, anchor = 'n')
    textres.configure(width=real_x(80))
    textres.place(relx=0.075, rely= 0.35, anchor = 'nw')
    textbase.delete('1.0', END)
    textbase.insert(1.0, basetext)
    textbase.place(relx=0.57, rely= 0.35, anchor = 'nw')
    c.create_text(real_x(900), real_y(270), text = 'Результат сохранен в файл: ' + resfile, font = real_font('Arial 40'), anchor = 'n', justify = CENTER, fill="black", tag= "clean")
    c.create_text(real_x(460), real_y(345), text = "Сформированный текст", font = real_font('System 30'), anchor = 'n', justify = CENTER, fill="SteelBlue4", tag= "clean")
    c.create_text(real_x(1355), real_y(345), text = "Изначальный текст", font = real_font('System 30'), anchor = 'n', justify = CENTER, fill="SteelBlue4", tag= "clean")
    c.update

# Отображение результата:
def show_res():    
    global result_text, resfile, c
    
    c.delete('clean')
    textbase.place_forget()
    hide.place_forget()

    main_title.place(relx=0.5, rely= 0.1, anchor = 'n')
    textres.configure(width=real_x(120))
    textres.delete('1.0', END)
    textres.insert(1.0, result_text)
    textres.place(relx=0.5, rely= 0.36, anchor = 'n')
    c.create_text(real_x(900), real_y(270), text = 'Результат сохранен в файл: ' + resfile, font = real_font('Arial 40'), anchor = 'n', justify = CENTER, fill="black", tag= "clean")
    c.create_text(real_x(900), real_y(340), text = "Сформированный текст", font = real_font('System 30'), anchor = 'n', justify = CENTER, fill="SteelBlue4", tag= "clean")
    backtom.place(relx=0.75, rely= 0.9, anchor = 'n')
    generate.place(relx=0.25, rely= 0.9, anchor = 'n')
    show.place(relx=0.5, rely= 0.9, anchor = 'n')
    c.update

# Создаем окно приложения
root = Tk()
myframe = Frame(root)
myframe.pack(fill=BOTH, expand=YES)
c = ResizingCanvas(myframe,width=1920, height=1080, highlightthickness=0)
c.pack(fill=BOTH, expand=YES)

root.title("Генератор текстов")

real_width = floor(root.winfo_screenwidth() * 0.95)   # получаем реальную ширину окна
real_height = floor(root.winfo_screenheight() * 0.95) # получаем реальную высоту окна

width = 1920  # стандарт ширины приложения
height = 1080 # стандарт высоты приложения

w_coefficient = real_width / width
h_coefficient = real_height / height

# Формируем виджеты:
main_title = Label(c, text='Сочинитель', font = real_font('Impact 103'), anchor = 'n', justify = CENTER, fg='SteelBlue4')
rule_title = Label(c, text='О программе', font = real_font('Impact 103'), anchor = 'n', justify = CENTER, fg='SteelBlue4')
settings_title = Label(c, text='Настройки', font = real_font('Impact 103'), anchor = 'n', justify = CENTER, fg='SteelBlue4')
settings_frame = Frame(c, width=1300, height=600, bg='white')
rules = Button(c, text='О программе', width=real_x(20), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = about_the_program)
settings = Button(c, text='Настройки', width=real_x(20), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = settings)
play = Button(c, text='Запуск', width=real_x(20), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = launch)
backtom = Button(c, text='На главную', width=real_x(20), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = home_page)
saves = Button(c, text='Сохранить', width=real_x(20), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = saving_settings)
generate = Button(c, text='Выбор файлов', width=real_x(20), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = set_states)
exit = Button(c, text='Выход', width=real_x(10), height=real_y(3), fg='black',font=real_font('Verdana 20'), command=root.destroy)

button_font = real_font("Arial 14")
noun, adg, verb, prtf, grnd, numb, advb, npro, pred, intw = StringVar(), StringVar(), StringVar(), StringVar(), StringVar(), StringVar(), StringVar(), StringVar(), StringVar(), StringVar()
checknoun=Checkbutton(c,text='существительные',variable=noun,onvalue='NOUN',offvalue='',font=button_font)
checkadg=Checkbutton(c,text='прилагательные',variable=adg,onvalue='ADJF',offvalue='',font=button_font)
checkverb=Checkbutton(c,text='глаголы',variable=verb,onvalue='VERB',offvalue='',font=button_font)
checkprtf=Checkbutton(c,text='причастия',variable=prtf,onvalue='PRTF',offvalue='',font=button_font)
checkgrnd=Checkbutton(c,text='деепричастия',variable=grnd,onvalue='GRND',offvalue='',font=button_font)
checknumb=Checkbutton(c,text='числительные',variable=numb,onvalue='NUMR',offvalue='',font=button_font)
checkadvb=Checkbutton(c,text='наречия',variable=advb,onvalue='ADVB',offvalue='',font=button_font)
checknpro=Checkbutton(c,text='местоимения-существительные',variable=npro,onvalue='NPRO',offvalue='',font=button_font)
checkpred=Checkbutton(c,text='предикативы',variable=pred,onvalue='PRED',offvalue='',font=button_font)
checkintw=Checkbutton(c,text='междометия',variable=intw,onvalue='INTJ',offvalue='',font=button_font)
noun.set('NOUN')
adg.set('ADJF')
verb.set('VERB')
prtf.set('PRTF')
grnd.set('GRND')
numb.set('NUMR')
advb.set('ADVB')
npro.set('NPRO')
pred.set('PRED')
intw.set('INTJ')
F_pairs, F_change, F_l = IntVar(), IntVar(), IntVar()
checkpairs=Checkbutton(c,text='запоминать соответствия',variable=F_pairs,onvalue=1,offvalue=0,font=button_font)
rbutton1=Radiobutton(c,text='гибкая',variable=F_change,value=1,font=button_font)
rbutton2=Radiobutton(c,text='строгая',variable=F_change,value=0,font=button_font)
lbutton1=Radiobutton(c,text='не проверять',variable=F_l,value=0,font=button_font)
lbutton2=Radiobutton(c,text='проверять равенство',variable=F_l,value=1,font=button_font)
lbutton3=Radiobutton(c,text='заменять большим',variable=F_l,value=2,font=button_font)

soursef = Entry(c, width=real_x(30), font = 'Arial 20')
destf = Entry(c, width=real_x(30), font = 'Arial 20')
ok = Button(c, text='Продолжить', width=real_x(13), height=real_y(2.5), fg='black', font=real_font('Verdana 30'), command = get_data)
show = Button(c, text='Показать начальный текст', width=real_x(30), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = show_base)
hide = Button(c, text='Скрыть начальный текст', width=real_x(30), height=real_y(3), fg='black',font=real_font('Verdana 20'), command = show_res)
textres = Text(c, height=real_y(30), width=real_x(150), fg='black', font = real_font('Times 20'), wrap = WORD)
textbase = Text(c, height=real_y(30), width=real_x(80), fg='black', font = real_font('Times 20'), wrap = WORD)
textrules = Label(c, text=prog_info, font = real_font('Arial 23'), anchor = 'n', justify = LEFT, fg='black')

home_page() # запускаем главую страницу

def gen():
    global result_text, basetext, f1, f2, resfile, c
    
    posdic = {} # словарь разобранный по частям речи 2-го текста
    relat = {}  # словарь отношений между словами 1-го и 2-го текста (вариативно)    
    
    cleaning() # очистка экрана

    # Окно загрузки:
    c.create_text(real_x(960), real_y(200), text = 'Формируем словари и строим предложения ...', font = real_font('Arial 40'), anchor = 'n', justify = CENTER, fill="Black", tag= "clean")
    c.update()
    
    # Открываем файл, начинаем строить словарь слов, которые мы позаимствуем из второго текста:
    result_text, basetext = '', ''
    
    file = open(f2, 'r')
    for s in file:
        s = s.strip()
        word = ''
        for ch in s:
            if ch.isalpha():
                word += ch
            elif word != '':
                tw.add_word(word, posdic)
                word = ''
        if word != '':
            tw.add_word(word, posdic)
    # сформировали словарь слов по частям речи:
    # posdic: {'NOUN': {'удивление', 'окно'}, ... }
    file.close()
    file = open(f1, 'r')
    
    dicpointer = relat if tw.f_type_dict else posdic # нужно ли делать словарь
    
    for s in file:
        basetext += s
        result = ''
        word = ''
        for ch in s:
            if ch.isalpha():
                word += ch
            elif word != '':
                if tw.f_type_dict:
                    tw.make_pair(word, relat, posdic)
                new = tw.change_to_pair(word, dicpointer)
                if word[0].isupper(): # если встречена большая буква (имена и начала предложений)
                    word = new[0].upper() + new[1:]
                else:
                    word = new
                result += word + ch
                word = ''
            else:
                result += ch
        if word != '':
            if tw.f_type_dict:
                    tw.make_pair(word, relat, posdic)
            new = tw.change_to_pair(word, dicpointer)
            if word[0].isupper():
                word = new[0].upper() + new[1:]
            else:
                word = new
            result += word
        result_text += result    
    file.close()

    file = open(resfile, 'w')
    for ch in result_text:
        file.write(ch)
    file.close()
    print('\n\nРезультат в файле:', resfile)    
    
    show_res()

c.addtag_all("all") # для выравниваний размеров объектов
root.mainloop()

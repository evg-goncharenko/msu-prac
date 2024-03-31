import ijson
import os

def parse_large_json(file_path):
    # Открытие файла для потоковой обработки
    answ_list = []
    
    with open(file_path, 'rb') as file:
        # Потоковая обработка элементов JSON
        
        parser = ijson.parse(file)
        
        for prefix, event, value in parser:
            # Тут нас интересуют элементы списков в json и событие по нахождению какого-либо ключа в этом списке
            if prefix.endswith('.item') and event == 'map_key':
                # Заготовка под объект json, который будем строить
                json_object = {}
                # Пока в текущем списке есть элементы, будем их анализировать
                
                while event == 'map_key':
                    # ijson идет итеративно по ключам и значениям
                    # поэтому сначала сохраняем уже полученный ключ,
                    # а потом запускаем следующую итерацию для получения значения
                    key = value
                    _, _, value = next(parser)
                    
                    answ_list.append(value)
                    json_object[key] = value
                    # Для продолжения нашего while нам нужно снова перейти на следующий ключ списка
                    # поэтому в этой итерации мы специально сеттим event (там как раз будет map_key)
                    prefix, event, value = next(parser)

                # С построенным объектом json можем делать что угодно, тут мы сверяем его с фильтром и записываем
    
    return answ_list

# script_dir - полный путь до posts.json
script_dir = os.path.dirname(os.path.realpath(__file__))

answ_list = parse_large_json(script_dir + '/answers_short.json')
quest_list = parse_large_json(script_dir + '/questions_short.json')

print(len(answ_list))
print(len(quest_list))

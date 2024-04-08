import ijson
import json
import os

def parse_large_json(file_path):
    with open(file_path, 'rb') as file:
        # Словарь для хранения вопросов и наилучших ответов
        qa_pairs = {}
        # Словарь для хранения максимального score у ответов по ParentId
        answers_max_scores = {}
        
        # Потоковая обработка элементов JSON
        for post in ijson.items(file, 'item'):
            post_type = post.get('PostTypeId')
            post_tag = post.get('Tags')
            
            # Формируем только пары вопрос-ответ связанные с темой python:
            if (post_tag is not None) and (post_tag.find('python') == -1):
                continue
            if post_type == '1':  # Вопрос
                # Инициализация с пустым ответом:
                qa_pairs[post['Id']] = [post['Title'] + ' ' + post['Body'], '']
            elif post_type == '2':  # Ответ
                parent_id = post.get('ParentId')
                score = int(post.get('Score', 0))
                
                # Обновляем лучший ответ, если текущий ответ имеет больший score:
                if parent_id in qa_pairs:
                    _, current_max_score = answers_max_scores.get(parent_id, ('', 0))
                    if score > current_max_score:
                        answers_max_scores[parent_id] = (post['Body'], score)
                        qa_pairs[parent_id][1] = post['Body']
    
    # Получаем список пар вопрос-ответ
    final_pairs = list(qa_pairs.values())
    
    questions = {}
    questions['questions'] = []
    
    answers = {}
    answers['answers'] = []
    
    count_write = 0
    size_file = 80
    count_files = 0
    
    for obj in final_pairs:
        count_write+=1
        
        if (obj[0] == '') or (obj[1] == ''): # избавляемся от пустых ответов
            continue
        
        questions['questions'].append({
            'value': obj[0][:200] # оставляем только первые 200 символов
        })
        answers['answers'].append({
            'value': obj[1][:200] # оставляем только первые 200 символов
        })
        
        if count_write % 1000 == 0:
            print ('Выполнили запись ', count_write, ' шагов')
        
        if len(questions['questions']) >= size_file:
            questions_file = open(script_dir + '/questions_' + str(count_files) + '.json', "w")
            answers_file = open(script_dir + '/answers_' + str(count_files) + '.json', "w")
            
            json.dump(questions, questions_file, ensure_ascii=False, indent=4)
            json.dump(answers, answers_file, ensure_ascii=False, indent=4)
            
            questions['questions'] = [] 
            answers['answers'] = []
            
            count_files += 1
        
        if count_files >= 4: # достаточно 4 файла вопросов и ответов
            break
    
    return final_pairs

# Получение полного пути к запускаемому файлу
file_path = os.path.realpath(__file__)

# script_dir - полный путь до posts.json
script_dir = os.path.dirname(os.path.realpath(__file__))

final_pairs = parse_large_json(script_dir + '/posts.json')

print('Создание и запись файлов прошла успешно')

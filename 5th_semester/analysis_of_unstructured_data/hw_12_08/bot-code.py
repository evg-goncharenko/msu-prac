from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model import LogisticRegression
from nltk.metrics.distance import edit_distance
import random
from telegram.ext import Updater, CommandHandler, MessageHandler, Filters
from somewhere import proxy_url, your_token
from botconfig import BOT_CONFIG

def get_intent(text):
    # Получение предсказания
    probas = CLF.predict_proba(VECTORIZER.transform([text]))
    max_proba = max(probas[0])
    if max_proba >= CLASSIFIER_THRESHOLD:
        index = list(probas[0]).index(max_proba)
        return CLF.classes_[index]

def get_answer_by_generative_model(text):
    # Получение ответа с помощью сгенерированной модели
    text = text.lower()
    for question, answer in GENERATIVE_DIALOGUES:
        if abs(len(text) - len(question)) / len(question) < 1 - GENERATIVE_THRESHOLD:
            dist = edit_distance(text, question)
            l = len(question)
            similarity = 1 - dist / l
            if similarity > GENERATIVE_THRESHOLD:
                return answer

def get_response_by_intent(intent):
    # Получение ответа на предсказании
    responses = BOT_CONFIG['intents'][intent]['responses']
    return random.choice(responses)

def get_failure_phrase():
    # Получение фраз о неудачах
    phrases = BOT_CONFIG['failure_phrases']
    return random.choice(phrases)

def generate_answer(text):
    # Получение предсказания
    intent = get_intent(text)
    # Формулировка ответа по сценарию:
    if intent:
        response = get_response_by_intent(intent)
        return response
    # Использование генеративной модели
    answer = get_answer_by_generative_model(text)
    if answer:
        return answer
    # Использование stub
    failure_phrase = get_failure_phrase()
    return failure_phrase

def start(update, context):
    # Отправка сообщения, когда получена команда /start
    update.message.reply_text('Hi!')

def help(update, context):
    # Отправка сообщения, когда получена команда /help
    update.message.reply_text('Help!')

def text(update, context):
    # Повторение сообщения пользователя
    answer = generate_answer(update.message.text)
    print(update.message.text, '->', answer)
    print()
    update.message.reply_text(answer)

def error(update, context):
    # Ошибки журнала, вызванные обновлениями:
    update.message.reply_text('Я работаю только с текстом')

def main():
    # Запуск бота:
    updater = Updater(your_token,
                      request_kwargs={'proxy_url': ''},
                      use_context=True)

    dp = updater.dispatcher
    dp.add_handler(CommandHandler("start", start))
    dp.add_handler(CommandHandler("help", help))
    dp.add_handler(MessageHandler(Filters.text, text))
    dp.add_error_handler(error)
    updater.start_polling()
    updater.idle()

CLASSIFIER_THRESHOLD = 0.15
GENERATIVE_THRESHOLD = 0.6
with open('dialogues.txt', encoding='utf-8') as f: # открытие сета диалогов для обучения
    data = f.read()
dialogues = []
for dialogue in data.split('\n\n'): # разбивает строки 
    replicas = [] # реплики
    for replica in dialogue.split('\n')[:2]:
        replica = replica[2:].lower()
        replicas.append(replica)
    if len(replicas) == 2 and 5 < len(replicas[0]) < 25 and 5 < len(replicas[1]) < 25:
        dialogues.append(replicas)
GENERATIVE_DIALOGUES = dialogues[:50000]
X_text = []  # обучение
y = []  # предсказание
for intent, value in BOT_CONFIG['intents'].items():
    for example in value['examples']:
        X_text.append(example)
        y.append(intent)
VECTORIZER = CountVectorizer()
X = VECTORIZER.fit_transform(X_text) # обучаем и трансформируем выборку
CLF = LogisticRegression(random_state=42) # использование метода логистической регрессии
CLF.fit(X, y) # обучение

# Запуск бота:
main()

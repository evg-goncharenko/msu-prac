import numpy as np
import matplotlib.pyplot as plt
from tensorflow import keras
from tensorflow.keras.layers import Dense
c = np.array([-3, -1, 0, 2,  4, 5, 6])   # вход
f = np.array([11, 7, 5, 1, -3, -5,  -7]) # выход   y=-2x+5
model = keras.Sequential()  # слои вид сети - последовательные, друг за другом 
model.add(Dense(units=1, input_shape=(1,), activation='linear'))
# Dense - конструктор, формирование полносвязного слоя
# add - добавление слоя с заданынми параметрами
# units - количество нейронов
# input_shape=(1,) - один выход
# activation - функция активации
model.compile(loss='mean_squared_error', optimizer=keras.optimizers.Adam(0.1))
# выбираем минимум среднего квадрата ошибки и оптимизацию по Adam
log = model.fit(c, f, epochs=200, verbose=False)
# fit - метод для обучения (выход, выход, к-во эпох, печатать ли текущую информацию
plt.plot(log.history['loss'])
plt.grid(True)
plt.show()
# график  показывает, как меняется loss - vj;yj bpvtybnm rjkbxtcndj 'gj[
print(model.predict([2]))  # печать предсказания модели для 2
print(model.predict([-10]))
print(model.predict([10]))
print(model.get_weights())  # при хорошей модели веса равны коэффициентам

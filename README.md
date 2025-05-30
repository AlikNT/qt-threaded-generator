# Qt Threaded Generator

🎲 **Многопоточный генератор случайных чисел на Qt5**

## 📌 Описание

Программа создаёт заданное пользователем количество потоков, каждый из которых генерирует случайные числа в диапазоне от 1 до 10. Потоки завершают работу при совпадении числа с целевым, по достижению общего лимита итераций, либо при нажатии кнопки "Стоп".

Результаты отображаются в таблице в виде:
- Время (с секундами)
- Номер потока
- Сгенерированное число
- Причина завершения потока

## 💡 Возможности

- Управление количеством потоков и итераций
- Квант времени генерации (1–9 секунд)
- Потокобезопасный счётчик итераций (`std::atomic`)
- Реализация на `QThread` с `QObject::moveToThread`
- Немедленное завершение по кнопке "Стоп"
- Безопасное удаление объектов и завершение потоков

## 🖥️ Как собрать

### В Qt Creator:

1. Открыть файл `QtThreadedGenerator.pro`
2. Нажать **Собрать** и затем **Запустить**

### В консоли:

```bash
qmake QtThreadedGenerator.pro
make
./QtThreadedGenerator
```

## 🛠 Зависимости

- Qt 5.x (Widgets, Core, GUI)
- C++11 или новее

## 📁 Структура

- `main.cpp` — точка входа
- `mainwindow.h/cpp` — интерфейс и логика UI
- `worker.h/cpp` — генератор, выполняющийся в потоке
- `mainwindow.ui` — форма UI
- `.pro` файл — сборка через qmake

## 📷 Интерфейс

Интерфейс содержит:
- Поля для ввода параметров
- Кнопки Старт и Стоп
- Таблицу с результатами

## 📄 .gitignore

Файл `.gitignore` добавлен, исключает сборочные файлы, `*.o`, `*.user`, `build*/` и прочее.

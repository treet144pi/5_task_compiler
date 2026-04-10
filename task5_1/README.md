# Loop Tree

Данная программа реализует построение дерева циклов на ориентированном графе

## Структура проекта

- `src/main.cpp` — точка входа
- `src/graph.cpp` — чтение графа
- `src/loops.cpp` — основной алгоритм
- `include/graph.hpp`
- `include/loops.hpp`
- `include/union_find.hpp` — union-find

## Сборка

```bash
mkdir build
cd build
cmake ..
make
```

## Запуск

./loop_analyzer


После запуска программа ожидает одну строку с ребрами графа

## Формат входа:
Одна строка, содержащая список дуг
{u,v}, {x,y}, ...
А также специальные вершины start и stop

## Формат вывода
- вершины, принадлежащие каждому циклу как самому внутреннему;
- пары {parent, child} для дерева циклов.

## Пример

Вход
{start,1}, {1,2}, {2,1}, {2,3}, {3,1}, {3,4}, {4,stop}


Вывод
Loop blocks:
[0] -> {start, 4, stop}
[1] -> {1, 2, 3}, header: 1, reducible
Loop tree:
{0,1}

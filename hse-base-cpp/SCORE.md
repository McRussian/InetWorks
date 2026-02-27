# Правила оценивания

Общая формула оценки:

![encoding](https://latex.codecogs.com/svg.image?0.275%20H%20+%200.625%20P%20+%200.1S)

где
- ![encoding](https://latex.codecogs.com/svg.image?H) - оценка за домашние задания
- ![encoding](https://latex.codecogs.com/svg.image?P) - оценка за проект
- ![encoding](https://latex.codecogs.com/svg.image?S) - оценка за работу на семинаре

Итоговая оценка округляется до ближайшего целого с помощью функции `std::round`.

## Домашние задания

Задача из домашнего задания считается сданной, если её решение проходит тесты и для неё закрыт Merge Request.

Начисление баллов за задачу:
- Задача, сданная до [дедлайна](https://base.cpp-hse.net/), оценивается в `100%` возможных баллов.
- Между дедлайном и неделей после дедлайна баллы убывают линейно от 100% до 0%.
- За задачу, не сданную через неделю после дедлайна, баллы не начисляются.

 Оценки за недельные ДЗ нормируются до `10` и вычисляются по формуле:

  ![encoding](https://latex.codecogs.com/svg.image?\frac{Sum_{week}}{Max}&space;\times&space;10), где ![encoding](https://latex.codecogs.com/svg.image?Sum_{week}) - число набранных студентом баллов за недельное ДЗ, ![encoding](https://latex.codecogs.com/svg.image?Max) - максимально возможное число баллов за задачи недели

Итоговый балл за ДЗ ![encoding](https://latex.codecogs.com/svg.image?H) является средней оценкой за недельные ДЗ и вычисляется по формуле:

  ![encoding](https://latex.codecogs.com/svg.image?\frac{Sum_{total}}{Weeks}), где ![encoding](https://latex.codecogs.com/svg.image?Sum_{total}) - сумма оценок за все недельные ДЗ, ![encoding](https://latex.codecogs.com/svg.image?Weeks) - количество выданных недельных ДЗ

## Проект

Правила начисления оценки за проект будут объявлены при публикации проекта.

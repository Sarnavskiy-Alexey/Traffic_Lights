# Тестовое задание

## Тема: "Разработка адаптивного алгоритма работы светофора"

### Описание

Адаптивный алгоритм работы светофоров призван оптимизировать общую пропускную способность перекрестка в зависимости от ситуации на перекрестке.

Для тестирования программы используется перекресток с 12 светофорами: 4 автомобильных и 8 пешеходных (по 1 с каждой стороны дороги для перехода на соседнюю сторону). Для автомобилей организовано движение по одной полосе в каждую сторону.

Автомобили пересекают перекресток либо в прямом направлении, либо с поворотом направо.

### Алгоритм работы

Светофоры на перекрестке являются самостоятельными, т.е. внешний интегратор отсутствует.

Для синхронизации светофоров используется внешняя функция, позволяющая передавать события от одного светофора остальным. Такой функцией может выступать, например, функция передачи по каким-либо интерфейсам: A429, A664 (AFDX), A825 (CAN) и т.п. Для симуляции разработана функция, перекладывающая сообщение от одного светофора другим.

В сообщении указывается идентификатор светофора-отправителя, количество объектов, находящихся в ожидании проезда светофора, и текущее состояние светофора (зеленый, желтый или красный).

Общий алгоритм работы каждого светофора следующий:

```
ЕСЛИ текущее состояние красное
  ИЛИ (текущее состояние желтое И предыдущее состояние красное)
ТО 
    ЕСЛИ в очереди текущего светофора есть объекты
      И среди светофоров_X нет зеленых
      И среди светофоров_X ожидающих в очереди не больше в 2 раза, чем у текущего светофора
      И среди светофоров_X нет тех, кто ожидает дольше положенного лимита
      И среди всех светофоров только красные
    ТО выполняется процедура перевода состояния в зеленый
ИНАЧЕ ЕСЛИ текущее состояние зеленое
  ИЛИ (текущее состояние желтое И предыдущее состояние зеленое)
ТО
    ЕСЛИ в очереди текущего светофора нет объектов
      ИЛИ среди светофоров_X есть зеленые
      ИЛИ среди светофоров_X ожидающих в очереди больше в 2 раза, чем у текущего светофора
      ИЛИ среди светофоров_X есть те, кто ожидает дольше положенного лимита
    ТО выполняется процедура перевода состояния в красный
```

где под ___светофоры_X___ подразумеваются те светофоры, которые _не могут_ быть в зеленом состоянии одновременно с текущим светофором.

Также в программе присутствует определение светофоров, которые _могут_ быть в зеленом состоянии одновременно с текущим светофором.

Список обозначенных выше светофоров задается перед запуском "перекрестка". Это важно для того, чтобы каждый светофор "видел" состояние других светоров и подстраивался под них таким образом, чтобы не нарушать безопасность дорожного движения. Пример нарушения БДД: одновременная перестановка зеленого состояния у автомобильного светофора и того пешеходного, который пересекает маршрут движения автомобилей.

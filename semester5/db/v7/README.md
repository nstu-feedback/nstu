# Базы данных (вариант 7) [2/8]

## ЛР 1
---

> Создать и заполнить базу данных для учета работы
> типографии, состоящую из пяти таблиц. Первая таблица должна содержать
> поля: идентификатор продукции, идентификатор типа продукции (например,
> этикетка, коробка, блокнот и т.п.), идентификатор материала, количество,
> стоимость заказа и другие поля при необходимости. Вторая: идентификатор
> заказчика, наименование заказчика, идентификатор города, где находится
> заказчик, способ расчета (наличный, безналичный) и другие поля при
> необходимости. Третья: идентификатор материала, тип используемого
> материала, количество материала, стоимость расходных материалов и другие
> поля при необходимости. Четвертая: справочник городов. Пятая: справочник
> типов продукции. На основании созданных таблиц создать таблицу,
> содержащую, например, поля: наименование заказчика, город, где находится
> заказчик, тип продукции, тип используемого материала, стоимость заказа.

## ЛР2
---
> Заполнить базу данных для учета работы типографии (при необходимости).
> Поле «количество» не должно содержать значений менее 100 шт.
> Поле «способ расчета» может содержать только наименования:
> наличный и безналичный. Стоимость заказа не должна быть отрицательной.
> Ограничить заполнение поля «тип продукции» следующими наименованиями:
> этикетка, коробка, блокнот. Продемонстрировать результаты работы.

## Воспроизведение
---

### __Linux/macos__

Экспорт переменных окружения для подключения к БД

```sh
export USERNAME=<username> # 'postgres' by default
export DATABASE=<database> # 'labs' by default
```

Выполнение скриптов:

```sh
make [{auth, all, lab1, lab2, etc…}]
```

### __Windows__

```sh
echo Соболезную
```
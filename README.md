# Consept2019
Архитектура х64. Все комманды 64 бита.
Подробнее - Lib\Defines.h

Fib.a - пример ассемблера.
Список команд и их номера в Lib\Command\CommandList.h
Большая часть похожа на at&t или понятна из наназвания.
Комманды интрепретируют все числа как беззнаковые.

() - раззыменование.
Поддержуются такие вещи ($rbx + 8 * $rcx).
То, что до умножения - константа не более 32 бита.
$ - регист.
& - ссылка на имя.
. - объявление имени.
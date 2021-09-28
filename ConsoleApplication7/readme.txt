Спецификация Маркасьян Данил КН-19-2

Очистка экрана и заливка цветом
clear color

Нарисовать пиксель
pixel x0 y0 color

Нарисовать линию
line x0 y0 x1 y1 color

Нарисовать прямоугольник
rect x0 y0 w h color

Нарисовать залитый прямоугольник
fill_rect x0 y0 w h color

Нарисовать эллипс
ell x0 y0 radius_x radius_y color

Нарисовать заполненый эллипс
fill_ell x0 y0 radius_x radius_y color

Нарисовать круг
circle x0 x0 radius color

Нарисовать заполненый круг
fill_circle x0 y0 radius color

Нарисовать прямоугольник с закругленными краями
round_rect x0 y0 w h radius color

Нарисовать заполненный прямоугольник с закругленными краями
fill_round_rect x0 y0 w h radius color

Вывести текст
text x0 y0 color font_size length text

Вывести изображение (max 127*127px)
img x0 y0 w h data //(data_length = w * h * sizeof(color))

Повернуть
orientation: angle //(0=0 1=90 2=180 3=270)

Получить ширину
get width 

Получить высоту
get height

	Параметры:
x0 x1 y0 y1 -- int16 (Координаты ограничены х – (0; 800), y – (0;600))
w h -- int16 (max w = 800; max h = 600)
radius radius_x radius_y -- int16
font_size length – int16
color -- RGB888(24 bits)


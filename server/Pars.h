#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <sstream>
#include <vector>
#include "draw.h"


using namespace std;

class Parser {
public:
    int const WIDTH = 800;
    int const HEIGHT = 600;
    string* err;
    char** words;
    draw bmp;
    void add_line() {
        *err = *err + "--------------------------------------\n";
    }
    Parser() {
    }
    //возврат большего числа
    int min_of_2(int a, int b)
    {
        if (a < b) return a;
        else return b;
    }

    // тут и так понятно
    int char_to_int(char str[]) {
        int out = 0, err_ = 0;
        for (int i = 0; i < strlen(str); i++) {
            int buf = (int)str[i] - (int)'0';
            if (buf <= 9 && buf >= 0) {
                out = out * 10 + buf;
            }
            else err_ = 1;
        }
        if (err_ == 1) return -999;
        return out;
    }

    // тут тоже все понятно
    template <typename T>
    string int_to_string(T Number)
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }

    //проверка на правильность координат
    bool check_cord(int x, int y) {
        if (x == -999 || x < 0 || x > WIDTH) {
            *err = *err + "X должно быть числом входящим в предел между 0 и " + int_to_string(WIDTH) + ";\n";
            add_line();
            return false;
        }
        if (y == -999 || y < 0 || y > HEIGHT) {
            *err = *err + "Y должно быть числом входящим в предел между 0 и " + int_to_string(HEIGHT) + ";\n";
            add_line();
            return false;
        }
        return true;
    }
    bool check_wh(int w, int h, int x, int y) {
        if (check_cord(x, y)) {
            if (w == -999 || w <= 0 || (w + x) > WIDTH) {
                *err = *err + "Width должно быть числом входящим в предел между 1 и " + int_to_string(WIDTH - x) + ";\n";
                add_line();
                return false;
            }
            if (h == -999 || h <= 0 || (h + y) > HEIGHT) {
                *err = *err + "Height должно быть числом входящим в предел между 1 и " + int_to_string(HEIGHT - y) + ";\n";
                add_line();
                return false;
            }
            return true;
        }
        else return false;
    }
    bool check_rad(int rad, int x, int y) {
        if (check_cord(x, y)) {
            if (rad == -999 || rad <= 0 || (rad + x) > WIDTH || (x - rad) <= 0 || (rad + y) > HEIGHT || (y - rad) <= 0) {
                *err = *err + "Радиус должен быть числом входящим в предел между 1 и " + int_to_string(min_of_2(min_of_2(x, y), min_of_2(WIDTH - x, HEIGHT - y))) + ";\n";
                add_line();
                return false;
            }
            return true;
        }
        else return false;
    }
    bool check_whrad(int w, int h, int x, int y, int rad) {
        if (w > rad * 2 && h > rad * 2) {
            if (check_cord(x, y)) {
                if (w == -999 || w <= 0 || (w + x) > WIDTH) {
                    *err = *err + "Width должно быть числом входящим в предел между 1 и " + int_to_string(WIDTH - x) + ";\n";
                    add_line();
                    return false;
                }
                if (h == -999 || h <= 0 || (h + y) > HEIGHT) {
                    *err = *err + "Height должно быть числом входящим в предел между 1 и " + int_to_string(HEIGHT - y) + ";\n";
                    add_line();
                    return false;
                }
                return true;
            }
        }
        else {
            *err = *err + "Радиус должен быть числом входящим в предел между 0 и " + int_to_string(min_of_2(w / 2, h / 2)) + ";\n";
            add_line();
        }
        return false;
    }
    // проверка цвета
    bool check_color(char col[], int* col_i) {
        int col_int = 0, err_ = 0;
        if (strlen(col) == 6) {
            for (int i = 0; i < strlen(col); i++) {
                if (((col[i] >= '0')&& (col[i] <= '9'))|| ((col[i] >= 'A') && (col[i] <= 'F')) || ((col[i] >= 'a') && (col[i] <= 'f'))) {
                    err_ = 0;
                }
                else err_ = 1;
            }
            if (err_ == 0) {
                *col_i = col_int;
                return true;
            }
        }
        *err = *err + "Некоректный цвет, принимается число от 000 до 888, где каждая цифра отвечает за свой цвет, красный, зеленый и синий соответственно\n";
        add_line();
        return false;
    }

    // команды

    bool clear(char** words, int cnt) {
        if (cnt == 2)
        {
            int col_int;
            if (check_color(words[1], &col_int)) {
                bmp.clear(words[1]);
                return true;
            }
        }
        else
        {
            *err = *err + "Ошибка: синтаксис команды: clear ххх (color 24bit: xxx, где х 0-8)\n";
            add_line();
            return false;
        }
    }

    bool pixel(char** words, int cnt) {
        if (cnt == 4)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]);
            int col_int;
            if (check_cord(x, y))
                if (check_color(words[3], &col_int))
                {
                    bmp.pixel(char_to_int(words[1]), char_to_int(words[2]), words[3]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: pixel X Y ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool line(char** words, int cnt) {
        if (cnt == 6)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), x1 = char_to_int(words[3]), y1 = char_to_int(words[4]);
            int col_int;
            if (check_cord(x, y) && check_cord(x1, y1))
                if (check_color(words[5], &col_int))
                {
                    bmp.line(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), words[5]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: line X1 Y1 X2 Y2 ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }


    bool rect(char** words, int cnt) {
        if (cnt == 6)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), w = char_to_int(words[3]), h = char_to_int(words[4]);
            int col_int;
            if (check_wh(w, h, x, y))
                if (check_color(words[5], &col_int))
                {
                    bmp.rect_(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), words[5]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: rect X Y Width Height ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }


    bool fill_rect(char** words, int cnt) {
        if (cnt == 6)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), w = char_to_int(words[3]), h = char_to_int(words[4]);
            int col_int;
            if (check_wh(w, h, x, y))
                if (check_color(words[5], &col_int))
                {
                    bmp.fillrect(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), words[5]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: fill_rect X Y Width Height ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }


    bool ell(char** words, int cnt) {
        if (cnt == 6)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), rad_x = char_to_int(words[3]), rad_y = char_to_int(words[4]);
            int col_int;
            if (check_rad(rad_x, x, y) && check_rad(rad_y, x, y))
                if (check_color(words[5], &col_int))
                {
                    bmp.ell(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), words[5]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: ell X Y radius_X radius_Y ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }


    bool fill_ell(char** words, int cnt) {
        if (cnt == 6)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), rad_x = char_to_int(words[3]), rad_y = char_to_int(words[4]);
            int col_int;
            if (check_rad(rad_x, x, y) && check_rad(rad_y, x, y))
                if (check_color(words[5], &col_int))
                {
                    bmp.fill_ell(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), words[5]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: fill_ell X Y radius_X radius_Y ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool circle(char** words, int cnt) {
        if (cnt == 5)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), rad = char_to_int(words[3]);
            int col_int;
            if (check_rad(rad, x, y))
                if (check_color(words[4], &col_int))
                {
                    bmp.circle(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), words[4]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: circle X Y radius ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool fill_circle(char** words, int cnt) {
        if (cnt == 5)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), rad = char_to_int(words[3]);
            int col_int;
            if (check_rad(rad, x, y))
                if (check_color(words[4], &col_int))
                {
                    bmp.fill_circle(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), words[4]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: fill_circle X Y radius ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool round_rect(char** words, int cnt) {
        if (cnt == 7)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), w = char_to_int(words[3]), h = char_to_int(words[4]), rad = char_to_int(words[5]);
            int col_int;
            if (check_whrad(w, h, x, y, rad))
                if (check_color(words[6], &col_int))
                {
                    bmp.round_rect(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), char_to_int(words[5]), words[6]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: round_rect X Y width height radius ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool fill_round_rect(char** words, int cnt) {
        if (cnt == 7)
        {
            int x = char_to_int(words[1]), y = char_to_int(words[2]), w = char_to_int(words[3]), h = char_to_int(words[4]), rad = char_to_int(words[5]);
            int col_int;
            if (check_whrad(w, h, x, y, rad))
                if (check_color(words[6], &col_int))
                {
                    bmp.fill_round_rect(char_to_int(words[1]), char_to_int(words[2]), char_to_int(words[3]), char_to_int(words[4]), char_to_int(words[5]), words[6]);
                    return true;
                }
        }
        *err = *err + "Ошибка: синтаксис команды: fill_round_rect X Y width height radius ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool text(char** words, int cnt) {
        if (cnt >= 7)
        {
            int count = 0, x = char_to_int(words[1]), y = char_to_int(words[2]), font = char_to_int(words[3]), lenght = char_to_int(words[4]);
            string text;
            for (int i = 5; i < cnt - 1; i++)
            {
                text = text + words[i] + " ";
                count += (strlen(words[i]) + 1);
            }
            int col_int;
            if (count <= lenght) {
                if (font != -999) {
                    if (check_cord(x, y))
                        if (check_color(words[cnt - 1], &col_int))
                        {
                            cout << "Вызвана команда добавление текста А: (" << x << "; " << y << "); font: " << font << "; length: " << lenght << "; text: ''" << text << "''; color: " << words[cnt-1] << endl;
                            return true;
                        }
                }
                else {
                    *err = *err + "Размер текста должен быть числом";
                    add_line();
                }
            }
            else
            {
                *err = *err + "Количество символов текста " + int_to_string(count) + " превышает length: " + int_to_string(lenght) + "\n";
                add_line();
            }
        }
        *err = *err + "Ошибка: синтаксис команды: text X Y font_size length text ххх (color 24bit: xxx, где х 0-8)\n";
        add_line();
        return false;
    }

    bool img(char** words, int cnt) {
        if (cnt >= 6)
        {
            int count = 0, x = char_to_int(words[1]), y = char_to_int(words[2]), w = char_to_int(words[3]), h = char_to_int(words[4]);
            int col_int, err_ = 0;
            string str;
            if (w * h > 9504) {
                *err = *err + "Ошибка: слишком большое изображение, максимальное разрешение w*h = 9504 px\n";
                add_line();
                return false;
            }
            for (int i = 5, j = 0; i < cnt; i++, j++)
            {
                if (check_color(words[i], &col_int)) {
                    str = str + words[i];
                    count += 1;
                }
                else
                {
                    *err = *err + "Пиксель №" + int_to_string(count + 1) + " не является 24 битным цветом\n";
                    add_line();
                    err_ = 1;
                    break;
                }
            }
            if (err_ == 0)
                if ((w * h) == count) {
                    if (check_wh(w, h, x, y))
                        if (w * h == count)
                        {
                            char data[65000];
                            strcpy(data, str.c_str());
                            bmp.image(x, y, w, h, data);
                            return true;
                        }
                        else
                        {
                            *err = *err + "Height * Width (" + int_to_string(w * h) + ") не равно количеству пикселей в data: " + int_to_string(count) + "\n";
                            add_line();
                        }
                }
            *err = *err + "Ошибка: синтаксис команды: img X Y Width Height data (набор пикселей ххх; color 24bit: xxx, где х 0-8)\n";
            add_line();
            return false;
        }
    }

    bool orintation(char** words, int cnt) {
        if (cnt == 2)
        {
            int col_int, angle = char_to_int(words[1]);
            if (angle != -999 && angle <= 3 && angle >= 0) {
                angle *= 90;
                cout << "вызвана команда установки ориентации на " << angle << " градусов" << endl;
                return true;
            }
            else {
                *err = *err + "Не корректный angle (0=0, 1=90, 2=180, 3=270)\n";
            }
        }
        else
        {
            *err = *err + "Ошибка: синтаксис команды: orientation angle (0=0, 1=90, 2=180, 3=270)\n";
            add_line();
        }
        return false;
    }

    bool get(char** words, int cnt) {
        if (cnt == 2)
        {
            if (strcmp(words[1], "width") == 0) { *err = *err + "Ширина: " + int_to_string(WIDTH) + "\n"; return true; }
            else if (strcmp(words[1], "height") == 0) { *err = *err + "Высота: " + int_to_string(HEIGHT) + "\n"; return true; }
            else
            {
                *err = *err + "Ошибка: синтаксис команды: get width/height\n";
                add_line();
            }

        }
        else
        {
            *err = *err + "Ошибка: синтаксис команды: get width/height1\n";
            add_line();
        }
        return false;
    }


    // проверка команды
    bool check_cmd(int cnt, bool* end_prog) {
        setlocale(LC_ALL, "Russian");
        add_line();
        //exit
        if (strcmp(words[0], "exit") == 0) { *end_prog = true; return true; }
        //clear color
        else if (strcmp(words[0], "clear") == 0)
        {
            return clear(words, cnt);
        }
        //pixel x1 y1 color
        else if (strcmp(words[0], "pixel") == 0)
        {
            return pixel(words, cnt);
        }
        //line x1 y1 x2 y2 color
        else if (strcmp(words[0], "line") == 0)
        {
            return line(words, cnt);
        }
        //rect x1 y1 w h color
        else if (strcmp(words[0], "rect") == 0)
        {
            return rect(words, cnt);
        }
        //fill_rect x1 y1 w h color
        else if (strcmp(words[0], "fill_rect") == 0)
        {
            return fill_rect(words, cnt);
        }
        //ell x y rad_x rad_y color
        else if (strcmp(words[0], "ell") == 0)
        {
            return ell(words, cnt);
        }
        //fill_ell x y rad_x rad_y color
        else if (strcmp(words[0], "fill_ell") == 0)
        {
            return fill_ell(words, cnt);
        }
        //circle x y rad color
        else if (strcmp(words[0], "circle") == 0)
        {
            return circle(words, cnt);
        }
        //fill_circle x y rad color
        else if (strcmp(words[0], "fill_circle") == 0)
        {
            return fill_circle(words, cnt);
        }
        //round_rect x1 y1 w h radius color
        else if (strcmp(words[0], "round_rect") == 0)
        {
            return round_rect(words, cnt);
        }
        //fill_round_rect x1 y1 w h radius color
        else if (strcmp(words[0], "fill_round_rect") == 0)
        {
            return fill_round_rect(words, cnt);
        }
        //text x1 y1 font lenght text color
        else if (strcmp(words[0], "text") == 0)
        {
            return text(words, cnt);
        }
        //img x1 y1 w h data
        else if (strcmp(words[0], "img") == 0)
        {
            return img(words, cnt);
        }
        //orintation angle
        else if (strcmp(words[0], "orintation") == 0)
        {
            return orintation(words, cnt);
        }
        //get width/height
        else if (strcmp(words[0], "get") == 0)
        {
            return get(words, cnt);
        }
        else
        {
            *err = *err + "Команда не найдена\n";
            add_line();
            return false;
        }
    }

    // разбив на лексемы
    bool lecs(char str[], bool* end_prog) {
        
        char* p = NULL;
        int i, count_ = 0;
        for (p = strtok(str, " "); p != NULL; p = strtok(NULL, " ")) {
            if ((words = (char**)realloc(words, sizeof(char*) * (count_ + 1))) == NULL) {
                printf("No memory to new word!\n");
                exit(1);
            }
            if ((words[count_] = _strdup(p)) == NULL) {
                printf("Can't duplicate word!\n");
                exit(1);
            }
            ++count_;
        }
      
        bool err_ = check_cmd(count_, end_prog);

        for (int i = 0; i < count_; ++i) {
            if (words[i] != NULL) {
                free(words[i]);
                words[i] = NULL;
            }
        }
        free(words);
        words = NULL;
        return err_;
    }
    bool do_cmd(char str[], bool* end_prog, string* er)
    {
        err = er;
        return lecs(str, end_prog);
    }
    void print() {
         bmp.print();
    }
};
#ifndef Pars
#define Pars
#include <stdio.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <sstream>

class Parser {
private:
    int const WIDTH = 800;
    int const HEIGHT = 600;
    string* err;
    char** words = NULL;
    void add_line() {
        *err = *err + "--------------------------------------\n";
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
        if (strlen(col) == 3) {
            for (int i = 0; i < strlen(col); i++) {
                int buf = col[i] - '0';
                if (buf <= 8) {
                    col_int = col_int * 10 + buf;
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
                cout << "вызвана команда очистки дисплея c параметром color: " << col_int << endl;
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
                    cout << "Вызвана команда заливки пикселя (" << x << "; " << y << ") : " << col_int << endl;
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
                    cout << "Вызвана команда создания линии A:(" << x << "; " << y << "); B:(" << x1 << "; " << y1 << "); color: " << col_int << endl;
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
                    cout << "Вызвана команда прямоугольника A:(" << x << "; " << y << "); width: " << w << "; height: " << h << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания залитого прямоугольника A:(" << x << "; " << y << "); width: " << w << "; height: " << h << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания эллипса A:(" << x << "; " << y << "); radius_x: " << rad_x << "; radius_y: " << rad_y << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания залитого эллипса A:(" << x << "; " << y << "); radius_x: " << rad_x << "; radius_y: " << rad_y << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания окружности A:(" << x << "; " << y << "); radius: " << rad << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания заполненого круга A:(" << x << "; " << y << "); radius: " << rad << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания прямоугольника A:(" << x << "; " << y << "); width: " << w << "; height: " << h << "; radius: " << rad << "; color: " << col_int << endl;
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
                    cout << "Вызвана команда создания залитого прямоугольника A:(" << x << "; " << y << "); width: " << w << "; height: " << h << "; radius: " << rad << "; color: " << col_int << endl;
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
                            cout << "Вызвана команда добавление текста А: (" << x << "; " << y << "); font: " << font << "; length: " << lenght << "; text: ''" << text << "''; color: " << col_int << endl;
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
            string data;
            for (int i = 5; i < cnt; i++)
            {
                if (check_color(words[i], &col_int)) {
                    data = data + words[i] + " ";
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
                            if (check_color(words[5], &col_int))
                            {
                                cout << "Вызвана команда вывода картинки A:(" << x << "; " << y << "); width: " << w << "; height: " << h << "; data: " << data << endl;
                                return true;
                            }
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
    void check_cmd(int cnt, bool* end_prog) {
        setlocale(LC_ALL, "Russian");
        add_line();
        //exit
        if (strcmp(words[0], "exit") == 0) { *end_prog = true; }
        //clear color
        else if (strcmp(words[0], "clear") == 0)
        {
            clear(words, cnt);
        }
        //pixel x1 y1 color
        else if (strcmp(words[0], "pixel") == 0)
        {
            pixel(words, cnt);
        }
        //line x1 y1 x2 y2 color
        else if (strcmp(words[0], "line") == 0)
        {
            line(words, cnt);
        }
        //rect x1 y1 w h color
        else if (strcmp(words[0], "rect") == 0)
        {
            rect(words, cnt);
        }
        //fill_rect x1 y1 w h color
        else if (strcmp(words[0], "fill_rect") == 0)
        {
            fill_rect(words, cnt);
        }
        //ell x y rad_x rad_y color
        else if (strcmp(words[0], "ell") == 0)
        {
            ell(words, cnt);
        }
        //fill_ell x y rad_x rad_y color
        else if (strcmp(words[0], "fill_ell") == 0)
        {
            fill_ell(words, cnt);
        }
        //circle x y rad color
        else if (strcmp(words[0], "circle") == 0)
        {
            circle(words, cnt);
        }
        //fill_circle x y rad color
        else if (strcmp(words[0], "fill_circle") == 0)
        {
            fill_circle(words, cnt);
        }
        //round_rect x1 y1 w h radius color
        else if (strcmp(words[0], "round_rect") == 0)
        {
            round_rect(words, cnt);
        }
        //fill_round_rect x1 y1 w h radius color
        else if (strcmp(words[0], "fill_round_rect") == 0)
        {
            fill_round_rect(words, cnt);
        }
        //text x1 y1 font lenght text color
        else if (strcmp(words[0], "text") == 0)
        {
            text(words, cnt);
        }
        //img x1 y1 w h data
        else if (strcmp(words[0], "img") == 0)
        {
            img(words, cnt);
        }
        //orintation angle
        else if (strcmp(words[0], "orintation") == 0)
        {
            orintation(words, cnt);
        }
        //get width/height
        else if (strcmp(words[0], "get") == 0)
        {
            get(words, cnt);
        }
        else
        {
            *err = "Команда не найдена\n";
            add_line();
        }
    }

    // разбив на лексемы
    void lecs(char str[], bool* end_prog) {
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

        check_cmd(count_, end_prog);

        for (i = 0; i < count_; ++i) {
            if (words[i] != NULL) {
                free(words[i]);
                words[i] = NULL;
            }
        }
        free(words);
        words = NULL;
    }
public: bool do_cmd(char str[], bool* end_prog, string* er)
{
    err = er;
    lecs(str, end_prog);
    return true;
}
};
#endif
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <sstream>
#include <vector>
#include "draw.h"
#include <time.h>
#include <math.h>

using namespace std;

class Parser {
public:
    int const WIDTH = 800;
    int const HEIGHT = 600;
    string* err;
    char** words;
    GraphicsLib bmp;
    bool clock = false;
    int col_cl[3];
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
    bool check_color(char col[]) {
        int err_ = 0;
        if (strlen(col) == 6) {
            for (int i = 0; i < strlen(col); i++) {
                if (((col[i] >= '0')&& (col[i] <= '9'))|| ((col[i] >= 'A') && (col[i] <= 'F')) || ((col[i] >= 'a') && (col[i] <= 'f'))) {
                    err_ = 0;
                }
                else err_ = 1;
            }
            if (err_ == 0) {
                return true;
            }
        }
        *err = *err + "Некоректный цвет, принимается число от 000 до 888, где каждая цифра отвечает за свой цвет, красный, зеленый и синий соответственно\n";
        add_line();
        return false;
    }

    void ch2col(char* str, int* col) {
        for (int i = 0; i < 3; i++) {
            col[i] = (int)str[i] + 128;
        }
        return;
    }

    void ch2int(char* str, int* out) {
        unsigned char bytes[4];
        for (int i = 0; i < 4; i++) bytes[i] = str[i] + 128;
        copy(bytes, bytes + 4, (char*)out);
    }

    // проверка команды
    bool check_cmd(char str[], bool* end_prog) {
        setlocale(LC_ALL, "Russian");
        //for (int i = 0; i < strlen(str); i++) cout << (int)str[i] + 128 << "|"; cout << endl;
        add_line();
        int x, y, x1, y1, w, h, r, r1, font;
        int col[3];
        switch (str[0])
        {
        case 1:
            //clear
            ch2col(&str[1], col);
            bmp.clear(col);
            break;
        case 2:
            //pixel
            ch2int(&str[1], &x); ch2int(&str[5], &y);
            if (check_cord(x, y))
            {
                ch2col(&str[9], col);
                bmp.pixel(x, y, col);
                return true;
            }
            else
                return false;
            break;
        case 3:
            //line
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &x1); ch2int(&str[13], &y1);
            if (check_cord(x, y) && check_cord(x1, y1))
            {
                ch2col(&str[17], col);
                bmp.line(x, y, x1, y1, col);
                return true;
            }
            else
                return false;
            break;
        case 4:
            //rect
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &w); ch2int(&str[13], &h);
            if (check_cord(x, y) && check_cord(x + w, y + h))
            {
                ch2col(&str[17], col);
                bmp.rect_(x, y, w, h, col);
                return true;
            }
            else
                return false;
            break;
        case 5:
            //fill_rect
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &w); ch2int(&str[13], &h);
            if (check_cord(x, y) && check_cord(x + w, y + h))
            {
                ch2col(&str[17], col);
                bmp.fillrect(x, y, w, h, col);
                return true;
            }
            else
                return false;
            break;
        case 6:
            //ell
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &r); ch2int(&str[13], &r1);
            if (check_cord(x, y))
            {
                ch2col(&str[17], col);
                bmp.ell(x, y, r, r1, col);
                return true;
            }
            else
                return false;
            break;
        case 7:
            //fill_ell
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &r); ch2int(&str[13], &r1);
            if (check_cord(x, y))
            {
                ch2col(&str[17], col);
                bmp.fill_ell(x, y, r, r1, col);
                return true;
            }
            else
                return false;
            break;
        case 8:
            //circle
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &r);
            if (check_cord(x, y))
            {
                ch2col(&str[13], col);
                bmp.circle(x, y, r, col);
                return true;
            }
            else
                return false;
            break;
        case 9:
            //fill_circle
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &r);
            if (check_cord(x, y))
            {
                ch2col(&str[13], col);
                bmp.fill_circle(x, y, r, col);
                return true;
            }
            else
                return false;
            break;
        case 10:
            //round_rect
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &w); ch2int(&str[13], &h);  ch2int(&str[17], &r);
            if (check_whrad(w, h, x, y, r))
            {
                ch2col(&str[21], col);
                bmp.round_rect(x, y, w, h, r, col);
                return true;
            }
            else
                return false;
            break;
        case 11:
            //round_rect
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &w); ch2int(&str[13], &h);  ch2int(&str[17], &r);
            if (check_whrad(w, h, x, y, r))
            {
                ch2col(&str[21], col);
                bmp.fill_round_rect(x, y, w, h, r, col);
                return true;
            }
            else
                return false;
            break;
        case 12:
            //round_rect
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &font);
            if (check_cord(x, y))
            {
                ch2col(&str[13], col);
                bmp.text(x, y, font, &str[16], col);
                return true;
            }
            else
                return false;
            break;
        case 13:
            //image
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &w);; ch2int(&str[13], &h);
            if (check_cord(x, y))
            {
                int data[1000];
                for (int i = 17, j = 0; i < (w * h) * 3 + 17; i++, j++) { ch2col(&str[i], &data[j]); }
                bmp.image(x, y, w, h, data);
                return true;
            }
            else
                return false;
            break;
        case 14:
            if ((int)str[1] == 32)  *err = *err + "Ширина робочей области: " + int_to_string(bmp.Width);
            else if ((int)str[1] == 1)  *err = *err + "Высота робочей области: " + int_to_string(bmp.Height);
            return true;
            break;
        case 15:
            //clock
            ch2col(&str[1],col_cl);
            clock = true;
            break;
        case 16:
            //poly
            ch2int(&str[1], &x); ch2int(&str[5], &y); ch2int(&str[9], &r); ch2int(&str[13], &h); ch2col(&str[17], col);
            cout << x << y << r << h;
            if (check_cord(x, y))
            {
                bmp.poly(x, y, r, h, col);
                return true;
            }
            else
                return false;
            break;
        case 17:
            clock = false;
            break;
        default:
            return false;
            break;

        }
    }
    void get_rot_xy(int x, int y, int r, int pices, int data, int* out_x, int* out_y) {
        
        *out_x = r * sin(3.1415 / 180 * (-(360 / pices) * data)) + x;
        *out_y = r * cos(3.1415 / 180 * (-(360/pices) * data)) + y;
    }
    void clock_() {
        if (clock) {
            int x = 50, y = 50, r = 40, x1, y1, x2, y2;
            int col[3] = { 0,0,0 };
            bmp.fill_circle(x, y, r, col);
            bmp.fillrect(x-30,y+r*1.5,x+40,y+r*2, col);
            bmp.circle(x, y, r, col_cl);
            time_t seconds = time(NULL);
            int hour = (2 + seconds / 3600) % 24, minute = seconds / 60 % 60; seconds = seconds % 60;
            char clock_t[100];
            sprintf(clock_t, "%d:%d", hour, minute);
            bmp.text(x - 30, y + r*1.5, 30, clock_t, col_cl);
            col[0] = 255; col[1] = 255; col[2] = 255;
            for (int i = 0; i < 12; i++)
            {
                get_rot_xy(x, y, r, 12, i, &x2, &y2);
                if (i%3==0)get_rot_xy(x, y, r - 7, 12, i, &x1, &y1);
                else get_rot_xy(x, y, r - 4, 12, i, &x1, &y1);
                bmp.line(x2, y2, x1, y1, col);
            }
            get_rot_xy(x, y, r / 2, 12, hour / 2, &x1, &y1);
            bmp.line(x, y, x1, y1, col);
            bmp.line(x+1, y+1, x1+1, y1+1, col);
            get_rot_xy(x, y, r - 3, 60, minute + 30, &x1, &y1);
            bmp.line(x, y, x1, y1, col);
            col[0] = 255; col[1] = 0; col[2] = 0;
            get_rot_xy(x, y, r, 60, seconds, &x1, &y1);
            bmp.line(x, y, x1, y1, col);
            Sleep(1000);
        }
        return;
    }

    bool do_cmd(char str[], bool* end_prog, string* er)
    {
        err = er;
        return check_cmd(str, end_prog);
    }
    void print() {
        bmp.print();
    }
};
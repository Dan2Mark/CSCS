#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <sstream>

using namespace std;

class Pars_client {
    char** words = NULL;
    string cmd = "0";

    char uch2ch(unsigned char in) {
        return in - 128;
    }
    int hex2dec(char st) {
        switch (toupper(st))
        {
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case '0': return 0;
        default: return -1;
        }
    }
    bool check_int(char str[])
    {
        int out = 0, err_ = 0;
        for (int i = 0; i < strlen(str); i++) {
            int buf = (int)str[i] - (int)'0';
            if (buf <= 9 && buf >= 0) {
                out = out * 10 + buf;
            }
            else err_ = 1;
        }
        if (err_ == 1) {
            cout << str << " не число\n";
            return false;
        }
        else {

            unsigned char bytes[4];
            copy((char*)&out, (char*)&out + 4, bytes);
            for (int i = 0; i < 4; i++) cmd += uch2ch(bytes[i]);
            return true;
        }
    }
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
    bool check_color(char col[]) {
        int col_int = 0, err_ = 0;
        unsigned char rgb[3];
        if (strlen(col) == 7|| strlen(col) == 6 ) {
            for (int i = 0; i < strlen(col) / 2; i++) {

                if (hex2dec(col[i * 2]) != -1 && hex2dec(col[i * 2 + 1]) != -1)
                    rgb[i] = 16 * hex2dec(col[i * 2]) + hex2dec(col[i * 2 + 1]);
                else err_ = 1;
            }
            if (err_ == 0) {
                for (int i = 0; i < 3; i++) {
                    cmd += uch2ch(rgb[i]);
                }
                return true;
            }
        }
        cout << "Некоректный цвет, принимается число от 000000 до FFFFFF, где каждая цифра отвечает за свой цвет, красный, зеленый и синий соответственно\n";
        return false;
    }


    bool check_cmd(int cnt) {
        setlocale(LC_ALL, "Russian");
        //exit
        if (strcmp(words[0], "exit") == 0) { cmd[0] = 99; return true; }
        //clear color
        else if (strcmp(words[0], "clear") == 0)
        {
            if (cnt == 2) {
                cmd[0] = 1;
                return check_color(words[1]);
            }
            else
            {
                cout << "Неверный синтаксис команды: clear color\n" << endl;
                return false;
            }
        }
        //pixel x1 y1 color
        else if (strcmp(words[0], "pixel") == 0)
        {
            if (cnt == 4) {
                cmd[0] = 2;
                bool out = (check_int(words[1]) && check_int(words[2]) && check_color(words[3]));
                return out;
            }
            else {
                cout << "Неверный синтаксис команды: pixel x1 y1 color\n" << endl;
                return false;
            }
        }
        //line x1 y1 x2 y2 color
        else if (strcmp(words[0], "line") == 0)
        {
            if (cnt == 6) {
                cmd[0] = 3;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_color(words[5]));
            }
            else {
                cout << "Неверный синтаксис команды: line x1 y1 x2 y2 color\n" << endl;
                return false;
            }
        }
        //rect x1 y1 w h color
        else if (strcmp(words[0], "rect") == 0)
        {
            if (cnt == 6) {
                cmd[0] = 4;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_color(words[5]));
            }
            else {
                cout << "Неверный синтаксис команды: rect x1 y1 w h color\n" << endl;
                return false;
            }
        }
        //fill_rect x1 y1 w h color
        else if (strcmp(words[0], "fill_rect") == 0)
        {
            if (cnt == 6) {
                cmd[0] = 5;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_color(words[5]));
            }
            else {
                cout << "Неверный синтаксис команды: fill_rect x1 y1 w h color\n" << endl;
                return false;
            }
        }
        //ell x y rad_x rad_y color
        else if (strcmp(words[0], "ell") == 0)
        {
            if (cnt == 6) {
                cmd[0] = 6;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_color(words[5]));
            }
            else {
                cout << "Неверный синтаксис команды: ell x1 y1 w h color\n" << endl;
                return false;
            }
        }
        //fill_ell x y rad_x rad_y color
        else if (strcmp(words[0], "fill_ell") == 0)
        {
            if (cnt == 6) {
                cmd[0] = 7;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_color(words[5]));
            }
            else {
                cout << "Неверный синтаксис команды: fill_ell x1 y1 w h color\n" << endl;
                return false;
            }
        }
        //circle x y rad color
        else if (strcmp(words[0], "circle") == 0)
        {
            if (cnt == 5) {
                cmd[0] = 8;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_color(words[4]));
            }
            else {
                cout << "Неверный синтаксис команды: circle x1 y1 rad color\n" << endl;
                return false;
            }
        }
        //fill_circle x y rad color
        else if (strcmp(words[0], "fill_circle") == 0)
        {
            if (cnt == 5) {
                cmd[0] = 9;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_color(words[4]));
            }
            else {
                cout << "Неверный синтаксис команды: fill_circle x1 y1 rad color\n" << endl;
                return false;
            }
        }
        //round_rect x1 y1 w h radius color
        else if (strcmp(words[0], "round_rect") == 0)
        {
            if (cnt == 7) {
                cmd[0] = 10;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_int(words[5]) && check_color(words[6]));
            }
            else {
                cout << "Неверный синтаксис команды: round_rect x1 y1 w h rad color\n" << endl;
                return false;
            }
        }
        //fill_round_rect x1 y1 w h radius color
        else if (strcmp(words[0], "fill_round_rect") == 0)
        {
            if (cnt == 7) {
                cmd[0] = 11;
                return (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]) && check_int(words[5]) && check_color(words[6]));
            }
            else {
                cout << "Неверный синтаксис команды: fill_round_rect x1 y1 w h rad color\n" << endl;
                return false;
            }
        }
        //text x1 y1 font text color
        else if (strcmp(words[0], "text") == 0)
        {
            if (cnt >= 6) {
                cmd[0] = 12;
                bool out = (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_color(words[cnt - 1]));
                for (int i = 4; i < cnt - 1; i++) cmd = cmd + words[i] + ' ';
                return out;
            }
            else {
                cout << "Неверный синтаксис команды: text x1 y1 font text color\n" << endl;
                return false;
            }
        }
        //img x1 y1 w h data
        else if (strcmp(words[0], "img") == 0)
        {

            int w = char_to_int(words[3]), h = char_to_int(words[4]);
            if (cnt == 5 + w * h) {
                cmd[0] = 13;
                bool out = (check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]));
                for (int i = 5; i < w * h + 5; i++) {
                    if (!check_color(words[i])) {
                        cout << "Неверный пиксель №" << i - 5 << ": " << words[i] << "\n";
                        return false;
                    }
                }
            }
            else {
                cout << "Неверный синтаксис команды: img x1 y1 w h data\n" << endl;
                return false;
            }
        }
        //get width/height
        else if (strcmp(words[0], "get") == 0)
        {
            cmd[0] = 14;
            cmd += '0';
            if (strcmp(words[1], "width") == 0) {
                cmd[1] = 0; return true;
            }
            else if (strcmp(words[1], "height") == 0) {
                cmd[1] = 1; return true;
            }
            else {
                cout << "Неверный синтаксис команды: get width/height"; return false;
            }
        }
        else if (strcmp(words[0], "clock") == 0)
        {
            if (cnt == 2) {
                if (strcmp(words[1], "off") == 0) {
                    cmd[0] = 17; return true;
                }
                check_color(words[1]);
                cmd[0] = 15;
                return true;
            }
            else {
                cout << "Неверный синтаксис команды: clock color/off"; return false;
            }
        }
        //poly x1 y1 r sides col
        else if (strcmp(words[0], "poly") == 0)
        {
            int x = char_to_int(words[3]), y = char_to_int(words[4]);
            if (cnt == 6) {
                cmd[0] = 16;
                if(check_int(words[1]) && check_int(words[2]) && check_int(words[3]) && check_int(words[4]), check_color(words[5])) return true; 
                else {
                    cout << "Неверный синтаксис команды: poly x y r sides color"; return false;
                }
            }
            else
            {
                std::cout << "Команда не найдена\n";
                return false;
            }
        }

    }
public: bool lecs(char str[], string* command) {

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

    bool err_ = check_cmd(count_);

    for (int i = 0; i < count_; ++i) {
        if (words[i] != NULL) {
            free(words[i]);
            words[i] = NULL;
        }
    }
    *command = cmd;
    free(words);
    words = NULL;
    return err_;
}

};
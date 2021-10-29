#pragma once
#include <windows.h>
#include <conio.h>
#include <windows.h>
#include<math.h>
#include<stdio.h>
#include <iostream>
#include <wingdi.h>
#include <string>

class draw {
private:
	RECT rect;
	HWND hWnd;
	HDC hDC, hBufferDC;
    HBRUSH hbr;
	POINT center;
	HBITMAP hBufferBmp, hBufferBmpOld;
    POINT oldPoint;
    HPEN hPen;
    int R, G, B;
    int HEX_TO_DEC(char st[2])
    {
        int i, s, k = 0, p;
        s = 0;
        p = strlen(st) - 1;
        for (i = 0; st[i] != '\0'; i++)
        {
            switch (toupper(st[i]))
            {
            case 'A': k = 10; break;
            case 'B': k = 11; break;
            case 'C': k = 12; break;
            case 'D': k = 13; break;
            case 'E': k = 14; break;
            case 'F': k = 15; break;
            case '1': k = 1; break;
            case '2': k = 2; break;
            case '3': k = 3; break;
            case '4': k = 4; break;
            case '5': k = 5; break;
            case '6': k = 6; break;
            case '7': k = 7; break;
            case '8': k = 8; break;
            case '9': k = 9; break;
            case '0': k = 0; break;
            }
            s = s + k * pow(16, p);
            p--;
        }
        return s;
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

    void color_set(char* str) {
        char st[2];
        st[1] = str[1]; st[1] = str[1]; std::cout << str[1];
        R = HEX_TO_DEC(st);
        st[0] = str[2]; st[1] = str[3];
        G = HEX_TO_DEC(st);
        st[0] = str[4]; st[1] = str[5];
        B = HEX_TO_DEC(st);
    }
public:

    void print() {
        BitBlt(hDC, 0, 0, rect.right, rect.bottom, hBufferDC, 0, 0, SRCCOPY);
    }
	draw () {
		hWnd = GetConsoleWindow();
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		center.x = (rect.right - rect.left) / 2;
		center.y = (rect.bottom - rect.top) / 2;
		hBufferDC = CreateCompatibleDC(hDC);
		hBufferBmp = CreateBitmap(rect.right, rect.bottom, 1, 32, NULL);
		hBufferBmpOld = (HBITMAP)SelectObject(hBufferDC, hBufferBmp);

	}

	~draw() {
		SelectObject(hBufferDC, hBufferBmpOld);
		DeleteObject(hBufferBmp);
		DeleteDC(hBufferDC);
		ReleaseDC(hWnd, hDC);
	}
	void clear(char* col) {
        color_set(col);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %d %d %d", R, G, B);
        hbr = CreateSolidBrush(RGB(R, G, B));
		FillRect(hBufferDC, &rect, hbr);
        print();
	}

    void pixel(int x, int y, char* col) {
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        LineTo(hBufferDC, x+1, y+1);
        print();
    }
    void line(int x, int y, int x1, int y1, char* col) {
        color_set(col);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        LineTo(hBufferDC, x1, y1);

    }
    void rect_(int x, int y, int w, int h, char* col) {
        int x1 = x + w, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        hbr = CreateSolidBrush(RGB(R,G,B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        Rectangle(hDC, x, y, x + w, y + h);
        return;
    }
    void fillrect(int x, int y, int w, int h, char* col) {
        int x1 = x + w, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        for (y1 = 0; y1 < y+h; y1++) {
            MoveToEx(hBufferDC, x, y1, &oldPoint);
            LineTo(hBufferDC, x1, y1);
        }    
    return;
    }
    void circle(int x, int y, int r, char* col)
    {
        int x1 = x, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x - r, y, &oldPoint);
        for (x1 = x-r; x1 <= x+r; x1++) {
            LineTo(hBufferDC, x1, y + sqrt(r * r - (x - x1) * (x - x1)));
        }

        MoveToEx(hBufferDC, x - r, y, &oldPoint);
        
        for (x1 = x - r; x1 <= x + r; x1++) {
            LineTo(hBufferDC, x1, y - sqrt(r * r - (x-x1) * (x-x1)));
        }
        return;
    }
    void fill_circle(int x, int y, int r, char* col)
    {
        int x1 = x, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        for (x1 = x - r; x1 <= x + r; x1++) {
            MoveToEx(hBufferDC, x1, y - sqrt(r * r - (x - x1) * (x - x1)), &oldPoint);
            LineTo(hBufferDC, x1, y + sqrt(r * r - (x - x1) * (x - x1)));
        }
        return;
    }
    void fill_ell(int x, int y, int a, int b, char* col)
    {
        int x1 = x, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        for (x1 = x - a; x1 <= x + a; x1++) {
            std::cout << x1 << "\t" << y - sqrt(b * b * (1 - ((double)((x - x1) * (x - x1)) / (a * a)))) << "\n";
            MoveToEx(hBufferDC, x1, y - sqrt(b * b * (1 - ((double)((x - x1) * (x - x1)) / (a * a)))), &oldPoint);
            LineTo(hBufferDC, x1, y + sqrt(b * b * (1 - ((double)((x - x1) * (x - x1)) / (a * a)))));
        }
        return;
    }

    void ell(int x, int y, int a, int b, char* col)
    {
        int x1 = x, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x - a, y, &oldPoint);
        for (x1 = x - a; x1 <= x + a; x1++) {
            LineTo(hBufferDC, x1, y - sqrt(b * b * (1 - ((double)((x - x1) * (x - x1)) / (a * a)))));
        }
        MoveToEx(hBufferDC, x - a, y, &oldPoint);
        for (x1 = x - a; x1 <= x + a; x1++) {
            LineTo(hBufferDC, x1, y + sqrt(b * b * (1 - ((double)((x - x1) * (x - x1)) / (a * a)))));
        }
        return;
    }
    void round_rect(int x, int y, int w, int h, int r, char* col)
    {
        int x1 = x, y1 = y, x2, y2;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y+r, &oldPoint);
        x2 = x1 + r; y2 = y1 + r;
        for (x1 = x2 - r; x1 <= x2; x1++) {
            LineTo(hBufferDC, x1, y1 = y2 - sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x1 + w - r, y1);
        x2 = x1 + w - r; y2 = y1 + r;
        for (x1 = x2; x1 <= x2+r; x1++) {
            LineTo(hBufferDC, x1, y1 = y2 - sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x1, y1 + h - r);
        x2 = x1 - r; y2 = y1 + h - r;
        for (x1 = x2 + r; x1 >= x2; x1--) {
            LineTo(hBufferDC, x1, y1 = y2 + sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x2 = x1 - w + r, y1);
        y2 = y1 - r;
        for (x1 = x2; x1 >= x2 - r; x1--) { 
            LineTo(hBufferDC, x1, y1 = y2 + sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x1, y1 - h + r);
        return;
    }
    void fill_round_rect(int x, int y, int w, int h, int r, char* col)
    {
        int x1 = x, y1 = y, x2, y2;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        x2 = x1 + r; y2 = y1 + r;
        for (y1 = y; y1 < y + r; y1++) {
            MoveToEx(hBufferDC, x + r  - sqrt(r * r - (y1-y-r) * (y1-y-r)), y1, &oldPoint);
            LineTo(hBufferDC, x - r + w + sqrt(r * r - (y1-y-r) * (y1-y-r)), y1);
        }
        for (y1 = y+r; y1 < y + h - r; y1++) {
            MoveToEx(hBufferDC, x, y1, &oldPoint);
            LineTo(hBufferDC, x + w, y1);
        }


        x2 = x1 + r; y2 = y1 + r;
        for (y1 = y; y1 < y + r; y1++) {
            MoveToEx(hBufferDC, x + r - sqrt(r * r - (y1 - y) * (y1 - y)), y1 + h - r, &oldPoint);
            LineTo(hBufferDC, x - r + w + sqrt(r * r - (y1 - y) * (y1 - y)), y1 + h - r);
        }
    }
    void image(int x, int y, int w, int h, char data_[])
    {
        int x1 = x, y1 = y, i = 0;
     
        for (y1 = y; y1 < y + h; y1++)
            for (x1 = x; x1 < x + w; x1++)
            {
                char data[6];
                for (int j = 0; j < 6; j++) {
                    data[j] = data_[i];
                    i++;
                }
                    color_set(data);
                    hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
                    DeleteObject(SelectObject(hBufferDC, hPen));
                    MoveToEx(hBufferDC, x1, y1, &oldPoint);
                    LineTo(hBufferDC, x1+1, y1);
            
            }
                    return;
    }
};
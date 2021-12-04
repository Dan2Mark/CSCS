#pragma once
#include <windows.h>
#include <conio.h>
#include <windows.h>
#include<math.h>
#include<stdio.h>
#include <iostream>
#include <wingdi.h>
#include <string>

class GraphicsLib {
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
    void color_set(int* str) {    
            R = str[0];
            G = str[1];
            B = str[2];
    }
public:
    int Width, Height;
    void print() {
        BitBlt(hDC, 0, 0, rect.right, rect.bottom, hBufferDC, 0, 0, SRCCOPY);
    }
	GraphicsLib () {
		hWnd = GetConsoleWindow();
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		center.x = (rect.right - rect.left) / 2;
		center.y = (rect.bottom - rect.top) / 2;
		hBufferDC = CreateCompatibleDC(hDC);
		hBufferBmp = CreateBitmap(rect.right, rect.bottom, 1, 32, NULL);
		hBufferBmpOld = (HBITMAP)SelectObject(hBufferDC, hBufferBmp);
        Width = rect.right - rect.left;
        Height = rect.bottom - rect.top;
	}

	~GraphicsLib() {
		SelectObject(hBufferDC, hBufferBmpOld);
		DeleteObject(hBufferBmp);
		DeleteDC(hBufferDC);
		ReleaseDC(hWnd, hDC);
	}
	void clear(int* col) {
        color_set(col);
        //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n %d %d %d %s", R, G, B, col);
        hbr = CreateSolidBrush(RGB(R, G, B));
		FillRect(hBufferDC, &rect, hbr);
        print();
	}

    void pixel(int x, int y, int* col) {
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        LineTo(hBufferDC, x+1, y+1);
        print();
    }
    void line(int x, int y, int x1, int y1, int* col) {
        color_set(col);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        LineTo(hBufferDC, x1, y1);
        print();

    }
    void rect_(int x, int y, int w, int h, int* col) {
        int x1 = x + w, y1 = y + h;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        hbr = CreateSolidBrush(RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        LineTo(hBufferDC, x1, y);
        LineTo(hBufferDC, x1, y1);
        LineTo(hBufferDC, x, y1); 
        LineTo(hBufferDC, x, y);
        print();
        return;
    }
    void fillrect(int x, int y, int w, int h, int* col) {
        int x1 = x + w, y1 = y;
        color_set(col);
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        MoveToEx(hBufferDC, x, y, &oldPoint);
        for (y1 = y; y1 < y+h; y1++) {
            MoveToEx(hBufferDC, x, y1, &oldPoint);
            LineTo(hBufferDC, x1, y1);
        }    
    return;
    }
    void circle(int x, int y, int r, int* col)
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
    void fill_circle(int x, int y, int r, int* col)
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
    void fill_ell(int x, int y, int a, int b, int* col)
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

    void ell(int x, int y, int a, int b, int* col)
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
    void round_rect(int x, int y, int w, int h, int r, int* col)
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
        LineTo(hBufferDC, x1 + w - 2*r, y1);
        x2 = x1 + w - 2*r; y2 = y1 + r;
        for (x1 = x2; x1 <= x2+r; x1++) {
            LineTo(hBufferDC, x1, y1 = y2 - sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x1, y1 + h - 2*r);
        x2 = x1 - r; y2 = y1 + h - 2*r;
        for (x1 = x2 + r; x1 >= x2; x1--) {
            LineTo(hBufferDC, x1, y1 = y2 + sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x2 = x1 - w + 2*r, y1);
        y2 = y1 - r;
        for (x1 = x2; x1 >= x2 - r; x1--) { 
            LineTo(hBufferDC, x1, y1 = y2 + sqrt(r * r - (x2 - x1) * (x2 - x1)));
        }
        LineTo(hBufferDC, x1, y1 - h + 2*r);
        return;
    }
    void fill_round_rect(int x, int y, int w, int h, int r, int* col)
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
    void image(int x, int y, int w, int h, int* data_)
    {
        int x1 = x, y1 = y, i = 0;
     
        for (y1 = y; y1 < y + h; y1++)
            for (x1 = x; x1 < x + w; x1++)
            {
                    color_set(&data_[i]);
                    hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
                    DeleteObject(SelectObject(hBufferDC, hPen));
                    MoveToEx(hBufferDC, x1, y1, &oldPoint);
                    LineTo(hBufferDC, x1+1, y1);
                    i+=3;
            }
                    return;
    }
    void text(int x, int y, int f, char* text, int* col) {
        RECT rc;
        HFONT font;
        HGDIOBJ old;
        color_set(col);
        font = CreateFont(-f, 0, 0, 0, 0, 0u, 0U, 0U, ANSI_CHARSET, 0U, 0U, 0U, 0U, TEXT("Calibri"));
        old = SelectObject(hBufferDC, font);
        SetRect(&rc, x, y, x+strlen(text)*f, y + f);
        SetBkMode(hBufferDC, TRANSPARENT);
        SetTextColor(hBufferDC, RGB(R, G, B)); 

        const size_t cSize = strlen(text) + 1;
        wchar_t* wc = new wchar_t[cSize]; 
        mbstowcs(wc, text, cSize);
        // вывести текст по середине(горизонтали)
        DrawText(hBufferDC, wc, -1, &rc, DT_SINGLELINE | DT_VCENTER);

        SelectObject(hBufferDC, old);
        DeleteObject(font);
   }

    void poly(int x, int y, int r, int side, int* col)
    {
        int x1 = x, y1 = y, x2, y2;
        color_set(col);
        for (int i = 0; i < 3; i++) std::cout << col[i];
        hPen = CreatePen(PS_SOLID, 1, RGB(R, G, B));
        DeleteObject(SelectObject(hBufferDC, hPen));
        x2 = r * sin(3.1415 / 180 * (-(360 / side))) + x;
        y2 = r * cos(3.1415 / 180 * (-(360 / side))) + y;
        MoveToEx(hBufferDC, x2, y2, &oldPoint);
        for (int i = 1; i <= side; i++) {
            x1 = r * sin(3.1415 / 180 * (-(360 / side) * i)) + x;
            y1 = r * cos(3.1415 / 180 * (-(360 / side) * i)) + y;
            LineTo(hBufferDC, x1, y1);
        }
        LineTo(hBufferDC, x2, y2);
    }
};
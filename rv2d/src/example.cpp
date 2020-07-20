/*
Copyright (c) 2020 seb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <iostream>
#include "rv2d.h"

int main(int argc, char *argv[])
{
    rv2d::RowVector2D<int> rv2d(5, 5);

    for (int i = 0; i < rv2d.rows(); ++i)
    {
        for (int j = 0; j < rv2d.cols(); ++j)
        {
            rv2d[i][j] = i + j;
        }
    }

    for (int i = 0; i < rv2d.rows(); ++i)
    {
        for (int j = 0; j < rv2d.cols(); ++j)
        {
            std::cout << rv2d[i][j] << " " ;
        }
        std::cout << std::endl;
    }
}
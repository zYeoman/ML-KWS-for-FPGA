/*
 * test.c
 * Copyright (C) 2018 Yongwen Zhuang <zeoman@163.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    int a,b;
    std::stringstream st;
    st<<1<<' '<<2<<4;
    st>>a;
    st>>b;
    std::cout<<a<<' '<<b;
    return 0;
}

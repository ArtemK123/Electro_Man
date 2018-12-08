#ifndef XML_H
#define XML_H

#include <QtXml>
#include <iostream>
#include <sstream>
#include <fstream>

struct Layer {
public:
    int firstgid;
    int sheet_columns;
    int sheet_rows;
    int sheet_tilecount;
    int width;
    int height;
    std::vector<int> data;
    std::string path;
};


class Parser {
public:
    static std::vector<std::shared_ptr<Layer>> parse_layers(QString path);
    static std::vector<int> parse_csv(QString data);
};



#endif // XML_H

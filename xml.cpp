#include "xml.h"

std::vector<int> Parser::parse_csv(QString data) {
    std::vector<int> res;
    std::stringstream stream(data.toStdString());
    std::string str_numb;
    while(stream.good()) {
        getline(stream, str_numb, ',');
        res.push_back(stoi(str_numb));
    }
    return res;
};

std::vector<std::shared_ptr<Layer>> Parser::parse_layers(QString path) {
    QFile* file = new QFile(path);
    std::vector<std::shared_ptr<Layer>> layers;
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return layers;
    }
    QXmlStreamReader xml(file);
    std::shared_ptr<Layer> layer = nullptr;
    while (!xml.atEnd() && !xml.hasError()) {
        if (xml.readNext() != QXmlStreamReader::EndDocument) {
            if (xml.isStartElement()) {
                if (xml.name() == "tileset") {
                    layer = std::shared_ptr<Layer>(new Layer());
                    layer->sheet_columns = xml.attributes().value("columns").toInt();
                    layer->firstgid = xml.attributes().value("firstgid").toInt();
                    layer->sheet_tilecount = xml.attributes().value("tilecount").toInt();
                    layer->sheet_rows = static_cast<int>(
                                ceil(static_cast<float>(layer->sheet_tilecount)
                                     / static_cast<float>(layer->sheet_columns)));
                }
                else if (xml.name() == "image") {
                    layer->path = xml.attributes().value("source").toString().toStdString();
                }
                //........................
                else if (xml.name() == "layer") {
                    layer = layers[static_cast<unsigned long long>(xml.attributes().value("id").toInt()-1)];
                    layer->width = xml.attributes().value("width").toInt();
                    layer->height = xml.attributes().value("height").toInt();
                }
                else if (xml.name() == "data") {
                    xml.readNext();
                    layer->data = parse_csv(xml.text().toString());
                }
            }

            else if (xml.isEndElement()) {
                if (xml.name() == "tileset") {
                    layers.push_back(layer);
                }
            }
        }
    }

    if (xml.hasError()) {
        std::cout<<xml.errorString().toStdString();
    }

    return layers;
}

#ifndef PROJECTDATASTRUCTURE_H
#define PROJECTDATASTRUCTURE_H


// Структура генерации файла (какие файлы нужно генерировать)
typedef struct {
    bool main_c;
    bool main_h;
    bool transmit_c;
    bool transmit_h;
    bool recieve_c;
    bool recieve_h;
    bool defines_h;
} project_structure_t;


#endif // PROJECTDATASTRUCTURE_H

#ifndef GENERAL_H
#define GENERAL_H

#include<string>
#include<QMessageBox>

#define APP_NAME "Ways Editor"
#define HELP_NAME "/help.txt"
#define ICON_PATH "/way_editor_icon.png"
#define BLACK_BUTTON_THEME {0, 0, 0}
#define BLACK_WINDOW_THEME {9, 27, 56}
#define WHITE_BUTTON_THEME {255, 255, 255}
#define WHITE_WINDOW_THEME {190, 230, 255}
#define EMPTY_PATH "ERROR::EMPTY_PATH"
#define FAILED_TO_OPEN "ERROR::FAILED_TO_OPEN_FILE"
#define UNDEFINED_ERROR "SOMETHING_WENT_WRONG"
#define UNDEFINED_EDGE "ERROR::FAILED_TO_FIND_EDGE"
#define UNDEFINED_VERTEX "ERROR::FAILED_TO_FIND_VERTEX"

enum exetensions{
    undefined = 0, mat, edgesList, vertexesList
};

static int getExtension(std::string path){
    std::string ext = "";
    std::string::iterator it = path.begin();
    while(*it != '.'){
        it++;
        if(it == path.end()){
            return undefined;
        }
    }
    for(; it != path.end(); it++){
        ext += (*it);
    }

    if(ext == ".mat"){
        return mat;
    }
    else if(ext == ".el"){
        return edgesList;
    }
    else if(ext == ".vl"){
        return vertexesList;
    }

    return undefined;
}

static int undefinedError(){
    QMessageBox fail;
    fail.setInformativeText(UNDEFINED_ERROR);
    fail.setWindowTitle("Fail");
    return fail.exec();
}

static int errorMassege(const char* massege){
    QMessageBox fail;
    fail.setInformativeText(massege);
    fail.setWindowTitle("Fail");
    return fail.exec();
}

static char dtoch(uint n){ // digit to char
    int _n = n % 10; // to digit
    return _n + 48; // to char
}

static std::string itos(uint n){ // integer to string
    if(!n) {
        return "0";
    }
    std::string res = "";
    while(n){
        res += dtoch(n);
        n /= 10;
    }
    return res;
}

#endif // GENERAL_H

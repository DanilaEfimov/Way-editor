#ifndef GENERAL_H
#define GENERAL_H

#include<string>
#include<QMessageBox>

#define APP_NAME "Ways Editor"
#define ICON_PATH "/way_editor_icon.png"
#define EMPTY_PATH "ERROR::EMPTY_PATH"
#define FAILED_TO_OPEN "ERROR::FAILED_TO_OPEN_FILE"
#define UNDEFINED_ERROR "SOMETHING_WENT_WWRONG"

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

#endif // GENERAL_H

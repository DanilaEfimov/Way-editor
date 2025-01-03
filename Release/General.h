#ifndef GENERAL_H
#define GENERAL_H

#include<string>
#include<map>

#define APP_NAME "Way Editor"
#define HELP_NAME "./help.txt"
#define HISTORY_NAME "./history.txt"
#define ICON_PATH "./way_editor_icon.png"

#define BLACK_BUTTON_THEME {0, 0, 0}
#define BLACK_WINDOW_THEME {22, 72, 122}
#define WHITE_BUTTON_THEME {255, 255, 255}
#define WHITE_WINDOW_THEME {190, 230, 255}
#define ERROR_BUTTON_THEME {190, 40, 40}
#define WARNING_BUTTON_THEME {170, 190, 40}

#define _ERROR_ "ERROR\n"
#define _WARNING_ "WARNING\n"
#define _ERROR_BOX_ "Error message\n"
#define _WARNING_BOX_ "Warning message\n"
#define _FAILED_TO_OPEN_ "Failed to open file\n"
#define _FILE_NOT_CHOOSED_ "File was not choosed\n"
#define _UNCORRECT_FILE_NAME_ "Uncorrect file name\n"
#define _ERROR_FILE_TYPE_ "Undefined file extention or graph\n"
#define _ERROR_GRAPH_TYPE_ "Undefined graph type\n"

#define __UNCORRECT_INPUT_FILE__ "File contains mistakes\n"
#define __EMPTY_GRAPH_SETTED__ "Was created empty graph\n"

#define _CONSOLE_START_ "You have created a new graph!\n"
#define _NEW_EMPTY_GRAPH_ "You have create an empty graph!\n"

#define OUT_SEED 10 /* it means count of double numbers in string, which will be printed */
#define _HELLO_ " Hello, Way Editor! \n \
It's little framework for working \n \
with math graph. \n \
best wishes WarerCode 2025 (c). \n \
\n \
You have three menues: File, View and Command Line. \n \
File menu: new file, save (saves current graph session) \n \
View menu: right/left mode, black/white theme \n \
Command Line menu: help (there are more about commands), hystory (about all last sessions). \n \
\n \
You can load graph from .vl/.mat/.el file \n \
for start push, please, 'file menu' -> 'new file' and choose \n \
Your graph file (not .txt)."

typedef unsigned int uint;
typedef std::pair<std::string, int> graphKey, extKey;

enum names {
    udirgraph = 1, dirgraph, udwgraph, wdgraph,
    upseudograph, dpseudograph, tree,
    wtree, bitree
};

enum exts {
    MAT = 1, VL, EL     // VL - vertex list, EL - edge list
};

const std::map<std::string, int> types {
    graphKey("UDirGraph", udirgraph), graphKey("DirGraph", dirgraph), graphKey("UDWGraph", udwgraph),
    graphKey("WDGraph", wdgraph), graphKey("UPseudoGraph", upseudograph), graphKey("DPseudoGraph", dpseudograph),
    graphKey("Tree", tree), graphKey("WTree", wtree), graphKey("BiTree", bitree),
};

const std::map<std::string, int> extentions {
    extKey(".mat", MAT), extKey(".MAT", MAT), extKey(".Mat", MAT),
    extKey(".vl", VL), extKey(".VL", VL), extKey(".Vl", VL),
    extKey(".el", EL), extKey(".EL", EL), extKey(".El", EL),
};

const std::map<std::string, int> commands {

};

const std::map<int, int> argcount {

};

#endif // GENERAL_H

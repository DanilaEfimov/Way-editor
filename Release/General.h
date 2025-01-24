#ifndef GENERAL_H
#define GENERAL_H

#ifndef GRAPHS
#define GRAPHS
#   include "Graphs/BiTree.h"
#   include "Graphs/Tree.h"
#   include "Graphs/WTree.h"
#   include "Graphs/DPseudoGraph.h"
#   include "Graphs/UPseudoGraph.h"
#   include "Graphs/DirGraph.h"
#   include "Graphs/UDWGraph.h"
#   include "Graphs/UDirGraph.h"
#   include "Graphs/UDWGraph.h"
#   include "Graphs/WDGraph.h"
#   include "Graphs/Graph.h"
#endif

#include <string>
#include <map>

#define NOTE_TAB "Hello, Way Editor!"
#define APP_NAME "Way Editor"
#define HELP_NAME "./help.txt"
#define HYSTORY_NAME "./hystory.txt"
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
#define _FAILED_TO_OPEND_ "Failed to open directori\n"
#define _FILE_NOT_CHOOSED_ "File was not choosed\n"
#define _UNCORRECT_FILE_NAME_ "Uncorrect file name\n"
#define _UNDEFINED_ERROR_ "Something went wrong...\n"
#define _ERROR_FILE_TYPE_ "Undefined file extention or graph\n"
#define _ERROR_GRAPH_TYPE_ "Undefined graph type\n"
#define _INVALID_ARGUMENT_ "Was inputed invalid argument\n"
#define _INVALID_ARGUMENT_COUNT_ "Invalid argument count\n"

#define __UNCORRECT_INPUT_FILE__ "File contains mistakes\n"
#define __EMPTY_GRAPH_SETTED__ "Was created empty graph\n"
#define __SYNTAX_ERROR__ "Syntax error\n"
#define __INVALID_COMMAND__ "Invalid command\n"
#define __EMPTY_INPUT__ "Line is empty\n"
#define __UNDEFINED_BAHAVIOUR__ "Undefined bahaviour\ncheck Your input\n"

#define _CONSOLE_START_ "You have created a new graph!\n"
#define _NEW_EMPTY_GRAPH_ "You have create an empty graph!\n"

#define OUT_SEED 10 /* it means count of double numbers in string, which will be printed */
#define FONT_SIZE 12 /* in points */
#define _CLEAN_ ""
#define _MAT_HELLO_ "\nhere writen matrix of graph\n"
#define _VL_HELLO_ "\nhere writen vertex list of graph\n"
#define _CHOOSE_ "Choose graph file (.mat/.vl/.el)\n"
#define _HELLO_ " Hello, Way Editor! \n \
It's a little framework for working \n \
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
typedef std::pair<std::string, int> graphKey, extKey, cmdKey;
typedef std::pair<int, int> countKey;
typedef std::pair<uint, uint> edge_t;

enum names {
    graph = 0,
    udirgraph = 1, dirgraph, udwgraph, wdgraph,
    upseudograph, dpseudograph, tree,
    wtree, bitree
};

enum exts {
    MAT = 1, VL, EL     // VL - vertex list, EL - edge list
};

enum count {
    ZERO, ONE, TWO, THREE,
    LIMITLESS,
};

enum functions { // names of functions & their id
    clear = 0x00,   // clear console input
    weight = 0x01,
    addV = 0x10, addE, eraseV, eraseE,
    EulerCycle = 0x20, CycleBase, Blocks,
    MaxV, MaxE, Degree, Weight, // Weight means 'getWeight'
    VW = 0x30, EW, // set vertexes weights; set edges weights
    computeDFS = 0x40, computeBFS, computePrima, Dejcstra
};

inline const std::map<std::string, int> types {
    graphKey("UDirGraph", udirgraph), graphKey("DirGraph", dirgraph), graphKey("UDWGraph", udwgraph),
    graphKey("WDGraph", wdgraph), graphKey("UPseudoGraph", upseudograph), graphKey("DPseudoGraph", dpseudograph),
    graphKey("Tree", tree), graphKey("WTree", wtree), graphKey("BiTree", bitree),
};

inline const std::map<std::string, int> extentions {
    extKey(".mat", MAT),    extKey(".MAT", MAT),    extKey(".Mat", MAT),
    extKey(".vl", VL),      extKey(".VL", VL),      extKey(".Vl", VL),
    extKey(".el", EL),      extKey(".EL", EL),      extKey(".El", EL),
};

inline const std::map<std::string, int> commands {
    cmdKey("clear", clear),
    cmdKey("weight", weight), cmdKey("addV", addV), cmdKey("addE", addE),
    cmdKey("eraseV", eraseV), cmdKey("eraseE", eraseE), cmdKey("EulerCycle", EulerCycle),
    cmdKey("CycleBase", CycleBase), cmdKey("Blocks", Blocks), cmdKey("MaxV", MaxV),
    cmdKey("MaxE", MaxE), cmdKey("Degree", Degree), cmdKey("Weight", Weight),
    cmdKey("VW", VW), cmdKey("EW", EW), cmdKey("computeDFS", computeDFS),
    cmdKey("computeBFS", computeBFS), cmdKey("computePrima", computePrima),
    cmdKey("Dejcstra", Dejcstra),
};

inline const std::map<int, int> argcount {
    countKey(weight, TWO), countKey(addV, LIMITLESS), countKey(addE, LIMITLESS),
    countKey(eraseV, LIMITLESS), countKey(eraseE, LIMITLESS), countKey(EulerCycle, ONE),
    countKey(CycleBase, ZERO), countKey(Blocks, ZERO), countKey(MaxV, ZERO),
    countKey(MaxE, ZERO), countKey(Degree, ONE), countKey(Weight, TWO),
    countKey(VW, ONE), countKey(EW, THREE), countKey(computeDFS, ONE),
    countKey(computeBFS, ONE), countKey(computePrima, ONE),
    countKey(Dejcstra, TWO),
};

#endif // GENERAL_H

#include "statMmanager.h"

StateManager* StateManager::state;

StateManager::StateManager(QObject *parent) : QObject(parent)
{
    colorBoard = false;
    hiddenNodeAct = false;
    delNodeAct = false;
    selectModel = true;
    mergeNodeAct = false;
    //初始值设为true
//    moneyRelation = true;
//    idRelation = true;
//    addrRelation = true;
//    societyRelation = true;
//    IPRelation = true;
//    phoneRelation = true;
//    MacRelation = true;
//    ATMRelation = true;
}

StateManager::~StateManager()
{

}
StateManager* StateManager::stateIns()
{
    if(state == NULL)
        state = new StateManager();
    return state;
}


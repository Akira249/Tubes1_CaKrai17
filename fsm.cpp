#include "fsm.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <limits>
using namespace std;

uint32_t millis() {
    using namespace chrono;
    return static_cast<uint32_t>(duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()).count());
}

FSM::FSM() : currentState(SystemState::INIT), lastHeartbeat(0), 
             delay(0), errorCount(0), moveCount(0) {
    stateHistory.clear();
    stateHistory.emplace_back(currentState, millis());
}

FSM::FSM(uint32_t delay) : currentState(SystemState::INIT), lastHeartbeat(0),
                          delay(delay), errorCount(0), moveCount(0) {
    stateHistory.clear();
    stateHistory.emplace_back(currentState, millis());
}

FSM::~FSM() {
    stateHistory.clear();
}

SystemState FSM::getCurrentState() const {
    return currentState;
}

void FSM::transitionToState(SystemState newState) {
    currentState = newState;
    lastHeartbeat = millis();
    addStateToHistory(currentState, lastHeartbeat);
}

void FSM::setDelay(uint32_t delay) {
    this->delay = delay;
}

void FSM::getDelay(uint32_t &delay) const {
    delay = this->delay;
}

void FSM::setErrorCount(int count) {
    errorCount = count;
}

int FSM::getErrorCount() const {
    return errorCount;
}

void FSM::setMoveCount(int count) {
    moveCount = count;
}

int FSM::getMoveCount() const {
    return moveCount;
}

void FSM::addStateToHistory(SystemState state, uint32_t time) {
    stateHistory.emplace_back(state, time);
}

vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const {
    return stateHistory;
}

uint32_t FSM::getLastHeartbeat() const {
    return lastHeartbeat;
}

void FSM::setLastHeartbeat(uint32_t heartbeat) {
    lastHeartbeat = heartbeat;
}

void FSM::start() {
    while (currentState != SystemState::STOPPED) {
        update();
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void FSM::update() {
    switch (currentState) {
        case SystemState::INIT:
            performInit();
            break;
        case SystemState::IDLE:
            performProcess();
            break;
        case SystemState::MOVEMENT:
            performMovement();
            break;
        case SystemState::SHOOTING:
            performShooting();
            break;
        case SystemState::CALCULATION:
            performCalculation();
            break;
        case SystemState::ERROR:
            performErrorHandling();
            break;
        case SystemState::STOPPED:
            shutdown();
            break;
    }
    lastHeartbeat = millis();
    addStateToHistory(currentState, lastHeartbeat);
}

void FSM::printStatus() {
    cout << "Current State: ";
    switch (currentState) {
        case SystemState::INIT: cout << "INIT"; break;
        case SystemState::IDLE: cout << "IDLE"; break;
        case SystemState::MOVEMENT: cout << "MOVEMENT"; break;
        case SystemState::SHOOTING: cout << "SHOOTING"; break;
        case SystemState::CALCULATION: cout << "CALCULATION"; break;
        case SystemState::ERROR: cout << "ERROR"; break;
        case SystemState::STOPPED: cout << "STOPPED"; break;
    }
    cout << "\nLast Heartbeat: " << lastHeartbeat
         << "\nDelay: " << delay
         << "\nError Count: " << errorCount
         << "\nMove Count: " << moveCount << endl;
}

void FSM::printStateHistory() {
    cout << "State History:\n";
    for (const auto& entry : stateHistory) {
        cout << "State: ";
        switch (entry.first) {
            case SystemState::INIT: cout << "INIT"; break;
            case SystemState::IDLE: cout << "IDLE"; break;
            case SystemState::MOVEMENT: cout << "MOVEMENT"; break;
            case SystemState::SHOOTING: cout << "SHOOTING"; break;
            case SystemState::CALCULATION: cout << "CALCULATION"; break;
            case SystemState::ERROR: cout << "ERROR"; break;
            case SystemState::STOPPED: cout << "STOPPED"; break;
        }
        cout << " at " << entry.second << " ms" << endl;
    }
}

void FSM::performInit() {
    cout << "Initializing system..." << endl;
    delay = 1000;
    transitionToState(SystemState::IDLE);
    printStatus();
}

void FSM::performProcess() {
    cout << "\nChoose an option:\n"
         << "1. Display status and state history (IDLE)\n"
         << "2. Move (MOVEMENT)\n"
         << "3. Shoot (SHOOTING)\n"
         << "4. Calculate (CALCULATION)\n"
         << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            printStatus();
            printStateHistory();
            break;
        case 2:
            transitionToState(SystemState::MOVEMENT);
            break;
        case 3:
            transitionToState(SystemState::SHOOTING);
            break;
        case 4:
            transitionToState(SystemState::CALCULATION);
            break;
        default:
            cout << "Invalid choice, staying in IDLE state." << endl;
    }
}

void FSM::performMovement() {
    cout << "Moving..." << endl;
    moveCount++;
    if (moveCount >= 3) {
        transitionToState(SystemState::SHOOTING);
    } else {
        transitionToState(SystemState::IDLE);
    }
}

void FSM::performShooting() {
    cout << "Shooting..." << endl;
    moveCount = 0;
    transitionToState(SystemState::IDLE);
}

void FSM::performCalculation() {
    cout << "Performing calculation..." << endl;
    if (moveCount == 0) {
        transitionToState(SystemState::ERROR);
    } else {
        transitionToState(SystemState::IDLE);
    }
}

void FSM::performErrorHandling() {
    cout << "Error occurred, performing error handling..." << endl;
    errorCount++;
    if (errorCount > 3) {
        transitionToState(SystemState::STOPPED);
    } else {
        transitionToState(SystemState::IDLE);
    }
}

void FSM::shutdown() {
    cout << "System stopped, shutting down..." << endl;
    stateHistory.clear();
}
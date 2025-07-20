#ifndef FSM_H_
#define FSM_H_

#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>
#include <vector>

using namespace std;

enum class SystemState {
        INIT,
        IDLE, 
        MOVEMENT,
        SHOOTING,
        CALCULATION,
        ERROR,
        STOPPED
};

uint32_t millis();

class FSM {

        private:
        SystemState currentState;       // Current state of the FSM
        uint32_t lastHeartbeat;            // Last heartbeat time in milliseconds
        uint32_t delay;                 // Delay in milliseconds for each state transition
        int errorCount;                 // Count of errors encountered
        vector<pair<SystemState, uint32_t>> stateHistory;  // List of state and time pairs
        int moveCount;              // Count of movements performed, if 3 moves are performed, the FSM will transition to SHOOTING state.

        public: 
        /**
         * @brief set currentState to INIT, lastHeartbeat and errorCount to 0, don't forget to intialize the stateHistory vector.
         * @note This is a constructor, by default C++ will generate a default constructor if none is provided.
         * @note The downside of this constructor is empty, meaning it does not perform any initialization if needed.
         */
        FSM();

        /**
         * @brief set currentState to INIT, lastHeartbeat and errorCount to 0, set delay to param, don't forget to intialize the stateHistory vector.
         * @param state The initial state of the FSM.
         * @param delay The delay in milliseconds for each state transition.
         * @note this is a user-defined constructor, initializes FSM with a specified delay.
         */
        FSM(uint32_t delay);

        /**
         * @brief Destructor for the FSM class.
         * @note This is a destructor, by default C++ will generate a default destructor if none is provided.
         * @note The downside of this destructor is empty, it doesn't perform any malloc cleanup, don't forget to cleanup the vector.
         */
        ~FSM();

        /**
         * @brief Get the current state of the FSM.
         * @note This function returns the copy to the fsmState attribute of the FSM.
         * @return The current state of the FSM as a SystemState enum.
         */
        SystemState getCurrentState() const;

        /**
         * @brief Transition to a new state.
         * @param newState The new state to transition to.
         * @note This function updates the current state of the FSM to the new state.
         * It also updates the last heartbeat time to the current time in milliseconds.
         */
        void transitionToState(SystemState newState);

        /**
         * @brief set the delay for the FSM.
         * @param delay The delay in milliseconds to set for the FSM.
         */
        void setDelay(uint32_t delay);

        /**
         * @brief Get the delay for the FSM.
         * @param delay Reference to a uint32_t variable to store the delay in milliseconds.
         */
        void getDelay(uint32_t &delay) const;

        /**
         * @brief Set the error count for the FSM.
         * @param count The number of errors to set.
         */
        void setErrorCount(int count);

        /**
         * @brief Get the error count for the FSM.
         * @return The number of errors encountered by the FSM.
         */
        int getErrorCount() const;

        /**
         * @brief Set the move count for the FSM.
         * @param count The number of moves to set.
         */
        void setMoveCount(int count);

        /**
         * @brief Get the move count for the FSM.
         * @return The number of moves performed by the FSM.
         */
        int getMoveCount() const;

        /**
         * @brief Add a state and its corresponding time to the state history.
         * @param state The state to add to the history.
         * @param time The time in milliseconds when the state was entered.
         * @note This function appends a pair of state and time to the stateHistory vector.
         */
        void addStateToHistory(SystemState state, uint32_t time);

        /**
         * @brief Get the state history of the FSM.
         * @return A vector of pairs containing the state and the time it was entered.
         * @note This function returns a copy of the stateHistory vector.
         */
        vector<pair<SystemState, uint32_t>> getStateHistory() const;

        /**
         * @brief Get the last heartbeat time of the FSM.
         * @return The last heartbeat time in milliseconds.
         * @note This function returns the lastHeartbeat attribute of the FSM.
         */
        uint32_t getLastHeartbeat() const;

        /**
         * @brief Set the last heartbeat time of the FSM.
         * @param heartbeat The time in milliseconds to set as the last heartbeat.
         */
        void setLastHeartbeat(uint32_t heartbeat);

        /**
         * @brief Start the FSM.
         * This function initializes the FSM and begins the state update loop.
         * Create a loop that checks the current state every 1000 milliseconds, 
         */
        void start();

        /**
         * @brief Update the FSM state based on the current state.
         * Check the attribute fsmState to determine its next process:
         * - INIT: calls performInit()
         * - IDLE: calls performProcess()
         * - MOVEMENT: calls performMovement()
         * - SHOOTING: calls performShooting()
         * - CALCULATION: calls performCalculation()
         * - ERROR: Handle error conditions.
         * - STOPPED: Stop the system operations.
         * Update the lastHeartbeat attribute to the current time in milliseconds.
         * Emplace the stateHistory vector with the current state and current time in milliseconds.
         */
        void update();

        /**
         * @brief Print the current status of the FSM.
         * This function prints the current state, last heartbeat time, delay, error count
         */
        void printStatus();
        
        /**
         * @brief Print the state history of the FSM.
         * This function prints the state history, showing each state and the time it was entered.
         * It iterates through the stateHistory vector and prints each state and its corresponding time.
         */
        void printStateHistory();

        /**
         * @brief Perform the initialization process.
         * set delay to 1000 milliseconds,
         * set currentState to IDLE after initialization.
         * set lastHeartbeat to current time in milliseconds.
         * @note This function is called when the FSM is in the INIT state.
         * print "Initializing system..." then invoke printStatus()
         */
        void performInit();

        /**
         * @brief Prompt the user to pick a process to be done.
         * If a command is received, set currentState to the process' state.
         * user is prompted to choose from: 
         * - displaying status and state history (IDLE)
         * - moving (MOVEMENT)
         * - shooting (SHOOTING)
         * - calculating (CALCULATION)
         * @note This function is called when the FSM is in the IDLE state.
         */
        void performProcess();

        /**
         * @brief Perform the movement process.
         * This function is called when the FSM is in the MOVEMENT state.
         * Print "Moving..."
         * Increment moveCount by 1.
         * If moveCount (after increment) reaches 3, transition to the SHOOTING state.
         * if not, transition to the IDLE state.
         * Update the last heartbeat time to the current time in milliseconds.
         */
        void performMovement();

        /**
         * @brief Perform the shooting process.
         * This function is called when the FSM is in the SHOOTING state.
         * Print "Shooting..."
         * Reset moveCount to 0.
         * Transition to the IDLE state.
         * Update the last heartbeat time to the current time in milliseconds.
         */
        void performShooting();

        /**
         * @brief Perform the calculation process.
         * This function is called when the FSM is in the CALCULATION state.
         * Print "Performing calculation..."
         * If moveCount is 0, transition to the ERROR state.
         * if moveCount is greater than 0, transition to the IDLE state.
         */
        void performCalculation();

        /**
         * @brief Handle error conditions.
         * This function is called when the FSM is in the ERROR state.
         * Print "Error occurred, performing error handling..."
         * Increment errorCount by 1.
         * If errorCount exceeds 3, transition to the STOPPED state.
         * If not, transition to the IDLE state.
         */
        void performErrorHandling();

        /**
         * @brief shutdown the FSM if the current state is STOPPED.
         * This function is called when the FSM is in the STOPPED state.
         * Print "System stopped, shutting down..."
         * Clear the stateHistory vector.
         */
        void shutdown();

};

#endif // FSM_H_

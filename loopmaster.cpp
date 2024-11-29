#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>

using namespace std;

struct Loop {
    int totalIterations; // Total iterations for the loop
    int currentIteration; // Current iteration count
    int breakAt; // Iteration number to break at (0 means no break)
    int continueAt; // Iteration number to continue at (0 means no continue)
};

void executeCommands(const vector<string>& commands) {
    stack<Loop> loopStack; // Stack to manage nested loops
    vector<string> output; // To store the output from print statements
    bool shouldPrint = true; // Flag to control printing

    for (const string& command : commands) {
        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "for") {
            int times;
            ss >> times; // Read the number of times
            loopStack.push({times, 0, 0, 0}); // Push a new loop onto the stack
            shouldPrint = true; // Reset print flag for new loop
        } else if (cmd == "do") {
            // Do nothing, just move to the next command
        } else if (cmd == "done") {
            // End the current loop
            if (!loopStack.empty()) {
                Loop& currentLoop = loopStack.top();
                currentLoop.currentIteration++;
                
                // Check if we need to continue or break
                if (currentLoop.breakAt > 0 && currentLoop.currentIteration >= currentLoop.breakAt) {
                    loopStack.pop(); // Break out of the loop
                } else if (currentLoop.currentIteration < currentLoop.totalIterations) {
                    // If there are more iterations left, keep the loop
                    loopStack.push(currentLoop);
                } else {
                    loopStack.pop(); // Finished this loop
                }
            }
            shouldPrint = true; // Reset print flag after loop ends
        } else if (cmd == "break") {
            int n;
            ss >> n; // Read the break iteration
            if (!loopStack.empty()) {
                Loop& currentLoop = loopStack.top();
                currentLoop.breakAt = n; // Set break iteration
            }
        } else if (cmd == "continue") {
            int n;
            ss >> n; // Read the continue iteration
            if (!loopStack.empty()) {
                Loop& currentLoop = loopStack.top();
                if (currentLoop.currentIteration + 1 == n) {
                    // Skip to the next iteration
                    currentLoop.currentIteration++; // Move to the next iteration
                    shouldPrint = false; // Prevent printing for this iteration
                }
            }
        } else if (cmd == "print") {
            string statement;
            getline(ss, statement);
            if (!statement.empty()) {
                // Remove leading spaces and quotes
                statement.erase(0, statement.find_first_not_of(' '));
                if (statement.front() == '"') {
                    statement.erase(0, 1); // Remove opening quote
                }
                if (statement.back() == '"') {
                    statement.pop_back(); // Remove closing quote
                }
                if (shouldPrint) {
                    output.push_back(statement); // Store the output if we should print
                }
            }
        }

        // After processing each command, check if we need to print
        if (!loopStack.empty() && shouldPrint) {
            // Print output collected so far
            for (const string& line : output) {
                cout << line << endl;
            }
        }
    }
}

int main() {
    int N;
    cin >> N;
    cin.ignore(); // Ignore the newline after the number of lines

    vector<string> commands(N);
    for (int i = 0; i < N; ++i) {
        getline(cin, commands[i]);
    }

    executeCommands(commands);

    return 0;
}
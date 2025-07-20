#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

bool isExitCommand(const std::string& s) {
    std::string input = s;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return (input == "exit" || input == "quit" || input == "leave" ||
            input == "e"    || input == "q"    || input == "l");
}

bool getIntOrExit(const std::string& prompt, int& out) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    // Check for exit commands
    if (isExitCommand(input)) return false;

    std::stringstream ss(input);
    ss >> out;
    if (ss.fail()) {
        std::cout << "Invalid number, please try again.\n";
        return getIntOrExit(prompt, out); // retry
    }
    return true;
}

int main() {
    while (true) {
        int current_value, goal_value, valve[3];

        // Get current_value
        if (!getIntOrExit("Enter current value: ", current_value)) break;

        // Get goal_value
        if (!getIntOrExit("Enter goal value: ", goal_value)) break;

        // Get each valve value
        for (int i = 0; i < 3; ++i) {
            std::string prompt = "valve" + std::to_string(i+1) + ": ";
            if (!getIntOrExit(prompt, valve[i])) goto done;
        }

        // Plus, Minus, or Remain Unchanged
        bool found = false;
        for (int combo = 0; combo < 27; ++combo) { // 3^3 = 27
            int ops[3], temp = combo;
            for (int i = 0; i < 3; ++i) {
                ops[i] = temp % 3; // 0=+, 1=-, 2=skip
                temp /= 3;
            }

            int value = current_value;
            std::string expr = std::to_string(current_value);

            // Build expression with colored output
            for (int i = 0; i < 3; ++i) {
                if (ops[i] == 0) {
                    value += valve[i];
                    expr += " \033[32m+" + std::to_string(valve[i]) + "\033[0m";
                } else if (ops[i] == 1) {
                    value -= valve[i];
                    expr += " \033[31m-" + std::to_string(valve[i]) + "\033[0m";
                }
                // skip does nothing
            }

            if (value == goal_value) {
                std::cout << expr << " = " << goal_value << std::endl;
                found = true;
            }
        }
        if (!found) {
            std::cout << "\033[31mNo combination found, please check again!\033[0m" << std::endl;
        }
        std::cout << std::endl;
    }
done:
    std::cout << "Goodbye!" << std::endl;
    return 0;
}

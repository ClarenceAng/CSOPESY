#include "MarqueeConsole.h"
#include "ConsoleManager.h"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <unistd.h>

    char _getch() {
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);   // disable buffering and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        char ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // restore terminal
        return ch;
    }

    // currently does not work correctly
    bool _kbhit() {
        termios term;
        tcgetattr(0, &term); // Get current terminal settings

        termios term2 = term;
        term2.c_lflag &= ~ICANON; // Disable line-buffering (Enter key requirement)
        tcsetattr(0, TCSANOW, &term2);

        int byteswaiting;
        ioctl(0, FIONREAD, &byteswaiting); // Check for bytes in the buffer

        tcsetattr(0, TCSANOW, &term); // Restore original settings
        return byteswaiting > 0;
    }
#endif

ConsoleDimensions getConsoleDimensions() {
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return {
            csbi.srWindow.Right  - csbi.srWindow.Left + 1,
            csbi.srWindow.Bottom - csbi.srWindow.Top  + 2
        };
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return { w.ws_col, w.ws_row };
    #endif
}

MarqueeConsole::MarqueeConsole() {}

void MarqueeConsole::run() {
    std::string text = "This is a Marquee Console.";
    std::string cmd;
    char ch;
    int row = 0;
    int vDirection = 1;
    int hDirection = 1;
    int splitFactor = 2;
    int originalLength = text.length();

    running = true; 

    while (running) {
        while(!_kbhit()){
            currentDimensions = getConsoleDimensions();
    
             std::cout << "\033[H";
    
            for (int i = 0; i < (currentDimensions.height + 1) / splitFactor; i++) {
                std::cout << "\033[2K\n";
            }
    
            std::cout << "\033[1;1H";
    
            for (int i = 0; i < (currentDimensions.height + 1) / splitFactor; i++) {
                if (row == i) {
                    std::cout << text << std::endl;
                } else {
                    std::cout << std::endl;
                }
            }
            
            if (row == 0) {
                vDirection = 1;
            }
            
            if (row >= currentDimensions.height / splitFactor) {
                vDirection = -1;
            }
    
            if (text.length() == originalLength) {
                hDirection = 1;
            }
            
            if (text.length() >= currentDimensions.width) {
                hDirection = -1;
            }
            
            row += vDirection;
            
            if (hDirection == 1) {
                text = " " + text;
            } else {
                text.erase(0, 1);
            }

            std::cout << "Enter a command: " << std::flush;
            
            std::cout << cmd << std::endl << std::flush;

            for (int i = 0; i < consoleHistory.size(); i++) {
                std::cout << consoleHistory.at(i) << std::endl << std::flush;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        
        if (_kbhit()) {
            ch = _getch();

            if (ch == '\r' || ch == '\n') {
                std::cout << std::endl;
                executeCommand(cmd);
                cmd = "";
            } else if (ch == 127 || ch == '\b') {
                if (!cmd.empty()) {
                    cmd.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
            } else {
                cmd += ch;
            }
        }
    }
}

void MarqueeConsole::executeCommand(std::string cmd) {
    if (cmd == "exit") {
        ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
        running = false;
    } else {
        consoleHistory.push_back("Command \'" + cmd + "\' recognized.");
    }
}
#pragma once

#ifdef SOME_RULE

#else
    #include <iostream>

    #define logDbg(TAG, MSG_TO_FORMAT) { std::cout << TAG << ": " << MSG_TO_FORMAT << std::endl; }
    #define logInf(TAG, MSG_TO_FORMAT) { std::cout << TAG << ": " << MSG_TO_FORMAT << std::endl; }
    #define logWrn(TAG, MSG_TO_FORMAT) { std::cout << TAG << ": " << MSG_TO_FORMAT << std::endl; }
    #define logErr(TAG, MSG_TO_FORMAT) { std::cout << TAG << ": " << MSG_TO_FORMAT << std::endl; }
    #define logFat(TAG, MSG_TO_FORMAT) { std::cout << TAG << ": " << MSG_TO_FORMAT << std::endl; }
#endif



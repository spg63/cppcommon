#include "../../OutputModifier.hpp"
#include <iostream>

int main(){
    Output::Modifier fg_red(Output::Foreground::RED);
    Output::Modifier bg_red(Output::Background::RED);
    Output::Modifier fg_green_bg_lg(Output::Foreground::GREEN, Output::Background::WHITE);
    Output::Modifier bold(Output::Control::BOLD);
    Output::Modifier three(Output::Foreground::RED, Output::Background::WHITE, Output::Control::BOLD);
    Output::Modifier dim(Output::Control::DIM);
    Output::Modifier reset(Output::Reset::ALL);
    

    // NOTE: You need to reset when you want the color to go back to 'default'
    // NOTE: Also note the newline needs to come after the reset
    // 
    std::cout << bg_red << "Red background" << reset << "\n";
    std::cout << fg_red << "Red text" << reset << "\n";
    std::cout << fg_green_bg_lg << "Green text on white backgroud" << reset << std::endl;
    std::cout << bold << "Default font, bolded" << reset << std::endl;
    std::cout << three << "Red text on white background, bolded" << reset << std::endl;
    std::cout << dim << "Dimmed text" << reset << "\n";
    std::cout << "Back to default text, background\n";

    std::cout << "modifier 1: " << three.modifier1() << "\n";
    std::cout << "modifier 2: " << three.modifier2() << "\n";
    std::cout << "modifier 3: " << three.modifier3() << "\n";
    std::cout << "TTYStatus(): " << three.TTYStatus() << "\n";
    std::cout << "shouldColor(): " << three.shouldColor() << "\n";
    std::cout << std::endl;

    return 0;
}

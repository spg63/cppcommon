//
//  OutputModifier.hpp
//  cppcommon
//
//  Created by Sean Grimes on 12/11/15.
//  Copyright © 2016. All rights reserved.
//
// Modeled from:
//      http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
//      Answer #2, Joel Sjogren

#pragma once

#include <ostream>
#ifdef __APPLE__
    #include <sys/uio.h>
#else
    #include <sys/io.h>
#endif
#include <unistd.h>

using OutputMod = short;

/**
    \brief Easily change the text and text background color for console output
    \details Class will do two basic checks to determine if output file (stdout / stderr) supports 
    modification. \n
    For example, IDEs like Xcode do not support output modification and it does not ignore the 
    control characters. This code will detect Xcode and ignore modification characters.
    \note Not all terminals support all modifiers, most modern terminals should ignore modifiers 
    they don't support but I haven't tested nearly all the available terminals
    \author Sean Grimes, spg63@cs.drexel.edu
    \date 12-11-15
 */
namespace Output{

    /**
        \brief Modifiers for foreground color
        \details Provides: \n
        - DEFAULT \n
        - BLACK \n
        - RED \n
        - GREEN \n
        - YELLOW \n
        - BLUE \n
        - MAGENTA \n
        - CYAN \n
        - LIGHT_GRAY \n
        - GRAY \n
        - LIGHT_RED \n
        - LIGHT_GREEN \n
        - LIGHT_YELLOW \n
        - LIGHT_BLUE \n
        - LIGHT_MAGENTA \n
        - LIGHT_CYAN \n
        - WHITE
     */
    namespace Foreground{
        static const OutputMod DEFAULT          = 39; 
        static const OutputMod BLACK            = 30; 
        static const OutputMod RED              = 31;
        static const OutputMod GREEN            = 32;
        static const OutputMod YELLOW           = 33;
        static const OutputMod BLUE             = 34;
        static const OutputMod MAGENTA          = 35;
        static const OutputMod CYAN             = 36;
        static const OutputMod LIGHT_GRAY       = 37;
        static const OutputMod GRAY             = 90;
        static const OutputMod LIGHT_RED        = 91;
        static const OutputMod LIGHT_GREEN      = 92;
        static const OutputMod LIGHT_YELLOW     = 93;
        static const OutputMod LIGHT_BLUE       = 94;
        static const OutputMod LIGHT_MAGENTA    = 95;
        static const OutputMod LIGHT_CYAN       = 96;
        static const OutputMod WHITE            = 97;
    }
    
    /**
        \brief Modifiers for background color
        \details Provides: \n
        - DEFAULT \n
        - BLACK \n
        - RED \n
        - GREEN \n
        - YELLOW \n
        - BLUE \n
        - MAGENTA \n
        - CYAN \n
        - LIGHT_GRAY \n
        - GRAY \n
        - LIGHT_RED \n
        - LIGHT_GREEN \n
        - LIGHT_BLUE \n
        - LIGHT_MAGENTA \n
        - LIGHT_CYAN \n
        - WHITE
    */
    namespace Background{
        static const OutputMod DEFAULT          = 49;
        static const OutputMod BLACK            = 40;
        static const OutputMod RED              = 41;
        static const OutputMod GREEN            = 42;
        static const OutputMod YELLOW           = 43;
        static const OutputMod BLUE             = 44;
        static const OutputMod MAGENTA          = 45;
        static const OutputMod CYAN             = 46;
        static const OutputMod LIGHT_GRAY       = 47;
        static const OutputMod GRAY             = 100;
        static const OutputMod LIGHT_RED        = 101;
        static const OutputMod LIGHT_GREEN      = 102;
        static const OutputMod LIGHT_BLUE       = 104;
        static const OutputMod LIGHT_MAGENTA    = 105;
        static const OutputMod LIGHT_CYAN       = 106;
        static const OutputMod WHITE            = 107;
    }
    
    /**
        \brief Modifiers for controls
        \details Provides: \n
        - BOLD \n
        - DIM \n
        - UNDERLINE \n
        - BLINK \n
        - INVERT \n
        - HIDDEN 
    */
    namespace Control{
        static const OutputMod BOLD             = 1;
        static const OutputMod DIM              = 2;
        static const OutputMod UNDERLINE        = 4;
        static const OutputMod BLINK            = 5;
        static const OutputMod INVERT           = 7;
        static const OutputMod HIDDEN           = 8;
    }
    
    /**
        \brief Modifiers for resets
        \details Provides: \n
        - ALL \n
        - BOLD \n
        - DIM \n
        - UNDERLINED \n
        - BLINK \n
        - REVERSE \n
        - HIDDEN \n
        - BACKGROUND \n
        - FOREGROUND 
    */
    namespace Reset{
        static const OutputMod ALL              = 0;
        static const OutputMod BOLD             = 21;
        static const OutputMod DIM              = 22;
        static const OutputMod UNDERLINED       = 24;
        static const OutputMod BLINK            = 25;
        static const OutputMod REVERSE          = 27;
        static const OutputMod HIDDEN           = 28;
        static const OutputMod BACKGROUND       = 49;
        static const OutputMod FOREGROUND       = 39;
    }
    
    /**
        \brief The modifier class
        \details This class uses the modifiers to format the output text
    */
    class Modifier{
    public:
        
        /**
            \brief Single modifier c'tor
            @param modifier_1 The modifier
        */
        Modifier(OutputMod modifier_1) : Modifier(modifier_1, -1, -1) {}
        
        /**
            \brief Dual modifier c'tor
            @param modifier_1 The first modifier
            @param modifier_2 The second modifier
        */
        Modifier(OutputMod modifier_1, OutputMod modifier_2)
        : Modifier(modifier_1, modifier_2, -1)
        {}
        
        /**
            \brief 3 modifier c'tor
            @param modifier_1 The first modifier
            @param modifier_2 The second modifier
            @param modifier_3 The third modifier
        */
        Modifier(OutputMod modifier_1, OutputMod modifier_2, OutputMod modifier_3)
        : modifier_1_(modifier_1)
        , modifier_2_(modifier_2)
        , modifier_3_(modifier_3)
        {
            // IDEs like XCode will report true for isatty, however it doesn't support the modifier
            // codes and doesn't bother ignoring them so output is all garbled, hence the second check
            // to getenv("TERM")
            is_tty_ = isatty(fileno(stdin));
            TERM_ = std::getenv("TERM");
            
            if(is_tty_ && TERM_)
                should_color_ = true;
        }
        
        /** 
            \brief Get the first modifier
            @return The first modifier
        */
        OutputMod modifier1() const {return modifier_1_;}
        /**
            \brief Get the second modifier
            @return The second modifier, or -1 if only 1 modifier
        */
        OutputMod modifier2() const {return modifier_2_;}
        /**
            \brief Get the third modifier
            @return The third modifier, or -1 if only 1 or 2 modifiers exist
        */
        OutputMod modifier3() const {return modifier_3_;}
        /**
            \brief Get TTY status
            @return True if output file is a TTY, false otherwise
        */
        bool TTYStatus() const {return is_tty_;}
        /**
            \brief Is modified output supported
            @return True if modified output is supported, false otherwise
        */
        bool shouldColor() const {return should_color_;}
        inline friend std::ostream& operator<<(std::ostream &out, const Modifier &om);
    
    private:
        char* TERM_;
        OutputMod modifier_1_;
        OutputMod modifier_2_;
        OutputMod modifier_3_;
        bool is_tty_{false};
        bool should_color_{false};
        const std::string MOD{"\033["};
        const std::string RESET{"\033[0m"};
    };
    
    /**
        \brief Allows streaming output
    */
    inline std::ostream &operator<<(std::ostream &out, const Modifier &om){
        if(!om.shouldColor())
            return out;
        if(om.modifier3() != -1)
            return out << om.MOD << om.modifier1() << ";" << om.modifier2() << ";" << om.modifier3() << "m";
        if(om.modifier2() != -1)
            return out << om.MOD << om.modifier1() << ";" << om.modifier2() << "m";
        return out << om.MOD << om.modifier1() << "m";
    }
}

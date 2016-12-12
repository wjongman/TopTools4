#ifndef InfoFormatterH
#define InfoFormatterH

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "PixelInfo.h"

///////////////////////////////////////////////////////////////////////////////
//
// Mask syntax:
//
// Everything not enclosed in square brackets is interpreted
// as literal text.
//
//  possible placeholders:
//
//  [x]
//  [y] - decimal value of position
//
//  [r]
//  [g]
//  [b] - decimal value of rgb color
//
//  [R]
//  [G]
//  [B] - hexadecimal value of rgb color, 2 positions
//
//  [h]
//  [s]
//  [v] - decimal value of hsv color
//
//  [[] - literal [
//  []] - literal ]
//
//  [n] - line break
//
//  [w] - lower-case webcolor (without #)
//  [W] - upper-case webcolor, same as [R][G][B]
//
//  Other placeholders are ignored
//
//  examples:
//
//  mask:    ([x], [y]) -> [[]#[R][G][B][]] ([r], [g], [b])
//  output:  (640, 480) -> [#ABCDEF] (171, 205, 239)
//
//
// example usage of this class:
//
// MaskFormatter mf("#[R][G][B]");
// std::string formatted = mf.GetFormattedString(x, y, r, g, b);
//

///////////////////////////////////////////////////////////////////////////////
class InfoFormatter
{
    std::vector<std::string> m_argstack;
    std::vector<std::string> m_literalstack;

public:
    //-------------------------------------------------------------------------
    InfoFormatter(char* mask)
    {
        Tokenize(std::string(mask));
    }

    //-------------------------------------------------------------------------
    InfoFormatter(std::string& mask)
    {
        Tokenize(mask);
    }

    //-------------------------------------------------------------------------
    std::string GetFormattedString(TPixelInfo const& pi)
    {
        // Merge literal strings with argument values
        m_newlines = 0;
        std::stringstream ss;
        for (size_t i = 0; i < m_argstack.size(); ++i)
        {
            ss << m_literalstack[i];
            std::string arg = m_argstack[i];
            switch (arg[1])
            {
            case '[':
                ss << "[";
                break;
            case ']':
                ss << "]";
                break;

            case 'x':
                ss << pi.x;
                break;
            case 'y':
                ss << pi.y;
                break;

            case 'h':
                ss << pi.h;
                break;
            case 's':
                ss << pi.s;
                break;
            case 'v':
                ss << pi.v;
                break;

            case 'r':
                ss << pi.r;
                break;
            case 'g':
                ss << pi.g;
                break;
            case 'b':
                ss << pi.b;
                break;

            case 'R':
                ss << std::setfill('0') << std::setw(2) << std::hex;
                ss << std::uppercase;
                ss << pi.r;
                ss << std::nouppercase;
                ss << std::setfill(' ') << std::setw(0) << std::dec;
                break;
            case 'G':
                ss << std::setfill('0') << std::setw(2) << std::hex;
                ss << std::uppercase;
                ss << pi.g;
                ss << std::nouppercase;
                ss << std::setfill(' ') << std::setw(0) << std::dec;
                break;
            case 'B':
                ss << std::setfill('0') << std::setw(2) << std::hex;
                ss << std::uppercase;
                ss << pi.b;
                ss << std::nouppercase;
                ss << std::setfill(' ') << std::setw(0) << std::dec;
                break;

            case 'w':
                ss << std::setfill('0') << std::setw(2) << std::hex;
                ss << pi.r << pi.g << pi.b;
                ss << std::setfill(' ') << std::setw(0) << std::dec;
                break;
            case 'W':
                ss << std::setfill('0') << std::setw(2) << std::hex;
                ss << std::uppercase;
                ss << pi.r << pi.g << pi.b;
                ss << std::nouppercase;
                ss << std::setfill(' ') << std::setw(0) << std::dec;
                break;
            case 'n':
                ss << "\r\n";
                m_newlines++;
                break;

            }
        }
        // Add remaining literal text
        ss << m_literalstack[m_literalstack.size() - 1];

        return ss.str();
    }

    //-------------------------------------------------------------------------
    bool hasNewlines()
    {
        return m_newlines > 0;
    }

private:
    int m_newlines;

    //-------------------------------------------------------------------------
    bool Tokenize(std::string& mask)
    {
        // Split mask into literal and argument parts
        // The number of literal parts is always the number of arguments + 1
        m_argstack.clear();
        m_literalstack.clear();

        int start = 0;
        while (1)
        {
            int open = mask.find('[', start);
            if (open == -1)
            {
                // No more placeholders in mask, save trailing literals and bail out
                std::string literal = mask.substr(start, mask.size() - start);
                m_literalstack.push_back(literal);
                return true;
            }
            int close = mask.find(']', open);
            if (close - open == 1 && mask[close + 1] == ']')
            {
                // Special case: "[]]" is a literal ']'
                close++;
            }
            else if (close == -1 || close - open > 2)
            {
                // Malformed tag
                m_literalstack.push_back("");
                return false;
            }
            m_literalstack.push_back(mask.substr(start, open - start));
            m_argstack.push_back(mask.substr(open, close - open + 1));

            start = close + 1;
        }
    }

};

#endif

#if 0
std::vector<std::string> testmasks =
{
    "([x], [y]) -> [[]#[R][G][B][]] ([r], [g], [b]) [r]#[g]",
    "([x], [y]) -> #[R][G][B] [[][b], [g], [r][]]",
    "[",
    "[[]",
    "[]]",
    "([x], [y]) -> ([r], [g], [b])",
};

int main(int argc, char* argv[])
{
    int x = 640, y = 480, r = 171, g = 205, b = 239;

    std::string mask = (argc > 1) ? argv[1] : testmasks[0];

    MaskFormatter mf(mask);
    printf("%s", mf.GetFormattedString(x, y, r, g, b).c_str());

    return 0;
}
#endif

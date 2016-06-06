#ifndef MaskFormatterH
#define MaskFormatterH

#include <vector>
#include <string>
#include <stdio.h>

#define MAXARGS 10
#define MAXMASK 512
#define MAXBUF 1024

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
//  [b] - decimal value of color
//
//  [R]
//  [G]
//  [B] - hexadecimal value of color, 2 positions
//
//  [[] - literal [
//  []] - literal ]
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
class MaskFormatter
{
    std::vector<std::string> argstack;
    std::vector<std::string> literalstack;

public:
    //-------------------------------------------------------------------------
    MaskFormatter(char* mask)
    {
        Tokenize(std::string(mask));
    }

    //-------------------------------------------------------------------------
    MaskFormatter(std::string& mask)
    {
        Tokenize(mask);
    }

    //-------------------------------------------------------------------------
    std::string GetFormattedString(int x, int y, int r, int g, int b)
    {
        std::vector<int> args;
        std::string formatstring;
        for (int i = 0; i < argstack.size(); ++i)
        {
            formatstring += literalstack[i];
            std::string arg = argstack[i];
            switch (arg[1])
            {
            case '[':
                formatstring += "[";
                break;
            case ']':
                formatstring += "]";
                break;
            case 'x':
                args.push_back(x);
                formatstring += "%d";
                break;
            case 'y':
                args.push_back(y);
                formatstring += "%d";
                break;

            case 'r':
                args.push_back(r);
                formatstring += "%d";
                break;
            case 'g':
                args.push_back(g);
                formatstring += "%d";
                break;
            case 'b':
                args.push_back(b);
                formatstring += "%d";
                break;

            case 'R':
                args.push_back(r);
                formatstring += "%02X";
                break;
            case 'G':
                args.push_back(g);
                formatstring += "%02X";
                break;
            case 'B':
                args.push_back(b);
                formatstring += "%02X";
                break;

            case 'w':
                args.push_back(r);
                args.push_back(g);
                args.push_back(b);
                formatstring += "%02x%02x%02x";
                break;
            case 'W':
                args.push_back(r);
                args.push_back(g);
                args.push_back(b);
                formatstring += "%02X%02X%02X";
                break;
            }
        }
        formatstring += literalstack[literalstack.size() - 1];

        // At this point we have our format string and a vector
        // of values to be inserted.
        // We leverage the fact that printf will ignore unused
        // arguments, we just give it MAXARGS arguments.
        // For now we limit the number of arguments to 10 and
        // set maximum length of resulting string to 1024 chars.

        // TODO: make this more flexible and less prone to buffer overflows

        // Make sure our vector has at least MAXARGS elements
        for (int i = args.size(); i < MAXARGS; ++i)
        {
            args.push_back(0);
        }
        char buffer[MAXBUF];
        sprintf(buffer, formatstring.c_str(), args[0], args[1],
            args[2], args[3], args[4], args[5], args[6], args[7],
            args[8], args[9]); // extend to match MAXARGS

        return std::string(buffer);
    }

private:
    //-------------------------------------------------------------------------
    // Split mask into literal and argument parts
    // The number of literal parts is always the number of arguments + 1
    void Tokenize(std::string& mask)
    {
        argstack.clear();
        literalstack.clear();

        // Truncate mask if too long
        if (mask.size() > MAXMASK)
        {
            mask = mask.substr(0, MAXMASK);
        }

        int start = 0;
        while (1)
        {
            int open = mask.find('[', start);
            if (open == -1)
            {
                // No more placeholders in mask, save trailing literals and bail out
                std::string literal = mask.substr(start, mask.size() - start);
                literalstack.push_back(literal);
                break;
            }
            int close = mask.find(']', open);
            if (close - open == 1 && mask[close + 1] == ']')
            {
                // Special case: "[]]" (a literal ']')
                close++;
            }
            else if (close == -1 || close - open > 2)
            {
                // Malformed tag, save mask as literal and bail out
                std::string msg;
                msg += mask.substr(start, open - start);
                msg += "^^ malformed tag: ";
                msg += mask.substr(open, close - open + 1);
                literalstack.push_back(msg);
                break;
            }
            literalstack.push_back(mask.substr(start, open - start));
            argstack.push_back(mask.substr(open, close - open + 1));

            start = close + 1;
            if (argstack.size() > MAXARGS)
            {
                // Maximum number of placeholders reached
                literalstack.push_back("");
                break;
            }
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

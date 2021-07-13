#include <limits>
#include <SDL2/SDL_messagebox.h>

#include "def_parser.h"
#include "utils.h"
#include "log.h"
#include "fmtlib/printf.h"

namespace ScriptParser
{

bool Parser::ParseNextLine(std::string *p1, std::string *p2)
{
    p1->clear();
    p2->clear();

    std::string buf;
    std::string word;

    Stok stok("= \t");

    bool p1read = false;
    bool loop = true;

    while ( loop )
    {
    	if ( !ReadLine(&buf) )
            return p1read;

        size_t line_start = buf.find(";#!");
        if (line_start != std::string::npos)
            buf = buf.substr(line_start + 3);
        
        size_t line_end = buf.find_first_of(";\n\r");
        if (line_end != std::string::npos)
            buf.erase(line_end);

        stok = buf;
        if (!p1read && stok.GetNext(&word) )
        {
            p1read = true;
            (*p1) = word;
        }

        if (p1read)
        {
            loop = false;
            if (stok.GetNext(&word))
            {
                (*p2) += word;
                if (p2->back() == '\\')
                {
                    p2->back() = '\n';
                    loop = true;
                }
            }
        }
    }

    return true;
}


int Parser::ParseRoutine(const std::string &filename, HandlersList &callbacks, int flags)
{
    std::string p1;
    std::string p2;

    int selectedHandler = -1;

    while ( ParseNextLine(&p1, &p2) )
    {
        if ( selectedHandler != -1 )
        {
            int res = callbacks[ selectedHandler ]->Handle(*this, p1, p2);

            if ( res == RESULT_SCOPE_END )
            {
                selectedHandler = -1;
            }
            else if ( res )
            {
                if ( res == RESULT_UNKNOWN )
                    ypa_log_out("PARSE ERROR: script %s line #%d unknown keyword %s.\n", filename.c_str(), _line, p1.c_str());
                else if ( res == RESULT_BAD_DATA )
                    ypa_log_out("PARSE ERROR: script %s line #%d data expected or bogus data.\n", filename.c_str(), _line);

                return res;
            }
        }
        else if ( !(flags & FLAG_NO_INCLUDE) && !StriCmp(p1, "include") )
        {
            Parser include(p2);
            if ( !include.ParseFile(p2, callbacks, flags) )
            {
                ypa_log_out("ERROR: script %s line #%d include %s failed!\n", filename.c_str(), _line, p1.c_str());
                return RESULT_BAD_DATA;
            }
        }
        else
        {
            for (uint32_t i = 0; i < callbacks.size(); i++)
            {
            	if ( callbacks[i]->IsScope(*this, p1, p2) )
                {
                    selectedHandler = i;
                    break;
                }
            }

            if ( selectedHandler == -1 && (flags & FLAG_NO_SCOPE_SKIP) )
            {
                ypa_log_out("PARSE ERROR: script %s line #%d unknown keyword %s.\n", filename.c_str(), _line, p1.c_str());
                return RESULT_UNKNOWN;
            }
        }
    }

    if ( selectedHandler != -1 )
    {
        ypa_log_out("PARSE ERROR: script %s unexpected EOF!\n", filename.c_str());
        return RESULT_UNEXP_EOF;
    }

    return RESULT_EOF;
}


bool Parser::ReadLine(std::string *out)
{
    bool ok = false;
    switch(_mode)
    {
        case MODE_FILE:
            ok = _file->ReadLine(&_lastLine);
            break;

        case MODE_SLIST:
            if (_strlistIt != _strlistEnd)
            {
                _lastLine = *_strlistIt;
                _strlistIt++;
                ok = true;
            }
            break;

        case MODE_NO:
        default:
            break;
    }
    *out = _lastLine;
    _line++;
    return ok;
}

bool Parser::ParseFile(const std::string &filename, HandlersList &callbacks, int flags)
{
    Parser parser(filename);
    parser._line = 0;
    parser._file = uaOpenFileAlloc(filename, "r");
    parser._mode = MODE_FILE;

    if (!parser._file)
        return false;

    int res = RESULT_SCOPE_END;
    while (res == RESULT_SCOPE_END)
        res = parser.ParseRoutine(filename, callbacks, flags);

    delete parser._file;
    parser._file = NULL;

    if ( res == RESULT_UNKNOWN || res == RESULT_BAD_DATA || res == RESULT_UNEXP_EOF )
        return false;

    return true;
}

bool Parser::ParseStringList(const Engine::StringList &slist, HandlersList &callbacks, int flags)
{
    Parser parser("StringList");
    parser._line = 0;
    parser._strlistIt = slist.cbegin();
    parser._strlistEnd = slist.cend();
    parser._mode = MODE_SLIST;

    int res = RESULT_SCOPE_END;
    while (res == RESULT_SCOPE_END)
        res = parser.ParseRoutine("StringList", callbacks, flags);

    parser._file = NULL;

    if ( res == RESULT_UNKNOWN || res == RESULT_BAD_DATA || res == RESULT_UNEXP_EOF )
        return false;

    return true;
}

long Parser::stol(const std::string& str, std::size_t* __idx, int __base)
{
    try {
        return std::stol(str, __idx, __base);
    }
    catch(const std::out_of_range &err) {
        std::string errmsg = fmt::sprintf("Idiotic modify detected in %s, line %d:\n%s\nvalue %s overflow integer: %d - %d:\n",
                     _Name.c_str(), _line, _lastLine.c_str(), str.c_str(), std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
        
        ypa_log_out(errmsg.c_str());
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error parsing file", errmsg.c_str(), NULL);
                
        if (str[0] == '-')
            return std::numeric_limits<int32_t>::min();
        return std::numeric_limits<int32_t>::max();
    }
    catch(...) {
        std::string errmsg = fmt::sprintf("Idiotic modify detected in %s, line %d:\n%s\ninvalid long value: %s\n",
                     _Name.c_str(), _line, _lastLine.c_str(), str.c_str());
        
        ypa_log_out(errmsg.c_str());
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error parsing file", errmsg.c_str(), NULL);
        
        return 0;
    }
}

int Parser::stoi(const std::string& str, std::size_t* __idx, int __base)
{
    try {
        return std::stoi(str, __idx, __base);
    }
    catch(const std::out_of_range &err) {
        std::string errmsg = fmt::sprintf("Idiotic modify detected in %s, line %d:\n%s\nvalue %s overflow integer: %d - %d:\n",
                     _Name.c_str(), _line, _lastLine.c_str(), str.c_str(), std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
        
        ypa_log_out(errmsg.c_str());
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error parsing file", errmsg.c_str(), NULL);
                
        if (str[0] == '-')
            return std::numeric_limits<int32_t>::min();
        return std::numeric_limits<int32_t>::max();
    }
    catch(...) {
        std::string errmsg = fmt::sprintf("Idiotic modify detected in %s, line %d:\n%s\ninvalid integer value: %s\n",
                     _Name.c_str(), _line, _lastLine.c_str(), str.c_str());
        
        ypa_log_out(errmsg.c_str());
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error parsing file", errmsg.c_str(), NULL);
        
        return 0;
    }
}

float Parser::stof(const std::string& str, std::size_t* __idx)
{
    try {
        return std::stof(str, __idx);
    }
    catch(...) {
        std::string errmsg = fmt::sprintf("Idiotic modify detected in %s, line %d:\n%s\ninvalid float value: %s\n",
                     _Name.c_str(), _line, _lastLine.c_str(), str.c_str());
        
        ypa_log_out(errmsg.c_str());
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error parsing file", errmsg.c_str(), NULL);
        
        return 0.0;
    }
}

double Parser::stod(const std::string& str, std::size_t* __idx)
{
    try {
        return std::stod(str, __idx);
    }
    catch(...) {
        std::string errmsg = fmt::sprintf("Idiotic modify detected in %s, line %d:\n%s\ninvalid double value: %s\n",
                     _Name.c_str(), _line, _lastLine.c_str(), str.c_str());
        
        ypa_log_out(errmsg.c_str());
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error parsing file", errmsg.c_str(), NULL);
        
        return 0.0;
    }
}

}



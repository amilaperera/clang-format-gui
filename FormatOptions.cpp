#include "FormatOptions.h"

FormatOptions::FormatOptions(QObject *parent) :
    QObject(parent)
{
    clangFormatCommand = new ClangFormatCommand(this);
    Clear();
}

void FormatOptions::Clear()
{
    style = LLVM;
}

void FormatOptions::SetStyle(Style s)
{
    style = s;

    switch (s) {
    case LLVM:
        clangFormatCommand->SetStyle("LLVM");
        break;
    case Google:
        clangFormatCommand->SetStyle("Google");
        break;
    case Chromium:
        clangFormatCommand->SetStyle("Chromium");
        break;
    case Mozilla:
        clangFormatCommand->SetStyle("Mozilla");
        break;
    case Webkit:
        clangFormatCommand->SetStyle("Webkit");
        break;
    default:
        clangFormatCommand->SetStyle("LLVM");
    }
}

ClangFormatCommand *FormatOptions::GetClangFormatCommand() const
{
    return clangFormatCommand;
}

QString FormatOptions::GetClangFormatCommandStr() const
{
    return clangFormatCommand->GetClangFormatCommand();
}

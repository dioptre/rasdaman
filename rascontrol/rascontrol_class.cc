/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/
/**
 * SOURCE: rascontrol_class.hh
 *
 * MODULE: rascontrol
 * CLASS:  EditLine, RascontrolConfig
 *
 * PURPOSE:
 *  RasControl helper classes
 *
 * COMMENTS:
 *
*/

#include "debug-clt.hh"

#include "globals.hh"

#include "rascontrol.hh"

#include <readline/readline.h>
#include <readline/history.h>

#define READLINELIB

/*
// if we have readline library on this machine ...
#ifdef READLINELIB

extern "C"
{
char *  rl_gets (char *prompt);
char *  readline(char *prompt);
void    add_history (char * line_read);
int     rl_insert();
int     rl_bind_key (int KEY, int (*FUNCTION)());
void     using_history ();
}

#endif
*/

extern UserLogin userLogin;

//int readWholeMessage(int socket,char *destBuffer,int buffSize);
//int writeWholeMessage(int socket,char *destBuffer,int buffSize);

EditLine::EditLine()
{
    line[0]= EOS_CHAR;

#ifdef READLINELIB
    using_history ();
    rl_bind_key ('\t', rl_insert);
#endif
}
EditLine::~EditLine()
{
}

const char *EditLine::interactiveCommand(const char *prompt)
{
#ifdef READLINELIB
    char *rasp=rl_gets (prompt);

#else
    std::cout<<prompt<<std::flush;
    char *rasp=fgets(line,MAXMSG-1,stdin);

#endif

    if(rasp==0) return 0;
    strcpy(line, rasp);
    return line;

}

const char *EditLine::fromStdinCommand(const char* prompt)
{
    if(prompt) std::cout<<prompt<<std::flush;
    char *rasp=fgets(line,MAXMSG-1,stdin);

    if(rasp==0)
    {
        return 0;
    }

    int i;
    for(i=0; line[i]; i++)
    {
        if(line[i]=='\r' || line[i]=='\n')
        {
            line[i]=0;
            break;
        }
    }

    for(i=0; line[i]; i++)
    {
        if(line[i]==' ' || line[i]=='\t') continue;
        break;
    }
    return line +i;
}

char * EditLine::rl_gets(const char *prompt)
{
#ifdef READLINELIB
    static char *line_read = (char *)NULL;

    /* If the buffer has already been allocated, return the memory
    to the free pool. */

    if (line_read)
    {
        free (line_read);
        line_read = (char *)NULL;
    }

    /* Get a line from the user. */
    line_read = readline ((char*)prompt);

    /* If the line has any text in it, save it on the history. */
    if (line_read && *line_read)
        add_history (line_read);

    return (line_read);
#else
    return " ";
#endif

}

//################ Config ##################################
RascontrolConfig::RascontrolConfig() :
    cmlInter        (CommandLineParser::getInstance()),
    cmlHost         (cmlInter.addStringParameter(CommandLineParser::noShortName,"host", "<name> name of host where master rasmgr runs", DEFAULT_HOSTNAME)),
    cmlPort         (cmlInter.addLongParameter(CommandLineParser::noShortName,"port", "<nnnn> rasmgr port",DEFAULT_PORT)),
    cmlLogin        (cmlInter.addFlagParameter('l',"login", "just login prompt, used to set the environment variable RASLOGIN")),
    cmlTestLogin    (cmlInter.addFlagParameter('t',"testlogin", "test if environment variable RASLOGIN is OK to login")),
    cmlInteractive  (cmlInter.addFlagParameter('e',"interactive", "interactive mode, login from environment variable RASLOGIN")),
    cmlQuiet        (cmlInter.addFlagParameter('q',"quiet", "quiet, don't print header (default on for -login and -testlogin)")),
#ifdef NO_OFFICIAL_RELEASE
    cmlHist         (cmlInter.addStringParameter(CommandLineParser::noShortName, "hist", "<file-name> used to store your commands in file, as help for batch file.")),
#endif
    cmlPrompt       (cmlInter.addStringParameter(CommandLineParser::noShortName, "prompt", "<nnn> change rascontrol prompt as following:\n\t\t 0 - prompt '>'\n\t\t 1 - prompt 'rasc>'\n\t\t 2 - prompt 'user:host>'","2")),
    cmlExecute      (cmlInter.addFlagParameter('x',"execute", "batch mode, login from environment variable RASLOGIN\n   <rasmgr-cmd>\ta rasmgr command (only in batch mode)\n\t\tif no command if provided, command is read from stdin\n\t\t(used for batch mode with '<inputfile')")),
    cmlHelp         (cmlInter.addFlagParameter('h',"help", "this help"))
{
    workModus = WKMINTERACTIV;
    loginModus= LGIINTERACTIV;

// done by default value of command line parameters
//    strcpy(rasmgrHost, cmlHost.getValueAsString());
//    rasmgrPort = cmlPort.getValueAsLong();

    reqHist = false;
    strcpy(histFileName,"rascontrol.hist");

    promptType=PROMPTFULL;
    prompt[0]= EOS_CHAR;
    command[0]= EOS_CHAR;
    quiet =false;
}

bool RascontrolConfig::interpretArguments(int argc, char **argv)
{
    //FIXME workarround for batch mode commands given to rascontrol
    //in current version it is generated an error if a parameter has many values:
    //e.g. -a c d f where c,d,f are values(not parameters)
    int lastArg = argc;
    string shortX = string("") + CommandLineParser::ShortSign + cmlExecute.getShortName();
    string longX  = string("") + CommandLineParser::LongSign  + cmlExecute.getLongName();

    for(lastArg = 1; lastArg<argc; lastArg++)
    {
        if( (strcmp(argv[lastArg], shortX.c_str())==0) ||
                (strcmp(argv[lastArg], longX.c_str())==0) )
        {
            lastArg++;
            break;
        }
    }

    //std::cout<<"argc="<<argc<<"  lastarg="<<lastArg<<std::endl;

    try
    {
        if(lastArg != argc)
            cmlInter.processCommandLine(lastArg, argv);
        else
            cmlInter.processCommandLine(argc, argv);
    }
    catch(CmlException& err)
    {
        std::cout << "Command Line Parsing Error:" << std::endl << err.what() << std::endl;
        return false;
    }



    if( cmlHelp.isPresent() )
    {
        //we stop processing
        return true;
    }

    try
    {
        rasmgrPort = cmlPort.getValueAsLong();
    }
    catch(CmlException& err)
    {
        std::cout << "Command Line Parsing Error:" << std::endl << err.what() << std::endl;
        return false;
    }

    strcpy(rasmgrHost, cmlHost.getValueAsString());

    if( cmlLogin.isPresent() )
    {
        workModus = WKMLOGIN;
        quiet = true;
    }

    if( cmlQuiet.isPresent() )
    {
        quiet = true;
    }

    if( cmlTestLogin.isPresent() )
    {
        if(workModus==WKMINTERACTIV)
        {
            workModus = WKMTESTLOGIN;
            loginModus = LGIENVIRONM;
        }
        else return paramError();
        quiet = true;
    }

    if( cmlInteractive.isPresent() )
    {
        if(workModus==WKMINTERACTIV) loginModus = LGIENVIRONM;
        else return paramError();
    }

    if( cmlExecute.isPresent() )
    {
        if(workModus==WKMINTERACTIV)
        {
            loginModus = LGIENVIRONM;
            workModus  = WKMBATCH;

            for(int i=lastArg; i<argc; i++)
            {
                strcat(command,argv[i]);
                strcat(command," ");
            }
        }
        else return paramError();
    }

#ifdef NO_OFFICIAL_RELEASE
    if( cmlHist.isPresent() )
    {
        reqHist = true;
        strcpy(histFileName, cmlHist.getValueAsString());
    }
#endif

    try
    {
        promptType = cmlPrompt.getValueAsLong();
    }
    catch(CmlException& err)
    {
        std::cout << "Command Line Parsing Error:" << std::endl << err.what() << std::endl;
        return false;
    }

    if(promptType<PROMPTSING || promptType>PROMPTFULL) promptType=PROMPTFULL;

    return true;
}

bool RascontrolConfig::paramError()
{
    std::cout<<"Invalid parameter combination in command line!"<<std::endl;
    return false;
}

void RascontrolConfig::printHelp()
{
    std::cout <<"Usage: " << std::endl << "\trascontrol\t["
              << CommandLineParser::LongSign << cmlHelp.getLongName()  << "]["
              << CommandLineParser::LongSign << cmlHost.getLongName()  << "<mainhost>]["
              << CommandLineParser::LongSign << cmlPort.getLongName()  << " <nn>]["
#ifdef NO_OFFICIAL_RELEASE
              << CommandLineParser::LongSign << cmlHist.getLongName()  << " <file>]["
#endif
              << CommandLineParser::LongSign << cmlPrompt.getLongName()  << " <n>]["
              << CommandLineParser::LongSign << cmlQuiet.getLongName()  << "]\n\t\t\t["
              << CommandLineParser::LongSign << cmlLogin.getLongName() << "|"
              << CommandLineParser::LongSign << cmlTestLogin.getLongName() << "|"
              << CommandLineParser::LongSign << cmlInteractive.getLongName() << "|"
              << CommandLineParser::LongSign << cmlExecute.getLongName() << " <rasmgr-command>]"
              << std::endl;
    std::cout << "Option description:" << std::endl;
    cmlInter.printHelp();
    std::cout<<std::endl;
}

int RascontrolConfig::getWorkModus()
{
    return workModus;
}
int RascontrolConfig::getLoginModus()
{
    return loginModus;
}

const char* RascontrolConfig::getRasMgrHost()
{
    return rasmgrHost;
}
int RascontrolConfig::getRasMgrPort()
{
    return rasmgrPort;
}
bool RascontrolConfig::histRequest()
{
    return reqHist;
}
bool RascontrolConfig::beQuiet()
{
    return quiet;
}

bool RascontrolConfig::showHelp()
{
    return cmlHelp.isPresent();
}

const char* RascontrolConfig::getHistFileName()
{
    return histFileName;
}
const char* RascontrolConfig::getPrompt()
{
    if(prompt[0]!= EOS_CHAR) return prompt;

    switch(promptType)
    {
    case PROMPTSING :
        strcpy(prompt,"> ");
        break;
    case PROMPTRASC :
        strcpy(prompt,"rasc> ");
        break;
    case PROMPTFULL :
        sprintf(prompt,"%s:%s> ",userLogin.getUserName(),rasmgrHost);
        break;
    }
    return prompt;
}
const char* RascontrolConfig::getCommand()
{
    return command;
}

void RascontrolConfig::printDebugInfo()
{
    std::cout<<"Working modus:.";
    switch(workModus)
    {
    case WKMUNKNOWN     :
        std::cout<<"unknown";
        break;
    case WKMINTERACTIV  :
        std::cout<<"interactiv";
        break;
    case WKMBATCH       :
        std::cout<<"batch";
        break;
    case WKMLOGIN       :
        std::cout<<"login";
        break;
    }

    std::cout<<std::endl<<"Login modus:...";
    switch(loginModus)
    {
    case LGIUNKNOWN     :
        std::cout<<"unknown";
        break;
    case LGIINTERACTIV  :
        std::cout<<"interactiv";
        break;
    case LGIENVIRONM    :
        std::cout<<"environm";
        break;
    }

    std::cout<<std::endl<<"Rasmgr:........"<<rasmgrHost<<":"<<rasmgrPort;

    std::cout<<std::endl<<"History:......."<<(reqHist ? histFileName : "not requested");

    std::cout<<std::endl<<"Prompt:........"<<getPrompt();

    std::cout<<std::endl<<"Command:......."<<command;

    std::cout<<std::endl;
}


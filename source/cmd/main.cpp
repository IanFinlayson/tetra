/* main.cpp
 * this file contains the main entry point of the command line Tetra
 * interpreter */

#include <argp.h>
#include <cstdlib>
#include <iostream>

#include "console.h"
#include "tetra.h"

/* the info for the command line parameters */
const char* argp_program_version = "tetra 0.1";
const char* argp_program_bug_address = "<finlayson@umw.edu>";
const char doc[] = "The Tetra Interpreter";
const char args_doc[] = "FILE";

/* the command line options for the compiler */
const struct argp_option options[] = {{"debug", 'd', 0, 0, "Start in debugging mode", 0},
                                      {"threads", 't', "NUM", 0, "Specify NUM threads maximum", 0},
                                      {"echo", 'e', 0, 0, "Echo input back", 0},
                                      {0, 0, 0, 0, 0, 0}};

/* used by main to communicate with parse_opt */
struct arguments {
    char* inputFileName;
    int debug;
    int threads;
    int echo;
};

/* the function which parses command line options */
error_t parse_opt(int key, char* arg, struct argp_state* state) {
    /* get the input argument from argp_parse */
    struct arguments* arguments = (struct arguments*) state->input;

    /* switch on the command line option that was passed in */
    switch (key) {
        case 'd':
            /* the debug flag is set */
            arguments->debug = 1;
            break;

        case 'e':
            /* they want to echo input back */
            arguments->echo = 1;
            break;

        case 't':
            /* the number of threads is set */
            arguments->threads = atoi(arg);
            break;

        /* we got a file name */
        case ARGP_KEY_ARG:
            if (state->arg_num > 1) {
                /* too many arguments */
                fprintf(stderr, "too many input files\n");
                argp_usage(state);
            }

            /* save it as an argument */
            arguments->inputFileName = arg;
            break;

        /* we hit the end of the arguments */
        case ARGP_KEY_END:
            if (state->arg_num < 1) {
                /* not enough arguments */
                fprintf(stderr, "no input file\n");
                argp_usage(state);
            }
            break;

        /* some other kind of thing happended */
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* the parameters to the argp library containing our program details */
struct argp info = {options, parse_opt, args_doc, doc, 0, 0, 0};

/* the main function */
int main(int argc, char** argv) {
    /* set up the arguments structure */
    struct arguments args;

    /* the default values */
    args.debug = 0;
    args.threads = 8;
    args.echo = 0;
    args.inputFileName = NULL;

    /* parse command line */
    argp_parse(&info, argc, argv, 0, 0, &args);

    /* set input file to what was passed in */
    FILE* input = fopen(args.inputFileName, "r");
    if (!input) {
        fprintf(stderr, "Error, can not open %s for reading!\n", args.inputFileName);
    }

    CommandConsole mainConsole = CommandConsole();

    Environment::initialize();
    Environment::setConsole(&mainConsole);

    /* turn on echoing of input */
    if (args.echo) {
        Environment::setEcho();
    }

    Node* tree;

    /* parse file, and check for initial errors. Print out and exit if an error
     */
    try {
        /* file is last parameter */
        tree = parseFile(String(args.inputFileName));
    } catch (Error e) {
        std::cout << e;
        exit(EXIT_FAILURE);
    }
    int ret = 0;
    try {
        ret = interpret(tree, args.debug, args.threads);
    } catch (SystemError e) {
        std::cout << "System Error: " << e << "Node giving error:\n";
        dumpTreeStdout(e.getNode(), 0);
        exit(EXIT_FAILURE);
    } catch (RuntimeError e) {
        std::cout << "Runtime Error: " << e << std::endl;
        exit(EXIT_FAILURE);
    } catch (Error e) {
        std::cout << "Error: " << e << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << std::endl;

    delete tree;
    return ret;
}


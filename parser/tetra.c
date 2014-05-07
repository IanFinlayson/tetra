#include <list>
#include <iostream>
#include <unistd.h>
#include "tetra_hdr.h"
#include "symbol_table.h"
#include "parsetree.h"
#include "node.h"
#include "parser.tab.h"

void patch(void);
void parse(FILE *);

TTR_Node *parse_tree;
Symbol_Table *symbol_table;
std::list<TTR_node> call_patch_list;
std::list<TTR_node> var_patch_list;

int main(int argc, char **argv)
{
    FILE *fp;
    int opt, verbose = 0;
    char *file;

    file = NULL;

    while ((opt = getopt(argc, argv, "f:v")) != -1) {
        switch (opt) {
        case 'f': file = optarg; break;
        case 'v': verbose = 1; break;
        default:
            err_exit("Unexpected case in option handling");
            break;
        }
    }

    if (optind != argc) {
        if (file != NULL)
            err_exit("Multiple input files specified");
        else 
            file = argv[optind];
    }
    if (file == NULL)
        fp = stdin;
    else if ((fp = fopen(file, "r")) == NULL)
        ferr_exit("Opening file", fp);

    parse(fp);
    patch();

    if (verbose)
        print_tree(parse_tree, 0);

    return 0;
}

void parse(FILE *fp)
{
    init_symbol_table(&symbol_table);
    symbol_table_enter_next_scope(symbol_table);
    init_dll(&call_patch_list);
    init_dll(&var_patch_list);

    yyrestart(fp);
    yyparse();

}

void patch(void)
{
    TTR_Node *node, *stored;
    int type;

    SET_GLOBAL_SCOPE();
    for (   node = dll_pop_head(call_patch_list);
            node != NULL;
            node = dll_pop_head(call_patch_list)) {
        stored = GET_SYMBOL(N_STR(node));    
        if (stored == NULL) {
            fprintf(stderr, "<Line %d> Function \"%s\" never defined\n",
                    N_LINE(node), N_STR(node));
            err_exit("Fatal error");
        } else {
            N_DTYPE(node) = N_DTYPE(stored);
            if (COMPARE_TYPES(N_CHILD(stored, 0), N_CHILD(node, 0))) {
                fprintf(stderr, 
                        "<Line %d> Incompatible types in call to %s.\n",
                        N_LINE(node), N_STR(node));
                err_exit("Fatal error");
            }
        }
    }

    for (   node = dll_pop_head(var_patch_list);
            node != NULL;
            node = dll_pop_head(var_patch_list)) {
        SET_GLOBAL_SCOPE();
        ENTER_SCOPE(N_SCOPE(node));
        if (N_TYPE(node) == N_ASSIGN) {
            TTR_set_ident_data_type(symbol_table,
                    N_CHILD(node, 0), N_DTYPE(N_CHILD(node, 1)));
        }
        type = TTR_infer_data_type(node);
        if (type == INVALID_T) {
            fprintf(stderr, "<Line %d> Incompatible types.\n",
                    N_LINE(node));
            err_exit("Fatal error");
        } else if (type == UNDEFINED_T) {
            fprintf(stderr, "<Line %d> Unknown symbol.\n", N_LINE(node));
            print_tree(node, 0);
            err_exit("Fatal error");
        }
    }

}

/* fastalen.c
 *
 * Read a file containing FASTA sequence(s) and return their ID and length.
 *
 * Use kseq.h from the klib set of libraries (attractivechaos.github.io/klib)
 */


#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include <unistd.h>
#include <fcntl.h>

#include "kseq.h"
#include "argtable3.h"

#define NAME "fastalen"

KSEQ_INIT(int, read)

unsigned long count_seqs(int fd)
{
    unsigned long nb = 0;
    kseq_t *seq;
    seq = kseq_init(fd);
    while(kseq_read(seq) >= 0) nb++;
    kseq_destroy(seq);
    return nb;
}

void print_seq(int fd, char delimiter[], bool count, size_t *sum)
{
    unsigned long nb = 0;
    kseq_t *seq;
    seq = kseq_init(fd);
    size_t size;
    while(kseq_read(seq) >= 0) {
        size = strlen(seq->seq.s);
        printf("%s%s%lu\n", seq->name.s, delimiter, size);
        *sum += size;
        if(count)
            nb++;
    }

    if(count)
        printf("Sequences%s%lu\n", delimiter, nb);
    kseq_destroy(seq);
}


/* global arg_xxx structs */
struct arg_lit *help, *version, *tsv, *csv, *count, *count_only, *sum;
struct arg_file *files;
struct arg_end *end;

int main(int argc, char *argv[])
{
    /* the global arg_xxx structs are initialised within the argtable */
    void *argtable[] = {
        arg_rem(NULL, "\rMandatory argument:"),
        files      = arg_filen(NULL, NULL, "<in.fasta>", 1, argc+2, "input fasta file(s)"),
        arg_rem(NULL, "\n\rFormatting options:"),
        tsv        = arg_litn("t", "tsv", 0, 1, "output as TSV"),
        csv        = arg_litn("c", "csv", 0, 1, "output as CSV"),
        count      = arg_litn("C", "count", 0, 1, "count the number of sequences"),
        count_only = arg_litn("O", "only-count", 0, 1, "ONLY count the number of sequences"),
        sum        = arg_litn("s", "sum", 0, 1, "sum the size of the sequences"),
        arg_rem(NULL, ""),
        help       = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
        version    = arg_litn(NULL, "version", 0, 1, "display version info and exit"),
        end        = arg_end(20),
    };

    char progname[] = "fastalen";
    char progversion[] = "1.4";
    char kseqversion[] = "19JAN2017 (commit d0b8b55)";
    char argtable3version[] = "3.0.3";

    int nerrors;
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntaxv(stdout, argtable, "\n");
        printf("Read file(s) containing FASTA sequence(s) and return their ID and length.\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        exit(EXIT_SUCCESS);
    }
    /* special case: '--version' takes precedence over error reporting */
    else if (version->count > 0)
    {
        printf("%s %s\nBuilt with\n kseq.h %s\n argtable3 %s\n", progname, progversion, kseqversion, argtable3version);
        exit(EXIT_SUCCESS);
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        exit(EXIT_FAILURE);
    }

    /* Let's go! */
    int fd;
    size_t sum_sizes = 0;
    for(int i=0; i < files->count; i++) {
        if(files->count > 1)
            printf("# File %s\n", files->filename[i]);

        fd = open(files->filename[i], O_RDONLY);
        if(fd <= 0)
        {
            perror(NAME);
            continue;
        }

        if(count_only->count > 0)
            printf("%lu\n", count_seqs(fd));
        else {
            char *delim;
            if(tsv->count > 0)
                delim = "\t";
            else if(csv->count > 0)
                delim = ",";
            else
                delim = ": ";

            print_seq(fd, delim, count->count, &sum_sizes);

            if(sum->count > 0) {
              printf("Sum%s%lu\n", delim, sum_sizes);
            }
        }

        close(fd);
    }

    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    exit(EXIT_SUCCESS);
}

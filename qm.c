#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "qm.h"

#define VALID_VARIABLES "ABCDEFGHIJKLMNOPQRSTUVXYZ"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments provided.\n");
    }

    parseDNFStr(argv[1]);
}

DNF parseDNFStr(const char* inpStr) {
    //remove whitespace
    char inpBfr[strlen(inpStr)];
    int i = 0,  j = 0;
    while (inpStr[i] != '\0') {
        if (inpStr[i] != ' ') {
            inpBfr[j++] = inpStr[i];
        }
        i++;
    }
    inpBfr[j] = '\0';

    DNF dnf;
    dnf.numTerms = strcnt(inpBfr, '+') + 1;

    //split up, by '+'s
    const char* delim = "+";
    char* subStr = strtok(inpBfr, delim);
    i = 0;
    while (subStr != NULL) {
        dnf.terms[i++] = parseDNFTerm(subStr);
        // printf("term %i: '%s'\n", i, subStr);
        subStr = strtok(NULL, delim);
    }

    // printf("%i terms\n", dnf.numTerms);
    printForm(dnf);
    printForm2(dnf);
    return dnf;
}

DNFTerm parseDNFTerm(const char* termStr) {
    DNFTerm term;
    term.numVars = 0;
    char c;
    for (int i = 0; i < INT_MAX; i++) {
        c = termStr[i];
        if (c == '\0') {
            break;
        }
        if (strcnt(VALID_VARIABLES, c) == 1) {
            term.vars[term.numVars] = (Variable){c, termStr[i - 1] == '-'};
            // printf("%i %c\n", term.numVars, term.vars[term.numVars]);
            term.numVars++;
        }
    }
    return term;
}

// returns an integer of how many times a char was found in a char
// array
unsigned int strcnt(const char* str, const char cnt) {
    int count = 0;
    for (int i = 0; i < INT_MAX; i++) {
        char c = str[i];
        if (c == '\0') {
            break;
        }
        if (c == cnt) {
            count++;
        }
    }
    return count;
}

void printForm(const DNF form) {
    for (int i = 0; i < form.numTerms; i++) {
        printf("(");
        DNFTerm term = form.terms[i];

        for (int j = 0; j < term.numVars; j++) {
            if (term.vars[j].not_) {
                printf("¬");
            }
            printf("%c", term.vars[j].var);
            if (term.numVars - 1 != j) {
                printf("∧");
            }
        }

        if (form.numTerms - 1 != i) {
            printf(") ∨ ");
        }
    }
    printf(")\n");
}

void printForm2(const DNF form) {
    for (int i = 0; i < form.numTerms; i++) {
        DNFTerm term = form.terms[i];

        for (int j = 0; j < term.numVars; j++) {
            printf("%c", term.vars[j].var);
            if (term.vars[j].not_) {
                printf("'");
            }
        }

        if (form.numTerms - 1 != i) {
            printf(" + ");
        }
    }
    printf("\n");
}


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "qm.h"

#define VALID_VARIABLES "ABCDEFGHIJKLMNOPQRSTUVXYZabcdefghijklmnopqrstuvwxyz"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments provided.\n");
    }

    DNF startDNF = parseDNFStr(argv[1]);
    printForm(&startDNF);
    printForm2(&startDNF);
    SimplifiedTableItem simplifiedTable[startDNF.numTerms];
    unsigned int simplifiedTableLength = startDNF.numTerms;
    setupMergeTable(simplifiedTable, &startDNF);

    mergeTermsInTurn(simplifiedTable, &startDNF.numTerms);
}

void setupMergeTable(SimplifiedTableItem simplifiedTable[], DNF* dnf) {
    for (int i = 0; i < dnf->numTerms; i++) {
        simplifiedTable[i].term = dnf->terms[i];
        simplifiedTable[i].simplifiedIndexes[0] = i;
        simplifiedTable[i].numIndexes = 1;
    }
}

void mergeTermsInTurn(SimplifiedTableItem simplifiedTable[], unsigned int* length) {
    SimplifiedTableItem nextIteration[*length];
    for (int i = 0; i < *length; i++) {
        for (int j = 0; j < *length; j++) {
            if (i == j) {
                continue;
            }

            if (canMergeTerms(&simplifiedTable[i].term, &simplifiedTable[j].term)) {
                // printDNFTerm(&simplifiedTable[i].term);
                // printf(" + ");
                // printDNFTerm(&simplifiedTable[j].term);
                // printf(" = ");
                DNFTerm mergedTerm = mergeTerms(&simplifiedTable[i].term, &simplifiedTable[j].term);
                // printDNFTerm(&mergedTerm);
                // printf("\n------\n");
            }
        }
    }
}

bool canMergeTerms(const DNFTerm* term1, const DNFTerm* term2) {
    unsigned int sameTerms = 0;
    for (int i = 0; i < term1->numVars; i++) {
        for (int j = 0; j < term2->numVars; j++) {
            if (term1->vars[i].not_ == term2->vars[j].not_ && term1->vars[i].var == term2->vars[j].var) {
                sameTerms++;
            }
        }
    }
    return term1->numVars - sameTerms == 1;
}

DNFTerm mergeTerms(const DNFTerm* term1, const DNFTerm* term2) {
    DNFTerm mergedTerm;
    mergedTerm.numVars = 0;
    for (int i = 0; i < term1->numVars; i++) {
        for (int j = 0; j < term2->numVars; j++) {
            if (term1->vars[i].not_ == term2->vars[j].not_ && term1->vars[i].var == term2->vars[j].var) {
                mergedTerm.vars[mergedTerm.numVars++] = (Variable){term1->vars[i].var, term1->vars[i].not_};
            }
        }
    }
    return mergedTerm;
}

bool termInTable(const SimplifiedTableItem simplifiedTable[], const unsigned int* length, const DNFTerm* term) {
    for (int i = 0; i < *length; i++) {
        if (termsAreEqual(&simplifiedTable[i].term, term)) {
            return true;
        }
    }
    return false;
}

bool termsAreEqual(const DNFTerm* term1, const DNFTerm* term2) {
    if (term1->numVars != term2->numVars) {
        return false;
    }

    //TODO
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

    char notChar;
    int relIndex;
    if (strcnt(inpBfr, "'"[0]) > 0) {
        dnf.notChar = "'"[0];
        relIndex = 1;
    } else if (strcnt(inpBfr, '-') > 0) {
        dnf.notChar = '-';
        relIndex = -1;
    }
    if (strcnt(inpBfr, "'"[0]) > 0 && strcnt(inpBfr, '-') > 0) {
        fprintf(stderr, "Inconsistent notation used for not.\n");
        exit(EXIT_FAILURE);
    }

    //split up, by '+'s
    const char* delim = "+";
    char* subStr = strtok(inpBfr, delim);
    i = 0;
    while (subStr != NULL) {
        dnf.terms[i++] = parseDNFTerm(subStr, dnf.notChar, relIndex);
        // printf("term %i: '%s'\n", i, subStr);
        subStr = strtok(NULL, delim);
    }

    return dnf;
}

DNFTerm parseDNFTerm(const char* termStr, const char notChar, const int relIndex) {
    DNFTerm term;
    term.numVars = 0;
    char c;
    for (int i = 0; i < INT_MAX; i++) {
        c = termStr[i];
        if (c == '\0') {
            break;
        }
        if (strcnt(VALID_VARIABLES, c) == 1) {
            term.vars[term.numVars++] = (Variable){c, termStr[i + relIndex] == notChar};
        }
    }
    return term;
}

unsigned int strcnt(const char* str, const char cnt) {
    // returns an integer of how many times a char was found in a char
    // array
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

void printForm(const DNF* form) {
    for (int i = 0; i < form->numTerms; i++) {
        printf("(");
        DNFTerm term = form->terms[i];
        printDNFTerm(&term);

        if (form->numTerms - 1 != i) {
            printf(") ∨ ");
        }
    }
    printf(")\n");
}

void printDNFTerm(const DNFTerm* term) {
    for (int j = 0; j < term->numVars; j++) {
            if (term->vars[j].not_) {
                printf("¬");
            }
            printf("%c", term->vars[j].var);
            if (term->numVars - 1 != j) {
                printf("∧");
            }
        }
}

void printForm2(const DNF* form) {
    for (int i = 0; i < form->numTerms; i++) {
        DNFTerm term = form->terms[i];

        for (int j = 0; j < term.numVars; j++) {
            if (term.vars[j].not_ && form->notChar == '-') {
                printf("-");
            }
            printf("%c", term.vars[j].var);
            if (term.vars[j].not_ && form->notChar == "'"[0]) {
                printf("'");
            }
        }

        if (form->numTerms - 1 != i) {
            printf(" + ");
        }
    }
    printf("\n");
}

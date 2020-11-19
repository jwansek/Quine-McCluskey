#ifndef QM_C
#define QM_C

#include <string.h>

typedef struct {
    char var;
    bool not_;
} Variable;

typedef struct {
    unsigned int numVars;
    Variable vars[26];
} DNFTerm;

typedef struct {
    unsigned int numTerms;
    DNFTerm terms[100];
} DNF;

unsigned int strcnt(const char* str, const char cnt);
DNF parseDNFStr(const char* inpStr);
DNFTerm parseDNFTerm(const char* termStr);
void printForm(const DNF form);
void printForm2(const DNF form);

#endif
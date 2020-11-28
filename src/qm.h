#ifndef QM_C
#define QM_C

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
    char notChar;
} DNF;

typedef struct {
    DNFTerm term;
    unsigned int simplifiedIndexes[100];
    unsigned int numIndexes;
} SimplifiedTableItem;

unsigned int strcnt(const char* str, const char cnt);
void setupMergeTable(SimplifiedTableItem simplifiedTable[], DNF* dnf);
void mergeTermsInTurn(SimplifiedTableItem simplifiedTable[], unsigned int* length);
bool canMergeTerms(const DNFTerm* term1, const DNFTerm* term2);
DNFTerm mergeTerms(const DNFTerm* term1, const DNFTerm* term2);
bool termInTable(const SimplifiedTableItem simplifiedTable[], const unsigned int* length, const DNFTerm* term);
bool termsAreEqual(const DNFTerm* term1, const DNFTerm* term2);
DNF parseDNFStr(const char* inpStr);
DNFTerm parseDNFTerm(const char* termStr, const char notChar, const int relIndex);
void printDNFTerm(const DNFTerm* term);
void printForm(const DNF* form);
void printForm2(const DNF* form);

#endif
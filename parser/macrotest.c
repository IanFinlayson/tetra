# 1 "parser.tab.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "parser.tab.c"
# 33 "parser.y"

# 1 "/usr/include/stdio.h" 1 3 4
# 39 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 40 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/sys/_null.h" 1 3 4
# 41 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4
# 51 "/usr/include/machine/_types.h" 3 4
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long __int64_t;
typedef unsigned long __uint64_t;




typedef __int32_t __clock_t;
typedef __int64_t __critical_t;
typedef double __double_t;
typedef float __float_t;
typedef __int64_t __intfptr_t;
typedef __int64_t __intmax_t;
typedef __int64_t __intptr_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;
typedef __int64_t __ptrdiff_t;
typedef __int64_t __register_t;
typedef __int64_t __segsz_t;
typedef __uint64_t __size_t;
typedef __int64_t __ssize_t;
typedef __int64_t __time_t;
typedef __uint64_t __uintfptr_t;
typedef __uint64_t __uintmax_t;
typedef __uint64_t __uintptr_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;
typedef __uint64_t __u_register_t;
typedef __uint64_t __vm_offset_t;
typedef __int64_t __vm_ooffset_t;
typedef __uint64_t __vm_paddr_t;
typedef __uint64_t __vm_pindex_t;
typedef __uint64_t __vm_size_t;





typedef __builtin_va_list __va_list;






typedef __va_list __gnuc_va_list;
# 34 "/usr/include/sys/_types.h" 2 3 4




typedef __uint32_t __blksize_t;
typedef __int64_t __blkcnt_t;
typedef __int32_t __clockid_t;
typedef __uint64_t __cap_rights_t;
typedef __uint32_t __fflags_t;
typedef __uint64_t __fsblkcnt_t;
typedef __uint64_t __fsfilcnt_t;
typedef __uint32_t __gid_t;
typedef __int64_t __id_t;
typedef __uint32_t __ino_t;
typedef long __key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int __accmode_t;
typedef int __nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;


typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long __suseconds_t;
typedef struct __timer *__timer_t;
typedef struct __mq *__mqd_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;
typedef int __cpuwhich_t;
typedef int __cpulevel_t;
typedef int __cpusetid_t;
# 88 "/usr/include/sys/_types.h" 3 4
typedef int __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;

typedef __uint32_t __dev_t;

typedef __uint32_t __fixpt_t;





typedef union {
 char __mbstate8[128];
 __int64_t _mbstateL;
} __mbstate_t;
# 42 "/usr/include/stdio.h" 2 3 4

typedef __off_t fpos_t;


typedef __size_t size_t;






typedef __off_t off_t;



typedef __ssize_t ssize_t;





typedef __va_list va_list;
# 77 "/usr/include/stdio.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 110 "/usr/include/stdio.h" 3 4
struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;


 void *_cookie;
 int (*_close)(void *);
 int (*_read)(void *, char *, int);
 fpos_t (*_seek)(void *, fpos_t, int);
 int (*_write)(void *, const char *, int);


 struct __sbuf _ub;
 unsigned char *_up;
 int _ur;


 unsigned char _ubuf[3];
 unsigned char _nbuf[1];


 struct __sbuf _lb;


 int _blksize;
 fpos_t _offset;

 struct pthread_mutex *_fl_mutex;
 struct pthread *_fl_owner;
 int _fl_count;
 int _orientation;
 __mbstate_t _mbstate;
};


typedef struct __sFILE FILE;



extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

# 227 "/usr/include/stdio.h" 3 4







void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * , fpos_t * );
char *fgets(char * , int, FILE * );
FILE *fopen(const char * , const char * );
int fprintf(FILE * , const char * , ...);
int fputc(int, FILE *);
int fputs(const char * , FILE * );
size_t fread(void * , size_t, size_t, FILE * );
FILE *freopen(const char * , const char * , FILE * );
int fscanf(FILE * , const char * , ...);
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * , size_t, size_t, FILE * );
int getc(FILE *);
int getchar(void);
char *gets(char *);
void perror(const char *);
int printf(const char * , ...);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char * , ...);
void setbuf(FILE * , char * );
int setvbuf(FILE * , char * , int, size_t);
int sprintf(char * , const char * , ...);
int sscanf(const char * , const char * , ...);
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE * , const char * ,
     __va_list);
int vprintf(const char * , __va_list);
int vsprintf(char * , const char * ,
     __va_list);


int snprintf(char * , size_t, const char * ,
     ...) __attribute__((__format__ (__printf__, 3, 4)));
int vfscanf(FILE * , const char * , __va_list)
     __attribute__((__format__ (__scanf__, 2, 0)));
int vscanf(const char * , __va_list) __attribute__((__format__ (__scanf__, 1, 0)));
int vsnprintf(char * , size_t, const char * ,
     __va_list) __attribute__((__format__ (__printf__, 3, 0)));
int vsscanf(const char * , const char * , __va_list)
     __attribute__((__format__ (__scanf__, 2, 0)));
# 300 "/usr/include/stdio.h" 3 4
char *ctermid(char *);
FILE *fdopen(int, const char *);
int fileno(FILE *);



int pclose(FILE *);
FILE *popen(const char *, const char *);



int ftrylockfile(FILE *);
void flockfile(FILE *);
void funlockfile(FILE *);





int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);


void clearerr_unlocked(FILE *);
int feof_unlocked(FILE *);
int ferror_unlocked(FILE *);
int fileno_unlocked(FILE *);



int fseeko(FILE *, __off_t, int);
__off_t ftello(FILE *);



int getw(FILE *);
int putw(int, FILE *);



char *tempnam(const char *, const char *);



ssize_t getdelim(char ** , size_t * , int,
     FILE * );
int renameat(int, const char *, int, const char *);
int vdprintf(int, const char * , __va_list);
# 394 "/usr/include/stdio.h" 3 4
int asprintf(char **, const char *, ...) __attribute__((__format__ (__printf__, 2, 3)));
char *ctermid_r(char *);
void fcloseall(void);
char *fgetln(FILE *, size_t *);
const char *fmtcheck(const char *, const char *) __attribute__((__format_arg__ (2)));
int fpurge(FILE *);
void setbuffer(FILE *, char *, int);
int setlinebuf(FILE *);
int vasprintf(char **, const char *, __va_list)
     __attribute__((__format__ (__printf__, 2, 0)));






extern const int sys_nerr;
extern const char *const sys_errlist[];




FILE *funopen(const void *,
     int (*)(void *, char *, int),
     int (*)(void *, const char *, int),
     fpos_t (*)(void *, fpos_t, int),
     int (*)(void *));
# 429 "/usr/include/stdio.h" 3 4
int ftruncate(int, __off_t);



__off_t lseek(int, __off_t, int);



void *mmap(void *, size_t, int, int, int, __off_t);



int truncate(const char *, __off_t);






int __srget(FILE *);
int __swbuf(int, FILE *);







static __inline int __sputc(int _c, FILE *_p) {
 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf(_c, _p));
}
# 477 "/usr/include/stdio.h" 3 4
extern int __isthreaded;
# 520 "/usr/include/stdio.h" 3 4

# 35 "parser.y" 2
# 1 "/usr/include/stdlib.h" 1 3 4
# 42 "/usr/include/stdlib.h" 3 4
typedef __rune_t rune_t;
# 54 "/usr/include/stdlib.h" 3 4
typedef __wchar_t wchar_t;




typedef struct {
 int quot;
 int rem;
} div_t;

typedef struct {
 long quot;
 long rem;
} ldiv_t;










extern int __mb_cur_max;
extern int ___mb_cur_max(void);


__attribute__((__noreturn__)) void abort(void);
int abs(int) __attribute__((__const__));
int atexit(void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);
void *bsearch(const void *, const void *, size_t,
     size_t, int (*)(const void *, const void *));
void *calloc(size_t, size_t) __attribute__((__malloc__));
div_t div(int, int) __attribute__((__const__));
__attribute__((__noreturn__)) void exit(int);
void free(void *);
char *getenv(const char *);
long labs(long) __attribute__((__const__));
ldiv_t ldiv(long, long) __attribute__((__const__));
void *malloc(size_t) __attribute__((__malloc__));
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t * , const char * , size_t);
int mbtowc(wchar_t * , const char * , size_t);
void qsort(void *, size_t, size_t,
     int (*)(const void *, const void *));
int rand(void);
void *realloc(void *, size_t);
void srand(unsigned);
double strtod(const char * , char ** );
float strtof(const char * , char ** );
long strtol(const char * , char ** , int);
long double
  strtold(const char * , char ** );
unsigned long
  strtoul(const char * , char ** , int);
int system(const char *);
int wctomb(char *, wchar_t);
size_t wcstombs(char * , const wchar_t * , size_t);
# 130 "/usr/include/stdlib.h" 3 4
typedef struct {
 long long quot;
 long long rem;
} lldiv_t;


long long
  atoll(const char *);

long long
  llabs(long long) __attribute__((__const__));

lldiv_t lldiv(long long, long long) __attribute__((__const__));

long long
  strtoll(const char * , char ** , int);

unsigned long long
  strtoull(const char * , char ** , int);


__attribute__((__noreturn__)) void _Exit(int);
# 168 "/usr/include/stdlib.h" 3 4
int posix_memalign(void **, size_t, size_t);
int rand_r(unsigned *);
char *realpath(const char * , char * );
int setenv(const char *, const char *, int);
int unsetenv(const char *);



int getsubopt(char **, char *const *, char **);

char *mkdtemp(char *);



int mkstemp(char *);
# 195 "/usr/include/stdlib.h" 3 4
long a64l(const char *);
double drand48(void);

double erand48(unsigned short[3]);


int grantpt(int);
char *initstate(unsigned long , char *, long);
long jrand48(unsigned short[3]);
char *l64a(long);
void lcong48(unsigned short[7]);
long lrand48(void);

char *mktemp(char *);


long mrand48(void);
long nrand48(unsigned short[3]);
int posix_openpt(int);
char *ptsname(int);
int putenv(char *);
long random(void);
unsigned short
 *seed48(unsigned short[3]);

int setkey(const char *);


char *setstate( char *);
void srand48(long);
void srandom(unsigned long);
int unlockpt(int);



extern const char *_malloc_options;
extern void (*_malloc_message)(const char *, const char *, const char *,
     const char *);
# 250 "/usr/include/stdlib.h" 3 4
void abort2(const char *, int, void **) __attribute__((__noreturn__));
__uint32_t
  arc4random(void);
void arc4random_addrandom(unsigned char *, int);
void arc4random_buf(void *, size_t);
void arc4random_stir(void);
__uint32_t
  arc4random_uniform(__uint32_t);
char *getbsize(int *, long *);

char *cgetcap(char *, const char *, int);
int cgetclose(void);
int cgetent(char **, char **, const char *);
int cgetfirst(char **, char **);
int cgetmatch(const char *, const char *);
int cgetnext(char **, char **);
int cgetnum(char *, const char *, long *);
int cgetset(const char *);
int cgetstr(char *, const char *, char **);
int cgetustr(char *, const char *, char **);

int daemon(int, int);
char *devname(__dev_t, __mode_t);
char *devname_r(__dev_t, __mode_t, char *, int);
char *fdevname(int);
char *fdevname_r(int, char *, int);
int getloadavg(double [], int);
const char *
  getprogname(void);

int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));
int l64a_r(long, char *, int);
int mergesort(void *, size_t, size_t, int (*)(const void *, const void *));
void qsort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *));
int radixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void *reallocf(void *, size_t);
int rpmatch(const char *);
void setprogname(const char *);
int sradixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void sranddev(void);
void srandomdev(void);
long long
 strtonum(const char *, long long, long long, const char **);


__int64_t
  strtoq(const char *, char **, int);
__uint64_t
  strtouq(const char *, char **, int);

extern char *suboptarg;


# 36 "parser.y" 2
# 1 "/usr/include/string.h" 1 3 4
# 45 "/usr/include/string.h" 3 4
# 1 "/usr/include/strings.h" 1 3 4
# 40 "/usr/include/strings.h" 3 4


int bcmp(const void *, const void *, size_t) __attribute__((__pure__));
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);


int ffs(int) __attribute__((__const__));


int ffsl(long) __attribute__((__const__));
int ffsll(long long) __attribute__((__const__));
int fls(int) __attribute__((__const__));
int flsl(long) __attribute__((__const__));
int flsll(long long) __attribute__((__const__));


char *index(const char *, int) __attribute__((__pure__));
char *rindex(const char *, int) __attribute__((__pure__));

int strcasecmp(const char *, const char *) __attribute__((__pure__));
int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));

# 46 "/usr/include/string.h" 2 3 4









void *memccpy(void * , const void * , int, size_t);

void *memchr(const void *, int, size_t) __attribute__((__pure__));

void *memrchr(const void *, int, size_t) __attribute__((__pure__));

int memcmp(const void *, const void *, size_t) __attribute__((__pure__));
void *memcpy(void * , const void * , size_t);

void *memmem(const void *, size_t, const void *, size_t) __attribute__((__pure__));

void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

char *stpcpy(char * , const char * );
char *stpncpy(char * , const char * , size_t);


char *strcasestr(const char *, const char *) __attribute__((__pure__));

char *strcat(char * , const char * );
char *strchr(const char *, int) __attribute__((__pure__));
int strcmp(const char *, const char *) __attribute__((__pure__));
int strcoll(const char *, const char *);
char *strcpy(char * , const char * );
size_t strcspn(const char *, const char *) __attribute__((__pure__));

char *strdup(const char *) __attribute__((__malloc__));

char *strerror(int);

int strerror_r(int, char *, size_t);


size_t strlcat(char * , const char * , size_t);
size_t strlcpy(char * , const char * , size_t);

size_t strlen(const char *) __attribute__((__pure__));

void strmode(int, char *);

char *strncat(char * , const char * , size_t);
int strncmp(const char *, const char *, size_t) __attribute__((__pure__));
char *strncpy(char * , const char * , size_t);

char *strndup(const char *, size_t) __attribute__((__malloc__));
size_t strnlen(const char *, size_t) __attribute__((__pure__));


char *strnstr(const char *, const char *, size_t) __attribute__((__pure__));

char *strpbrk(const char *, const char *) __attribute__((__pure__));
char *strrchr(const char *, int) __attribute__((__pure__));

char *strsep(char **, const char *);


char *strsignal(int);

size_t strspn(const char *, const char *) __attribute__((__pure__));
char *strstr(const char *, const char *) __attribute__((__pure__));
char *strtok(char * , const char * );

char *strtok_r(char *, const char *, char **);

size_t strxfrm(char * , const char * , size_t);
# 131 "/usr/include/string.h" 3 4
void swab(const void * , void * , ssize_t);





# 1 "/usr/include/xlocale/_string.h" 1 3 4
# 31 "/usr/include/xlocale/_string.h" 3 4
typedef struct _xlocale *locale_t;
# 46 "/usr/include/xlocale/_string.h" 3 4
int strcoll_l(const char *, const char *, locale_t);
size_t strxfrm_l(char *, const char *, size_t, locale_t);
# 138 "/usr/include/string.h" 2 3 4


# 37 "parser.y" 2
# 1 "tetra_hdr.h" 1







# 1 "/usr/include/inttypes.h" 1 3 4
# 32 "/usr/include/inttypes.h" 3 4
# 1 "/usr/include/machine/_inttypes.h" 1 3 4





# 1 "/usr/include/x86/_inttypes.h" 1 3 4
# 6 "/usr/include/machine/_inttypes.h" 2 3 4
# 33 "/usr/include/inttypes.h" 2 3 4
# 1 "/usr/include/sys/stdint.h" 1 3 4
# 35 "/usr/include/sys/stdint.h" 3 4
# 1 "/usr/include/machine/_stdint.h" 1 3 4
# 36 "/usr/include/sys/stdint.h" 2 3 4
# 1 "/usr/include/sys/_stdint.h" 1 3 4
# 34 "/usr/include/sys/_stdint.h" 3 4
typedef __int8_t int8_t;




typedef __int16_t int16_t;




typedef __int32_t int32_t;




typedef __int64_t int64_t;




typedef __uint8_t uint8_t;




typedef __uint16_t uint16_t;




typedef __uint32_t uint32_t;




typedef __uint64_t uint64_t;




typedef __intptr_t intptr_t;



typedef __uintptr_t uintptr_t;
# 37 "/usr/include/sys/stdint.h" 2 3 4

typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least32_t int_least32_t;
typedef __int_least64_t int_least64_t;

typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least64_t uint_least64_t;

typedef __int_fast8_t int_fast8_t;
typedef __int_fast16_t int_fast16_t;
typedef __int_fast32_t int_fast32_t;
typedef __int_fast64_t int_fast64_t;

typedef __uint_fast8_t uint_fast8_t;
typedef __uint_fast16_t uint_fast16_t;
typedef __uint_fast32_t uint_fast32_t;
typedef __uint_fast64_t uint_fast64_t;


typedef __intmax_t intmax_t;



typedef __uintmax_t uintmax_t;
# 34 "/usr/include/inttypes.h" 2 3 4
# 42 "/usr/include/inttypes.h" 3 4
typedef struct {
 intmax_t quot;
 intmax_t rem;
} imaxdiv_t;





intmax_t imaxabs(intmax_t) __attribute__((__const__));
imaxdiv_t imaxdiv(intmax_t, intmax_t) __attribute__((__const__));

intmax_t strtoimax(const char * , char ** , int);
uintmax_t strtoumax(const char * , char ** , int);
intmax_t wcstoimax(const wchar_t * ,
      wchar_t ** , int);
uintmax_t wcstoumax(const wchar_t * ,
      wchar_t ** , int);

# 9 "tetra_hdr.h" 2
# 1 "exit_funcs.h" 1





void err_exit(char *msg);
void ferr_exit(char *msg, FILE *file);
# 10 "tetra_hdr.h" 2

typedef int32_t tetra_int;
typedef double tetra_float;

enum Comp_Type { LT, LTE, GT, GTE, EQ, NEQ };
enum Assign_Type {
    MUL_BEC, DIV_BEC, PLU_BEC, MIN_BEC, MOD_BEC, POW_BEC,
    RSH_BEC, LSH_BEC, AND_BEC, XOR_BEC, OR_BEC, BEC_BEC
};
enum Shift_Type { SHIFT_L, SHIFT_R };
enum TYPES { INT_T, FLOAT_T, STRING_T, BOOL_T, VOID_T };
# 38 "parser.y" 2
# 1 "node.h" 1
# 9 "node.h"
typedef struct node {
    int type;
    char *str;
    tetra_int d;
    tetra_float f;
    int lineno;
    int n_children;
    int capacity;
    struct node **children;
} TTR_Node;

enum Node_Types {

    N_TYPE,
    N_IDENTIFIER,
    N_EXPR,
    N_BOOL,
    N_STRING,
    N_FLOAT,
    N_INT,
    N_POW,
    N_ADD,
    N_SUB,
    N_POS,
    N_MOD,
    N_NEG,
    N_INV,
    N_MULT,
    N_DIV,
    N_RSHIFT,
    N_LSHIFT,
    N_OR,
    N_AND,
    N_NOT,
    N_LT,
    N_GT,
    N_LTE,
    N_GTE,
    N_EQ,
    N_NEQ,
    N_BOR,
    N_BXOR,
    N_BAND,
    N_CONDITIONAL,
    N_ASSIGN,

    N_STMT,
    N_SMALLSTMT,
    N_PASS,
    N_IF,
    N_ELIFLIST,
    N_ELIF,
    N_ELSE,
    N_WHILESTMT,
    N_WHILE,
    N_FOR,
    N_FORSTMT,
    N_BREAK,
    N_CONTINUE,
    N_FUNC,
    N_FUNCDEF,
    N_PARAMLIST,
    N_POSARGS,
    N_CALL,
    N_GLOBAL,
    N_IDENTLIST,
    N_RETURN,
    N_TGTS
};
# 96 "node.h"
TTR_Node * TTR_make_node(int type, const char *str, tetra_int d,
        tetra_float f, int lineno);




void TTR_free_node(TTR_Node *node);






int TTR_add_child(TTR_Node *parent, TTR_Node *child);
# 39 "parser.y" 2
# 67 "parser.y"
int yywrap(void);
void yyerror(const char *msg);

extern int yylineno;
TTR_Node *parse_tree;
# 109 "parser.tab.c"
# 143 "parser.tab.c"
   enum yytokentype {
     TOK_IDENTIFIER = 258,
     TOK_STRING = 259,
     TOK_BOOL = 260,
     TOK_INT = 261,
     TOK_ASSIGN = 262,
     TOK_TYPE = 263,
     TOK_REAL = 264,
     TOK_OR = 265,
     TOK_AND = 266,
     TOK_NOT = 267,
     TOK_RSHIFT = 268,
     TOK_LSHIFT = 269,
     TOK_LTE = 270,
     TOK_GTE = 271,
     TOK_EQ = 272,
     TOK_NEQ = 273,
     TOK_INDENT = 274,
     TOK_DEDENT = 275,
     TOK_NEWLINE = 276,
     TOK_INT_DIV = 277,
     TOK_POW = 278,
     TOK_IF = 279,
     TOK_ELIF = 280,
     TOK_ELSE = 281,
     TOK_WHILE = 282,
     TOK_FOR = 283,
     TOK_IN = 284,
     TOK_BREAK = 285,
     TOK_CONTINUE = 286,
     TOK_FORALL = 287,
     TOK_PARALLEL = 288,
     TOK_DEF = 289,
     TOK_GLOBAL = 290,
     TOK_PASS = 291,
     TOK_RETURN = 292
   };




typedef union YYSTYPE
{
# 74 "parser.y"

    tetra_float f;
    tetra_int i;
    char *text;
    struct node *node;
# 197 "parser.tab.c"
} YYSTYPE;





extern YYSTYPE yylval;
# 213 "parser.tab.c"
int yyparse (void);
# 225 "parser.tab.c"
# 233 "parser.tab.c"
typedef unsigned char yytype_uint8;






typedef signed char yytype_int8;







typedef unsigned short int yytype_uint16;





typedef short int yytype_int16;
# 386 "parser.tab.c"
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};
# 463 "parser.tab.c"
static const yytype_uint8 yytranslate[] =
{
       0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 38, 54, 49, 2,
      42, 43, 52, 50, 41, 51, 2, 53, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 40, 39,
      45, 44, 46, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 48, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 47, 2, 55, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
       2, 2, 2, 2, 2, 2, 1, 2, 3, 4,
       5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
      25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
      35, 36, 37
};
# 576 "parser.tab.c"
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_IDENTIFIER", "TOK_STRING",
  "TOK_BOOL", "TOK_INT", "TOK_ASSIGN", "TOK_TYPE", "TOK_REAL", "TOK_OR",
  "TOK_AND", "TOK_NOT", "TOK_RSHIFT", "TOK_LSHIFT", "TOK_LTE", "TOK_GTE",
  "TOK_EQ", "TOK_NEQ", "TOK_INDENT", "TOK_DEDENT", "TOK_NEWLINE",
  "TOK_INT_DIV", "TOK_POW", "TOK_IF", "TOK_ELIF", "TOK_ELSE", "TOK_WHILE",
  "TOK_FOR", "TOK_IN", "TOK_BREAK", "TOK_CONTINUE", "TOK_FORALL",
  "TOK_PARALLEL", "TOK_DEF", "TOK_GLOBAL", "TOK_PASS", "TOK_RETURN", "'$'",
  "';'", "':'", "','", "'('", "')'", "'='", "'<'", "'>'", "'|'", "'^'",
  "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "$accept", "program",
  "stmt-list", "stmt", "simple-stmt", "small-stmt-list", "compound-stmt",
  "suite", "if-stmt", "if", "elif-list", "elif", "else", "while-stmt",
  "while", "for-stmt", "for", "target-list", "target", "func-def",
  "funcname", "parameter-list", "return-type", "type", "small-stmt",
  "expression-stmt", "expression-list", "pass-stmt", "return-stmt",
  "break-stmt", "continue-stmt", "global-stmt", "identifier-list",
  "expression", "assignment-expr", "identifier", "conditional-expr",
  "or-test", "and-test", "not-test", "comparison", "or-expr", "xor-expr",
  "and-expr", "shift-expr", "add-expr", "mult-expr", "unary-expr",
  "power-expr", "primary", "atom", "literal", "enclosure", "call",
  "argument-list", "positional-arguments", 0
};
# 616 "parser.tab.c"
static const yytype_uint8 yyr1[] =
{
       0, 56, 57, 58, 58, 59, 59, 60, 61, 61,
      62, 62, 62, 62, 63, 63, 64, 64, 64, 64,
      65, 66, 66, 67, 68, 69, 69, 70, 71, 71,
      72, 73, 73, 74, 75, 76, 77, 77, 77, 78,
      78, 79, 80, 80, 80, 80, 80, 80, 81, 82,
      82, 82, 83, 84, 85, 86, 87, 88, 88, 89,
      90, 90, 90, 91, 92, 92, 93, 93, 94, 94,
      95, 95, 96, 96, 96, 96, 96, 96, 96, 97,
      97, 98, 98, 99, 99, 100, 100, 100, 101, 101,
     101, 102, 102, 102, 102, 103, 103, 103, 103, 104,
     104, 105, 105, 106, 106, 106, 107, 107, 107, 107,
     108, 109, 109, 110, 111, 111
};


static const yytype_uint8 yyr2[] =
{
       0, 2, 3, 1, 2, 1, 1, 2, 1, 3,
       1, 1, 1, 1, 1, 4, 1, 2, 2, 3,
       4, 1, 2, 4, 3, 1, 2, 4, 1, 2,
       6, 1, 3, 1, 7, 1, 0, 1, 3, 0,
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       3, 4, 1, 1, 1, 1, 2, 1, 3, 1,
       1, 3, 3, 1, 1, 5, 1, 3, 1, 3,
       1, 2, 1, 3, 3, 3, 3, 3, 3, 1,
       3, 1, 3, 1, 3, 1, 3, 3, 1, 3,
       3, 1, 3, 3, 3, 1, 2, 2, 2, 1,
       3, 1, 1, 1, 1, 1, 1, 1, 1, 1,
       3, 3, 4, 1, 1, 3
};




static const yytype_uint8 yydefact[] =
{
       0, 63, 109, 106, 107, 108, 0, 0, 0, 0,
      54, 55, 0, 0, 52, 53, 0, 0, 0, 0,
       0, 0, 3, 5, 0, 6, 10, 16, 11, 25,
      12, 28, 0, 13, 8, 42, 48, 43, 44, 45,
      46, 47, 49, 59, 103, 60, 64, 66, 68, 70,
      72, 79, 81, 83, 85, 88, 91, 95, 99, 101,
     104, 105, 102, 103, 71, 0, 0, 0, 31, 33,
       0, 35, 56, 57, 0, 96, 97, 98, 1, 0,
       4, 7, 0, 0, 18, 21, 17, 26, 29, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 36, 0,
     110, 2, 0, 0, 22, 19, 62, 61, 9, 50,
      67, 0, 69, 75, 76, 77, 78, 73, 74, 80,
      82, 84, 87, 86, 89, 90, 92, 93, 94, 0,
     100, 111, 114, 0, 113, 0, 14, 20, 27, 0,
      32, 0, 37, 58, 0, 24, 51, 0, 112, 0,
       0, 0, 0, 39, 23, 65, 115, 0, 30, 38,
      41, 0, 40, 15, 34
};


static const yytype_int16 yydefgoto[] =
{
      -1, 20, 21, 22, 156, 24, 25, 157, 26, 27,
      84, 85, 86, 28, 29, 30, 31, 67, 32, 33,
      70, 161, 181, 182, 34, 35, 36, 37, 38, 39,
      40, 41, 72, 42, 43, 63, 45, 46, 47, 48,
      49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
      59, 60, 61, 62, 153, 154
};




static const yytype_int16 yypact[] =
{
     150, -105, -105, -105, -105, -105, 240, 240, 240, 21,
    -105, -105, 21, 21, -105, -105, 240, 255, 255, 255,
      47, 33, 150, -105, 35, -105, -105, 18, -105, 46,
    -105, 46, 11, -105, 34, -105, 36, -105, -105, -105,
    -105, -105, -105, -105, 14, -105, -4, 48, -105, 20,
      28, 26, 29, 27, -5, -26, 56, -105, 38, -105,
    -105, -105, -105, -105, -105, 41, 43, 1, -105, -105,
      42, -105, 44, -105, 50, 56, 56, 56, -105, 76,
    -105, -105, 240, 58, 18, -105, -105, -105, -105, 240,
     240, 220, 240, 240, 240, 240, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 45, 185, 185, 240, 21, 21, 21,
    -105, -105, 59, 185, -105, -105, -105, -105, -105, 60,
      48, 13, -105, 28, 28, 28, 28, 28, 28, 26,
      29, 27, -5, -5, -26, -26, 56, 56, 56, 56,
      79, -105, -105, 61, 62, 86, -105, -105, -105, 12,
    -105, -12, -105, -105, 185, -105, -105, 240, -105, 240,
     150, 185, 21, 98, -105, -105, -105, 88, -105, -105,
    -105, 69, -105, -105, -105
};


static const yytype_int8 yypgoto[] =
{
    -105, -105, -18, -105, 3, 19, -105, -104, -105, -105,
    -105, 37, -14, -105, -105, -105, -105, -105, 5, -105,
    -105, -105, -105, -105, -105, -105, 4, -105, -105, -105,
    -105, -105, -105, -6, -28, 0, -105, 17, 49, -1,
    -105, 32, 10, 31, 39, -42, -39, 15, 23, -105,
    -105, -105, -105, -105, -105, -105
};





static const yytype_int16 yytable[] =
{
      44, 65, 66, 23, 80, 64, 93, 44, 44, 69,
      74, 158, 71, 73, 68, 87, 44, 88, 89, 165,
      94, -33, 44, 93, 1, 23, 109, 110, 111, 172,
     116, 173, 75, 76, 77, 96, 97, 98, 99, 167,
     105, 106, 117, 82, 83, 107, 108, 78, 1, 2,
       3, 4, 171, 92, 5, 90, 81, 6, -33, 95,
     174, 126, 127, 142, 143, 100, 101, 178, 144, 145,
     125, 79, 83, 91, 103, 102, 122, 92, 104, 112,
     113, 114, 44, 115, 118, 119, 129, 16, 151, 44,
      44, 44, 44, 120, 132, 17, 18, 121, 123, 164,
      19, 166, -95, 169, 168, 170, 180, 152, 183, 184,
     128, 131, 139, 44, 44, 44, 44, 69, 162, 163,
     159, 124, 160, 44, 146, 147, 148, 149, 133, 134,
     135, 136, 137, 138, 140, 150, 0, 0, 0, 0,
       0, 0, 130, 141, 0, 0, 0, 0, 0, 0,
       0, 0, 177, 1, 2, 3, 4, 0, 0, 5,
       0, 175, 6, 176, 44, 0, 0, 44, 0, 44,
      44, 44, 179, 23, 7, 0, 0, 8, 9, 0,
      10, 11, 0, 0, 12, 13, 14, 15, 1, 2,
       3, 4, 16, 0, 5, 0, 0, 6, 0, 0,
      17, 18, 0, 0, 0, 19, 155, 0, 0, 0,
       0, 0, 0, 0, 0, 10, 11, 0, 0, 0,
      13, 14, 15, 1, 2, 3, 4, 16, 0, 5,
       0, 0, 6, 0, 0, 17, 18, 0, 0, 0,
      19, 0, 0, 1, 2, 3, 4, 0, 0, 5,
      10, 11, 6, 0, 0, 13, 14, 15, 1, 2,
       3, 4, 16, 0, 5, 0, 0, 0, 0, 0,
      17, 18, 0, 0, 0, 19, 0, 0, 0, 0,
       0, 0, 16, 0, 0, 0, 0, 0, 0, 0,
      17, 18, 0, 0, 0, 19, 0, 16, 0, 0,
       0, 0, 0, 0, 0, 17, 18, 0, 0, 0,
      19
};







static const yytype_int16 yycheck[] =
{
       0, 7, 8, 0, 22, 6, 10, 7, 8, 9,
      16, 115, 12, 13, 9, 29, 16, 31, 7, 123,
      24, 7, 22, 10, 3, 22, 52, 53, 54, 41,
      29, 43, 17, 18, 19, 15, 16, 17, 18, 26,
      13, 14, 41, 25, 26, 50, 51, 0, 3, 4,
       5, 6, 40, 41, 9, 44, 21, 12, 44, 11,
     164, 89, 90, 105, 106, 45, 46, 171, 107, 108,
      84, 38, 26, 39, 48, 47, 82, 41, 49, 23,
      42, 40, 82, 40, 42, 41, 92, 42, 43, 89,
      90, 91, 92, 43, 95, 50, 51, 21, 40, 40,
      55, 41, 23, 41, 43, 19, 8, 113, 20, 40,
      91, 94, 102, 113, 114, 115, 116, 117, 118, 119,
     116, 84, 117, 123, 109, 110, 111, 112, 96, 97,
      98, 99, 100, 101, 103, 112, -1, -1, -1, -1,
      -1, -1, 93, 104, -1, -1, -1, -1, -1, -1,
      -1, -1, 170, 3, 4, 5, 6, -1, -1, 9,
      -1, 167, 12, 169, 164, -1, -1, 167, -1, 169,
     170, 171, 172, 170, 24, -1, -1, 27, 28, -1,
      30, 31, -1, -1, 34, 35, 36, 37, 3, 4,
       5, 6, 42, -1, 9, -1, -1, 12, -1, -1,
      50, 51, -1, -1, -1, 55, 21, -1, -1, -1,
      -1, -1, -1, -1, -1, 30, 31, -1, -1, -1,
      35, 36, 37, 3, 4, 5, 6, 42, -1, 9,
      -1, -1, 12, -1, -1, 50, 51, -1, -1, -1,
      55, -1, -1, 3, 4, 5, 6, -1, -1, 9,
      30, 31, 12, -1, -1, 35, 36, 37, 3, 4,
       5, 6, 42, -1, 9, -1, -1, -1, -1, -1,
      50, 51, -1, -1, -1, 55, -1, -1, -1, -1,
      -1, -1, 42, -1, -1, -1, -1, -1, -1, -1,
      50, 51, -1, -1, -1, 55, -1, 42, -1, -1,
      -1, -1, -1, -1, -1, 50, 51, -1, -1, -1,
      55
};



static const yytype_uint8 yystos[] =
{
       0, 3, 4, 5, 6, 9, 12, 24, 27, 28,
      30, 31, 34, 35, 36, 37, 42, 50, 51, 55,
      57, 58, 59, 60, 61, 62, 64, 65, 69, 70,
      71, 72, 74, 75, 80, 81, 82, 83, 84, 85,
      86, 87, 89, 90, 91, 92, 93, 94, 95, 96,
      97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
     107, 108, 109, 91, 95, 89, 89, 73, 74, 91,
      76, 91, 88, 91, 89, 103, 103, 103, 0, 38,
      58, 21, 25, 26, 66, 67, 68, 68, 68, 7,
      44, 39, 41, 10, 24, 11, 15, 16, 17, 18,
      45, 46, 47, 48, 49, 13, 14, 50, 51, 52,
      53, 54, 23, 42, 40, 40, 29, 41, 42, 41,
      43, 21, 89, 40, 67, 68, 90, 90, 61, 89,
      94, 93, 95, 97, 97, 97, 97, 97, 97, 98,
      99, 100, 101, 101, 102, 102, 103, 103, 103, 103,
     104, 43, 89, 110, 111, 21, 60, 63, 63, 82,
      74, 77, 91, 91, 40, 63, 41, 26, 43, 41,
      19, 40, 41, 43, 63, 89, 89, 58, 63, 91,
       8, 78, 79, 20, 40
};
# 1084 "parser.tab.c"
static long unsigned int
yystrlen (const char *yystr)





{
  long unsigned int yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# 1108 "parser.tab.c"
static char *
yystpcpy (char *yydest, const char *yysrc)






{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# 1136 "parser.tab.c"
static long unsigned int
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      long unsigned int yyn = 0;
      char const *yyp = yystr;

      for (;;)
 switch (*++yyp)
   {
   case '\'':
   case ',':
     goto do_not_strip_quotes;

   case '\\':
     if (*++yyp != '\\')
       goto do_not_strip_quotes;

   default:
     if (yyres)
       yyres[yyn] = *yyp;
     yyn++;
     break;

   case '"':
     if (yyres)
       yyres[yyn] = '\0';
     return yyn;
   }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# 1184 "parser.tab.c"
static int
yysyntax_error (long unsigned int *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  long unsigned int yysize0 = yytnamerr (0, yytname[yytoken]);
  long unsigned int yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };

  const char *yyformat = 0;

  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];


  int yycount = 0;
# 1226 "parser.tab.c"
  if (yytoken != (-2))
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!(!!((yyn) == (-105))))
        {



          int yyxbegin = yyn < 0 ? -yyn : 0;

          int yychecklim = 310 - yyn + 1;
          int yyxend = yychecklim < 56 ? yychecklim : 56;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != 1
                && !(0))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  long unsigned int yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= ((long unsigned int) -1)))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {




      case 0: yyformat = "syntax error"; break;
      case 1: yyformat = "syntax error, unexpected %s"; break;
      case 2: yyformat = "syntax error, unexpected %s, expecting %s"; break;
      case 3: yyformat = "syntax error, unexpected %s, expecting %s or %s"; break;
      case 4: yyformat = "syntax error, unexpected %s, expecting %s or %s or %s"; break;
      case 5: yyformat = "syntax error, unexpected %s, expecting %s or %s or %s or %s"; break;

    }

  {
    long unsigned int yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= ((long unsigned int) -1)))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= ((long unsigned int) -1)))
        *yymsg_alloc = ((long unsigned int) -1);
      return 1;
    }




  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
# 1323 "parser.tab.c"
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)







{
  ((void) (yyvaluep));

  if (!yymsg)
    yymsg = "Deleting";
  ;

  switch (yytype)
    {

      default:
        break;
    }
}





int yychar;
# 1363 "parser.tab.c"
YYSTYPE yylval ;


int yynerrs;
# 1386 "parser.tab.c"
int
yyparse (void)






{
    int yystate;

    int yyerrstatus;
# 1407 "parser.tab.c"
    yytype_int16 yyssa[200];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;


    YYSTYPE yyvsa[200];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    long unsigned int yystacksize;

  int yyn;
  int yyresult;

  int yytoken = 0;


  YYSTYPE yyval;



  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  long unsigned int yymsg_alloc = sizeof yymsgbuf;






  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = 200;

  ;

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = (-2);
  goto yysetstate;




 yynewstate:


  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {

      long unsigned int yysize = yyssp - yyss + 1;
# 1492 "parser.tab.c"
      if (10000 <= yystacksize)
 goto yyexhaustedlab;
      yystacksize *= 2;
      if (10000 < yystacksize)
 yystacksize = 10000;

      {
 yytype_int16 *yyss1 = yyss;
 union yyalloc *yyptr =
   (union yyalloc *) malloc (((yystacksize) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) + (sizeof (union yyalloc) - 1)));
 if (! yyptr)
   goto yyexhaustedlab;
 do { long unsigned int yynewbytes; __builtin_memcpy (&yyptr->yyss_alloc, yyss, (yysize) * sizeof (*(yyss))); yyss = &yyptr->yyss_alloc; yynewbytes = yystacksize * sizeof (*yyss) + (sizeof (union yyalloc) - 1); yyptr += yynewbytes / sizeof (*yyptr); } while ((0));
 do { long unsigned int yynewbytes; __builtin_memcpy (&yyptr->yyvs_alloc, yyvs, (yysize) * sizeof (*(yyvs))); yyvs = &yyptr->yyvs_alloc; yynewbytes = yystacksize * sizeof (*yyvs) + (sizeof (union yyalloc) - 1); yyptr += yynewbytes / sizeof (*yyptr); } while ((0));

 if (yyss1 != yyssa)
   free (yyss1);
      }



      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      ;


      if (yyss + yystacksize - 1 <= yyssp)
 goto yyabortlab;
    }

  ;

  if (yystate == 78)
    goto yyacceptlab;

  goto yybackup;




yybackup:





  yyn = yypact[yystate];
  if ((!!((yyn) == (-105))))
    goto yydefault;




  if (yychar == (-2))
    {
      ;
      yychar = yylex ();
    }

  if (yychar <= 0)
    {
      yychar = yytoken = 0;
      ;
    }
  else
    {
      yytoken = ((unsigned int) (yychar) <= 292 ? yytranslate[yychar] : 2);
      ;
    }



  yyn += yytoken;
  if (yyn < 0 || 310 < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if ((0))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }



  if (yyerrstatus)
    yyerrstatus--;


  ;


  yychar = (-2);

  yystate = yyn;
 
  *++yyvsp = yylval;
 

  goto yynewstate;





yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;





yyreduce:

  yylen = yyr2[yyn];
# 1621 "parser.tab.c"
  yyval = yyvsp[1-yylen];


  ;
  switch (yyn)
    {
        case 2:
# 118 "parser.y"
    { parse_tree = (yyvsp[(1) - (3)].node); }
    break;

  case 3:
# 120 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_STMT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 4:
# 121 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_STMT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (2)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node)));}
    break;

  case 5:
# 123 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 6:
# 124 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 7:
# 126 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); }
    break;

  case 8:
# 128 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_SMALLSTMT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 9:
# 130 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_SMALLSTMT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 13:
# 135 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 14:
# 137 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 15:
# 138 "parser.y"
    { (yyval.node) = (yyvsp[(3) - (4)].node); }
    break;

  case 16:
# 140 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 17:
# 141 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IF), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (2)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 18:
# 142 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IF), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (2)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 19:
# 143 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IF), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 20:
# 145 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IF), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (4)].node))); TTR_add_child((yyval.node), ((yyvsp[(4) - (4)].node))); }
    break;

  case 21:
# 147 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_ELIFLIST), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 22:
# 148 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_ELIFLIST), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (2)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 23:
# 150 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_ELIF), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (4)].node))); TTR_add_child((yyval.node), ((yyvsp[(4) - (4)].node))); }
    break;

  case 24:
# 152 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_ELSE), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 25:
# 154 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 26:
# 155 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_WHILESTMT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (2)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 27:
# 157 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_WHILE), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (4)].node))); TTR_add_child((yyval.node), ((yyvsp[(4) - (4)].node))); }
    break;

  case 28:
# 159 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 29:
# 160 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_FORSTMT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (2)].node))); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 30:
# 163 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_FOR), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (6)].node))); TTR_add_child((yyval.node), ((yyvsp[(4) - (6)].node))); TTR_add_child((yyval.node), ((yyvsp[(6) - (6)].node))); }
    break;

  case 31:
# 165 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_TGTS), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 32:
# 166 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_TGTS), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 33:
# 168 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 34:
# 171 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_FUNCDEF), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (7)].node))); TTR_add_child((yyval.node), ((yyvsp[(4) - (7)].node))); TTR_add_child((yyval.node), ((yyvsp[(6) - (7)].node))); }
    break;

  case 35:
# 173 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 36:
# 175 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_PARAMLIST), "", 0, 0.0, yylineno); }
    break;

  case 37:
# 176 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_PARAMLIST), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 38:
# 178 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_PARAMLIST), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 39:
# 180 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_TYPE), "", (VOID_T), 0.0, yylineno); }
    break;

  case 40:
# 181 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 41:
# 183 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_TYPE), "", (yylval.i), 0.0, yylineno); }
    break;

  case 47:
# 190 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 49:
# 194 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_EXPR), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 51:
# 197 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_EXPR), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (4)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (4)].node))); }
    break;

  case 52:
# 199 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_PASS), "", 0, 0.0, yylineno); }
    break;

  case 53:
# 201 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_RETURN), "", 0, 0.0, yylineno); }
    break;

  case 54:
# 203 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_BREAK), "", 0, 0.0, yylineno); }
    break;

  case 55:
# 205 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_CONTINUE), "", 0, 0.0, yylineno); }
    break;

  case 56:
# 207 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_GLOBAL), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 57:
# 209 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IDENTLIST), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 58:
# 211 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IDENTLIST), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 59:
# 213 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 60:
# 215 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 61:
# 217 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_ASSIGN), "", (BEC_BEC), 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 62:
# 219 "parser.y"
    { fprintf(__stderrp, "Assign type: %d\n", (yyvsp[(2) - (3)].i));
            ((yyval.node)) = TTR_make_node((N_ASSIGN), "", ((yyvsp[(2) - (3)].i)), 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 63:
# 222 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_IDENTIFIER), (yylval.text), 0, 0.0, yylineno); }
    break;

  case 64:
# 224 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 65:
# 226 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_CONDITIONAL), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (5)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (5)].node))); TTR_add_child((yyval.node), ((yyvsp[(5) - (5)].node))); }
    break;

  case 66:
# 228 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 67:
# 229 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_OR), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 68:
# 231 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 69:
# 232 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_AND), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 70:
# 234 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 71:
# 235 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_NOT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 72:
# 237 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 73:
# 238 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_LT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 74:
# 239 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_GT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 75:
# 240 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_LTE), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 76:
# 241 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_GTE), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 77:
# 242 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_EQ), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 78:
# 243 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_NEQ), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 79:
# 245 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 80:
# 246 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_BOR), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 81:
# 248 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 82:
# 249 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_BXOR), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 83:
# 251 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 84:
# 252 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_BAND), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 85:
# 254 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 86:
# 255 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_LSHIFT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 87:
# 256 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_RSHIFT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 88:
# 258 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 89:
# 259 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_ADD), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 90:
# 260 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_SUB), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 91:
# 262 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 92:
# 263 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_MULT), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 93:
# 264 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_DIV), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 94:
# 265 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_MOD), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 95:
# 267 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 96:
# 268 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_POS), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 97:
# 269 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_NEG), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 98:
# 270 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_INV), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(2) - (2)].node))); }
    break;

  case 99:
# 272 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 100:
# 273 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_POW), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;

  case 102:
# 276 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 103:
# 278 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_STRING), (yylval.text), 0, 0.0, yylineno); }
    break;

  case 104:
# 279 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 105:
# 280 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 106:
# 282 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_BOOL), "", (yylval.i), 0.0, yylineno); }
    break;

  case 107:
# 283 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_BOOL), "", (yylval.i), 0.0, yylineno); }
    break;

  case 108:
# 284 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_FLOAT), "", 0, (yylval.f), yylineno); }
    break;

  case 109:
# 285 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_STRING), (yylval.text), 0, 0.0, yylineno); }
    break;

  case 110:
# 287 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 111:
# 289 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_CALL), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); }
    break;

  case 112:
# 290 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_CALL), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (4)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (4)].node))); }
    break;

  case 113:
# 292 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 114:
# 294 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_POSARGS), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (1)].node))); }
    break;

  case 115:
# 296 "parser.y"
    { ((yyval.node)) = TTR_make_node((N_POSARGS), "", 0, 0.0, yylineno); TTR_add_child((yyval.node), ((yyvsp[(1) - (3)].node))); TTR_add_child((yyval.node), ((yyvsp[(3) - (3)].node))); }
    break;
# 2249 "parser.tab.c"
      default: break;
    }
# 2262 "parser.tab.c"
  ;

  (yyvsp -= (yylen), yyssp -= (yylen));
  yylen = 0;
  ;

  *++yyvsp = yyval;





  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - 56] + *yyssp;
  if (0 <= yystate && yystate <= 310 && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - 56];

  goto yynewstate;





yyerrlab:


  yytoken = yychar == (-2) ? (-2) : ((unsigned int) (yychar) <= 292 ? yytranslate[yychar] : 2);


  if (!yyerrstatus)
    {
      ++yynerrs;





      {
        char const *yymsgp = "syntax error";
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, yyssp, yytoken);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              free (yymsg);
            yymsg = (char *) malloc (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, yyssp, yytoken);
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }


    }



  if (yyerrstatus == 3)
    {



      if (yychar <= 0)
 {

   if (yychar == 0)
     goto yyabortlab;
 }
      else
 {
   yydestruct ("Error: discarding",
        yytoken, &yylval);
   yychar = (-2);
 }
    }



  goto yyerrlab1;





yyerrorlab:




  if ( 0)
     goto yyerrorlab;



  (yyvsp -= (yylen), yyssp -= (yylen));
  yylen = 0;
  ;
  yystate = *yyssp;
  goto yyerrlab1;





yyerrlab1:
  yyerrstatus = 3;

  for (;;)
    {
      yyn = yypact[yystate];
      if (!(!!((yyn) == (-105))))
 {
   yyn += 1;
   if (0 <= yyn && yyn <= 310 && yycheck[yyn] == 1)
     {
       yyn = yytable[yyn];
       if (0 < yyn)
  break;
     }
 }


      if (yyssp == yyss)
 goto yyabortlab;


      yydestruct ("Error: popping",
    yystos[yystate], yyvsp);
      (yyvsp -= (1), yyssp -= (1));
      yystate = *yyssp;
      ;
    }

 
  *++yyvsp = yylval;
 



  ;

  yystate = yyn;
  goto yynewstate;





yyacceptlab:
  yyresult = 0;
  goto yyreturn;




yyabortlab:
  yyresult = 1;
  goto yyreturn;





yyexhaustedlab:
  yyerror ("memory exhausted");
  yyresult = 2;



yyreturn:
  if (yychar != (-2))
    {


      yytoken = ((unsigned int) (yychar) <= 292 ? yytranslate[yychar] : 2);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }


  (yyvsp -= (yylen), yyssp -= (yylen));
  ;
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
    yystos[*yyssp], yyvsp);
      (yyvsp -= (1), yyssp -= (1));
    }

  if (yyss != yyssa)
    free (yyss);


  if (yymsg != yymsgbuf)
    free (yymsg);


  return (yyresult);
}
# 298 "parser.y"


int yywrap()
{
    return 1;
}

void yyerror(const char *msg)
{
    fprintf(__stderrp, "<Line %d> Parser error: '%s'.\n", yylineno, msg);
}

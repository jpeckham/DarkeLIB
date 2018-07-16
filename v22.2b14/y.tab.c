/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 3 "grammar.y" /* yacc.c:339  */


#include "std.h"
#include "compiler.h"
#include "lex.h"
#include "scratchpad.h"

#include "lpc_incl.h"
#include "simul_efun.h"
#include "generate.h"
#include "master.h"

/* gross. Necessary? - Beek */
#ifdef WIN32
#define MSDOS
#endif
#line 18 "grammar.y.pre"
/*
 * This is the grammar definition of LPC, and its parse tree generator.
 */

/* down to one global :) 
   bits:
      SWITCH_CONTEXT     - we're inside a switch
      LOOP_CONTEXT       - we're inside a loop
      SWITCH_STRINGS     - a string case has been found
      SWITCH_NUMBERS     - a non-zero numeric case has been found
      SWITCH_RANGES      - a range has been found
      SWITCH_DEFAULT     - a default has been found
 */
int context;
int num_refs;

/*
 * bison & yacc don't prototype this in y.tab.h
 */
int yyparse PROT((void));


#line 106 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    L_STRING = 258,
    L_NUMBER = 259,
    L_REAL = 260,
    L_BASIC_TYPE = 261,
    L_TYPE_MODIFIER = 262,
    L_DEFINED_NAME = 263,
    L_IDENTIFIER = 264,
    L_EFUN = 265,
    L_INC = 266,
    L_DEC = 267,
    L_ASSIGN = 268,
    L_LAND = 269,
    L_LOR = 270,
    L_LSH = 271,
    L_RSH = 272,
    L_ORDER = 273,
    L_NOT = 274,
    L_IF = 275,
    L_ELSE = 276,
    L_SWITCH = 277,
    L_CASE = 278,
    L_DEFAULT = 279,
    L_RANGE = 280,
    L_DOT_DOT_DOT = 281,
    L_WHILE = 282,
    L_DO = 283,
    L_FOR = 284,
    L_FOREACH = 285,
    L_IN = 286,
    L_BREAK = 287,
    L_CONTINUE = 288,
    L_RETURN = 289,
    L_ARROW = 290,
    L_INHERIT = 291,
    L_COLON_COLON = 292,
    L_ARRAY_OPEN = 293,
    L_MAPPING_OPEN = 294,
    L_FUNCTION_OPEN = 295,
    L_NEW_FUNCTION_OPEN = 296,
    L_SSCANF = 297,
    L_CATCH = 298,
    L_PARSE_COMMAND = 299,
    L_TIME_EXPRESSION = 300,
    L_CLASS = 301,
    L_NEW = 302,
    L_PARAMETER = 303,
    L_LAMBDA = 304,
    LOWER_THAN_ELSE = 305,
    L_EQ = 306,
    L_NE = 307
  };
#endif
/* Tokens.  */
#define L_STRING 258
#define L_NUMBER 259
#define L_REAL 260
#define L_BASIC_TYPE 261
#define L_TYPE_MODIFIER 262
#define L_DEFINED_NAME 263
#define L_IDENTIFIER 264
#define L_EFUN 265
#define L_INC 266
#define L_DEC 267
#define L_ASSIGN 268
#define L_LAND 269
#define L_LOR 270
#define L_LSH 271
#define L_RSH 272
#define L_ORDER 273
#define L_NOT 274
#define L_IF 275
#define L_ELSE 276
#define L_SWITCH 277
#define L_CASE 278
#define L_DEFAULT 279
#define L_RANGE 280
#define L_DOT_DOT_DOT 281
#define L_WHILE 282
#define L_DO 283
#define L_FOR 284
#define L_FOREACH 285
#define L_IN 286
#define L_BREAK 287
#define L_CONTINUE 288
#define L_RETURN 289
#define L_ARROW 290
#define L_INHERIT 291
#define L_COLON_COLON 292
#define L_ARRAY_OPEN 293
#define L_MAPPING_OPEN 294
#define L_FUNCTION_OPEN 295
#define L_NEW_FUNCTION_OPEN 296
#define L_SSCANF 297
#define L_CATCH 298
#define L_PARSE_COMMAND 299
#define L_TIME_EXPRESSION 300
#define L_CLASS 301
#define L_NEW 302
#define L_PARAMETER 303
#define L_LAMBDA 304
#define LOWER_THAN_ELSE 305
#define L_EQ 306
#define L_NE 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 121 "grammar.y" /* yacc.c:355  */

    POINTER_INT pointer_int;
    int number;
    float real;
    char *string;
    int type;
    struct { short num_arg; char flags; } argument;
    ident_hash_elem_t *ihe;
    parse_node_t *node;
    function_context_t *contextp;
    struct {
	parse_node_t *node;
        char num;
    } decl; /* 5 */
    struct {
	char num_local;
	char max_num_locals; 
	short context; 
	short save_current_type; 
	short save_exact_types;
    } func_block; /* 8 */

#line 273 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 288 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1804

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  100
/* YYNRULES -- Number of rules.  */
#define YYNRULES  251
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  478

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    71,    61,    54,     2,
      65,    66,    60,    58,    68,    59,     2,    62,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    67,    64,
      57,     2,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    72,     2,    73,    53,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,    52,    70,    63,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    55,    56
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   211,   211,   219,   225,   231,   233,   243,   313,   321,
     330,   334,   342,   350,   355,   363,   368,   373,   399,   372,
     457,   464,   465,   466,   469,   487,   501,   502,   505,   508,
     507,   517,   516,   597,   598,   616,   617,   634,   651,   655,
     659,   666,   675,   689,   694,   695,   714,   726,   741,   745,
     755,   764,   772,   774,   781,   782,   786,   812,   868,   878,
     878,   878,   882,   888,   887,   909,   921,   956,   968,  1001,
    1007,  1019,  1023,  1030,  1038,  1051,  1052,  1053,  1054,  1055,
    1056,  1062,  1067,  1090,  1104,  1103,  1119,  1118,  1134,  1133,
    1159,  1181,  1192,  1210,  1216,  1228,  1227,  1249,  1253,  1257,
    1263,  1273,  1272,  1307,  1314,  1322,  1330,  1338,  1353,  1370,
    1382,  1399,  1404,  1409,  1414,  1419,  1424,  1433,  1438,  1443,
    1448,  1453,  1458,  1463,  1468,  1473,  1478,  1483,  1488,  1493,
    1501,  1506,  1515,  1542,  1548,  1573,  1580,  1587,  1613,  1618,
    1642,  1665,  1680,  1725,  1763,  1768,  1773,  1938,  2033,  2114,
    2119,  2215,  2237,  2259,  2282,  2292,  2304,  2329,  2352,  2374,
    2375,  2376,  2377,  2378,  2379,  2383,  2390,  2412,  2416,  2421,
    2429,  2434,  2442,  2449,  2463,  2468,  2473,  2481,  2492,  2511,
    2519,  2627,  2628,  2635,  2640,  2660,  2661,  2704,  2721,  2727,
    2726,  2758,  2783,  2788,  2793,  2801,  2809,  2814,  2819,  2863,
    2914,  2915,  2920,  2922,  2921,  2978,  3012,  3102,  3121,  3126,
    3134,  3139,  3148,  3147,  3163,  3173,  3185,  3184,  3200,  3206,
    3221,  3230,  3231,  3236,  3244,  3245,  3252,  3264,  3268,  3279,
    3278,  3294,  3293,  3308,  3344,  3364,  3363,  3434,  3433,  3507,
    3506,  3560,  3559,  3591,  3617,  3633,  3634,  3649,  3665,  3681,
    3716,  3721
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "L_STRING", "L_NUMBER", "L_REAL",
  "L_BASIC_TYPE", "L_TYPE_MODIFIER", "L_DEFINED_NAME", "L_IDENTIFIER",
  "L_EFUN", "L_INC", "L_DEC", "L_ASSIGN", "L_LAND", "L_LOR", "L_LSH",
  "L_RSH", "L_ORDER", "L_NOT", "L_IF", "L_ELSE", "L_SWITCH", "L_CASE",
  "L_DEFAULT", "L_RANGE", "L_DOT_DOT_DOT", "L_WHILE", "L_DO", "L_FOR",
  "L_FOREACH", "L_IN", "L_BREAK", "L_CONTINUE", "L_RETURN", "L_ARROW",
  "L_INHERIT", "L_COLON_COLON", "L_ARRAY_OPEN", "L_MAPPING_OPEN",
  "L_FUNCTION_OPEN", "L_NEW_FUNCTION_OPEN", "L_SSCANF", "L_CATCH",
  "L_PARSE_COMMAND", "L_TIME_EXPRESSION", "L_CLASS", "L_NEW",
  "L_PARAMETER", "L_LAMBDA", "LOWER_THAN_ELSE", "'?'", "'|'", "'^'", "'&'",
  "L_EQ", "L_NE", "'<'", "'+'", "'-'", "'*'", "'%'", "'/'", "'~'", "';'",
  "'('", "')'", "':'", "','", "'{'", "'}'", "'$'", "'['", "']'", "$accept",
  "all", "program", "possible_semi_colon", "inheritance", "real", "number",
  "optional_star", "block_or_semi", "identifier", "def", "$@1", "@2",
  "modifier_change", "member_name", "member_name_list", "member_list",
  "$@3", "type_decl", "@4", "new_local_name", "atomic_type", "basic_type",
  "arg_type", "new_arg", "argument", "argument_list", "type_modifier_list",
  "type", "cast", "opt_basic_type", "name_list", "new_name", "block",
  "decl_block", "local_declarations", "$@5", "new_local_def",
  "single_new_local_def", "single_new_local_def_with_init",
  "local_name_list", "statements", "statement", "while", "$@6", "do",
  "$@7", "for", "$@8", "foreach_var", "foreach_vars", "foreach", "$@9",
  "for_expr", "first_for_expr", "switch", "$@10", "switch_block", "case",
  "case_label", "constant", "comma_expr", "expr0", "return", "expr_list",
  "expr_list_node", "expr_list2", "expr_list3", "expr_list4", "assoc_pair",
  "lvalue", "l_new_function_open", "simple_function_pointer", "expr4",
  "@11", "@12", "expr_or_block", "catch", "@13", "sscanf", "parse_command",
  "time_expression", "@14", "lvalue_list", "string", "string_con1",
  "string_con2", "class_init", "opt_class_init", "function_call", "@15",
  "@16", "@17", "@18", "@19", "@20", "efun_override", "function_name",
  "cond", "optional_else_part", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    63,   124,    94,    38,   306,   307,    60,    43,    45,
      42,    37,    47,   126,    59,    40,    41,    58,    44,   123,
     125,    36,    91,    93
};
# endif

#define YYPACT_NINF -306

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-306)))

#define YYTABLE_NINF -246

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -306,    38,    58,  -306,    40,  -306,    74,  -306,  -306,    84,
      92,  -306,  -306,  -306,  -306,    13,   268,  -306,  -306,  -306,
    -306,  -306,   330,    85,   100,  -306,    13,   132,   184,   126,
     133,   139,  -306,  -306,    28,  -306,    92,    -2,    13,  -306,
    -306,  -306,  1599,   140,   330,  -306,  -306,  -306,  -306,   144,
    -306,  -306,   190,     9,    18,   196,   183,   183,  1599,   330,
    1114,   481,  1599,  -306,   187,  -306,   195,  -306,   219,  -306,
     304,  1599,  1599,   905,   263,  -306,  -306,   295,  1599,  1014,
     340,   279,  -306,    14,  -306,  -306,  -306,  -306,  -306,   184,
    -306,   278,   317,    36,   206,     7,  1599,   330,   342,  -306,
     154,  1185,  -306,     0,  -306,  -306,  -306,  1085,   300,  -306,
     341,   379,   325,   349,  -306,   301,  1014,   278,  1599,     2,
    1599,     2,   362,  -306,  -306,  -306,    89,   381,  1599,    92,
     -18,  -306,   330,  -306,  1599,  1599,  1599,  1599,  1599,  1599,
    1599,  1599,  1599,  1599,  1599,  1599,  1599,  1599,  1599,  1599,
    1599,  -306,  -306,  1599,   344,  1599,   330,  1254,  -306,  -306,
    -306,  -306,  -306,  -306,    92,  -306,   357,     8,  -306,  -306,
    1014,  -306,    36,  1323,  -306,  -306,  -306,   358,   974,  1599,
     361,   552,   376,  1599,  1635,  1599,  -306,  -306,  -306,  1657,
    -306,   383,  1323,  -306,  -306,   179,   377,  -306,  1599,  -306,
    1153,   454,   197,   197,   212,   521,   246,   181,  1426,   119,
     119,   212,   302,   302,  -306,  -306,  -306,  1014,  -306,   333,
     363,  1599,    59,  1323,  1323,   396,  -306,  -306,    36,    92,
     378,   390,  -306,  -306,  1014,  -306,  -306,  -306,  1014,  1599,
     200,   694,  1599,  -306,  -306,   394,   380,  -306,   224,  1599,
     395,  1599,  -306,    88,   277,  -306,   397,   398,  -306,    53,
    -306,   330,   406,   401,   393,  -306,  1684,  -306,    30,   411,
     412,   413,  -306,   414,   415,   402,   424,  1392,  -306,  -306,
    -306,  -306,   425,   765,  -306,  -306,  -306,  -306,  -306,   182,
    -306,  -306,  1706,   229,   267,  -306,  -306,  -306,  1014,  -306,
    1323,   410,  -306,  1599,  -306,    60,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,    92,  -306,  -306,   183,   428,  -306,  1599,
    1599,  1599,   836,  1045,   161,  -306,  -306,  -306,   211,    92,
    -306,  -306,  -306,  1599,  -306,   330,  -306,  1323,   430,  1599,
    -306,    73,    91,  -306,  -306,   429,  -306,   312,   315,   319,
     471,    92,   486,  -306,  -306,   437,   434,  -306,  -306,  -306,
     435,   473,  -306,   396,   449,   441,  1684,   460,  -306,   465,
    -306,   136,  -306,  -306,  -306,   836,  -306,  -306,   467,   396,
    1599,  1461,   161,  1599,   520,    92,  -306,   468,  1599,  -306,
    -306,   522,   472,   836,  1599,  -306,  1014,   478,  -306,   874,
    1599,  -306,  -306,  1014,   836,  -306,  -306,  -306,   320,  1530,
    -306,  1014,  -306,   331,   483,   479,   836,    56,   482,   623,
    -306,  -306,  -306,  -306,   544,   546,   547,    56,    -5,  1222,
     501,  -306,   623,   495,   623,   836,  -306,  -306,  -306,   942,
      56,  -306,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,  -306,  -306,  -306,  -306,
    -306,   499,    66,   298,   298,   314,  1290,  1358,  1495,   156,
     156,   314,   305,   305,  -306,  -306,  -306,  -306
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    48,     1,    48,    21,     5,    23,    22,    53,
      10,    49,     6,     3,    35,     0,     0,    24,    38,    52,
      50,    11,     0,     0,    54,   224,     0,     0,   221,    36,
      37,     0,    15,    16,    56,    20,    10,     0,     0,     7,
     225,    31,     0,     0,     0,    55,   222,   223,    28,     0,
       9,     8,   203,   186,   187,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,   212,     0,   216,     0,   188,
       0,     0,     0,     0,     0,   164,   163,     0,     0,    57,
       0,     0,   205,   159,   202,   160,   161,   162,   200,   220,
     185,     0,     0,    43,    56,     0,     0,     0,     0,   235,
       0,     0,   152,   180,   153,   154,   246,   170,     0,   172,
     168,     0,     0,   175,   177,     0,   130,   182,     0,     0,
       0,     0,   231,   184,   156,   155,    35,     0,     0,    10,
       0,   189,     0,   151,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,   158,     0,     0,     0,     0,     0,   229,   237,
      34,    33,    42,    39,    10,    46,     0,    44,    32,    29,
     133,   247,    43,     0,   244,   243,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,   210,   213,     0,
     217,     0,     0,    36,    37,     0,     0,   201,     0,   248,
     136,   135,   144,   145,   142,     0,   137,   138,   139,   140,
     141,   143,   146,   147,   148,   149,   150,   132,   183,     0,
     191,     0,     0,     0,     0,    40,    18,    45,     0,    10,
       0,     0,   209,   173,   179,   208,   178,   207,   131,     0,
       0,     0,     0,   227,   227,     0,     0,    51,     0,     0,
       0,     0,   239,     0,     0,   199,     0,     0,    41,     0,
      47,     0,    26,     0,     0,   236,   218,   211,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,    81,    63,
      59,    80,     0,     0,    76,    77,    60,    61,    78,     0,
      79,    75,     0,     0,     0,   232,   241,   190,   134,   206,
       0,     0,   198,     0,   196,     0,   230,   238,    14,    13,
      19,    12,    25,    10,    30,   204,     0,     0,    73,     0,
       0,     0,     0,     0,     0,    82,    83,   165,     0,    10,
      58,    72,    74,     0,   233,     0,   234,     0,     0,     0,
     197,     0,     0,   192,    27,   218,   214,     0,     0,     0,
       0,    10,     0,   100,    99,     0,    98,    90,    92,    91,
      93,     0,   166,     0,    69,     0,   218,     0,   228,     0,
     240,     0,   193,   195,   219,     0,   101,    84,     0,     0,
       0,     0,     0,     0,    65,    10,    64,     0,     0,   242,
     194,   250,     0,     0,     0,    67,    68,     0,    94,     0,
       0,    70,   215,   226,     0,   249,    62,    85,     0,     0,
      95,    66,   251,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    96,   126,     0,     0,     0,     0,     0,   109,
     110,   108,     0,     0,     0,     0,   128,   127,   129,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,   102,   103,    89,
     125,     0,     0,   118,   119,   116,   111,   112,   113,   114,
     115,   117,   120,   121,   122,   123,   124,   107
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -306,  -306,  -306,  -306,  -306,  -306,  -306,    -6,  -306,    -8,
    -306,  -306,  -306,  -306,  -306,   254,  -306,  -306,  -306,  -306,
    -213,  -306,    -7,  -295,   356,   426,  -306,   564,  -306,  -306,
    -306,   533,  -306,  -104,  -306,   164,  -306,  -306,   262,  -306,
     201,   321,  -210,  -306,  -306,  -306,  -306,  -306,  -306,   205,
    -306,  -306,  -306,  -238,  -306,  -306,  -306,  -183,   189,   163,
    1342,   -10,   -39,  -306,  -155,  -171,   450,  -306,  -306,   427,
     -51,  -306,  -306,   -47,  -306,  -306,   485,  -306,  -306,  -306,
    -306,  -306,  -306,  -305,  -306,   -14,   -15,  -306,   365,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,   545,  -306,  -306,  -306
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    13,     5,    75,    76,   261,   310,    77,
       6,    43,   259,     7,   262,   263,    95,   229,     8,    48,
     162,    18,   163,   164,   165,   166,   167,     9,    10,    78,
      20,    23,    24,   280,   281,   241,   329,   364,   359,   353,
     365,   282,   432,   284,   393,   285,   322,   286,   435,   360,
     361,   287,   416,   354,   355,   288,   392,   433,   434,   428,
     429,   289,   116,   290,   108,   109,   110,   112,   113,   114,
      80,    81,    82,    83,   198,    98,   188,    84,   119,    85,
      86,    87,   121,   317,    88,    37,    89,   368,   293,    90,
     223,   192,   173,   224,   300,   337,    91,    92,   291,   405
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,    27,    19,    79,    22,   102,   104,   233,    31,   103,
     103,    28,   258,    14,    34,   187,    25,   187,   231,   105,
     440,   107,   111,    28,    47,  -180,  -180,  -180,   351,   351,
      44,   283,   124,   125,   227,   156,    94,   245,     3,   133,
     374,    42,    14,    96,   160,   161,   -15,     4,   197,   156,
     183,   106,   115,   127,   308,   -16,    38,   170,    -2,    25,
     423,   387,   441,   130,    46,     4,   129,   185,   256,   257,
     423,   186,   157,   283,    99,   424,   228,   168,    26,   184,
     233,   189,   127,  -245,   254,   424,   157,   351,   169,   171,
      14,   130,   175,   -17,   318,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   350,   301,   217,   425,   107,   309,   195,   426,
      15,   427,   186,   196,   199,   425,    97,   183,   183,   426,
      16,   462,   255,   343,   107,   136,   137,   138,    12,   107,
     234,   183,   111,   397,   238,   338,   372,   222,   220,    35,
     384,    17,    21,   107,  -203,   311,   183,    96,   225,   183,
     315,   302,    32,    33,   373,   391,   395,    14,    36,   357,
     358,   415,   442,   443,   444,   240,   145,   146,   147,   148,
     149,   150,   369,   407,   107,   107,    25,    40,   248,    52,
      38,    53,    54,    55,   412,   -15,    39,   136,   137,   138,
     266,   174,   -16,   292,   183,    93,   422,   127,    41,   390,
     298,   253,   107,   450,   451,   452,   453,   454,   455,    42,
      59,    60,    61,    62,    63,   459,    65,    97,   136,   137,
      68,    69,    70,   100,   279,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   305,   246,   332,   183,   101,   456,
     183,   458,   118,   312,    74,   146,   147,   148,   149,   150,
     120,   107,   136,   137,   138,   345,   267,   328,   183,   103,
     146,   147,   148,   149,   150,   362,    29,    30,    49,   183,
      25,    50,    51,    52,   122,    53,    54,    55,    56,    57,
     297,   341,   183,   342,   366,   334,    58,   335,   107,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   347,
     348,   349,   123,   356,    59,    60,    61,    62,    63,    64,
      65,    66,    67,   363,    68,    69,    70,   367,   131,   371,
     442,   443,   132,   336,   303,   335,    71,    14,    32,    33,
      72,   396,    73,   158,   399,   379,   154,   155,    74,   403,
     304,   151,   152,   153,   417,   418,   451,   452,   453,   454,
     455,   411,   148,   149,   150,   453,   454,   455,   182,   183,
     177,   356,   451,   452,   453,   454,   455,   127,   375,   363,
     183,   376,   159,   183,   408,   377,   414,   183,   183,   193,
     194,   243,   244,   134,   135,   136,   137,   138,   180,   356,
     250,   251,    28,   430,   160,   161,   279,   172,   191,   178,
     218,    49,    28,    25,    50,    51,    52,   181,    53,    54,
      55,    56,    57,   226,   232,    28,   430,   235,   252,    58,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   237,   247,   264,   296,   179,    59,    60,    61,
      62,    63,    64,    65,    66,    67,   265,    68,    69,    70,
     295,   299,   186,   306,   307,   314,   325,   339,   134,    71,
     136,   137,   138,    72,   313,    73,   319,   320,   321,   323,
     324,    74,    49,   340,    25,    50,    51,    52,   326,    53,
      54,    55,    56,    57,   346,   330,   370,   316,   378,   380,
      58,   381,   183,   382,   383,   386,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   385,    59,    60,
      61,    62,    63,    64,    65,    66,    67,   388,    68,    69,
      70,   389,   394,   400,   402,   134,   135,   136,   137,   138,
      71,   406,   409,   404,    72,   421,    73,   420,   436,   431,
     437,   438,    74,    49,  -174,    25,    50,    51,    52,    38,
      53,    54,    55,    56,    57,   457,   477,   344,    11,    45,
     413,    58,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   260,   352,   401,   398,   249,    59,
      60,    61,    62,    63,    64,    65,    66,    67,   230,    68,
      69,    70,   419,   461,   331,   219,   190,   117,   236,   294,
       0,    71,     0,     0,     0,    72,     0,    73,     0,     0,
       0,     0,     0,    74,    49,  -176,    25,    50,    51,    52,
       0,    53,    54,    55,    56,    57,     0,     0,     0,     0,
       0,     0,    58,   269,     0,   270,   417,   418,     0,     0,
     271,   272,   273,   274,     0,   275,   276,   277,     0,     0,
      59,    60,    61,    62,    63,    64,    65,    66,    67,     0,
      68,    69,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,     0,     0,    72,   278,    73,     0,
       0,     0,   186,  -105,    74,   268,     0,    25,    50,    51,
     126,     0,    53,    54,    55,    56,    57,     0,     0,     0,
       0,     0,     0,    58,   269,     0,   270,     0,     0,     0,
       0,   271,   272,   273,   274,     0,   275,   276,   277,     0,
       0,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     127,    68,    69,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,     0,     0,    72,   278,    73,
       0,     0,     0,   186,   -71,    74,   268,     0,    25,    50,
      51,    52,     0,    53,    54,    55,    56,    57,     0,     0,
       0,     0,     0,     0,    58,   269,     0,   270,     0,     0,
       0,     0,   271,   272,   273,   274,     0,   275,   276,   277,
       0,     0,    59,    60,    61,    62,    63,    64,    65,    66,
      67,     0,    68,    69,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,     0,     0,    72,   278,
      73,     0,     0,     0,   186,   -71,    74,    49,     0,    25,
      50,    51,    52,     0,    53,    54,    55,    56,    57,     0,
       0,     0,     0,     0,     0,    58,   269,     0,   270,     0,
       0,     0,     0,   271,   272,   273,   274,     0,   275,   276,
     277,     0,     0,    59,    60,    61,    62,    63,    64,    65,
      66,    67,     0,    68,    69,    70,     0,     0,   134,   135,
     136,   137,   138,     0,     0,    71,     0,     0,     0,    72,
     278,    73,     0,     0,     0,   186,    49,    74,    25,    50,
      51,   126,     0,    53,    54,    55,    56,    57,     0,     0,
       0,     0,     0,     0,    58,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,     0,     0,     0,
     410,     0,    59,    60,    61,    62,    63,    64,    65,    66,
      67,   127,    68,    69,    70,     0,     0,     0,   442,   443,
     444,     0,     0,     0,    71,   128,     0,     0,    72,     0,
      73,     0,     0,     0,     0,    49,    74,    25,    50,    51,
      52,     0,    53,    54,    55,    56,    57,     0,     0,     0,
       0,     0,     0,    58,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,     0,   460,     0,
       0,    59,    60,    61,    62,    63,    64,    65,    66,    67,
       0,    68,    69,    70,     0,     0,     0,     0,   134,   135,
     136,   137,   138,    71,     0,     0,     0,    72,     0,    73,
    -169,     0,     0,     0,  -169,    74,    49,     0,    25,    50,
      51,   126,     0,    53,    54,    55,    56,    57,     0,     0,
       0,     0,     0,     0,    58,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,     0,     0,     0,
       0,     0,    59,    60,    61,    62,    63,    64,    65,    66,
      67,   127,    68,    69,    70,     0,     0,     0,     0,   134,
     135,   136,   137,   138,    71,     0,     0,     0,    72,   -97,
      73,   176,     0,     0,     0,    49,    74,    25,    50,    51,
      52,     0,    53,    54,    55,    56,    57,     0,     0,     0,
       0,     0,     0,    58,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,     0,     0,
       0,    59,    60,    61,    62,    63,    64,    65,    66,    67,
       0,    68,    69,    70,     0,     0,     0,     0,     0,   136,
     137,   138,     0,    71,     0,     0,     0,    72,     0,    73,
       0,     0,     0,     0,  -167,    74,    49,     0,    25,    50,
      51,    52,     0,    53,    54,    55,    56,    57,     0,     0,
       0,     0,     0,     0,    58,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,     0,     0,     0,     0,
       0,     0,    59,    60,    61,    62,    63,    64,    65,    66,
      67,     0,    68,    69,    70,     0,     0,     0,   442,   443,
     444,     0,     0,     0,    71,   128,     0,     0,    72,     0,
      73,     0,     0,     0,     0,    49,    74,    25,    50,    51,
      52,     0,    53,    54,    55,    56,    57,     0,     0,     0,
       0,     0,     0,    58,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,    63,    64,    65,    66,    67,
       0,    68,    69,    70,     0,     0,   442,   443,   444,     0,
       0,   221,     0,    71,     0,     0,     0,    72,     0,    73,
       0,     0,     0,     0,    49,    74,    25,    50,    51,    52,
       0,    53,    54,    55,    56,    57,     0,     0,     0,     0,
       0,     0,    58,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,     0,     0,     0,     0,     0,     0,     0,
      59,    60,    61,    62,    63,    64,    65,    66,    67,     0,
      68,    69,    70,     0,   442,   443,   444,     0,     0,     0,
       0,     0,    71,     0,     0,     0,    72,     0,    73,  -167,
       0,     0,     0,    49,    74,    25,    50,    51,    52,     0,
      53,    54,    55,    56,    57,     0,     0,     0,     0,     0,
       0,    58,   447,   448,   449,   450,   451,   452,   453,   454,
     455,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,    64,    65,    66,    67,     0,    68,
      69,    70,   136,   137,   138,     0,     0,     0,     0,     0,
       0,    71,     0,     0,     0,    72,   327,    73,     0,     0,
       0,     0,    49,    74,    25,    50,    51,    52,     0,    53,
      54,    55,    56,    57,     0,     0,     0,     0,     0,     0,
      58,   143,   144,   145,   146,   147,   148,   149,   150,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,    62,    63,    64,    65,    66,    67,     0,    68,    69,
      70,   442,   443,   444,     0,     0,     0,     0,     0,     0,
      71,     0,     0,     0,    72,   -97,    73,     0,     0,     0,
       0,    49,    74,    25,    50,    51,    52,     0,    53,    54,
      55,    56,    57,     0,     0,     0,     0,     0,     0,    58,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,    61,
      62,    63,    64,    65,    66,    67,     0,    68,    69,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,     0,     0,    72,     0,    73,   -97,     0,     0,     0,
      49,    74,    25,    50,    51,    52,     0,    53,    54,    55,
      56,    57,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,    62,
      63,    64,    65,    66,    67,     0,    68,    69,    70,   134,
     135,   136,   137,   138,     0,     0,     0,     0,    71,     0,
       0,     0,    72,     0,    73,     0,     0,     0,     0,     0,
      74,   134,   135,   136,   137,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   134,   135,
     136,   137,   138,   239,     0,     0,     0,     0,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     134,   135,   136,   137,   138,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   439,
       0,     0,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,     0,   439
};

static const yytype_int16 yycheck[] =
{
      15,    15,     9,    42,    10,    56,    57,   178,    16,    56,
      57,    26,   225,     6,    22,   119,     3,   121,   173,    58,
      25,    60,    61,    38,    38,    11,    12,    13,   323,   324,
      36,   241,    71,    72,    26,    35,    44,   192,     0,    78,
     345,    13,     6,    13,     8,     9,    37,     7,    66,    35,
      68,    59,    62,    46,     1,    37,    58,    96,     0,     3,
       4,   366,    67,    73,    66,     7,    73,    65,   223,   224,
       4,    69,    72,   283,    65,    19,    68,    70,    65,   118,
     251,   120,    46,    65,    25,    19,    72,   382,    95,    97,
       6,   101,   100,    65,    64,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   322,    25,   153,    59,   155,    64,   128,    63,
      36,    65,    69,   129,   132,    59,    37,    68,    68,    63,
      46,    65,    73,    73,   173,    16,    17,    18,    64,   178,
     179,    68,   181,   381,   183,   300,    73,   157,   156,    64,
     363,    67,    60,   192,    65,   259,    68,    13,   164,    68,
     264,    73,     8,     9,    73,   375,   379,     6,    68,     8,
       9,   409,    16,    17,    18,   185,    57,    58,    59,    60,
      61,    62,   337,   393,   223,   224,     3,     3,   198,     6,
      58,     8,     9,    10,   404,    69,    64,    16,    17,    18,
     239,    47,    69,   242,    68,    65,   416,    46,    69,    73,
     249,   221,   251,    57,    58,    59,    60,    61,    62,    13,
      37,    38,    39,    40,    41,   435,    43,    37,    16,    17,
      47,    48,    49,    37,   241,    54,    55,    56,    57,    58,
      59,    60,    61,    62,   254,    66,    64,    68,    65,   432,
      68,   434,    65,   261,    71,    58,    59,    60,    61,    62,
      65,   300,    16,    17,    18,   316,    66,   277,    68,   316,
      58,    59,    60,    61,    62,    64,     8,     9,     1,    68,
       3,     4,     5,     6,    65,     8,     9,    10,    11,    12,
      66,   301,    68,   303,   333,    66,    19,    68,   337,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   319,
     320,   321,     8,   323,    37,    38,    39,    40,    41,    42,
      43,    44,    45,   329,    47,    48,    49,   335,    65,   339,
      16,    17,    37,    66,    57,    68,    59,     6,     8,     9,
      63,   380,    65,    65,   383,   351,    67,    68,    71,   388,
      73,    11,    12,    13,    23,    24,    58,    59,    60,    61,
      62,   400,    60,    61,    62,    60,    61,    62,    67,    68,
      70,   381,    58,    59,    60,    61,    62,    46,    66,   385,
      68,    66,    65,    68,   394,    66,    66,    68,    68,     8,
       9,     8,     9,    14,    15,    16,    17,    18,    73,   409,
      67,    68,   417,   417,     8,     9,   413,    65,    46,    68,
      66,     1,   427,     3,     4,     5,     6,    68,     8,     9,
      10,    11,    12,    66,    66,   440,   440,    66,    65,    19,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    66,    66,    66,    65,    67,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    66,    47,    48,    49,
      66,    66,    69,    66,    66,    64,    64,    57,    14,    59,
      16,    17,    18,    63,    68,    65,    65,    65,    65,    65,
      65,    71,     1,    73,     3,     4,     5,     6,    64,     8,
       9,    10,    11,    12,    66,    70,    66,    68,    27,    13,
      19,    64,    68,    68,    31,    64,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    68,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    67,    47,    48,
      49,    66,    65,    13,    66,    14,    15,    16,    17,    18,
      59,    69,    64,    21,    63,    66,    65,    64,     4,    67,
       4,     4,    71,     1,    73,     3,     4,     5,     6,    58,
       8,     9,    10,    11,    12,    70,    67,   313,     4,    36,
     406,    19,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,   228,   323,   385,   382,    67,    37,
      38,    39,    40,    41,    42,    43,    44,    45,   172,    47,
      48,    49,   413,   440,   283,   155,   121,    62,   181,   244,
      -1,    59,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    71,     1,    73,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    -1,    22,    23,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    32,    33,    34,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    63,    64,    65,    -1,
      -1,    -1,    69,    70,    71,     1,    -1,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    22,    -1,    -1,    -1,
      -1,    27,    28,    29,    30,    -1,    32,    33,    34,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    63,    64,    65,
      -1,    -1,    -1,    69,    70,    71,     1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    22,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    -1,    32,    33,    34,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,    64,
      65,    -1,    -1,    -1,    69,    70,    71,     1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    22,    -1,
      -1,    -1,    -1,    27,    28,    29,    30,    -1,    32,    33,
      34,    -1,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    59,    -1,    -1,    -1,    63,
      64,    65,    -1,    -1,    -1,    69,     1,    71,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    16,    17,
      18,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,     1,    71,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    59,    -1,    -1,    -1,    63,    -1,    65,
      66,    -1,    -1,    -1,    70,    71,     1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    59,    -1,    -1,    -1,    63,    64,
      65,    26,    -1,    -1,    -1,     1,    71,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    16,
      17,    18,    -1,    59,    -1,    -1,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    71,     1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    -1,    -1,    16,    17,
      18,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,     1,    71,     3,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    -1,    16,    17,    18,    -1,
      -1,    57,    -1,    59,    -1,    -1,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,     1,    71,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    63,    -1,    65,    66,
      -1,    -1,    -1,     1,    71,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,
      -1,    -1,     1,    71,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      -1,     1,    71,     3,     4,     5,     6,    -1,     8,     9,
      10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    63,    -1,    65,    66,    -1,    -1,    -1,
       1,    71,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      71,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    14,    15,
      16,    17,    18,    68,    -1,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      14,    15,    16,    17,    18,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   427,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,    -1,    -1,
      -1,    -1,    -1,    -1,   462
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    75,    76,     0,     7,    78,    84,    87,    92,   101,
     102,   101,    64,    77,     6,    36,    46,    67,    95,    96,
     104,    60,    81,   105,   106,     3,    65,   159,   160,     8,
       9,    83,     8,     9,    83,    64,    68,   159,    58,    64,
       3,    69,    13,    85,    81,   105,    66,   159,    93,     1,
       4,     5,     6,     8,     9,    10,    11,    12,    19,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    47,    48,
      49,    59,    63,    65,    71,    79,    80,    83,   103,   136,
     144,   145,   146,   147,   151,   153,   154,   155,   158,   160,
     163,   170,   171,    65,    83,    90,    13,    37,   149,    65,
      37,    65,   144,   147,   144,   136,    83,   136,   138,   139,
     140,   136,   141,   142,   143,   135,   136,   170,    65,   152,
      65,   156,    65,     8,   136,   136,     6,    46,    60,    96,
     135,    65,    37,   136,    14,    15,    16,    17,    18,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    11,    12,    13,    67,    68,    35,    72,    65,    65,
       8,     9,    94,    96,    97,    98,    99,   100,    70,    96,
     136,    83,    65,   166,    47,    83,    26,    70,    68,    67,
      73,    68,    67,    68,   136,    65,    69,   107,   150,   136,
     150,    46,   165,     8,     9,   135,    81,    66,   148,    83,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,    66,   140,
      83,    57,   135,   164,   167,    81,    66,    26,    68,    91,
      99,   138,    66,   139,   136,    66,   143,    66,   136,    68,
     135,   109,    68,     8,     9,   138,    66,    66,   135,    67,
      67,    68,    65,   135,    25,    73,   138,   138,    94,    86,
      98,    81,    88,    89,    66,    66,   136,    66,     1,    20,
      22,    27,    28,    29,    30,    32,    33,    34,    64,    96,
     107,   108,   115,   116,   117,   119,   121,   125,   129,   135,
     137,   172,   136,   162,   162,    66,    65,    66,   136,    66,
     168,    25,    73,    57,    73,   135,    66,    66,     1,    64,
      82,   107,    83,    68,    64,   107,    68,   157,    64,    65,
      65,    65,   120,    65,    65,    64,    64,    64,   135,   110,
      70,   115,    64,    68,    66,    68,    66,   169,   138,    57,
      73,   135,   135,    73,    89,   144,    66,   135,   135,   135,
     116,    97,   112,   113,   127,   128,   135,     8,     9,   112,
     123,   124,    64,    81,   111,   114,   136,    83,   161,   138,
      66,   135,    73,    73,   157,    66,    66,    66,    27,    81,
      13,    64,    68,    31,    94,    68,    64,   157,    67,    66,
      73,   116,   130,   118,    65,    94,   136,   127,   123,   136,
      13,   114,    66,   136,    21,   173,    69,   116,   135,    64,
      66,   136,   116,   109,    66,   127,   126,    23,    24,   132,
      64,    66,   116,     4,    19,    59,    63,    65,   133,   134,
     159,    67,   116,   131,   132,   122,     4,     4,     4,   134,
      25,    67,    16,    17,    18,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,   131,    70,   131,   116,
      66,   133,    65,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,    67
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    76,    76,    77,    77,    78,    79,    80,
      81,    81,    82,    82,    82,    83,    83,    85,    86,    84,
      84,    84,    84,    84,    87,    88,    89,    89,    90,    91,
      90,    93,    92,    94,    94,    95,    95,    95,    96,    97,
      98,    98,    98,    99,    99,    99,   100,   100,   101,   101,
     102,   103,   104,   104,   105,   105,   106,   106,   107,   108,
     108,   108,   109,   110,   109,   111,   111,   112,   113,   114,
     114,   115,   115,   115,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   118,   117,   120,   119,   122,   121,
     123,   123,   123,   124,   124,   126,   125,   127,   127,   128,
     128,   130,   129,   131,   131,   131,   132,   132,   132,   133,
     133,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     135,   135,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   137,   137,   138,   138,   138,
     139,   139,   140,   140,   141,   141,   141,   142,   142,   143,
     144,   145,   145,   146,   146,   147,   147,   147,   147,   148,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   149,   147,   147,   147,   147,   147,   147,
     150,   150,   152,   151,   153,   154,   156,   155,   157,   157,
     158,   159,   159,   159,   160,   160,   161,   162,   162,   164,
     163,   165,   163,   163,   163,   166,   163,   167,   163,   168,
     163,   169,   163,   170,   170,   171,   171,   171,   171,   172,
     173,   173
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     0,     0,     1,     4,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     0,     0,     9,
       3,     1,     1,     1,     2,     2,     1,     3,     0,     0,
       5,     0,     7,     1,     1,     1,     2,     2,     1,     1,
       2,     3,     1,     0,     1,     2,     1,     3,     0,     2,
       2,     4,     1,     0,     1,     3,     2,     4,     4,     1,
       1,     1,     0,     0,     5,     2,     4,     3,     3,     1,
       3,     0,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     0,     6,     0,     8,     0,    10,
       1,     1,     1,     1,     3,     0,     8,     0,     1,     1,
       1,     0,    10,     2,     2,     0,     3,     5,     2,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     2,
       1,     3,     3,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     3,     0,     1,     2,
       1,     2,     1,     3,     0,     1,     2,     1,     3,     3,
       1,     1,     2,     3,     2,     1,     1,     1,     1,     0,
       5,     3,     6,     7,     8,     7,     5,     6,     5,     4,
       1,     3,     1,     0,     6,     1,     5,     4,     4,     4,
       1,     3,     0,     3,     7,     9,     0,     3,     0,     3,
       1,     1,     3,     3,     1,     2,     3,     0,     3,     0,
       5,     0,     5,     6,     6,     0,     5,     0,     5,     0,
       7,     0,     8,     3,     3,     1,     2,     3,     3,     6,
       0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
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
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 212 "grammar.y" /* yacc.c:1646  */
    {
#line 229 "grammar.y.pre"
	    comp_trees[TREE_MAIN] = (yyval.node);
	}
#line 1991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 220 "grammar.y" /* yacc.c:1646  */
    {
#line 236 "grammar.y.pre"
	    CREATE_TWO_VALUES((yyval.node), 0, (yyvsp[-2].node), (yyvsp[-1].node));
	}
#line 2000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 225 "grammar.y" /* yacc.c:1646  */
    {
#line 240 "grammar.y.pre"
	    (yyval.node) = 0;
	}
#line 2009 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 234 "grammar.y" /* yacc.c:1646  */
    {
#line 248 "grammar.y.pre"

		yywarn("Extra ';'. Ignored.");
	    }
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 244 "grammar.y" /* yacc.c:1646  */
    {
#line 257 "grammar.y.pre"
		object_t *ob;
		inherit_t inherit;
		int initializer;
		
		(yyvsp[-3].type) |= global_modifiers;

		
		if (!((yyvsp[-3].type) & DECL_ACCESS)) (yyvsp[-3].type) |= DECL_PUBLIC;
		
		if (var_defined)
		    yyerror("Illegal to inherit after defining global variables.");
		ob = find_object2((yyvsp[-1].string));
		if (ob == 0) {
		    inherit_file = alloc_cstring((yyvsp[-1].string), "inherit");
		    /* Return back to load_object() */
		    YYACCEPT;
		}
		scratch_free((yyvsp[-1].string));
		inherit.prog = ob->prog;

		if (mem_block[A_INHERITS].current_size){
		    inherit_t *prev_inherit = INHERIT(NUM_INHERITS - 1);
		    
		    inherit.function_index_offset 
			= prev_inherit->function_index_offset
			+ prev_inherit->prog->num_functions_defined
			+ prev_inherit->prog->last_inherited;
		    if (prev_inherit->prog->num_functions_defined &&
			prev_inherit->prog->function_table[prev_inherit->prog->num_functions_defined - 1].name[0] == '#')
			inherit.function_index_offset--;
		} else inherit.function_index_offset = 0;
		
		inherit.variable_index_offset =
		    mem_block[A_VAR_TEMP].current_size /
		    sizeof (variable_t);
		inherit.type_mod = (yyvsp[-3].type);
		add_to_mem_block(A_INHERITS, (char *)&inherit, sizeof inherit);

		/* The following has to come before copy_vars - Sym */
		copy_structures(ob->prog);
		copy_variables(ob->prog, (yyvsp[-3].type));
		initializer = copy_functions(ob->prog, (yyvsp[-3].type));
		if (initializer >= 0) {
		    parse_node_t *node, *newnode;
		    /* initializer is an index into the object we're
		       inheriting's function table; this finds the
		       appropriate entry in our table and generates
		       a call to it */
		    node = new_node_no_line();
		    node->kind = NODE_CALL_2;
		    node->r.expr = 0;
		    node->v.number = F_CALL_INHERITED;
		    node->l.number = initializer | ((NUM_INHERITS - 1) << 16);
		    node->type = TYPE_ANY;
		    
		    /* The following illustrates a distinction between */
		    /* macros and funcs...newnode is needed here - Sym */
		    newnode = comp_trees[TREE_INIT];
		    CREATE_TWO_VALUES(comp_trees[TREE_INIT],0, newnode, node);
		    comp_trees[TREE_INIT] = pop_value(comp_trees[TREE_INIT]);
		    
		} 
		(yyval.node) = 0;
	    }
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 314 "grammar.y" /* yacc.c:1646  */
    {
#line 342 "grammar.y.pre"
		CREATE_REAL((yyval.node), (yyvsp[0].real));
	    }
#line 2099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 322 "grammar.y" /* yacc.c:1646  */
    {
#line 349 "grammar.y.pre"
		CREATE_NUMBER((yyval.node), (yyvsp[0].number));
	    }
#line 2108 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 330 "grammar.y" /* yacc.c:1646  */
    {
#line 356 "grammar.y.pre"
		(yyval.type) = 0;
	    }
#line 2117 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 335 "grammar.y" /* yacc.c:1646  */
    {
#line 360 "grammar.y.pre"
		(yyval.type) = TYPE_MOD_ARRAY;
	    }
#line 2126 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 343 "grammar.y" /* yacc.c:1646  */
    {
#line 367 "grammar.y.pre"
		(yyval.node) = (yyvsp[0].decl).node;
		if (!(yyval.node)) {
		    CREATE_RETURN((yyval.node), 0);
		}
            }
#line 2138 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 351 "grammar.y" /* yacc.c:1646  */
    {
#line 374 "grammar.y.pre"
		(yyval.node) = 0;
	    }
#line 2147 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 356 "grammar.y" /* yacc.c:1646  */
    {
#line 378 "grammar.y.pre"
		(yyval.node) = 0;
	    }
#line 2156 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 364 "grammar.y" /* yacc.c:1646  */
    {
#line 385 "grammar.y.pre"
		(yyval.string) = scratch_copy((yyvsp[0].ihe)->name);
	    }
#line 2165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 373 "grammar.y" /* yacc.c:1646  */
    {
#line 393 "grammar.y.pre"
		int flags;
		
		flags = ((yyvsp[-2].type) >> 16);
		
		flags |= global_modifiers;


		if (!(flags & DECL_ACCESS)) flags |= DECL_PUBLIC;
                (yyvsp[-2].type) = (flags << 16) | ((yyvsp[-2].type) & 0xffff);
		/* Handle type checking here so we know whether to typecheck
		   'argument' */
		if ((yyvsp[-2].type) & 0xffff) {
                    exact_types = 0;
		} else {
		    if (pragmas & PRAGMA_STRICT_TYPES) {
			if (strcmp((yyvsp[0].string), "create") != 0)
			    yyerror("\"#pragma strict_types\" requires type of function");
			else
			    exact_types = TYPE_VOID; /* default for create() */
		    } else
			exact_types = 0;
		}
	    }
#line 2195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 399 "grammar.y" /* yacc.c:1646  */
    {
#line 444 "grammar.y.pre"
		char *p = (yyvsp[-4].string);
		(yyvsp[-4].string) = make_shared_string((yyvsp[-4].string));
		scratch_free(p);

		/* If we had nested functions, we would need to check */
		/* here if we have enough space for locals */
		
		/*
		 * Define a prototype. If it is a real function, then the
		 * prototype will be replaced below.
		 */

		(yyval.number) = FUNC_PROTOTYPE;
		if ((yyvsp[-1].argument).flags & ARG_IS_VARARGS) {
		    (yyval.number) |= (FUNC_TRUE_VARARGS | FUNC_VARARGS);
		}
		(yyval.number) |= ((yyvsp[-6].type) >> 16);

		define_new_function((yyvsp[-4].string), (yyvsp[-1].argument).num_arg, 0, (yyval.number), ((yyvsp[-6].type) & 0xffff)| (yyvsp[-5].type));
		/* This is safe since it is guaranteed to be in the
		   function table, so it can't be dangling */
		free_string((yyvsp[-4].string)); 
		context = 0;
	    }
#line 2226 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 426 "grammar.y" /* yacc.c:1646  */
    {
#line 470 "grammar.y.pre"
		/* Either a prototype or a block */
		if ((yyvsp[0].node)) {
		    int fun;

		    (yyvsp[-1].number) &= ~FUNC_PROTOTYPE;
		    if ((yyvsp[0].node)->kind != NODE_RETURN &&
			((yyvsp[0].node)->kind != NODE_TWO_VALUES
			 || (yyvsp[0].node)->r.expr->kind != NODE_RETURN)) {
			parse_node_t *replacement;
			CREATE_STATEMENTS(replacement, (yyvsp[0].node), 0);
			CREATE_RETURN(replacement->r.expr, 0);
			(yyvsp[0].node) = replacement;
		    }

		    fun = define_new_function((yyvsp[-6].string), (yyvsp[-3].argument).num_arg, 
					      max_num_locals - (yyvsp[-3].argument).num_arg,
					      (yyvsp[-1].number), ((yyvsp[-8].type) & 0xffff) | (yyvsp[-7].type));
		    if (fun != -1) {
			(yyval.node) = new_node_no_line();
			(yyval.node)->kind = NODE_FUNCTION;
			(yyval.node)->v.number = fun;
			(yyval.node)->l.number = max_num_locals;
			(yyval.node)->r.expr = (yyvsp[0].node);
		    } else 
			(yyval.node) = 0;
		} else
		    (yyval.node) = 0;
		free_all_local_names(!!(yyvsp[0].node));
	    }
#line 2262 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 458 "grammar.y" /* yacc.c:1646  */
    {
#line 501 "grammar.y.pre"
		if (!((yyvsp[-2].type) & ~(DECL_MODS)) && (pragmas & PRAGMA_STRICT_TYPES))
		    yyerror("Missing type for global variable declaration");
		(yyval.node) = 0;
	    }
#line 2273 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 470 "grammar.y" /* yacc.c:1646  */
    {
#line 512 "grammar.y.pre"
		if (!(yyvsp[-1].type)) 
		    yyerror("modifier list may not be empty.");
		
		if ((yyvsp[-1].type) & FUNC_VARARGS) {
		    yyerror("Illegal modifier 'varargs' in global modifier list.");
		    (yyvsp[-1].type) &= ~FUNC_VARARGS;
		}

		if (!((yyvsp[-1].type) & DECL_ACCESS)) (yyvsp[-1].type) |= DECL_PUBLIC;
		global_modifiers = (yyvsp[-1].type);
		(yyval.node) = 0;
	    }
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 488 "grammar.y" /* yacc.c:1646  */
    {
#line 529 "grammar.y.pre"
		/* At this point, the current_type here is only a basic_type */
		/* and cannot be unused yet - Sym */
		
		if (current_type == TYPE_VOID)
		    yyerror("Illegal to declare class member of type void.");
		add_local_name((yyvsp[0].string), current_type | (yyvsp[-1].type));
		scratch_free((yyvsp[0].string));
	    }
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 508 "grammar.y" /* yacc.c:1646  */
    {
#line 548 "grammar.y.pre"
	      current_type = (yyvsp[0].type);
	  }
#line 2316 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 517 "grammar.y" /* yacc.c:1646  */
    {
#line 556 "grammar.y.pre"
		ident_hash_elem_t *ihe;

		ihe = find_or_add_ident(
			   PROG_STRING((yyval.number) = store_prog_string((yyvsp[-1].string))),
			   FOA_GLOBAL_SCOPE);
		if (ihe->dn.class_num == -1) {
		    ihe->sem_value++;
		    ihe->dn.class_num = mem_block[A_CLASS_DEF].current_size / sizeof(class_def_t);
		    if (ihe->dn.class_num > CLASS_NUM_MASK){
			char buf[256];
			char *p;

			p = buf;
			sprintf(p, "Too many classes, max is %d.\n", CLASS_NUM_MASK + 1);
			yyerror(buf);
		    }

		    scratch_free((yyvsp[-1].string));
		    (yyvsp[-2].ihe) = 0;
		}
		else {
		    (yyvsp[-2].ihe) = ihe;
		}
	    }
#line 2347 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 544 "grammar.y" /* yacc.c:1646  */
    {
#line 582 "grammar.y.pre"
		class_def_t *sd;
		class_member_entry_t *sme;
		int i, raise_error = 0;
		
		/* check for a redefinition */
		if ((yyvsp[-5].ihe) != 0) {
		    sd = CLASS((yyvsp[-5].ihe)->dn.class_num);
		    if (sd->size != current_number_of_locals)
			raise_error = 1;
		    else {
			i = sd->size;
			sme = (class_member_entry_t *)mem_block[A_CLASS_MEMBER].block + sd->index;
			while (i--) {
			    /* check for matching names and types */
			    if (strcmp(PROG_STRING(sme[i].name), locals_ptr[i].ihe->name) != 0 ||
				sme[i].type != (type_of_locals_ptr[i] & ~LOCAL_MODS)) {
				raise_error = 1;
				break;
			    }
			}
		    }
		}

		if (raise_error) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Illegal to redefine class ");
		    p = strput(p, end, PROG_STRING((yyval.number)));
		    yyerror(buf);
		} else {
		    sd = (class_def_t *)allocate_in_mem_block(A_CLASS_DEF, sizeof(class_def_t));
		    i = sd->size = current_number_of_locals;
		    sd->index = mem_block[A_CLASS_MEMBER].current_size / sizeof(class_member_entry_t);
		    sd->name = (yyvsp[-2].number);

		    sme = (class_member_entry_t *)allocate_in_mem_block(A_CLASS_MEMBER, sizeof(class_member_entry_t) * current_number_of_locals);

		    while (i--) {
			sme[i].name = store_prog_string(locals_ptr[i].ihe->name);
			sme[i].type = type_of_locals_ptr[i] & ~LOCAL_MODS;
		    }
		}

		free_all_local_names(0);
		(yyval.node) = 0;
	    }
#line 2402 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 599 "grammar.y" /* yacc.c:1646  */
    {
#line 636 "grammar.y.pre"
		if ((yyvsp[0].ihe)->dn.local_num != -1) {
		    char buff[256];
		    char *end = EndOf(buff);
		    char *p;
		    
		    p = strput(buff, end, "Illegal to redeclare local name '");
		    p = strput(p, end, (yyvsp[0].ihe)->name);
		    p = strput(p, end, "'");
		    yyerror(buff);
		}
		(yyval.string) = scratch_copy((yyvsp[0].ihe)->name);
	    }
#line 2421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 618 "grammar.y" /* yacc.c:1646  */
    {
#line 654 "grammar.y.pre"
		if ((yyvsp[0].ihe)->dn.class_num == -1) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "Undefined class '");
		    p = strput(p, end, (yyvsp[0].ihe)->name);
		    p = strput(p, end, "'");
		    yyerror(buf);
		    (yyval.type) = TYPE_ANY;
		} else {
		    (yyval.type) = (yyvsp[0].ihe)->dn.class_num | TYPE_MOD_CLASS;
		}
	    }
#line 2442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 635 "grammar.y" /* yacc.c:1646  */
    {
#line 670 "grammar.y.pre"
		char buf[256];
		char *end = EndOf(buf);
		char *p;

		p = strput(buf, end, "Undefined class '");
		p = strput(p, end, (yyvsp[0].string));
		p = strput(p, end, "'");
		yyerror(buf);
		(yyval.type) = TYPE_ANY;
	    }
#line 2459 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 660 "grammar.y" /* yacc.c:1646  */
    {
#line 715 "grammar.y.pre"
                (yyval.number) = (yyvsp[-1].type) | (yyvsp[0].type);
		if ((yyvsp[-1].type) != TYPE_VOID)
		    add_local_name("", (yyvsp[-1].type) | (yyvsp[0].type));
            }
#line 2470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 667 "grammar.y" /* yacc.c:1646  */
    {
#line 721 "grammar.y.pre"
		if ((yyvsp[-2].type) == TYPE_VOID)
		    yyerror("Illegal to declare argument of type void.");
                add_local_name((yyvsp[0].string), (yyvsp[-2].type) | (yyvsp[-1].type));
		scratch_free((yyvsp[0].string));
                (yyval.number) = (yyvsp[-2].type) | (yyvsp[-1].type);
	    }
#line 2483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 676 "grammar.y" /* yacc.c:1646  */
    {
#line 729 "grammar.y.pre"
		if (exact_types) {
		    yyerror("Missing type for argument");
		}
		add_local_name((yyvsp[0].string), TYPE_ANY);
		scratch_free((yyvsp[0].string));
		(yyval.number) = TYPE_ANY;
            }
#line 2497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 689 "grammar.y" /* yacc.c:1646  */
    {
#line 741 "grammar.y.pre"
		(yyval.argument).num_arg = 0;
                (yyval.argument).flags = 0;
	    }
#line 2507 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 696 "grammar.y" /* yacc.c:1646  */
    {
#line 747 "grammar.y.pre"
		int x = type_of_locals_ptr[max_num_locals-1];
		int lt = x & ~LOCAL_MODS;
		
		(yyval.argument) = (yyvsp[-1].argument);
		(yyval.argument).flags |= ARG_IS_VARARGS;

		if (x & LOCAL_MOD_REF) {
		    yyerror("Variable to hold remainder of args may not be a reference");
		    x &= ~LOCAL_MOD_REF;
		}
		if (lt != TYPE_ANY && !(lt & TYPE_MOD_ARRAY))
		    yywarn("Variable to hold remainder of arguments should be an array.");
	    }
#line 2527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 715 "grammar.y" /* yacc.c:1646  */
    {
#line 765 "grammar.y.pre"
		if (((yyvsp[0].number) & TYPE_MASK) == TYPE_VOID && !((yyvsp[0].number) & TYPE_MOD_CLASS)) {
		    if ((yyvsp[0].number) & ~TYPE_MASK)
			yyerror("Illegal to declare argument of type void.");
		    (yyval.argument).num_arg = 0;
		} else {
		    (yyval.argument).num_arg = 1;
		}
                (yyval.argument).flags = 0;
	    }
#line 2543 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 727 "grammar.y" /* yacc.c:1646  */
    {
#line 776 "grammar.y.pre"
		if (!(yyval.argument).num_arg)    /* first arg was void w/no name */
		    yyerror("argument of type void must be the only argument.");
		if (((yyvsp[0].number) & TYPE_MASK) == TYPE_VOID && !((yyvsp[0].number) & TYPE_MOD_CLASS))
		    yyerror("Illegal to declare argument of type void.");

                (yyval.argument) = (yyvsp[-2].argument);
		(yyval.argument).num_arg++;
	    }
#line 2558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 741 "grammar.y" /* yacc.c:1646  */
    {
#line 789 "grammar.y.pre"
		(yyval.type) = 0;
	    }
#line 2567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 746 "grammar.y" /* yacc.c:1646  */
    {
#line 793 "grammar.y.pre"
		
		(yyval.type) = (yyvsp[-1].type) | (yyvsp[0].type);
		
	    }
#line 2578 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 756 "grammar.y" /* yacc.c:1646  */
    {
#line 819 "grammar.y.pre"
		(yyval.type) = ((yyvsp[-1].type) << 16) | (yyvsp[0].type);
		current_type = (yyval.type);
	    }
#line 2588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 765 "grammar.y" /* yacc.c:1646  */
    {
#line 827 "grammar.y.pre"
		(yyval.type) = (yyvsp[-2].type) | (yyvsp[-1].type);
	    }
#line 2597 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 774 "grammar.y" /* yacc.c:1646  */
    {
#line 835 "grammar.y.pre"
		(yyval.type) = TYPE_UNKNOWN;
	    }
#line 2606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 787 "grammar.y" /* yacc.c:1646  */
    {
#line 847 "grammar.y.pre"
		if (current_type & (FUNC_VARARGS << 16)){
		    yyerror("Illegal to declare varargs variable.");
		    current_type &= ~(FUNC_VARARGS << 16);
		}
		/* Now it is ok to merge the two
		 * remember that class_num and varargs was the reason for above
		 * Do the merging once only per row of decls
		 */

		if (current_type & 0xffff0000){
		    current_type = (current_type >> 16) | (current_type & 0xffff);
		}

		current_type |= global_modifiers;

		if (!(current_type & DECL_ACCESS)) current_type |= DECL_PUBLIC;

		if ((current_type & ~DECL_MODS) == TYPE_VOID)
		    yyerror("Illegal to declare global variable of type void.");

		define_new_variable((yyvsp[0].string), current_type | (yyvsp[-1].type));
		scratch_free((yyvsp[0].string));
	    }
#line 2636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 813 "grammar.y" /* yacc.c:1646  */
    {
#line 872 "grammar.y.pre"
		parse_node_t *expr, *newnode;
		int type;

		if (current_type & (FUNC_VARARGS << 16)){
		    yyerror("Illegal to declare varargs variable.");
		    current_type &= ~(FUNC_VARARGS << 16);
		}
		
		if (current_type & 0xffff0000){
		    current_type = (current_type >> 16) | (current_type & 0xffff);
		}

		current_type |= global_modifiers;

		if (!(current_type & DECL_ACCESS)) current_type |= DECL_PUBLIC;

		if ((current_type & ~DECL_MODS) == TYPE_VOID)
		    yyerror("Illegal to declare global variable of type void.");

		if ((yyvsp[-1].number) != F_ASSIGN)
		    yyerror("Only '=' is legal in initializers.");

		/* ignore current_type == 0, which gets a missing type error
		   later anyway */
		if (current_type) {
		    type = (current_type | (yyvsp[-3].type)) & ~DECL_MODS;
		    if ((current_type & ~DECL_MODS) == TYPE_VOID)
			yyerror("Illegal to declare global variable of type void.");
		    if (!compatible_types(type, (yyvsp[0].node)->type)) {
			char buff[256];
			char *end = EndOf(buff);
			char *p;
			
			p = strput(buff, end, "Type mismatch ");
			p = get_two_types(p, end, type, (yyvsp[0].node)->type);
			p = strput(p, end, " when initializing ");
			p = strput(p, end, (yyvsp[-2].string));
			yyerror(buff);
		    }
		}
		(yyvsp[0].node) = do_promotions((yyvsp[0].node), type);

		CREATE_BINARY_OP(expr, F_VOID_ASSIGN, 0, (yyvsp[0].node), 0);
		CREATE_OPCODE_1(expr->r.expr, F_GLOBAL_LVALUE, 0,
				define_new_variable((yyvsp[-2].string), current_type | (yyvsp[-3].type)));
		newnode = comp_trees[TREE_INIT];
		CREATE_TWO_VALUES(comp_trees[TREE_INIT], 0,
				  newnode, expr);
		scratch_free((yyvsp[-2].string));
	    }
#line 2693 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 869 "grammar.y" /* yacc.c:1646  */
    {
#line 927 "grammar.y.pre"
		if ((yyvsp[-2].decl).node && (yyvsp[-1].node)) {
		    CREATE_STATEMENTS((yyval.decl).node, (yyvsp[-2].decl).node, (yyvsp[-1].node));
		} else (yyval.decl).node = ((yyvsp[-2].decl).node ? (yyvsp[-2].decl).node : (yyvsp[-1].node));
                (yyval.decl).num = (yyvsp[-2].decl).num;
            }
#line 2705 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 882 "grammar.y" /* yacc.c:1646  */
    {
#line 939 "grammar.y.pre"
                (yyval.decl).node = 0;
                (yyval.decl).num = 0;
            }
#line 2715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 888 "grammar.y" /* yacc.c:1646  */
    {
#line 944 "grammar.y.pre"
		if ((yyvsp[0].type) == TYPE_VOID)
		    yyerror("Illegal to declare local variable of type void.");
                /* can't do this in basic_type b/c local_name_list contains
                 * expr0 which contains cast which contains basic_type
                 */
                current_type = (yyvsp[0].type);
            }
#line 2729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 898 "grammar.y" /* yacc.c:1646  */
    {
#line 953 "grammar.y.pre"
                if ((yyvsp[-4].decl).node && (yyvsp[-1].decl).node) {
		    CREATE_STATEMENTS((yyval.decl).node, (yyvsp[-4].decl).node, (yyvsp[-1].decl).node);
                } else (yyval.decl).node = ((yyvsp[-4].decl).node ? (yyvsp[-4].decl).node : (yyvsp[-1].decl).node);
                (yyval.decl).num = (yyvsp[-4].decl).num + (yyvsp[-1].decl).num;
            }
#line 2741 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 910 "grammar.y" /* yacc.c:1646  */
    {
#line 964 "grammar.y.pre"
		if (current_type & LOCAL_MOD_REF) {
		    yyerror("Illegal to declare local variable as reference");
		    current_type &= ~LOCAL_MOD_REF;
		}
		add_local_name((yyvsp[0].string), current_type | (yyvsp[-1].type) | LOCAL_MOD_UNUSED);

		scratch_free((yyvsp[0].string));
		(yyval.node) = 0;
	    }
#line 2757 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 922 "grammar.y" /* yacc.c:1646  */
    {
#line 975 "grammar.y.pre"
		int type = (current_type | (yyvsp[-3].type)) & ~DECL_MODS;

		if (current_type & LOCAL_MOD_REF) {
		    yyerror("Illegal to declare local variable as reference");
		    current_type &= ~LOCAL_MOD_REF;
		    type &= ~LOCAL_MOD_REF;
		}

		if ((yyvsp[-1].number) != F_ASSIGN)
		    yyerror("Only '=' is allowed in initializers.");
		if (!compatible_types((yyvsp[0].node)->type, type)) {
		    char buff[256];
		    char *end = EndOf(buff);
		    char *p;
		    
		    p = strput(buff, end, "Type mismatch ");
		    p = get_two_types(p, end, type, (yyvsp[0].node)->type);
		    p = strput(p, end, " when initializing ");
		    p = strput(p, end, (yyvsp[-2].string));

		    yyerror(buff);
		}
		
		(yyvsp[0].node) = do_promotions((yyvsp[0].node), type);

		CREATE_UNARY_OP_1((yyval.node), F_VOID_ASSIGN_LOCAL, 0, (yyvsp[0].node),
				  add_local_name((yyvsp[-2].string), current_type | (yyvsp[-3].type) | LOCAL_MOD_UNUSED));
		scratch_free((yyvsp[-2].string));
	    }
#line 2793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 957 "grammar.y" /* yacc.c:1646  */
    {
#line 1009 "grammar.y.pre"
		if ((yyvsp[-2].type) == TYPE_VOID)
		    yyerror("Illegal to declare local variable of type void.");

		(yyval.number) = add_local_name((yyvsp[0].string), (yyvsp[-2].type) | (yyvsp[-1].type));
		scratch_free((yyvsp[0].string));
	    }
#line 2806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 969 "grammar.y" /* yacc.c:1646  */
    {
#line 1020 "grammar.y.pre"
                int type = type_of_locals_ptr[(yyvsp[-2].number)];

		if (type & LOCAL_MOD_REF) {
		    yyerror("Illegal to declare local variable as reference");
		    type_of_locals_ptr[(yyvsp[-2].number)] &= ~LOCAL_MOD_REF;
		}
		type &= ~LOCAL_MODS;

		if ((yyvsp[-1].number) != F_ASSIGN)
		    yyerror("Only '=' is allowed in initializers.");
		if (!compatible_types((yyvsp[0].node)->type, type)) {
		    char buff[256];
		    char *end = EndOf(buff);
		    char *p;
		    
		    p = strput(buff, end, "Type mismatch ");
		    p = get_two_types(p, end, type, (yyvsp[0].node)->type);
		    p = strput(p, end, " when initializing.");
		    yyerror(buff);
		}

		(yyvsp[0].node) = do_promotions((yyvsp[0].node), type);

		/* this is an expression */
		CREATE_BINARY_OP((yyval.node), F_ASSIGN, 0, (yyvsp[0].node), 0);
                CREATE_OPCODE_1((yyval.node)->r.expr, F_LOCAL_LVALUE, 0, (yyvsp[-2].number));
	    }
#line 2840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1002 "grammar.y" /* yacc.c:1646  */
    {
#line 1052 "grammar.y.pre"
                (yyval.decl).node = (yyvsp[0].node);
                (yyval.decl).num = 1;
            }
#line 2850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1008 "grammar.y" /* yacc.c:1646  */
    {
#line 1057 "grammar.y.pre"
                if ((yyvsp[-2].node) && (yyvsp[0].decl).node) {
		    CREATE_STATEMENTS((yyval.decl).node, (yyvsp[-2].node), (yyvsp[0].decl).node);
                } else (yyval.decl).node = ((yyvsp[-2].node) ? (yyvsp[-2].node) : (yyvsp[0].decl).node);
                (yyval.decl).num = 1 + (yyvsp[0].decl).num;
            }
#line 2862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1019 "grammar.y" /* yacc.c:1646  */
    {
#line 1067 "grammar.y.pre"
		(yyval.node) = 0;
	    }
#line 2871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1024 "grammar.y" /* yacc.c:1646  */
    {
#line 1071 "grammar.y.pre"
		if ((yyvsp[-1].node) && (yyvsp[0].node)) {
		    CREATE_STATEMENTS((yyval.node), (yyvsp[-1].node), (yyvsp[0].node));
		} else (yyval.node) = ((yyvsp[-1].node) ? (yyvsp[-1].node) : (yyvsp[0].node));
            }
#line 2882 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1031 "grammar.y" /* yacc.c:1646  */
    {
#line 1077 "grammar.y.pre"
		(yyval.node) = 0;
            }
#line 2891 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1039 "grammar.y" /* yacc.c:1646  */
    {
#line 1084 "grammar.y.pre"
		(yyval.node) = pop_value((yyvsp[-1].node));
#ifdef DEBUG
		{
		    parse_node_t *replacement;
		    CREATE_STATEMENTS(replacement, (yyval.node), 0);
		    CREATE_OPCODE(replacement->r.expr, F_BREAK_POINT, 0);
		    (yyval.node) = replacement;
		}
#endif
	    }
#line 2908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1057 "grammar.y" /* yacc.c:1646  */
    {
#line 1101 "grammar.y.pre"
                (yyval.node) = (yyvsp[0].decl).node;
                pop_n_locals((yyvsp[0].decl).num);
            }
#line 2918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1063 "grammar.y" /* yacc.c:1646  */
    {
#line 1106 "grammar.y.pre"
		(yyval.node) = 0;
	    }
#line 2927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1068 "grammar.y" /* yacc.c:1646  */
    {
#line 1110 "grammar.y.pre"
		if (context & SPECIAL_CONTEXT) {
		    yyerror("Cannot break out of catch { } or time_expression { }");
		    (yyval.node) = 0;
		} else
		if (context & SWITCH_CONTEXT) {
		    CREATE_CONTROL_JUMP((yyval.node), CJ_BREAK_SWITCH);
		} else
		if (context & LOOP_CONTEXT) {
		    CREATE_CONTROL_JUMP((yyval.node), CJ_BREAK);
		    if (context & LOOP_FOREACH) {
			parse_node_t *replace;
			CREATE_STATEMENTS(replace, 0, (yyval.node));
			CREATE_OPCODE(replace->l.expr, F_EXIT_FOREACH, 0);
			(yyval.node) = replace;
		    }
		} else {
		    yyerror("break statement outside loop");
		    (yyval.node) = 0;
		}
	    }
#line 2954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1091 "grammar.y" /* yacc.c:1646  */
    {
#line 1132 "grammar.y.pre"
		if (context & SPECIAL_CONTEXT)
		    yyerror("Cannot continue out of catch { } or time_expression { }");
		else
		if (!(context & LOOP_CONTEXT))
		    yyerror("continue statement outside loop");
		CREATE_CONTROL_JUMP((yyval.node), CJ_CONTINUE);
	    }
#line 2968 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1104 "grammar.y" /* yacc.c:1646  */
    {
#line 1144 "grammar.y.pre"
		(yyvsp[-3].number) = context;
		context = LOOP_CONTEXT;
	    }
#line 2978 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1110 "grammar.y" /* yacc.c:1646  */
    {
#line 1149 "grammar.y.pre"
		CREATE_LOOP((yyval.node), 1, (yyvsp[0].node), 0, optimize_loop_test((yyvsp[-3].node)));
		context = (yyvsp[-5].number);
	    }
#line 2988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1119 "grammar.y" /* yacc.c:1646  */
    {
#line 1157 "grammar.y.pre"
		(yyvsp[0].number) = context;
		context = LOOP_CONTEXT;
	    }
#line 2998 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1125 "grammar.y" /* yacc.c:1646  */
    {
#line 1162 "grammar.y.pre"
		CREATE_LOOP((yyval.node), 0, (yyvsp[-5].node), 0, optimize_loop_test((yyvsp[-2].node)));
		context = (yyvsp[-7].number);
	    }
#line 3008 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1134 "grammar.y" /* yacc.c:1646  */
    {
#line 1170 "grammar.y.pre"
		(yyvsp[-5].decl).node = pop_value((yyvsp[-5].decl).node);
		(yyvsp[-7].number) = context;
		context = LOOP_CONTEXT;
	    }
#line 3019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1141 "grammar.y" /* yacc.c:1646  */
    {
#line 1176 "grammar.y.pre"
		(yyval.decl).num = (yyvsp[-7].decl).num; /* number of declarations (0/1) */
		
		(yyvsp[-3].node) = pop_value((yyvsp[-3].node));
		if ((yyvsp[-3].node) && IS_NODE((yyvsp[-3].node), NODE_UNARY_OP, F_INC)
		    && IS_NODE((yyvsp[-3].node)->r.expr, NODE_OPCODE_1, F_LOCAL_LVALUE)) {
		    int lvar = (yyvsp[-3].node)->r.expr->l.number;
		    CREATE_OPCODE_1((yyvsp[-3].node), F_LOOP_INCR, 0, lvar);
		}

		CREATE_STATEMENTS((yyval.decl).node, (yyvsp[-7].decl).node, 0);
		CREATE_LOOP((yyval.decl).node->r.expr, 1, (yyvsp[0].node), (yyvsp[-3].node), optimize_loop_test((yyvsp[-5].node)));

		context = (yyvsp[-9].number);
	      }
#line 3040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1160 "grammar.y" /* yacc.c:1646  */
    {
#line 1194 "grammar.y.pre"
		if ((yyvsp[0].ihe)->dn.local_num != -1) {
		    CREATE_OPCODE_1((yyval.decl).node, F_LOCAL_LVALUE, 0, (yyvsp[0].ihe)->dn.local_num);
		    type_of_locals_ptr[(yyvsp[0].ihe)->dn.local_num] &= ~LOCAL_MOD_UNUSED;
		} else
		if ((yyvsp[0].ihe)->dn.global_num != -1) {
		    CREATE_OPCODE_1((yyval.decl).node, F_GLOBAL_LVALUE, 0, (yyvsp[0].ihe)->dn.global_num);
		} else {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "'");
		    p = strput(p, end, (yyvsp[0].ihe)->name);
		    p = strput(p, end, "' is not a local or a global variable.");
		    yyerror(buf);
		    CREATE_OPCODE_1((yyval.decl).node, F_GLOBAL_LVALUE, 0, 0);
		}
		(yyval.decl).num = 0;
	    }
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1182 "grammar.y" /* yacc.c:1646  */
    {
#line 1215 "grammar.y.pre"
		if (type_of_locals_ptr[(yyvsp[0].number)] & LOCAL_MOD_REF) {
		    CREATE_OPCODE_1((yyval.decl).node, F_REF_LVALUE, 0, (yyvsp[0].number));
		} else {
		    CREATE_OPCODE_1((yyval.decl).node, F_LOCAL_LVALUE, 0, (yyvsp[0].number));
		    type_of_locals_ptr[(yyvsp[0].number)] &= ~LOCAL_MOD_UNUSED;
		}
		(yyval.decl).num = 1;
            }
#line 3081 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1193 "grammar.y" /* yacc.c:1646  */
    {
#line 1225 "grammar.y.pre"
		char buf[256];
		char *end = EndOf(buf);
		char *p;
		
		p = strput(buf, end, "'");
		p = strput(p, end, (yyvsp[0].string));
		p = strput(p, end, "' is not a local or a global variable.");
		yyerror(buf);
		CREATE_OPCODE_1((yyval.decl).node, F_GLOBAL_LVALUE, 0, 0);
		scratch_free((yyvsp[0].string));
		(yyval.decl).num = 0;
	    }
#line 3100 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1211 "grammar.y" /* yacc.c:1646  */
    {
#line 1242 "grammar.y.pre"
		CREATE_FOREACH((yyval.decl).node, (yyvsp[0].decl).node, 0);
		(yyval.decl).num = (yyvsp[0].decl).num;
            }
#line 3110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1217 "grammar.y" /* yacc.c:1646  */
    {
#line 1247 "grammar.y.pre"
		CREATE_FOREACH((yyval.decl).node, (yyvsp[-2].decl).node, (yyvsp[0].decl).node);
		(yyval.decl).num = (yyvsp[-2].decl).num + (yyvsp[0].decl).num;
		if ((yyvsp[-2].decl).node->v.number == F_REF_LVALUE)
		    yyerror("Mapping key may not be a reference in foreach()");
            }
#line 3122 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1228 "grammar.y" /* yacc.c:1646  */
    {
#line 1257 "grammar.y.pre"
		(yyvsp[-3].decl).node->v.expr = (yyvsp[-1].node);
		(yyvsp[-5].number) = context;
		context = LOOP_CONTEXT | LOOP_FOREACH;
            }
#line 3133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1235 "grammar.y" /* yacc.c:1646  */
    {
#line 1263 "grammar.y.pre"
		(yyval.decl).num = (yyvsp[-5].decl).num;

		CREATE_STATEMENTS((yyval.decl).node, (yyvsp[-5].decl).node, 0);
		CREATE_LOOP((yyval.decl).node->r.expr, 2, (yyvsp[0].node), 0, 0);
		CREATE_OPCODE((yyval.decl).node->r.expr->r.expr, F_NEXT_FOREACH, 0);
		
		context = (yyvsp[-7].number);
	    }
#line 3148 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1249 "grammar.y" /* yacc.c:1646  */
    {
#line 1276 "grammar.y.pre"
		(yyval.node) = 0;
	    }
#line 3157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1258 "grammar.y" /* yacc.c:1646  */
    {
#line 1284 "grammar.y.pre"
	 	(yyval.decl).node = (yyvsp[0].node);
		(yyval.decl).num = 0;
	    }
#line 3167 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1264 "grammar.y" /* yacc.c:1646  */
    {
#line 1289 "grammar.y.pre"
		(yyval.decl).node = (yyvsp[0].node);
		(yyval.decl).num = 1;
	    }
#line 3177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1273 "grammar.y" /* yacc.c:1646  */
    {
#line 1297 "grammar.y.pre"
                (yyvsp[-3].number) = context;
                context &= LOOP_CONTEXT;
                context |= SWITCH_CONTEXT;
                (yyvsp[-2].number) = mem_block[A_CASES].current_size;
            }
#line 3189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1281 "grammar.y" /* yacc.c:1646  */
    {
#line 1304 "grammar.y.pre"
                parse_node_t *node1, *node2;

                if ((yyvsp[-1].node)) {
		    CREATE_STATEMENTS(node1, (yyvsp[-2].node), (yyvsp[-1].node));
                } else node1 = (yyvsp[-2].node);

                if (context & SWITCH_STRINGS) {
                    NODE_NO_LINE(node2, NODE_SWITCH_STRINGS);
                } else if (context & SWITCH_RANGES) {
		    NODE_NO_LINE(node2, NODE_SWITCH_RANGES);
		} else {
                    NODE_NO_LINE(node2, NODE_SWITCH_NUMBERS);
                }

                node2->l.expr = (yyvsp[-7].node);
                node2->r.expr = node1;
                prepare_cases(node2, (yyvsp[-8].number));
                context = (yyvsp[-9].number);
		(yyval.node) = node2;
		pop_n_locals((yyvsp[-3].decl).num);
            }
#line 3217 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1308 "grammar.y" /* yacc.c:1646  */
    {
#line 1330 "grammar.y.pre"
               if ((yyvsp[0].node)){
		   CREATE_STATEMENTS((yyval.node), (yyvsp[-1].node), (yyvsp[0].node));
               } else (yyval.node) = (yyvsp[-1].node);
           }
#line 3228 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1315 "grammar.y" /* yacc.c:1646  */
    {
#line 1336 "grammar.y.pre"
               if ((yyvsp[0].node)){
		   CREATE_STATEMENTS((yyval.node), (yyvsp[-1].node), (yyvsp[0].node));
               } else (yyval.node) = (yyvsp[-1].node);
           }
#line 3239 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1322 "grammar.y" /* yacc.c:1646  */
    {
#line 1342 "grammar.y.pre"
               (yyval.node) = 0;
           }
#line 3248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1331 "grammar.y" /* yacc.c:1646  */
    {
#line 1350 "grammar.y.pre"
                (yyval.node) = (yyvsp[-1].node);
                (yyval.node)->v.expr = 0;

                add_to_mem_block(A_CASES, (char *)&((yyvsp[-1].node)), sizeof((yyvsp[-1].node)));
            }
#line 3260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1339 "grammar.y" /* yacc.c:1646  */
    {
#line 1357 "grammar.y.pre"
                if ( (yyvsp[-3].node)->kind != NODE_CASE_NUMBER
                    || (yyvsp[-1].node)->kind != NODE_CASE_NUMBER )
                    yyerror("String case labels not allowed as range bounds");
                if ((yyvsp[-3].node)->r.number > (yyvsp[-1].node)->r.number) break;

		context |= SWITCH_RANGES;

                (yyval.node) = (yyvsp[-3].node);
                (yyval.node)->v.expr = (yyvsp[-1].node);

                add_to_mem_block(A_CASES, (char *)&((yyvsp[-3].node)), sizeof((yyvsp[-3].node)));
            }
#line 3279 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1354 "grammar.y" /* yacc.c:1646  */
    {
#line 1371 "grammar.y.pre"
                if (context & SWITCH_DEFAULT) {
                    yyerror("Duplicate default");
                    (yyval.node) = 0;
                    break;
                }
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_DEFAULT;
                (yyval.node)->v.expr = 0;
                add_to_mem_block(A_CASES, (char *)&((yyval.node)), sizeof((yyval.node)));
                context |= SWITCH_DEFAULT;
            }
#line 3297 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1371 "grammar.y" /* yacc.c:1646  */
    {
#line 1387 "grammar.y.pre"
                if ((context & SWITCH_STRINGS) && (yyvsp[0].pointer_int))
                    yyerror("Mixed case label list not allowed");

                if ((yyvsp[0].pointer_int)) context |= SWITCH_NUMBERS;

		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_CASE_NUMBER;
                (yyval.node)->r.expr = (parse_node_t *)(yyvsp[0].pointer_int);
            }
#line 3313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1383 "grammar.y" /* yacc.c:1646  */
    {
#line 1398 "grammar.y.pre"
		int str;
		
		str = store_prog_string((yyvsp[0].string));
                scratch_free((yyvsp[0].string));
                if (context & SWITCH_NUMBERS)
                    yyerror("Mixed case label list not allowed");
                context |= SWITCH_STRINGS;
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_CASE_STRING;
                (yyval.node)->r.number = str;
            }
#line 3331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1400 "grammar.y" /* yacc.c:1646  */
    {
#line 1414 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) | (yyvsp[0].pointer_int);
            }
#line 3340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1405 "grammar.y" /* yacc.c:1646  */
    {
#line 1418 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) ^ (yyvsp[0].pointer_int);
            }
#line 3349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1410 "grammar.y" /* yacc.c:1646  */
    {
#line 1422 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) & (yyvsp[0].pointer_int);
            }
#line 3358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1415 "grammar.y" /* yacc.c:1646  */
    {
#line 1426 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) == (yyvsp[0].pointer_int);
            }
#line 3367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1420 "grammar.y" /* yacc.c:1646  */
    {
#line 1430 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) != (yyvsp[0].pointer_int);
            }
#line 3376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1425 "grammar.y" /* yacc.c:1646  */
    {
#line 1434 "grammar.y.pre"
                switch((yyvsp[-1].number)){
                    case F_GE: (yyval.pointer_int) = (yyvsp[-2].pointer_int) >= (yyvsp[0].pointer_int); break;
                    case F_LE: (yyval.pointer_int) = (yyvsp[-2].pointer_int) <= (yyvsp[0].pointer_int); break;
                    case F_GT: (yyval.pointer_int) = (yyvsp[-2].pointer_int) >  (yyvsp[0].pointer_int); break;
                }
            }
#line 3389 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1434 "grammar.y" /* yacc.c:1646  */
    {
#line 1442 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) < (yyvsp[0].pointer_int);
            }
#line 3398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1439 "grammar.y" /* yacc.c:1646  */
    {
#line 1446 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) << (yyvsp[0].pointer_int);
            }
#line 3407 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1444 "grammar.y" /* yacc.c:1646  */
    {
#line 1450 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) >> (yyvsp[0].pointer_int);
            }
#line 3416 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1449 "grammar.y" /* yacc.c:1646  */
    {
#line 1454 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) + (yyvsp[0].pointer_int);
            }
#line 3425 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1454 "grammar.y" /* yacc.c:1646  */
    {
#line 1458 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) - (yyvsp[0].pointer_int);
            }
#line 3434 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1459 "grammar.y" /* yacc.c:1646  */
    {
#line 1462 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-2].pointer_int) * (yyvsp[0].pointer_int);
            }
#line 3443 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1464 "grammar.y" /* yacc.c:1646  */
    {
#line 1466 "grammar.y.pre"
                if ((yyvsp[0].pointer_int)) (yyval.pointer_int) = (yyvsp[-2].pointer_int) % (yyvsp[0].pointer_int); else yyerror("Modulo by zero");
            }
#line 3452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1469 "grammar.y" /* yacc.c:1646  */
    {
#line 1470 "grammar.y.pre"
                if ((yyvsp[0].pointer_int)) (yyval.pointer_int) = (yyvsp[-2].pointer_int) / (yyvsp[0].pointer_int); else yyerror("Division by zero");
            }
#line 3461 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1474 "grammar.y" /* yacc.c:1646  */
    {
#line 1474 "grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[-1].pointer_int);
            }
#line 3470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1479 "grammar.y" /* yacc.c:1646  */
    {
#line 1478 "grammar.y.pre"
		(yyval.pointer_int) = (yyvsp[0].number);
	    }
#line 3479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1484 "grammar.y" /* yacc.c:1646  */
    {
#line 1482 "grammar.y.pre"
                (yyval.pointer_int) = -(yyvsp[0].number);
            }
#line 3488 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1489 "grammar.y" /* yacc.c:1646  */
    {
#line 1486 "grammar.y.pre"
                (yyval.pointer_int) = !(yyvsp[0].number);
            }
#line 3497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1494 "grammar.y" /* yacc.c:1646  */
    {
#line 1490 "grammar.y.pre"
                (yyval.pointer_int) = ~(yyvsp[0].number);
            }
#line 3506 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1502 "grammar.y" /* yacc.c:1646  */
    {
#line 1497 "grammar.y.pre"
		(yyval.node) = (yyvsp[0].node);
	    }
#line 3515 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1507 "grammar.y" /* yacc.c:1646  */
    {
#line 1501 "grammar.y.pre"
		CREATE_TWO_VALUES((yyval.node), (yyvsp[0].node)->type, pop_value((yyvsp[-2].node)), (yyvsp[0].node));
	    }
#line 3524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1516 "grammar.y" /* yacc.c:1646  */
    {
#line 1545 "grammar.y.pre"
	        parse_node_t *l = (yyvsp[-2].node), *r = (yyvsp[0].node);
		/* set this up here so we can change it below */
		/* assignments are backwards; rhs is evaluated before
		   lhs, so put the RIGHT hand side on the LEFT hand
		   side of the tree node. */
		CREATE_BINARY_OP((yyval.node), (yyvsp[-1].number), r->type, r, l);

		if (exact_types && !compatible_types(r->type, l->type) &&
		    !((yyvsp[-1].number) == F_ADD_EQ
		      && r->type == TYPE_STRING && 
		      COMP_TYPE(l->type, TYPE_NUMBER))) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "Bad assignment ");
		    p = get_two_types(p, end, l->type, r->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}
		
		if ((yyvsp[-1].number) == F_ASSIGN)
		    (yyval.node)->l.expr = do_promotions(r, l->type);
	    }
#line 3555 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1543 "grammar.y" /* yacc.c:1646  */
    {
#line 1571 "grammar.y.pre"
		yyerror("Illegal LHS");
		CREATE_ERROR((yyval.node));
	    }
#line 3565 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1549 "grammar.y" /* yacc.c:1646  */
    {
#line 1576 "grammar.y.pre"
		parse_node_t *p1 = (yyvsp[-2].node), *p2 = (yyvsp[0].node);

		if (exact_types && !compatible_types2(p1->type, p2->type)) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "Types in ?: do not match ");
		    p = get_two_types(p, end, p1->type, p2->type);
		    p = strput(p, end, ".");
		    yywarn(buf);
		}

		/* optimize if last expression did F_NOT */
		if (IS_NODE((yyvsp[-4].node), NODE_UNARY_OP, F_NOT)) {
		    /* !a ? b : c  --> a ? c : b */
		    CREATE_IF((yyval.node), (yyvsp[-4].node)->r.expr, p2, p1);
		} else {
		    CREATE_IF((yyval.node), (yyvsp[-4].node), p1, p2);
		}
		(yyval.node)->type = ((p1->type == p2->type) ? p1->type : TYPE_ANY);
	    }
#line 3594 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1574 "grammar.y" /* yacc.c:1646  */
    {
#line 1600 "grammar.y.pre"
		CREATE_LAND_LOR((yyval.node), F_LOR, (yyvsp[-2].node), (yyvsp[0].node));
		if (IS_NODE((yyvsp[-2].node), NODE_LAND_LOR, F_LOR))
		    (yyvsp[-2].node)->kind = NODE_BRANCH_LINK;
	    }
#line 3605 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1581 "grammar.y" /* yacc.c:1646  */
    {
#line 1606 "grammar.y.pre"
		CREATE_LAND_LOR((yyval.node), F_LAND, (yyvsp[-2].node), (yyvsp[0].node));
		if (IS_NODE((yyvsp[-2].node), NODE_LAND_LOR, F_LAND))
		    (yyvsp[-2].node)->kind = NODE_BRANCH_LINK;
	    }
#line 3616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1588 "grammar.y" /* yacc.c:1646  */
    {
#line 1612 "grammar.y.pre"
		int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;
		
		if (is_boolean((yyvsp[-2].node)) && is_boolean((yyvsp[0].node)))
		    yywarn("bitwise operation on boolean values.");
		if ((t1 & TYPE_MOD_ARRAY) || (t3 & TYPE_MOD_ARRAY)) {
		    if (t1 != t3) {
			if ((t1 != TYPE_ANY) && (t3 != TYPE_ANY) &&
			    !(t1 & t3 & TYPE_MOD_ARRAY)) {
			    char buf[256];
			    char *end = EndOf(buf);
			    char *p;

			    p = strput(buf, end, "Incompatible types for | ");
			    p = get_two_types(p, end, t1, t3);
			    p = strput(p, end, ".");
			    yyerror(buf);
			}
			t1 = TYPE_ANY | TYPE_MOD_ARRAY;
		    }
		    CREATE_BINARY_OP((yyval.node), F_OR, t1, (yyvsp[-2].node), (yyvsp[0].node));
		}
		else (yyval.node) = binary_int_op((yyvsp[-2].node), (yyvsp[0].node), F_OR, "|");		
	    }
#line 3646 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1614 "grammar.y" /* yacc.c:1646  */
    {
#line 1637 "grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[-2].node), (yyvsp[0].node), F_XOR, "^");
	    }
#line 3655 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1619 "grammar.y" /* yacc.c:1646  */
    {
#line 1641 "grammar.y.pre"
		int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;
		if (is_boolean((yyvsp[-2].node)) && is_boolean((yyvsp[0].node)))
		    yywarn("bitwise operation on boolean values.");
		if ((t1 & TYPE_MOD_ARRAY) || (t3 & TYPE_MOD_ARRAY)) {
		    if (t1 != t3) {
			if ((t1 != TYPE_ANY) && (t3 != TYPE_ANY) &&
			    !(t1 & t3 & TYPE_MOD_ARRAY)) {
			    char buf[256];
			    char *end = EndOf(buf);
			    char *p;
			    
			    p = strput(buf, end, "Incompatible types for & ");
			    p = get_two_types(p, end, t1, t3);
			    p = strput(p, end, ".");
			    yyerror(buf);
			}
			t1 = TYPE_ANY | TYPE_MOD_ARRAY;
		    } 
		    CREATE_BINARY_OP((yyval.node), F_AND, t1, (yyvsp[-2].node), (yyvsp[0].node));
		} else (yyval.node) = binary_int_op((yyvsp[-2].node), (yyvsp[0].node), F_AND, "&");
	    }
#line 3683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1643 "grammar.y" /* yacc.c:1646  */
    {
#line 1664 "grammar.y.pre"
		if (exact_types && !compatible_types2((yyvsp[-2].node)->type, (yyvsp[0].node)->type)){
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "== always false because of incompatible types ");
		    p = get_two_types(p, end, (yyvsp[-2].node)->type, (yyvsp[0].node)->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}
		/* x == 0 -> !x */
		if (IS_NODE((yyvsp[-2].node), NODE_NUMBER, 0)) {
		    CREATE_UNARY_OP((yyval.node), F_NOT, TYPE_NUMBER, (yyvsp[0].node));
		} else
		if (IS_NODE((yyvsp[0].node), NODE_NUMBER, 0)) {
		    CREATE_UNARY_OP((yyval.node), F_NOT, TYPE_NUMBER, (yyvsp[-2].node));
		} else {
		    CREATE_BINARY_OP((yyval.node), F_EQ, TYPE_NUMBER, (yyvsp[-2].node), (yyvsp[0].node));
		}
	    }
#line 3710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1666 "grammar.y" /* yacc.c:1646  */
    {
#line 1686 "grammar.y.pre"
		if (exact_types && !compatible_types2((yyvsp[-2].node)->type, (yyvsp[0].node)->type)){
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "!= always true because of incompatible types ");
		    p = get_two_types(p, end, (yyvsp[-2].node)->type, (yyvsp[0].node)->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}
                CREATE_BINARY_OP((yyval.node), F_NE, TYPE_NUMBER, (yyvsp[-2].node), (yyvsp[0].node));
	    }
#line 3729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1681 "grammar.y" /* yacc.c:1646  */
    {
#line 1700 "grammar.y.pre"
		if (exact_types) {
		    int t1 = (yyvsp[-2].node)->type;
		    int t3 = (yyvsp[0].node)->type;

		    if (!COMP_TYPE(t1, TYPE_NUMBER) 
			&& !COMP_TYPE(t1, TYPE_STRING)) {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Bad left argument to '");
			p = strput(p, end, query_instr_name((yyvsp[-1].number)));
			p = strput(p, end, "' : \"");
			p = get_type_name(p, end, t1);
			p = strput(p, end, "\"");
			yyerror(buf);
		    } else if (!COMP_TYPE(t3, TYPE_NUMBER) 
			       && !COMP_TYPE(t3, TYPE_STRING)) {
                        char buf[256];
			char *end = EndOf(buf);
			char *p;
			
                        p = strput(buf, end, "Bad right argument to '");
                        p = strput(p, end, query_instr_name((yyvsp[-1].number)));
                        p = strput(p, end, "' : \"");
                        p = get_type_name(p, end, t3);
			p = strput(p, end, "\"");
			yyerror(buf);
		    } else if (!compatible_types2(t1,t3)) {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Arguments to ");
			p = strput(p, end, query_instr_name((yyvsp[-1].number)));
			p = strput(p, end, " do not have compatible types : ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
		    }
		}
                CREATE_BINARY_OP((yyval.node), (yyvsp[-1].number), TYPE_NUMBER, (yyvsp[-2].node), (yyvsp[0].node));
	    }
#line 3778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1726 "grammar.y" /* yacc.c:1646  */
    {
#line 1744 "grammar.y.pre"
                if (exact_types) {
                    int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;

                    if (!COMP_TYPE(t1, TYPE_NUMBER) 
			&& !COMP_TYPE(t1, TYPE_STRING)) {
                        char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Bad left argument to '<' : \"");
                        p = get_type_name(p, end, t1);
			p = strput(p, end, "\"");
                        yyerror(buf);
                    } else if (!COMP_TYPE(t3, TYPE_NUMBER)
			       && !COMP_TYPE(t3, TYPE_STRING)) {
                        char buf[200];
			char *end = EndOf(buf);
			char *p;
			
                        p = strput(buf, end, "Bad right argument to '<' : \"");
                        p = get_type_name(p, end, t3);
                        p = strput(p, end, "\"");
                        yyerror(buf);
                    } else if (!compatible_types2(t1,t3)) {
                        char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Arguments to < do not have compatible types : ");
			p = get_two_types(p, end, t1, t3);
                        yyerror(buf);
                    }
                }
                CREATE_BINARY_OP((yyval.node), F_LT, TYPE_NUMBER, (yyvsp[-2].node), (yyvsp[0].node));
            }
#line 3820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1764 "grammar.y" /* yacc.c:1646  */
    {
#line 1781 "grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[-2].node), (yyvsp[0].node), F_LSH, "<<");
	    }
#line 3829 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1769 "grammar.y" /* yacc.c:1646  */
    {
#line 1785 "grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[-2].node), (yyvsp[0].node), F_RSH, ">>");
	    }
#line 3838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1774 "grammar.y" /* yacc.c:1646  */
    {
#line 1789 "grammar.y.pre"
		int result_type;

		if (exact_types) {
		    int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;

		    if (t1 == t3){
#ifdef CAST_CALL_OTHERS
			if (t1 == TYPE_UNKNOWN){
			    yyerror("Bad arguments to '+' (unknown vs unknown)");
			    result_type = TYPE_ANY;
			} else
#endif
			    result_type = t1;
		    }
		    else if (t1 == TYPE_ANY) {
			if (t3 == TYPE_FUNCTION) {
			    yyerror("Bad right argument to '+' (function)");
			    result_type = TYPE_ANY;
			} else result_type = t3;
		    } else if (t3 == TYPE_ANY) {
			if (t1 == TYPE_FUNCTION) {
			    yyerror("Bad left argument to '+' (function)");
			    result_type = TYPE_ANY;
			} else result_type = t1;
		    } else {
			switch(t1) {
			    case TYPE_STRING:
			    {
				if (t3 == TYPE_REAL || t3 == TYPE_NUMBER){
				    result_type = TYPE_STRING;
				} else goto add_error;
				break;
			    }
			    case TYPE_NUMBER:
			    {
				if (t3 == TYPE_REAL || t3 == TYPE_STRING)
				    result_type = t3;
				else goto add_error;
				break;
			    }
			case TYPE_REAL:
			    {
				if (t3 == TYPE_NUMBER) result_type = TYPE_REAL;
				else if (t3 == TYPE_STRING) result_type = TYPE_STRING;
				else goto add_error;
				break;
			    }
			    default:
			    {
				if (t1 & t3 & TYPE_MOD_ARRAY) {
				    result_type = TYPE_ANY|TYPE_MOD_ARRAY;
				    break;
				}
add_error:
				{
				    char buf[256];
				    char *end = EndOf(buf);
				    char *p;
				    
				    p = strput(buf, end, "Invalid argument types to '+' ");
				    p = get_two_types(p, end, t1, t3);
				    yyerror(buf);
				    result_type = TYPE_ANY;
				}
			    }
			}
		    }
		} else 
		    result_type = TYPE_ANY;

		/* TODO: perhaps we should do (string)+(number) and
		 * (number)+(string) constant folding as well.
		 *
		 * codefor string x = "foo" + 1;
		 *
		 * 0000: push string 13, number 1
		 * 0004: +
		 * 0005: (void)assign_local LV0
		 */
		switch ((yyvsp[-2].node)->kind) {
		case NODE_NUMBER:
		    /* 0 + X */
		    if ((yyvsp[-2].node)->v.number == 0 &&
			((yyvsp[0].node)->type == TYPE_NUMBER || (yyvsp[0].node)->type == TYPE_REAL)) {
			(yyval.node) = (yyvsp[0].node);
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.number += (yyvsp[0].node)->v.number;
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[0].node);
			(yyvsp[0].node)->v.real += (yyvsp[-2].node)->v.number;
			break;
		    }
		    /* swapping the nodes may help later constant folding */
		    if ((yyvsp[0].node)->type != TYPE_STRING && (yyvsp[0].node)->type != TYPE_ANY)
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[0].node), (yyvsp[-2].node));
		    else
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    break;
		case NODE_REAL:
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real += (yyvsp[0].node)->v.number;
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real += (yyvsp[0].node)->v.real;
			break;
		    }
		    /* swapping the nodes may help later constant folding */
		    if ((yyvsp[0].node)->type != TYPE_STRING && (yyvsp[0].node)->type != TYPE_ANY)
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[0].node), (yyvsp[-2].node));
		    else
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    break;
		case NODE_STRING:
		    if ((yyvsp[0].node)->kind == NODE_STRING) {
			/* Combine strings */
			int n1, n2;
			char *new, *s1, *s2;
			int l;

			n1 = (yyvsp[-2].node)->v.number;
			n2 = (yyvsp[0].node)->v.number;
			s1 = PROG_STRING(n1);
			s2 = PROG_STRING(n2);
			new = (char *)DXALLOC( (l = strlen(s1))+strlen(s2)+1, TAG_COMPILER, "combine string" );
			strcpy(new, s1);
			strcat(new + l, s2);
			/* free old strings (ordering may help shrink table) */
			if (n1 > n2) {
			    free_prog_string(n1); free_prog_string(n2);
			} else {
			    free_prog_string(n2); free_prog_string(n1);
			}
			(yyval.node) = (yyvsp[-2].node);
			(yyval.node)->v.number = store_prog_string(new);
			FREE(new);
			break;
		    }
		    /* Yes, this can actually happen for absurd code like:
		     * (int)"foo" + 0
		     * for which I guess we ought to generate (int)"foo"
		     * in order to be consistent.  Then shoot the coder.
		     */
		    /* FALLTHROUGH */
		default:
		    /* X + 0 */
		    if (IS_NODE((yyvsp[0].node), NODE_NUMBER, 0) &&
			((yyvsp[-2].node)->type == TYPE_NUMBER || (yyvsp[-2].node)->type == TYPE_REAL)) {
			(yyval.node) = (yyvsp[-2].node);
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    break;
		}
	    }
#line 4007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1939 "grammar.y" /* yacc.c:1646  */
    {
#line 1953 "grammar.y.pre"
		int result_type;

		if (exact_types) {
		    int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;

		    if (t1 == t3){
			switch(t1){
			    case TYPE_ANY:
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			        result_type = t1;
				break;
			    default:
				if (!(t1 & TYPE_MOD_ARRAY)){
				    type_error("Bad argument number 1 to '-'", t1);
				    result_type = TYPE_ANY;
				} else result_type = t1;
			}
		    } else if (t1 == TYPE_ANY){
			switch(t3){
			    case TYPE_REAL:
			    case TYPE_NUMBER:
			        result_type = t3;
				break;
			    default:
				if (!(t3 & TYPE_MOD_ARRAY)){
				    type_error("Bad argument number 2 to '-'", t3);
				    result_type = TYPE_ANY;
				} else result_type = t3;
			}
		    } else if (t3 == TYPE_ANY){
			switch(t1){
			    case TYPE_REAL:
			    case TYPE_NUMBER:
			        result_type = t1;
				break;
			    default:
				if (!(t1 & TYPE_MOD_ARRAY)){
				    type_error("Bad argument number 1 to '-'", t1);
				    result_type = TYPE_ANY;
				} else result_type = t1;
			}
		    } else if ((t1 == TYPE_REAL && t3 == TYPE_NUMBER) ||
			       (t3 == TYPE_REAL && t1 == TYPE_NUMBER)){
			result_type = TYPE_REAL;
		    } else if (t1 & t3 & TYPE_MOD_ARRAY){
			result_type = TYPE_MOD_ARRAY|TYPE_ANY;
		    } else {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Invalid types to '-' ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
			result_type = TYPE_ANY;
		    }
		} else result_type = TYPE_ANY;
		
		switch ((yyvsp[-2].node)->kind) {
		case NODE_NUMBER:
		    if ((yyvsp[-2].node)->v.number == 0) {
			CREATE_UNARY_OP((yyval.node), F_NEGATE, (yyvsp[0].node)->type, (yyvsp[0].node));
		    } else if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.number -= (yyvsp[0].node)->v.number;
		    } else if ((yyvsp[0].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[0].node);
			(yyvsp[0].node)->v.real = (yyvsp[-2].node)->v.number - (yyvsp[0].node)->v.real;
		    } else {
			CREATE_BINARY_OP((yyval.node), F_SUBTRACT, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    }
		    break;
		case NODE_REAL:
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real -= (yyvsp[0].node)->v.number;
		    } else if ((yyvsp[0].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real -= (yyvsp[0].node)->v.real;
		    } else {
			CREATE_BINARY_OP((yyval.node), F_SUBTRACT, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    }
		    break;
		default:
		    /* optimize X-0 */
		    if (IS_NODE((yyvsp[0].node), NODE_NUMBER, 0)) {
			(yyval.node) = (yyvsp[-2].node);
		    } 
		    CREATE_BINARY_OP((yyval.node), F_SUBTRACT, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		}
	    }
#line 4106 "y.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 2034 "grammar.y" /* yacc.c:1646  */
    {
#line 2047 "grammar.y.pre"
		int result_type;

		if (exact_types){
		    int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;

		    if (t1 == t3){
			switch(t1){
			    case TYPE_MAPPING:
			    case TYPE_ANY:
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			        result_type = t1;
				break;
			default:
				type_error("Bad argument number 1 to '*'", t1);
				result_type = TYPE_ANY;
			}
		    } else if (t1 == TYPE_ANY || t3 == TYPE_ANY){
			int t = (t1 == TYPE_ANY) ? t3 : t1;
			switch(t){
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			    case TYPE_MAPPING:
			        result_type = t;
				break;
			    default:
				type_error((t1 == TYPE_ANY) ?
					   "Bad argument number 2 to '*'" :
					   "Bad argument number 1 to '*'",
					   t);
				result_type = TYPE_ANY;
			}
		    } else if ((t1 == TYPE_NUMBER && t3 == TYPE_REAL) ||
			       (t1 == TYPE_REAL && t3 == TYPE_NUMBER)){
			result_type = TYPE_REAL;
		    } else {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Invalid types to '*' ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
			result_type = TYPE_ANY;
		    }
		} else result_type = TYPE_ANY;

		switch ((yyvsp[-2].node)->kind) {
		case NODE_NUMBER:
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[-2].node);
			(yyval.node)->v.number *= (yyvsp[0].node)->v.number;
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[0].node);
			(yyvsp[0].node)->v.real *= (yyvsp[-2].node)->v.number;
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_MULTIPLY, result_type, (yyvsp[0].node), (yyvsp[-2].node));
		    break;
		case NODE_REAL:
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real *= (yyvsp[0].node)->v.number;
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real *= (yyvsp[0].node)->v.real;
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_MULTIPLY, result_type, (yyvsp[0].node), (yyvsp[-2].node));
		    break;
		default:
		    CREATE_BINARY_OP((yyval.node), F_MULTIPLY, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		}
	    }
#line 4191 "y.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 2115 "grammar.y" /* yacc.c:1646  */
    {
#line 2127 "grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[-2].node), (yyvsp[0].node), F_MOD, "%");
	    }
#line 4200 "y.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 2120 "grammar.y" /* yacc.c:1646  */
    {
#line 2131 "grammar.y.pre"
		int result_type;

		if (exact_types){
		    int t1 = (yyvsp[-2].node)->type, t3 = (yyvsp[0].node)->type;

		    if (t1 == t3){
			switch(t1){
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			case TYPE_ANY:
			        result_type = t1;
				break;
			    default:
				type_error("Bad argument 1 to '/'", t1);
				result_type = TYPE_ANY;
			}
		    } else if (t1 == TYPE_ANY || t3 == TYPE_ANY){
			int t = (t1 == TYPE_ANY) ? t3 : t1;
			if (t == TYPE_REAL || t == TYPE_NUMBER)
			    result_type = t; 
			else {
			    type_error(t1 == TYPE_ANY ?
				       "Bad argument 2 to '/'" :
				       "Bad argument 1 to '/'", t);
			    result_type = TYPE_ANY;
			}
		    } else if ((t1 == TYPE_NUMBER && t3 == TYPE_REAL) ||
			       (t1 == TYPE_REAL && t3 == TYPE_NUMBER)) {
			result_type = TYPE_REAL;
		    } else {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Invalid types to '/' ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
			result_type = TYPE_ANY;
		    }
		} else result_type = TYPE_ANY;		    

		/* constant expressions */
		switch ((yyvsp[-2].node)->kind) {
		case NODE_NUMBER:
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			if ((yyvsp[0].node)->v.number == 0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[-2].node);
			    break;
			}
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.number /= (yyvsp[0].node)->v.number;
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_REAL) {
			if ((yyvsp[0].node)->v.real == 0.0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[-2].node);
			    break;
			}
			(yyval.node) = (yyvsp[0].node);
			(yyvsp[0].node)->v.real = ((yyvsp[-2].node)->v.number / (yyvsp[0].node)->v.real);
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_DIVIDE, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    break;
		case NODE_REAL:
		    if ((yyvsp[0].node)->kind == NODE_NUMBER) {
			if ((yyvsp[0].node)->v.number == 0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[-2].node);
			    break;
			}
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real /= (yyvsp[0].node)->v.number;
			break;
		    }
		    if ((yyvsp[0].node)->kind == NODE_REAL) {
			if ((yyvsp[0].node)->v.real == 0.0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[-2].node);
			    break;
			}
			(yyval.node) = (yyvsp[-2].node);
			(yyvsp[-2].node)->v.real /= (yyvsp[0].node)->v.real;
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_DIVIDE, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		    break;
		default:
		    CREATE_BINARY_OP((yyval.node), F_DIVIDE, result_type, (yyvsp[-2].node), (yyvsp[0].node));
		}
	    }
#line 4300 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 2216 "grammar.y" /* yacc.c:1646  */
    {
#line 2226 "grammar.y.pre"
		(yyval.node) = (yyvsp[0].node);
		(yyval.node)->type = (yyvsp[-1].type);

		if (exact_types &&
		    (yyvsp[0].node)->type != (yyvsp[-1].type) &&
		    (yyvsp[0].node)->type != TYPE_ANY && 
		    (yyvsp[0].node)->type != TYPE_UNKNOWN &&
		    (yyvsp[-1].type) != TYPE_VOID) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "Cannot cast ");
		    p = get_type_name(p, end, (yyvsp[0].node)->type);
		    p = strput(p, end, "to ");
		    p = get_type_name(p, end, (yyvsp[-1].type));
		    yyerror(buf);
		}
	    }
#line 4326 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 2238 "grammar.y" /* yacc.c:1646  */
    {
#line 2247 "grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_PRE_INC, 0, (yyvsp[0].node));
                if (exact_types){
                    switch((yyvsp[0].node)->type){
                        case TYPE_NUMBER:
                        case TYPE_ANY:
                        case TYPE_REAL:
                        {
                            (yyval.node)->type = (yyvsp[0].node)->type;
                            break;
                        }

                        default:
                        {
                            (yyval.node)->type = TYPE_ANY;
                            type_error("Bad argument 1 to ++x", (yyvsp[0].node)->type);
                        }
                    }
                } else (yyval.node)->type = TYPE_ANY;
	    }
#line 4352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 2260 "grammar.y" /* yacc.c:1646  */
    {
#line 2268 "grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_PRE_DEC, 0, (yyvsp[0].node));
                if (exact_types){
                    switch((yyvsp[0].node)->type){
                        case TYPE_NUMBER:
                        case TYPE_ANY:
                        case TYPE_REAL:
                        {
                            (yyval.node)->type = (yyvsp[0].node)->type;
                            break;
                        }

                        default:
                        {
                            (yyval.node)->type = TYPE_ANY;
                            type_error("Bad argument 1 to --x", (yyvsp[0].node)->type);
                        }
                    }
                } else (yyval.node)->type = TYPE_ANY;

	    }
#line 4379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 2283 "grammar.y" /* yacc.c:1646  */
    {
#line 2290 "grammar.y.pre"
		if ((yyvsp[0].node)->kind == NODE_NUMBER) {
		    (yyval.node) = (yyvsp[0].node);
		    (yyval.node)->v.number = !((yyval.node)->v.number);
		} else {
		    CREATE_UNARY_OP((yyval.node), F_NOT, TYPE_NUMBER, (yyvsp[0].node));
		}
	    }
#line 4393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 2293 "grammar.y" /* yacc.c:1646  */
    {
#line 2299 "grammar.y.pre"
		if (exact_types && !IS_TYPE((yyvsp[0].node)->type, TYPE_NUMBER))
		    type_error("Bad argument to ~", (yyvsp[0].node)->type);
		if ((yyvsp[0].node)->kind == NODE_NUMBER) {
		    (yyval.node) = (yyvsp[0].node);
		    (yyval.node)->v.number = ~(yyval.node)->v.number;
		} else {
		    CREATE_UNARY_OP((yyval.node), F_COMPL, TYPE_NUMBER, (yyvsp[0].node));
		}
	    }
#line 4409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 2305 "grammar.y" /* yacc.c:1646  */
    {
#line 2310 "grammar.y.pre"
		int result_type;
                if (exact_types){
		    int t = (yyvsp[0].node)->type;
		    if (!COMP_TYPE(t, TYPE_NUMBER)){
			type_error("Bad argument to unary '-'", t);
			result_type = TYPE_ANY;
		    } else result_type = t;
		} else result_type = TYPE_ANY;

		switch ((yyvsp[0].node)->kind) {
		case NODE_NUMBER:
		    (yyval.node) = (yyvsp[0].node);
		    (yyval.node)->v.number = -(yyval.node)->v.number;
		    break;
		case NODE_REAL:
		    (yyval.node) = (yyvsp[0].node);
		    (yyval.node)->v.real = -(yyval.node)->v.real;
		    break;
		default:
		    CREATE_UNARY_OP((yyval.node), F_NEGATE, result_type, (yyvsp[0].node));
		}
	    }
#line 4438 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 2330 "grammar.y" /* yacc.c:1646  */
    {
#line 2334 "grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_POST_INC, 0, (yyvsp[-1].node));
		(yyval.node)->v.number = F_POST_INC;
                if (exact_types){
                    switch((yyvsp[-1].node)->type){
                        case TYPE_NUMBER:
		    case TYPE_ANY:
                        case TYPE_REAL:
                        {
                            (yyval.node)->type = (yyvsp[-1].node)->type;
                            break;
                        }

                        default:
                        {
                            (yyval.node)->type = TYPE_ANY;
                            type_error("Bad argument 1 to x++", (yyvsp[-1].node)->type);
                        }
                    }
                } else (yyval.node)->type = TYPE_ANY;
	    }
#line 4465 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 2353 "grammar.y" /* yacc.c:1646  */
    {
#line 2356 "grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_POST_DEC, 0, (yyvsp[-1].node));
                if (exact_types){
                    switch((yyvsp[-1].node)->type){
		    case TYPE_NUMBER:
		    case TYPE_ANY:
		    case TYPE_REAL:
		    {
			(yyval.node)->type = (yyvsp[-1].node)->type;
			break;
		    }

		    default:
		    {
			(yyval.node)->type = TYPE_ANY;
			type_error("Bad argument 1 to x--", (yyvsp[-1].node)->type);
		    }
                    }
                } else (yyval.node)->type = TYPE_ANY;
	    }
#line 4491 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 2384 "grammar.y" /* yacc.c:1646  */
    {
#line 2386 "grammar.y.pre"
		if (exact_types && !IS_TYPE(exact_types, TYPE_VOID))
		    yywarn("Non-void functions must return a value.");
		CREATE_RETURN((yyval.node), 0);
	    }
#line 4502 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 2391 "grammar.y" /* yacc.c:1646  */
    {
#line 2392 "grammar.y.pre"
		if (exact_types && !compatible_types((yyvsp[-1].node)->type, exact_types)) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "Type of returned value doesn't match function return type ");
		    p = get_two_types(p, end, (yyvsp[-1].node)->type, exact_types);
		    yyerror(buf);
		}
		if (IS_NODE((yyvsp[-1].node), NODE_NUMBER, 0)) {
		    CREATE_RETURN((yyval.node), 0);
		} else {
		    CREATE_RETURN((yyval.node), (yyvsp[-1].node));
		}
	    }
#line 4524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 2412 "grammar.y" /* yacc.c:1646  */
    {
#line 2412 "grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), 0);
	    }
#line 4533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 2417 "grammar.y" /* yacc.c:1646  */
    {
#line 2416 "grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[0].node));
	    }
#line 4542 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 2422 "grammar.y" /* yacc.c:1646  */
    {
#line 2420 "grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[-1].node));
	    }
#line 4551 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 2430 "grammar.y" /* yacc.c:1646  */
    {
#line 2427 "grammar.y.pre"
		CREATE_EXPR_NODE((yyval.node), (yyvsp[0].node), 0);
	    }
#line 4560 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 2435 "grammar.y" /* yacc.c:1646  */
    {
#line 2431 "grammar.y.pre"
		CREATE_EXPR_NODE((yyval.node), (yyvsp[-1].node), 1);
	    }
#line 4569 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 2443 "grammar.y" /* yacc.c:1646  */
    {
#line 2438 "grammar.y.pre"
		(yyvsp[0].node)->kind = 1;

		(yyval.node) = (yyvsp[0].node);
	    }
#line 4580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 2450 "grammar.y" /* yacc.c:1646  */
    {
#line 2444 "grammar.y.pre"
		(yyvsp[0].node)->kind = 0;

		(yyval.node) = (yyvsp[-2].node);
		(yyval.node)->kind++;
		(yyval.node)->l.expr->r.expr = (yyvsp[0].node);
		(yyval.node)->l.expr = (yyvsp[0].node);
	    }
#line 4594 "y.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 2463 "grammar.y" /* yacc.c:1646  */
    {
#line 2456 "grammar.y.pre"
		/* this is a dummy node */
		CREATE_EXPR_LIST((yyval.node), 0);
	    }
#line 4604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 2469 "grammar.y" /* yacc.c:1646  */
    {
#line 2461 "grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[0].node));
	    }
#line 4613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 2474 "grammar.y" /* yacc.c:1646  */
    {
#line 2465 "grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[-1].node));
	    }
#line 4622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 2482 "grammar.y" /* yacc.c:1646  */
    {
#line 2472 "grammar.y.pre"
		(yyval.node) = new_node_no_line();
		(yyval.node)->kind = 2;
		(yyval.node)->v.expr = (yyvsp[0].node);
		(yyval.node)->r.expr = 0;
		(yyval.node)->type = 0;
		/* we keep track of the end of the chain in the left nodes */
		(yyval.node)->l.expr = (yyval.node);
            }
#line 4637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 2493 "grammar.y" /* yacc.c:1646  */
    {
#line 2482 "grammar.y.pre"
		parse_node_t *expr;

		expr = new_node_no_line();
		expr->kind = 0;
		expr->v.expr = (yyvsp[0].node);
		expr->r.expr = 0;
		expr->type = 0;
		
		(yyvsp[-2].node)->l.expr->r.expr = expr;
		(yyvsp[-2].node)->l.expr = expr;
		(yyvsp[-2].node)->kind += 2;
		(yyval.node) = (yyvsp[-2].node);
	    }
#line 4657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 2512 "grammar.y" /* yacc.c:1646  */
    {
#line 2500 "grammar.y.pre"
		CREATE_TWO_VALUES((yyval.node), 0, (yyvsp[-2].node), (yyvsp[0].node));
            }
#line 4666 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 2520 "grammar.y" /* yacc.c:1646  */
    {
#line 2507 "grammar.y.pre"
#define LV_ILLEGAL 1
#define LV_RANGE 2
#define LV_INDEX 4
                /* Restrictive lvalues, but I think they make more sense :) */
                (yyval.node) = (yyvsp[0].node);
                switch((yyval.node)->kind) {
		default:
		    yyerror("Illegal lvalue");
		    break;
		case NODE_PARAMETER:
		    (yyval.node)->kind = NODE_PARAMETER_LVALUE;
		    break;
		case NODE_TERNARY_OP:
		    (yyval.node)->v.number = (yyval.node)->r.expr->v.number;
		case NODE_OPCODE_1:
		case NODE_UNARY_OP_1:
		case NODE_BINARY_OP:
		    if ((yyval.node)->v.number >= F_LOCAL && (yyval.node)->v.number <= F_MEMBER)
			(yyval.node)->v.number++; /* make it an lvalue */
		    else if ((yyval.node)->v.number >= F_INDEX 
			     && (yyval.node)->v.number <= F_RE_RANGE) {
                        parse_node_t *node = (yyval.node);
                        int flag = 0;
                        do {
                            switch(node->kind) {
			    case NODE_PARAMETER:
				node->kind = NODE_PARAMETER_LVALUE;
				flag |= LV_ILLEGAL;
				break;
			    case NODE_TERNARY_OP:
				node->v.number = node->r.expr->v.number;
			    case NODE_OPCODE_1:
			    case NODE_UNARY_OP_1:
			    case NODE_BINARY_OP:
				if (node->v.number >= F_LOCAL 
				    && node->v.number <= F_MEMBER) {
				    node->v.number++;
				    flag |= LV_ILLEGAL;
				    break;
				} else if (node->v.number == F_INDEX ||
					 node->v.number == F_RINDEX) {
				    node->v.number++;
				    flag |= LV_INDEX;
				    break;
				} else if (node->v.number >= F_ADD_EQ
					   && node->v.number <= F_ASSIGN) {
				    if (!(flag & LV_INDEX)) {
					yyerror("Illegal lvalue, a possible lvalue is (x <assign> y)[a]");
				    }
				    if (node->r.expr->kind == NODE_BINARY_OP||
					node->r.expr->kind == NODE_TERNARY_OP){
					if (node->r.expr->v.number >= F_NN_RANGE_LVALUE && node->r.expr->v.number <= F_NR_RANGE_LVALUE)
					    yyerror("Illegal to have (x[a..b] <assign> y) to be the beginning of an lvalue");
				    }
				    flag = LV_ILLEGAL;
				    break;
				} else if (node->v.number >= F_NN_RANGE
					 && node->v.number <= F_RE_RANGE) {
				    if (flag & LV_RANGE) {
					yyerror("Can't do range lvalue of range lvalue.");
					flag |= LV_ILLEGAL;
					break;
				    }
                                    if (flag & LV_INDEX){
					yyerror("Can't do indexed lvalue of range lvalue.");
					flag |= LV_ILLEGAL;
					break;
				    }
				    if (node->v.number == F_NE_RANGE) {
					/* x[foo..] -> x[foo..<1] */
					parse_node_t *rchild = node->r.expr;
					node->kind = NODE_TERNARY_OP;
					CREATE_BINARY_OP(node->r.expr,
							 F_NR_RANGE_LVALUE,
							 0, 0, rchild);
					CREATE_NUMBER(node->r.expr->l.expr, 1);
				    } else if (node->v.number == F_RE_RANGE) {
					/* x[<foo..] -> x[<foo..<1] */
					parse_node_t *rchild = node->r.expr;
					node->kind = NODE_TERNARY_OP;
					CREATE_BINARY_OP(node->r.expr,
							 F_RR_RANGE_LVALUE,
							 0, 0, rchild);
					CREATE_NUMBER(node->r.expr->l.expr, 1);
				    } else
					node->r.expr->v.number++;
				    flag |= LV_RANGE;
				    node = node->r.expr->r.expr;
				    continue;
				}
			    default:
				yyerror("Illegal lvalue");
				flag = LV_ILLEGAL;
				break;
			    }   
                            if ((flag & LV_ILLEGAL) || !(node = node->r.expr)) break;
                        } while (1);
                        break;
		    } else 
			yyerror("Illegal lvalue");
		    break;
                }
            }
#line 4776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 2629 "grammar.y" /* yacc.c:1646  */
    {
#line 2615 "grammar.y.pre"
		(yyval.number) = ((yyvsp[0].number) << 8) | FP_EFUN;
	    }
#line 4785 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 2636 "grammar.y" /* yacc.c:1646  */
    { 
#line 2622 "grammar.y.pre"
		(yyval.number) = (yyvsp[-2].number);
	    }
#line 4794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 2641 "grammar.y" /* yacc.c:1646  */
    {
#line 2626 "grammar.y.pre"
		int val;
		
		if ((val=(yyvsp[0].ihe)->dn.local_num) >= 0){
		    (yyval.number) = (val << 8) | FP_L_VAR;
		} else if ((val=(yyvsp[0].ihe)->dn.global_num) >= 0) {
		    (yyval.number) = (val << 8) | FP_G_VAR;
		} else if ((val=(yyvsp[0].ihe)->dn.function_num) >=0) {
		    (yyval.number) = (val << 8)|FP_LOCAL;
		} else if ((val=(yyvsp[0].ihe)->dn.simul_num) >=0) {
		    (yyval.number) = (val << 8)|FP_SIMUL;
		} else if ((val=(yyvsp[0].ihe)->dn.efun_num) >=0) {
		    (yyval.number) = (val << 8)|FP_EFUN;
		}
	    }
#line 4815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 2662 "grammar.y" /* yacc.c:1646  */
    {
#line 2647 "grammar.y.pre"
              int i;
              if ((i = (yyvsp[0].ihe)->dn.local_num) != -1) {
		  type_of_locals_ptr[i] &= ~LOCAL_MOD_UNUSED;
		  if (type_of_locals_ptr[i] & LOCAL_MOD_REF)
		      CREATE_OPCODE_1((yyval.node), F_REF, type_of_locals_ptr[i] & ~LOCAL_MOD_REF,i & 0xff);
		  else
		      CREATE_OPCODE_1((yyval.node), F_LOCAL, type_of_locals_ptr[i], i & 0xff);
		  if (current_function_context)
		      current_function_context->num_locals++;
              } else
		  if ((i = (yyvsp[0].ihe)->dn.global_num) != -1) {
		      if (current_function_context)
			  current_function_context->bindable = FP_NOT_BINDABLE;
                          CREATE_OPCODE_1((yyval.node), F_GLOBAL,
				      VAR_TEMP(i)->type & ~DECL_MODS, i);
		      if (VAR_TEMP(i)->type & DECL_HIDDEN) {
			  char buf[256];
			  char *end = EndOf(buf);
			  char *p;

			  p = strput(buf, end, "Illegal to use private variable '");
			  p = strput(p, end, (yyvsp[0].ihe)->name);
			  p = strput(p, end, "'");
			  yyerror(buf);
		      }
		  } else {
		      char buf[256];
		      char *end = EndOf(buf);
		      char *p;
		      
		      p = strput(buf, end, "Undefined variable '");
		      p = strput(p, end, (yyvsp[0].ihe)->name);
		      p = strput(p, end, "'");
		      if (current_number_of_locals < CFG_MAX_LOCAL_VARIABLES) {
			  add_local_name((yyvsp[0].ihe)->name, TYPE_ANY);
		      }
		      CREATE_ERROR((yyval.node));
		      yyerror(buf);
		  }
	    }
#line 4862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 2705 "grammar.y" /* yacc.c:1646  */
    {
#line 2689 "grammar.y.pre"
		char buf[256];
		char *end = EndOf(buf);
		char *p;
		
		p = strput(buf, end, "Undefined variable '");
		p = strput(p, end, (yyvsp[0].string));
		p = strput(p, end, "'");
                if (current_number_of_locals < CFG_MAX_LOCAL_VARIABLES) {
                    add_local_name((yyvsp[0].string), TYPE_ANY);
                }
                CREATE_ERROR((yyval.node));
                yyerror(buf);
                scratch_free((yyvsp[0].string));
            }
#line 4883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 2722 "grammar.y" /* yacc.c:1646  */
    {
#line 2705 "grammar.y.pre"
		CREATE_PARAMETER((yyval.node), TYPE_ANY, (yyvsp[0].number));
            }
#line 4892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 2727 "grammar.y" /* yacc.c:1646  */
    {
#line 2709 "grammar.y.pre"
		(yyval.contextp) = current_function_context;
		/* already flagged as an error */
		if (current_function_context)
		    current_function_context = current_function_context->parent;
            }
#line 4904 "y.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 2735 "grammar.y" /* yacc.c:1646  */
    {
#line 2716 "grammar.y.pre"
		parse_node_t *node;

		current_function_context = (yyvsp[-2].contextp);

		if (!current_function_context || current_function_context->num_parameters == -2) {
		    /* This was illegal, and error'ed when the '$' token
		     * was returned.
		     */
		    CREATE_ERROR((yyval.node));
		} else {
		    CREATE_OPCODE_1((yyval.node), F_LOCAL, (yyvsp[-1].node)->type,
				    current_function_context->values_list->kind++);

		    node = new_node_no_line();
		    node->type = 0;
		    current_function_context->values_list->l.expr->r.expr = node;
		    current_function_context->values_list->l.expr = node;
		    node->r.expr = 0;
		    node->v.expr = (yyvsp[-1].node);
		}
	    }
#line 4932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 2759 "grammar.y" /* yacc.c:1646  */
    {
#line 2739 "grammar.y.pre"
		if ((yyvsp[-2].node)->type == TYPE_ANY) {
		    int cmi;
		    unsigned char tp;
		    
		    if ((cmi = lookup_any_class_member((yyvsp[0].string), &tp)) != -1) {
			CREATE_UNARY_OP_1((yyval.node), F_MEMBER, tp, (yyvsp[-2].node), 0);
			(yyval.node)->l.number = cmi;
		    } else {
			CREATE_ERROR((yyval.node));
		    }
		} else if (!IS_CLASS((yyvsp[-2].node)->type)) {
		    yyerror("Left argument of -> is not a class");
		    CREATE_ERROR((yyval.node));
		} else {
		    CREATE_UNARY_OP_1((yyval.node), F_MEMBER, 0, (yyvsp[-2].node), 0);
		    (yyval.node)->l.number = lookup_class_member(CLASS_IDX((yyvsp[-2].node)->type),
						       (yyvsp[0].string),
						       &((yyval.node)->type));
		}
		    
		scratch_free((yyvsp[0].string));
            }
#line 4961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 2784 "grammar.y" /* yacc.c:1646  */
    {
#line 2763 "grammar.y.pre"
                (yyval.node) = make_range_node(F_NN_RANGE, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node));
            }
#line 4970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 2789 "grammar.y" /* yacc.c:1646  */
    {
#line 2772 "grammar.y.pre"
                (yyval.node) = make_range_node(F_RN_RANGE, (yyvsp[-6].node), (yyvsp[-3].node), (yyvsp[-1].node));
            }
#line 4979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 2794 "grammar.y" /* yacc.c:1646  */
    {
#line 2776 "grammar.y.pre"
		if ((yyvsp[-1].node)->kind == NODE_NUMBER && (yyvsp[-1].node)->v.number <= 1)
		    (yyval.node) = make_range_node(F_RE_RANGE, (yyvsp[-7].node), (yyvsp[-4].node), 0);
		else
		    (yyval.node) = make_range_node(F_RR_RANGE, (yyvsp[-7].node), (yyvsp[-4].node), (yyvsp[-1].node));
            }
#line 4991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 2802 "grammar.y" /* yacc.c:1646  */
    {
#line 2783 "grammar.y.pre"
		if ((yyvsp[-1].node)->kind == NODE_NUMBER && (yyvsp[-1].node)->v.number <= 1)
		    (yyval.node) = make_range_node(F_NE_RANGE, (yyvsp[-6].node), (yyvsp[-4].node), 0);
		else
		    (yyval.node) = make_range_node(F_NR_RANGE, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-1].node));
            }
#line 5003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 2810 "grammar.y" /* yacc.c:1646  */
    {
#line 2790 "grammar.y.pre"
                (yyval.node) = make_range_node(F_NE_RANGE, (yyvsp[-4].node), (yyvsp[-2].node), 0);
            }
#line 5012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 2815 "grammar.y" /* yacc.c:1646  */
    {
#line 2794 "grammar.y.pre"
                (yyval.node) = make_range_node(F_RE_RANGE, (yyvsp[-5].node), (yyvsp[-2].node), 0);
            }
#line 5021 "y.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 2820 "grammar.y" /* yacc.c:1646  */
    {
#line 2798 "grammar.y.pre"
                if (IS_NODE((yyvsp[-4].node), NODE_CALL, F_AGGREGATE)
		    && (yyvsp[-1].node)->kind == NODE_NUMBER) {
                    int i = (yyvsp[-1].node)->v.number;
                    if (i < 1 || i > (yyvsp[-4].node)->l.number)
                        yyerror("Illegal index to array constant.");
                    else {
                        parse_node_t *node = (yyvsp[-4].node)->r.expr;
                        i = (yyvsp[-4].node)->l.number - i;
                        while (i--)
                            node = node->r.expr;
                        (yyval.node) = node->v.expr;
                        break;
                    }
                }
		CREATE_BINARY_OP((yyval.node), F_RINDEX, 0, (yyvsp[-1].node), (yyvsp[-4].node));
                if (exact_types) {
		    switch((yyvsp[-4].node)->type) {
		    case TYPE_MAPPING:
			yyerror("Illegal index for mapping.");
		    case TYPE_ANY:
			(yyval.node)->type = TYPE_ANY;
			break;
		    case TYPE_STRING:
		    case TYPE_BUFFER:
			(yyval.node)->type = TYPE_NUMBER;
			if (!IS_TYPE((yyvsp[-1].node)->type,TYPE_NUMBER))
			    type_error("Bad type of index", (yyvsp[-1].node)->type);
			break;
			
		    default:
			if ((yyvsp[-4].node)->type & TYPE_MOD_ARRAY) {
			    (yyval.node)->type = (yyvsp[-4].node)->type & ~TYPE_MOD_ARRAY;
			    if (!IS_TYPE((yyvsp[-1].node)->type,TYPE_NUMBER))
				type_error("Bad type of index", (yyvsp[-1].node)->type);
			} else {
			    type_error("Value indexed has a bad type ", (yyvsp[-4].node)->type);
			    (yyval.node)->type = TYPE_ANY;
			}
		    }
		} else (yyval.node)->type = TYPE_ANY;
            }
#line 5069 "y.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 2864 "grammar.y" /* yacc.c:1646  */
    {
#line 2841 "grammar.y.pre"
		/* Something stupid like ({ 1, 2, 3 })[1]; we take the
		 * time to optimize this because people who don't understand
		 * the preprocessor often write things like:
		 *
		 * #define MY_ARRAY ({ "foo", "bar", "bazz" })
		 * ...
		 * ... MY_ARRAY[1] ...
		 *
		 * which of course expands to the above.
		 */
                if (IS_NODE((yyvsp[-3].node), NODE_CALL, F_AGGREGATE) && (yyvsp[-1].node)->kind == NODE_NUMBER) {
                    int i = (yyvsp[-1].node)->v.number;
                    if (i < 0 || i >= (yyvsp[-3].node)->l.number)
                        yyerror("Illegal index to array constant.");
                    else {
                        parse_node_t *node = (yyvsp[-3].node)->r.expr;
                        while (i--)
                            node = node->r.expr;
                        (yyval.node) = node->v.expr;
                        break;
                    }
                }
                CREATE_BINARY_OP((yyval.node), F_INDEX, 0, (yyvsp[-1].node), (yyvsp[-3].node));
                if (exact_types) {
		    switch((yyvsp[-3].node)->type) {
		    case TYPE_MAPPING:
		    case TYPE_ANY:
			(yyval.node)->type = TYPE_ANY;
			break;
		    case TYPE_STRING:
		    case TYPE_BUFFER:
			(yyval.node)->type = TYPE_NUMBER;
			if (!IS_TYPE((yyvsp[-1].node)->type,TYPE_NUMBER))
			    type_error("Bad type of index", (yyvsp[-1].node)->type);
			break;
			
		    default:
			if ((yyvsp[-3].node)->type & TYPE_MOD_ARRAY) {
			    (yyval.node)->type = (yyvsp[-3].node)->type & ~TYPE_MOD_ARRAY;
			    if (!IS_TYPE((yyvsp[-1].node)->type,TYPE_NUMBER))
				type_error("Bad type of index", (yyvsp[-1].node)->type);
			} else {
			    type_error("Value indexed has a bad type ", (yyvsp[-3].node)->type);
			    (yyval.node)->type = TYPE_ANY;
			}
                    }
                } else (yyval.node)->type = TYPE_ANY;
            }
#line 5124 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 2916 "grammar.y" /* yacc.c:1646  */
    {
#line 2896 "grammar.y.pre"
		(yyval.node) = (yyvsp[-1].node);
	    }
#line 5133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 2922 "grammar.y" /* yacc.c:1646  */
    {
#line 2904 "grammar.y.pre"
	        if ((yyvsp[0].type) != TYPE_FUNCTION) yyerror("Reserved type name unexpected.");
		(yyval.func_block).num_local = current_number_of_locals;
		(yyval.func_block).max_num_locals = max_num_locals;
		(yyval.func_block).context = context;
		(yyval.func_block).save_current_type = current_type;
		(yyval.func_block).save_exact_types = exact_types;
	        if (type_of_locals_ptr + max_num_locals + CFG_MAX_LOCAL_VARIABLES >= &type_of_locals[type_of_locals_size])
		    reallocate_locals();
		deactivate_current_locals();
		locals_ptr += current_number_of_locals;
		type_of_locals_ptr += max_num_locals;
		max_num_locals = current_number_of_locals = 0;
		push_function_context();
		current_function_context->num_parameters = -1;
		exact_types = TYPE_ANY;
		context = 0;
            }
#line 5157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 2942 "grammar.y" /* yacc.c:1646  */
    {
#line 2923 "grammar.y.pre"
		if ((yyvsp[-2].argument).flags & ARG_IS_VARARGS) {
		    yyerror("Anonymous varargs functions aren't implemented");
		}
		if (!(yyvsp[0].decl).node) {
		    CREATE_RETURN((yyvsp[0].decl).node, 0);
		} else if ((yyvsp[0].decl).node->kind != NODE_RETURN &&
			   ((yyvsp[0].decl).node->kind != NODE_TWO_VALUES || (yyvsp[0].decl).node->r.expr->kind != NODE_RETURN)) {
		    parse_node_t *replacement;
		    CREATE_STATEMENTS(replacement, (yyvsp[0].decl).node, 0);
		    CREATE_RETURN(replacement->r.expr, 0);
		    (yyvsp[0].decl).node = replacement;
		}
		
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_ANON_FUNC;
		(yyval.node)->type = TYPE_FUNCTION;
		(yyval.node)->l.number = (max_num_locals - (yyvsp[-2].argument).num_arg);
		(yyval.node)->r.expr = (yyvsp[0].decl).node;
		(yyval.node)->v.number = (yyvsp[-2].argument).num_arg;
		if (current_function_context->bindable)
		    (yyval.node)->v.number |= 0x10000;
		free_all_local_names(1);
		
		current_number_of_locals = (yyvsp[-4].func_block).num_local;
		max_num_locals = (yyvsp[-4].func_block).max_num_locals;
		context = (yyvsp[-4].func_block).context;
		current_type = (yyvsp[-4].func_block).save_current_type;
		exact_types = (yyvsp[-4].func_block).save_exact_types;
		pop_function_context();
		
		locals_ptr -= current_number_of_locals;
		type_of_locals_ptr -= max_num_locals;
		reactivate_current_locals();
	    }
#line 5198 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 2979 "grammar.y" /* yacc.c:1646  */
    {
#line 2963 "grammar.y.pre"
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_FUNCTION_CONSTRUCTOR;
		(yyval.node)->type = TYPE_FUNCTION;
		(yyval.node)->r.expr = 0;
		switch ((yyvsp[0].number) & 0xff) {
		case FP_L_VAR:
		    yyerror("Illegal to use local variable in a functional.");
		    CREATE_NUMBER((yyval.node)->l.expr, 0);
		    (yyval.node)->l.expr->r.expr = 0;
		    (yyval.node)->l.expr->l.expr = 0;
		    (yyval.node)->v.number = FP_FUNCTIONAL;
		    break;
		case FP_G_VAR:
		    CREATE_OPCODE_1((yyval.node)->l.expr, F_GLOBAL, 0, (yyvsp[0].number) >> 8);
		    (yyval.node)->v.number = FP_FUNCTIONAL | FP_NOT_BINDABLE;
		    if (VAR_TEMP((yyval.node)->l.expr->l.number)->type & DECL_HIDDEN) {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			
			p = strput(buf, end, "Illegal to use private variable '");
			p = strput(p, end, VAR_TEMP((yyval.node)->l.expr->l.number)->name);
			p = strput(p, end, "'");
			yyerror(buf);
		    }
		    break;
		default:
		    (yyval.node)->v.number = (yyvsp[0].number);
		    break;
		}
	    }
#line 5236 "y.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 3013 "grammar.y" /* yacc.c:1646  */
    {
#line 2996 "grammar.y.pre"
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_FUNCTION_CONSTRUCTOR;
		(yyval.node)->type = TYPE_FUNCTION;
		(yyval.node)->v.number = (yyvsp[-4].number);
		(yyval.node)->r.expr = (yyvsp[-2].node);
		
		switch ((yyvsp[-4].number) & 0xff) {
		case FP_EFUN: {
		    int *argp;
		    int f = (yyvsp[-4].number) >>8;
		    int num = (yyvsp[-2].node)->kind;
		    int max_arg = predefs[f].max_args;
		    
		    if (num > max_arg && max_arg != -1) {
			parse_node_t *pn = (yyvsp[-2].node);
			
			while (pn) {
			    if (pn->type & 1) break;
			    pn = pn->r.expr;
			}
			
			if (!pn) {
			    char bff[256];
			    char *end = EndOf(bff);
			    char *p;
			    
			    p = strput(bff, end, "Too many arguments to ");
			    p = strput(p, end, predefs[f].word);
			    yyerror(bff);
			}
		    } else if (max_arg != -1 && exact_types) {
			/*
			 * Now check all types of arguments to efuns.
			 */
			int i, argn, tmp;
			parse_node_t *enode = (yyvsp[-2].node);
			argp = &efun_arg_types[predefs[f].arg_index];
			
			for (argn = 0; argn < num; argn++) {
			    if (enode->type & 1) break;
			    
			    tmp = enode->v.expr->type;
			    for (i=0; !compatible_types(tmp, argp[i])
				 && argp[i] != 0; i++)
				;
			    if (argp[i] == 0) {
				char buf[256];
				char *end = EndOf(buf);
				char *p;

				p = strput(buf, end, "Bad argument ");
				p = strput_int(p, end, argn+1);
				p = strput(p, end, " to efun ");
				p = strput(p, end, predefs[f].word);
				p = strput(p, end, "()");
				yyerror(buf);
			    } else {
				/* this little section necessary b/c in the
				   case float | int we dont want to do
				   promoting. */
				if (tmp == TYPE_NUMBER && argp[i] == TYPE_REAL) {
				    for (i++; argp[i] && argp[i] != TYPE_NUMBER; i++)
					;
				    if (!argp[i])
					enode->v.expr = promote_to_float(enode->v.expr);
				}
				if (tmp == TYPE_REAL && argp[i] == TYPE_NUMBER) {
				    for (i++; argp[i] && argp[i] != TYPE_REAL; i++)
					;
				    if (!argp[i])
					enode->v.expr = promote_to_int(enode->v.expr);
				}
			    }
			    while (argp[i] != 0)
				i++;
			    argp += i + 1;
			    enode = enode->r.expr;
			}
		    }
		    break;
		}
		case FP_L_VAR:
		case FP_G_VAR:
		    yyerror("Can't give parameters to functional.");
		    break;
		}
	    }
#line 5330 "y.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 3103 "grammar.y" /* yacc.c:1646  */
    {
#line 3085 "grammar.y.pre"
		 if (current_function_context->num_locals)
		     yyerror("Illegal to use local variable in functional.");
		 if (current_function_context->values_list->r.expr)
		     current_function_context->values_list->r.expr->kind = current_function_context->values_list->kind;
		 
		 (yyval.node) = new_node();
		 (yyval.node)->kind = NODE_FUNCTION_CONSTRUCTOR;
		 (yyval.node)->type = TYPE_FUNCTION;
		 (yyval.node)->l.expr = (yyvsp[-2].node);
		 if ((yyvsp[-2].node)->kind == NODE_STRING)
		     yywarn("Function pointer returning string constant is NOT a function call");
		 (yyval.node)->r.expr = current_function_context->values_list->r.expr;
		 (yyval.node)->v.number = FP_FUNCTIONAL + current_function_context->bindable
		     + (current_function_context->num_parameters << 8);
		 pop_function_context();
             }
#line 5353 "y.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 3122 "grammar.y" /* yacc.c:1646  */
    {
#line 3103 "grammar.y.pre"
		CREATE_CALL((yyval.node), F_AGGREGATE_ASSOC, TYPE_MAPPING, (yyvsp[-2].node));
	    }
#line 5362 "y.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 3127 "grammar.y" /* yacc.c:1646  */
    {
#line 3107 "grammar.y.pre"
		CREATE_CALL((yyval.node), F_AGGREGATE, TYPE_ANY | TYPE_MOD_ARRAY, (yyvsp[-2].node));
	    }
#line 5371 "y.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 3135 "grammar.y" /* yacc.c:1646  */
    {
#line 3114 "grammar.y.pre"
		(yyval.node) = (yyvsp[0].decl).node;
	    }
#line 5380 "y.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 3140 "grammar.y" /* yacc.c:1646  */
    {
#line 3118 "grammar.y.pre"
		(yyval.node) = insert_pop_value((yyvsp[-1].node));
	    }
#line 5389 "y.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 3148 "grammar.y" /* yacc.c:1646  */
    {
#line 3125 "grammar.y.pre"
		(yyval.number) = context;
		context = SPECIAL_CONTEXT;
	    }
#line 5399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 3154 "grammar.y" /* yacc.c:1646  */
    {
#line 3130 "grammar.y.pre"
		CREATE_CATCH((yyval.node), (yyvsp[0].node));
		context = (yyvsp[-1].number);
	    }
#line 5409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 3164 "grammar.y" /* yacc.c:1646  */
    {
#line 3154 "grammar.y.pre"
		int p = (yyvsp[-1].node)->v.number;
		CREATE_LVALUE_EFUN((yyval.node), TYPE_NUMBER, (yyvsp[-1].node));
		CREATE_BINARY_OP_1((yyval.node)->l.expr, F_SSCANF, 0, (yyvsp[-4].node), (yyvsp[-2].node), p);
	    }
#line 5420 "y.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 3174 "grammar.y" /* yacc.c:1646  */
    {
#line 3163 "grammar.y.pre"
		int p = (yyvsp[-1].node)->v.number;
		CREATE_LVALUE_EFUN((yyval.node), TYPE_NUMBER, (yyvsp[-1].node));
		CREATE_TERNARY_OP_1((yyval.node)->l.expr, F_PARSE_COMMAND, 0, 
				    (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), p);
	    }
#line 5432 "y.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 3185 "grammar.y" /* yacc.c:1646  */
    {
#line 3173 "grammar.y.pre"
		(yyval.number) = context;
		context = SPECIAL_CONTEXT;
	    }
#line 5442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 3191 "grammar.y" /* yacc.c:1646  */
    {
#line 3178 "grammar.y.pre"
		CREATE_TIME_EXPRESSION((yyval.node), (yyvsp[0].node));
		context = (yyvsp[-1].number);
	    }
#line 5452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 3200 "grammar.y" /* yacc.c:1646  */
    {
#line 3186 "grammar.y.pre"
	        (yyval.node) = new_node_no_line();
		(yyval.node)->r.expr = 0;
	        (yyval.node)->v.number = 0;
	    }
#line 5463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 3207 "grammar.y" /* yacc.c:1646  */
    {
#line 3192 "grammar.y.pre"
		parse_node_t *insert;
		
		(yyval.node) = (yyvsp[0].node);
		insert = new_node_no_line();
		insert->r.expr = (yyvsp[0].node)->r.expr;
		insert->l.expr = (yyvsp[-1].node);
		(yyvsp[0].node)->r.expr = insert;
		(yyval.node)->v.number++;
	    }
#line 5479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 3222 "grammar.y" /* yacc.c:1646  */
    {
#line 3206 "grammar.y.pre"
		CREATE_STRING((yyval.node), (yyvsp[0].string));
		scratch_free((yyvsp[0].string));
	    }
#line 5489 "y.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 3232 "grammar.y" /* yacc.c:1646  */
    {
#line 3215 "grammar.y.pre"
		(yyval.string) = (yyvsp[-1].string);
	    }
#line 5498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 3237 "grammar.y" /* yacc.c:1646  */
    {
#line 3219 "grammar.y.pre"
		(yyval.string) = scratch_join((yyvsp[-2].string), (yyvsp[0].string));
	    }
#line 5507 "y.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 3246 "grammar.y" /* yacc.c:1646  */
    {
#line 3227 "grammar.y.pre"
		(yyval.string) = scratch_join((yyvsp[-1].string), (yyvsp[0].string));
	    }
#line 5516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 3253 "grammar.y" /* yacc.c:1646  */
    {
#line 3233 "grammar.y.pre"
	(yyval.node) = new_node();
	(yyval.node)->l.expr = (parse_node_t *)(yyvsp[-2].string);
	(yyval.node)->v.expr = (yyvsp[0].node);
	(yyval.node)->r.expr = 0;
    }
#line 5528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 3264 "grammar.y" /* yacc.c:1646  */
    {
#line 3243 "grammar.y.pre"
	(yyval.node) = 0;
    }
#line 5537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 3269 "grammar.y" /* yacc.c:1646  */
    {
#line 3247 "grammar.y.pre"
	(yyval.node) = (yyvsp[0].node);
	(yyval.node)->r.expr = (yyvsp[-2].node);
    }
#line 5547 "y.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 3279 "grammar.y" /* yacc.c:1646  */
    {
#line 3256 "grammar.y.pre"
		(yyval.number) = context;
		(yyvsp[0].number) = num_refs;
		context |= ARG_LIST; 
	    }
#line 5558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 3286 "grammar.y" /* yacc.c:1646  */
    {
#line 3262 "grammar.y.pre"
		context = (yyvsp[-2].number);
		(yyval.node) = validate_efun_call((yyvsp[-4].number),(yyvsp[-1].node));
		(yyval.node) = check_refs(num_refs - (yyvsp[-3].number), (yyvsp[-1].node), (yyval.node));
		num_refs = (yyvsp[-3].number);
	    }
#line 5570 "y.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 3294 "grammar.y" /* yacc.c:1646  */
    {
#line 3269 "grammar.y.pre"
		(yyval.number) = context;
		(yyvsp[0].number) = num_refs;
		context |= ARG_LIST;
	    }
#line 5581 "y.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 3301 "grammar.y" /* yacc.c:1646  */
    {
#line 3275 "grammar.y.pre"
		context = (yyvsp[-2].number);
		(yyval.node) = validate_efun_call(new_efun, (yyvsp[-1].node));
		(yyval.node) = check_refs(num_refs - (yyvsp[-3].number), (yyvsp[-1].node), (yyval.node));
		num_refs = (yyvsp[-3].number);
            }
#line 5593 "y.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 3309 "grammar.y" /* yacc.c:1646  */
    {
#line 3282 "grammar.y.pre"
		parse_node_t *node;
		
		if ((yyvsp[-2].ihe)->dn.class_num == -1) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    
		    p = strput(buf, end, "Undefined class '");
		    p = strput(p, end, (yyvsp[-2].ihe)->name);
		    p = strput(p, end, "'");
		    yyerror(buf);
		    CREATE_ERROR((yyval.node));
		    node = (yyvsp[-1].node);
		    while (node) {
			scratch_free((char *)node->l.expr);
			node = node->r.expr;
		    }
		} else {
		    int type = (yyvsp[-2].ihe)->dn.class_num | TYPE_MOD_CLASS;
		    
		    if ((node = (yyvsp[-1].node))) {
			CREATE_TWO_VALUES((yyval.node), type, 0, 0);
			(yyval.node)->l.expr = reorder_class_values((yyvsp[-2].ihe)->dn.class_num,
							node);
			CREATE_OPCODE_1((yyval.node)->r.expr, F_NEW_CLASS,
					type, (yyvsp[-2].ihe)->dn.class_num);
			
		    } else {
			CREATE_OPCODE_1((yyval.node), F_NEW_EMPTY_CLASS,
					type, (yyvsp[-2].ihe)->dn.class_num);
		    }
		}
            }
#line 5633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 3345 "grammar.y" /* yacc.c:1646  */
    {
#line 3317 "grammar.y.pre"
		parse_node_t *node;
		char buf[256];
		char *end = EndOf(buf);
		char *p;

		p = strput(buf, end, "Undefined class '");
		p = strput(p, end, (yyvsp[-2].string));
		p = strput(p, end, "'");
		yyerror(buf);
		CREATE_ERROR((yyval.node));
		node = (yyvsp[-1].node);
		while (node) {
		    scratch_free((char *)node->l.expr);
		    node = node->r.expr;
		}
	    }
#line 5656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 3364 "grammar.y" /* yacc.c:1646  */
    {
#line 3335 "grammar.y.pre"
		(yyval.number) = context;
		(yyvsp[0].number) = num_refs;
		context |= ARG_LIST;
	    }
#line 5667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 3371 "grammar.y" /* yacc.c:1646  */
    {
#line 3341 "grammar.y.pre"
		int f;
		
		context = (yyvsp[-2].number);
		(yyval.node) = (yyvsp[-1].node);
		if ((f = (yyvsp[-4].ihe)->dn.function_num) != -1) {
		    if (current_function_context)
			current_function_context->bindable = FP_NOT_BINDABLE;
		    
		    (yyval.node)->kind = NODE_CALL_1;
		    (yyval.node)->v.number = F_CALL_FUNCTION_BY_ADDRESS;
		    (yyval.node)->l.number = f;
		    (yyval.node)->type = validate_function_call(f, (yyvsp[-1].node)->r.expr);
		} else if ((f=(yyvsp[-4].ihe)->dn.simul_num) != -1) {
		    (yyval.node)->kind = NODE_CALL_1;
		    (yyval.node)->v.number = F_SIMUL_EFUN;
		    (yyval.node)->l.number = f;
		    (yyval.node)->type = (SIMUL(f)->type) & ~DECL_MODS;
		} else if ((f=(yyvsp[-4].ihe)->dn.efun_num) != -1) {
		    (yyval.node) = validate_efun_call(f, (yyvsp[-1].node));
		} else {
		    /* This here is a really nasty case that only occurs with
		     * exact_types off.  The user has done something gross like:
		     *
		     * func() { int f; f(); } // if f was prototyped we wouldn't
		     * f() { }                // need this case
		     *
		     * Don't complain, just grok it.
		     */
		    int f;
		    
		    if (current_function_context)
			current_function_context->bindable = FP_NOT_BINDABLE;
		    
		    f = define_new_function((yyvsp[-4].ihe)->name, 0, 0, 
					    DECL_PUBLIC|FUNC_UNDEFINED, TYPE_ANY);
		    (yyval.node)->kind = NODE_CALL_1;
		    (yyval.node)->v.number = F_CALL_FUNCTION_BY_ADDRESS;
		    (yyval.node)->l.number = f;
		    (yyval.node)->type = TYPE_ANY; /* just a guess */
		    if (exact_types) {
			char buf[256];
			char *end = EndOf(buf);
			char *p;
			char *n = (yyvsp[-4].ihe)->name;
			if (*n == ':') n++;
			/* prevent some errors; by making it look like an
			 * inherited function we prevent redeclaration errors
			 * if it shows up later
			 */
			
			FUNCTION_FLAGS(f) &= ~FUNC_UNDEFINED;
			FUNCTION_FLAGS(f) |= (FUNC_INHERITED | FUNC_VARARGS);
			p = strput(buf, end, "Undefined function ");
			p = strput(p, end, n);
			yyerror(buf);
		    }
		}
		(yyval.node) = check_refs(num_refs - (yyvsp[-3].number), (yyvsp[-1].node), (yyval.node));
		num_refs = (yyvsp[-3].number);
	    }
#line 5734 "y.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 3434 "grammar.y" /* yacc.c:1646  */
    {
#line 3403 "grammar.y.pre"
		(yyval.number) = context;
		(yyvsp[0].number) = num_refs;
		context |= ARG_LIST;
	    }
#line 5745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 3441 "grammar.y" /* yacc.c:1646  */
    {
#line 3409 "grammar.y.pre"
	      char *name = (yyvsp[-4].string);

	      context = (yyvsp[-2].number);
	      (yyval.node) = (yyvsp[-1].node);
	      
	      if (current_function_context)
		  current_function_context->bindable = FP_NOT_BINDABLE;

	      if (*name == ':') {
		  int f;
		  
		  if ((f = arrange_call_inherited(name + 1, (yyval.node))) != -1)
		      /* Can't do this; f may not be the correct function
			 entry.  It might be overloaded.
			 
		      validate_function_call(f, $$->r.expr)
		      */
		      ;
	      } else {
		  int f;
		  ident_hash_elem_t *ihe;
		  
		  f = (ihe = lookup_ident(name)) ? ihe->dn.function_num : -1;
		  (yyval.node)->kind = NODE_CALL_1;
		  (yyval.node)->v.number = F_CALL_FUNCTION_BY_ADDRESS;
		  if (f!=-1) {
		      /* The only way this can happen is if function_name
		       * below made the function name.  The lexer would
		       * return L_DEFINED_NAME instead.
		       */
		      (yyval.node)->type = validate_function_call(f, (yyvsp[-1].node)->r.expr);
		  } else {
		      f = define_new_function(name, 0, 0, 
					      DECL_PUBLIC|FUNC_UNDEFINED, TYPE_ANY);
		  }
		  (yyval.node)->l.number = f;
		  /*
		   * Check if this function has been defined.
		   * But, don't complain yet about functions defined
		   * by inheritance.
		   */
		  if (exact_types && (FUNCTION_FLAGS(f) & FUNC_UNDEFINED)) {
		      char buf[256];
		      char *end = EndOf(buf);
		      char *p;
		      char *n = (yyvsp[-4].string);
		      if (*n == ':') n++;
		      /* prevent some errors */
		      FUNCTION_FLAGS(f) &= ~FUNC_UNDEFINED;
		      FUNCTION_FLAGS(f) |= (FUNC_INHERITED | FUNC_VARARGS);
		      p = strput(buf, end, "Undefined function ");
		      p = strput(p, end, n);
		      yyerror(buf);
		  }
		  if (!(FUNCTION_FLAGS(f) & FUNC_UNDEFINED))
		      (yyval.node)->type = FUNCTION_DEF(f)->type;
		  else
		      (yyval.node)->type = TYPE_ANY;  /* Just a guess */
	      }
	      (yyval.node) = check_refs(num_refs - (yyvsp[-3].number), (yyvsp[-1].node), (yyval.node));
	      num_refs = (yyvsp[-3].number);
	      scratch_free(name);
	  }
#line 5815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 3507 "grammar.y" /* yacc.c:1646  */
    {
#line 3474 "grammar.y.pre"
		(yyval.number) = context;
		(yyvsp[0].number) = num_refs;
		context |= ARG_LIST;
	    }
#line 5826 "y.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 3514 "grammar.y" /* yacc.c:1646  */
    {
#line 3480 "grammar.y.pre"
		ident_hash_elem_t *ihe;
		int f;
		parse_node_t *pn1, *pn2;
		
		(yyvsp[-1].node)->v.number += 2;

		pn1 = new_node_no_line();
		pn1->type = 0;
		pn1->v.expr = (yyvsp[-6].node);
		pn1->kind = (yyvsp[-1].node)->v.number;
		
		pn2 = new_node_no_line();
		pn2->type = 0;
		CREATE_STRING(pn2->v.expr, (yyvsp[-4].string));
		scratch_free((yyvsp[-4].string));
		
		/* insert the two nodes */
		pn2->r.expr = (yyvsp[-1].node)->r.expr;
		pn1->r.expr = pn2;
		(yyvsp[-1].node)->r.expr = pn1;
		
		if (!(yyvsp[-1].node)->l.expr) (yyvsp[-1].node)->l.expr = pn2;
		    
		context = (yyvsp[-2].number);
		ihe = lookup_ident("call_other");

		if ((f = ihe->dn.simul_num) != -1) {
		    (yyval.node) = (yyvsp[-1].node);
		    (yyval.node)->kind = NODE_CALL_1;
		    (yyval.node)->v.number = F_SIMUL_EFUN;
		    (yyval.node)->l.number = f;
		    (yyval.node)->type = (SIMUL(f)->type) & ~DECL_MODS;
		} else {
		    (yyval.node) = validate_efun_call(arrow_efun, (yyvsp[-1].node));
#ifdef CAST_CALL_OTHERS
		    (yyval.node)->type = TYPE_UNKNOWN;
#else
		    (yyval.node)->type = TYPE_ANY;
#endif		  
		}
		(yyval.node) = check_refs(num_refs - (yyvsp[-3].number), (yyvsp[-1].node), (yyval.node));
		num_refs = (yyvsp[-3].number);
	    }
#line 5876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 3560 "grammar.y" /* yacc.c:1646  */
    {
#line 3525 "grammar.y.pre"
		(yyval.number) = context;
		(yyvsp[0].number) = num_refs;
		context |= ARG_LIST;
	    }
#line 5887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 3567 "grammar.y" /* yacc.c:1646  */
    {
#line 3531 "grammar.y.pre"
	        parse_node_t *expr;

		context = (yyvsp[-2].number);
		(yyval.node) = (yyvsp[-1].node);
		(yyval.node)->kind = NODE_EFUN;
		(yyval.node)->l.number = (yyval.node)->v.number + 1;
		(yyval.node)->v.number = predefs[evaluate_efun].token;
#ifdef CAST_CALL_OTHERS
		(yyval.node)->type = TYPE_UNKNOWN;
#else
		(yyval.node)->type = TYPE_ANY;
#endif
		expr = new_node_no_line();
		expr->type = 0;
		expr->v.expr = (yyvsp[-5].node);
		expr->r.expr = (yyval.node)->r.expr;
		(yyval.node)->r.expr = expr;
		(yyval.node) = check_refs(num_refs - (yyvsp[-3].number), (yyvsp[-1].node), (yyval.node));
		num_refs = (yyvsp[-3].number);
	    }
#line 5914 "y.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 3591 "grammar.y" /* yacc.c:1646  */
    {
#line 3554 "grammar.y.pre"
	svalue_t *res;
	ident_hash_elem_t *ihe;

	(yyval.number) = (ihe = lookup_ident((yyvsp[0].string))) ? ihe->dn.efun_num : -1;
	if ((yyval.number) == -1) {
	    char buf[256];
	    char *end = EndOf(buf);
	    char *p;
	    
	    p = strput(buf, end, "Unknown efun: ");
	    p = strput(p, end, (yyvsp[0].string));
	    yyerror(buf);
	} else {
	    push_malloced_string(the_file_name(current_file));
	    share_and_push_string((yyvsp[0].string));
	    push_malloced_string(add_slash(main_file_name()));
	    res = safe_apply_master_ob(APPLY_VALID_OVERRIDE, 3);
	    if (!MASTER_APPROVED(res)) {
		yyerror("Invalid simulated efunction override");
		(yyval.number) = -1;
	    }
	}
	scratch_free((yyvsp[0].string));
      }
#line 5945 "y.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 3617 "grammar.y" /* yacc.c:1646  */
    {
#line 3579 "grammar.y.pre"
	svalue_t *res;
	
	push_malloced_string(the_file_name(current_file));
	push_constant_string("new");
	push_malloced_string(add_slash(main_file_name()));
	res = safe_apply_master_ob(APPLY_VALID_OVERRIDE, 3);
	if (!MASTER_APPROVED(res)) {
	    yyerror("Invalid simulated efunction override");
	    (yyval.number) = -1;
	} else (yyval.number) = new_efun;
      }
#line 5963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 3635 "grammar.y" /* yacc.c:1646  */
    {
#line 3596 "grammar.y.pre"
		int l = strlen((yyvsp[0].string)) + 1;
		char *p;
		/* here we be a bit cute.  we put a : on the front so we
		 * don't have to strchr for it.  Here we do:
		 * "name" -> ":::name"
		 */
		(yyval.string) = scratch_realloc((yyvsp[0].string), l + 3);
		p = (yyval.string) + l;
		while (p--,l--)
		    *(p+3) = *p;
		strncpy((yyval.string), ":::", 3);
	    }
#line 5982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 3650 "grammar.y" /* yacc.c:1646  */
    {
#line 3610 "grammar.y.pre"
		int z, l = strlen((yyvsp[0].string)) + 1;
		char *p;
		/* <type> and "name" -> ":type::name" */
		z = strlen(compiler_type_names[(yyvsp[-2].type)]) + 3; /* length of :type:: */
		(yyval.string) = scratch_realloc((yyvsp[0].string), l + z);
		p = (yyval.string) + l;
		while (p--,l--)
		    *(p+z) = *p;
		(yyval.string)[0] = ':';
		strncpy((yyval.string) + 1, compiler_type_names[(yyvsp[-2].type)], z - 3);
		(yyval.string)[z-2] = ':';
		(yyval.string)[z-1] = ':';
	    }
#line 6002 "y.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 3666 "grammar.y" /* yacc.c:1646  */
    {
#line 3625 "grammar.y.pre"
		int l = strlen((yyvsp[-2].string));
		/* "ob" and "name" -> ":ob::name" */
		(yyval.string) = scratch_alloc(l + strlen((yyvsp[0].string)) + 4);
		*((yyval.string)) = ':';
		strcpy((yyval.string) + 1, (yyvsp[-2].string));
		strcpy((yyval.string) + l + 1, "::");
		strcpy((yyval.string) + l + 3, (yyvsp[0].string));
		scratch_free((yyvsp[-2].string));
		scratch_free((yyvsp[0].string));
	    }
#line 6019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 3682 "grammar.y" /* yacc.c:1646  */
    {
#line 3640 "grammar.y.pre"
		/* x != 0 -> x */
		if (IS_NODE((yyvsp[-3].node), NODE_BINARY_OP, F_NE)) {
		    if (IS_NODE((yyvsp[-3].node)->r.expr, NODE_NUMBER, 0))
			(yyvsp[-3].node) = (yyvsp[-3].node)->l.expr;
		    else if (IS_NODE((yyvsp[-3].node)->l.expr, NODE_NUMBER, 0))
			     (yyvsp[-3].node) = (yyvsp[-3].node)->r.expr;
		}

		/* TODO: should optimize if (0), if (1) here.  
		 * Also generalize this.
		 */

		if ((yyvsp[-1].node) == 0) {
		    if ((yyvsp[0].node) == 0) {
			/* if (x) ; -> x; */
			(yyval.node) = pop_value((yyvsp[-3].node));
			break;
		    } else {
			/* if (x) {} else y; -> if (!x) y; */
			parse_node_t *repl;
			
			CREATE_UNARY_OP(repl, F_NOT, TYPE_NUMBER, (yyvsp[-3].node));
			(yyvsp[-3].node) = repl;
			(yyvsp[-1].node) = (yyvsp[0].node);
			(yyvsp[0].node) = 0;
		    }
		}
		CREATE_IF((yyval.node), (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));
	    }
#line 6055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 3717 "grammar.y" /* yacc.c:1646  */
    {
#line 3674 "grammar.y.pre"
		(yyval.node) = 0;
	    }
#line 6064 "y.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 3722 "grammar.y" /* yacc.c:1646  */
    {
#line 3678 "grammar.y.pre"
		(yyval.node) = (yyvsp[0].node);
            }
#line 6073 "y.tab.c" /* yacc.c:1646  */
    break;


#line 6077 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 3727 "grammar.y" /* yacc.c:1906  */



#line 3683 "grammar.y.pre"

// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





// "%code requires" blocks.
#line 12 "hammer_fgd.y"

	

#include "..\application.h"
#include "..\common.h"
#include "..\hammer_fgd.h"

GoldSource::FGDTokenTypes yylex(GoldSource::HammerFGDFile* ctx);


#line 54 "hammer_fgd.bison.cpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif

#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

namespace yy {
#line 194 "hammer_fgd.bison.cpp"




  /// A Bison parser.
  class HammerFGDParser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;


    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    SolidClass = 258,              // "@SolidClass"
    BaseClass = 259,               // "@BaseClass"
    PointClass = 260,              // "@PointClass"
    EqualsSign = 261,              // "="
    Colon = 262,                   // ":"
    StringLiteral = 263,           // StringLiteral
    Identifier = 264,              // Identifier
    Number = 265,                  // Number
    BaseDef = 266,                 // "base"
    OpeningParenthesis = 267,      // "("
    ClosingParenthesis = 268,      // ")"
    OpeningBracket = 269,          // "["
    ClosingBracket = 270,          // "]"
    SizeBoundingBox = 271,         // "size"
    Iconsprite = 272,              // "iconsprite"
    Color = 273,                   // "color"
    IntegerType = 274,             // "integer"
    Color255 = 275,                // "color255"
    String = 276,                  // "string"
    Sprite = 277,                  // "sprite"
    Studio = 278,                  // "studio"
    Flags = 279,                   // "flags"
    Choices = 280,                 // "choices"
    EndOfFile = 281                // EndOfFile
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 28, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_SolidClass = 3,                        // "@SolidClass"
        S_BaseClass = 4,                         // "@BaseClass"
        S_PointClass = 5,                        // "@PointClass"
        S_EqualsSign = 6,                        // "="
        S_Colon = 7,                             // ":"
        S_StringLiteral = 8,                     // StringLiteral
        S_Identifier = 9,                        // Identifier
        S_Number = 10,                           // Number
        S_BaseDef = 11,                          // "base"
        S_OpeningParenthesis = 12,               // "("
        S_ClosingParenthesis = 13,               // ")"
        S_OpeningBracket = 14,                   // "["
        S_ClosingBracket = 15,                   // "]"
        S_SizeBoundingBox = 16,                  // "size"
        S_Iconsprite = 17,                       // "iconsprite"
        S_Color = 18,                            // "color"
        S_IntegerType = 19,                      // "integer"
        S_Color255 = 20,                         // "color255"
        S_String = 21,                           // "string"
        S_Sprite = 22,                           // "sprite"
        S_Studio = 23,                           // "studio"
        S_Flags = 24,                            // "flags"
        S_Choices = 25,                          // "choices"
        S_EndOfFile = 26,                        // EndOfFile
        S_27_ = 27,                              // ','
        S_YYACCEPT = 28,                         // $accept
        S_FGDFile = 29,                          // FGDFile
        S_classes = 30,                          // classes
        S_SolidClassDef = 31,                    // SolidClassDef
        S_BasesClassesOpt = 32,                  // BasesClassesOpt
        S_PointClassDef = 33,                    // PointClassDef
        S_BaseClassDef = 34,                     // BaseClassDef
        S_PointEntityConstructorListOpt = 35,    // PointEntityConstructorListOpt
        S_PointEntityConstructor = 36,           // PointEntityConstructor
        S_ColorDefOpt = 37,                      // ColorDefOpt
        S_BoundingBoxDefOpt = 38,                // BoundingBoxDefOpt
        S_StudioDefOpt = 39,                     // StudioDefOpt
        S_IconspriteDefOpt = 40,                 // IconspriteDefOpt
        S_identifierListOpt = 41,                // identifierListOpt
        S_identifierList = 42,                   // identifierList
        S_ClassFieldsOpt = 43,                   // ClassFieldsOpt
        S_ClassFieldDef = 44,                    // ClassFieldDef
        S_FlagsValues = 45,                      // FlagsValues
        S_EnumValues = 46,                       // EnumValues
        S_TypeId = 47,                           // TypeId
        S_DefaultValueOpt = 48                   // DefaultValueOpt
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
      {
        switch (this->kind ())
    {
      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#else
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return HammerFGDParser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#else
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::YYEOF
                   || (token::YYerror <= tok && tok <= token::EndOfFile)
                   || tok == 44);
#endif
      }
    };

    /// Build a parser object.
    HammerFGDParser (GoldSource::HammerFGDFile* ctx_yyarg);
    virtual ~HammerFGDParser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    HammerFGDParser (const HammerFGDParser&) = delete;
    /// Non copyable.
    HammerFGDParser& operator= (const HammerFGDParser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF ()
      {
        return symbol_type (token::YYEOF);
      }
#else
      static
      symbol_type
      make_YYEOF ()
      {
        return symbol_type (token::YYEOF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror ()
      {
        return symbol_type (token::YYerror);
      }
#else
      static
      symbol_type
      make_YYerror ()
      {
        return symbol_type (token::YYerror);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF ()
      {
        return symbol_type (token::YYUNDEF);
      }
#else
      static
      symbol_type
      make_YYUNDEF ()
      {
        return symbol_type (token::YYUNDEF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SolidClass ()
      {
        return symbol_type (token::SolidClass);
      }
#else
      static
      symbol_type
      make_SolidClass ()
      {
        return symbol_type (token::SolidClass);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BaseClass ()
      {
        return symbol_type (token::BaseClass);
      }
#else
      static
      symbol_type
      make_BaseClass ()
      {
        return symbol_type (token::BaseClass);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PointClass ()
      {
        return symbol_type (token::PointClass);
      }
#else
      static
      symbol_type
      make_PointClass ()
      {
        return symbol_type (token::PointClass);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EqualsSign ()
      {
        return symbol_type (token::EqualsSign);
      }
#else
      static
      symbol_type
      make_EqualsSign ()
      {
        return symbol_type (token::EqualsSign);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Colon ()
      {
        return symbol_type (token::Colon);
      }
#else
      static
      symbol_type
      make_Colon ()
      {
        return symbol_type (token::Colon);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_StringLiteral ()
      {
        return symbol_type (token::StringLiteral);
      }
#else
      static
      symbol_type
      make_StringLiteral ()
      {
        return symbol_type (token::StringLiteral);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Identifier ()
      {
        return symbol_type (token::Identifier);
      }
#else
      static
      symbol_type
      make_Identifier ()
      {
        return symbol_type (token::Identifier);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Number ()
      {
        return symbol_type (token::Number);
      }
#else
      static
      symbol_type
      make_Number ()
      {
        return symbol_type (token::Number);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BaseDef ()
      {
        return symbol_type (token::BaseDef);
      }
#else
      static
      symbol_type
      make_BaseDef ()
      {
        return symbol_type (token::BaseDef);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OpeningParenthesis ()
      {
        return symbol_type (token::OpeningParenthesis);
      }
#else
      static
      symbol_type
      make_OpeningParenthesis ()
      {
        return symbol_type (token::OpeningParenthesis);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ClosingParenthesis ()
      {
        return symbol_type (token::ClosingParenthesis);
      }
#else
      static
      symbol_type
      make_ClosingParenthesis ()
      {
        return symbol_type (token::ClosingParenthesis);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OpeningBracket ()
      {
        return symbol_type (token::OpeningBracket);
      }
#else
      static
      symbol_type
      make_OpeningBracket ()
      {
        return symbol_type (token::OpeningBracket);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ClosingBracket ()
      {
        return symbol_type (token::ClosingBracket);
      }
#else
      static
      symbol_type
      make_ClosingBracket ()
      {
        return symbol_type (token::ClosingBracket);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SizeBoundingBox ()
      {
        return symbol_type (token::SizeBoundingBox);
      }
#else
      static
      symbol_type
      make_SizeBoundingBox ()
      {
        return symbol_type (token::SizeBoundingBox);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Iconsprite ()
      {
        return symbol_type (token::Iconsprite);
      }
#else
      static
      symbol_type
      make_Iconsprite ()
      {
        return symbol_type (token::Iconsprite);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Color ()
      {
        return symbol_type (token::Color);
      }
#else
      static
      symbol_type
      make_Color ()
      {
        return symbol_type (token::Color);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IntegerType ()
      {
        return symbol_type (token::IntegerType);
      }
#else
      static
      symbol_type
      make_IntegerType ()
      {
        return symbol_type (token::IntegerType);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Color255 ()
      {
        return symbol_type (token::Color255);
      }
#else
      static
      symbol_type
      make_Color255 ()
      {
        return symbol_type (token::Color255);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_String ()
      {
        return symbol_type (token::String);
      }
#else
      static
      symbol_type
      make_String ()
      {
        return symbol_type (token::String);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Sprite ()
      {
        return symbol_type (token::Sprite);
      }
#else
      static
      symbol_type
      make_Sprite ()
      {
        return symbol_type (token::Sprite);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Studio ()
      {
        return symbol_type (token::Studio);
      }
#else
      static
      symbol_type
      make_Studio ()
      {
        return symbol_type (token::Studio);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Flags ()
      {
        return symbol_type (token::Flags);
      }
#else
      static
      symbol_type
      make_Flags ()
      {
        return symbol_type (token::Flags);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Choices ()
      {
        return symbol_type (token::Choices);
      }
#else
      static
      symbol_type
      make_Choices ()
      {
        return symbol_type (token::Choices);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EndOfFile ()
      {
        return symbol_type (token::EndOfFile);
      }
#else
      static
      symbol_type
      make_EndOfFile ()
      {
        return symbol_type (token::EndOfFile);
      }
#endif


    class context
    {
    public:
      context (const HammerFGDParser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const HammerFGDParser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    HammerFGDParser (const HammerFGDParser&);
    /// Non copyable.
    HammerFGDParser& operator= (const HammerFGDParser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef signed char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const signed char yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const signed char yytable_[];

    static const signed char yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const signed char yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 85,     ///< Last index in yytable_.
      yynnts_ = 21,  ///< Number of nonterminal symbols.
      yyfinal_ = 13 ///< Termination state number.
    };


    // User arguments.
    GoldSource::HammerFGDFile* ctx;

  };

  HammerFGDParser::symbol_kind_type
  HammerFGDParser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    27,     2,     2,     2,     2,     2,
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
      25,    26
    };
    // Last valid token kind.
    const int code_max = 281;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

  // basic_symbol.
  template <typename Base>
  HammerFGDParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      default:
        break;
    }

  }




  template <typename Base>
  HammerFGDParser::symbol_kind_type
  HammerFGDParser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  HammerFGDParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  HammerFGDParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      default:
        break;
    }

  }

  // by_kind.
  HammerFGDParser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  HammerFGDParser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  HammerFGDParser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  HammerFGDParser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  HammerFGDParser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  HammerFGDParser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  HammerFGDParser::symbol_kind_type
  HammerFGDParser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  HammerFGDParser::symbol_kind_type
  HammerFGDParser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


} // yy
#line 1664 "hammer_fgd.bison.cpp"








#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 1743 "hammer_fgd.bison.cpp"

  /// Build a parser object.
  HammerFGDParser::HammerFGDParser (GoldSource::HammerFGDFile* ctx_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      ctx (ctx_yyarg)
  {}

  HammerFGDParser::~HammerFGDParser ()
  {}

  HammerFGDParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  HammerFGDParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  HammerFGDParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  HammerFGDParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  HammerFGDParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  HammerFGDParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  HammerFGDParser::symbol_kind_type
  HammerFGDParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  HammerFGDParser::stack_symbol_type::stack_symbol_type ()
  {}

  HammerFGDParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  HammerFGDParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  HammerFGDParser::stack_symbol_type&
  HammerFGDParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      default:
        break;
    }

    return *this;
  }

  HammerFGDParser::stack_symbol_type&
  HammerFGDParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  HammerFGDParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  HammerFGDParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  HammerFGDParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  HammerFGDParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  HammerFGDParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  HammerFGDParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  HammerFGDParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  HammerFGDParser::debug_level_type
  HammerFGDParser::debug_level () const
  {
    return yydebug_;
  }

  void
  HammerFGDParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  HammerFGDParser::state_type
  HammerFGDParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  HammerFGDParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  HammerFGDParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  HammerFGDParser::operator() ()
  {
    return parse ();
  }

  int
  HammerFGDParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (ctx));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {

#line 2123 "hammer_fgd.bison.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
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


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  HammerFGDParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  HammerFGDParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  HammerFGDParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // HammerFGDParser::context.
  HammerFGDParser::context::context (const HammerFGDParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  HammerFGDParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  HammerFGDParser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  HammerFGDParser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char HammerFGDParser::yypact_ninf_ = -56;

  const signed char HammerFGDParser::yytable_ninf_ = -1;

  const signed char
  HammerFGDParser::yypact_[] =
  {
      11,    -3,    12,   -56,    19,   -56,   -56,   -56,   -56,     6,
      15,     8,    -6,   -56,    14,    16,    17,    18,    20,    21,
      22,    23,   -56,   -56,   -56,   -56,   -56,   -56,     9,     1,
      24,    25,    26,   -56,    29,    27,    30,    32,    35,   -56,
      31,    36,   -18,   -56,    37,    38,    33,    39,    34,   -56,
      40,   -56,   -56,   -56,   -56,   -56,    41,    42,    43,    45,
      50,   -56,    51,   -56,    17,    44,    46,    48,    17,     2,
      49,    52,    54,    55,    56,    57,    59,   -56,   -56,    60,
      58,    64,   -56,    63,    67,    61,    65,    69,    68,    71,
     -56,    74,   -56,   -56,    53,   -56,    70,   -56,    72,    76,
      66,    77,   -56,   -56
  };

  const signed char
  HammerFGDParser::yydefact_[] =
  {
       3,     8,     0,    12,     0,     2,     4,     5,     6,     0,
       0,     0,     0,     1,    22,     0,    26,     0,     0,     0,
       0,     0,    13,    17,    16,    15,    14,    25,     0,    23,
       0,     0,     0,    27,     0,     0,     0,     0,     0,     9,
       0,     0,     0,    11,     0,     0,     0,     0,     0,    24,
       0,    35,    36,    37,    38,    39,     0,     0,     0,     0,
       0,    21,     0,    20,    26,     0,     0,     0,    26,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     7,    31,
       0,     0,    10,     0,     0,     0,     0,    40,     0,     0,
      29,     0,    41,    28,     0,    32,     0,    19,    33,     0,
       0,     0,    30,    34
  };

  const signed char
  HammerFGDParser::yypgoto_[] =
  {
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -55,   -56,   -56,   -56,   -56,
     -56
  };

  const signed char
  HammerFGDParser::yydefgoto_[] =
  {
       0,     4,     5,     6,    10,     7,     8,    12,    22,    23,
      24,    25,    26,    28,    29,    32,    33,    85,   100,    58,
      93
  };

  const signed char
  HammerFGDParser::yytable_[] =
  {
      17,    51,    52,    53,    54,    55,    56,    57,     9,    71,
      18,    19,    20,    75,     1,     2,     3,    21,    11,    13,
      14,    15,    16,    27,    39,    30,    31,    34,    40,    76,
       0,    41,    35,    36,    37,    38,    44,    45,    46,    42,
      49,    43,    47,    48,    50,    59,    61,    63,    60,    62,
      72,     0,     0,    73,    64,    74,    65,    66,    67,    68,
      69,    70,    77,    80,    81,    86,    97,    78,    79,    83,
      84,    87,    82,    88,    89,    91,    90,    92,    94,    95,
      96,   102,    99,   101,    98,   103
  };

  const signed char
  HammerFGDParser::yycheck_[] =
  {
       6,    19,    20,    21,    22,    23,    24,    25,    11,    64,
      16,    17,    18,    68,     3,     4,     5,    23,     6,     0,
      14,     6,    14,     9,    15,     9,     9,     9,    27,    27,
      -1,     7,    12,    12,    12,    12,     7,    10,     8,    14,
       9,    15,    10,     8,     8,     8,    13,    13,    10,    10,
       6,    -1,    -1,     7,    14,     7,    15,    15,    15,    14,
      10,    10,    13,     8,     8,     7,    13,    15,    14,    10,
      10,     7,    15,    10,     7,    10,    15,     8,    10,     8,
       6,    15,    10,     7,    14,     8
  };

  const signed char
  HammerFGDParser::yystos_[] =
  {
       0,     3,     4,     5,    29,    30,    31,    33,    34,    11,
      32,     6,    35,     0,    14,     6,    14,     6,    16,    17,
      18,    23,    36,    37,    38,    39,    40,     9,    41,    42,
       9,     9,    43,    44,     9,    12,    12,    12,    12,    15,
      27,     7,    14,    15,     7,    10,     8,    10,     8,     9,
       8,    19,    20,    21,    22,    23,    24,    25,    47,     8,
      10,    13,    10,    13,    14,    15,    15,    15,    14,    10,
      10,    43,     6,     7,     7,    43,    27,    13,    15,    14,
       8,     8,    15,    10,    10,    45,     7,     7,    10,     7,
      15,    10,     8,    48,    10,     8,     6,    13,    14,    10,
      46,     7,    15,     8
  };

  const signed char
  HammerFGDParser::yyr1_[] =
  {
       0,    28,    29,    30,    30,    30,    30,    31,    32,    32,
      33,    34,    35,    35,    36,    36,    36,    36,    37,    38,
      39,    40,    41,    41,    42,    42,    43,    43,    44,    44,
      44,    45,    45,    46,    46,    47,    47,    47,    47,    47,
      48,    48
  };

  const signed char
  HammerFGDParser::yyr2_[] =
  {
       0,     2,     1,     0,     1,     1,     1,     9,     0,     4,
       9,     5,     0,     2,     1,     1,     1,     1,     6,    10,
       4,     4,     0,     1,     3,     1,     0,     1,     8,     8,
      12,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       0,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const HammerFGDParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"@SolidClass\"",
  "\"@BaseClass\"", "\"@PointClass\"", "\"=\"", "\":\"", "StringLiteral",
  "Identifier", "Number", "\"base\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\"size\"", "\"iconsprite\"", "\"color\"", "\"integer\"", "\"color255\"",
  "\"string\"", "\"sprite\"", "\"studio\"", "\"flags\"", "\"choices\"",
  "EndOfFile", "','", "$accept", "FGDFile", "classes", "SolidClassDef",
  "BasesClassesOpt", "PointClassDef", "BaseClassDef",
  "PointEntityConstructorListOpt", "PointEntityConstructor", "ColorDefOpt",
  "BoundingBoxDefOpt", "StudioDefOpt", "IconspriteDefOpt",
  "identifierListOpt", "identifierList", "ClassFieldsOpt", "ClassFieldDef",
  "FlagsValues", "EnumValues", "TypeId", "DefaultValueOpt", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const signed char
  HammerFGDParser::yyrline_[] =
  {
       0,    33,    33,    36,    37,    38,    39,    43,    45,    46,
      49,    52,    56,    57,    60,    61,    62,    63,    66,    69,
      72,    75,    78,    79,    81,    82,    85,    86,    91,    93,
      95,    99,   100,   103,   104,   107,   108,   109,   110,   111,
     115,   116
  };

  void
  HammerFGDParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  HammerFGDParser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2649 "hammer_fgd.bison.cpp"

#line 118 "hammer_fgd.y"



GoldSource::FGDTokenTypes yylex(GoldSource::HammerFGDFile* ctx)
{
    char* s = ctx->ParserCursor();

    
#line 129 "<stdout>"
{
	char yych;
	yych = *s;
	switch (yych) {
	case 0x00:	goto yy3;
	case '"':	goto yy5;
	case '(':	goto yy6;
	case ')':	goto yy8;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy10;
	case ':':	goto yy13;
	case '=':	goto yy15;
	case '@':	goto yy17;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy18;
	case '[':	goto yy21;
	case '\\':	goto yy23;
	case ']':	goto yy26;
	default:	goto yy2;
	}
yy2:
yy3:
	++s;
#line 158 "hammer_fgd.y"
	{
            return GoldSource::FGDTokenTypes::EndOfFile;
        }
#line 216 "<stdout>"
yy5:
	yych = *++s;
	switch (yych) {
	case '.':	goto yy28;
	default:	goto yy2;
	}
yy6:
	++s;
#line 171 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::OpeningParenthesis; }
#line 227 "<stdout>"
yy8:
	++s;
#line 172 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::ClosingParenthesis; }
#line 232 "<stdout>"
yy10:
	yych = *++s;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy10;
	default:	goto yy12;
	}
yy12:
#line 170 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::Number; }
#line 251 "<stdout>"
yy13:
	++s;
#line 167 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::Colon; }
#line 256 "<stdout>"
yy15:
	++s;
#line 166 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::EqualsSign; }
#line 261 "<stdout>"
yy17:
	yych = *++s;
	switch (yych) {
	case 'B':	goto yy29;
	case 'P':	goto yy30;
	case 'S':	goto yy31;
	default:	goto yy2;
	}
yy18:
	yych = *++s;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy32;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy18;
	default:	goto yy20;
	}
yy20:
#line 169 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::Identifier; }
#line 341 "<stdout>"
yy21:
	++s;
#line 173 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::OpeningBracket; }
#line 346 "<stdout>"
yy23:
	yych = *++s;
	switch (yych) {
	case '\n':	goto yy25;
	default:	goto yy23;
	}
yy25:
#line 175 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::Comment; }
#line 356 "<stdout>"
yy26:
	++s;
#line 174 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::ClosingBracket; }
#line 361 "<stdout>"
yy28:
	yych = *++s;
	switch (yych) {
	case '*':	goto yy34;
	default:	goto yy2;
	}
yy29:
	yych = *++s;
	switch (yych) {
	case 'a':	goto yy35;
	default:	goto yy2;
	}
yy30:
	yych = *++s;
	switch (yych) {
	case 'o':	goto yy36;
	default:	goto yy2;
	}
yy31:
	yych = *++s;
	switch (yych) {
	case 'o':	goto yy37;
	default:	goto yy2;
	}
yy32:
	yych = *++s;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy32;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy38;
	default:	goto yy20;
	}
yy34:
	yych = *++s;
	switch (yych) {
	case '"':	goto yy40;
	default:	goto yy2;
	}
yy35:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy42;
	default:	goto yy2;
	}
yy36:
	yych = *++s;
	switch (yych) {
	case 'i':	goto yy43;
	default:	goto yy2;
	}
yy37:
	yych = *++s;
	switch (yych) {
	case 'l':	goto yy44;
	default:	goto yy2;
	}
yy38:
	yych = *++s;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy38;
	default:	goto yy20;
	}
yy40:
	++s;
#line 168 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::StringLiteral;}
#line 540 "<stdout>"
yy42:
	yych = *++s;
	switch (yych) {
	case 'e':	goto yy45;
	default:	goto yy2;
	}
yy43:
	yych = *++s;
	switch (yych) {
	case 'n':	goto yy46;
	default:	goto yy2;
	}
yy44:
	yych = *++s;
	switch (yych) {
	case 'i':	goto yy47;
	default:	goto yy2;
	}
yy45:
	yych = *++s;
	switch (yych) {
	case 'C':	goto yy48;
	default:	goto yy2;
	}
yy46:
	yych = *++s;
	switch (yych) {
	case 't':	goto yy49;
	default:	goto yy2;
	}
yy47:
	yych = *++s;
	switch (yych) {
	case 'd':	goto yy50;
	default:	goto yy2;
	}
yy48:
	yych = *++s;
	switch (yych) {
	case 'l':	goto yy51;
	default:	goto yy2;
	}
yy49:
	yych = *++s;
	switch (yych) {
	case 'C':	goto yy52;
	default:	goto yy2;
	}
yy50:
	yych = *++s;
	switch (yych) {
	case 'C':	goto yy53;
	default:	goto yy2;
	}
yy51:
	yych = *++s;
	switch (yych) {
	case 'a':	goto yy54;
	default:	goto yy2;
	}
yy52:
	yych = *++s;
	switch (yych) {
	case 'l':	goto yy55;
	default:	goto yy2;
	}
yy53:
	yych = *++s;
	switch (yych) {
	case 'l':	goto yy56;
	default:	goto yy2;
	}
yy54:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy57;
	default:	goto yy2;
	}
yy55:
	yych = *++s;
	switch (yych) {
	case 'a':	goto yy58;
	default:	goto yy2;
	}
yy56:
	yych = *++s;
	switch (yych) {
	case 'a':	goto yy59;
	default:	goto yy2;
	}
yy57:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy60;
	default:	goto yy2;
	}
yy58:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy62;
	default:	goto yy2;
	}
yy59:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy63;
	default:	goto yy2;
	}
yy60:
	++s;
#line 164 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::BaseClass; }
#line 653 "<stdout>"
yy62:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy64;
	default:	goto yy2;
	}
yy63:
	yych = *++s;
	switch (yych) {
	case 's':	goto yy66;
	default:	goto yy2;
	}
yy64:
	++s;
#line 165 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::PointClass; }
#line 670 "<stdout>"
yy66:
	++s;
#line 163 "hammer_fgd.y"
	{ return  GoldSource::FGDTokenTypes::SolidClass; }
#line 675 "<stdout>"
}
#line 190 "hammer_fgd.y"

	
	return GoldSource::FGDTokenTypes::EndOfFile;
}

void yy::HammerFGDParser::error(const std::string& m)
{
    Con_Printf("%s\n",m.c_str());
}







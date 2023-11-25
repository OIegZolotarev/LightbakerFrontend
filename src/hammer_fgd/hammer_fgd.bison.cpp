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


#include "..\application.h"
#include "..\common.h"
#include "..\hammer_fgd.h"

struct FGDParsingContext;
using namespace GoldSource;



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
# define YYDEBUG 1
#endif

namespace yy {


  /// A point in a source file.
  class position
  {
  public:
    /// Type for file name.
    typedef const std::string filename_type;
    /// Type for line and column numbers.
    typedef int counter_type;

    /// Construct a position.
    explicit position (filename_type* f = YY_NULLPTR,
                       counter_type l = 1,
                       counter_type c = 1)
      : filename (f)
      , line (l)
      , column (c)
    {}


    /// Initialization.
    void initialize (filename_type* fn = YY_NULLPTR,
                     counter_type l = 1,
                     counter_type c = 1)
    {
      filename = fn;
      line = l;
      column = c;
    }

    /** \name Line and Column related manipulators
     ** \{ */
    /// (line related) Advance to the COUNT next lines.
    void lines (counter_type count = 1)
    {
      if (count)
        {
          column = 1;
          line = add_ (line, count, 1);
        }
    }

    /// (column related) Advance to the COUNT next columns.
    void columns (counter_type count = 1)
    {
      column = add_ (column, count, 1);
    }
    /** \} */

    /// File name to which this position refers.
    filename_type* filename;
    /// Current line number.
    counter_type line;
    /// Current column number.
    counter_type column;

  private:
    /// Compute max (min, lhs+rhs).
    static counter_type add_ (counter_type lhs, counter_type rhs, counter_type min)
    {
      return lhs + rhs < min ? min : lhs + rhs;
    }
  };

  /// Add \a width columns, in place.
  inline position&
  operator+= (position& res, position::counter_type width)
  {
    res.columns (width);
    return res;
  }

  /// Add \a width columns.
  inline position
  operator+ (position res, position::counter_type width)
  {
    return res += width;
  }

  /// Subtract \a width columns, in place.
  inline position&
  operator-= (position& res, position::counter_type width)
  {
    return res += -width;
  }

  /// Subtract \a width columns.
  inline position
  operator- (position res, position::counter_type width)
  {
    return res -= width;
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param pos a reference to the position to redirect
   */
  template <typename YYChar>
  std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const position& pos)
  {
    if (pos.filename)
      ostr << *pos.filename << ':';
    return ostr << pos.line << '.' << pos.column;
  }

  /// Two points in a source file.
  class location
  {
  public:
    /// Type for file name.
    typedef position::filename_type filename_type;
    /// Type for line and column numbers.
    typedef position::counter_type counter_type;

    /// Construct a location from \a b to \a e.
    location (const position& b, const position& e)
      : begin (b)
      , end (e)
    {}

    /// Construct a 0-width location in \a p.
    explicit location (const position& p = position ())
      : begin (p)
      , end (p)
    {}

    /// Construct a 0-width location in \a f, \a l, \a c.
    explicit location (filename_type* f,
                       counter_type l = 1,
                       counter_type c = 1)
      : begin (f, l, c)
      , end (f, l, c)
    {}


    /// Initialization.
    void initialize (filename_type* f = YY_NULLPTR,
                     counter_type l = 1,
                     counter_type c = 1)
    {
      begin.initialize (f, l, c);
      end = begin;
    }

    /** \name Line and Column related manipulators
     ** \{ */
  public:
    /// Reset initial location to final location.
    void step ()
    {
      begin = end;
    }

    /// Extend the current location to the COUNT next columns.
    void columns (counter_type count = 1)
    {
      end += count;
    }

    /// Extend the current location to the COUNT next lines.
    void lines (counter_type count = 1)
    {
      end.lines (count);
    }
    /** \} */


  public:
    /// Beginning of the located region.
    position begin;
    /// End of the located region.
    position end;
  };

  /// Join two locations, in place.
  inline location&
  operator+= (location& res, const location& end)
  {
    res.end = end.end;
    return res;
  }

  /// Join two locations.
  inline location
  operator+ (location res, const location& end)
  {
    return res += end;
  }

  /// Add \a width columns to the end position, in place.
  inline location&
  operator+= (location& res, location::counter_type width)
  {
    res.columns (width);
    return res;
  }

  /// Add \a width columns to the end position.
  inline location
  operator+ (location res, location::counter_type width)
  {
    return res += width;
  }

  /// Subtract \a width columns to the end position, in place.
  inline location&
  operator-= (location& res, location::counter_type width)
  {
    return res += -width;
  }

  /// Subtract \a width columns to the end position.
  inline location
  operator- (location res, location::counter_type width)
  {
    return res -= width;
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param loc a reference to the location to redirect
   **
   ** Avoid duplicate information.
   */
  template <typename YYChar>
  std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const location& loc)
  {
    location::counter_type end_col
      = 0 < loc.end.column ? loc.end.column - 1 : 0;
    ostr << loc.begin;
    if (loc.end.filename
        && (!loc.begin.filename
            || *loc.begin.filename != *loc.end.filename))
      ostr << '-' << loc.end.filename << ':' << loc.end.line << '.' << end_col;
    else if (loc.begin.line < loc.end.line)
      ostr << '-' << loc.end.line << '.' << end_col;
    else if (loc.begin.column < end_col)
      ostr << '-' << end_col;
    return ostr;
  }


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
    {
      // EntityClassDef
      // SolidClassDef
      // PointClassDef
      // BaseClassDef
      char dummy1[sizeof (GoldSource::FGDEntityClass*)];

      // FlagsValues
      // EnumValues
      char dummy2[sizeof (GoldSource::FGDFlagsList)];

      // FlagValue
      // EnumValue
      char dummy3[sizeof (GoldSource::FGDFlagsValue_t)];

      // ClassFieldsOpt
      char dummy4[sizeof (GoldSource::FGDPropertiesList)];

      // ClassFieldDef
      char dummy5[sizeof (GoldSource::FGDPropertyDescriptor*)];

      // Number
      char dummy6[sizeof (float)];

      // identifierListOpt
      char dummy7[sizeof (std::list<std::string>)];

      // StringLiteral
      // Identifier
      // "integer"
      // "color255"
      // "string"
      // "sprite"
      // "studio"
      // "flags"
      // "choices"
      // "model"
      // "sky"
      // "target_source"
      // "target_destination"
      // "sound"
      // StringLiteralOpt
      // ModelType
      // TypeId
      // DefaultValueOpt
      char dummy8[sizeof (std::string)];
    };

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

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    EndOfFile = 0,                 // EndOfFile
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    SolidClass = 258,              // "@SolidClass"
    BaseClass = 259,               // "@BaseClass"
    PointClass = 260,              // "@PointClass"
    EqualsSign = 261,              // "="
    Colon = 262,                   // ":"
    Comma = 263,                   // ","
    StringLiteral = 264,           // StringLiteral
    Identifier = 265,              // Identifier
    Number = 266,                  // Number
    BaseDef = 267,                 // "base"
    OpeningParenthesis = 268,      // "("
    ClosingParenthesis = 269,      // ")"
    OpeningBracket = 270,          // "["
    ClosingBracket = 271,          // "]"
    SizeBoundingBox = 272,         // "size"
    Iconsprite = 273,              // "iconsprite"
    Color = 274,                   // "color"
    IntegerType = 275,             // "integer"
    Color255 = 276,                // "color255"
    String = 277,                  // "string"
    Sprite = 278,                  // "sprite"
    Decal = 279,                   // "decal"
    Studio = 280,                  // "studio"
    Flags = 281,                   // "flags"
    Choices = 282,                 // "choices"
    Model = 283,                   // "model"
    Sky = 284,                     // "sky"
    TargetSource = 285,            // "target_source"
    TargetDestination = 286,       // "target_destination"
    Sound = 287,                   // "sound"
    Comment = 288                  // Comment
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
        YYNTOKENS = 34, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // EndOfFile
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_SolidClass = 3,                        // "@SolidClass"
        S_BaseClass = 4,                         // "@BaseClass"
        S_PointClass = 5,                        // "@PointClass"
        S_EqualsSign = 6,                        // "="
        S_Colon = 7,                             // ":"
        S_Comma = 8,                             // ","
        S_StringLiteral = 9,                     // StringLiteral
        S_Identifier = 10,                       // Identifier
        S_Number = 11,                           // Number
        S_BaseDef = 12,                          // "base"
        S_OpeningParenthesis = 13,               // "("
        S_ClosingParenthesis = 14,               // ")"
        S_OpeningBracket = 15,                   // "["
        S_ClosingBracket = 16,                   // "]"
        S_SizeBoundingBox = 17,                  // "size"
        S_Iconsprite = 18,                       // "iconsprite"
        S_Color = 19,                            // "color"
        S_IntegerType = 20,                      // "integer"
        S_Color255 = 21,                         // "color255"
        S_String = 22,                           // "string"
        S_Sprite = 23,                           // "sprite"
        S_Decal = 24,                            // "decal"
        S_Studio = 25,                           // "studio"
        S_Flags = 26,                            // "flags"
        S_Choices = 27,                          // "choices"
        S_Model = 28,                            // "model"
        S_Sky = 29,                              // "sky"
        S_TargetSource = 30,                     // "target_source"
        S_TargetDestination = 31,                // "target_destination"
        S_Sound = 32,                            // "sound"
        S_Comment = 33,                          // Comment
        S_YYACCEPT = 34,                         // $accept
        S_FGDFile = 35,                          // FGDFile
        S_classesOpt = 36,                       // classesOpt
        S_EntityClassDef = 37,                   // EntityClassDef
        S_SolidClassDef = 38,                    // SolidClassDef
        S_PointClassDef = 39,                    // PointClassDef
        S_BaseClassDef = 40,                     // BaseClassDef
        S_ExtraNotesOpt = 41,                    // ExtraNotesOpt
        S_CtorsOpt = 42,                         // CtorsOpt
        S_Ctor = 43,                             // Ctor
        S_ColorDefOpt = 44,                      // ColorDefOpt
        S_BoundingBoxDefOpt = 45,                // BoundingBoxDefOpt
        S_StudioDefOpt = 46,                     // StudioDefOpt
        S_SpriteDefOpt = 47,                     // SpriteDefOpt
        S_DecalDefOpt = 48,                      // DecalDefOpt
        S_IconspriteDefOpt = 49,                 // IconspriteDefOpt
        S_ExtendedConstructor = 50,              // ExtendedConstructor
        S_BaseClassRefOpt = 51,                  // BaseClassRefOpt
        S_StringLiteralOpt = 52,                 // StringLiteralOpt
        S_identifierListOpt = 53,                // identifierListOpt
        S_ClassFieldsOpt = 54,                   // ClassFieldsOpt
        S_ClassFieldDef = 55,                    // ClassFieldDef
        S_ModelType = 56,                        // ModelType
        S_FlagsValues = 57,                      // FlagsValues
        S_FlagValue = 58,                        // FlagValue
        S_EnumValues = 59,                       // EnumValues
        S_EnumValue = 60,                        // EnumValue
        S_TypeId = 61,                           // TypeId
        S_DefaultValueOpt = 62                   // DefaultValueOpt
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
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.move< GoldSource::FGDEntityClass* > (std::move (that.value));
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.move< GoldSource::FGDFlagsList > (std::move (that.value));
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.move< GoldSource::FGDFlagsValue_t > (std::move (that.value));
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.move< GoldSource::FGDPropertiesList > (std::move (that.value));
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.move< GoldSource::FGDPropertyDescriptor* > (std::move (that.value));
        break;

      case symbol_kind::S_Number: // Number
        value.move< float > (std::move (that.value));
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.move< std::list<std::string> > (std::move (that.value));
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, GoldSource::FGDEntityClass*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const GoldSource::FGDEntityClass*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, GoldSource::FGDFlagsList&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const GoldSource::FGDFlagsList& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, GoldSource::FGDFlagsValue_t&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const GoldSource::FGDFlagsValue_t& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, GoldSource::FGDPropertiesList&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const GoldSource::FGDPropertiesList& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, GoldSource::FGDPropertyDescriptor*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const GoldSource::FGDPropertyDescriptor*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, float&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const float& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::list<std::string>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::list<std::string>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
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
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.template destroy< GoldSource::FGDEntityClass* > ();
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.template destroy< GoldSource::FGDFlagsList > ();
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.template destroy< GoldSource::FGDFlagsValue_t > ();
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.template destroy< GoldSource::FGDPropertiesList > ();
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.template destroy< GoldSource::FGDPropertyDescriptor* > ();
        break;

      case symbol_kind::S_Number: // Number
        value.template destroy< float > ();
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.template destroy< std::list<std::string> > ();
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.template destroy< std::string > ();
        break;

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

      /// The location.
      location_type location;

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
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::EndOfFile
                   || (token::YYerror <= tok && tok <= token::Comma)
                   || (token::BaseDef <= tok && tok <= token::Color)
                   || tok == token::Decal
                   || tok == token::Comment);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, float v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const float& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::Number);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::StringLiteral <= tok && tok <= token::Identifier)
                   || (token::IntegerType <= tok && tok <= token::Sprite)
                   || (token::Studio <= tok && tok <= token::Sound));
#endif
      }
    };

    /// Build a parser object.
    HammerFGDParser (FGDParsingContext* ctx_yyarg);
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
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EndOfFile (location_type l)
      {
        return symbol_type (token::EndOfFile, std::move (l));
      }
#else
      static
      symbol_type
      make_EndOfFile (const location_type& l)
      {
        return symbol_type (token::EndOfFile, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SolidClass (location_type l)
      {
        return symbol_type (token::SolidClass, std::move (l));
      }
#else
      static
      symbol_type
      make_SolidClass (const location_type& l)
      {
        return symbol_type (token::SolidClass, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BaseClass (location_type l)
      {
        return symbol_type (token::BaseClass, std::move (l));
      }
#else
      static
      symbol_type
      make_BaseClass (const location_type& l)
      {
        return symbol_type (token::BaseClass, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PointClass (location_type l)
      {
        return symbol_type (token::PointClass, std::move (l));
      }
#else
      static
      symbol_type
      make_PointClass (const location_type& l)
      {
        return symbol_type (token::PointClass, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EqualsSign (location_type l)
      {
        return symbol_type (token::EqualsSign, std::move (l));
      }
#else
      static
      symbol_type
      make_EqualsSign (const location_type& l)
      {
        return symbol_type (token::EqualsSign, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Colon (location_type l)
      {
        return symbol_type (token::Colon, std::move (l));
      }
#else
      static
      symbol_type
      make_Colon (const location_type& l)
      {
        return symbol_type (token::Colon, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Comma (location_type l)
      {
        return symbol_type (token::Comma, std::move (l));
      }
#else
      static
      symbol_type
      make_Comma (const location_type& l)
      {
        return symbol_type (token::Comma, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_StringLiteral (std::string v, location_type l)
      {
        return symbol_type (token::StringLiteral, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_StringLiteral (const std::string& v, const location_type& l)
      {
        return symbol_type (token::StringLiteral, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Identifier (std::string v, location_type l)
      {
        return symbol_type (token::Identifier, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Identifier (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Identifier, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Number (float v, location_type l)
      {
        return symbol_type (token::Number, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Number (const float& v, const location_type& l)
      {
        return symbol_type (token::Number, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BaseDef (location_type l)
      {
        return symbol_type (token::BaseDef, std::move (l));
      }
#else
      static
      symbol_type
      make_BaseDef (const location_type& l)
      {
        return symbol_type (token::BaseDef, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OpeningParenthesis (location_type l)
      {
        return symbol_type (token::OpeningParenthesis, std::move (l));
      }
#else
      static
      symbol_type
      make_OpeningParenthesis (const location_type& l)
      {
        return symbol_type (token::OpeningParenthesis, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ClosingParenthesis (location_type l)
      {
        return symbol_type (token::ClosingParenthesis, std::move (l));
      }
#else
      static
      symbol_type
      make_ClosingParenthesis (const location_type& l)
      {
        return symbol_type (token::ClosingParenthesis, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OpeningBracket (location_type l)
      {
        return symbol_type (token::OpeningBracket, std::move (l));
      }
#else
      static
      symbol_type
      make_OpeningBracket (const location_type& l)
      {
        return symbol_type (token::OpeningBracket, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ClosingBracket (location_type l)
      {
        return symbol_type (token::ClosingBracket, std::move (l));
      }
#else
      static
      symbol_type
      make_ClosingBracket (const location_type& l)
      {
        return symbol_type (token::ClosingBracket, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SizeBoundingBox (location_type l)
      {
        return symbol_type (token::SizeBoundingBox, std::move (l));
      }
#else
      static
      symbol_type
      make_SizeBoundingBox (const location_type& l)
      {
        return symbol_type (token::SizeBoundingBox, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Iconsprite (location_type l)
      {
        return symbol_type (token::Iconsprite, std::move (l));
      }
#else
      static
      symbol_type
      make_Iconsprite (const location_type& l)
      {
        return symbol_type (token::Iconsprite, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Color (location_type l)
      {
        return symbol_type (token::Color, std::move (l));
      }
#else
      static
      symbol_type
      make_Color (const location_type& l)
      {
        return symbol_type (token::Color, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IntegerType (std::string v, location_type l)
      {
        return symbol_type (token::IntegerType, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IntegerType (const std::string& v, const location_type& l)
      {
        return symbol_type (token::IntegerType, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Color255 (std::string v, location_type l)
      {
        return symbol_type (token::Color255, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Color255 (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Color255, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_String (std::string v, location_type l)
      {
        return symbol_type (token::String, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_String (const std::string& v, const location_type& l)
      {
        return symbol_type (token::String, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Sprite (std::string v, location_type l)
      {
        return symbol_type (token::Sprite, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Sprite (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Sprite, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Decal (location_type l)
      {
        return symbol_type (token::Decal, std::move (l));
      }
#else
      static
      symbol_type
      make_Decal (const location_type& l)
      {
        return symbol_type (token::Decal, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Studio (std::string v, location_type l)
      {
        return symbol_type (token::Studio, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Studio (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Studio, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Flags (std::string v, location_type l)
      {
        return symbol_type (token::Flags, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Flags (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Flags, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Choices (std::string v, location_type l)
      {
        return symbol_type (token::Choices, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Choices (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Choices, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Model (std::string v, location_type l)
      {
        return symbol_type (token::Model, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Model (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Model, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Sky (std::string v, location_type l)
      {
        return symbol_type (token::Sky, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Sky (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Sky, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TargetSource (std::string v, location_type l)
      {
        return symbol_type (token::TargetSource, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TargetSource (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TargetSource, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TargetDestination (std::string v, location_type l)
      {
        return symbol_type (token::TargetDestination, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TargetDestination (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TargetDestination, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Sound (std::string v, location_type l)
      {
        return symbol_type (token::Sound, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_Sound (const std::string& v, const location_type& l)
      {
        return symbol_type (token::Sound, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_Comment (location_type l)
      {
        return symbol_type (token::Comment, std::move (l));
      }
#else
      static
      symbol_type
      make_Comment (const location_type& l)
      {
        return symbol_type (token::Comment, l);
      }
#endif


    class context
    {
    public:
      context (const HammerFGDParser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

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
    typedef unsigned char state_type;

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
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const unsigned char yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
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
      yylast_ = 220,     ///< Last index in yytable_.
      yynnts_ = 29,  ///< Number of nonterminal symbols.
      yyfinal_ = 3 ///< Termination state number.
    };


    // User arguments.
    FGDParsingContext* ctx;

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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
    };
    // Last valid token kind.
    const int code_max = 288;

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
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.copy< GoldSource::FGDEntityClass* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.copy< GoldSource::FGDFlagsList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.copy< GoldSource::FGDFlagsValue_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.copy< GoldSource::FGDPropertiesList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.copy< GoldSource::FGDPropertyDescriptor* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Number: // Number
        value.copy< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.copy< std::list<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.copy< std::string > (YY_MOVE (that.value));
        break;

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
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.move< GoldSource::FGDEntityClass* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.move< GoldSource::FGDFlagsList > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.move< GoldSource::FGDFlagsValue_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.move< GoldSource::FGDPropertiesList > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.move< GoldSource::FGDPropertyDescriptor* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Number: // Number
        value.move< float > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.move< std::list<std::string> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
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






// Unqualified %code blocks.


struct FGDParsingContext
{
    const char* cursor;
    yy::location loc;

    FGDEntityClass * current_entity = nullptr;
    FGDPropertyDescriptor* current_property = nullptr;

    GoldSource::HammerFGDFile * fgd;

    
    struct 
    {
        glm::vec3 color = { 1,1,1};
        glm::vec3 mins = {-4,-4,-4};
        glm::vec3 maxs = {4,4,4};
        std::string model = "";
        std::string sprite = "";
        bool decal = false;
        std::string editorSprite = "";

        unsigned int flags = 0;
        
        std::list<std::string> baseClasses;

        void clear()
        {
            color = { 1,1,1};
            mins = {-4,-4,-4};
            maxs = {4,4,4};
            model = "";
            sprite = "";
            decal = false;
            editorSprite = "";
            flags = 0;
        }

    }entityCtorData;

    FGDParsingContext(GoldSource::HammerFGDFile * file)
    {
       fgd = file;
       cursor = file->Data();
    }

    FGDEntityClass* new_entity(FGDEntityClassType type, std::string className, std::string description,FGDPropertiesList & props)
    {
		
        current_entity = new FGDEntityClass(type, className, description, props);
        current_entity->SetCtorFlags(entityCtorData.flags);

        if (entityCtorData.flags & FL_SET_COLOR)            current_entity->SetColor(entityCtorData.color);
        if (entityCtorData.flags & FL_SET_SIZE)             current_entity->SetBBox(entityCtorData.mins, entityCtorData.maxs);
        if (entityCtorData.flags & FL_SET_MODEL)            current_entity->SetModel(entityCtorData.model);
        if (entityCtorData.flags & FL_SET_SPRITE)           current_entity->SetSprite(entityCtorData.sprite);
        if (entityCtorData.flags & FL_SET_DECAL)            current_entity->SetDecalEntity(entityCtorData.decal);
        if (entityCtorData.flags & FL_SET_EDITOR_SPRITE)    current_entity->SetEditorSprite(entityCtorData.editorSprite);
        if (entityCtorData.flags & FL_SET_BASE_CLASSES)    current_entity->SetBaseClasses(entityCtorData.baseClasses);

        entityCtorData.clear();
        
        fgd->AddEntityClass(current_entity);

        return current_entity;
    }

    void SetColor(float r, float g, float b)
	{
        entityCtorData.color.r = r / 255.f;
        entityCtorData.color.g = g / 255.f;
        entityCtorData.color.b = b / 255.f;

        entityCtorData.flags |= FL_SET_COLOR;
    }

    
    void SetBbox(float x, float y, float z)
    {
		entityCtorData.mins.x = -x / 2;
		entityCtorData.mins.y = -y / 2;
		entityCtorData.mins.z = -z / 2;

        entityCtorData.maxs.x = x / 2;
		entityCtorData.maxs.y = y / 2;
		entityCtorData.maxs.z = z / 2;

        entityCtorData.flags |= FL_SET_SIZE;
    }

    void SetBbox(float x, float y, float z, float x2, float y2, float z2)
    {
		entityCtorData.mins.x = x;
		entityCtorData.mins.y = y;
		entityCtorData.mins.z = z;

        entityCtorData.maxs.x = x2;
		entityCtorData.maxs.y = y2;
		entityCtorData.maxs.z = z2;

        entityCtorData.flags |= FL_SET_SIZE;
    }

    void SetModel(std::string model)
    {
        entityCtorData.model = model;
        entityCtorData.flags |= FL_SET_MODEL;
    }

    void SetSprite(std::string model)
    {
        entityCtorData.sprite = model;
        entityCtorData.flags |= FL_SET_SPRITE;
    }

    void SetDecalEntity(bool flag)
    {
        entityCtorData.decal = flag;
        entityCtorData.flags |= FL_SET_DECAL;
    }

    void SetEditorSprite(std::string sprite)
    {
        entityCtorData.editorSprite = sprite;
        entityCtorData.flags |= FL_SET_EDITOR_SPRITE;
    }

    void SetBaseClasses(std::list<std::string> & classList)
    {        
        entityCtorData.baseClasses = classList; 
        entityCtorData.flags |= FL_SET_BASE_CLASSES;
    }

    void SetPropertyExtra(std::string property, float value)
    {
		Con_Printf("SetPropertyExtra(%s, %f)\n", property.c_str(), value);

//         assert(current_entity);
//         current_entity->SetPropertyExtra(property, value);
    }


};

namespace yy { HammerFGDParser::symbol_type yylex(FGDParsingContext* ctx); }


void ParseFGD(GoldSource::HammerFGDFile * file)
{

    FGDParsingContext ctx(file);    

    auto fileName = file->FileName();

    ctx.loc.begin.filename = &fileName;
    ctx.loc.end.filename   = &fileName;

    yy::HammerFGDParser parser(&ctx);

    
    parser.set_debug_level(999);

    parser.parse();  
}




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

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


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

  /// Build a parser object.
  HammerFGDParser::HammerFGDParser (FGDParsingContext* ctx_yyarg)
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
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.YY_MOVE_OR_COPY< GoldSource::FGDEntityClass* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.YY_MOVE_OR_COPY< GoldSource::FGDFlagsList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.YY_MOVE_OR_COPY< GoldSource::FGDFlagsValue_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.YY_MOVE_OR_COPY< GoldSource::FGDPropertiesList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.YY_MOVE_OR_COPY< GoldSource::FGDPropertyDescriptor* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Number: // Number
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.YY_MOVE_OR_COPY< std::list<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  HammerFGDParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.move< GoldSource::FGDEntityClass* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.move< GoldSource::FGDFlagsList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.move< GoldSource::FGDFlagsValue_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.move< GoldSource::FGDPropertiesList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.move< GoldSource::FGDPropertyDescriptor* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Number: // Number
        value.move< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.move< std::list<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.move< std::string > (YY_MOVE (that.value));
        break;

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
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.copy< GoldSource::FGDEntityClass* > (that.value);
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.copy< GoldSource::FGDFlagsList > (that.value);
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.copy< GoldSource::FGDFlagsValue_t > (that.value);
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.copy< GoldSource::FGDPropertiesList > (that.value);
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.copy< GoldSource::FGDPropertyDescriptor* > (that.value);
        break;

      case symbol_kind::S_Number: // Number
        value.copy< float > (that.value);
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.copy< std::list<std::string> > (that.value);
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  HammerFGDParser::stack_symbol_type&
  HammerFGDParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        value.move< GoldSource::FGDEntityClass* > (that.value);
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        value.move< GoldSource::FGDFlagsList > (that.value);
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        value.move< GoldSource::FGDFlagsValue_t > (that.value);
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        value.move< GoldSource::FGDPropertiesList > (that.value);
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        value.move< GoldSource::FGDPropertyDescriptor* > (that.value);
        break;

      case symbol_kind::S_Number: // Number
        value.move< float > (that.value);
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        value.move< std::list<std::string> > (that.value);
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
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
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
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

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

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
      case symbol_kind::S_EntityClassDef: // EntityClassDef
      case symbol_kind::S_SolidClassDef: // SolidClassDef
      case symbol_kind::S_PointClassDef: // PointClassDef
      case symbol_kind::S_BaseClassDef: // BaseClassDef
        yylhs.value.emplace< GoldSource::FGDEntityClass* > ();
        break;

      case symbol_kind::S_FlagsValues: // FlagsValues
      case symbol_kind::S_EnumValues: // EnumValues
        yylhs.value.emplace< GoldSource::FGDFlagsList > ();
        break;

      case symbol_kind::S_FlagValue: // FlagValue
      case symbol_kind::S_EnumValue: // EnumValue
        yylhs.value.emplace< GoldSource::FGDFlagsValue_t > ();
        break;

      case symbol_kind::S_ClassFieldsOpt: // ClassFieldsOpt
        yylhs.value.emplace< GoldSource::FGDPropertiesList > ();
        break;

      case symbol_kind::S_ClassFieldDef: // ClassFieldDef
        yylhs.value.emplace< GoldSource::FGDPropertyDescriptor* > ();
        break;

      case symbol_kind::S_Number: // Number
        yylhs.value.emplace< float > ();
        break;

      case symbol_kind::S_identifierListOpt: // identifierListOpt
        yylhs.value.emplace< std::list<std::string> > ();
        break;

      case symbol_kind::S_StringLiteral: // StringLiteral
      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_IntegerType: // "integer"
      case symbol_kind::S_Color255: // "color255"
      case symbol_kind::S_String: // "string"
      case symbol_kind::S_Sprite: // "sprite"
      case symbol_kind::S_Studio: // "studio"
      case symbol_kind::S_Flags: // "flags"
      case symbol_kind::S_Choices: // "choices"
      case symbol_kind::S_Model: // "model"
      case symbol_kind::S_Sky: // "sky"
      case symbol_kind::S_TargetSource: // "target_source"
      case symbol_kind::S_TargetDestination: // "target_destination"
      case symbol_kind::S_Sound: // "sound"
      case symbol_kind::S_StringLiteralOpt: // StringLiteralOpt
      case symbol_kind::S_ModelType: // ModelType
      case symbol_kind::S_TypeId: // TypeId
      case symbol_kind::S_DefaultValueOpt: // DefaultValueOpt
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 5: // EntityClassDef: SolidClassDef
                { yylhs.value.as < GoldSource::FGDEntityClass* > () = yystack_[0].value.as < GoldSource::FGDEntityClass* > (); }
    break;

  case 6: // EntityClassDef: PointClassDef
 { yylhs.value.as < GoldSource::FGDEntityClass* > () = yystack_[0].value.as < GoldSource::FGDEntityClass* > (); }
    break;

  case 7: // EntityClassDef: BaseClassDef
 { yylhs.value.as < GoldSource::FGDEntityClass* > () = yystack_[0].value.as < GoldSource::FGDEntityClass* > (); }
    break;

  case 8: // SolidClassDef: "@SolidClass" CtorsOpt "=" Identifier ":" StringLiteral ExtraNotesOpt "[" ClassFieldsOpt "]"
                                                                                                                                        { yylhs.value.as < GoldSource::FGDEntityClass* > () = ctx->new_entity(FGDEntityClassType::Solid,yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[1].value.as < GoldSource::FGDPropertiesList > ()); }
    break;

  case 9: // PointClassDef: "@PointClass" CtorsOpt "=" Identifier ":" StringLiteral ExtraNotesOpt "[" ClassFieldsOpt "]"
                                                                                                                                        { yylhs.value.as < GoldSource::FGDEntityClass* > () = ctx->new_entity(FGDEntityClassType::Point,yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[1].value.as < GoldSource::FGDPropertiesList > ()); }
    break;

  case 10: // BaseClassDef: "@BaseClass" CtorsOpt "=" Identifier ExtraNotesOpt "[" ClassFieldsOpt "]"
                                                                                                                  { yylhs.value.as < GoldSource::FGDEntityClass* > () = ctx->new_entity(FGDEntityClassType::BaseDef,yystack_[4].value.as < std::string > (), "<none>", yystack_[1].value.as < GoldSource::FGDPropertiesList > ()); }
    break;

  case 23: // ColorDefOpt: "color" "(" Number Number Number ")"
                                                                              { ctx->SetColor(yystack_[3].value.as < float > (),yystack_[2].value.as < float > (),yystack_[1].value.as < float > ()); }
    break;

  case 24: // BoundingBoxDefOpt: "size" "(" Number Number Number "," Number Number Number ")"
                                                                                                                         { ctx->SetBbox(yystack_[7].value.as < float > (),yystack_[6].value.as < float > (),yystack_[5].value.as < float > (),yystack_[3].value.as < float > (),yystack_[2].value.as < float > (),yystack_[1].value.as < float > ()); }
    break;

  case 25: // BoundingBoxDefOpt: "size" "(" Number Number Number ")"
                                                                            { ctx->SetBbox(yystack_[3].value.as < float > (),yystack_[2].value.as < float > (),yystack_[1].value.as < float > ()); }
    break;

  case 26: // StudioDefOpt: "studio" "(" StringLiteralOpt ")"
                                                                            { ctx->SetModel(yystack_[1].value.as < std::string > ()); }
    break;

  case 27: // SpriteDefOpt: "sprite" "(" StringLiteralOpt ")"
                                                                            { ctx->SetSprite(yystack_[1].value.as < std::string > ()); }
    break;

  case 28: // DecalDefOpt: "decal" "(" ")"
                                                         { ctx->SetDecalEntity(true); }
    break;

  case 29: // IconspriteDefOpt: "iconsprite" "(" StringLiteral ")"
                                                                                 { ctx->SetEditorSprite(yystack_[1].value.as < std::string > ()); }
    break;

  case 30: // ExtendedConstructor: Identifier "(" Number ")"
                                                                             { ctx->SetPropertyExtra(yystack_[3].value.as < std::string > (),yystack_[1].value.as < float > ()); }
    break;

  case 31: // BaseClassRefOpt: "base" "(" identifierListOpt ")"
                                                                                 { ctx->SetBaseClasses(yystack_[1].value.as < std::list<std::string> > ()); }
    break;

  case 32: // StringLiteralOpt: StringLiteral
                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();}
    break;

  case 33: // StringLiteralOpt: %empty
        { yylhs.value.as < std::string > () = ""; }
    break;

  case 34: // identifierListOpt: identifierListOpt "," Identifier
                                                      { yystack_[2].value.as < std::list<std::string> > ().push_back(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::list<std::string> > () = yystack_[2].value.as < std::list<std::string> > (); }
    break;

  case 35: // identifierListOpt: Identifier
            { yylhs.value.as < std::list<std::string> > ().push_back(yystack_[0].value.as < std::string > ()); }
    break;

  case 36: // identifierListOpt: %empty
        { (void)0; }
    break;

  case 37: // ClassFieldsOpt: ClassFieldsOpt ClassFieldDef
                                              { yystack_[1].value.as < GoldSource::FGDPropertiesList > ().push_back(yystack_[0].value.as < GoldSource::FGDPropertyDescriptor* > ()); yylhs.value.as < GoldSource::FGDPropertiesList > () = yystack_[1].value.as < GoldSource::FGDPropertiesList > ();}
    break;

  case 38: // ClassFieldsOpt: %empty
         { (void)0; }
    break;

  case 39: // ClassFieldDef: Identifier "(" TypeId ")" ":" StringLiteral DefaultValueOpt
                                                                                            { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor(yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > (), ""); }
    break;

  case 40: // ClassFieldDef: Identifier "(" "flags" ")" "=" "[" FlagsValues "]"
                                                                                                              { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDFlagsEnumProperty(yystack_[7].value.as < std::string > (),"<spawnflags>",yystack_[1].value.as < GoldSource::FGDFlagsList > (), 0, ""); }
    break;

  case 41: // ClassFieldDef: Identifier "(" "choices" ")" ":" StringLiteral ":" Number "=" "[" EnumValues "]"
                                                                                                                                                { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDFlagsEnumProperty(yystack_[11].value.as < std::string > (),yystack_[6].value.as < std::string > (),yystack_[1].value.as < GoldSource::FGDFlagsList > (), yystack_[4].value.as < float > (), ""); }
    break;

  case 42: // ClassFieldDef: "model" "(" ModelType ")" ":" StringLiteral DefaultValueOpt
                                                                                            { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor("model", yystack_[4].value.as < std::string > (), yystack_[1].value.as < std::string > (), "", ""); }
    break;

  case 43: // ClassFieldDef: "color" "(" "choices" ")" ":" StringLiteral ":" Number "=" "[" EnumValues "]"
                                                                                                                                           {yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor("color", yystack_[9].value.as < std::string > (), yystack_[6].value.as < std::string > (), "", ""); }
    break;

  case 44: // ClassFieldDef: "color" "(" TypeId ")" ":" StringLiteral DefaultValueOpt
                                                                                          { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor("color", yystack_[4].value.as < std::string > (), yystack_[1].value.as < std::string > (), "", ""); }
    break;

  case 45: // ClassFieldDef: "sound" "(" "choices" ")" ":" StringLiteral ":" Number "=" "[" EnumValues "]"
                                                                                                                                           { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor("sound", yystack_[9].value.as < std::string > (), yystack_[6].value.as < std::string > (), "", ""); }
    break;

  case 46: // ClassFieldDef: "sound" "(" TypeId ")" ":" StringLiteral DefaultValueOpt
                                                                                           { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor("sound", yystack_[4].value.as < std::string > (), yystack_[1].value.as < std::string > (), "", ""); }
    break;

  case 47: // ClassFieldDef: "size" "(" "choices" ")" ":" StringLiteral ":" Number "=" "[" EnumValues "]"
                                                                                                                                                     { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDFlagsEnumProperty("size", yystack_[6].value.as < std::string > (), yystack_[1].value.as < GoldSource::FGDFlagsList > (), yystack_[4].value.as < float > (), ""); }
    break;

  case 48: // ClassFieldDef: Identifier "(" "decal" ")"
                                                         { yylhs.value.as < GoldSource::FGDPropertyDescriptor* > () = new FGDPropertyDescriptor("decal", "decal", "decal", "", ""); }
    break;

  case 49: // ModelType: "studio"
           { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 50: // ModelType: "sprite"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 51: // FlagsValues: FlagsValues FlagValue
                                   { yystack_[1].value.as < GoldSource::FGDFlagsList > ().push_back(yystack_[0].value.as < GoldSource::FGDFlagsValue_t > ()); yylhs.value.as < GoldSource::FGDFlagsList > () = yystack_[1].value.as < GoldSource::FGDFlagsList > ();}
    break;

  case 52: // FlagsValues: %empty
        { (void)0; }
    break;

  case 53: // FlagValue: Number ":" StringLiteral ":" Number
                                                    { yylhs.value.as < GoldSource::FGDFlagsValue_t > () = FGDFlagsValue_t(yystack_[2].value.as < std::string > (), yystack_[4].value.as < float > (), yystack_[0].value.as < float > ()); }
    break;

  case 54: // EnumValues: EnumValues EnumValue
                                  { yystack_[1].value.as < GoldSource::FGDFlagsList > ().push_back(yystack_[0].value.as < GoldSource::FGDFlagsValue_t > ()); yylhs.value.as < GoldSource::FGDFlagsList > () = yystack_[1].value.as < GoldSource::FGDFlagsList > ();}
    break;

  case 55: // EnumValues: %empty
        { (void)0;}
    break;

  case 56: // EnumValue: Number ":" StringLiteral
                                       { yylhs.value.as < GoldSource::FGDFlagsValue_t > () = FGDFlagsValue_t(yystack_[0].value.as < std::string > (), yystack_[2].value.as < float > (), true); }
    break;

  case 57: // TypeId: "integer"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 58: // TypeId: "color255"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 59: // TypeId: "string"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 60: // TypeId: "sprite"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 61: // TypeId: "studio"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 62: // TypeId: "sky"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 63: // TypeId: "target_destination"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 64: // TypeId: "target_source"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 65: // TypeId: "sound"
  { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
    break;

  case 66: // DefaultValueOpt: ":" StringLiteral
                                     { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();}
    break;

  case 67: // DefaultValueOpt: ":" Number
               { yylhs.value.as < std::string > () = std::format("{0}", yystack_[0].value.as < float > ()); }
    break;

  case 68: // DefaultValueOpt: ":"
        { yylhs.value.as < std::string > () = ""; }
    break;

  case 69: // DefaultValueOpt: %empty
         { (void)0;}
    break;



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
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
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

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

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
    error (yyexc.location, yyexc.what ());
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


  const signed char HammerFGDParser::yypact_ninf_ = -74;

  const signed char HammerFGDParser::yytable_ninf_ = -1;

  const short
  HammerFGDParser::yypact_[] =
  {
     -74,     1,   104,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,    19,    29,    39,    11,    27,    37,    60,   101,   106,
     107,   108,   109,   -74,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,    76,   113,   117,   114,   116,   118,   119,   120,
     121,   122,   121,   125,   126,   128,   124,   -74,    12,   123,
     127,   129,   -74,   130,   -74,   131,   133,   112,   134,   125,
     -74,   136,   -74,   137,   -74,   138,   -74,   -74,   -74,   -74,
     125,   132,   -74,    16,   139,   -10,   135,   -74,   140,   -74,
     -74,   141,   -74,   142,   143,   144,   145,   -74,   -74,    -5,
     148,    45,   146,    58,    36,    71,     0,   -74,   149,   -74,
     -74,   -74,   -74,   147,   -74,   150,   151,   -74,   -74,   -74,
     -74,   152,   153,   154,   155,   -74,   -74,   156,   157,   158,
     -74,   160,   -74,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   -74,   161,   178,   179,   180,   181,   182,   183,
     184,   185,   -74,   188,   189,   190,   191,   189,   189,   192,
     189,    -8,   193,    73,   -74,   194,   195,   -74,   -74,   196,
     -74,   201,   -74,   -74,   197,   -74,   -74,   203,   204,   205,
     206,   186,   187,   198,   199,   209,   -74,   -74,   -74,   -74,
     207,    44,    88,    94,    95,   -74,   210,   -74,   -74,   -74,
     -74,   -74,   211,   -74
  };

  const signed char
  HammerFGDParser::yydefact_[] =
  {
       4,     0,     2,     1,    14,    14,    14,     3,     5,     6,
       7,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    13,    22,    17,    16,    19,    20,    15,
      21,    18,     0,     0,     0,     0,    36,     0,     0,     0,
      33,     0,    33,    12,     0,     0,     0,    35,     0,     0,
       0,     0,    32,     0,    28,     0,     0,     0,     0,    12,
      30,     0,    31,     0,    29,     0,    27,    26,    11,    38,
      12,     0,    34,     0,     0,     0,     0,    38,     0,    25,
      23,     0,    10,     0,     0,     0,     0,    37,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     8,     0,    57,
      58,    59,    60,     0,    61,     0,     0,    62,    64,    63,
      65,     0,     0,     0,     0,    50,    49,     0,     0,     0,
       9,     0,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,     0,    69,     0,     0,    69,    69,     0,
      69,     0,     0,    68,    39,     0,     0,    44,    42,     0,
      46,     0,    40,    51,     0,    66,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,    55,    55,    55,
       0,     0,     0,     0,     0,    53,     0,    41,    54,    47,
      43,    45,     0,    56
  };

  const signed char
  HammerFGDParser::yypgoto_[] =
  {
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -57,    28,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,    93,   -74,
     -73,   -74,   -74,   -74,   -74,   -61,   -74,     2,   -35
  };

  const unsigned char
  HammerFGDParser::yydefgoto_[] =
  {
       0,     1,     2,     7,     8,     9,    10,    57,    11,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    53,    48,
      75,    87,   117,   151,   163,   181,   188,   111,   154
  };

  const unsigned char
  HammerFGDParser::yytable_[] =
  {
      81,     3,    71,   161,    89,    81,    82,    83,   162,    84,
      81,    97,    83,    76,    84,    96,   120,    83,    85,    84,
      61,    34,    86,    85,    78,    14,    62,    86,    85,    15,
      79,    16,    86,    12,    13,    32,    17,    18,    19,    15,
      35,    16,    20,    21,    22,    33,    17,    18,    19,    15,
      36,    16,    20,    21,    22,   186,    17,    18,    19,   115,
     187,   116,    20,    21,    22,    99,   100,   101,   102,   103,
     104,   105,   106,    37,   107,   108,   109,   110,    99,   100,
     101,   102,   165,   104,   166,   113,    43,   107,   108,   109,
     110,    99,   100,   101,   102,   114,   104,   119,   118,   186,
     107,   108,   109,   110,   189,   186,   186,     4,     5,     6,
     190,   191,   157,   158,    38,   160,   182,   183,   184,    39,
      40,    41,    42,    44,    45,    46,    47,    69,    50,    49,
      52,    51,    56,    58,    63,    55,    54,    59,    60,     0,
      65,    64,    68,    70,    66,    67,    72,    77,    73,    74,
      88,    90,     0,    80,    91,    92,    93,    94,    95,    98,
     121,   122,     0,     0,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   112,   132,   133,   142,   134,   135,   136,
     137,   138,   139,   140,   141,     0,     0,   143,   144,   145,
     146,   147,   148,   149,   150,   152,   153,   155,   156,   159,
       0,   176,   177,   171,   164,   167,   168,   169,   170,   172,
     173,   174,     0,   178,   179,   175,   180,   192,   185,     0,
     193
  };

  const short
  HammerFGDParser::yycheck_[] =
  {
      10,     0,    59,    11,    77,    10,    16,    17,    16,    19,
      10,    16,    17,    70,    19,    88,    16,    17,    28,    19,
       8,    10,    32,    28,     8,     6,    14,    32,    28,    10,
      14,    12,    32,     5,     6,     6,    17,    18,    19,    10,
      13,    12,    23,    24,    25,     6,    17,    18,    19,    10,
      13,    12,    23,    24,    25,    11,    17,    18,    19,    23,
      16,    25,    23,    24,    25,    20,    21,    22,    23,    24,
      25,    26,    27,    13,    29,    30,    31,    32,    20,    21,
      22,    23,     9,    25,    11,    27,    10,    29,    30,    31,
      32,    20,    21,    22,    23,    93,    25,    95,    27,    11,
      29,    30,    31,    32,    16,    11,    11,     3,     4,     5,
      16,    16,   147,   148,    13,   150,   177,   178,   179,    13,
      13,    13,    13,    10,     7,    11,    10,    15,     9,    11,
       9,    11,     7,     7,    11,    42,    14,     9,    14,    -1,
      11,    14,     9,     9,    14,    14,    10,    15,    11,    11,
      15,    11,    -1,    14,    13,    13,    13,    13,    13,    11,
      11,    14,    -1,    -1,    14,    14,    14,    14,    14,    14,
      14,    14,    14,    27,    14,     6,    15,     7,     7,     7,
       7,     7,     7,     7,     7,    -1,    -1,     9,     9,     9,
       9,     9,     9,     9,     9,     7,     7,     7,     7,     7,
      -1,    15,    15,     6,    11,    11,    11,    11,     7,     6,
       6,     6,    -1,    15,    15,     9,     7,     7,    11,    -1,
       9
  };

  const signed char
  HammerFGDParser::yystos_[] =
  {
       0,    35,    36,     0,     3,     4,     5,    37,    38,    39,
      40,    42,    42,    42,     6,    10,    12,    17,    18,    19,
      23,    24,    25,    43,    44,    45,    46,    47,    48,    49,
      50,    51,     6,     6,    10,    13,    13,    13,    13,    13,
      13,    13,    13,    10,    10,     7,    11,    10,    53,    11,
       9,    11,     9,    52,    14,    52,     7,    41,     7,     9,
      14,     8,    14,    11,    14,    11,    14,    14,     9,    15,
       9,    41,    10,    11,    11,    54,    41,    15,     8,    14,
      14,    10,    16,    17,    19,    28,    32,    55,    15,    54,
      11,    13,    13,    13,    13,    13,    54,    16,    11,    20,
      21,    22,    23,    24,    25,    26,    27,    29,    30,    31,
      32,    61,    27,    27,    61,    23,    25,    56,    27,    61,
      16,    11,    14,    14,    14,    14,    14,    14,    14,    14,
      14,    14,    14,     6,     7,     7,     7,     7,     7,     7,
       7,     7,    15,     9,     9,     9,     9,     9,     9,     9,
       9,    57,     7,     7,    62,     7,     7,    62,    62,     7,
      62,    11,    16,    58,    11,     9,    11,    11,    11,    11,
       7,     6,     6,     6,     6,     9,    15,    15,    15,    15,
       7,    59,    59,    59,    59,    11,    11,    16,    60,    16,
      16,    16,     7,     9
  };

  const signed char
  HammerFGDParser::yyr1_[] =
  {
       0,    34,    35,    36,    36,    37,    37,    37,    38,    39,
      40,    41,    41,    42,    42,    43,    43,    43,    43,    43,
      43,    43,    43,    44,    45,    45,    46,    47,    48,    49,
      50,    51,    52,    52,    53,    53,    53,    54,    54,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    56,
      56,    57,    57,    58,    59,    59,    60,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    62,    62,    62,    62
  };

  const signed char
  HammerFGDParser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     1,     1,     1,    10,    10,
       8,     2,     0,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     6,    10,     6,     4,     4,     3,     4,
       4,     4,     1,     0,     3,     1,     0,     2,     0,     7,
       8,    12,     7,    12,     7,    12,     7,    12,     4,     1,
       1,     2,     0,     5,     2,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     0
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const HammerFGDParser::yytname_[] =
  {
  "EndOfFile", "error", "\"invalid token\"", "\"@SolidClass\"",
  "\"@BaseClass\"", "\"@PointClass\"", "\"=\"", "\":\"", "\",\"",
  "StringLiteral", "Identifier", "Number", "\"base\"", "\"(\"", "\")\"",
  "\"[\"", "\"]\"", "\"size\"", "\"iconsprite\"", "\"color\"",
  "\"integer\"", "\"color255\"", "\"string\"", "\"sprite\"", "\"decal\"",
  "\"studio\"", "\"flags\"", "\"choices\"", "\"model\"", "\"sky\"",
  "\"target_source\"", "\"target_destination\"", "\"sound\"", "Comment",
  "$accept", "FGDFile", "classesOpt", "EntityClassDef", "SolidClassDef",
  "PointClassDef", "BaseClassDef", "ExtraNotesOpt", "CtorsOpt", "Ctor",
  "ColorDefOpt", "BoundingBoxDefOpt", "StudioDefOpt", "SpriteDefOpt",
  "DecalDefOpt", "IconspriteDefOpt", "ExtendedConstructor",
  "BaseClassRefOpt", "StringLiteralOpt", "identifierListOpt",
  "ClassFieldsOpt", "ClassFieldDef", "ModelType", "FlagsValues",
  "FlagValue", "EnumValues", "EnumValue", "TypeId", "DefaultValueOpt", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  HammerFGDParser::yyrline_[] =
  {
       0,   251,   251,   255,   256,   259,   260,   261,   265,   268,
     271,   274,   275,   281,   282,   285,   286,   287,   288,   289,
     290,   291,   292,   295,   298,   299,   302,   305,   308,   311,
     314,   317,   319,   320,   325,   326,   327,   330,   331,   338,
     340,   342,   345,   348,   349,   352,   353,   357,   360,   364,
     365,   368,   369,   373,   376,   377,   380,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   397,   398,   399,   400
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




yy::HammerFGDParser::symbol_type yy::yylex(FGDParsingContext *  ctx)
{
    const char* anchor = ctx->cursor;
    ctx->loc.step();

    auto s = [&](auto func, auto&&... params) 
    { 
        ctx->loc.columns(ctx->cursor - anchor); 
        return func(params...,  ctx->loc); 
    };

    
#line 424 "<stdout>"
{
	char yych;
	yych = *ctx->cursor;
	switch (yych) {
	case 0x00:	goto yy3;
	case 0x08:
	case '\t':
	case '\v':
	case '\f':
	case ' ':	goto yy5;
	case '\n':	goto yy7;
	case '\r':	goto yy9;
	case '"':	goto yy10;
	case '(':	goto yy12;
	case ')':	goto yy14;
	case ',':	goto yy16;
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy18;
	case '/':	goto yy21;
	case ':':	goto yy22;
	case '=':	goto yy24;
	case '@':	goto yy26;
	case 'A':
	case 'E':
	case 'G':
	case 'H':
	case 'J':
	case 'K':
	case 'L':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'e':
	case 'g':
	case 'h':
	case 'j':
	case 'k':
	case 'l':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'B':
	case 'b':	goto yy30;
	case 'C':
	case 'c':	goto yy31;
	case 'D':
	case 'd':	goto yy32;
	case 'F':
	case 'f':	goto yy33;
	case 'I':
	case 'i':	goto yy34;
	case 'M':
	case 'm':	goto yy35;
	case 'S':
	case 's':	goto yy36;
	case 'T':
	case 't':	goto yy37;
	case '[':	goto yy38;
	case ']':	goto yy40;
	default:	goto yy2;
	}
yy2:
yy3:
	++ctx->cursor;
#line 429 "hammer_fgd.y"
	{ return s(yy::HammerFGDParser::make_EndOfFile); }
#line 518 "<stdout>"
yy5:
	++ctx->cursor;
#line 463 "hammer_fgd.y"
	{ ctx->loc.columns(); return yylex(ctx); }
#line 523 "<stdout>"
yy7:
	++ctx->cursor;
yy8:
#line 461 "hammer_fgd.y"
	{ ctx->loc.lines();   return yylex(ctx); }
#line 529 "<stdout>"
yy9:
	yych = *++ctx->cursor;
	switch (yych) {
	case '\n':	goto yy7;
	default:	goto yy8;
	}
yy10:
	yych = *++ctx->cursor;
	switch (yych) {
	case '"':	goto yy42;
	default:	goto yy10;
	}
yy12:
	++ctx->cursor;
#line 438 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_OpeningParenthesis); }
#line 546 "<stdout>"
yy14:
	++ctx->cursor;
#line 439 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_ClosingParenthesis); }
#line 551 "<stdout>"
yy16:
	++ctx->cursor;
#line 436 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Comma); }
#line 556 "<stdout>"
yy18:
	yych = *++ctx->cursor;
	switch (yych) {
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy18;
	default:	goto yy20;
	}
yy20:
#line 437 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Number, std::stol(std::string(anchor,ctx->cursor))); }
#line 576 "<stdout>"
yy21:
	yych = *++ctx->cursor;
	switch (yych) {
	case '/':	goto yy44;
	default:	goto yy2;
	}
yy22:
	++ctx->cursor;
#line 435 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Colon); }
#line 587 "<stdout>"
yy24:
	++ctx->cursor;
#line 434 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_EqualsSign); }
#line 592 "<stdout>"
yy26:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'B':
	case 'b':	goto yy47;
	case 'P':
	case 'p':	goto yy48;
	case 'S':
	case 's':	goto yy49;
	default:	goto yy2;
	}
yy27:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy29;
	}
yy29:
#line 460 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Identifier, std::string(anchor, ctx->cursor)); }
#line 675 "<stdout>"
yy30:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy52;
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
	case 'z':	goto yy27;
	default:	goto yy51;
	}
yy31:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
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
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
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
	case 'z':	goto yy27;
	case 'H':
	case 'h':	goto yy53;
	case 'O':
	case 'o':	goto yy54;
	default:	goto yy51;
	}
yy32:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy55;
	default:	goto yy51;
	}
yy33:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'L':
	case 'l':	goto yy56;
	default:	goto yy51;
	}
yy34:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
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
	case 'z':	goto yy27;
	case 'C':
	case 'c':	goto yy57;
	case 'N':
	case 'n':	goto yy58;
	default:	goto yy51;
	}
yy35:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy59;
	default:	goto yy51;
	}
yy36:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'J':
	case 'L':
	case 'M':
	case 'N':
	case 'Q':
	case 'R':
	case 'S':
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
	case 'j':
	case 'l':
	case 'm':
	case 'n':
	case 'q':
	case 'r':
	case 's':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy60;
	case 'K':
	case 'k':	goto yy61;
	case 'O':
	case 'o':	goto yy62;
	case 'P':
	case 'p':	goto yy63;
	case 'T':
	case 't':	goto yy64;
	default:	goto yy51;
	}
yy37:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy65;
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
	case 'z':	goto yy27;
	default:	goto yy51;
	}
yy38:
	++ctx->cursor;
#line 440 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_OpeningBracket); }
#line 1144 "<stdout>"
yy40:
	++ctx->cursor;
#line 441 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_ClosingBracket); }
#line 1149 "<stdout>"
yy42:
	++ctx->cursor;
#line 459 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_StringLiteral, std::string(anchor+1, ctx->cursor-1)); }
#line 1154 "<stdout>"
yy44:
	yych = *++ctx->cursor;
	switch (yych) {
	case '\n':
	case '\r':	goto yy46;
	default:	goto yy44;
	}
yy46:
#line 462 "hammer_fgd.y"
	{                     return yylex(ctx); }
#line 1165 "<stdout>"
yy47:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy66;
	default:	goto yy2;
	}
yy48:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'O':
	case 'o':	goto yy67;
	default:	goto yy2;
	}
yy49:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'O':
	case 'o':	goto yy68;
	default:	goto yy2;
	}
yy50:
	yych = *++ctx->cursor;
yy51:
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
	case '9':	goto yy50;
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
	case 'z':	goto yy69;
	default:	goto yy29;
	}
yy52:
	yych = *++ctx->cursor;
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
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'S':
	case 's':	goto yy71;
	default:	goto yy51;
	}
yy53:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy72;
	default:	goto yy51;
	}
yy54:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'L':
	case 'l':	goto yy73;
	default:	goto yy51;
	}
yy55:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
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
	case 'z':	goto yy27;
	case 'C':
	case 'c':	goto yy74;
	default:	goto yy51;
	}
yy56:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy75;
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
	case 'z':	goto yy27;
	default:	goto yy51;
	}
yy57:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy76;
	default:	goto yy51;
	}
yy58:
	yych = *++ctx->cursor;
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
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'T':
	case 't':	goto yy77;
	default:	goto yy51;
	}
yy59:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
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
	case 'z':	goto yy27;
	case 'D':
	case 'd':	goto yy78;
	default:	goto yy51;
	}
yy60:
	yych = *++ctx->cursor;
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
	case 'y':	goto yy27;
	case 'Z':
	case 'z':	goto yy79;
	default:	goto yy51;
	}
yy61:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'Y':
	case 'y':	goto yy80;
	default:	goto yy51;
	}
yy62:
	yych = *++ctx->cursor;
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
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'U':
	case 'u':	goto yy82;
	default:	goto yy51;
	}
yy63:
	yych = *++ctx->cursor;
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
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy83;
	default:	goto yy51;
	}
yy64:
	yych = *++ctx->cursor;
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
	case 'S':
	case 'T':
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
	case 's':
	case 't':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy84;
	case 'U':
	case 'u':	goto yy85;
	default:	goto yy51;
	}
yy65:
	yych = *++ctx->cursor;
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
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy86;
	default:	goto yy51;
	}
yy66:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy87;
	default:	goto yy2;
	}
yy67:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'I':
	case 'i':	goto yy88;
	default:	goto yy2;
	}
yy68:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'L':
	case 'l':	goto yy89;
	default:	goto yy2;
	}
yy69:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy69;
	default:	goto yy29;
	}
yy71:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy90;
	default:	goto yy51;
	}
yy72:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy92;
	default:	goto yy51;
	}
yy73:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy93;
	default:	goto yy51;
	}
yy74:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy94;
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
	case 'z':	goto yy27;
	default:	goto yy51;
	}
yy75:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
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
	case 'z':	goto yy27;
	case 'G':
	case 'g':	goto yy95;
	default:	goto yy51;
	}
yy76:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'N':
	case 'n':	goto yy96;
	default:	goto yy51;
	}
yy77:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy97;
	default:	goto yy51;
	}
yy78:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy98;
	default:	goto yy51;
	}
yy79:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy99;
	default:	goto yy51;
	}
yy80:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy81;
	}
yy81:
#line 455 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Sky, std::string(anchor, ctx->cursor)) ; }
#line 2740 "<stdout>"
yy82:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'N':
	case 'n':	goto yy101;
	default:	goto yy51;
	}
yy83:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy102;
	default:	goto yy51;
	}
yy84:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy103;
	default:	goto yy51;
	}
yy85:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
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
	case 'z':	goto yy27;
	case 'D':
	case 'd':	goto yy104;
	default:	goto yy51;
	}
yy86:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
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
	case 'z':	goto yy27;
	case 'G':
	case 'g':	goto yy105;
	default:	goto yy51;
	}
yy87:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'E':
	case 'e':	goto yy106;
	default:	goto yy2;
	}
yy88:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'N':
	case 'n':	goto yy107;
	default:	goto yy2;
	}
yy89:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'I':
	case 'i':	goto yy108;
	default:	goto yy2;
	}
yy90:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy91;
	}
yy91:
#line 442 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_BaseDef); }
#line 3123 "<stdout>"
yy92:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
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
	case 'z':	goto yy27;
	case 'C':
	case 'c':	goto yy109;
	default:	goto yy51;
	}
yy93:
	yych = *++ctx->cursor;
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
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy110;
	default:	goto yy51;
	}
yy94:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'L':
	case 'l':	goto yy112;
	default:	goto yy51;
	}
yy95:
	yych = *++ctx->cursor;
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
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'S':
	case 's':	goto yy114;
	default:	goto yy51;
	}
yy96:
	yych = *++ctx->cursor;
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
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'S':
	case 's':	goto yy116;
	default:	goto yy51;
	}
yy97:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
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
	case 'z':	goto yy27;
	case 'G':
	case 'g':	goto yy117;
	default:	goto yy51;
	}
yy98:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'L':
	case 'l':	goto yy118;
	default:	goto yy51;
	}
yy99:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy100;
	}
yy100:
#line 443 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_SizeBoundingBox); }
#line 3601 "<stdout>"
yy101:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
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
	case 'z':	goto yy27;
	case 'D':
	case 'd':	goto yy120;
	default:	goto yy51;
	}
yy102:
	yych = *++ctx->cursor;
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
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'T':
	case 't':	goto yy122;
	default:	goto yy51;
	}
yy103:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'N':
	case 'n':	goto yy123;
	default:	goto yy51;
	}
yy104:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy124;
	default:	goto yy51;
	}
yy105:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy125;
	default:	goto yy51;
	}
yy106:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'C':
	case 'c':	goto yy126;
	default:	goto yy2;
	}
yy107:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'T':
	case 't':	goto yy127;
	default:	goto yy2;
	}
yy108:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'D':
	case 'd':	goto yy128;
	default:	goto yy2;
	}
yy109:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy129;
	default:	goto yy51;
	}
yy110:
	yych = *++ctx->cursor;
	switch (yych) {
	case '0':
	case '1':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy50;
	case '2':	goto yy130;
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
	case 'z':	goto yy27;
	default:	goto yy111;
	}
yy111:
#line 445 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Color); }
#line 4042 "<stdout>"
yy112:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy113;
	}
yy113:
#line 449 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Decal) ; }
#line 4114 "<stdout>"
yy114:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy115;
	}
yy115:
#line 456 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Flags, std::string(anchor, ctx->cursor)) ; }
#line 4186 "<stdout>"
yy116:
	yych = *++ctx->cursor;
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
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'P':
	case 'p':	goto yy131;
	default:	goto yy51;
	}
yy117:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy132;
	default:	goto yy51;
	}
yy118:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy119;
	}
yy119:
#line 454 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Model, std::string(anchor, ctx->cursor)) ; }
#line 4374 "<stdout>"
yy120:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy121;
	}
yy121:
#line 452 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Sound, std::string(anchor, ctx->cursor)) ; }
#line 4446 "<stdout>"
yy122:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy133;
	default:	goto yy51;
	}
yy123:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
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
	case 'z':	goto yy27;
	case 'G':
	case 'g':	goto yy135;
	default:	goto yy51;
	}
yy124:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy137;
	default:	goto yy51;
	}
yy125:
	yych = *++ctx->cursor;
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
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'T':
	case 't':	goto yy139;
	default:	goto yy51;
	}
yy126:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'L':
	case 'l':	goto yy140;
	default:	goto yy2;
	}
yy127:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'C':
	case 'c':	goto yy141;
	default:	goto yy2;
	}
yy128:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'C':
	case 'c':	goto yy142;
	default:	goto yy2;
	}
yy129:
	yych = *++ctx->cursor;
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
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'S':
	case 's':	goto yy143;
	default:	goto yy51;
	}
yy130:
	yych = *++ctx->cursor;
	switch (yych) {
	case '5':	goto yy145;
	default:	goto yy51;
	}
yy131:
	yych = *++ctx->cursor;
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
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy146;
	default:	goto yy51;
	}
yy132:
	yych = *++ctx->cursor;
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
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy147;
	default:	goto yy51;
	}
yy133:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy134;
	}
yy134:
#line 448 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Sprite, std::string(anchor, ctx->cursor)) ; }
#line 4951 "<stdout>"
yy135:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy136;
	}
yy136:
#line 458 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_String, std::string(anchor, ctx->cursor)) ; }
#line 5023 "<stdout>"
yy137:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy138;
	}
yy138:
#line 453 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Studio, std::string(anchor, ctx->cursor)) ; }
#line 5095 "<stdout>"
yy139:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case '_':	goto yy149;
	default:	goto yy51;
	}
yy140:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy150;
	default:	goto yy2;
	}
yy141:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'L':
	case 'l':	goto yy151;
	default:	goto yy2;
	}
yy142:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'L':
	case 'l':	goto yy152;
	default:	goto yy2;
	}
yy143:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy144;
	}
yy144:
#line 457 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Choices, std::string(anchor, ctx->cursor)) ; }
#line 5246 "<stdout>"
yy145:
	yych = *++ctx->cursor;
	switch (yych) {
	case '5':	goto yy153;
	default:	goto yy51;
	}
yy146:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy155;
	default:	goto yy51;
	}
yy147:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy148;
	}
yy148:
#line 446 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_IntegerType, std::string(anchor, ctx->cursor)) ; }
#line 5382 "<stdout>"
yy149:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
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
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'D':
	case 'd':	goto yy156;
	case 'S':
	case 's':	goto yy157;
	default:	goto yy51;
	}
yy150:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy158;
	default:	goto yy2;
	}
yy151:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy159;
	default:	goto yy2;
	}
yy152:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy160;
	default:	goto yy2;
	}
yy153:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy69;
	default:	goto yy154;
	}
yy154:
#line 447 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Color255, std::string(anchor, ctx->cursor)) ; }
#line 5533 "<stdout>"
yy155:
	yych = *++ctx->cursor;
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
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'T':
	case 't':	goto yy161;
	default:	goto yy51;
	}
yy156:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy162;
	default:	goto yy51;
	}
yy157:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy163;
	default:	goto yy51;
	}
yy158:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy164;
	default:	goto yy2;
	}
yy159:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy166;
	default:	goto yy2;
	}
yy160:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy167;
	default:	goto yy2;
	}
yy161:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy168;
	default:	goto yy51;
	}
yy162:
	yych = *++ctx->cursor;
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
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'S':
	case 's':	goto yy170;
	default:	goto yy51;
	}
yy163:
	yych = *++ctx->cursor;
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
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'U':
	case 'u':	goto yy171;
	default:	goto yy51;
	}
yy164:
	++ctx->cursor;
#line 432 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_BaseClass); }
#line 5907 "<stdout>"
yy166:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy172;
	default:	goto yy2;
	}
yy167:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'S':
	case 's':	goto yy174;
	default:	goto yy2;
	}
yy168:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy169;
	}
yy169:
#line 444 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_Iconsprite); }
#line 5993 "<stdout>"
yy170:
	yych = *++ctx->cursor;
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
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'T':
	case 't':	goto yy176;
	default:	goto yy51;
	}
yy171:
	yych = *++ctx->cursor;
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
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'R':
	case 'r':	goto yy177;
	default:	goto yy51;
	}
yy172:
	++ctx->cursor;
#line 433 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_PointClass); }
#line 6114 "<stdout>"
yy174:
	++ctx->cursor;
#line 431 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_SolidClass); }
#line 6119 "<stdout>"
yy176:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy178;
	default:	goto yy51;
	}
yy177:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
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
	case 'z':	goto yy27;
	case 'C':
	case 'c':	goto yy179;
	default:	goto yy51;
	}
yy178:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'N':
	case 'n':	goto yy180;
	default:	goto yy51;
	}
yy179:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
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
	case 'z':	goto yy27;
	case 'E':
	case 'e':	goto yy181;
	default:	goto yy51;
	}
yy180:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'a':	goto yy183;
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
	case 'z':	goto yy27;
	default:	goto yy51;
	}
yy181:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy182;
	}
yy182:
#line 450 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_TargetSource, std::string(anchor, ctx->cursor)) ; }
#line 6481 "<stdout>"
yy183:
	yych = *++ctx->cursor;
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
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	case 'T':
	case 't':	goto yy184;
	default:	goto yy51;
	}
yy184:
	yych = *++ctx->cursor;
	switch (yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
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
	case 'z':	goto yy27;
	case 'I':
	case 'i':	goto yy185;
	default:	goto yy51;
	}
yy185:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'O':
	case 'o':	goto yy186;
	default:	goto yy51;
	}
yy186:
	yych = *++ctx->cursor;
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
	case 'z':	goto yy27;
	case 'N':
	case 'n':	goto yy187;
	default:	goto yy51;
	}
yy187:
	yych = *++ctx->cursor;
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
	case '9':	goto yy50;
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
	case 'z':	goto yy27;
	default:	goto yy188;
	}
yy188:
#line 451 "hammer_fgd.y"
	{                     return s(yy::HammerFGDParser::make_TargetDestination, std::string(anchor, ctx->cursor)) ; }
#line 6785 "<stdout>"
}
#line 464 "hammer_fgd.y"

		
}

void yy::HammerFGDParser::error(const location_type& l,const std::string& m)
{
    Con_Printf("%s\n",m.c_str());
}







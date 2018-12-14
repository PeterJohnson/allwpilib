/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef WPIUTIL_INJA_PARSER_H_
#define WPIUTIL_INJA_PARSER_H_

#include "inja_internal.h"
#include "wpi/SmallString.h"
#include "wpi/StringRef.h"
#include "wpi/Twine.h"
#include "wpi/inja.h"

namespace wpi {
namespace inja {

class ParserStatic {
 public:
  ParserStatic(const ParserStatic&) = delete;
  ParserStatic& operator=(const ParserStatic&) = delete;

  static const ParserStatic& GetInstance();

  FunctionStorage functions;

 private:
  ParserStatic();
};

struct Token {
  enum Kind {
    kText,
    kExpressionOpen,      // {{
    kExpressionClose,     // }}
    kLineStatementOpen,   // ##
    kLineStatementClose,  // \n
    kStatementOpen,       // {%
    kStatementClose,      // %}
    kCommentOpen,         // {#
    kCommentClose,        // #}
    kId,                  // this, this.foo
    kNumber,              // 1, 2, -1, 5.2, -5.3
    kString,              // "this"
    kComma,               // ,
    kColon,               // :
    kLeftParen,           // (
    kRightParen,          // )
    kLeftBracket,         // [
    kRightBracket,        // ]
    kLeftBrace,           // {
    kRightBrace,          // }
    kEqual,               // ==
    kGreaterThan,         // >
    kLessThan,            // <
    kLessEqual,           // <=
    kGreaterEqual,        // >=
    kNotEqual,            // !=
    kUnknown,
    kEof
  };
  Kind kind = kUnknown;
  StringRef text;

  Token() = default;
  constexpr Token(Kind kind, StringRef text) : kind(kind), text(text) {}
  StringRef Describe() const;
};

struct LexerConfig {
  SmallString<4> statementOpen{"{%"};
  SmallString<4> statementClose{"%}"};
  SmallString<4> lineStatement{"##"};
  SmallString<4> expressionOpen{"{{"};
  SmallString<4> expressionClose{"}}"};
  SmallString<4> commentOpen{"{#"};
  SmallString<4> commentClose{"#}"};
  SmallString<4> openChars{"#{"};

  void UpdateOpenChars();
};

class Lexer {
 public:
  explicit Lexer(const LexerConfig& config) : m_config(config) {}

  void Start(StringRef in);
  Token Scan();

  const LexerConfig& GetConfig() const { return m_config; }

 private:
  Token ScanBody(StringRef close, Token::Kind closeKind);
  Token ScanId();
  Token ScanNumber();
  Token ScanString();
  Token MakeToken(Token::Kind kind) const {
    return Token(kind, m_in.slice(m_tokStart, m_pos));
  }

  const LexerConfig& m_config;
  StringRef m_in;
  size_t m_tokStart;
  size_t m_pos;
  enum class State {
    Text,
    ExpressionStart,
    ExpressionBody,
    LineStart,
    LineBody,
    StatementStart,
    StatementBody,
    CommentStart,
    CommentBody
  };
  State m_state;
};

struct ParserConfig {
  ElementNotation notation = ElementNotation::Pointer;
  std::function<std::string(StringRef filename)> loadFile;
};

class Parser {
 public:
  Parser(const ParserConfig& parserConfig, const LexerConfig& lexerConfig,
         TemplateStorage& includedTemplates);

  bool ParseExpression(Template& tmpl);
  bool ParseExpressionAnd(Template& tmpl);
  bool ParseExpressionNot(Template& tmpl);
  bool ParseExpressionComparison(Template& tmpl);
  bool ParseExpressionDatum(Template& tmpl);
  bool ParseStatement(Template& tmpl, StringRef path);
  void AppendFunction(Template& tmpl, Bytecode::Op op, unsigned int numArgs);
  void AppendCallback(Template& tmpl, StringRef name, unsigned int numArgs);
  void ParseInto(Template& tmpl, StringRef path);

  Template Parse(StringRef input, StringRef path = "./");
  Template ParseTemplate(StringRef filename);

 private:
  const ParserConfig& m_config;
  Lexer m_lexer;
  Token m_tok;
  Token m_peekTok;
  bool m_havePeekTok = false;
  TemplateStorage& m_includedTemplates;
  const ParserStatic& m_static;
  struct IfData {
    unsigned int prevCondJump;
    SmallVector<unsigned int, 4> uncondJumps;

    explicit IfData(unsigned int condJump) : prevCondJump(condJump) {}
  };
  SmallVector<IfData, 4> m_ifStack;
  SmallVector<unsigned int, 4> m_loopStack;

  void GetNextToken() {
    if (m_havePeekTok) {
      m_tok = m_peekTok;
      m_havePeekTok = false;
    } else {
      m_tok = m_lexer.Scan();
    }
  }

  void GetPeekToken() {
    if (!m_havePeekTok) {
      m_peekTok = m_lexer.Scan();
      m_havePeekTok = true;
    }
  }
};

}  // namespace inja
}  // namespace wpi

#endif  // WPIUTIL_INJA_PARSER_H_

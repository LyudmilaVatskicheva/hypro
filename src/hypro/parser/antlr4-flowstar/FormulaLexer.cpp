
// Generated from Formula.g4 by ANTLR 4.7


#include "FormulaLexer.h"


using namespace antlr4;


FormulaLexer::FormulaLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

FormulaLexer::~FormulaLexer() {
  delete _interpreter;
}

std::string FormulaLexer::getGrammarFileName() const {
  return "Formula.g4";
}

const std::vector<std::string>& FormulaLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& FormulaLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& FormulaLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& FormulaLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& FormulaLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> FormulaLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& FormulaLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> FormulaLexer::_decisionToDFA;
atn::PredictionContextCache FormulaLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN FormulaLexer::_atn;
std::vector<uint16_t> FormulaLexer::_serializedATN;

std::vector<std::string> FormulaLexer::_ruleNames = {
  u8"T__0", u8"T__1", u8"T__2", u8"IN", u8"COMMENT", u8"EQUALS", u8"BOOLRELATION", 
  u8"PLUS", u8"TIMES", u8"UPPERCASE", u8"LOWERCASE", u8"DIGIT", u8"SPECIALCHAR", 
  u8"NUMBER", u8"VARIABLE", u8"WS"
};

std::vector<std::string> FormulaLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> FormulaLexer::_modeNames = {
  u8"DEFAULT_MODE"
};

std::vector<std::string> FormulaLexer::_literalNames = {
  "", u8"'['", u8"','", u8"']'", u8"'in'", "", u8"'='", "", u8"'+'", u8"'*'"
};

std::vector<std::string> FormulaLexer::_symbolicNames = {
  "", "", "", "", u8"IN", u8"COMMENT", u8"EQUALS", u8"BOOLRELATION", u8"PLUS", 
  u8"TIMES", u8"NUMBER", u8"VARIABLE", u8"WS"
};

dfa::Vocabulary FormulaLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> FormulaLexer::_tokenNames;

FormulaLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x2, 0xe, 0x6a, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 
    0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
    0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 
    0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 
    0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
    0x5, 0x8, 0x39, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 
    0xe, 0x3, 0xf, 0x5, 0xf, 0x48, 0xa, 0xf, 0x3, 0xf, 0x6, 0xf, 0x4b, 0xa, 
    0xf, 0xd, 0xf, 0xe, 0xf, 0x4c, 0x3, 0xf, 0x3, 0xf, 0x6, 0xf, 0x51, 0xa, 
    0xf, 0xd, 0xf, 0xe, 0xf, 0x52, 0x5, 0xf, 0x55, 0xa, 0xf, 0x3, 0x10, 
    0x3, 0x10, 0x5, 0x10, 0x59, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x7, 0x10, 0x5f, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x62, 0xb, 
    0x10, 0x3, 0x11, 0x6, 0x11, 0x65, 0xa, 0x11, 0xd, 0x11, 0xe, 0x11, 0x66, 
    0x3, 0x11, 0x3, 0x11, 0x2, 0x2, 0x12, 0x3, 0x3, 0x5, 0x4, 0x7, 0x5, 
    0x9, 0x6, 0xb, 0x7, 0xd, 0x8, 0xf, 0x9, 0x11, 0xa, 0x13, 0xb, 0x15, 
    0x2, 0x17, 0x2, 0x19, 0x2, 0x1b, 0x2, 0x1d, 0xc, 0x1f, 0xd, 0x21, 0xe, 
    0x3, 0x2, 0x9, 0x4, 0x2, 0xc, 0xc, 0xf, 0xf, 0x4, 0x2, 0x3e, 0x3e, 0x40, 
    0x40, 0x3, 0x2, 0x43, 0x5c, 0x3, 0x2, 0x63, 0x7c, 0x3, 0x2, 0x32, 0x3b, 
    0x4, 0x2, 0x29, 0x29, 0x61, 0x61, 0x5, 0x2, 0xb, 0xc, 0xf, 0xf, 0x22, 
    0x22, 0x2, 0x71, 0x2, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 0x3, 0x2, 0x2, 0x2, 0x2, 
    0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 0x3, 0x2, 0x2, 0x2, 0x2, 0xf, 0x3, 
    0x2, 0x2, 0x2, 0x2, 0x11, 0x3, 0x2, 0x2, 0x2, 0x2, 0x13, 0x3, 0x2, 0x2, 
    0x2, 0x2, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x2, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x2, 
    0x21, 0x3, 0x2, 0x2, 0x2, 0x3, 0x23, 0x3, 0x2, 0x2, 0x2, 0x5, 0x25, 
    0x3, 0x2, 0x2, 0x2, 0x7, 0x27, 0x3, 0x2, 0x2, 0x2, 0x9, 0x29, 0x3, 0x2, 
    0x2, 0x2, 0xb, 0x2c, 0x3, 0x2, 0x2, 0x2, 0xd, 0x31, 0x3, 0x2, 0x2, 0x2, 
    0xf, 0x38, 0x3, 0x2, 0x2, 0x2, 0x11, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x13, 
    0x3c, 0x3, 0x2, 0x2, 0x2, 0x15, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x17, 0x40, 
    0x3, 0x2, 0x2, 0x2, 0x19, 0x42, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x44, 0x3, 
    0x2, 0x2, 0x2, 0x1d, 0x47, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x58, 0x3, 0x2, 
    0x2, 0x2, 0x21, 0x64, 0x3, 0x2, 0x2, 0x2, 0x23, 0x24, 0x7, 0x5d, 0x2, 
    0x2, 0x24, 0x4, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x7, 0x2e, 0x2, 0x2, 
    0x26, 0x6, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x7, 0x5f, 0x2, 0x2, 0x28, 
    0x8, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x7, 0x6b, 0x2, 0x2, 0x2a, 0x2b, 
    0x7, 0x70, 0x2, 0x2, 0x2b, 0xa, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x2d, 0x7, 
    0x25, 0x2, 0x2, 0x2d, 0x2e, 0xa, 0x2, 0x2, 0x2, 0x2e, 0x2f, 0x3, 0x2, 
    0x2, 0x2, 0x2f, 0x30, 0x8, 0x6, 0x2, 0x2, 0x30, 0xc, 0x3, 0x2, 0x2, 
    0x2, 0x31, 0x32, 0x7, 0x3f, 0x2, 0x2, 0x32, 0xe, 0x3, 0x2, 0x2, 0x2, 
    0x33, 0x34, 0x7, 0x3e, 0x2, 0x2, 0x34, 0x39, 0x7, 0x3f, 0x2, 0x2, 0x35, 
    0x36, 0x7, 0x40, 0x2, 0x2, 0x36, 0x39, 0x7, 0x3f, 0x2, 0x2, 0x37, 0x39, 
    0x9, 0x3, 0x2, 0x2, 0x38, 0x33, 0x3, 0x2, 0x2, 0x2, 0x38, 0x35, 0x3, 
    0x2, 0x2, 0x2, 0x38, 0x37, 0x3, 0x2, 0x2, 0x2, 0x39, 0x10, 0x3, 0x2, 
    0x2, 0x2, 0x3a, 0x3b, 0x7, 0x2d, 0x2, 0x2, 0x3b, 0x12, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x3d, 0x7, 0x2c, 0x2, 0x2, 0x3d, 0x14, 0x3, 0x2, 0x2, 0x2, 
    0x3e, 0x3f, 0x9, 0x4, 0x2, 0x2, 0x3f, 0x16, 0x3, 0x2, 0x2, 0x2, 0x40, 
    0x41, 0x9, 0x5, 0x2, 0x2, 0x41, 0x18, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 
    0x9, 0x6, 0x2, 0x2, 0x43, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x44, 0x45, 0x9, 
    0x7, 0x2, 0x2, 0x45, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x46, 0x48, 0x7, 0x2f, 
    0x2, 0x2, 0x47, 0x46, 0x3, 0x2, 0x2, 0x2, 0x47, 0x48, 0x3, 0x2, 0x2, 
    0x2, 0x48, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4b, 0x5, 0x19, 0xd, 0x2, 
    0x4a, 0x49, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4c, 
    0x4a, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x54, 
    0x3, 0x2, 0x2, 0x2, 0x4e, 0x50, 0x7, 0x30, 0x2, 0x2, 0x4f, 0x51, 0x5, 
    0x19, 0xd, 0x2, 0x50, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x51, 0x52, 0x3, 0x2, 
    0x2, 0x2, 0x52, 0x50, 0x3, 0x2, 0x2, 0x2, 0x52, 0x53, 0x3, 0x2, 0x2, 
    0x2, 0x53, 0x55, 0x3, 0x2, 0x2, 0x2, 0x54, 0x4e, 0x3, 0x2, 0x2, 0x2, 
    0x54, 0x55, 0x3, 0x2, 0x2, 0x2, 0x55, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x56, 
    0x59, 0x5, 0x15, 0xb, 0x2, 0x57, 0x59, 0x5, 0x17, 0xc, 0x2, 0x58, 0x56, 
    0x3, 0x2, 0x2, 0x2, 0x58, 0x57, 0x3, 0x2, 0x2, 0x2, 0x59, 0x60, 0x3, 
    0x2, 0x2, 0x2, 0x5a, 0x5f, 0x5, 0x15, 0xb, 0x2, 0x5b, 0x5f, 0x5, 0x17, 
    0xc, 0x2, 0x5c, 0x5f, 0x5, 0x19, 0xd, 0x2, 0x5d, 0x5f, 0x5, 0x1b, 0xe, 
    0x2, 0x5e, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5b, 0x3, 0x2, 0x2, 0x2, 
    0x5e, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x5f, 
    0x62, 0x3, 0x2, 0x2, 0x2, 0x60, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x60, 0x61, 
    0x3, 0x2, 0x2, 0x2, 0x61, 0x20, 0x3, 0x2, 0x2, 0x2, 0x62, 0x60, 0x3, 
    0x2, 0x2, 0x2, 0x63, 0x65, 0x9, 0x8, 0x2, 0x2, 0x64, 0x63, 0x3, 0x2, 
    0x2, 0x2, 0x65, 0x66, 0x3, 0x2, 0x2, 0x2, 0x66, 0x64, 0x3, 0x2, 0x2, 
    0x2, 0x66, 0x67, 0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 0x3, 0x2, 0x2, 0x2, 
    0x68, 0x69, 0x8, 0x11, 0x2, 0x2, 0x69, 0x22, 0x3, 0x2, 0x2, 0x2, 0xc, 
    0x2, 0x38, 0x47, 0x4c, 0x52, 0x54, 0x58, 0x5e, 0x60, 0x66, 0x3, 0x8, 
    0x2, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

FormulaLexer::Initializer FormulaLexer::_init;
